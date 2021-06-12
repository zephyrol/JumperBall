/*
 * File: Camera.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera(const Map& map, const Ball& ball):
    _map(map),
    _ball(ball),
    _fovy(glm::radians(70.f)),
    _ratio(static_cast <float>(Utility::windowResolutionX) /
           static_cast <float>(Utility::windowResolutionY)),
    _movement(Camera::Movement::TurningAroundMap),
    _pos(1.f, 0.f, 0.f),
    _center(0.f, 0.f, 0.f),
    _up(0.f, 1.f, 0.f),
    _willComeBack(false),
    _isComingBack(false),
    _cameraAboveWay(0.f),
    _timeSinceCreation(map.getTimeSinceCreation()),
    _timePointComeBack() {
}

void Camera::update() noexcept {
    _timeSinceCreation = _map.getTimeSinceCreation();
    if (_movement == Camera::Movement::TurningAroundMap) {
        turningAroundMapUpdate();
        return;
    }
    if (_movement == Camera::Movement::FollowingBall) {
        followingBallUpdate();
        return;
    }
    if (approachingBallUpdate()) {
        _movement = Camera::Movement::FollowingBall;
        update();
    }
}
const Camera::Movement& Camera::getMovement() noexcept {
    return _movement;
}

void Camera::turnAroundMap() noexcept {
    _movement = Camera::Movement::TurningAroundMap;
}

void Camera::followBall() noexcept {
    _movement = Camera::Movement::FollowingBall;
}

void Camera::approachBall() noexcept {
    _movement = Camera::Movement::ApproachingBall;
}

void Camera::followingBallUpdate() noexcept{

    const JBTypes::vec3f& position = _ball.get3DPosition();
    const auto sideBall = _ball.currentSideAsVector();
    const auto lookingDirection = _ball.lookTowardsAsVector();
    const glm::vec3 vecLookingDirection = Utility::convertToOpenGLFormat(lookingDirection);

    const auto stateBall = _ball.state();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.3f;
    constexpr float distAboveBall = 1.2f;

    glm::mat4 matRotationCam(1.f);

    const glm::mat4 matPosBall = glm::translate(Utility::convertToOpenGLFormat(position));

    const glm::vec3 toSkyVec3 = Utility::convertToOpenGLFormat(sideBall);
    const glm::vec4 toSky(toSkyVec3, 1.f);


    const glm::vec4 initPosCam(distAboveBall * toSkyVec3 - distBehindBall * vecLookingDirection, 1.f);
    const glm::vec4 initCenterCam
        (-glm::vec3(vecLookingDirection.x * -distDirPoint,
                    vecLookingDirection.y * -distDirPoint,
                    vecLookingDirection.z * -distDirPoint), 1.f);

    constexpr float durationMoveAbove = 0.2f;
    constexpr float durationMoveComingBack = 0.2f;

    const float timeSinceAction = _ball.getTimeSecondsSinceAction();
    const JBTypes::timePointMs now = JBTypesMethods::getTimePointMSNow();

    if (stateBall == Ball::State::Staying) {
        if (_willComeBack) {
            _timePointComeBack = now;
            _isComingBack = true;
            _willComeBack = false;
        }
    }
    if (stateBall == Ball::State::Moving) {
        const JBTypes::vec3f vecNextLook = _ball.getNextLook();

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
        const float timeSinceBeginningMoving = _ball.getTimeSecondsSinceAction();
        const float timeToGetDestination = _ball.getTimeToGetDestination();
        if (timeSinceBeginningMoving > (timeToGetDestination + offsetTimeToBeginCamMoving)) {

            float timeSinceBeginningCameraMoving = timeSinceBeginningMoving - (timeToGetDestination
                                                                               + offsetTimeToBeginCamMoving);

            if (timeSinceBeginningCameraMoving > durationMoveAbove) {
                timeSinceBeginningCameraMoving = durationMoveAbove;
            }
            _willComeBack = true;
            _cameraAboveWay = timeSinceBeginningCameraMoving / durationMoveAbove;

        }
    }

    if (_isComingBack) {
        const float durationSinceComeBack = JBTypesMethods::getFloatFromDurationMS(now - _timePointComeBack);
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
    const glm::vec3 upVector = matRotationCam * glm::vec4(glm::cross(right, direction), 1.f);

    const glm::vec4 posVec = matPosBall * matRotationCam * initPosCam;
    const glm::vec4 centerVec = matPosBall * matRotationCam * initCenterCam;

    _up = upVector;
    _pos = posVec;
    _center = centerVec;

}

void Camera::turningAroundMapUpdate() noexcept{

    const unsigned int xMax = _map.width();
    const unsigned int yMax = _map.height();
    const unsigned int zMax = _map.deep();

    const glm::vec3 center { xMax / 2.f, yMax / 2.f, zMax / 2.f };

    float distanceMax = static_cast <float>(xMax);
    if (distanceMax < yMax) distanceMax = static_cast <float>(yMax);
    if (distanceMax < zMax) distanceMax = static_cast <float>(zMax);

    const float cameraDistanceNear = distanceMax * 1.f;
    const float cameraDistanceFar = distanceMax * 1.3f;
    const float diffF = _timeSinceCreation;
    const float distanceX = cameraDistanceNear +
                            (cameraDistanceFar - cameraDistanceNear) *
                            ((static_cast <float>(-cos(diffF)) + 1.f) / 2.f);

    const float distanceZ = distanceX;

    const glm::mat4 translation = glm::translate(glm::vec3(distanceX, 0, distanceZ));

    const glm::mat4 rotation = glm::rotate(
        1.5f * diffF,
        glm::vec3(0.f,
                  1.f + static_cast<float>(sin(diffF / 5.f)),
                  1.f + static_cast<float>(sin(diffF / 3.f)))
    );
    const glm::mat4 transform = rotation * translation;


    const glm::vec4 positionTransform = transform * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const glm::vec3 positionCamera(positionTransform.x, positionTransform.y, positionTransform.z);

    const glm::vec4 up = rotation * glm::vec4(0.f, 1.f, 0.f, 1.f);

    _pos = positionCamera;
    _center = center;
    _up = up;

}

bool Camera::approachingBallUpdate() noexcept{
    bool animationIsFinished;

    const JBTypes::vec3f position = _ball.get3DPosition();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.3f;
    constexpr float distAboveBall = 1.2f;

    const float diffF = _timeSinceCreation;
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

    const float tCos = cosf(t * static_cast <float>(M_PI_2) + static_cast <float>(M_PI)) + 1.f;

    const glm::vec3 directionVector = glm::normalize(_center - _pos);
    const glm::mat4 upRotation = glm::rotate(tCos * 2.f * static_cast <float>(M_PI), directionVector);
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

glm::mat4 Camera::genVPMatrixFromStar (const StarState& starState) {

    constexpr float offsetJumpingBall = 1.f; // size of ball + jump height
    const float envSize = starState.getEnvSize();
    const float halfBoundingBoxSize = envSize / 2.f + offsetJumpingBall;

    // We use a close star position to get a better ZBuffer accuracy
    const JBTypes::vec3f& rotationCenter = starState.getRotationCenter();
    const glm::vec3 centerWorld = Utility::convertToOpenGLFormat(rotationCenter);

    const JBTypes::vec3f& position = starState.getPosition();
    const glm::vec3 closeStarPosition = centerWorld + glm::normalize(
        (Utility::convertToOpenGLFormat(position)) - centerWorld
        )  * halfBoundingBoxSize;

    return glm::ortho(
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        zNear, zNear + 2.f * halfBoundingBoxSize
        ) * glm::lookAt(closeStarPosition, centerWorld, glm::vec3(0.f, 1.f, 0.f));

}

const glm::vec3& Camera::pos() const noexcept{
    return _pos;
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
