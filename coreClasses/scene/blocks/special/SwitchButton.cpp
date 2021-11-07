/*
 * File: SwitchButton.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "SwitchButton.h"

SwitchButton::SwitchButton(const JBTypes::Color& color,
                           const JBTypes::Dir& dir,
                           const JBTypes::vec3ui& position,
                           const Ball_sptr &ball,
                           bool isActivated
                           ):InteractiveSpecial(color, position, dir, ball, isActivated) {
}

void SwitchButton::applySpecialEffect() const {

}

vecCstShape_sptr SwitchButton::getShapes() const {
    const auto switchButtonShape = std::make_shared<const Shape>(
        Shape::Aspect::Cylinder,
        getColor(),
        std::initializer_list<Transformation>({
            Transformation(Transformation::Type::Translation, { 0.f, -0.5f, 0.f}),
            Transformation(Transformation::Type::Scale, { 0.2f, 0.05f, 0.2f }),
        })
    );

    return { switchButtonShape };
}

