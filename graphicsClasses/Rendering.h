/*
 * File: Rendering.h
 * Author: Morgenthaler S
 *
 * Created on 9 avril 2021, 22h45
 */

#ifndef RENDERING_H
#define RENDERING_H
#include "process/RenderProcess.h"

class Rendering {

public:

    Rendering(GLsizei width, GLsizei height);

virtual void update() = 0;
virtual void freeGPUMemory() = 0;

virtual void render() const = 0;

protected:
template<typename T> using UniformVariableUpdatingFct = std::function <
    void (const Mesh::UniformVariables_uptr <T>&)
    >;

template<typename T> struct ExternalUniformVariables {
    Mesh::UniformVariables_uptr <T> uniformVariables;
    UniformVariableUpdatingFct <T> uniformVariablesUpdatingFct;
};

using UniformBlockUpdatingFct = std::function <void (const RenderPass::UniformBlockVariables_uptr&)>;
struct ExternalUniformBlockVariables {
    RenderPass::UniformBlockVariables_uptr uniformBlockVariables;
    UniformBlockUpdatingFct uniformBlockVariablesUpdatingFct;
};

protected:
const GLsizei _width;
const GLsizei _height;

};


#endif /* RENDERING_H */
