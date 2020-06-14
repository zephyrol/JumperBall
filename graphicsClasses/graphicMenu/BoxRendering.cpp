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

BoxRendering::BoxRendering(const glm::vec3& color1,
                           const glm::vec3& color2):
    _boxQuad(color1, color2),
    _spBox( Shader (GL_VERTEX_SHADER,   vsshaderBox),
            Shader (GL_FRAGMENT_SHADER, fsshaderBox))
{
}


void BoxRendering::render(const Label& label) const {
    _spBox.use();

    const glm::mat4 biasMatrix  = glm::mat4{ 1.f, 0.f,  0.f, 0.f,
                                             0.f,  1.f, 0.f, 0.f,
                                             0.f,  0.f,  1.f, 0.f,
                                             -1.f, -1.f, 0.f, 1.f} ;

    _boxQuad.bind();
    constexpr float biasScalar = 2.f; //To multiply the translation by 2
    const glm::vec3 scale = glm::vec3{label.width(),label.height(),0.f};

    const glm::mat4 scaleMatrix = glm::scale(scale);

    const glm::mat4 translate = glm::translate( biasScalar *
                                    glm::vec3{ label.position().x,
                                               label.position().y , 0.f});
    const glm::mat4 transformCharacter =
                    biasMatrix * translate * scaleMatrix;

    _spBox.bindUniform("M",transformCharacter);
    _boxQuad.draw();
}


const std::string BoxRendering::vsshaderBox = "shaders/boxVs.vs";
const std::string BoxRendering::fsshaderBox = "shaders/boxFs.fs";

