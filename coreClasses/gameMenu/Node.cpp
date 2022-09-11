//
// Created by S.Morgenthaler on 13/07/22.
//

#include "Node.h"

Node::Node(const Node::Transform &transform, float ratio) :
    _transform(transform),
    _ratio(ratio)
{
}

const Node::Transform &Node::getTransform() const {
    return _transform;
}

float Node::ratio() const {
    return _ratio;
}

JBTypes::vec2f Node::computeNodeSize(float parentRatio, float childRatio) {
    if (childRatio < parentRatio) {
        return {childRatio / parentRatio, 1.f};
    }
    return {1.f, parentRatio / childRatio};
}

std::unique_ptr<Node::Transform> Node::getAdditionalLocalTransform() const {
    return nullptr;
}

Node::Transform Node::getIdentityTransform() {
    return {1.f, 1.f, 0.f, 0.f};
}

float Node::width() const {
    return _transform.width;
}

float Node::height() const {
    return _transform.height;
}

float Node::positionX() const {
    return _transform.positionX;
}

float Node::positionY() const {
    return _transform.positionY;
}

bool Node::intersect(float mouseX, float mouseY) const {
     const auto halfWidth = _transform.width / 2.f;
     const auto halfHeight = _transform.height / 2.f;

     return mouseX < _transform.positionX + halfWidth
            && mouseX > _transform.positionX - halfWidth
            && mouseY < _transform.positionY + halfHeight
            && mouseY > _transform.positionY - halfHeight;
}
