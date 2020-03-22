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
    FrameBuffer         (bool HDRTexture = false, float scale = 1.f);
    virtual             ~FrameBuffer();

    static constexpr float     
                        luminanceKey                                     = 0.4f;

    
    //-------CONST METHODS----------//
    GLuint              getHandle()                                       const;
    void                bindFrameBuffer()                                 const;
    void                bindRenderTexture(unsigned int offset = 0)        const;
    float               computeLogAverageLuminance()                      const;
    
    
    //--------STATIC METHODS-------//
    static void         bindDefaultFrameBuffer();


private:

    //--------ATTRIBUTES-----------//
    GLuint              _fboHandle;

    GLuint              _renderTexture;
    const bool          _isHDRTexture;
    const float         _scale;

    GLuint              _depthBuffer;

};

#endif /* FRAMEBUFFER_H */

