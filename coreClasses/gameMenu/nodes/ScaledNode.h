//
// Created by S.Morgenthaler on 16/07/2022.
//

#ifndef JUMPERBALLAPPLICATION_SCALEDNODE_H
#define JUMPERBALLAPPLICATION_SCALEDNODE_H

#include "Node.h"

class ScaledNode : public Node {
public:
    ScaledNode(
        const CstNode_sptr &parent,
        float scale
    );
};


#endif //JUMPERBALLAPPLICATION_SCALEDNODE_H
