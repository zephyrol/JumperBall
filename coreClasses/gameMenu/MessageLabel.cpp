/*
 * File: MessageLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */
#include <functional>
#include "MessageLabel.h"

MessageLabel::MessageLabel(
    const Label::WidthUnit& widthUnit,
    const std::function<Label::LabelDimensions(float)>& updateLabelSizesFct,
    float ratio,
    const std::string& message,
    const std::shared_ptr <LabelAnswer>& action,
    bool activated):
    Label(widthUnit, updateLabelSizesFct, ratio, activated, action),
    _message(message) {
}

std::string MessageLabel::message() const {
    return _message;
}
