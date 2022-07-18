//
// Created by S.Morgenthaler on 13/07/22.
//

#include "Node.h"


Node::Node(const Node_sptr &parent, const Node::Transform &transform, float ratio) :
    _parent(parent),
    _localTransform(transform),
    _ratio(ratio),
    _children{} {
    parent->addChild(this);
}

const Node::Transform &Node::getLocalTransform() const {
    return _localTransform;
}

void Node::addChild(Node_rptr child) {
    _children.push_back(child);
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
    : updateScreenTransform(_parent->getScreenTranform());
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
            parentTransform.width * _localTransform.width,
            parentTransform.height * _localTransform.height,
            parentTransform.positionX + parentTransform.width * _localTransform.positionX,
            parentTransform.positionY + parentTransform.height * _localTransform.positionY
        }
    ));

    _screenTransform = std::move(screenTransform);
    for (const auto &child: _children) {
        child->updateScreenTransform(*_screenTransform);
    }
}

std::unique_ptr<Node::Transform> Node::getAdditionalLocalTransform() {
    return nullptr;
}

Node::Transform Node::getIdentityTransform() {
    return {1.f, 1.f, 0.f, 0.f};
}

CstNode_rptr Node::getClickNode(float screenX, float screenY) {
    const auto halfWidth = _screenTransform->width / 2.f;
    const auto halfHeight = _screenTransform->height/ 2.f;

    if (!(screenX < _screenTransform->positionX + halfWidth
        && screenX > _screenTransform->positionX - halfWidth
        && screenX < _screenTransform->positionY + halfHeight
        && screenX > _screenTransform->positionY - halfHeight)) {
        return nullptr;
    }
    for (const auto &child: _children) {
        const auto& childResult = child->getClickNode(screenX, screenY);
        if(childResult != nullptr) {
            return childResult;
        }
    }
    return this;
}

float Node::getScreenSpaceHeight() const {
    return _screenTransform->height;
}

float Node::getScreenSpaceWidth() const {
    return _screenTransform->width;
}

const Node::Transform &Node::getScreenTranform() const {
    return *_screenTransform;
}


