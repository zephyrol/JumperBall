//
// Created by S.Morgenthaler on 18/09/2022.
//

#include "RightNode.h"

RightNode::RightNode(const CstNode_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const JBTypes::vec2f &localSize) -> JBTypes::vec2f {
        return {0.5f - (localSize.x * 0.5f), 0.f};
    }
) {

}
