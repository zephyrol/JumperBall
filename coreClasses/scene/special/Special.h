/*
 * File: Special.h
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __SPECIAL_H__
#define __SPECIAL_H__
#include "scene/blocks/Block.h"

class Special {
public:
enum class SpecialEffect { Switch,
                           Teleportation };

Special(
    const JBTypes::Color& color,
    const Block& tieBlock,
    const JBTypes::Dir& dir,
    const JBTypes::vec3ui& position,
    bool isActivated = true
    );

virtual ~Special() = default;

const JBTypes::Color& getColor() const;
const JBTypes::timePointMs& creationTime() const;
const JBTypes::Dir& direction() const;
const JBTypes::vec3f& position3D() const;
const JBTypes::vec3ui& position() const;
bool isActivated() const;
virtual SpecialEffect getEffect() const = 0;

void switchOnOff();

private:
const Block& _tieBlock;
const JBTypes::timePointMs _creationTime;
const JBTypes::Dir _direction;
const JBTypes::Color _color;
const JBTypes::vec3ui _position;
const JBTypes::vec3f _position3D;
bool _isActivated;

JBTypes::vec3f initPosition(const JBTypes::vec3ui& position)
const;
};


#endif // __SPECIAL_H__
