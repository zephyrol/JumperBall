//
// Created by S.Morgenthaler on 13/07/22.
//

#ifndef JUMPERBALLAPPLICATION_FILLINGNODE_H
#define JUMPERBALLAPPLICATION_FILLINGNODE_H


#include "Node.h"

class FillingNode: public Node {

    FillingNode(CstNode_sptr &parent, float ratio);

    Node::LocalProperties computeLocalProperties(const Node* parent) override;
};


#endif //JUMPERBALLAPPLICATION_FILLINGNODE_H
