/*
 * File: MessageLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */
#include "MessageLabel.h"

MessageLabel::MessageLabel(
    const std::string &message,
    std::vector<MessageLabel::CharacterLocalTransform>&& transforms,
    const CstNode_sptr &node,
    size_t height,
    bool isActivated
) : Label(node, isActivated),
    _message(message),
    _transforms(std::move(transforms)),
    _height(height),
    _letterHashes(createLetterHashes()){
}

std::string MessageLabel::message() const {
    return _message;
}

vecGeometry MessageLabel::genGeometries() const {

    vecGeometry geometries {};
    _node->getScreenSpaceWidth();
    for (const auto &transform: _transforms) {

    }
    const Geometry quad(
        Geometry::Shape::Quad,
        {0.4f, 0.f, 0.f},
        {0.f, 0.f, 0.f},
        {0.6f, 0.5f, 1.f}
    );
}

const std::vector<std::string>& MessageLabel::getLetterHashes() const {
    return _letterHashes;
}

std::vector<std::string> MessageLabel::createLetterHashes() const {
    std::vector<std::string> letterHashes {};
    for (unsigned char c: _message) {
        letterHashes.push_back(createLetterHash(_height, c));
    }
    return letterHashes;
}

MessageLabel::LetterHash MessageLabel::createLetterHash(size_t fontSize, unsigned char letter) {
    MessageLabel::LetterHash hash = std::to_string(fontSize) + ",";
    hash += static_cast<char>(letter);
    return hash;
}

