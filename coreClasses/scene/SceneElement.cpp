//
// Created by S.Morgenthaler on 23/08/2023.
//

#include "SceneElement.h"

Displayable::DynamicNames SceneElement::getDynamicQuaternionNames() const {
    return {"rotation"};
}

Displayable::DynamicNames SceneElement::getDynamicVec3fNames() const {
    return {"translation", "scale"};
}

Displayable::DynamicNames SceneElement::getDynamicFloatNames() const {
    return {"burningCoeff, alwaysDisplay"};
}

Displayable::DynamicValues<float> SceneElement::getDynamicFloatValues() const {
    return {0.f, -1.f};
}
