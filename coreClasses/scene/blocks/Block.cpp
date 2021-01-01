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

Block::Block(bool hasInteraction, bool isFixed):
  _localTransform {0.f,0.f,0.f,1.f,1.f,1.f},
  _objects {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr},
  _hasInteraction(hasInteraction),
  _hasObjects(false),
  _isFixed(isFixed)
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


const std::array<float, 6>& Block::localTransform() const {
    // 3 values for the translation, 3 for rotation, 3 for the scale
    return _localTransform;
}

Block::Effect Block::interaction(const JBTypes::Dir&,
                        const JBTypes::timePointMs&,
                        const JBTypes::vec3f&,
                        const JBTypes::vec3ui&) {
    return Block::Effect::Nothing;
}

void Block::createObject( Object::CategoryOfObjects category, 
                        JBTypes::Dir dir) {
    _hasObjects = true;
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

bool Block::hasObjects() const
{
   return _hasObjects;
}

void Block::catchObject( const JBTypes::vec3ui& blockPosition,
                         const JBTypes::vec3f& entityPosition,
                         float radiusEntity) 
{
    for (size_t i = 0; i < _objects.size(); ++i){
        const std::shared_ptr<Object> object = _objects.at(i);
        if (object && !object->isGotten()) {
            const JBTypes::vec3f objectPos =
                objectPosition(blockPosition,static_cast<unsigned int>(i));
            object->catchingTest(objectPos, entityPosition, radiusEntity);
        }
    }
}

const bool& Block::isFixed() const
{
   return _isFixed;
}

JBTypes::vec3f Block::objectPosition(
                 const JBTypes::vec3ui& pos, unsigned int dirUint) {

    constexpr float offsetPosition = 1.f; 
    float x = static_cast<float> (pos.at(0)+ 0.5f);
    float y = static_cast<float> (pos.at(1)+ 0.5f);
    float z = static_cast<float> (pos.at(2)+ 0.5f);
    const JBTypes::Dir direction = JBTypesMethods::integerAsDirection(dirUint);

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

const std::array<std::shared_ptr<Object>, 6> &Block::objects() const
{
    return _objects;
}



