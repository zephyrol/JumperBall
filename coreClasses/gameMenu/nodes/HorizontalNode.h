//
// Created by S.Morgenthaler on 29/04/2023.
//
#ifndef JUMPERBALLAPPLICATION_HORIZONTALNODE_H
#define JUMPERBALLAPPLICATION_HORIZONTALNODE_H
#include "FillingNode.h"


class HorizontalNode: public FillingNode {
public:
    explicit HorizontalNode(const Node_sptr &parent, float ratio, float positionX);

};


#endif //JUMPERBALLAPPLICATION_HORIZONTALNODE_H
