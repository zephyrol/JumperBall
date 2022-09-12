/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#include "Label.h"

Label::Label(
    const CstNode_sptr &node,
    bool isActivated
) :
    _node(node),
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
    const auto& screenTransform = _node->getTransform();
    return {
        Geometry(
            Geometry::Shape::Quad,
            {
                // convert to -1,1 space
                screenTransform.positionX,
                screenTransform.positionY,
                0.f
            },
            {
                0.f, 0.f, 0.f
            },
            {
                screenTransform.width,
                screenTransform.height
            }
        )
    };
}

const CstNode_sptr &Label::getNode() const {
    return _node;
}