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

    struct CharacterLocalTransform {
        /**
         * Size of the letter in screen space
         */
        float width;
        float height;

        /**
         * Offset from baseline to left/top of the letter in screen space;
         */
        float bearingX;
        float bearingY;

        /**
         * Offset until the next letter in screen space
         */
        float advance;
    };

    MessageLabel(
        const std::string &message,
        std::vector<CharacterLocalTransform>&& transforms,
        const CstNode_sptr &node,
        size_t lettersSize,
        bool isActivated = false
    );

    ~MessageLabel() override = default;

    vecGeometry genGeometries() const override;

    /**
     * Letter hash containing the character and its font size
     */
    using LetterHash = std::string;

    std::string message() const override;

    const std::vector<std::string> &getLetterHashes() const;

    static LetterHash createLetterHash(size_t fontSize, unsigned char letter);

private:
    const std::string _message;
    const std::vector<CharacterLocalTransform> _transforms;
    const size_t _lettersSize;
    const std::vector<std::string> _letterHashes;

    std::vector<std::string> createLetterHashes() const;

};

#endif // MESSAGELABEL_H
