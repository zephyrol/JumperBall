/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   MessageLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */
#include "MessageLabel.h"

MessageLabel::MessageLabel(float width,float height,
    const JumperBallTypes::vec2f& position,
    const std::string& message,
    bool activated):
    Label(width,height,position,activated),
    _message(message)
{
}

std::string MessageLabel::message() const
{
    return _message;
}



Label::TypeOfLabel MessageLabel::typeOfLabel() const {
    return Label::TypeOfLabel::Message;
 }
