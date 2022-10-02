/*
 * File: ArrowLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */
#include "ArrowLabel.h"

ArrowLabel::ArrowLabel(
    Node_sptr node,
    bool isActivated
) : Label(std::move(node), isActivated) {
}

vecGeometry ArrowLabel::genGeometries() const {

    return {
        createDisplayableQuad(0.25f, 0.f, 0.5f, 0.3f),
        createDisplayableTriangle(0.f, 0.f, JBTypes::Direction::West, 0.5f),
    };
}

SceneElement::StaticValues<JBTypes::vec2f> ArrowLabel::getStaticVec2fValues() const {
    return {{0.f, 0.f}};
}
