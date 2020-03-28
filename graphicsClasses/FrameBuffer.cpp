/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FrameBuffer.cpp
 * Author: Morgenthaler S
 * 
 * Created on 29 janvier 2020, 21:51
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(bool HDRTexture, float scale) :
_fboHandle(),
_renderTexture(),
_isHDRTexture(HDRTexture),
_scale(scale),
_depthBuffer()
{
    constexpr GLsizei levelTexture  = 1;
    constexpr GLsizei mipmapLevel   = 0;

    glGenFramebuffers   (1,&_fboHandle);
    glGenTextures       (1,&_renderTexture);
    glGenRenderbuffers  (1,&_depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER,_fboHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _renderTexture);

    GLenum internalFormat; 
    if (_isHDRTexture) {
        internalFormat = GL_RGB32F;
    } else {
        internalFormat = GL_RGB8;
    }

    glTexStorage2D(GL_TEXTURE_2D,levelTexture,internalFormat,
                    RESOLUTION_X * scale ,RESOLUTION_Y * scale);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
                            GL_TEXTURE_2D, _renderTexture, mipmapLevel);

    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 
                            RESOLUTION_X * scale, RESOLUTION_Y * scale);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
                                GL_RENDERBUFFER, _depthBuffer);
    const GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1,&drawBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER,0);

}

void FrameBuffer::bindFrameBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::bindRenderTexture(unsigned int offset) const {
    glActiveTexture(GL_TEXTURE0+offset);
    glBindTexture(GL_TEXTURE_2D,_renderTexture);

}

void FrameBuffer::bindDefaultFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}


FrameBuffer::~FrameBuffer() {
}

GLuint FrameBuffer::getHandle() const{
    return _fboHandle;
}



std::pair<float,float> FrameBuffer::computeLogAverageLuminanceAndMax() const {

    constexpr float         epsilon             = 0.001f;
    constexpr unsigned int  levelOfDetail       = 0; //0 is the base image level
    constexpr unsigned int  numberOfComponents  = 3; //RGB
    const size_t            numberOfPixels      = RESOLUTION_X * RESOLUTION_Y
                                                  * _scale;
    std::vector<GLfloat>    textureData         (numberOfPixels * 
                                                  numberOfComponents);

    
    bindRenderTexture();
    glGetTexImage(GL_TEXTURE_2D, levelOfDetail, 
                  GL_RGB, GL_FLOAT, textureData.data());
    
    float sumLogLuminance = 0.f;
    float maxLuminance = 0.f;
    for ( unsigned int i = 0; i < numberOfPixels; ++i) {
        const float luminance = 
          Utility::getLuminance ( epsilon + glm::vec3(textureData.at(i),
                                            textureData.at(i+1),
                                            textureData.at(i+2)
                                            ));
        sumLogLuminance += logf ( luminance);
        if ( luminance > maxLuminance) maxLuminance = luminance;
    }

   return std::pair<float,float>(exp(sumLogLuminance/numberOfPixels),
                                  maxLuminance); 
}


