//
// Created by sebastien on 13/07/22.
//

#include "FillingNode.h"

FillingNode::FillingNode(CstNode_sptr &parent, float ratio) : Node(
    parent,
    [&ratio, &parent]() {
        const auto &parentProperties = parent->getLocalProperties();

        Node::LocalProperties localProperties;

        return localProperties;
    }()
) {
}

Node::LocalProperties FillingNode::computeLocalProperties(const Node *parent) {
    return Node::LocalProperties();
}
