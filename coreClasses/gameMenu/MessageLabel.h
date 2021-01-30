/*
 * File: MessageLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */


#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H
#include "Label.h"

class MessageLabel:public Label {
public:
MessageLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    const std::string& message,
    const std::shared_ptr <LabelAnswer> action = nullptr,
    bool activated = true
    );
virtual ~MessageLabel() = default;

std::string message() const override;

private:
const std::string _message;

};

#endif // MESSAGELABEL_H
