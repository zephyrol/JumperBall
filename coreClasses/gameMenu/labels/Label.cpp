/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#include "Label.h"

#include <utility>

Label::Label(Node_sptr node, const JBTypes::Color &color) :
    _node(std::move(node)),
    _color(color) {
}

std::string Label::message() const {
    return {};
}

vecLabelGeometry Label::genGeometries() const {
    return {};
}

const JBTypes::Color& Label::color() const {
    return _color;
}

const Node_sptr &Label::getNode() {
    return _node;
}

Displayable::GlobalState Label::getGlobalState() const {
    return GlobalState::United;
}

LabelGeometry Label::createDisplayableTriangle(
    float localX,
    float localY,
    float triangleScaleX,
    float triangleScaleY,
    const JBTypes::Direction &dir
) const {

    const auto getRotation = [&dir]() -> LabelGeometry::ShapeRotation {
        if (dir == JBTypes::Direction::East) {
            return {0.f, 0.f, static_cast<float>(-M_PI_2)};
        }
        if (dir == JBTypes::Direction::West) {
            return {0.f, 0.f, static_cast<float>(M_PI_2)};
        }
        if (dir == JBTypes::Direction::South) {
            return {0.f, 0.f, static_cast<float>(M_PI)};
        }
        return {0.f, 0.f, 0.f};
    };

    return LabelGeometry(
        LabelGeometry::Shape::Triangle,
        {
            (_node->positionX() + localX * _node->width()) * 2.f,
            (_node->positionY() + localY * _node->height()) * 2.f,
            0.f
        },
        getRotation(),
        {_node->width() * triangleScaleX, _node->height() * triangleScaleY, 1.f},
        _color
    );
}

LabelGeometry Label::createDisplayableQuad(
    float localX,
    float localY,
    float quadScaleX,
    float quadScaleY
) const {
    return LabelGeometry(
        LabelGeometry::Shape::Quad,
        {
            (_node->positionX() + localX * _node->width()) * 2.f,
            (_node->positionY() + localY * _node->height()) * 2.f,
            0.f
        },
        {0.f, 0.f, 0.f},
        {_node->width() * quadScaleX, _node->height() * quadScaleY, 1.f},
        _color
    );
}


