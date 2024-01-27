/*
 * File: ArrowLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */
#include "ArrowLabel.h"

ArrowLabel::ArrowLabel(
    Node_sptr node,
    const JBTypes::Color &color,
    const JBTypes::Direction &direction,
    short id
) :
    Label(std::move(node), color, id), _direction(direction) {
}

vecLabelGeometry ArrowLabel::genGeometries() const {

    constexpr auto quadOffset = 0.25f;
    constexpr auto quadWidth = 0.5f;
    constexpr auto quadHeight = 0.3f;

    constexpr auto triangleWidth = 0.5f;
    constexpr auto triangleHeight = 0.6f;
    if (_direction == JBTypes::Direction::West) {
        return {
            createDisplayableQuad(quadOffset, 0.f, quadWidth, quadHeight),
            createDisplayableTriangle(0.f, 0.f, triangleWidth, triangleHeight, _direction),
        };
    }
    if (_direction == JBTypes::Direction::East) {
        return {
            createDisplayableQuad(-quadOffset, 0.f, quadWidth, quadHeight),
            createDisplayableTriangle(0.f, 0.f, triangleWidth, triangleHeight, _direction),
        };
    }
    if (_direction == JBTypes::Direction::North) {
        return {
            createDisplayableQuad(0.f, quadOffset, quadHeight, quadWidth),
            createDisplayableTriangle(0.f, 0.f, triangleHeight, triangleWidth, _direction),
        };
    }
    return {
        createDisplayableQuad(0.f, -quadOffset, quadHeight, quadWidth),
        createDisplayableTriangle(0.f, 0.f, triangleHeight, triangleWidth, _direction),
    };
}
