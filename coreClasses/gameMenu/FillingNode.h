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
        const std::function<JBTypes::vec2f(const JBTypes::vec2f &)> &computePositionFromLocalSize
    );

    virtual ~FillingNode() = default;
};


#endif //JUMPERBALLAPPLICATION_FILLINGNODE_H
