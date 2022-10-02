/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#include "Label.h"

#include <utility>

Label::Label(
    Node_sptr node,
    bool isActivated
) :
    _node(std::move(node)),
    _isActivated(isActivated) {
}

std::string Label::message() const {
    return {};
}

bool Label::isActivated() const {
    return _isActivated;
}

void Label::activate() {
    _isActivated = true;
}

void Label::deactivate() {
    _isActivated = false;
}

void Label::updateLabelsLevels(vecLabel_sptr &labels, size_t end) {
    for (size_t i = 0; i < labels.size(); i++) {
        if (i < end) {
            labels.at(i)->activate();
        } else {
            labels.at(i)->deactivate();
        }
    }
}

vecGeometry Label::genGeometries() const {
    return {};
}

const Node_sptr &Label::getNode() {
    return _node;
}

SceneElement::GlobalState Label::getGlobalState() const {
    return GlobalState::United;
}

Geometry Label::createDisplayableTriangle(
    float localX,
    float localY,
    const JBTypes::Direction &dir,
    float triangleWidth
) const {

    const auto getRotation = [&dir]() -> Geometry::ShapeRotation {
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

    return Geometry(
        Geometry::Shape::Triangle,
        {
            (_node->positionX() + localX * _node->width()) * 2.f,
            (_node->positionY() + localY * _node->height()) * 2.f,
            0.f
        },
        getRotation(),
        {_node->width() * triangleWidth, _node->height(), 1.f}
    );
}

Geometry Label::createDisplayableQuad(
    float localX,
    float localY,
    float quadScaleX,
    float quadScaleY
) const {
    return Geometry(
        Geometry::Shape::Quad,
        {
            (_node->positionX() + localX * _node->width()) * 2.f,
            (_node->positionY() + localY * _node->height()) * 2.f,
            0.f
        },
        { 0.f, 0.f, 0.f },
        {_node->width() * quadScaleX, _node->height() * quadScaleY, 1.f}
    );
}


