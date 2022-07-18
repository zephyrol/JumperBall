//
// Created by S.Morgenthaler on 18/07/2022.
//

#ifndef JUMPERBALLAPPLICATION_UPNODE_H
#define JUMPERBALLAPPLICATION_UPNODE_H


#include "FillingNode.h"

class UpNode : public FillingNode {
public:
    UpNode(const Node_sptr &parent, float ratio);
};


#endif //JUMPERBALLAPPLICATION_UPNODE_H
