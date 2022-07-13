//
// Created by S.Morgenthaler on 13/07/22.
//

#include "Node.h"


Node::Node(CstNode_sptr &parent, const Node::LocalProperties &localProperties) :
    _parent(parent),
    _localProperties(localProperties),
    _children{} {

}

void Node::setChildren(vecCstNode_wptr &&children) {
    _children = children;
}

const Node::LocalProperties &Node::getLocalProperties() const {
    return _localProperties;
}


