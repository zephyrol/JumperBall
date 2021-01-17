/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ArrowRendering.h
 * Author: Morgenthaler S
 * 
 * Created on 17 janvier 2021, 11:20
 */

#include "ArrowRendering.h"
#include "ShaderProgram.h"
#include <iterator>

ArrowRendering::ArrowRendering(const Label &label,
                             const ShaderProgram &spArrow):
    LabelRendering(label),
    _spArrow(spArrow),
    _transformQuad(1.f),
    _transformTriangle(1.f)
{
    updateGeometry();
}


void ArrowRendering::updateGeometry() {
    if (!displayQuad) {
        displayQuad = std::make_shared<const Quad>();
    }
    if (!displayTriangle) {
        displayTriangle = std::make_shared<const Triangle>();
    }
}

void ArrowRendering::render() const {
    _spArrow.bindUniform("M",_transformQuad);
    displayQuad->bind();
    displayQuad->draw();
    _spArrow.bindUniform("M",_transformTriangle);
    displayTriangle->bind();
    displayTriangle->draw();
}

const glm::vec3& ArrowRendering::getArrowColor() const {
    return  _label.isActivated() 
            ? enabledLetterColor
            : disabledLetterColor;
}

void ArrowRendering::update(float offset) {
    static_cast<void> (offset); // TODO add offset in the computing
    const glm::mat4 biasMatrix  = glm::mat4{ 1.f, 0.f,  0.f, 0.f,
                                             0.f,  1.f, 0.f, 0.f,
                                             0.f,  0.f,  1.f, 0.f,
                                             -1.f, -1.f, 0.f, 1.f} ;

    constexpr float biasScalar = 2.f; //To multiply the translation by 2
    const glm::vec3 scale = glm::vec3{_label.width(),_label.height(),0.f};

    const glm::mat4 scaleMatrix = glm::scale(scale);

    const glm::mat4 translate = glm::translate( biasScalar *
                                    glm::vec3{ _label.position().x,
                                               _label.position().y , 0.f});
    _transformTriangle = biasMatrix * translate * scaleMatrix;
    _transformQuad = biasMatrix * translate * scaleMatrix;
}

const ShaderProgram& ArrowRendering::getShaderProgram() const {
    return _spArrow;
}

std::shared_ptr<const Quad> ArrowRendering::displayQuad = nullptr;
std::shared_ptr<const Triangle> ArrowRendering::displayTriangle = nullptr;
const glm::vec3 ArrowRendering::enabledLetterColor =
    glm::vec3(0.f,1.f,1.f);
const glm::vec3 ArrowRendering::disabledLetterColor =
    glm::vec3(0.5f,0.5f,0.5f);
