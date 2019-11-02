/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testClass.cpp
 * Author: seb
 * 
 * Created on 2 novembre 2019, 11:17
 */

#include "testClass.h"


testClass::testClass(): window(nullptr)
              
{

    if( !glfwInit() )
    {
      std::cerr << "Failed to init glfw" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow( 1024,768,"JumperBall",nullptr,nullptr);

    if( window == nullptr ){
    std::cerr << "Failed to open GLFW window" << std::endl;
    glfwTerminate();
    }

    glfwMakeContextCurrent(window);
}

void testClass::run() {

}


