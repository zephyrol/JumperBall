//
// Created by S.Morgenthaler on 13/07/22.
//

#ifndef JUMPERBALLAPPLICATION_LEFTNODE_H
#define JUMPERBALLAPPLICATION_LEFTNODE_H


#include "Node.h"

class LeftNode: public Node {
    LeftNode(vecNode_sptr&& children, float ratio);
};


#endif //JUMPERBALLAPPLICATION_LEFTNODE_H
