//
// Created by S.Morgenthaler on 13/07/22.
//

#include "FillingNode.h"

FillingNode::FillingNode(
    const CstNode_sptr &parent,
    float ratio,
    const std::function<JBTypes::vec2f(const JBTypes::vec2f &)> &computePositionFromLocalSize
) : Node(
    [&ratio, &parent, &computePositionFromLocalSize]() -> Transform {
        const auto localSize = Node::computeNodeSize(
            parent->ratio(),
            ratio
        );
        const auto localPosition = computePositionFromLocalSize(localSize);

        const decltype(localSize) screenSize = {
            localSize.x * parent->width(),
            localSize.y * parent->height()
        };

        const decltype(localPosition) screenPosition = {
            parent->positionX() + parent->width() * localPosition.x,
            parent->positionY() + parent->height() * localPosition.y
        };
        return {screenSize.x, screenSize.y, screenPosition.x, screenPosition.y};
    }(),
    ratio
) {
}
