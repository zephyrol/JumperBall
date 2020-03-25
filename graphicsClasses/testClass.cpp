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
    
    /*_window = glfwCreateWindow(RESOLUTION_X,RESOLUTION_Y,
                                "JumperBall",glfwGetPrimaryMonitor(),
           nullptr);*/
    _window = glfwCreateWindow( RESOLUTION_X,RESOLUTION_Y,
                                "JumperBall",nullptr, nullptr);
    if( _window == nullptr ){
    std::cerr << "Failed to open GLFW window" << std::endl;
    glfwTerminate();
    }
    
    glfwMakeContextCurrent(_window);
    if (!gladLoadGL()) {
        std::cerr << "Unable to load OpenGL functions !" << std::endl;
        exit(EXIT_FAILURE);
    }

    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);  

    //alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //-----
}

void testClass::run(Rendering& r, Ball& b, Camera& c, Map& m) {
   

    bool leftButton   = false;
    bool rightButton  = false;
    bool upButton     = false;
    bool enterButton  = false;

    
    auto before = JumperBallTypesMethods::getTimePointMSNow();
    unsigned int counter = 0;
    m.printMap();
    glfwSetInputMode(_window,GLFW_STICKY_KEYS,GL_TRUE) ;
    while (glfwGetKey(_window,GLFW_KEY_ESCAPE) != GLFW_PRESS 
          && glfwWindowShouldClose(_window) == 0 ) {
        
        glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        b.update();
        c.follow(b);
        
        if( glfwGetKey(_window,GLFW_KEY_ENTER) == GLFW_PRESS || 
            glfwGetKey(_window,GLFW_KEY_SPACE) == GLFW_PRESS)
        {
                enterButton = true;
                b.doAction(Ball::ActionRequest::Jump);
        }

        if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_PRESS) {
            if (!rightButton) {
                rightButton = true;
                b.doAction(Ball::ActionRequest::TurnRight);
            }
        }
        
        if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_PRESS) {
            if (!leftButton) {
                leftButton = true;
                b.doAction(Ball::ActionRequest::TurnLeft);
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_PRESS) {
                upButton = true;
                b.doAction(Ball::ActionRequest::GoStraightAhead);
        }

        if(glfwGetKey(_window,GLFW_KEY_ENTER) == GLFW_RELEASE ||
            glfwGetKey(_window,GLFW_KEY_SPACE) == GLFW_RELEASE)
        {
            if (enterButton) {
                enterButton = false;
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_RELEASE) {
            if (rightButton) {
                rightButton = false;
            }
        }
        
        if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_RELEASE) {
            if (leftButton) {
                leftButton = false ;
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_RELEASE) {
            if (upButton) {
                upButton = false;
            }
        }


        b.update();
        c.follow(b);

        r.render();
       
        glfwSwapInterval(1);
        glfwSwapBuffers(_window);
      
        glfwPollEvents();

        counter++;
        auto after = JumperBallTypesMethods::getTimePointMSNow();
        auto diff = after - before;
        const std::chrono::duration<float> durationFloatDifference= diff;
        float diffF = durationFloatDifference.count();
        if (diffF > 1.f) {
            before = after;
            std::cout << counter << " FPS"  << std::endl;
            counter = 0;

        }

   }
}

testClass::~testClass() {
    glfwTerminate();
}
