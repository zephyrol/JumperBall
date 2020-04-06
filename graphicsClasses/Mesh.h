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
#include <objects/Object.h>
#include "geometry/Cylinder.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "animations/BallAnimation.h"
#include "animations/ObjectAnimation.h"
#include "FrameBuffer.h"
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


    //--------STATIC METHODS-------//
    static std::vector<MeshComponent>
                            genComponents(const Ball& ball);
    static std::vector<MeshComponent>
                            genComponents(const Map&  map);
    static std::vector<MeshComponent>
                            genComponents(const Quad& screen);
    static std::vector<MeshComponent>
                            genComponents(const Star& star);
    static std::vector<MeshComponent>
                            genComponents(
                                      const std::shared_ptr<const Object>& obj,
                                      const glm::vec3& position,
                                      const JumperBallTypes::Direction& dir);

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


    auto blockManager = [] (
            std::vector<MeshComponent>& components,
            const Block& block, 
            const std::array<unsigned int,3>& position
            ) {
        
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
                    glm::vec2(0.f,0.f),glm::vec2(-0.6f,-0.4f), 
                            glm::vec2(0.6f,-0.6f), glm::vec2(0.2f,0.6f),
                            glm::vec2(-0.2f,-0.6f), glm::vec2(0.6f,0.6f),
                            glm::vec2(-0.6f,0.6f)
                };
                
                for(size_t i = 0; i < block.faceInfo().size(); i++) {
                    
                    bool isSharp = block.faceInfo().at(i);
                    if (isSharp) {
                        
                        constexpr float sizeBlock = 1.f;
                        constexpr float offset = sizeBlock/2.f;
                        
                        JumperBallTypes::Direction currentDir =
                                JumperBallTypesMethods::integerAsDirection(i);
                        JumperBallTypes::vec3f vecDir = JumperBallTypesMethods::
                                directionAsVector(currentDir);
                        
                        glm::mat4 translationOffset = glm::translate(
                                glm::vec3( -offset, 0 , -offset ));
                        
                        glm::mat4 rotationLocal = 
                                Utility::rotationUpToDir(currentDir);
                        
                        
                        for (size_t j = 0 ; j < scales.size() ; j++) {
                            
                            glm::mat4 scaleLocal = 
                                    glm::scale(glm::vec3(scales.at(j),0.5f,
                                    scales.at(j)));
                            
                            
                            glm::mat4 translationLocal =
                                    glm::translate( glm::vec3(
                                    posWorld.x + offset + vecDir.x * offset,
                                    posWorld.y + offset + vecDir.y * offset,
                                    posWorld.z + offset + vecDir.z * offset
                                    ));
                            
                            glm::mat4 translationFloor = glm::translate( 
                                    glm::vec3(
                                    offset * translationFloorFactor.at(j).x,
                                    0.f,
                                    offset * translationFloorFactor.at(j).y
                                    ));
                            
                            glm::mat4 modelTranf= translationLocal * 
                                rotationLocal * translationFloor * scaleLocal *
                                    translationOffset;
                            glm::mat4 normalsTrans= rotationLocal; 
                            
                            MeshComponent component ( std::make_shared<Pyramid>
                            (pyramid,modelTranf,normalsTrans),
                                    nullptr);
                            components.push_back(component);
                        } 
                    }
                }
            }
        };
        
        
        Cube  cube; // Basic cube for the blocks
        Pyramid pyramidSharp; // Basic pyramid for the sharps
        
        const glm::vec3 glmPosition { 
            position.at(0), position.at(1), position.at(2)};
        const glm::mat4 transform (glm::translate(glmPosition));
        
        MeshComponent component 
        (std::make_shared<Cube>(cube,transform), nullptr);
        components.push_back(component);
        
        genSharps(components,block,glmPosition,pyramidSharp);
        
        const std::array<std::shared_ptr<const Object>,6> objects =
        block.objects();
        for (size_t i = 0; i < objects.size() ; ++i) {
            if ( objects.at(i) ) {
                const JumperBallTypes::Direction dir =
                        JumperBallTypesMethods::integerAsDirection(i);
                const std::vector<MeshComponent> v =
                        genComponents(objects.at(i),glmPosition,dir);
                for(const MeshComponent& m : v) {
                    components.push_back(m);
                }
                //components.insert(components.end(),v.begin(),v.end()) ;
            }
        }
    };

    std::vector<MeshComponent> components;
    for (unsigned int x = 0; x < map.boundingBoxXMax() ; ++x ) {
        for (unsigned int y = 0; y < map.boundingBoxYMax() ; ++y ) {
            for (unsigned int z = 0; z < map.boundingBoxZMax() ; ++z ) {
                auto block = map.getBlock(x,y,z);
                if (block) {
                    blockManager(components,*block,
                                std::array<unsigned int,3> {x,y,z});
                }
            }
        }
    }


    //We have better performances if the components are stored with objects
    //with the same VAO/VBO, we will not have to change the binding later

    return components;
}

template<typename T>
std::vector<MeshComponent> Mesh<T>::genComponents(const Quad& quad) {

     
    MeshComponent component ( 
                                std::make_shared<Quad>(quad),
                                nullptr);
    return std::vector<MeshComponent> {component};
}


template<typename T>
std::vector<MeshComponent> Mesh<T>::genComponents(const Star& star) {

    Quad quad;
    MeshComponent component ( 
                                std::make_shared<Quad>(quad,
                                                       star.initialTransform()),
                                nullptr);
    return std::vector<MeshComponent> {component};
}

template<typename T>
std::vector<MeshComponent>
                    Mesh<T>::genComponents(
                                const std::shared_ptr<const Object>& obj,
                                const glm::vec3& position,
                                const JumperBallTypes::Direction& dir)
{

    Cube  cube(glm::vec3(1.f,215.f/255.f,0.f)); // Gold cube for the keys
    Sphere sphere(glm::vec3(1.f,215.f/255.f,0.f)); // Gold cube for the keys
    std::vector<MeshComponent> components;
    if (obj) {
        switch ( obj->getCategory() ) {
            case Object::CategoryOfObjects::Key: {
                constexpr size_t nbGeometriesToCreateAKey = 4;

                const std::array<glm::vec3,nbGeometriesToCreateAKey> scales
                { glm::vec3(0.075f,0.075f,0.075f),glm::vec3(0.05f,0.3f,0.05f),
                  glm::vec3(0.1f,0.05f,0.05f), glm::vec3(0.1f,0.05f,0.05f) };
                
                const std::array<glm::vec3,nbGeometriesToCreateAKey> 
                    translations
                { glm::vec3(0.3f+0.2f,   0.1f+0.35f,0.5f),
                  glm::vec3(0.3f+0.175f, 0.1f+0.0f, 0.475f),
                  glm::vec3(0.3f+0.175f, 0.1f+0.f,  0.475f),
                  glm::vec3(0.3f+0.175f, 0.1f+0.1f, 0.475f)
                };


                for (unsigned int i = 0; i <  nbGeometriesToCreateAKey; ++i ) {
                    const glm::mat4 scaleMatrix = glm::scale(scales.at(i));
                    const glm::mat4 translationMatrix =
                                        glm::translate(translations.at(i));
                    const glm::mat4 tranform = translationMatrix * scaleMatrix;
                    if (i == 0){
                    const MeshComponent componentSphere = 
                      {std::make_shared<Sphere>(sphere, tranform), 
                        std::make_shared<ObjectAnimation>(*obj,position,dir)};
                    components.push_back(componentSphere); 
                    } else {
                    const MeshComponent componentCube = 
                      {std::make_shared<Cube>(cube, tranform), 
                        std::make_shared<ObjectAnimation>(*obj,position,dir)};
                    components.push_back(componentCube); 
                    }
               }
                
                break;
            }
            case Object::CategoryOfObjects::Coin: {
              

                Cylinder cylinder(glm::vec3(1.f,215.f/255.f,0.f),
                                  //glm::vec3(0.f,0.f,0.f),
                                  glm::vec3(150.f/255.f,75.f/255.f,0.f));
                const glm::vec3 scale { 0.3f,0.3f,0.05f};
                
                const glm::vec3 translation {0.5f, 0.3f, 0.5f-0.025f};


                const glm::mat4 scaleMatrix = glm::scale(scale);
                const glm::mat4 translationMatrix = glm::translate(translation);
                const glm::mat4 rotationMatrix = 
                  glm::rotate(static_cast<float>(M_PI/2.),
                              glm::vec3(1.f,0.f,0.f));

                //We apply the rotation before the scale
                const glm::mat4 tranformLocal = 
                  translationMatrix *scaleMatrix * rotationMatrix;
                const glm::mat4 tranformNormals = rotationMatrix;

                const MeshComponent componentCube = 
                {std::make_shared<Cylinder>(cylinder, tranformLocal,
                        tranformNormals ), 
                std::make_shared<ObjectAnimation>(*obj,position,dir)};
                components.push_back(componentCube); 
                break;
            }
            case Object::CategoryOfObjects::Clock: {
                break;
            }
            default : break;
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

