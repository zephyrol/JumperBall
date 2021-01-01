/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Special.cpp
 * Author: Morgenthaler S
 * 
 * Created on 7 novembre 2020, 10:42
 */
#include "Special.h"

Special::Special(const JBTypes::Color &color,
        const Block &tieBlock,
        const JBTypes::Dir &dir,
        const JBTypes::vec3ui& position,
        bool isActivated
        ):
        _tieBlock(tieBlock),
        _creationTime(JBTypesMethods::getTimePointMSNow()),
        _direction(dir),
        _color(color),
        _position(position),
        _position3D(initPosition(position)),
        _isActivated(isActivated)
{
}

const JBTypes::Color& Special::getColor() const {
    return _color;
}

const JBTypes::timePointMs &Special::creationTime() const {
    return _creationTime;
}

const JBTypes::Dir &Special::direction() const {
    return _direction;
}

const JBTypes::vec3f& Special::position3D() const {
    return _position3D; 
}

const JBTypes::vec3ui& Special::position() const {
    return _position;    
}

JBTypes::vec3f Special::initPosition(const JBTypes::vec3ui& position)
                                                                         const {
    constexpr float sizeBlock = 1.f;
    constexpr float offset = sizeBlock / 2.f;

    const JBTypes::Dir &currentDir = _direction;
    const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(currentDir);

    const JBTypes::vec3f posWorld = 
        { static_cast<float>(position.at(0)),
          static_cast<float>(position.at(1)),
          static_cast<float>(position.at(2)) };

    return {
        posWorld.x + offset + vecDir.x * offset,
        posWorld.y + offset + vecDir.y * offset,
        posWorld.z + offset + vecDir.z * offset 
        };
}
