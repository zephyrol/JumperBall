/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: seb
 *
 * Created on 30 septembre 2019, 20:52
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "coreClasses/Types.h"
#include "coreClasses/Ball.h"
#include "coreClasses/Map.h"
#include "coreClasses/ClassicalMechanics.h"
#include "graphicsClasses/testClass.h"
#include "graphicsClasses/Rendering.h"

using namespace std;

int main(int argc, char** argv) {
    
    if (argc > 1 ) {
        ifstream file(argv[1]);  //Ouverture d'un fichier en lecture
        if (!file) {
          std::cerr << "ERROR: Opening " << argv[1] << " impossible .." 
                  << std::endl;
          return EXIT_FAILURE;
        }
        
        if (argc == 3) {
            const std::string arg (argv[2]);
            if (arg == "-compress")
                Map::compress(file);
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
                ,0.1f,0.5f,50.f,5.f);
        Rendering rendering (m,b,s,c);
        t.run(rendering,b,c,m);
    }
    else {
        std::cout << "File not specified" << std::endl;        
    }
    
    
    return EXIT_SUCCESS;
}

