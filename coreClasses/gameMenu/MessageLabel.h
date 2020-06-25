/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   MessageLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */


#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H
#include "Label.h"

class MessageLabel : public Label
{
public:
    MessageLabel(float width,float height,
        const JumperBallTypes::vec2f& position,
        const std::string& message,
        bool activated = true,
        const std::shared_ptr<LabelAnswer> action = nullptr
        );

    virtual Label::TypeOfLabel typeOfLabel() const override;
    std::string message() const override;



private:
    const std::string _message;
    
};

#endif // MESSAGELABEL_H
