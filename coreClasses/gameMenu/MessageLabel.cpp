/*
 * File: MessageLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */
#include "MessageLabel.h"

MessageLabel::MessageLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    const std::string& message,
    const std::shared_ptr <LabelAnswer> action,
    bool activated):
    Label(widthUnit, width, height, position, activated, action),
    _message(message) {
}

std::string MessageLabel::message() const {
    return _message;
}
