//
//  TurnRight.cpp
//  JumperBallApplication
//
//  Created by Sebastien Morgenthaler on 30/07/2020.
//

#include "TurnRight.h"
#include "TurnLeft.h"
#include "TurnBack.h"

TurnRight::TurnRight():
Movement<JBTypes::Dir,2>(
    
[](const std::array<JBTypes::Dir,2>& sideAndLook) -> JBTypes::Dir{
    
    TurnLeft turnLeftMovement;
    TurnBack turnBackMovement;

    const JBTypes::Dir turnLeftDir = turnLeftMovement.evaluate(sideAndLook);
    return turnBackMovement.evaluate(std::array<JBTypes::Dir,1> {turnLeftDir});
})
{}
