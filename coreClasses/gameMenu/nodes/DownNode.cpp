//
// Created by S.Morgenthaler on 10/09/2022.
//

#include "DownNode.h"

DownNode::DownNode(const Node_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const JBTypes::vec2f &localSize) -> JBTypes::vec2f {
        return {0.f, (localSize.y * 0.5f) - 0.5f};
    }) {

}
