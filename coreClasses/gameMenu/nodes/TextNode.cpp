//
// Created by S.Morgenthaler on 22/09/22.
//

#include "TextNode.h"

TextNode::TextNode(Node_sptr node, std::string &&text, const JBTypes::vec2f &staticProperty) :
    _node(std::move(node)),
    _text(std::move(text)),
    _staticProperty(staticProperty) {

}

const JBTypes::vec2f &TextNode::staticProperty() const {
    return _staticProperty;
}

const Node_sptr &TextNode::node() const {
    return _node;
}

const std::string &TextNode::text() const {
    return _text;
}
