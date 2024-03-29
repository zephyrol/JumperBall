//
// NextBlock.cpp
// JumperBallApplication
//
// Created by S.Morgenthaler on 30/07/2020.
//

#include "NextBlock.h"

NextBlock::NextBlock():
    Movement <std::array <int, 12>, 2>(

        [] (const std::array <JBTypes::Dir, 2>& sideAndLook)->std::array <int, 12> {

            int inFrontOfX = 0, inFrontOfY = 0, inFrontOfZ = 0;
            int leftX = 0, leftY = 0, leftZ = 0;
            int rightX = 0, rightY = 0, rightZ = 0;
            int aboveX = 0, aboveY = 0, aboveZ = 0;

            switch (sideAndLook.at(0)) {
            case JBTypes::Dir::North:
                switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North: break;
                case JBTypes::Dir::South: break;
                case JBTypes::Dir::East:
                    ++inFrontOfX; ++aboveX; --aboveZ; --leftY; ++rightY; break;
                case JBTypes::Dir::West:
                    --inFrontOfX; --aboveX; --aboveZ; ++leftY; --rightY; break;
                case JBTypes::Dir::Up:
                    ++inFrontOfY; ++aboveY; --aboveZ; ++leftX; --rightX; break;
                case JBTypes::Dir::Down:
                    --inFrontOfY; --aboveY; --aboveZ; --leftX; ++rightX; break;
                default: break;
                }
                break;
            case JBTypes::Dir::South:
                switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North: break;
                case JBTypes::Dir::South: break;
                case JBTypes::Dir::East:
                    ++inFrontOfX; ++aboveX; ++aboveZ; ++leftY; --rightY; break;
                case JBTypes::Dir::West:
                    --inFrontOfX; --aboveX; ++aboveZ; --leftY; ++rightY; break;
                case JBTypes::Dir::Up:
                    ++inFrontOfY; ++aboveY; ++aboveZ; --leftX; ++rightX; break;
                case JBTypes::Dir::Down:
                    --inFrontOfY; --aboveY; ++aboveZ; ++leftX; --rightX; break;
                default: break;
                }
                break;
            case JBTypes::Dir::East:
                switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    --inFrontOfZ; --aboveZ; ++aboveX; ++leftY; --rightY; break;
                case JBTypes::Dir::South:
                    ++inFrontOfZ; ++aboveZ; ++aboveX; --leftY; ++rightY; break;
                case JBTypes::Dir::East: break;
                case JBTypes::Dir::West: break;
                case JBTypes::Dir::Up:
                    ++inFrontOfY; ++aboveY; ++aboveX; --leftZ; ++rightZ; break;
                case JBTypes::Dir::Down:
                    --inFrontOfY; --aboveY; ++aboveX; ++leftZ; --rightZ; break;
                default: break;
                }
                break;
            case JBTypes::Dir::West:
                switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    --inFrontOfZ; --aboveZ; --aboveX; --leftY; ++rightY; break;
                case JBTypes::Dir::South:
                    ++inFrontOfZ; ++aboveZ; --aboveX; ++leftY; --rightY; break;
                case JBTypes::Dir::East: break;
                case JBTypes::Dir::West: break;
                case JBTypes::Dir::Up:
                    ++inFrontOfY; ++aboveY; --aboveX; ++leftZ; --rightZ; break;
                case JBTypes::Dir::Down:
                    --inFrontOfY; --aboveY; --aboveX; --leftZ; ++rightZ; break;
                default: break;
                }
                break;
            case JBTypes::Dir::Up:
                switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    --inFrontOfZ; --aboveZ; ++aboveY; --leftX; ++rightX; break;
                case JBTypes::Dir::South:
                    ++inFrontOfZ; ++aboveZ; ++aboveY; ++leftX; --rightX; break;
                case JBTypes::Dir::East:
                    ++inFrontOfX; ++aboveX; ++aboveY; --leftZ; ++rightZ; break;
                case JBTypes::Dir::West:
                    --inFrontOfX; --aboveX; ++aboveY; ++leftZ; --rightZ; break;
                case JBTypes::Dir::Up: break;
                case JBTypes::Dir::Down: break;
                default: break;
                }
                break;
            case JBTypes::Dir::Down:
                switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    --inFrontOfZ; --aboveZ; --aboveY; ++leftX; --rightX; break;
                case JBTypes::Dir::South:
                    ++inFrontOfZ; ++aboveZ; --aboveY; --leftX; ++rightX; break;
                case JBTypes::Dir::East:
                    ++inFrontOfX; ++aboveX; --aboveY; ++leftZ; --rightZ; break;
                case JBTypes::Dir::West:
                    --inFrontOfX; --aboveX; --aboveY; --leftZ; ++rightZ; break;
                case JBTypes::Dir::Up: break;
                case JBTypes::Dir::Down: break;
                default: break;
                }
            default:
                break;
            }

            return { inFrontOfX, inFrontOfY, inFrontOfZ, leftX, leftY, leftZ, rightX,
                     rightY, rightZ, aboveX, aboveY, aboveZ };
        })
{}
