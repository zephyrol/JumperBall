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

Block::Block(bool hasInteraction):
  _localTransform {0.f,0.f,0.f,0.f,0.f,0.f,1.f,1.f,1.f},
  _objects {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
  _hasInteraction(hasInteraction)
{
}

Block::Block(const std::array<float,9>& localTransform, bool hasInteraction):
  _localTransform(localTransform),
  _objects {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
  _hasInteraction(hasInteraction)
{
}

std::array<bool, 6 > Block::faceInfo() const {
    return {false,false,false,false,false,false};
}

bool Block::stillExists() const {
    return true;
}

Block::Effect Block::detectionEvent(const JBTypes::Dir&,
        const JBTypes::timePointMs&)
{
    return Block::Effect::Nothing;
}


const std::array<float, 9>& Block::localTransform() const {
    // 3 values for the translation, 3 for rotation, 3 for the scale
    return _localTransform;
}

Block::Effect Block::interaction(const JBTypes::Dir&,
                        const JBTypes::timePointMs&,
                        const JBTypes::vec3f&,
                        const std::array<unsigned int, 3>&) {
    return Block::Effect::Nothing;
}

void Block::createObject( Object::CategoryOfObjects category, 
                        JBTypes::Dir dir) {
    switch (category) {
        case Object::CategoryOfObjects::Clock : 
            _objects.at(JBTypesMethods::directionAsInteger(dir)) 
                    = std::make_shared<Clock>();
            break;
        case Object::CategoryOfObjects::Coin : 
            _objects.at(JBTypesMethods::directionAsInteger(dir)) 
                    = std::make_shared<Coin>();
            break;
        case Object::CategoryOfObjects::Key : 
            _objects.at(JBTypesMethods::directionAsInteger(dir)) 
                    = std::make_shared<Key>();
            break;
        default :
            break;
    }
}

const std::shared_ptr<const Object> Block::object(size_t number) const
{
    return _objects.at(number);
}

bool Block::hasInteraction() const
{
   return _hasInteraction;
}

void Block::catchObject( const JBTypes::vec3f& blockPosition,
                         const JBTypes::vec3f& entityPosition,
                         float radiusEntity) 
{
    for (size_t i = 0 ; i < _objects.size() ; ++i){
        const std::shared_ptr<Object> object = _objects.at(i);
        const auto dir = JBTypesMethods::directionAsVector(
            JBTypesMethods::integerAsDirection(i));;
        const JBTypes::vec3f objectPosition 
        {blockPosition.x + dir.x, 
         blockPosition.y + dir.y,
         blockPosition.z + dir.z};
        object->catchingTest(objectPosition, entityPosition, radiusEntity);
    }
}

JBTypes::vec3f Block::positionObject(
                 const std::array<unsigned int, 3>& pos, unsigned int dirUint) {

    constexpr float offsetPosition = 1.f; 
    float x = static_cast<float> (pos.at(0)+ 0.5f);
    float y = static_cast<float> (pos.at(1)+ 0.5f);
    float z = static_cast<float> (pos.at(2)+ 0.5f);
    const JBTypes::Dir direction =
                  JBTypesMethods::integerAsDirection(dirUint);

    switch (direction) {
        case JBTypes::Dir::North:
            z -= offsetPosition ;
            break;
        case JBTypes::Dir::South:
            z += offsetPosition ;
            break;
        case JBTypes::Dir::East:
            x += offsetPosition ;
            break;
        case JBTypes::Dir::West:
            x -= offsetPosition ;
            break;
        case JBTypes::Dir::Up:
            y += offsetPosition ;
            break;
        case JBTypes::Dir::Down:
            y -= offsetPosition ;
            break;
        default :
            break;
    }

    return JBTypes::vec3f{x,y,z};

}



