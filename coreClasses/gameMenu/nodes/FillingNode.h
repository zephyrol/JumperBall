//
// Created by S.Morgenthaler on 13/07/22.
//

#ifndef JUMPERBALLAPPLICATION_FILLINGNODE_H
#define JUMPERBALLAPPLICATION_FILLINGNODE_H


#include "Node.h"

class FillingNode : public Node {
public:
    FillingNode(
        const CstNode_sptr &parent,
        float ratio,
        const std::function<glm::vec2(const glm::vec2 &)> &computePositionFromLocalSize
    );

    virtual ~FillingNode() = default;
};


#endif //JUMPERBALLAPPLICATION_FILLINGNODE_H
