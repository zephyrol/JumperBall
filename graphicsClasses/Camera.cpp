/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera() :  _posX(1), _posY(1), _posZ(1),
                    _centerX(0), _centerY(0), _centerZ(0),
                    _upX(0),  _upY(1),  _upZ(0),
                    _displayBehind(false),
                    _willComeBack(false),
                    _isComingBack(false),
                    _cameraAboveWay(0.f),
                    _timePointComeBack()
                    
{
}

void Camera::follow(const Ball& ball) noexcept {
    
    const JBTypes::vec3f& position      = ball.get3DPosition();
    const auto sideBall                 = ball.currentSide();
    const auto lookingDirection         = ball.lookTowards();

    const auto stateBall                = ball.state();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.3f;
    constexpr float distAboveBall = 1.2f;

    glm::mat4 matRotationCam(1.f);

    const glm::mat4 matPosBall =
        glm::translate( glm::vec3(position.x,position.y,position.z));

    const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(sideBall);
    
    const glm::vec3 toSkyVec3 {vecDir.x, vecDir.y, vecDir.z};
    const glm::vec4 toSky (toSkyVec3,1.f);
    
    const JBTypes::vec3f coreVecLookingDirection =
        JBTypesMethods::directionAsVector(lookingDirection);
    
    const glm::vec3 vecLookingDirection { coreVecLookingDirection.x,
        coreVecLookingDirection.y, coreVecLookingDirection.z };
    
    const glm::mat4 matPosCam (glm::translate( distAboveBall * toSkyVec3 -
        distBehindBall * vecLookingDirection));
    
    const glm::mat4 matDirCam =
        glm::translate( -glm::vec3(vecLookingDirection.x * -distDirPoint,
                                   vecLookingDirection.y * -distDirPoint,
                                   vecLookingDirection.z * -distDirPoint));

    constexpr float durationMoveAbove = 0.2f;
    constexpr float durationMoveComingBack = 0.2f;

    const float timeSinceAction = ball.getTimeSecondsSinceAction();
    const JBTypes::timePointMs timeAction = ball.getTimeActionMs();
    const JBTypes::timePointMs now = JBTypesMethods::getTimePointMSNow();

    if (stateBall == Ball::State::Staying) {
        if(_willComeBack) {
            _timePointComeBack= now;
            _isComingBack = true;
            _willComeBack = false;
        }
    }
    if (stateBall == Ball::State::Moving) {
        const auto  infoNext             = ball.getNextBlockInfo();
        const JBTypes::vec3f vecLookDir  =
            JBTypesMethods::directionAsVector(lookingDirection);
        const JBTypes::vec3f vecNextLook  =
            JBTypesMethods::directionAsVector(infoNext.nextLook);
        const glm::vec3 axisOldLook {vecLookDir.x, vecLookDir.y, vecLookDir.z};
        const glm::vec3 axisNewLook
            {vecNextLook.x, vecNextLook.y, vecNextLook.z};
        
        const glm::vec3 axisRotation     = glm::cross(axisOldLook,axisNewLook);
        const glm::vec3 eulerAngles = (timeSinceAction*
          (static_cast<float>(M_PI)/2)/ ball.timeToGetNextBlock) * axisRotation;
         
        const glm::quat quaternion (eulerAngles);
        matRotationCam = glm::toMat4(quaternion); 
    }
    else if (stateBall == Ball::State::TurningLeft ||
             stateBall == Ball::State::TurningRight) {
        const glm::vec3& axisRotation     = toSkyVec3;
         
        glm::vec3 eulerAngles =
            (static_cast<float>(M_PI)/2) * axisRotation -
            (timeSinceAction* (static_cast<float>(M_PI)/2)/ (ball.timeToTurn))
            * axisRotation;
        
        if (stateBall == Ball::State::TurningLeft)
            eulerAngles = -eulerAngles;

        const glm::quat quaternion (eulerAngles);
        matRotationCam = glm::toMat4(quaternion);
    }
    else if (stateBall == Ball::State::Jumping) {
        constexpr float offsetTimeToBeginCamMoving = 0.2f;
        const float timeSinceBeginningMoving =
          JBTypesMethods::getFloatFromDurationMS(now-timeAction);
        if (timeSinceBeginningMoving > 
                (ball.getMechanicsJumping().getTimeToGetDestination()
                  + offsetTimeToBeginCamMoving)){
            
            float timeSinceBeginningCameraMoving = timeSinceBeginningMoving- 
                (ball.getMechanicsJumping().getTimeToGetDestination()
                  + offsetTimeToBeginCamMoving);

            if (timeSinceBeginningCameraMoving > durationMoveAbove) {
                timeSinceBeginningCameraMoving = durationMoveAbove;
            }
            _willComeBack = true;
            _cameraAboveWay = timeSinceBeginningCameraMoving/durationMoveAbove; 
            
        } 
    }
    
    if ( _isComingBack) {
        const float durationSinceComeBack = 
            JBTypesMethods::getFloatFromDurationMS(now- _timePointComeBack);
        if (durationSinceComeBack > durationMoveComingBack)
        {
          _isComingBack = false;
          _willComeBack= false;
          _cameraAboveWay = 0.f;           
        } else {
          _cameraAboveWay = 1.f-(durationSinceComeBack/durationMoveComingBack);
        }
    }

    const JBTypes::vec3f vecLookDir  =
        JBTypesMethods::directionAsVector(lookingDirection);
    const glm::vec3 axisView {vecLookDir.x, vecLookDir.y, vecLookDir.z};
    
    const glm::vec3 axisRotation = glm::cross(toSkyVec3,axisView);
    const glm::vec3 eulerAngles =
        _cameraAboveWay * static_cast<float>(M_PI/3) * axisRotation;
    
    const glm::quat quaternion (eulerAngles);
    
    matRotationCam = glm::toMat4(quaternion) * matRotationCam; 

    const glm::vec4 posVec =
        matPosBall * matRotationCam * matPosCam * glm::vec4{0.f,0.f,0.f,1.f};
    const glm::vec4 centerVec =
        matPosBall * matRotationCam * matDirCam * glm::vec4{0.f,0.f,0.f,1.f};
    
    const glm::vec4 upVector = matRotationCam * toSky;

    _upX  = upVector.x;
    _upY  = upVector.y;
    _upZ  = upVector.z;

    _posX = posVec.x;
    _posY = posVec.y;
    _posZ = posVec.z;

    _centerX = centerVec.x;
    _centerY = centerVec.y;
    _centerZ = centerVec.z;

    _displayBehind = false;

}

void Camera::follow(const Map& map) noexcept{

    const unsigned int xMax = map.boundingBoxXMax();
    const unsigned int yMax = map.boundingBoxYMax();
    const unsigned int zMax = map.boundingBoxZMax();
    
    
    const float centerX = xMax/2.f;
    const float centerY = yMax/2.f;
    const float centerZ = zMax/2.f;
    
    float distanceMax = static_cast<float>(xMax);
    if ( distanceMax < yMax) distanceMax = static_cast<float>(yMax);
    if ( distanceMax < zMax) distanceMax = static_cast<float>(zMax);
    
    const float cameraDistanceNear = distanceMax * 0.75f;
    const float cameraDistanceFar =  distanceMax * 1.2f;
    
    const JBTypes::timePointMs now  = 
    JBTypesMethods::getTimePointMSNow();

    const JBTypes::durationMs  diff = now - 
      JBTypesMethods::getTimePointMsFromTimePoint(map.timeCreation());

    
    const float diffF = JBTypesMethods::getFloatFromDurationMS(diff);
    
    const float distanceX = cameraDistanceNear +
            (cameraDistanceFar-cameraDistanceNear) *
            ((static_cast<float>(-cos(diffF))+1.f)/2.f);

    const float distanceY = cameraDistanceNear +
            (cameraDistanceFar-cameraDistanceNear) *
            ((static_cast<float>(-cos(diffF))+1.f)/2.f);

    const float distanceZ = cameraDistanceNear +
            (cameraDistanceFar-cameraDistanceNear) *
            ((static_cast<float>(-cos(diffF))+1.f)/2.f);

    const glm::mat4 translation = 
        glm::translate(glm::vec3(distanceX,distanceY,distanceZ));

    const glm::mat4 rotation = glm::rotate( 1.5f*diffF ,
    glm::vec3(0.f,(1.f+static_cast<float>(sin(diffF/5.f)))/2.f,
              (1.f+static_cast<float>(sin(diffF/3.f)))/2.f));
    const glm::mat4 transform = rotation * translation;

    
    const glm::vec4 positionTransform = transform * glm::vec4(0.f,0.f,0.f,1.f);
    const glm::vec3 positionCamera (positionTransform.x,
                                    positionTransform.y,
                                    positionTransform.z);

    _posX = positionCamera.x;
    _posY = positionCamera.y;
    _posZ = positionCamera.z;


    _posX = positionCamera.x;
    _posY = positionCamera.y;
    _posZ = positionCamera.z;

    _centerX = centerX;
    _centerY = centerY;
    _centerZ = centerZ;

    glm::vec4 up(0.f,1.f,0.f,1.f);
    up = rotation * up;

    _upX  = up.x;
    _upY  = up.y;
    _upZ  = up.z;

    _displayBehind = true;
}

bool Camera::transitionEffect(const Ball &ball, const Map &map) noexcept
{
    bool animationIsFinished;

    const JBTypes::vec3f position  = ball.get3DPosition();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.3f;
    constexpr float distAboveBall = 1.2f;
    //-----------------------

    const JBTypes::timePointMs now =
                                    JBTypesMethods::getTimePointMSNow();

    const JBTypes::durationMs  diff = now -
      JBTypesMethods::getTimePointMsFromTimePoint(map.timeCreation());

    const float diffF = JBTypesMethods::getFloatFromDurationMS(diff);
    constexpr float transitionDuration = 2.f;
    float t = diffF / transitionDuration ;

    constexpr float distanceXStarting = 10.f;
    constexpr float distanceYStarting = 5.f;
    constexpr float distanceZStarting = 10.f;

    if (t > 1.f) {
        t = 1.f;
        animationIsFinished = true;
    } else  {
        animationIsFinished = false;
    }

    const float tCos = cosf( t * static_cast<float>(M_PI_2) +
                                  static_cast<float>(M_PI)) + 1.f;

    const glm::vec3 directionVector =
            glm::normalize( glm::vec3{_centerX - _posX, _centerY - _posY,
                                _centerZ - _posZ });
    const glm::mat4 upRotation = glm::rotate( tCos * 2.f *
                                              static_cast<float>(M_PI),
                                              directionVector);
    const glm::vec4 upVector = upRotation * glm::vec4(0.f,1.f,0.f,1.f);

    _posX = tCos *  position.x +
            (1.f-tCos) * (position.x + distanceXStarting);
    _posY = t* (position.y + distAboveBall) +
            (1.f-tCos) * (position.y + distAboveBall + distanceYStarting);
    _posZ = t* (position.z + distBehindBall) +
            (1.f-tCos) * (position.z + distBehindBall + distanceZStarting);

    _centerX = position.x;
    _centerY = position.y;
    _centerZ = position.z - distDirPoint;

    _upX = upVector.x;
    _upY = upVector.y;
    _upZ = upVector.z;

    _displayBehind = true;

    return animationIsFinished;
}


glm::vec3 Camera::center() const noexcept{
    return glm::vec3 {_centerX,_centerY,_centerZ};
}

glm::vec3 Camera::pos() const noexcept{
    return glm::vec3 {_posX,_posY,_posZ};
}

glm::vec3 Camera::up() const noexcept{
    return glm::vec3 {_upX,_upY,_upZ};
}

bool Camera::displayBehind() const noexcept{
    return _displayBehind;
}

float Camera::distanceBehindBall(const Ball& ball) noexcept {
    constexpr float offsetCenterBlock = 0.5f;
    const JBTypes::vec3f lookVec = ball.lookTowardsAsVector();
    const auto position = ball.get3DPosition();
    float distance = fmod(fabsf(position.x * lookVec.x + position.y * lookVec.y
                                + position.z * lookVec.z
                                - offsetCenterBlock),1.f) + offsetCenterBlock;
    
    if ( ball.state() == Ball::State::TurningLeft || ball.state() == Ball::State::TurningRight)
        distance *= 10;
    return distance;
}
