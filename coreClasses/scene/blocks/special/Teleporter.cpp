/*
 * File: Teleporter.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "Teleporter.h"

Teleporter::Teleporter(
    const JBTypes::Color &color,
    const JBTypes::Dir &dir,
    const JBTypes::vec3ui &position,
    const Ball_sptr &ball,
    bool isActivated
) :
    InteractiveSpecial(color, position, dir, ball, isActivated) {
}

void Teleporter::applySpecialEffect() {
    if (!isActivated()) {
        return;
    }
    const auto &ball = _ball.lock();
    if (ball->currentSide() == direction()) {
        ball->teleport(this->getColor());
    }
}

vecCstShape_sptr Teleporter::getShapes() const {
    const auto teleporterShape = std::make_shared<const Shape>(
        Shape::Aspect::Pedestal,
        getColor(),
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale, {0.7f, 0.05f, 0.7f}),
                Transformation(
                    Transformation::Type::Rotation,
                    JBTypesMethods::rotationVectorUpToDir(direction())
                )
            }
        )
    );

    return {teleporterShape};
}

Displayable::DynamicValues<JBTypes::vec3f> Teleporter::getDynamicVec3fValues() const {
    return {
        positionF(),
        {1.f, 1.f, 1.f}
    };
}

Displayable::DynamicValues<JBTypes::Quaternion> Teleporter::getDynamicQuaternionValues() const {
    constexpr auto speedFactor = 5.f;
    if (!isActivated()) {
        return {{{0.f, 0.f, 0.f}, 1.f}};
    }
    return {{JBTypesMethods::createRotationQuaternion(_directionVec, speedFactor * _chronometer->getTime())}};
}

