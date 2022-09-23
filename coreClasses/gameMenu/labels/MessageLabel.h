/*
 * File: MessageLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */


#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H

#include "Label.h"
#include "gameMenu/nodes/TextNode.h"

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
         * Horizontal offset from baseline to left the letter in screen space;
         */
        float bearingX;

        /**
         * Vertical offset from baseline to the origin in screen space;
         */
        float originHeight;

        /**
         * Offset until the next letter in screen space
         */
        float advance;
    };


    /**
     * Letter hash containing the character and its font size
     */
    using LetterHash = std::string;

    struct LetterUv {
        /**
         * UV character in texture corresponding to the bottom left corner
         */
        float uvMinX;
        float uvMinY;

        /**
         * UV character in texture corresponding to the top right corner
         */
        float uvMaxX;
        float uvMaxY;
    };

    using LettersUvs = std::unordered_map<LetterHash, LetterUv>;
    using LettersUvs_sptr = std::shared_ptr<LettersUvs>;
    using CstLettersUvs_sptr = std::shared_ptr<const LettersUvs>;

    MessageLabel(
        CstTextNode_uptr &&textNode,
        const Node_sptr& suitedNode,
        LettersUvs_sptr lettersUvs,
        std::vector<CharacterLocalTransform> &&transforms,
        size_t lettersSize,
        bool isActivated
    );

    vecGeometry genGeometries() const override;

    StaticValues <JBTypes::vec2f> getStaticVec2fValues() const override;

    std::string message() const override;

    static LetterHash createLetterHash(size_t fontSize, unsigned char letter);

private:
    const CstTextNode_uptr _textNode;
    const LettersUvs_sptr _lettersUvs;
    const std::vector<CharacterLocalTransform> _transforms;
    const size_t _lettersSize;
    const std::vector<std::string> _letterHashes;

    std::vector<std::string> createLetterHashes() const;

};

#endif // MESSAGELABEL_H
