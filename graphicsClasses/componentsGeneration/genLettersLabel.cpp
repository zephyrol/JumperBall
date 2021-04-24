/*
 * File: genLettersLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 21 avril 2020, 16:00
 */

#include "MeshGenerator.h"

std::map <unsigned char, vecMesh_sptr> MeshGenerator::genLettersLabel (const Page& page) {

    std::map <unsigned char, vecMesh_sptr> lettersMeshes;

    const glm::vec3 enabledLetterColor = glm::vec3(0.f, 1.f, 1.f);
    const glm::vec3 disabledLetterColor = glm::vec3(0.5f, 0.5f, 0.5f);

    for (const auto& label : page.labels()) {
        for (unsigned char character : label->message()) {
            if (lettersMeshes.find(character) == lettersMeshes.end()) {
                lettersMeshes[character] = {};
            }
            // TODO it's NOT ONLY the label that we have to use to compute the quad position width and height
            lettersMeshes[character].push_back(
              genQuadLabel(
                label->position(),
                label->width(),
                label->height(),
                label->isActivated() ? enabledLetterColor : disabledLetterColor)
            );
        }
    }

    return lettersMeshes;
}