/*
 * File: Rendering.h
 * Author: Morgenthaler S
 *
 * Created on 9 avril 2021, 22h45
 */

#ifndef RENDERING_H
#define RENDERING_H
#include "Utility.h"

class Rendering {

public:

    Rendering(GLsizei width, GLsizei height);

virtual void update() = 0;
virtual void freeGPUMemory() = 0;

virtual void render() const = 0;
virtual ~Rendering() = default;

protected:
const GLsizei _width;
const GLsizei _height;

};


#endif /* RENDERING_H */
