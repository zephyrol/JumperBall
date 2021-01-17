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
    _spArrow(spArrow),
    LabelRendering(label),
    _charactersTransforms(label.message().size())
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
}

const glm::vec3& ArrowRendering::getArrowColor() const {
    return  _label.isActivated() 
            ? enabledLetterColor
            : disabledLetterColor;
}

void ArrowRendering::update(float offset) {
}

const ShaderProgram& ArrowRendering::getShaderProgram() const {
    return _spArrow;
}

GLuint ArrowRendering::getQuadVAO() const
{
    // displayQuad can not be null because the pointer is allocated in the
    // constructor
    // displayQuad->vertexArrayObject() can not be null because the pointer is
    // allocated in the GeometryShape constructor
    return *displayQuad->vertexArrayObject();
}

const Quad& ArrowRendering::getDisplayQuad() const
{
    // displayQuad can not be null because the pointer is allocated in the
    // constructor
    return *displayQuad;
}

std::shared_ptr<const Quad> ArrowRendering::displayQuad = nullptr;
const glm::vec3 ArrowRendering::enabledLetterColor =
    glm::vec3(0.f,1.f,1.f);
const glm::vec3 ArrowRendering::disabledLetterColor =
    glm::vec3(0.5f,0.5f,0.5f);
