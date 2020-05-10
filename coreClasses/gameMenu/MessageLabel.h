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
    MessageLabel(float width,float height,const std::string& message);

    virtual Label::TypeOfLabel typeOfLabel() const override;
    const std::string& message() const;

private:
    const std::string _message;
};

#endif // MESSAGELABEL_H
