/*
 * File: Camera.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera(const Map& map, float ratio):
    _map(map),
    _fovy(computeFovy(ratio)),
    _rotationAngle(computeRotationAngle(_fovy)),
    _movement(Camera::Movement::TurningAroundMap),
    _pos(1.f, 0.f, 0.f),
    _center(0.f, 0.f, 0.f),
    _up(0.f, 1.f, 0.f),
    _willComeBack(false),
    _isComingBack(false),
    _cameraAboveWay(0.f),
    _timeSinceCreation(map.getTimeSinceCreation()),
    _timePointComeBack(),
    _perspectiveMatrix(glm::perspective(_fovy, ratio, zNear, zFar))
{
}

void Camera::update() noexcept{
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
const Camera::Movement& Camera::getMovement() noexcept{
    return _movement;
}

void Camera::turnAroundMap() noexcept{
    _movement = Camera::Movement::TurningAroundMap;
}

void Camera::followBall() noexcept{
    _movement = Camera::Movement::FollowingBall;
}

void Camera::approachBall() noexcept{
    _movement = Camera::Movement::ApproachingBall;
}

void Camera::followingBallUpdate() noexcept{

    const auto& ball = *_map.getBall();
    const auto lookingDirection = ball.lookTowardsAsVector();

    const auto stateBall = ball.state();

    constexpr float durationMoveAbove = 0.2f;
    constexpr float durationMoveComingBack = 0.2f;

    const auto sideBall = ball.currentSideAsVector();
    const glm::vec3 toSkyVec3 = Utility::convertToOpenGLFormat(sideBall);
    const glm::vec4 toSky(toSkyVec3, 1.f);

    const float timeSinceAction = ball.getTimeSecondsSinceAction();
    const JBTypes::timePointMs now = JBTypesMethods::getTimePointMSNow();

    if (_willComeBack) {
        _timePointComeBack = now;
        _isComingBack = true;
        _willComeBack = false;
    }

    glm::mat4 matRotationCam(1.f);
    const glm::vec3 vecLookingDirection = Utility::convertToOpenGLFormat(lookingDirection);

    if (stateBall == Ball::State::Moving) {
        const JBTypes::vec3f vecNextLook = _map.getNextLook();

        const glm::vec3 axisNewLook = Utility::convertToOpenGLFormat(vecNextLook);

        const glm::vec3 axisRotation = glm::cross(vecLookingDirection, axisNewLook);
        const glm::vec3 eulerAngles = (timeSinceAction *
                                       (static_cast <float>(M_PI_2)) / Ball::timeToGetNextBlock) *
                                      axisRotation;

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);
    } else if ( stateBall == Ball::State::TurningLeft || stateBall == Ball::State::TurningRight ) {
        const glm::vec3& axisRotation = toSkyVec3;

        glm::vec3 eulerAngles = static_cast <float>(M_PI_2) * axisRotation -
            (timeSinceAction * static_cast <float>(M_PI_2) / (Ball::timeToTurn)) * axisRotation;

        if (stateBall == Ball::State::TurningLeft) {
            eulerAngles = -eulerAngles;
        }

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);

    } else if (stateBall == Ball::State::Jumping) {
        constexpr float offsetTimeToBeginCamMoving = 0.4f;
        const float timeSinceBeginningMoving = ball.getTimeSecondsSinceAction();
        const float timeToGetDestination = ball.getTimeToGetDestination();
        if (timeSinceBeginningMoving > (timeToGetDestination + offsetTimeToBeginCamMoving)) {

            float timeSinceBeginningCameraMoving = timeSinceBeginningMoving -
                (timeToGetDestination + offsetTimeToBeginCamMoving);

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

    const glm::vec3 axisRotation = glm::cross(vecLookingDirection, toSkyVec3);

    const glm::vec3 eulerAngles = _cameraAboveWay * static_cast <float>(M_PI / 3) * axisRotation;

    const glm::quat quaternion(eulerAngles);

    const glm::vec3 offset (0.3f * vecLookingDirection + ball.getRadius() * toSkyVec3);
    constexpr float distAboveBall = 1.2f;

    const glm::vec3 toBall(distBehindBall * vecLookingDirection - distAboveBall * toSkyVec3);
    const glm::vec3 toGround = toBall - offset;
    const glm::vec3 initPosCam = -toBall;

    const glm::mat4 rotationToRespectFov = glm::rotate(_fovy / 2.f, axisRotation);
    const glm::vec4 lookAtDirection = rotationToRespectFov * glm::vec4(toGround, 1.f);
    //const glm::vec3 initCenterCam = initPosCam + glm::vec3(lookAtDirection);
    const glm::vec3 initCenterCam = 2.f * vecLookingDirection;

    const JBTypes::vec3f& position = ball.get3DPosition();
    const glm::mat4 matPosBall = glm::translate(Utility::convertToOpenGLFormat(position));
    matRotationCam = glm::toMat4(quaternion) * matRotationCam;
    const glm::mat4 matPosBallRotationCam = matPosBall * matRotationCam;

    const glm::vec4 posVec =  matPosBallRotationCam * glm::vec4(initPosCam, 1.f);
    _pos = posVec;

    const glm::vec4 centerVec = matPosBallRotationCam * glm::vec4(initCenterCam, 1.f);
    _center = centerVec;

    const glm::vec3 upVector = matRotationCam * glm::vec4(glm::cross(axisRotation, glm::vec3(lookAtDirection)), 1.f);
    _up = upVector;

}

void Camera::turningAroundMapUpdate() noexcept{

    const auto xMax = static_cast<float>(_map.width());
    const auto yMax = static_cast<float>(_map.height());
    const auto zMax = static_cast<float>(_map.depth());

    const glm::vec3 center { (xMax - 1.f) / 2.f, (yMax - 1.f) / 2.f, (zMax - 1.f) / 2.f };

    float distanceMax = xMax;
    if (distanceMax < yMax) distanceMax = yMax;
    if (distanceMax < zMax) distanceMax = zMax;

    const float cameraDistanceNear = distanceMax * 1.f;
    const float cameraDistanceFar = distanceMax * 1.3f;
    const float diffF = _timeSinceCreation;
    const float distanceX = cameraDistanceNear +
                            (cameraDistanceFar - cameraDistanceNear) *
                            ((-cosf(diffF) + 1.f) / 2.f);

    const float distanceZ = distanceX;

    const glm::mat4 translation = glm::translate(glm::vec3(distanceX, 0, distanceZ));

    const glm::mat4 rotation = glm::rotate(
        1.5f * diffF,
        glm::vec3(0.f, 1.f + sinf(diffF / 5.f), 1.f + sinf(diffF / 3.f))
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

    const JBTypes::vec3f& position = _map.getBall()->get3DPosition();

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

    const auto oneMinusTCos = 1.f - tCos;

    _pos = {
        tCos * position.x + oneMinusTCos * (position.x + distanceXStarting),
        t * (position.y + distAboveBall) + oneMinusTCos * (position.y + distAboveBall + distanceYStarting),
        t * (position.z + distBehindBall) + oneMinusTCos * (position.z + distBehindBall + distanceZStarting)
    };
    _center = { position.x, position.y, position.z - distDirPoint };
    _up = upVector;

    return animationIsFinished;
}

glm::mat4 Camera::genVPMatrixFromStar (const Star& star) {

    constexpr float offsetJumpingBall = 1.f; // size of ball + jump height
    const float envSize = star.envSize();
    const float halfBoundingBoxSize = envSize / 2.f + offsetJumpingBall;

    // We use a close star position to get a better ZBuffer accuracy
    const JBTypes::vec3f& rotationCenter = star.rotationCenter();
    const glm::vec3 centerWorld = Utility::convertToOpenGLFormat(rotationCenter);

    const JBTypes::vec3f& position = star.position();
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
    return _perspectiveMatrix * viewMatrix;
}

SceneElement::GlobalState Camera::getGlobalState() const {
    return SceneElement::GlobalState::United;
}

void Camera::setRatio(float ratio) {
    _fovy = computeFovy(ratio);
    _rotationAngle = computeRotationAngle(_fovy);
    std::cout << "angle" << _rotationAngle << std::endl;
    _perspectiveMatrix = glm::perspective(_fovy, ratio, zNear, zFar);
}

float Camera::computeRotationAngle(float fovy) noexcept {

    // Using law of cosines
    constexpr float squaredDistBehindBall = distBehindBall * distBehindBall;
    constexpr float squaredDistDirPoint = distDirPoint * distDirPoint;
    constexpr float twoDistDirPointDistBehindBall = 2.f * distBehindBall * distDirPoint;
    const float squaredEyeTargetLength = squaredDistDirPoint - squaredDistBehindBall +
                                         twoDistDirPointDistBehindBall * cosf(fovy / 2.f);
    std::cout << "C" << sqrtf(squaredEyeTargetLength) << std::endl;

    return static_cast<float>(M_PI) - acosf(
        (squaredDistBehindBall + squaredDistDirPoint - squaredEyeTargetLength) / twoDistDirPointDistBehindBall
    );
}

float Camera::computeFovy(float ratio) noexcept {
    constexpr float defaultFovy = 1.22173f;// 70 degrees;
    if (ratio > 1.f)  {
        return defaultFovy;
    }
    constexpr float maxFov = 1.74;
    const float theoricalFovVerticalMod = 2.f * atanf((1.f / ratio) * tanf(defaultFovy/ 2.f));
    return theoricalFovVerticalMod > maxFov
        ? maxFov
        : theoricalFovVerticalMod;
}

