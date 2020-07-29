/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Star.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 20 décembre 2019, 13:46
 */

#include "Star.h"

Star::Star( const glm::vec3& colorInside, const glm::vec3& colorOutside, 
            const GLfloat radiusInside,   const GLfloat radiusOutside, 
            const float distance,         const float radius):
        _colorInside(colorInside),
        _colorOutside(colorOutside),
        _radiusInside(radiusInside),
        _radiusOutside(radiusOutside),
        _initialTransform( glm::translate(glm::vec3(0.f,0.f,-distance))
                    * glm::scale(glm::vec3(radius,radius,radius))),
        _timeCreation(JBTypesMethods::getTimePointMSNow())
{
}

glm::vec3 Star::colorInside() const {
    return _colorInside;
}

glm::vec3 Star::colorOutside() const {
    return _colorOutside;
}

GLfloat Star::radiusInside() const {
    return _radiusInside;
}

GLfloat Star::radiusOutside() const {
    return _radiusOutside;
}

const JBTypes::timePointMs& Star::timeCreation() const {
  return _timeCreation; 
}

glm::mat4 Star::initialTransform() const {
  return _initialTransform;
}

glm::mat4 Star::transform() const {

    const float timeSinceCreation  = 
            JBTypesMethods::getTimeSecondsSinceTimePoint(_timeCreation);
    constexpr float timePerRound       = 20.f;
    return glm::rotate( timeSinceCreation * 2.f
                        *static_cast<float>(M_PI)/timePerRound,
                        glm::vec3(0.5f,1.f,0.f));

}

glm::vec3 Star::centralPosition() const {
    return glm::vec3( transform() * _initialTransform * 
            glm::vec4(0.5f,0.5f,0.f,1.f) );
}

