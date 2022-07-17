/*
 * File: MessageLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */
#include "MessageLabel.h"

MessageLabel::MessageLabel(
    const std::string &message,
    const CstNode_sptr &node,
    bool isActivated
) : Label(node, isActivated),
    _message(message) {
}

std::string MessageLabel::message() const {
    return _message;
}

