/*
 * File: genLettersLabel.cpp
 * Author: Morgenthaler S
 *
 */

#include "MeshGenerator.h"

std::map <unsigned char, vecMesh_sptr> MeshGenerator::genLettersLabel (
    const Page& page,
    const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet
    ) {

    std::map <unsigned char, vecMesh_sptr> lettersMeshes;

    const glm::vec3 enabledLetterColor(0.f, 1.f, 1.f);
    const glm::vec3 disabledLetterColor(0.5f, 0.5f, 0.5f);

    for (const auto& label : page.labels()) {
        const float height = label->height();
        const JBTypes::vec2f& position = label->position();
        const float pitch = label->width() / label->message().size();

        const float initialOffsetX = -label->width() / 2.f + pitch / 2.f;
        const std::string message = label->message();
        for (size_t i = 0; i < message.size(); ++i) {
            const char character = message.at(i);
            if (lettersMeshes.find(character) == lettersMeshes.end()) {
                lettersMeshes[character] = {};
            }

            const float offsetX = initialOffsetX + i * pitch;
            const auto& graphicCharacter = graphicAlphabet.at(character);
            const auto& localTransform = graphicCharacter.characterTransform;

            const auto& localScale = localTransform.localScale;
            const float localScaleX = localScale.x;
            const float localScaleY = localScale.y;
            const float localTranslateY = localTransform.localTranslate.y;

            const float scaleX = pitch * localScaleX;
            const float scaleY = height * localScaleY;

            const float translateX = Utility::menuPositionToOpenGLScreenFormat(position.x + offsetX);
            const float translateY = Utility::menuPositionToOpenGLScreenFormat(
                position.y + localTranslateY * height
                );

            lettersMeshes[character].push_back(
                genQuadLabel(
                    { translateX, translateY },
                    scaleX,
                    scaleY,
                    label->isActivated() ? enabledLetterColor : disabledLetterColor)
                );
        }
    }

    return lettersMeshes;
}
