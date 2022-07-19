//
// Created by S.Morgenthaler on 13/07/22.
//

#include "Node.h"


Node::Node(const CstNode_sptr &parent, const Node::Transform &transform, float ratio) :
    _parent(parent),
    _height(parent == nullptr ? 0 : (parent->height() + 1)),
    _localTransform(transform),
    _ratio(ratio) {
}

const Node::Transform &Node::getLocalTransform() const {
    return _localTransform;
}

float Node::ratio() const {
    return _ratio;
}

JBTypes::vec2f Node::computeChildNodeSize(float parentRatio, float childRatio) {
    if (childRatio < parentRatio) {
        return {childRatio / parentRatio, 1.f};
    }
    return {1.f, parentRatio / childRatio};
}

void Node::updateScreenTransform() {
    _parent == nullptr
    ? updateScreenTransform(getIdentityTransform())
    : updateScreenTransform(_parent->getScreenTransform());
}

void Node::updateScreenTransform(const Node::Transform &parentTransform) {

    const auto additionLocalTransform = getAdditionalLocalTransform();
    const Node::Transform &localTransform = additionLocalTransform != nullptr ? Node::Transform{
        _localTransform.width * additionLocalTransform->width,
        _localTransform.height * additionLocalTransform->height,
        _localTransform.positionX + additionLocalTransform->positionX,
        _localTransform.positionY + additionLocalTransform->positionY,
    } : _localTransform;

    std::unique_ptr<Node::Transform> screenTransform(new Node::Transform(
        {
            parentTransform.width * localTransform.width,
            parentTransform.height * localTransform.height,
            parentTransform.positionX + parentTransform.width * localTransform.positionX,
            parentTransform.positionY + parentTransform.height * localTransform.positionY
        }
    ));

    _screenTransform = std::move(screenTransform);
}

std::unique_ptr<Node::Transform> Node::getAdditionalLocalTransform() {
    return nullptr;
}

Node::Transform Node::getIdentityTransform() {
    return {1.f, 1.f, 0.f, 0.f};
}

bool Node::intersect(float screenX, float screenY) {
    const auto halfWidth = _screenTransform->width / 2.f;
    const auto halfHeight = _screenTransform->height / 2.f;

    return screenX < _screenTransform->positionX + halfWidth
           && screenX > _screenTransform->positionX - halfWidth
           && screenX < _screenTransform->positionY + halfHeight
           && screenX > _screenTransform->positionY - halfHeight;
}

void Node::updateScreenTransforms(const vecNode_sptr& nodes) {
    auto sortedNodes = nodes;
    std::sort(
        sortedNodes.begin(),
        sortedNodes.end(),
        [](const Node_sptr &a, const Node_sptr &b) { return a->height() < b->height(); }
    );
    for (const auto &node: sortedNodes) {
        node->updateScreenTransform();
    }
}

float Node::getScreenSpaceHeight() const {
    return _screenTransform->height;
}

float Node::getScreenSpaceWidth() const {
    return _screenTransform->width;
}

const Node::Transform &Node::getScreenTransform() const {
    return *_screenTransform;
}

size_t Node::height() const {
    return _height;
}


