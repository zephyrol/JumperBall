
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
        const JBTypes::Dir &dir,
        const JBTypes::vec3ui& position
        );

    virtual Special::SpecialEffect getEffect() const override;
    
};

#endif // __TELEPORTER_H__