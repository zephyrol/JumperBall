//
// Created by S.Morgenthaler on 22/09/22.
//

#ifndef JUMPERBALLAPPLICATION_TEXTNODE_H
#define JUMPERBALLAPPLICATION_TEXTNODE_H

#include "Node.h"


class TextNode;
using CstTextNode_uptr = std::unique_ptr<const TextNode>;
using vecCstTextNode_uptr = std::vector<CstTextNode_uptr>;

class TextNode {

public:
    explicit TextNode(Node_sptr node, std::string &&text, float id);

    float getId() const;

    const Node_sptr &node() const;

    const std::string &text() const;

private:
    const Node_sptr _node;
    const std::string _text;
    const float _id;
};


#endif //JUMPERBALLAPPLICATION_TEXTNODE_H
