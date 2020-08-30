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
#include <future>
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
    ~Mesh();
    
    //-------CONST METHODS----------//
    const glm::mat4&        world()                                       const;
    void                    render(const ShaderProgram& sp)               const;
    const T&                base()                                        const;
     
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

    //--------MULTITHREADING--------//
    bool                    _endOfTasksIsRequested;
    size_t                  _treatedComponents;
    std::mutex              _mutexComponentsInProgress;
    std::mutex              _mutexComponentsDone;
    std::vector<std::future<void> > _asyncTasksComponent;
    void                    treatComponent();
    std::vector<std::future<void> >genAsyncTasks();

};



template<typename T>
Mesh<T>::Mesh(const T& base):
  _base(base),
  _components(MeshGenerator::genComponents(base)),
  _world(1.f),
  _endOfTasksIsRequested(false),
  _treatedComponents(0),
  _mutexComponentsInProgress(),
  _mutexComponentsDone(),
  _asyncTasksComponent{}
{
    const size_t nbOfThreads = std::thread::hardware_concurrency();
    for (size_t i = 0; i < nbOfThreads; ++i){
    _asyncTasksComponent.push_back(
                std::async(std::launch::async,[this](){ treatComponent();}));
    }
    _mutexComponentsDone.lock(); // initialized as locked
}

template<typename T>
Mesh<T>::~Mesh()
{
    _endOfTasksIsRequested = true;
    for (std::future<void>& asyncTaskComponent: _asyncTasksComponent) {
        // Waiting the end of tasks
        _mutexComponentsInProgress.unlock();
        //std::cout << "wait first one" << std::endl;
        asyncTaskComponent.wait();
    }
    //std::cout <<"finish"<<std::endl;
}


template<typename T>
void Mesh<T>::update() {
    update(_base);
}

template<typename T>
void Mesh<T>::update(const Ball& base) {

    JBTypes::vec3f positionBall = base.get3DPosition();
    _world = glm::translate(glm::mat4(1.f), glm::vec3(positionBall.x,
                            positionBall.y ,positionBall.z));

    if (_components.size() > 0)
        _components.at(0).animation()->updateTrans();
}

template<typename T>
void Mesh<T>::treatComponent() {
    //std::cout << "thread created!" << std::endl;
    while(!_endOfTasksIsRequested) {
        _mutexComponentsInProgress.lock();
        //std::cout << "inProgress lock!" << std::endl;
        MeshComponent& component = _components.at(_treatedComponents);
        if (component.animation()) {
            component.animation()->updateTrans();
        }
        ++_treatedComponents;
        //std::cout << _treatedComponents << std::endl;
        if (_treatedComponents < _components.size()) {
            _mutexComponentsInProgress.unlock();
        } else {
            _mutexComponentsDone.unlock();
        }
    }
    _mutexComponentsInProgress.unlock();
}

/*template<typename T>
std::vector<std::future<void> > Mesh<T>::genAsyncTasks()
{
}*/

template<typename T>
void Mesh<T>::update(const Map&) {
    std::vector<std::future<void> > asyncUpdateTrans;
    //unsigned int nthreads = std::thread::hardware_concurrency();
    /*for(MeshComponent& component : _components){
        if (component.animation()) {
            //asyncUpdateTrans.push_back(std::async(&Animation::updateTrans,
                                                  //component.animation()));
            component.animation()->updateTrans(); //converge vers 3900
        }
    }*/
    //for (std::future<void>& asyncUpdateResult : asyncUpdateTrans) {
       //asyncUpdateResult.wait();
    //}


    _treatedComponents = 0;
    _mutexComponentsInProgress.unlock();
    //std::cout << "unlock in progress" << std::endl;
    _mutexComponentsDone.lock();
    //std::cout << "lock components" << std::endl;
}

template<typename T>
void Mesh<T>::update(const Quad&) {
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

template<typename T>
inline const T& Mesh<T>::base() const
{
    return _base;
}

#endif /* MESH_H */

