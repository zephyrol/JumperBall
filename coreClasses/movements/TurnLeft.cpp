//
// TurnLeft.cpp
// JumperBallApplication
//
// Created by S.Morgenthaler on 30/07/2020.
//

#include "TurnLeft.h"

TurnLeft::TurnLeft():
    Movement <JBTypes::Dir, 2>(
        [] (const std::array <JBTypes::Dir, 2>& sideAndLook)->JBTypes::Dir {
            const JBTypes::vec3f side =
                JBTypesMethods::directionAsVector(sideAndLook.at(0));
            const JBTypes::vec3f look =
                JBTypesMethods::directionAsVector(sideAndLook.at(1));

            return JBTypesMethods::vectorAsDirection(
                JBTypesMethods::cross(side, look));
        })
{}
