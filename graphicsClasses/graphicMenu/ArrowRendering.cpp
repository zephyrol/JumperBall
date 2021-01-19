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

ArrowRendering::ArrowRendering(const Label &label):
    LabelRendering(label),
    _transformQuad(0.f),
    _transformTriangle(0.f)
{
    updateGeometry();
    update(0.f);
}


void ArrowRendering::updateGeometry() {
    if (!displayQuad) {
        displayQuad = std::make_shared<const Quad>();
    }
    if (!displayTriangle) {
        displayTriangle = std::make_shared<const Triangle>();
    }
    if(!spArrow) {
        spArrow = std::make_shared<ShaderProgram> (
            Shader (GL_VERTEX_SHADER,   vsshaderArrow),
            Shader (GL_FRAGMENT_SHADER, fsshaderArrow)
        );
    }
}

void ArrowRendering::render() const {
    //spArrow->use();
    //spArrow->bindUniform("arrowColor",arrowColor);
    //spArrow->bindUniform("M",_transformQuad);
    //displayQuad->bind();
    //displayQuad->draw();
    //_spArrow.bindUniform("M",_transformTriangle);
    //displayTriangle->bind();
    //displayTriangle->draw();
}

const glm::vec4& ArrowRendering::getArrowColor() const {
    //return  _label.isActivated() 
    //        ? enabledLetterColor
    //        : disabledLetterColor;
    return arrowColor;
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

const std::string ArrowRendering::vsshaderArrow = "shaders/arrowVs.vs";
const std::string ArrowRendering::fsshaderArrow = "shaders/arrowFs.fs";
std::shared_ptr<const ShaderProgram> ArrowRendering::spArrow = nullptr;

std::shared_ptr<const Quad> ArrowRendering::displayQuad = nullptr;
std::shared_ptr<const Triangle> ArrowRendering::displayTriangle = nullptr;
const glm::vec4 ArrowRendering::arrowColor = glm::vec4(1.f,0.f,1.f,1.f);
