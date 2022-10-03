/*
 * File: TextLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */
#include "TextLabel.h"
#include <utility>

TextLabel::TextLabel(
    CstTextNode_uptr &&textNode,
    const Node_sptr &suitedNode,
    LettersUvs_sptr lettersUvs,
    std::vector<CharacterLocalTransform> &&transforms,
    size_t lettersSize,
    const JBTypes::Color &color
) : Label(suitedNode, color),
    _textNode(std::move(textNode)),
    _lettersUvs(std::move(lettersUvs)),
    _transforms(std::move(transforms)),
    _lettersSize(lettersSize),
    _letterHashes(createLetterHashes()) {
}

std::string TextLabel::message() const {
    return _textNode->text();
}

vecLabelGeometry TextLabel::genGeometries() const {
    vecLabelGeometry geometries{};
    const auto &screenTransform = _node->getTransform();
    auto baseX = screenTransform.positionX - screenTransform.width / 2.f;
    const auto baseY = screenTransform.positionY - screenTransform.height / 2.f;

    const auto maxOriginHeight = [this]() {
        float maxOrigHeight = 0.f;
        for (const auto &transform: _transforms) {
            maxOrigHeight = std::max(maxOrigHeight, transform.originHeight);
        }
        return maxOrigHeight;
    }();

    for (size_t i = 0; i < _textNode->text().size(); ++i) {
        const auto &transform = _transforms[i];
        const auto &uv = _lettersUvs->at(_letterHashes[i]);
        const auto letterOffsetOriginHeight = maxOriginHeight - transform.originHeight;
        const auto xPosition = baseX + (transform.bearingX + transform.width / 2.f) * screenTransform.width;
        const auto yPosition =
            baseY + (letterOffsetOriginHeight + transform.height / 2.f) * screenTransform.height;
        const LabelGeometry quad(
            LabelGeometry::Shape::Quad,
            {
                xPosition * 2.f,
                yPosition * 2.f,
                0.f
            },
            {0.f, 0.f, 0.f},
            {
                screenTransform.width * transform.width,
                screenTransform.height * transform.height
            },
            _color,
            {
                // Up left
                {uv.uvMinX, uv.uvMaxY},
                // Down left
                {uv.uvMinX, uv.uvMinY},
                // Down right
                {uv.uvMaxX, uv.uvMinY},
                // Up left
                {uv.uvMinX, uv.uvMaxY},
                // Down right
                {uv.uvMaxX, uv.uvMinY},
                // Up right
                {uv.uvMaxX, uv.uvMaxY},
            }
        );
        geometries.push_back(quad);
        baseX += (transform.advance * screenTransform.width);
    }
    return geometries;
}

std::vector<std::string> TextLabel::createLetterHashes() const {
    std::vector<std::string> letterHashes{};
    for (unsigned char c: _textNode->text()) {
        letterHashes.push_back(createLetterHash(_lettersSize, c));
    }
    return letterHashes;
}

TextLabel::LetterHash TextLabel::createLetterHash(size_t fontSize, unsigned char letter) {
    TextLabel::LetterHash hash = std::to_string(fontSize) + ",";
    hash += static_cast<char>(letter);
    return hash;
}

Displayable::StaticValues<JBTypes::vec2f> TextLabel::getStaticVec2fValues() const {
    return {_textNode->staticProperty()};
}

