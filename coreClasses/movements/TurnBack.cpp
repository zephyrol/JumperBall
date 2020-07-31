//
//  TurnBack.cpp
//  JumperBallApplication
//
//  Created by Sebastien Morgenthaler on 31/07/2020.
//

#include "TurnBack.h"

TurnBack::TurnBack(): 
Movement<JBTypes::Dir,1>(
[](const std::array<JBTypes::Dir,1>& dir) -> JBTypes::Dir{
    
    const unsigned int uintDir = JBTypesMethods::directionAsInteger(dir.at(0));
    unsigned int newDir; 
    if ( uintDir % 2 == 0) {
        newDir = uintDir + 1 ;
    } else {
        newDir = uintDir - 1 ;
    }
    return JBTypesMethods::integerAsDirection(newDir);
})
{}
