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
#include <objects/Object.h>
#include "geometry/GeometricShape.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "animations/BallAnimation.h"
#include "MeshComponent.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/glm.hpp>


template<typename T> class Mesh;

template<typename T>
class Mesh {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Mesh                    (const T& base);
    //Mesh                    (const Ball& ball);
    //Mesh                    (const Map& map);
    //Mesh                    (Object::CategoryOfObjects category);

    /*Mesh                    (const Mesh& mesh)                        = default;
    Mesh&                    operator= (const Mesh& mesh)             = default;

    Mesh                    (Mesh&& mesh)                             = default;
    Mesh&                    operator= (Mesh&& mesh)                  = default;*/

    virtual                 ~Mesh();
    
    //-------CONST METHODS----------//
    const glm::mat4&        world()                                       const;

    void                    render(const ShaderProgram& sp)               const;
     
    //----------METHODS-------------//
    void                    world(const glm::mat4& w);
    void                    updateMatrices (const Ball& b);

    void                    update();
    //void                    updateMatrices(
    //                          const std::array<unsigned int,3>& blockPosition,
    //                          JumperBallTypes::Direction objectDirection);

private:

    //--------ATTRIBUTES-----------//
    /*std::vector<glm::vec3>  _positions;
    std::vector<glm::vec3>  _normals;
    std::vector<glm::vec3>  _colors;
    std::vector<glm::vec2>  _uvCoords;

    const bool              _useIndexing;
    std::vector<GLushort>   _indices;
    GLuint                  _idElementBuffer;

    GLuint                  _idVertexArray;
    std::array<GLuint,4>    _idVertexBuffer;*/


    const T&                _base;
    std::vector<MeshComponent>  
                            _components;

    glm::mat4               _world;
    //----------METHODS-------------//
    void                    genSharps(const Block& block,
                                      glm::vec3 posWorld,
                                      const Pyramid& pyramid);


    void                    update(const Ball& base);
    void                    update(const Map&  base);

    static std::vector<MeshComponent>
                            genComponents(const Ball& ball);
    static std::vector<MeshComponent>
                            genComponents(const Map&  map);


    //-------CONST METHODS----------//
    //void                    bindVertexData()                              const;
};



template<typename T>
Mesh<T>::Mesh(const T& base):
  _base(base),
  _components(genComponents(base)),
  _world(1.f)
{

}

template<typename T>
void Mesh<T>::update() {
    update(_base);
}

template<typename T>
std::vector<MeshComponent> Mesh<T>::genComponents(const Ball& ball) {

    glm::mat4 scaleMatrix = glm::scale(glm::vec3(ball.getRadius()));
    Sphere sphere;
     
    MeshComponent component ( 
                                std::make_shared<Sphere>(sphere,scaleMatrix),
                                std::make_shared<BallAnimation>(ball));
    return std::vector<MeshComponent> {component};
}

template<typename T>
std::vector<MeshComponent> Mesh<T>::genComponents(const Map& map) {

    auto genSharps = [] (
            std::vector<MeshComponent>& components,
            const Block& block, 
            glm::vec3 posWorld, 
            const Pyramid& pyramid) {
        
        if (block.getType() == Block::categoryOfBlocksInFile::Sharp) {
            
            const std::array<float,7> scales 
            {.2f,.1f,.05f,.1f,.075f,.15f,0.175f};
            
            const std::array<glm::vec2,7> translationFloorFactor 
            {
                glm::vec2(0.f,0.f),   glm::vec2(-0.6f,-0.4f), glm::vec2(0.6f,-0.6f),
                        glm::vec2(0.2f,0.6f), glm::vec2(-0.2f,-0.6f), glm::vec2(0.6f,0.6f),
                        glm::vec2(-0.6f,0.6f)
            };
            
            for(size_t i = 0; i < block.faceInfo().size(); i++) {
                
                bool isSharp = block.faceInfo().at(i);
                if (isSharp) {
                    
                    constexpr float sizeBlock = 1.f;
                    constexpr float offset = sizeBlock/2.f;
                    
                    JumperBallTypes::Direction currentDir =
                            JumperBallTypesMethods::integerAsDirection(i);
                    JumperBallTypes::vec3f vecDir = 
                            JumperBallTypesMethods::directionAsVector(currentDir);
                    
                    glm::mat4 translationOffset = glm::translate(
                            glm::vec3( -offset, 0 , -offset ));
                    
                    glm::mat4 rotationLocal = 
                            Utility::rotationUpToDir(currentDir);
                    
                    
                    for (size_t j = 0 ; j < scales.size() ; j++) {
                        
                        glm::mat4 scaleLocal = 
                                glm::scale(glm::vec3(scales.at(j),0.5f,scales.at(j)));
                        
                        
                        glm::mat4 translationLocal =
                                glm::translate( glm::vec3(
                                posWorld.x + offset + vecDir.x * offset,
                                posWorld.y + offset + vecDir.y * offset,
                                posWorld.z + offset + vecDir.z * offset
                                ));
                        
                        glm::mat4 translationFloor = glm::translate( glm::vec3(
                                offset * translationFloorFactor.at(j).x,
                                0.f,
                                offset * translationFloorFactor.at(j).y
                                ));
                        
                        glm::mat4 modelTranf= translationLocal * rotationLocal * 
                                translationFloor * scaleLocal * 
                                translationOffset;
                        glm::mat4 normalsTrans= rotationLocal /* * scaleLocal*/;

                        MeshComponent component ( std::make_shared<Pyramid>
                        (pyramid,modelTranf,normalsTrans),
                                nullptr);
                        components.push_back(component);
                    } 
                }
            }
        }
    };
    std::vector<MeshComponent> components;
    Cube  cube; // Basic cube for the blocks
    for (unsigned int x = 0; x < map.boundingBoxXMax() ; ++x ) {
        for (unsigned int y = 0; y < map.boundingBoxYMax() ; ++y ) {
            for (unsigned int z = 0; z < map.boundingBoxZMax() ; ++z ) {
                auto block = map.getBlock(x,y,z);
                if (block) {
                    glm::mat4 transform (glm::translate(glm::vec3(x,y,z)));
                    MeshComponent component 
                        (std::make_shared<Cube>(cube,transform), nullptr);
                    components.push_back(component);
                }
            }
        }
    }


    //We have better performances if the components are stored with objects
    //with the same VAO/VBO, we will not have to change the binding later
    Pyramid pyramid; // Basic pyramid for the sharps
    for (unsigned int x = 0; x < map.boundingBoxXMax() ; ++x ) {
        for (unsigned int y = 0; y < map.boundingBoxYMax() ; ++y ) {
            for (unsigned int z = 0; z < map.boundingBoxZMax() ; ++z ) {
                auto block = map.getBlock(x,y,z);
                if (block) {
                    genSharps(components,*block,glm::vec3(x,y,z),pyramid);
                }
            }
        }
    }

    return components;
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
void Mesh<T>::update(const Map& map) {
    (void) map;
}


template<typename T>
const glm::mat4& Mesh<T>::world() const {
    return _world;
}

template<typename T>
void Mesh<T>::world(const glm::mat4& w) {
    _world = w;
}

template<typename T>
void Mesh<T>::genSharps(const Block& block, glm::vec3 posWorld,
                      const Pyramid& pyramid) {
    

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


template<typename T>
Mesh<T>::~Mesh() {
}


#endif /* MESH_H */

