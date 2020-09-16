/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BoxRendering.cpp
 * Author: Morgenthaler S
 * 
 * Created on 14 juin 2020
 */

#include "BoxRendering.h"
#include "ShaderProgram.h"

BoxRendering::BoxRendering(const Label &label, const glm::vec3& color1,
                           const glm::vec3& color2):
    LabelRendering(label),
    _boxQuad(color1, color2),
    _spBox( Shader (GL_VERTEX_SHADER,   vsshaderBox),
            Shader (GL_FRAGMENT_SHADER, fsshaderBox)),
    _transformCharacter(1.f)
{
}


void BoxRendering::render() const {
    _spBox.use();
    _boxQuad.bind();
    _spBox.bindUniform("M",_transformCharacter);
    _boxQuad.draw();
}

void BoxRendering::update(float offset) {
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
    _transformCharacter = biasMatrix * translate * scaleMatrix;
}

const std::string BoxRendering::vsshaderBox = "shaders/boxVs.vs";
const std::string BoxRendering::fsshaderBox = "shaders/boxFs.fs";

