//
// Created by S.Morgenthaler on 17/07/2022.
//

#include "CenteredNode.h"

CenteredNode::CenteredNode(const CstNode_sptr& parent, float ratio): FillingNode(
    parent,
    ratio,
    [](const glm::vec2&) -> glm::vec2 {
        return { 0.f, 0.f};
    }
)
{}

