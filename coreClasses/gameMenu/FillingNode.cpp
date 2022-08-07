//
// Created by sebastien on 13/07/22.
//

#include "FillingNode.h"

FillingNode::FillingNode(
    const CstNode_sptr &parent,
    float ratio,
    const std::function<JBTypes::vec2f(const JBTypes::vec2f &)> &computePositionFromSize
) : Node(
    parent,
    [&ratio, &parent, &computePositionFromSize]() -> Transform {
        const auto size =
            Node::computeNodeSize(
                parent != nullptr ? parent->ratio() : 1.f,
                ratio
            );
        const auto position = computePositionFromSize(size);
        return {size.x, size.y, position.x, position.y};
    }(),
    ratio
) {
}

