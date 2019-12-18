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
#include <fstream>
#include <istream>
#include <sstream>
#include <unistd.h>


testClass::testClass(): _window(nullptr)
              
{


    if( !glfwInit() )
    {
      std::cerr << "Failed to init glfw" << std::endl;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    _window = glfwCreateWindow(1024,768,"JumperBall",nullptr,nullptr);

    if( _window == nullptr ){
    std::cerr << "Failed to open GLFW window" << std::endl;
    glfwTerminate();
    }
    
    glfwMakeContextCurrent(_window);
    if (!gladLoadGL()) {
        std::cerr << "Unable to load OpenGL functions !" << std::endl;
        exit(EXIT_FAILURE);
    }


    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
}

void testClass::run(Rendering& r, Ball& b, Camera& c) {
   

    bool leftButton = false;
    bool rightButton = false;
    bool upButton = false;
    bool enterButton = false;
    //unsigned int counter = 0;
    glfwSetInputMode(_window,GLFW_STICKY_KEYS,GL_TRUE) ;
    while (glfwGetKey(_window,GLFW_KEY_ESCAPE) != GLFW_PRESS 
          && glfwWindowShouldClose(_window) == 0 ) {

        //std::cout << "counter " << counter << std::endl;
        //counter++;
        glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_PRESS) {
            if (!rightButton) {
                rightButton = true;
            std::cout << "Key Right Press" << std::endl;
            b.doAction(Ball::ActionRequest::TurnRight);
            }
        }
        
        if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_PRESS) {
            if (!leftButton) {
                leftButton = true;
                std::cout << "Key Left Press" << std::endl;
                b.doAction(Ball::ActionRequest::TurnLeft);
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_PRESS) {
            if (!upButton) {
                upButton = true;
                std::cout << "Key Up Press" << std::endl;
                b.doAction(Ball::ActionRequest::GoStraightAhead);
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (!enterButton) {
                enterButton = true;
                std::cout << "Key Enter Press" << std::endl;
                b.doAction(Ball::ActionRequest::Jump);
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_RELEASE) {
            if (rightButton) {
                rightButton = false;
            std::cout << "Key Right Release" << std::endl;
            }
        }
        
        if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_RELEASE) {
            if (leftButton) {
                leftButton = false ;
                std::cout << "Key Left Release" << std::endl;
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_RELEASE) {
            if (upButton) {
                upButton = false;
                std::cout << "Key Up Release" << std::endl;
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_ENTER) == GLFW_RELEASE) {
            if (enterButton) {
                enterButton = false;
                std::cout << "Key Enter Release" << std::endl;
            }
        }

        b.update();
        c.follow(b);
        r.render();
       
        glfwSwapInterval(1);
        glfwSwapBuffers(_window);
        //glfwWaitEvents();
      
        glfwPollEvents();
   }
}

testClass::~testClass() {
    glfwTerminate();
}
