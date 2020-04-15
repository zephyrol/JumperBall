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
                    _dirX(0), _dirY(0), _dirZ(0),
                    _upX(0),  _upY(1),  _upZ(0),
                    _displayBehind(false),
                    _willComeBack(false),
                    _isComingBack(false),
                    _cameraAboveWay(0.f),
                    _timePointComeBack()
                    
{
}

void Camera::follow(const Ball& ball) noexcept {
    
    const JumperBallTypes::vec3f position  = ball.get3DPosition();
    const auto sideBall                 = ball.currentSide();
    const auto lookingDirection         = ball.lookTowards();

    const auto stateBall                = ball.state();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.3f;
    constexpr float distAboveBall = 1.2f;


    glm::mat4 matPosCam     (1.f);
    glm::mat4 matDirCam     (1.f);
    glm::mat4 matRotationCam(1.f);

    glm::vec4 posVec(0.f,0.f,0.f,1.f);
    glm::vec4 dirVec(0.f,0.f,0.f,1.f);
    glm::vec4 upVec (0.f,0.f,0.f,1.f);

    glm::mat4 matPosBall (1.f);

    matPosBall = glm::translate(matPosBall, 
            glm::vec3(position.x,position.y,position.z));

    
    switch (sideBall) {
        case JumperBallTypes::Direction::North:
            upVec.z = -1.f;
            matPosCam = glm::translate(matPosCam,
                    glm::vec3(0.f,0.f,-distAboveBall));
            break;
        case JumperBallTypes::Direction::South:
            upVec.z = 1.f;
            matPosCam = glm::translate(matPosCam,
                    glm::vec3(0.f,0.f,distAboveBall));
            break;
        case JumperBallTypes::Direction::East:
            upVec.x = 1.f;
            matPosCam = glm::translate(matPosCam,
                    glm::vec3(distAboveBall,0.f,0.f));
            break;
        case JumperBallTypes::Direction::West:
            upVec.x = -1.f;
            matPosCam = glm::translate(matPosCam,
                    glm::vec3(-distAboveBall,0.f,0.f));
            break;
        case JumperBallTypes::Direction::Up:
            upVec.y = 1.f;
            matPosCam = glm::translate(matPosCam,
                    glm::vec3(0.f,distAboveBall,0.f));
            break;
        case JumperBallTypes::Direction::Down:
            upVec.y = -1.f;
            matPosCam = glm::translate(matPosCam,
                    glm::vec3(0.f,-distAboveBall,0.f));
            break;
        default :
            break;
    }
    
    JumperBallTypes::vec3f dir = 
        JumperBallTypesMethods::directionAsVector(lookingDirection);
    matPosCam = glm::translate(matPosCam,
            -glm::vec3(dir.x * distBehindBall,dir.y * distBehindBall,
              dir.z * distBehindBall));
    matDirCam = glm::translate(matDirCam, 
            -glm::vec3(dir.x * -distDirPoint, dir.y * -distDirPoint,
            dir.z * -distDirPoint));


    const auto getAxis = [] (JumperBallTypes::Direction direction) {
        glm::vec3 axisVector; 
        switch (direction) {
            case JumperBallTypes::Direction::North:
                axisVector = {0.f,0.f,-1.f}; break;
            case JumperBallTypes::Direction::South:
                axisVector = {0.f,0.f,1.f}; break;
            case JumperBallTypes::Direction::East:
                axisVector = {1.f,0.f,0.f}; break;
            case JumperBallTypes::Direction::West:
                axisVector = {-1.f,0.f,0.f}; break;
            case JumperBallTypes::Direction::Up:
                axisVector = {0.f,1.f,0.f}; break;
            case JumperBallTypes::Direction::Down:
                axisVector = {0.f,-1.f,0.f}; break;
            default :
                break;
        }
        return axisVector;
    };

    constexpr float durationMoveAbove = 0.2f;
    constexpr float durationMoveComingBack = 0.2f;

    const float timeSinceAction         = ball.getTimeSecondsSinceAction();
    const JumperBallTypes::timePointMs timeAction = ball.getTimeActionMs();
    const JumperBallTypes::timePointMs now = 
                                    JumperBallTypesMethods::getTimePointMSNow();

    if (stateBall == Ball::State::Staying) {
        if(_willComeBack) {
            _timePointComeBack= now;
            _isComingBack = true;
            _willComeBack = false;
        }
    }
    if (stateBall == Ball::State::Moving) {
        const auto  infoNext             = ball.getNextBlockInfo();

        const glm::vec3 axisOldLook      = getAxis(lookingDirection);
        const glm::vec3 axisNewLook      = getAxis(infoNext.nextLook);
        const glm::vec3 axisRotation     = glm::cross(axisOldLook,axisNewLook);

        const glm::vec3 eulerAngles = (timeSinceAction* 
          (static_cast<float>(M_PI)/2)/ ball.timeToGetNextBlock) * axisRotation;
         
        const glm::quat quaternion (eulerAngles);
        matRotationCam = glm::toMat4(quaternion); 
    }
    else if (stateBall == Ball::State::TurningLeft) {
        const glm::vec3 axisRotation     = glm::vec3(upVec);
         
        const glm::vec3 eulerAngles = 
        -(static_cast<float>(M_PI)/2) * axisRotation
        +
        (timeSinceAction* 
          (static_cast<float>(M_PI)/2)/ ball.timeToGetNextBlock) * axisRotation;

        const glm::quat quaternion (eulerAngles);
        matRotationCam = glm::toMat4(quaternion); 
    }
    else if (stateBall == Ball::State::TurningRight) {
        const glm::vec3 axisRotation     = glm::vec3(upVec);
         
        const glm::vec3 eulerAngles = 
        (static_cast<float>(M_PI)/2) * axisRotation
        -
        (timeSinceAction* 
          (static_cast<float>(M_PI)/2)/ ball.timeToGetNextBlock) * axisRotation;

        const glm::quat quaternion (eulerAngles);
        matRotationCam = glm::toMat4(quaternion); 
         
    }
    else if (stateBall == Ball::State::Jumping) {
        constexpr float offsetTimeToBeginCamMoving = 0.2f;
        const float timeSinceBeginningMoving =
          JumperBallTypesMethods::getFloatFromDurationMS(now-timeAction);
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
        JumperBallTypesMethods::getFloatFromDurationMS(now- _timePointComeBack);
        std::cout << durationSinceComeBack << std::endl;
        if (durationSinceComeBack > durationMoveComingBack)
        {
          _isComingBack = false;
          _willComeBack= false;
          _cameraAboveWay = 0.f;           
        } else {
          _cameraAboveWay = 1.f-(durationSinceComeBack/durationMoveComingBack);
        }
    }

    const glm::vec3 upVector          = glm::vec3(upVec);
    const glm::vec3 axisView          = getAxis(lookingDirection);
    const glm::vec3 axisRotation      = glm::cross(upVector,axisView);
    glm::vec3 eulerAngles;
    eulerAngles = _cameraAboveWay * static_cast<float>(M_PI/3) * axisRotation;
    
    const glm::quat quaternion (eulerAngles);
    matRotationCam = glm::toMat4(quaternion) * matRotationCam; 

    posVec = matPosBall * matRotationCam * matPosCam * posVec;
    dirVec = matPosBall * matRotationCam * matDirCam * dirVec;
    upVec  = matRotationCam * upVec;

    _upX  = upVec.x;
    _upY  = upVec.y;
    _upZ  = upVec.z;

    _posX = posVec.x;
    _posY = posVec.y;
    _posZ = posVec.z;

    _dirX = dirVec.x;
    _dirY = dirVec.y;
    _dirZ = dirVec.z;

    _displayBehind = false;

}

void Camera::follow(const Map& map) noexcept{

    const unsigned int xMax = map.boundingBoxXMax();
    const unsigned int yMax = map.boundingBoxYMax();
    const unsigned int zMax = map.boundingBoxZMax();
    
    
    const float centerX = xMax/2.f;
    const float centerY = yMax/2.f;
    const float centerZ = zMax/2.f;
    
    float distanceMax = xMax;
    if ( distanceMax < yMax) distanceMax = yMax;
    if ( distanceMax < zMax) distanceMax = zMax;
    
    const float cameraDistanceNear = distanceMax * 0.75f;
    const float cameraDistanceFar =  distanceMax * 1.2f;
    
    const JumperBallTypes::timePointMs now  = 
    JumperBallTypesMethods::getTimePointMSNow();

    const JumperBallTypes::durationMs  diff = now - 
      JumperBallTypesMethods::getTimePointMsFromTimePoint(map.timeCreation());

    
    const float diffF = JumperBallTypesMethods::getFloatFromDurationMS(diff);
    
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

    _dirX = centerX;
    _dirY = centerY;
    _dirZ = centerZ;

    /*glm::vec3 directionVector(_dirX-_posX,_dirY-_posY, _dirZ-_posZ);
    glm::normalize(directionVector);
    glm::vec3 axeOne (-directionVector.y,direction)*/
    glm::vec4 up(0.f,1.f,0.f,1.f);
    up = rotation * up;

    _upX  = up.x;
    _upY  = up.y;
    _upZ  = up.z;

    _displayBehind = true;
}


glm::vec3 Camera::dir() const noexcept{
    return glm::vec3 {_dirX,_dirY,_dirZ};
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
