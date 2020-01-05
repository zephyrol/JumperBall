/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Animation.h
 * Author: seb
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
    Animation                   ();
    virtual ~Animation          ()                                           =0;
    glm::mat4                   model()                                   const;
    glm::mat4                   scaleRotation()                           const;
    glm::mat4                   translation()                             const;
    virtual void                updateTrans()                                =0;

protected:
    glm::mat4                   _scale;
    glm::mat4                   _rotation;
    glm::mat4                   _translation;
};

#endif /* ANIMATION_H */
