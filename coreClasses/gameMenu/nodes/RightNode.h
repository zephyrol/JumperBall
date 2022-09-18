//
// Created by S.Morgenthaler on 18/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_RIGHTNODE_H
#define JUMPERBALLAPPLICATION_RIGHTNODE_H

#include "FillingNode.h"


class RightNode : public FillingNode {
public:
    explicit RightNode(const CstNode_sptr &parent, float ratio);
};


#endif //JUMPERBALLAPPLICATION_RIGHTNODE_H
