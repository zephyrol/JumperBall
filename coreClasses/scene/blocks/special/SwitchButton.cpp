/*
 * File: SwitchButton.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "SwitchButton.h"

SwitchButton::SwitchButton(
    const JBTypes::Color &color,
    const JBTypes::Dir &dir,
    const glm::u32vec3 &position,
    const Ball_sptr &ball,
    bool isActivated
) : InteractiveSpecial(color, position, dir, ball, isActivated) {
}

void SwitchButton::applySpecialEffect() {
    const auto &ball = _ball.lock();
    for (const auto &block: *ball->getBlocksWithInteraction()) {
        for (const auto &special: block->getSpecials()) {
            if (
                special->getColor() == getColor() ||
                special->getColor() == JBTypesMethods::colorToShiny(getColor())
                ) {
                special->switchOnOff();
            }
        }
        for (const auto &enemy: block->getEnemies()) {
            if (
                enemy->getColor() == getColor() ||
                enemy->getColor() == JBTypesMethods::colorToShiny(getColor())
                ) {
                enemy->switchOnOff();
            }
        }
    }
}

vecCstShape_sptr SwitchButton::getShapes() const {
    const auto switchButtonShape = std::make_shared<const Shape>(
        Shape::Aspect::Cylinder,
        getColor(),
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale, {0.4f, 0.075f, 0.4f}),
                Transformation(
                    Transformation::Type::Rotation,
                    JBTypesMethods::rotationVectorUpToDir(direction())
                )
            }
        )
    );

    return {switchButtonShape};
}

Displayable::DynamicValues<glm::vec3> SwitchButton::getDynamicVec3fValues() const {
    return {
        positionF(),
        isActivated() ? glm::vec3{1.f, 0.1f, 1.f} : glm::vec3{1.f, 1.f, 1.f}
    };
}

Displayable::DynamicValues<glm::quat> SwitchButton::getDynamicQuaternionValues() const {
    return {{{0.f, 0.f, 0.f}, 1.f}};
}

