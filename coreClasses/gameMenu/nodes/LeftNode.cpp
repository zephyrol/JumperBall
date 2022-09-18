//
// Created by S.Morgenthaler on 13/07/22.
//

#include "LeftNode.h"

LeftNode::LeftNode(const CstNode_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const JBTypes::vec2f &localSize) -> JBTypes::vec2f {
        return {(localSize.x * 0.5f) - 0.5f, 0.f};
    }
) {}

