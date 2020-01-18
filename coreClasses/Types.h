/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Types.h
 * Author: seb
 *
 * Created on 6 octobre 2019, 09:49
 */

#ifndef TYPES_H
#define TYPES_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#define EPSILON_F 0.0001f


namespace JumperBallTypes {
     
    enum class Direction{
        North,
        South,
        East,
        West,
        Up,
        Down 
    };

    struct vec3f {float x; float y; float z;};
}

namespace JumperBallTypesMethods {
    JumperBallTypes::vec3f directionAsVector (JumperBallTypes::Direction dir);
    JumperBallTypes::Direction integerAsDirection (unsigned int number);
}

#endif /* TYPES_H */

