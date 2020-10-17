/*
 * File:   StarState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "StarState.h"

StarState::StarState(const Star &star):
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

void StarState::update()
{
    _transform = _star.transform();
    _centralPosition = _star.centralPosition();
    _colorInside = _star.colorInside();
    _colorOutside = _star.colorOutside();
    _radiusInside = _star.radiusInside();
    _radiusOutside = _star.radiusOutside();
}

const glm::mat4& StarState::transform() const
{
    return _transform;
}

const glm::vec3& StarState::centralPosition() const
{
    return _centralPosition;
}

GLfloat StarState::radiusInside() const
{
   return _radiusInside;
}

GLfloat StarState::radiusOutside() const
{
  return _radiusOutside;
}

const glm::vec3 &StarState::colorInside() const
{
   return _colorInside;
}

const glm::vec3 &StarState::colorOutside() const
{
    return _colorOutside;
}

const glm::mat4 &StarState::initialTransform() const
{
   return _initialTransform;
}
