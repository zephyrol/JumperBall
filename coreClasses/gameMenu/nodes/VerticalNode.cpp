//
// Created by S.Morgenthaler on 18/09/2022.
//

#include "VerticalNode.h"

VerticalNode::VerticalNode(const Node_sptr &parent, float ratio, float positionY) : FillingNode(
    parent,
    ratio,
    [&positionY](const glm::vec2 &localSize) -> glm::vec2 {
        return {0.f, (localSize.y / 2.f) - 0.5f + positionY * (1.f - localSize.y)};
    }) {

}
