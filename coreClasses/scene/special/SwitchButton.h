/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SwitchButton.h
 * Author: Morgenthaler S
 * 
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __SWITCHBUTTON_H__
#define __SWITCHBUTTON_H__
#include "Special.h" 

class SwitchButton : public Special {
public:
    SwitchButton(
        const JBTypes::Color &color,
        const Block &tieBlock,
        const JBTypes::Dir &dir);

    virtual SpecialEffect interaction() const override;
    
};

#endif // __SWITCHBUTTON_H__