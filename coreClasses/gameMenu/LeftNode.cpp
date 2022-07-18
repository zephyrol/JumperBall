//
// Created by S.Morgenthaler on 13/07/22.
//

#include "LeftNode.h"

LeftNode::LeftNode(const Node_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const JBTypes::vec2f &size) -> JBTypes::vec2f {
        return {size.y - 0.5f, 0.f};
    }
) {}

