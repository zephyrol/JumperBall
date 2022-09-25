/*
 * File: ArrowLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */
#include "ArrowLabel.h"

ArrowLabel::ArrowLabel(
    CstNode_sptr node,
    bool isActivated
) : Label(std::move(node), isActivated) {
}

vecGeometry ArrowLabel::genGeometries() const {

    const Geometry quad(
        Geometry::Shape::Quad,
        transformTranslate({0.4f, 0.f, 0.f}),
        {0.f, 0.f, 0.f},
        transformScale({0.6f, 0.3f, 1.f})
    );

    const Geometry triangle(
        Geometry::Shape::Triangle,
        transformTranslate({-0.6f, 0.f, 0.f}),
        {0.f, 0.f, static_cast <float>(M_PI_2)},
        transformScale({1.f, 0.4f, 1.f})
    );

    return {quad, triangle};
}

SceneElement::StaticValues <JBTypes::vec2f> ArrowLabel::getStaticVec2fValues() const {
    return { { 0.f, 0.f } };
}
