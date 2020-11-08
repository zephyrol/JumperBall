/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Special.h
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
        const JBTypes::Color &color,
        const Block &tieBlock,
        const JBTypes::Dir &dir);

    const JBTypes::Color& getColor() const;
    const JBTypes::timePointMs &creationTime() const;
    const JBTypes::Dir &direction() const;
    virtual SpecialEffect interaction() const = 0;

private:
    const Block& _tieBlock;
    const JBTypes::timePointMs _creationTime;
    const JBTypes::Dir _direction;
    const JBTypes::Color _color;

};


#endif // __SPECIAL_H__