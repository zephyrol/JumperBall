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
        Map m (file);
        std::cout << "Map created" << std::endl;
        m.printMap();


        file.close();

        ClassicalMechanics newton;
        newton.printEulerBuffer();

        
        Ball b (m);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::TurnRight);
        /*b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::TurnLeft);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::TurnLeft);
        b.doAction(Ball::ActionRequest::TurnLeft);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::GoStraightOn);
        b.doAction(Ball::ActionRequest::GoStraightOn);*/
        Camera c;
        testClass t;
        Rendering rendering (m,b,c);
        t.run(rendering,b);
    }
    else {
        std::cout << "File not specified" << std::endl;        
    }
    
    
    return EXIT_SUCCESS;
}

