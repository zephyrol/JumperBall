/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#include <iostream>
#include <algorithm>
#include <math.h>
#include <cctype>
#include "Label.h"

Label::Label(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    bool activated,
    const std::shared_ptr <LabelAnswer>& action,
    bool fixed):
    _widthUnit(widthUnit),
    _width(width),
    _height(height),
    _children{},
    _position(position),
    _fixed(fixed),
    _activated(activated),
    _action(action) {
}

float Label::width() const {
    return _width;
}

float Label::height() const {
    return _height;
}

const vecCstLabel_sptr& Label::children() const {
    return _children;
}

const JBTypes::vec2f& Label::position() const {
    return _position;
}

std::string Label::message() const {
    return {};
}

bool Label::isFixed() const {
    return _fixed;
}

bool Label::isActivated() const {
    return _activated;
}

void Label::activate() {
    _activated = true;
}

void Label::deactivate() {
    _activated = false;
}

void Label::updateLabelsLevels (vecLabel_sptr& labels, size_t end) {
    for (size_t i = 0; i < labels.size(); i++) {
        if (i < end) {
            labels.at(i)->activate();
        } else {
            labels.at(i)->deactivate();
        }
    }
}

const std::shared_ptr <Label::LabelAnswer>& Label::action() const {
    return _action;
}

const Label::WidthUnit& Label::widthUnit() const {
    return _widthUnit;
}

vecGeometry Label::genGeometries() const {
    return {};
}

Label::~Label() {
}
