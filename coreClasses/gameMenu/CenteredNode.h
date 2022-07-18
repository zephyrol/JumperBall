//
// Created by S.Morgenthaler on 17/07/2022.
//

#ifndef JUMPERBALLAPPLICATION_CENTEREDNODE_H
#define JUMPERBALLAPPLICATION_CENTEREDNODE_H


#include "FillingNode.h"

class CenteredNode: public FillingNode {
public:
    CenteredNode(const Node_sptr& parent, float ratio);
    virtual ~CenteredNode() = default;
};



#endif //JUMPERBALLAPPLICATION_CENTEREDNODE_H
