//
// Created by S.Morgenthaler on 18/07/2022.
//

#include "UpNode.h"

UpNode::UpNode(const Node_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const glm::vec2 &localSize) -> glm::vec2 {
        return {0.f,  0.5f - (localSize.y * 0.5f) };
    }
) {}
