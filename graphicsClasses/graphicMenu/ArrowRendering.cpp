/*
 * File: ArrowRendering.h
 * Author: Morgenthaler S
 *
 * Created on 17 janvier 2021, 11:20
 */

#include "ArrowRendering.h"
#include "ShaderProgram.h"
#include <iterator>

ArrowRendering::ArrowRendering(const Label& label):
    LabelRendering(label),
    _transformQuad(0.f),
    _transformTriangle(0.f) {
    updateGeometry();
    update(0.f);
}


void ArrowRendering::updateGeometry() {
    if (!displayQuad) {
        displayQuad = std::make_shared <const Quad>();
    }
    if (!displayTriangle) {
        displayTriangle = std::make_shared <const Triangle>();
    }
    if (!spArrow) {
        spArrow = std::make_shared <ShaderProgram>(
            Shader(GL_VERTEX_SHADER, vsshaderArrow),
            Shader(GL_FRAGMENT_SHADER, fsshaderArrow)
            );
    }
}

void ArrowRendering::render() const {
    spArrow->use();
    spArrow->bindUniform("arrowColor", arrowColor);
    spArrow->bindUniform("M", _transformQuad);
    // displayQuad->bind();
    // displayQuad->draw();
    spArrow->bindUniform("M", _transformTriangle);
    // displayTriangle->bind();
    // displayTriangle->draw();
}

const glm::vec4& ArrowRendering::getArrowColor() const {
    // return _label.isActivated()
    // ? enabledLetterColor
    // : disabledLetterColor;
    return arrowColor;
}

void ArrowRendering::update (float offset) {

    const auto getPosition = [this] (float offset) {
                                 JBTypes::vec2f position = _label.position();
                                 if (!_label.isFixed()) {
                                     position.y += offset;
                                 }
                                 return position;
                             };
    const glm::mat4 biasMatrix = glm::mat4 { 1.f, 0.f, 0.f, 0.f,
                                             0.f, 1.f, 0.f, 0.f,
                                             0.f, 0.f, 1.f, 0.f,
                                             -1.f, -1.f, 0.f, 1.f };

    constexpr float biasScalar = 2.f; // To multiply the translation by 2
    const glm::vec3 scale = glm::vec3 { _label.width(), _label.height(), 0.f };

    const glm::mat4 scaleMatrix = glm::scale(scale);

    const JBTypes::vec2f position = getPosition(offset);

    const glm::mat4 translate = glm::translate(biasScalar *
                                               glm::vec3 { position.x,
                                                           position.y,
                                                           0.f
                                               });
    _transformTriangle = biasMatrix * translate *
                         scaleMatrix * leftTriangleTransform;
    _transformQuad = biasMatrix * translate *
                     scaleMatrix * leftQuadTransform;
}

const std::string ArrowRendering::vsshaderArrow = "shaders/arrowVs.vs";
const std::string ArrowRendering::fsshaderArrow = "shaders/arrowFs.fs";
std::shared_ptr <const ShaderProgram> ArrowRendering::spArrow = nullptr;

std::shared_ptr <const Quad> ArrowRendering::displayQuad = nullptr;
std::shared_ptr <const Triangle> ArrowRendering::displayTriangle = nullptr;
const glm::vec4 ArrowRendering::arrowColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

const glm::mat4 ArrowRendering::leftTriangleTransform =
    glm::translate(glm::vec3(-0.6f, 0.f, 0.f)) *
    glm::rotate((static_cast <float>(M_PI) / 2.f), glm::vec3(0.f, 0.f, 1.f)) *
    glm::scale(glm::vec3(1.f, 0.4f, 1.f));
const glm::mat4 ArrowRendering::rightTriangleTransform =
    glm::translate(glm::vec3(0.6f, 0.f, 0.f)) *
    glm::rotate((-static_cast <float>(M_PI) / 2.f), glm::vec3(0.f, 0.f, 1.f)) *
    glm::scale(glm::vec3(1.f, 0.4f, 1.f));
const glm::mat4 ArrowRendering::leftQuadTransform =
    glm::translate(glm::vec3(0.4f, 0.f, 0.f)) *
    glm::scale(glm::vec3(0.6f, 0.5f, 1.f));
const glm::mat4 ArrowRendering::rightQuadTransform =
    glm::translate(glm::vec3(-0.4f, 0.f, 0.f)) *
    glm::scale(glm::vec3(0.6f, 0.5f, 1.f));
