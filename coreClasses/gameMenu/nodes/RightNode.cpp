//
// Created by S.Morgenthaler on 18/09/2022.
//

#include "RightNode.h"

RightNode::RightNode(const CstNode_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const glm::vec2 &localSize) -> glm::vec2 {
        return {0.5f - (localSize.x * 0.5f), 0.f};
    }
) {

}
