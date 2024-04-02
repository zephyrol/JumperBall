//
// TurnRight.cpp
// JumperBallApplication
//
// Created by S.Morgenthaler on 30/07/2020.
//

#include "TurnRight.h"

TurnRight::TurnRight():
    Movement <JBTypes::Dir, 2>(
        [] (const std::array <JBTypes::Dir, 2>& sideAndLook)->JBTypes::Dir {
            const glm::vec3 side =
                JBTypesMethods::directionAsVector(sideAndLook.at(0));
            const glm::vec3 look =
                JBTypesMethods::directionAsVector(sideAndLook.at(1));

            return JBTypesMethods::vectorAsDirection(
                JBTypesMethods::cross(look, side));
        })
{}
