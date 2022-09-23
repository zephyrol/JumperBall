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
    explicit TextNode(
        Node_sptr node,
        std::string &&text,
        const JBTypes::vec2f &staticProperty = {0.f, 0.f}
    );

    const JBTypes::vec2f &staticProperty() const;

    const Node_sptr &node() const;

    const std::string &text() const;

private:
    const Node_sptr _node;
    const std::string _text;
    const JBTypes::vec2f _staticProperty;
};


#endif //JUMPERBALLAPPLICATION_TEXTNODE_H
