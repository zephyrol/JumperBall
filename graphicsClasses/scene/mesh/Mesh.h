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
#include <system/ParallelTask.h>
#include <scene/Ball.h>
#include <scene/Map.h>
#include "Star.h"
#include "MeshGenerator.h"


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
    const BaseType&         _base;
    FrameType               _frame;
    void                    setWorld(const glm::mat4& world);
    virtual void            updateWorld() = 0;

private:
    //--------ATTRIBUTES-----------//
    vecMeshComponent_sptr   _components;
    vecMeshComponent_sptr   _animatedComponents;
    glm::mat4               _world;
    std::vector<glm::mat4>  _modelWorldTransforms;
    std::vector<glm::mat4>  _normalsTransforms;
    ParallelTask<void>      _componentsMapComputing;
    ParallelTask<void>      _componentsTransformsComputing;

    void                    updateComponents();
    void                    updateTransforms();
    virtual void            updateFrame();
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
_modelWorldTransforms(_components.size(),glm::mat4(1.f)),
_normalsTransforms(_components.size(),glm::mat4(1.f)),
_componentsMapComputing( [this](size_t componentNumber) -> void {

    const MeshComponent_sptr& component = _animatedComponents.at(componentNumber);
    component->animation()->updateTrans();
}, _animatedComponents.size()),
_componentsTransformsComputing( [this](size_t componentNumber) -> void {

    const MeshComponent_sptr& component = _components.at(componentNumber);
    glm::mat4 modelTransform = component->getShapeModelTransform();
    glm::mat4 normalsTransform = component->getShapeNormalsTransform();
    if(component->animation()) {
        modelTransform = component->getAnimationModel() * modelTransform;
        normalsTransform = component->getAnimationScaleRotation() *
            normalsTransform;
    }
    _modelWorldTransforms.at(componentNumber) = _world * modelTransform;
    _normalsTransforms.at(componentNumber) = normalsTransform;
}, _components.size())
{
}

template<typename BaseType, typename FrameType>
void Mesh<BaseType,FrameType>::update() {
    updateFrame();
    updateComponents();
    updateWorld();
    updateTransforms();
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

    GLuint currentVAO = 0;
    for( size_t i = 0; i < _components.size(); ++i) {

        sp.bindUniform("MW",_modelWorldTransforms.at(i));
        sp.bindUniform("N",_normalsTransforms.at(i));

        const MeshComponent_sptr& component = _components.at(i);
        if(currentVAO != *component->getShapeVAO()){
            component->bindShape();
            currentVAO = *component->getShapeVAO();
        }
        component->drawShape();
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

template<typename BaseType, typename FrameType>
void Mesh<BaseType, FrameType>::updateTransforms() 
{
    _componentsTransformsComputing.runTasks();
    _componentsTransformsComputing.waitTasks();
}


#endif /* MESH_H */
