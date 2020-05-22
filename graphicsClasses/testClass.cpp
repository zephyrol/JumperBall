/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testClass.cpp
 * Author: Morgenthaler S
 * 
 * Created on 2 novembre 2019, 11:17
 */

#include <fstream>
#include <istream>
#include <sstream>
#include "testClass.h"
#include <initializer_list>


testClass::testClass(): _window(nullptr)
{
    if( !glfwInit() )
    {
      std::cerr << "Failed to init glfw" << std::endl;
    }
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    /*_window = glfwCreateWindow(RESOLUTION_X,RESOLUTION_Y,
                                "JumperBall",glfwGetPrimaryMonitor(),
           nullptr);*/
    _window = glfwCreateWindow( RESOLUTION_X,RESOLUTION_Y,
                                "JumperBall",nullptr, nullptr);
    
    int widthWindow;
    int heightWindow;
    glfwGetFramebufferSize(_window ,&widthWindow ,&heightWindow);
    
    Utility::windowResolutionX = widthWindow;
    Utility::windowResolutionY = heightWindow;

    if( _window == nullptr ){
    std::cerr << "Failed to open GLFW window" << std::endl;
    glfwTerminate();
    }
    
    glfwMakeContextCurrent(_window);
    if (!gladLoadGL()) {
        std::cerr << "Unable to load OpenGL functions !" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!TextRendering::initFreeTypeAndFont()) {
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

void testClass::run(Rendering& r, Ball& b, Camera& c) {
   
    bool leftButton   = false;
    bool rightButton  = false;
    bool upButton     = false;
    bool enterButton  = false;
    
    auto before = JumperBallTypesMethods::getTimePointMSNow();
    unsigned int counter = 0;
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

        if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_PRESS ||
           glfwGetKey(_window,GLFW_KEY_L) == GLFW_PRESS) {
            if (!rightButton) {
                rightButton = true;
                b.doAction(Ball::ActionRequest::TurnRight);
            }
        }
        
        if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_PRESS ||
           glfwGetKey(_window,GLFW_KEY_H) == GLFW_PRESS) {
            if (!leftButton) {
                leftButton = true;
                b.doAction(Ball::ActionRequest::TurnLeft);
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_PRESS ||
           glfwGetKey(_window,GLFW_KEY_K) == GLFW_PRESS) {
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

        if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_RELEASE ||
           glfwGetKey(_window,GLFW_KEY_L) == GLFW_RELEASE) {
            if (rightButton) {
                rightButton = false;
            }
        }
        
        if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_RELEASE ||
           glfwGetKey(_window,GLFW_KEY_H) == GLFW_RELEASE) {
            if (leftButton) {
                leftButton = false;
            }
        }

        if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_RELEASE || 
           glfwGetKey(_window,GLFW_KEY_K) == GLFW_RELEASE) {
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

void testClass::runMenu(Rendering& r, Ball& b, Camera& c, Map& m) {

    std::shared_ptr<const MessageLabel> label = 
        std::make_shared<const MessageLabel>(0.5f, 0.1f, 
            JumperBallTypes::vec2f{0.5f,0.8f},
            "Jumper Ball");
    std::shared_ptr<const MessageLabel> label2 = 
        std::make_shared<const MessageLabel> (0.2f,0.05f,
            JumperBallTypes::vec2f{0.5f,0.6f},
            "Play");
    std::shared_ptr<const MessageLabel> label3 = 
        std::make_shared<const MessageLabel> (0.3f,0.05f,
            JumperBallTypes::vec2f{0.5f,0.4f},
            "Store");
    std::shared_ptr<const MessageLabel> label4 = 
        std::make_shared<const MessageLabel> (0.2f,0.05f,
            JumperBallTypes::vec2f{0.5f,0.2f},
            "Exit");


    const std::vector<std::shared_ptr<const Label> > labels 
    {label, label2, label3, label4};


    const std::map<std::shared_ptr<const Label>,
        std::shared_ptr<const Page> > bridges;

    const std::shared_ptr<const Page> page =
        std::make_shared<const Page> (labels,bridges, nullptr,false);

    Menu menu(page);

    /*const TextRendering textRendering (
        {'a','B','e','J','l','m','p','r','u',' '},
        static_cast<unsigned int>(RESOLUTION_Y*label->height()));*/

    

  
    const ShaderProgram spLabels ( 
    Shader (GL_VERTEX_SHADER,   "shaders/fontVs.vs"),
    Shader (GL_FRAGMENT_SHADER, "shaders/fontFs.fs" ));
        

    glfwSetInputMode(_window,GLFW_STICKY_KEYS,GL_TRUE) ;
    while (glfwGetKey(_window,GLFW_KEY_ESCAPE) != GLFW_PRESS
           && glfwWindowShouldClose(_window) == 0 ) {
        
        glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //textRendering.render(spLabels,*label, glm::vec3(0,1.f,1.f));
        b.update();
        c.follow(m);
        
        r.render();
        menu.render(spLabels);
        
        glfwSwapInterval(1);
        glfwSwapBuffers(_window);
        
        glfwPollEvents();
        
    }
}



testClass::~testClass() {
    glfwTerminate();
    TextRendering::clearFreeTypeRessources();
}
