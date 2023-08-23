//
// Created by S.Morgenthaler on 23/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_SCENEELEMENT_H
#define JUMPERBALLAPPLICATION_SCENEELEMENT_H


#include "Displayable.h"

class SceneElement : public Displayable {

public:

    DynamicNames getDynamicFloatNames() const override;

    DynamicNames getDynamicQuaternionNames() const override;

    Displayable::DynamicNames getDynamicVec3fNames() const override;
};


#endif //JUMPERBALLAPPLICATION_SCENEELEMENT_H
