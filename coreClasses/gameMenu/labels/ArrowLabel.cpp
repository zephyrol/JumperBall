/*
 * File: ArrowLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */
#include "ArrowLabel.h"

ArrowLabel::ArrowLabel(Node_sptr node, const JBTypes::Color &color) :
    Label(std::move(node), color) {
}

vecLabelGeometry ArrowLabel::genGeometries() const {

    return {
        createDisplayableQuad(0.25f, 0.f, 0.5f, 0.3f),
        createDisplayableTriangle(0.f, 0.f, 0.5f, 0.6f, JBTypes::Direction::West),
    };
}

SceneElement::StaticValues<JBTypes::vec2f> ArrowLabel::getStaticVec2fValues() const {
    return {{0.f, 0.f}};
}
