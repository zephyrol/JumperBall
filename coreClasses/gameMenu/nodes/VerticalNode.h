//
// Created by S.Morgenthaler on 18/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_VERTICALNODE_H
#define JUMPERBALLAPPLICATION_VERTICALNODE_H
#include "FillingNode.h"


class VerticalNode: public FillingNode {
public:
    explicit VerticalNode(const Node_sptr &parent, float ratio, float positionY);

};


#endif //JUMPERBALLAPPLICATION_VERTICALNODE_H
