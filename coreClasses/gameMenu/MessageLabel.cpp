/*
 * File: MessageLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */
#include "MessageLabel.h"
#include <utility>

MessageLabel::MessageLabel(
    std::string message,
    std::vector<MessageLabel::CharacterLocalTransform> &&transforms,
    MessageLabel::LettersUvs_sptr lettersUvs,
    const CstNode_sptr &node,
    size_t lettersSize,
    bool isActivated
) : Label(node, isActivated),
    _message(std::move(message)),
    _lettersUvs(std::move(lettersUvs)),
    _transforms(std::move(transforms)),
    _lettersSize(lettersSize),
    _letterHashes(createLetterHashes()) {
}

std::string MessageLabel::message() const {
    return _message;
}

vecGeometry MessageLabel::genGeometries() const {
    vecGeometry geometries{};
    const auto &screenTransform = _node->getTransform();
    auto baseX = screenTransform.positionX - screenTransform.width / 2.f;
    const auto baseY = screenTransform.positionY - screenTransform.height / 2.f;

    const auto maxOriginHeight = [this](){
        float maxOrigHeight = 0.f;
        for(const auto& transform: _transforms) {
            maxOrigHeight = std::max(maxOrigHeight, transform.originHeight);
        }
        return maxOrigHeight;
    }();

    for (size_t i = 0; i < _message.size(); ++i) {
        const auto& transform = _transforms[i];
        const auto& uv = _lettersUvs->at(_letterHashes[i]);
        const auto letterOffsetOriginHeight = maxOriginHeight - transform.originHeight;
        const auto xPosition = baseX + (transform.bearingX + transform.width / 2.f) * screenTransform.width;
        const auto yPosition = baseY + (letterOffsetOriginHeight + transform.height / 2.f) * screenTransform.height;
        const Geometry quad(
            Geometry::Shape::Quad,
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

// TODO remove _letterHashes if useless
const std::vector<std::string> &MessageLabel::getLetterHashes() const {
    return _letterHashes;
}

std::vector<std::string> MessageLabel::createLetterHashes() const {
    std::vector<std::string> letterHashes{};
    for (unsigned char c: _message) {
        letterHashes.push_back(createLetterHash(_lettersSize, c));
    }
    return letterHashes;
}

MessageLabel::LetterHash MessageLabel::createLetterHash(size_t fontSize, unsigned char letter) {
    MessageLabel::LetterHash hash = std::to_string(fontSize) + ",";
    hash += static_cast<char>(letter);
    return hash;
}

