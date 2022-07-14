//
// Created by sebastien on 13/07/22.
//

#include "FillingNode.h"

FillingNode::FillingNode(
    const Node_sptr &parent,
    float ratio,
    const std::function<JBTypes::vec2f(const JBTypes::vec2f &)> &computePositionFromSize
) : Node(
    parent,
    [&ratio, &parent, &computePositionFromSize]() -> Transform {
        const float parentRatio = parent->ratio();
        const auto size = Node::computeChildNodeSize(parentRatio, ratio);
        const auto position = computePositionFromSize(size);
        return {size.x, size.y, position.x, position.y};
    }(),
    ratio
) {
}

