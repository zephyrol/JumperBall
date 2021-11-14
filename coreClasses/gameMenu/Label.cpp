/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#include <algorithm>
#include <cmath>
#include <functional>
#include "Label.h"

Label::Label(
    const Label::WidthUnit& widthUnit,
    const std::function<Label::LabelDimensions(float)>& updateLabelSizesFct,
    float ratio,
    bool activated,
    const std::shared_ptr <LabelAnswer>& action,
    bool fixed):
    _widthUnit(widthUnit),
    _updateLabelSizes(updateLabelSizesFct),
    _dimensions(_updateLabelSizes(ratio)),
    _children{},
    _fixed(fixed),
    _activated(activated),
    _action(action) {
}

float Label::width() const {
    return _dimensions.width;
}

float Label::height() const {
    return _dimensions.height;
}

const vecCstLabel_sptr& Label::children() const {
    return _children;
}

float Label::positionX() const {
    return _dimensions.positionX;
}

float Label::positionY() const {
    return _dimensions.positionY;
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

void Label::resize(float screenRatio) {
    _dimensions = _updateLabelSizes(screenRatio);
}
