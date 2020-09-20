/*
 * File:   GraphicStar.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef GRAPHICSTAR_H
#define GRAPHICSTAR_H
#include "Star.h"


class GraphicStar
{
public:
    GraphicStar(const Star& star);

    void update();
    const glm::mat4&                     transform()                      const;
    const glm::vec3&                     centralPosition()                const;
    GLfloat                              radiusInside()                   const;
    GLfloat                              radiusOutside()                  const;
    const glm::vec3&                     colorInside()                    const;
    const glm::vec3&                     colorOutside()                   const;
    const glm::mat4&                     initialTransform()               const;

private:
    const Star& _star;
    glm::mat4 _transform;
    glm::vec3 _centralPosition;
    glm::vec3 _colorInside;
    glm::vec3 _colorOutside;
    GLfloat   _radiusInside;
    GLfloat   _radiusOutside;
    glm::mat4 _initialTransform;
};


#endif // GRAPHICSTAR_H
