/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Types.h
 * Author: Morgenthaler S
 *
 * Created on 6 octobre 2019, 09:49
 */

#ifndef TYPES_H
#define TYPES_H
#define __STDC_WANT_LIB_EXT1__ 1
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <memory>
#include <chrono>
#define EPSILON_F 0.0001f
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
    #define M_PI_2 3.14159265358979323846/2.
#endif


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
    struct vec2f {float x; float y;};

    using timePointMs =         std::chrono::time_point <
                                    std::chrono::system_clock,
                                    std::chrono::duration <
                                    long int, std::ratio<1,1000> 
                                                          > 
                                                        >;
    using durationMs =          std::chrono::duration<long int,
                                                      std::ratio<1,1000> > ;
}


namespace JumperBallTypesMethods {
    JumperBallTypes::vec3f        directionAsVector 
                                              (JumperBallTypes::Direction dir);
    JumperBallTypes::Direction    integerAsDirection (unsigned int number);
    unsigned int                  directionAsInteger (JumperBallTypes::Direction
                                                      dir);
    JumperBallTypes::timePointMs  getTimePointMSNow ()                 noexcept;
    float                         getTimeSecondsSinceTimePoint( 
                                  const JumperBallTypes::timePointMs& timePoint)
                                                                       noexcept;
    float                         getFloatFromDurationMS(
                                    const JumperBallTypes::durationMs& dms );
    JumperBallTypes::timePointMs  getTimePointMsFromTimePoint( const 
       std::chrono::time_point<std::chrono::system_clock> & timePoint) noexcept;

}

#endif /* TYPES_H */
