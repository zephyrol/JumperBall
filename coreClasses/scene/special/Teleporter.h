/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Teleporter.h
 * Author: Morgenthaler S
 * 
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __TELEPORTER_H__
#define __TELEPORTER_H__
#include "Special.h"

class Teleporter : public Special {
public:
    Teleporter(
        const JBTypes::Color &color,
        const Block &tieBlock,
        const JBTypes::Dir &dir);

    virtual Special::SpecialEffect interaction() const override;
    

};

#endif // __TELEPORTER_H__