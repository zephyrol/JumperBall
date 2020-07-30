//
//  TurnLeft.cpp
//  JumperBallApplication
//
//  Created by Sebastien Morgenthaler on 30/07/2020.
//

#include "TurnLeft.h"

TurnLeft::TurnLeft():
Movement<JBTypes::Dir,2>(
    
[](const std::array<JBTypes::Dir,2>& sideAndLook) -> JBTypes::Dir{
    
    
    switch (sideAndLook.at(0)) {
        case JBTypes::Dir::North:
            switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North: break;
                case JBTypes::Dir::South: break;
                case JBTypes::Dir::East:
                    return  JBTypes::Dir::Down; break;
                case JBTypes::Dir::West:
                    return  JBTypes::Dir::Up; break;
                case JBTypes::Dir::Up:
                    return  JBTypes::Dir::East; break;
                case JBTypes::Dir::Down:
                    return  JBTypes::Dir::West; break;
                default : break;
            }
            break;
        case JBTypes::Dir::South:
            switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North: break;
                case JBTypes::Dir::South: break;
                case JBTypes::Dir::East:
                    return  JBTypes::Dir::Up; break;
                case JBTypes::Dir::West:
                    return  JBTypes::Dir::Down; break;
                case JBTypes::Dir::Up:
                    return  JBTypes::Dir::West; break;
                case JBTypes::Dir::Down:
                    return  JBTypes::Dir::East; break;
                default : break;
            }
            break;
        case JBTypes::Dir::East:
            switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    return  JBTypes::Dir::Up; break;
                case JBTypes::Dir::South:
                    return  JBTypes::Dir::Down; break;
                case JBTypes::Dir::East: break;
                case JBTypes::Dir::West: break;
                case JBTypes::Dir::Up:
                    return  JBTypes::Dir::South; break;
                case JBTypes::Dir::Down:
                    return  JBTypes::Dir::North; break;
                default : break;
            }
            break;
        case JBTypes::Dir::West:
            switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    return  JBTypes::Dir::Down; break;
                case JBTypes::Dir::South:
                    return  JBTypes::Dir::Up; break;
                case JBTypes::Dir::East: break;
                case JBTypes::Dir::West: break;
                case JBTypes::Dir::Up:
                    return  JBTypes::Dir::North; break;
                case JBTypes::Dir::Down:
                    return  JBTypes::Dir::South; break;
                default : break;
            }
            break;
        case JBTypes::Dir::Up:
            switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    return  JBTypes::Dir::West; break;
                case JBTypes::Dir::South:
                    return  JBTypes::Dir::East; break;
                case JBTypes::Dir::East:
                    return  JBTypes::Dir::North; break;
                case JBTypes::Dir::West:
                    return  JBTypes::Dir::South; break;
                case JBTypes::Dir::Up: break;
                case JBTypes::Dir::Down: break;
                default : break;
            }
            break;
        case JBTypes::Dir::Down:
            switch (sideAndLook.at(1)) {
                case JBTypes::Dir::North:
                    return  JBTypes::Dir::East; break;
                case JBTypes::Dir::South:
                    return  JBTypes::Dir::West; break;
                case JBTypes::Dir::East:
                    return  JBTypes::Dir::South; break;
                case JBTypes::Dir::West:
                    return  JBTypes::Dir::North; break;
                case JBTypes::Dir::Up: break;
                case JBTypes::Dir::Down: break;
                default : break;
            }
            break;
        default :
            break;
    }
    return JBTypes::Dir::North;
})
{
    
}
