//
// Created by S.Morgenthaler on 10/09/2022.
//

#include "DownNode.h"

DownNode::DownNode(const Node_sptr &parent, float ratio) : FillingNode(
    parent,
    ratio,
    [](const glm::vec2 &localSize) -> glm::vec2 {
        return {0.f, (localSize.y * 0.5f) - 0.5f};
    }) {

}
