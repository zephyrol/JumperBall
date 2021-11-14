/*
 * File: ArrowLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */
#include <functional>
#include "ArrowLabel.h"

ArrowLabel::ArrowLabel(
    const Label::WidthUnit& widthUnit,
    const std::function<Label::LabelDimensions(float)>& updateLabelSizesFct,
    float ratio,
    const std::shared_ptr <LabelAnswer>& action,
    bool activated):
    Label(widthUnit, updateLabelSizesFct, ratio, activated, action) {
}

vecGeometry ArrowLabel::genGeometries() const {

    const Geometry quad(
        Geometry::Shape::Quad,
        { 0.4f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.6f, 0.5f, 1.f });

    const Geometry triangle(
        Geometry::Shape::Triangle,
        { -0.6f, 0.f, 0.f },
        { 0.f, 0.f, static_cast <float>(M_PI_2) },
        { 1.f, 0.4f, 1.f });

    return { quad, triangle };

}