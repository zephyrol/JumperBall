//
// Created by Sebastien Morgenthaler on 16/07/2022.
//

#include "ScaledNode.h"

ScaledNode::ScaledNode(const CstNode_sptr &parent, float scale) : Node(
    {
        scale, scale, 0.f, 0.f
    }
) {
}
