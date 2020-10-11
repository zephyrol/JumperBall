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
#include <ParallelTask.h>


template<typename BaseType, typename FrameType>
class Mesh {

public:
    //--CONSTRUCTORS & DESTRUCTORS--//
    Mesh                    (const BaseType& base);

    //-------CONST METHODS----------//
    const glm::mat4&        world()                                       const;
    void                    render(const ShaderProgram& sp)               const;
    const FrameType&        getInstanceFrame()                            const;
    const BaseType&         getBase()                                     const;
    virtual void            update();

    //----------METHODS-------------//


protected:
    const BaseType& _base;
    FrameType   _frame;
    void setWorld(const glm::mat4& world);
    virtual void            updateWorld() = 0;

private:
    //--------ATTRIBUTES-----------//
    vecMeshComponent_sptr   _components;
    vecMeshComponent_sptr   _animatedComponents;
    glm::mat4               _world;
    ParallelTask<void>      _componentsMapComputing;

    void                    updateComponents();
    virtual void                    updateFrame();
    //----------METHODS-------------//
    static vecMeshComponent_sptr
                            getAnimatedComponents(
                                              vecMeshComponent_sptr components);

};

template<typename BaseType, typename FrameType>
Mesh<BaseType, FrameType>::Mesh(const BaseType& base):
_base(base),
_frame(base),
_components(MeshGenerator::genComponents(_frame)),
_animatedComponents(getAnimatedComponents(_components)),
_world(1.f),
_componentsMapComputing( [this](size_t componentNumber) -> void {
    const MeshComponent_sptr& component =
            _animatedComponents.at(componentNumber);
    component->animation()->updateTrans();
}, _animatedComponents.size())
{
}

template<typename BaseType, typename FrameType>
void Mesh<BaseType,FrameType>::update() {
    updateFrame();
    updateComponents();
    updateWorld();
}

template<typename BaseType, typename FrameType>
vecMeshComponent_sptr Mesh<BaseType,FrameType>::getAnimatedComponents
    (vecMeshComponent_sptr components)
{
    vecMeshComponent_sptr animatedComponents;
    for(const MeshComponent_sptr& component : components){
        if (component->animation()) {
            animatedComponents.push_back(component);
        }
    }
    return animatedComponents;
}

template<typename BaseType, typename FrameType>
const glm::mat4& Mesh<BaseType,FrameType>::world() const {
    return _world;
}

template<typename BaseType, typename FrameType>
void Mesh<BaseType,FrameType>::render(const ShaderProgram& sp) const {

    sp.bindUniform("W",_world);
    GLuint currentVAO = 0;
    for( CstMeshComponent_sptr component : _components) {
        glm::mat4 modelTransform(1.f);
        glm::mat4 normalsTransform(1.f);
        if(component->shape()) {
            modelTransform = component->shape()->modelTransform();
            normalsTransform = component->shape()->normalsTransform();
        }
        if(component->animation()) {
            modelTransform = component->animation()->model() * modelTransform;
            normalsTransform = component->animation()->scaleRotation() *
                    normalsTransform;
        }
        sp.bindUniform("M",modelTransform);
        sp.bindUniform("SR",normalsTransform);

        if(currentVAO != *component->shape()->vertexArrayObject()){
            component->shape()->bind();
            currentVAO = *component->shape()->vertexArrayObject();
        }
        component->shape()->draw();
    }
}

template<typename BaseType, typename FrameType>
const FrameType &Mesh<BaseType,FrameType>::getInstanceFrame() const
{
    return _frame;
}

template<typename BaseType, typename FrameType>
const BaseType &Mesh<BaseType,FrameType>::getBase() const
{
    return _base;
}

template<typename BaseType, typename FrameType>
void Mesh<BaseType,FrameType>::setWorld(const glm::mat4 &world)
{
    _world = world;
}

template<typename BaseType, typename FrameType>
void Mesh<BaseType,FrameType>::updateFrame()
{
    _frame.update();
}

template<typename BaseType, typename FrameType>
void Mesh<BaseType,FrameType>::updateComponents()
{
    _componentsMapComputing.runTasks();
    _componentsMapComputing.waitTasks();
}


#endif /* MESH_H */
