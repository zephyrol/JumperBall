/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 septembre 2019, 20:52
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <Types.h>
#include <Ball.h>
#include <Map.h>
#include <ClassicalMechanics.h>
#include <Rendering.h>
#include "testClass.h"

using namespace std;

GLFWwindow* initLibraries() {
    GLFWwindow* window;
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
    window = glfwCreateWindow( RESOLUTION_X,RESOLUTION_Y,
                                "JumperBall",nullptr, nullptr);

    int widthWindow;
    int heightWindow;
    glfwGetFramebufferSize(window ,&widthWindow ,&heightWindow);

    Utility::windowResolutionX = widthWindow;
    Utility::windowResolutionY = heightWindow;

    if( window == nullptr ){
    std::cerr << "Failed to open GLFW window" << std::endl;
    glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cerr << "Unable to load OpenGL functions !" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!TextRendering::initFreeTypeAndFont()) {
        exit(EXIT_FAILURE);
    }
    return window;

}

int main(int argc, char** argv) {
    
    ifstream file;
    std::string fileToOpen;
    if (argc > 1) {
        fileToOpen = argv[1];
    }
    else {
        fileToOpen = "maps/map1.txt";
    }
    file.open(fileToOpen);  //Opening file to read
    if (!file) {
        std::cerr << "ERROR: Opening " << fileToOpen << " impossible .."
                  << std::endl;
        Utility::displayInstallError();
        return EXIT_FAILURE;
    }
    
    if (argc == 3) {
        const std::string arg (argv[2]);
        if (arg == "-compress")
            Map::compress(file);
        else {
            std::cerr << "ERROR: Unknown option" << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    GLFWwindow* window = initLibraries();
    
    Map m (file);
    std::cout << "Map created" << std::endl;
    
    file.close();
    

    std::shared_ptr<Ball> b = std::make_shared<Ball>(m);
    Camera c;
    
    //Defining t allows to create an OpenGl context
    testClass t(b,c,m,window);
    
    Star s (glm::vec3(1.f,1.f,1.f),glm::vec3(0.f,1.f,1.f)
            ,0.3f,0.5f,50.f,5.f);
    //Rendering rendering (m,*b,s,c);
    std::shared_ptr<Rendering> rendering =
            std::make_shared<Rendering>(m,*b,s,c);

    t.assignRenderingEngine(rendering);
    t.run();
    //t.runController(rendering,b,c);
    //t.runMenu(rendering,*b,c,m);
    
    return EXIT_SUCCESS;
}

