/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Star.h
 * Author: Morgenthlaler S 
 *
 * Created on 20 décembre 2019, 13:46
 */

#ifndef STAR_H
#define STAR_H
#include "Utility.h"
#include <Ball.h>
#include <glm/glm.hpp>
#include "uniformBlocks/uniformLight.h"

class Star {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Star                        ( const glm::vec3& colorInside,
                                  const glm::vec3& colorOutside,
                                  const GLfloat radiusInside,
                                  const GLfloat radiusOutside,
                                  const float distance,         
                                  const float radius  );

    //-------CONST METHODS----------//
    void                          draw()                                  const;
    glm::vec3                     colorInside()                           const;
    glm::vec3                     colorOutside()                          const;
    GLfloat                       radiusInside()                          const;
    GLfloat                       radiusOutside()                         const;
    glm::mat4                     transform()                             const;
    glm::vec3                     centralPosition()                       const;
    

private:

    //--------ATTRIBUTES-----------//
    GLuint                        _idVertexArray;

    // We only have one buffer about vertices (the positions)
    GLuint                        _idVertexBuffer;

    const glm::vec3               _colorInside;
    const glm::vec3               _colorOutside;
    const GLfloat                 _radiusInside;
    const GLfloat                 _radiusOutside;
    glm::mat4                     _transform;

    JumperBallTypes::timePointMs  _timeCreation;
    
    //-------CONST METHODS----------//
    void                          bindVertexData()                        const;
    
};

#endif /* STAR_H */

