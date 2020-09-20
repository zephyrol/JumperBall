/*
 * File:   GraphicStar.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "GraphicStar.h"

GraphicStar::GraphicStar(const Star &star):
    _star(star),
    _transform(star.transform()),
    _centralPosition(star.centralPosition()),
    _colorInside(star.colorInside()),
    _colorOutside(star.colorOutside()),
    _radiusInside(star.radiusInside()),
    _radiusOutside(star.radiusOutside()),
    _initialTransform(star.initialTransform())
{

}

void GraphicStar::update()
{
    _transform = _star.transform();
    _centralPosition = _star.centralPosition();
    _colorInside = _star.colorInside();
    _colorOutside = _star.colorOutside();
    _radiusInside = _star.radiusInside();
    _radiusOutside = _star.radiusOutside();
}

const glm::mat4& GraphicStar::transform() const
{
    return _transform;
}

const glm::vec3& GraphicStar::centralPosition() const
{
    return _centralPosition;
}

GLfloat GraphicStar::radiusInside() const
{
   return _radiusInside;
}

GLfloat GraphicStar::radiusOutside() const
{
  return _radiusOutside;
}

const glm::vec3 &GraphicStar::colorInside() const
{
   return _colorInside;
}

const glm::vec3 &GraphicStar::colorOutside() const
{
    return _colorOutside;
}

const glm::mat4 &GraphicStar::initialTransform() const
{
   return _initialTransform;
}
