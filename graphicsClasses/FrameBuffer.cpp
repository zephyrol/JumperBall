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

FrameBuffer::FrameBuffer(FrameBuffer::TextureCaterory category, 
    bool hasDepthBuffer, float scale) :
_fboHandle(),
_renderTexture(),
_textureCategory(category),
_hasDepthBuffer(hasDepthBuffer),
_depthBuffer(),
_scale(scale)
{
    constexpr GLsizei levelTexture  = 1;
    constexpr GLsizei mipmapLevel   = 0;

    glGenFramebuffers   (1,&_fboHandle);
    glGenTextures       (1,&_renderTexture);

    glBindFramebuffer(GL_FRAMEBUFFER,_fboHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _renderTexture);

    GLenum dataFormat;
    if (_textureCategory == FrameBuffer::TextureCaterory::SDR) {
        dataFormat = GL_RGB8;
        glTexStorage2D(GL_TEXTURE_2D, levelTexture, dataFormat,
               static_cast<GLsizei>(Utility::windowResolutionX * scale),
               static_cast<GLsizei>(Utility::windowResolutionY * scale));
    }
    else if (_textureCategory == FrameBuffer::TextureCaterory::HDR) {
        dataFormat = GL_RGB32F;
        glTexStorage2D(GL_TEXTURE_2D, levelTexture, dataFormat,
               static_cast<GLsizei>(Utility::windowResolutionX * scale),
               static_cast<GLsizei>(Utility::windowResolutionY * scale));
    }
    else {
        dataFormat = GL_DEPTH_COMPONENT16;
        glTexImage2D(GL_TEXTURE_2D, 0,dataFormat,
                     Utility::windowResolutionX , Utility::windowResolutionY,
                     0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    GLenum drawBuffer;
    if (_textureCategory == FrameBuffer::TextureCaterory::Depth){
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
                            GL_TEXTURE_2D, _renderTexture, mipmapLevel);
    drawBuffer = GL_NONE;
    }
    else {
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
                            GL_TEXTURE_2D, _renderTexture, mipmapLevel);
    drawBuffer = GL_COLOR_ATTACHMENT0;
    }
    if (hasDepthBuffer){
    glGenRenderbuffers(1,&_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 
                Utility::windowResolutionX, Utility::windowResolutionY);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
                                GL_RENDERBUFFER, _depthBuffer);
    }
    glDrawBuffers(1,&drawBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::bindFrameBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
    if (_textureCategory == FrameBuffer::TextureCaterory::SDR ||
            _textureCategory == FrameBuffer::TextureCaterory::HDR ){
        glClear(GL_COLOR_BUFFER_BIT);
    }
    if (_hasDepthBuffer) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    glViewport(0,0,static_cast<GLsizei>(Utility::windowResolutionX*_scale),
        static_cast<GLsizei>(Utility::windowResolutionY*_scale));
}

void FrameBuffer::bindRenderTexture(unsigned int offset) const {
    glActiveTexture(GL_TEXTURE0+offset);
    glBindTexture(GL_TEXTURE_2D,_renderTexture);

}

void FrameBuffer::bindDefaultFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

GLuint FrameBuffer::getHandle() const{
    return _fboHandle;
}

std::pair<float,float> FrameBuffer::computeLogAverageLuminanceAndMax() const {

    constexpr float         epsilon             = 0.001f;
    constexpr unsigned int  levelOfDetail       = 0; //0 is the base image level
    constexpr unsigned int  numberOfComponents  = 3; //RGB
    const size_t            numberOfPixels      =
        Utility::windowResolutionX * Utility::windowResolutionY;
    std::vector<GLfloat>    textureData         (numberOfPixels * 
                                                  numberOfComponents);

    
    bindRenderTexture();
    glGetTexImage(GL_TEXTURE_2D, levelOfDetail, 
                  GL_RGB, GL_FLOAT, textureData.data());
    
    float sumLogLuminance = 0.f;
    float maxLuminance = 0.f;
    for ( size_t i = 0; i < numberOfPixels; ++i) {
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


