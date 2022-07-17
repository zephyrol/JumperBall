/*
 * File: MessageLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */


#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H

#include "Label.h"

class MessageLabel;

using MessageLabel_sptr = std::shared_ptr<MessageLabel>;
using CstMessageLabel_sptr = std::shared_ptr<const MessageLabel>;
using vecCstMessageLabel_sptr = std::vector<CstMessageLabel_sptr>;
using vecMessageLabel_sptr = std::vector<MessageLabel_sptr>;

class MessageLabel : public Label {
public:
    MessageLabel(
        const std::string &message,
        const CstNode_sptr &node,
        bool isActivated = false
    );

    ~MessageLabel() override = default;

    std::string message() const override;

private:
    const std::string _message;

};

#endif // MESSAGELABEL_H
