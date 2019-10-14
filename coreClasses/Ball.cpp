/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.cpp
 * Author: seb
 * 
 * Created on 1 octobre 2019, 21:17
 */

#include "Ball.h"

Ball::Ball(): _currentBlockX(0),
        _currentBlockY(0),
        _currentBlockZ(0),
        _3DPosX(0.f),
        _3DPosY(0.f),
        _3DPosZ(0.f),
        _currentSide(ballJumperTypes::Direction::Up),
        _lookTowards(ballJumperTypes::Direction::North)
{
}

/*Ball::Ball(const Ball& orig):
 {
 }*/

Ball::~Ball() {
}


void Ball::turnLeft() noexcept {
    switch (_currentSide) {
        case ballJumperTypes::Direction::North:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North: break;
                case ballJumperTypes::Direction::South: break;
                case ballJumperTypes::Direction::East: 
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::South:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North: break;
                case ballJumperTypes::Direction::South: break;
                case ballJumperTypes::Direction::East: 
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::East:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::East: break;
                case ballJumperTypes::Direction::West: break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::West:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::East: break;
                case ballJumperTypes::Direction::West: break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::Up:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                case ballJumperTypes::Direction::East:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                case ballJumperTypes::Direction::Up: break;
                case ballJumperTypes::Direction::Down: break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::Down:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                case ballJumperTypes::Direction::East:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                case ballJumperTypes::Direction::Up: break;
                case ballJumperTypes::Direction::Down: break;
                default : break;
            }
            break;
        default :
            break;
    }
}


void Ball::turnRight() noexcept {
    switch (_currentSide) {
        case ballJumperTypes::Direction::North:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North: break;
                case ballJumperTypes::Direction::South: break;
                case ballJumperTypes::Direction::East: 
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::South:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North: break;
                case ballJumperTypes::Direction::South: break;
                case ballJumperTypes::Direction::East: 
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::East:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::East: break;
                case ballJumperTypes::Direction::West: break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::West:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::Up; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::Down; break;
                case ballJumperTypes::Direction::East: break;
                case ballJumperTypes::Direction::West: break;
                case ballJumperTypes::Direction::Up:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                case ballJumperTypes::Direction::Down:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::Up:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                case ballJumperTypes::Direction::East:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                case ballJumperTypes::Direction::Up: break;
                case ballJumperTypes::Direction::Down: break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::Down:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    _lookTowards =  ballJumperTypes::Direction::West; break;
                case ballJumperTypes::Direction::South:
                    _lookTowards =  ballJumperTypes::Direction::East; break;
                case ballJumperTypes::Direction::East:
                    _lookTowards =  ballJumperTypes::Direction::North; break;
                case ballJumperTypes::Direction::West:
                    _lookTowards =  ballJumperTypes::Direction::South; break;
                case ballJumperTypes::Direction::Up: break;
                case ballJumperTypes::Direction::Down: break;
                default : break;
            }
            break;
        default :
            break;
    }
}

void Ball::goStraightOn() noexcept {
    
    int inFrontOfX = _currentBlockX;
    int inFrontOfY = _currentBlockY;
    int inFrontOfZ = _currentBlockZ;
    
    int leftX = _currentBlockX;
    int leftY = _currentBlockY;
    int leftZ = _currentBlockZ;
    
    int rightX = _currentBlockX;
    int rightY = _currentBlockY;
    int rightZ = _currentBlockZ;
    
    int aboveX = _currentBlockX;
    int aboveY = _currentBlockY;
    int aboveZ = _currentBlockZ;
    
    switch (_currentSide) {
        case ballJumperTypes::Direction::North:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North: break;
                case ballJumperTypes::Direction::South: break;
                case ballJumperTypes::Direction::East: 
                    ++inFrontOfX; ++aboveX; --aboveZ; --leftY; ++rightY; break;
                case ballJumperTypes::Direction::West:
                    --inFrontOfX; --aboveX; --aboveZ; ++leftY; --rightY; break;
                case ballJumperTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; --aboveZ; ++leftX; --rightX; break;
                case ballJumperTypes::Direction::Down:
                    --inFrontOfY; --aboveY; --aboveZ; --leftX; ++rightX; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::South:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North: break;
                case ballJumperTypes::Direction::South: break;
                case ballJumperTypes::Direction::East: 
                    ++inFrontOfX; ++aboveX; ++aboveZ; ++leftY; --rightY; break;
                case ballJumperTypes::Direction::West:
                    --inFrontOfX; --aboveX; ++aboveZ; --leftY; ++rightY; break;
                case ballJumperTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; ++aboveZ; --leftX; ++rightX; break;
                case ballJumperTypes::Direction::Down:
                    --inFrontOfY; --aboveY; ++aboveZ; ++leftX; --rightX; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::East:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; ++aboveX; ++leftY; --rightY; break;
                case ballJumperTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; ++aboveX; --leftY; ++rightY; break;
                case ballJumperTypes::Direction::East: break;
                case ballJumperTypes::Direction::West: break;
                case ballJumperTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; ++aboveX; --leftZ; ++rightZ; break;
                case ballJumperTypes::Direction::Down:
                    --inFrontOfY; --aboveY; ++aboveX; ++leftZ; --rightZ; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::West:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; --aboveX; --leftY; ++rightY; break;
                case ballJumperTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; --aboveX; ++leftY; --rightY; break;
                case ballJumperTypes::Direction::East: break;
                case ballJumperTypes::Direction::West: break;
                case ballJumperTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; --aboveX; ++leftZ; --rightZ; break;
                case ballJumperTypes::Direction::Down:
                    --inFrontOfY; --aboveY; --aboveX; --leftZ; ++rightZ; break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::Up:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; ++aboveY; --leftX; ++rightX; break;
                case ballJumperTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; ++aboveY; ++leftX; --rightX; break;
                case ballJumperTypes::Direction::East:
                    ++inFrontOfX; ++aboveX; ++aboveY; --leftZ; ++rightZ; break;
                case ballJumperTypes::Direction::West:
                    --inFrontOfX; --aboveX; ++aboveY; ++leftZ; --rightZ; break;
                case ballJumperTypes::Direction::Up: break;
                case ballJumperTypes::Direction::Down: break;
                default : break;
            }
            break;
        case ballJumperTypes::Direction::Down:
            switch (_lookTowards) {
                case ballJumperTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; --aboveY; ++leftX; --rightX; break;
                case ballJumperTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; --aboveY; --leftX; ++rightX; break;
                case ballJumperTypes::Direction::East:
                    ++inFrontOfX; ++aboveX; --aboveY; ++leftZ; --rightZ; break;
                case ballJumperTypes::Direction::West:
                    --inFrontOfX; --aboveX; --aboveY; --leftZ; ++rightZ; break;
                case ballJumperTypes::Direction::Up: break;
                case ballJumperTypes::Direction::Down: break;
                default : break;
            }
        default :
            break;
    }
}