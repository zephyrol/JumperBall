/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.cpp
 * Author: seb
 * 
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera() : _posX(1), _posY(1), _posZ(1),
                   _dirX(0), _dirY(0), _dirZ(0),
                   _upX(0), _upY(1), _upZ(0)
{
}

void Camera::follow(const Ball& ball) {
    
    const std::array<float,3> position= ball.get3DPos();
    const auto sideBall = ball.currentSide();
    const auto lookingDirection= ball.lookTowards();

    constexpr float distDirPoint = 2.f;
    constexpr float distBehindBall = 1.5f;
    constexpr float distAboveBall = 1.25f;

    switch (sideBall) {
        case JumperBallTypes::Direction::North:
            _upX = 0.f; _upY = 0.f; _upZ = -1.f;
            break;
        case JumperBallTypes::Direction::South:
            _upX = 0.f; _upY = 0.f; _upZ = 1.f;
            break;
        case JumperBallTypes::Direction::East:
            _upX = 1.f; _upY = 0.f; _upZ = 0.f;
            break;
        case JumperBallTypes::Direction::West:
            _upX = -1.f; _upY = 0.f; _upZ = 0.f;
            break;
        case JumperBallTypes::Direction::Up:
            _upX = 0.f; _upY = 1.f; _upZ = 0.f;
            break;
        case JumperBallTypes::Direction::Down:
            _upX = 0.f; _upY = -1.f; _upZ = 0.f;
            break;
        default :
            break;
    }
    
    switch (lookingDirection) {
        case JumperBallTypes::Direction::North:
            _posX = position.at(0) + distAboveBall * _upX; 
            _posY = position.at(1) + distAboveBall * _upY;
            _posZ = position.at(2) + distBehindBall + distAboveBall * _upZ;
            _dirX = position.at(0); 
            _dirY = position.at(1);
            _dirZ = position.at(2) - distDirPoint;
            break;
        case JumperBallTypes::Direction::South:
            _posX = position.at(0) + distAboveBall * _upX; 
            _posY = position.at(1) + distAboveBall * _upY;
            _posZ = position.at(2) - distBehindBall + distAboveBall * _upZ;
            _dirX = position.at(0); 
            _dirY = position.at(1);
            _dirZ = position.at(2) + distDirPoint;
            break;
        case JumperBallTypes::Direction::East:
            _posX = position.at(0) - distBehindBall + distAboveBall * _upX; 
            _posY = position.at(1) + distAboveBall * _upY;
            _posZ = position.at(2) + distAboveBall * _upZ;
            _dirX = position.at(0) + distDirPoint; 
            _dirY = position.at(1);
            _dirZ = position.at(2);
            break;
        case JumperBallTypes::Direction::West:
            _posX = position.at(0) + distBehindBall + distAboveBall * _upX; 
            _posY = position.at(1) + distAboveBall * _upY;
            _posZ = position.at(2) + distAboveBall * _upZ;
            _dirX = position.at(0) - distDirPoint; 
            _dirY = position.at(1);
            _dirZ = position.at(2);
            break;
        case JumperBallTypes::Direction::Up:
            _posX = position.at(0) + distAboveBall * _upX; 
            _posY = position.at(1) - distBehindBall + distAboveBall * _upY;
            _posZ = position.at(2) + distAboveBall * _upZ;
            _dirX = position.at(0); 
            _dirY = position.at(1) + distDirPoint;
            _dirZ = position.at(2);
            break;
        case JumperBallTypes::Direction::Down:
            _posX = position.at(0) + distAboveBall * _upX; 
            _posY = position.at(1) + distBehindBall + distAboveBall * _upY;
            _posZ = position.at(2) + distAboveBall * _upZ;
            _dirX = position.at(0); 
            _dirY = position.at(1) - distDirPoint;
            _dirZ = position.at(2);
            break;
        default :
            break;
    }

}

/*void Camera::follow(const Map& map) {
  (void)map;
}*/
/*Camera::Camera(const Camera& orig) {
}*/

std::array<float, 3> Camera::dir() const {
    return std::array<float,3> {_dirX,_dirY,_dirZ};
}

std::array<float, 3> Camera::pos() const {
    return std::array<float,3> {_posX,_posY,_posZ};
}

std::array<float, 3> Camera::up() const {
    return std::array<float,3> {_upX,_upY,_upZ};
}

Camera::~Camera() {
}

