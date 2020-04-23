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
#include <testClass.h>

using namespace std;

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
        #ifdef _MSC_VER
        std::cerr << "Did you forget to install the files ?" << std::endl
         << "On Visual Studio, build the INSTALL solution" << std::endl;
        #else
        std::cerr << "Did you forget to install the project ?" << std::endl <<
            "Run \"cmake --build . --target install\" " <<
	    "or \"make install\"" << std::endl;
        #endif
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
    
    Map m (file);
    std::cout << "Map created" << std::endl;
    
    
    file.close();
    
    Ball b (m);
    Camera c;
    
    //Defining t allows to create an OpenGl context
    testClass t;
    
    Star s (glm::vec3(1.f,1.f,1.f),glm::vec3(0.f,1.f,1.f)
    ,0.3f,0.5f,50.f,5.f);
    Rendering rendering (m,b,s,c);
    t.run(rendering,b,c);
    
    
    return EXIT_SUCCESS;
}

