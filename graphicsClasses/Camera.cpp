/*
 * File: Camera.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera():_fovy(glm::radians(70.f)),
    _ratio(static_cast <float>(Utility::windowResolutionX) /
           static_cast <float>(Utility::windowResolutionY)),
    _pos(glm::vec3(1.f, 0.f, 0.f)),
    _center(glm::vec3(0.f, 0.f, 0.f)),
    _up(glm::vec3(0.f, 1.f, 0.f)),
    _willComeBack(false),
    _isComingBack(false),
    _cameraAboveWay(0.f),
    _timePointComeBack() {
}

void Camera::follow (const BallState& ballState) noexcept{

    const JBTypes::vec3f& position = ballState.get3DPosition();
    const auto sideBall = ballState.currentSideAsVector();
    const auto lookingDirection = ballState.lookTowardsAsVector();
    const glm::vec3 vecLookingDirection = Utility::convertToOpenGLFormat(lookingDirection);

    const auto stateBall = ballState.state();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.3f;
    constexpr float distAboveBall = 1.2f;

    glm::mat4 matRotationCam(1.f);

    const glm::mat4 matPosBall =
        glm::translate(Utility::convertToOpenGLFormat(position));

    const glm::vec3 toSkyVec3 = Utility::convertToOpenGLFormat(sideBall);
    const glm::vec4 toSky(toSkyVec3, 1.f);


    const glm::vec4 initPosCam(distAboveBall * toSkyVec3 - distBehindBall * vecLookingDirection, 1.f);
    const glm::vec4 initCenterCam
        (-glm::vec3(vecLookingDirection.x * -distDirPoint,
                    vecLookingDirection.y * -distDirPoint,
                    vecLookingDirection.z * -distDirPoint), 1.f);

    constexpr float durationMoveAbove = 0.2f;
    constexpr float durationMoveComingBack = 0.2f;

    const float timeSinceAction = ballState.getTimeSecondsSinceAction();
    const JBTypes::timePointMs now = JBTypesMethods::getTimePointMSNow();

    if (stateBall == Ball::State::Staying) {
        if (_willComeBack) {
            _timePointComeBack = now;
            _isComingBack = true;
            _willComeBack = false;
        }
    }
    if (stateBall == Ball::State::Moving) {
        const JBTypes::vec3f vecNextLook = ballState.nextLook();

        const glm::vec3 axisNewLook = Utility::convertToOpenGLFormat(vecNextLook);

        const glm::vec3 axisRotation = glm::cross(vecLookingDirection, axisNewLook);
        const glm::vec3 eulerAngles = (timeSinceAction *
                                       (static_cast <float>(M_PI) / 2) / Ball::timeToGetNextBlock) *
                                      axisRotation;

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);
    } else if (
        stateBall == Ball::State::TurningLeft ||
        stateBall == Ball::State::TurningRight
        ) {
        const glm::vec3& axisRotation = toSkyVec3;

        glm::vec3 eulerAngles =
            (static_cast <float>(M_PI) / 2) * axisRotation -
            (timeSinceAction * (static_cast <float>(M_PI) / 2) / (Ball::timeToTurn))
            * axisRotation;

        if (stateBall == Ball::State::TurningLeft)
            eulerAngles = -eulerAngles;

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);
    } else if (stateBall == Ball::State::Jumping) {
        constexpr float offsetTimeToBeginCamMoving = 0.4f;
        const float timeSinceBeginningMoving = ballState.getTimeSecondsSinceAction();
        const float getTimeToGetDestination = ballState.timeToGetDestination();
        if (timeSinceBeginningMoving > (getTimeToGetDestination + offsetTimeToBeginCamMoving)) {

            float timeSinceBeginningCameraMoving = timeSinceBeginningMoving - (getTimeToGetDestination
                                                                               + offsetTimeToBeginCamMoving);

            if (timeSinceBeginningCameraMoving > durationMoveAbove) {
                timeSinceBeginningCameraMoving = durationMoveAbove;
            }
            _willComeBack = true;
            _cameraAboveWay = timeSinceBeginningCameraMoving / durationMoveAbove;

        }
    }

    if (_isComingBack) {
        const float durationSinceComeBack =
            JBTypesMethods::getFloatFromDurationMS(now - _timePointComeBack);
        if (durationSinceComeBack > durationMoveComingBack) {
            _isComingBack = false;
            _willComeBack = false;
            _cameraAboveWay = 0.f;
        } else {
            _cameraAboveWay = 1.f - (durationSinceComeBack / durationMoveComingBack);
        }
    }

    const glm::vec3 axisRotation = glm::cross(toSkyVec3, vecLookingDirection);
    const glm::vec3 eulerAngles = _cameraAboveWay * static_cast <float>(M_PI / 3) * axisRotation;

    const glm::quat quaternion(eulerAngles);

    matRotationCam = glm::toMat4(quaternion) * matRotationCam;

    const glm::vec3 direction = glm::normalize(initCenterCam - initPosCam);
    const glm::vec3 right = glm::cross(direction, toSkyVec3);
    const glm::vec3 upVector =
        matRotationCam * glm::vec4(glm::cross(right, direction), 1.f);

    const glm::vec4 posVec = matPosBall * matRotationCam * initPosCam;
    const glm::vec4 centerVec = matPosBall * matRotationCam * initCenterCam;

    _up = upVector;
    _pos = posVec;
    _center = centerVec;

}

void Camera::follow (const MapState& mapState) noexcept{

    const unsigned int xMax = mapState.width();
    const unsigned int yMax = mapState.height();
    const unsigned int zMax = mapState.deep();

    const glm::vec3 center { xMax / 2.f, yMax / 2.f, zMax / 2.f };

    float distanceMax = static_cast <float>(xMax);
    if (distanceMax < yMax) distanceMax = static_cast <float>(yMax);
    if (distanceMax < zMax) distanceMax = static_cast <float>(zMax);

    const float cameraDistanceNear = distanceMax * 1.f;
    const float cameraDistanceFar = distanceMax * 1.3f;
    const float diffF = mapState.getTimeSinceCreation();
    const float distanceX = cameraDistanceNear +
                            (cameraDistanceFar - cameraDistanceNear) *
                            ((static_cast <float>(-cos(diffF)) + 1.f) / 2.f);

    const float distanceZ = distanceX;

    const glm::mat4 translation =
        glm::translate(glm::vec3(distanceX, 0, distanceZ));

    const glm::mat4 rotation =
        glm::rotate(1.5f * diffF,
                    glm::vec3(0.f,
                              1.f + static_cast <float>(sin(diffF / 5.f)),
                              1.f + static_cast <float>(sin(diffF / 3.f))
                              )
                    );
    const glm::mat4 transform = rotation * translation;


    const glm::vec4 positionTransform = transform * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const glm::vec3 positionCamera(positionTransform.x,
                                   positionTransform.y,
                                   positionTransform.z);

    const glm::vec4 up = rotation * glm::vec4(0.f, 1.f, 0.f, 1.f);

    _pos = positionCamera;
    _center = center;
    _up = up;

}

bool Camera::transitionEffect (const BallState& ball, const MapState& map) noexcept{
    bool animationIsFinished;

    const JBTypes::vec3f position = ball.get3DPosition();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.3f;
    constexpr float distAboveBall = 1.2f;

    const float diffF = map.getTimeSinceCreation();
    constexpr float transitionDuration = 2.f;
    float t = diffF / transitionDuration;

    constexpr float distanceXStarting = 10.f;
    constexpr float distanceYStarting = 5.f;
    constexpr float distanceZStarting = 10.f;

    if (t > 1.f) {
        t = 1.f;
        animationIsFinished = true;
    } else {
        animationIsFinished = false;
    }

    const float tCos = cosf(t * static_cast <float>(M_PI_2) +
                            static_cast <float>(M_PI)) + 1.f;

    const glm::vec3 directionVector =
        glm::normalize(_center - _pos);
    const glm::mat4 upRotation = glm::rotate(tCos * 2.f *
                                             static_cast <float>(M_PI),
                                             directionVector);
    const glm::vec4 upVector = upRotation * glm::vec4(0.f, 1.f, 0.f, 1.f);

    _pos =
    {
        tCos*position.x + (1.f - tCos) * (position.x + distanceXStarting),
        t*(position.y + distAboveBall) +
        (1.f - tCos) * (position.y + distAboveBall + distanceYStarting),
        t*(position.z + distBehindBall) +
        (1.f - tCos) * (position.z + distBehindBall + distanceZStarting)
    };
    _center = { position.x, position.y, position.z - distDirPoint };
    _up = upVector;

    return animationIsFinished;
}


const glm::vec3& Camera::center() const noexcept{
    return _center;
}

const glm::vec3& Camera::pos() const noexcept{
    return _pos;
}

const glm::vec3& Camera::up() const noexcept{
    return _up;
}

glm::mat4 Camera::viewProjection() const noexcept{
    const glm::mat4 viewMatrix = glm::lookAt(_pos, _center, _up);
    const glm::mat4 perspectiveMatrix =
        Utility::windowResolutionX > Utility::windowResolutionY
        ? glm::perspective(_fovy, _ratio, zNear, zFar)
        : glm::perspective(
            2.f * atanf((1.f / _ratio) * tanf(_fovy / 2.f)),
            _ratio,
            zNear,
            zFar
            );
    return perspectiveMatrix * viewMatrix;
}
