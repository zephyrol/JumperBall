//
// Created by S.Morgenthaler on 10/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_DOWNNODE_H
#define JUMPERBALLAPPLICATION_DOWNNODE_H


#include "FillingNode.h"

class DownNode : public FillingNode {
public:
    DownNode(const Node_sptr &localSize, float ratio);
};


#endif //JUMPERBALLAPPLICATION_DOWNNODE_H
