/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Block.cpp
 * Author: Morgenthaler S
 * 
 * Created on 1 octobre 2019, 21:18
 */

#include "Block.h"

Block::Block():
  _localTransform {0.f,0.f,0.f,0.f,0.f,0.f,1.f,1.f,1.f},
  _objects {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr}
{
}

Block::Block(const std::array<float,9>& localTransform):
  _localTransform(localTransform),
  _objects {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr}
{
}

std::array<bool, 6 > Block::faceInfo() const {
    return {false,false,false,false,false,false};
}

bool Block::stillExists() const {
    return true;
}

void Block::detectionEvent(const JumperBallTypes::Direction& ballDir, 
        const JumperBallTypes::timePointMs& currentTime)
{
    //To remove warning... We want to do nothing.
    //Others classes may override the method.
    static_cast<void> (ballDir);
    static_cast<void> (currentTime);
}


const std::array<float, 9>& Block::localTransform() const {
    // 3 values for the translation, 3 for rotation, 3 for the scale
    return _localTransform;

}

bool Block::burstBall() const {
    return false;
}



void Block::interaction(const JumperBallTypes::Direction& ballDir, 
                        const JumperBallTypes::timePointMs& currentTime, 
                        const JumperBallTypes::vec3f& posBall, 
                        const std::array<unsigned int, 3>& posBlock) {

    //To remove warning... We want to do nothing.
    //Others classes may override the method.
    static_cast<void> (ballDir);
    static_cast<void> (currentTime);
    static_cast<void> (posBall);
    static_cast<void> (posBlock);
}

void Block::createObject( Object::CategoryOfObjects category, 
                        JumperBallTypes::Direction dir) {
    switch (category) {
        case Object::CategoryOfObjects::Clock : 
            _objects.at(JumperBallTypesMethods::directionAsInteger(dir)) 
                    = std::make_shared<Clock>();
            break;
        case Object::CategoryOfObjects::Coin : 
            _objects.at(JumperBallTypesMethods::directionAsInteger(dir)) 
                    = std::make_shared<Coin>();
            break;
        case Object::CategoryOfObjects::Key : 
            _objects.at(JumperBallTypesMethods::directionAsInteger(dir)) 
                    = std::make_shared<Key>();
            break;
        default :
            break;
    }
}

const std::array<std::shared_ptr<const Object>,6 >& Block::objects() const {
    return _objects;
}

JumperBallTypes::vec3f Block::positionObject(
                 const std::array<unsigned int, 3>& pos, unsigned int dirUint) {

    constexpr float offsetPosition = 1.f; 
    float x = static_cast<float> (pos.at(0)+ 0.5f);
    float y = static_cast<float> (pos.at(1)+ 0.5f);
    float z = static_cast<float> (pos.at(2)+ 0.5f);
    const JumperBallTypes::Direction direction =
                  JumperBallTypesMethods::integerAsDirection(dirUint);

    switch (direction) {
        case JumperBallTypes::Direction::North:
            z -= offsetPosition ;
            break;
        case JumperBallTypes::Direction::South:
            z += offsetPosition ;
            break;
        case JumperBallTypes::Direction::East:
            x += offsetPosition ;
            break;
        case JumperBallTypes::Direction::West:
            x -= offsetPosition ;
            break;
        case JumperBallTypes::Direction::Up:
            y += offsetPosition ;
            break;
        case JumperBallTypes::Direction::Down:
            y -= offsetPosition ;
            break;
        default :
            break;
    }

    return JumperBallTypes::vec3f{x,y,z};

}



