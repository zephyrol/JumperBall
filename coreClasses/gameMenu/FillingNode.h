//
// Created by S.Morgenthaler on 13/07/22.
//

#ifndef JUMPERBALLAPPLICATION_FILLINGNODE_H
#define JUMPERBALLAPPLICATION_FILLINGNODE_H


#include "Node.h"

class FillingNode : public Node {
public:
    FillingNode(
        const Node_sptr &parent,
        float ratio,
        const std::function<JBTypes::vec2f(const JBTypes::vec2f &)> &computePositionFromSize
    );
};


#endif //JUMPERBALLAPPLICATION_FILLINGNODE_H
