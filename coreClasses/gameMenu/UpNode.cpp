//
// Created by Sebastien Morgenthaler on 18/07/2022.
//

#include "UpNode.h"

UpNode::UpNode(const Node_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const JBTypes::vec2f &size) -> JBTypes::vec2f {
        return {0.f, size.y + 0.5f};
    }
) {}
