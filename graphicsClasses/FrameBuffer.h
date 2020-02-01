/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FrameBuffer.h
 * Author: seb
 *
 * Created on 29 janvier 2020, 21:51
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "ShaderProgram.h"

class FrameBuffer {

public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    FrameBuffer         ();
    virtual             ~FrameBuffer();

    
    //-------CONST METHODS----------//
    GLuint              getHandle()                                       const;
    void                bindFrameBuffer()                                 const;
    void                bindRenderTexture()                               const;
    void                unbind()                                          const;

private:

    //--------ATTRIBUTES-----------//
    GLuint              _fboHandle;
    GLuint              _renderTexture;
    GLuint              _depthBuffer;
};

#endif /* FRAMEBUFFER_H */

