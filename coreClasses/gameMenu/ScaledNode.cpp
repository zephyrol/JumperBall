//
// Created by Sebastien Morgenthaler on 16/07/2022.
//

#include "ScaledNode.h"

ScaledNode::ScaledNode(const CstNode_sptr &parent, float scale) : Node(
    [parent, scale]() {
        const auto parentTransform = parent->getTransform();
        const decltype(parentTransform) transform{
            parentTransform.width * scale,
            parentTransform.height * scale,
            parentTransform.positionX,
            parentTransform.positionY
        };
        return transform;
    }(),
    parent->ratio()
) {
}
