//
// Created by S.Morgenthaler on 22/09/22.
//

#include "TextNode.h"

TextNode::TextNode(Node_sptr node, std::string &&text, short id) :
    _node(std::move(node)),
    _text(std::move(text)),
    _id(id) {
}

const Node_sptr &TextNode::node() const {
    return _node;
}

const std::string &TextNode::text() const {
    return _text;
}

short TextNode::getId() const {
    return _id;
}
