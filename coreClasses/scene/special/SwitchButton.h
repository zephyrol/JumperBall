
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
        const JBTypes::Dir &dir,
        const JBTypes::vec3ui& position
        );

    virtual Special::SpecialEffect getEffect() const override;
    
};

#endif // __SWITCHBUTTON_H__