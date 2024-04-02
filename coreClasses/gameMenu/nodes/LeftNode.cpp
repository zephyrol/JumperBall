//
// Created by S.Morgenthaler on 13/07/22.
//

#include "LeftNode.h"

LeftNode::LeftNode(const CstNode_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const glm::vec2 &localSize) -> glm::vec2 {
        return {(localSize.x * 0.5f) - 0.5f, 0.f};
    }
) {}

