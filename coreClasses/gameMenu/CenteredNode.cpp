//
// Created by S.Morgenthaler on 17/07/2022.
//

#include "CenteredNode.h"

CenteredNode::CenteredNode(const CstNode_sptr& parent, float ratio): FillingNode(
    parent,
    ratio,
    [](const JBTypes::vec2f&) -> JBTypes::vec2f {
        return { 0.f, 0.f};
    }
)
{}

