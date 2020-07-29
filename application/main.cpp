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
    
    
    if (argc == 3) {

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
            JBTypesMethods::displayInstallError();
            return EXIT_FAILURE;
        }
        const std::string arg (argv[2]);
        if (arg == "-compress") {
            Map::compress(file);
            std::ifstream mapFile ("outMap.txt");
            std::ifstream mapFile2 (fileToOpen);
            Map map (mapFile);
            map.verificationMap(mapFile2);
        }
        else {
            std::cerr << "ERROR: Unknown option" << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    GLFWwindow* window = initLibraries();
    
    testClass t(window);
    
    t.run();
    
    return EXIT_SUCCESS;
}

