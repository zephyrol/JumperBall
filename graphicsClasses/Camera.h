/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#ifndef CAMERA_H
#define CAMERA_H
#include <Utility.h>
#include <Ball.h>
#include <Map.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera {

public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Camera                        ();


    //---------CONSTANTS------------//
    static constexpr float        zNear                                 = 0.1f;
    static constexpr float        zFar                                  = 100.f;


    //-------CONST METHODS----------//
    const glm::vec3&              pos()                          const noexcept;
    const glm::vec3&              center()                       const noexcept;
    const glm::vec3&              up()                           const noexcept;
    glm::mat4                     viewProjection()               const noexcept;


    //----------METHODS-------------//
    void                          follow(const Ball& ball)             noexcept;
    void                          follow(const Map& map)               noexcept;
    bool                          transitionEffect(const Ball& ball,
                                                   const Map& map)     noexcept;


private:

    //--------ATTRIBUTES-----------//
    const float                   _fovy;
    const float                   _ratio;
    glm::vec3                     _pos;
    glm::vec3                     _center;
    glm::vec3                     _up;
    bool                          _willComeBack;
    bool                          _isComingBack;
    float                         _cameraAboveWay;
    JBTypes::timePointMs          _timePointComeBack;

};
#endif /* CAMERA_H */
