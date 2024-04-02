//
// Created by S.Morgenthaler on 29/04/2023.
//

#include "HorizontalNode.h"

HorizontalNode::HorizontalNode(const Node_sptr &parent, float ratio, float positionX) : FillingNode(
    parent,
    ratio,
    [&positionX](const glm::vec2 &localSize) -> glm::vec2 {
        return {(localSize.x / 2.f) - 0.5f + positionX * (1.f - localSize.x), 0.f };
    }) {

}
