/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Animation.h
 * Author: Morgenthaler S
 *
 * Created on 5 janvier 2020, 11:19
 */

#ifndef ANIMATION_H
#define ANIMATION_H
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Animation {
public:
    
    //--CONSTRUCTORS & DESTRUCTORS--//
    Animation                   ();
    virtual ~Animation          ()                                           =0;

    //-------CONST METHODS----------//
    virtual glm::mat4           model()                                   const;
    virtual glm::mat4           scaleRotation()                           const;
    glm::mat4                   translation()                             const;

    //----------METHODS------------//
    virtual void                updateTrans()                                =0;

protected:
    
    //--------ATTRIBUTES-----------//
    glm::mat4                   _scale;
    glm::mat4                   _rotation;
    glm::mat4                   _translation;
};

#endif /* ANIMATION_H */

