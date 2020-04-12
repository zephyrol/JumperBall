/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 13 novembre 2019, 21:05
 */

#ifndef MESH_H
#define MESH_H
#include "Shader.h"
#include "ShaderProgram.h"
#include <Ball.h>
#include <Map.h>
#include "Star.h"
#include "MeshGenerator.h"


template<typename T> class Mesh;

template<typename T>
class Mesh {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Mesh                    (const T& base);
    
    //-------CONST METHODS----------//
    const glm::mat4&        world()                                       const;
    void                    render(const ShaderProgram& sp)               const;
     
    //----------METHODS-------------//
    void                    update();


private:

    //--------ATTRIBUTES-----------//
    const T&                _base;
    std::vector<MeshComponent>  
                            _components;
    glm::mat4               _world;


    //----------METHODS-------------//
    void                    update(const Ball& base);
    void                    update(const Map&  base);
    void                    update(const Quad& base);
    void                    update(const Star& base);

};



template<typename T>
Mesh<T>::Mesh(const T& base):
  _base(base),
  _components(MeshGenerator::genComponents(base)),
  _world(1.f)
{
}

template<typename T>
void Mesh<T>::update() {
    update(_base);
}

template<typename T>
void Mesh<T>::update(const Ball& base) {

    JumperBallTypes::vec3f positionBall = base.get3DPosition();
    _world = glm::translate(glm::mat4(1.f), glm::vec3(positionBall.x,
                            positionBall.y ,positionBall.z));

    if (_components.size() > 0)
        _components.at(0).animation()->updateTrans();
}

template<typename T>
void Mesh<T>::update(const Map& base) {
    (void) base;
    for(const MeshComponent& component : _components){
        if (component.animation()) {
            component.animation()->updateTrans();
        }
    }
}

template<typename T>
void Mesh<T>::update(const Quad& base) {
    (void) base;
}

template<typename T>
void Mesh<T>::update(const Star& base) {
    _world = base.transform();
}


template<typename T>
const glm::mat4& Mesh<T>::world() const {
    return _world;
}


template<typename T>
void Mesh<T>::render(const ShaderProgram& sp) const {

    sp.bindUniform("W",_world);
    GLuint currentVAO = 0; 
    for( const MeshComponent& component : _components) {
        glm::mat4 modelTransform(1.f);
        glm::mat4 normalsTransform(1.f);
        if(component.shape()) {
            modelTransform = component.shape()->modelTransform();
            normalsTransform = component.shape()->normalsTransform();
        }
        if(component.animation()) {
            modelTransform = component.animation()->model() * modelTransform;
            normalsTransform = component.animation()->scaleRotation() * 
                    normalsTransform;
        }
        sp.bindUniform("M",modelTransform);
        sp.bindUniform("SR",normalsTransform);
        
        if(currentVAO != *component.shape()->vertexArrayObject()){
            component.shape()->bind();
            currentVAO = *component.shape()->vertexArrayObject();
        }
        component.shape()->draw();
    }
}

#endif /* MESH_H */

