//
// Created by S.Morgenthaler on 13/07/22.
//

#include "Node.h"

Node::Node(const Node::Transform &transform, float ratio) :
    _transform(transform),
    _ratio(ratio) {
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

std::unique_ptr<float> Node::intersect(float mouseX, float mouseY, float touchThreshold) const {
    const auto halfWidth = _transform.width / 2.f;
    const auto halfHeight = _transform.height / 2.f;

    const auto widthThreshold = halfWidth * touchThreshold;
    const auto heightThreshold = halfHeight * touchThreshold;

    if (mouseX < _transform.positionX + widthThreshold
        && mouseX > _transform.positionX - widthThreshold
        && mouseY < _transform.positionY + heightThreshold
        && mouseY > _transform.positionY - heightThreshold) {
        const auto mouseToPositionX = _transform.positionX - mouseX;
        const auto mouseToPositionY = _transform.positionY - mouseY;

        return std::unique_ptr<float>(
            new float(mouseToPositionX * mouseToPositionX + mouseToPositionY * mouseToPositionY)
        );
    }
    return nullptr;
}

Node_sptr Node::getNearest(vecNode_sptr nodes, float mouseX, float mouseY) {
    return std::accumulate(
        nodes.begin(),
        nodes.end(),
        std::pair<float, Node_sptr>(0.f, nullptr),
        [mouseX, mouseY](const std::pair<float, Node_sptr> &current, const Node_sptr &node) {
            const auto intersectDistance = node->intersect(mouseX, mouseY, 2.f);
            if(intersectDistance == nullptr) {
                return current;
            }
            return current.second == nullptr || *intersectDistance < current.first
                ? std::pair<float, Node_sptr>(*intersectDistance, node)
                : current;
        }
    ).second;
}
