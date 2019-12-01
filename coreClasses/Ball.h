/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.h
 * Author: seb
 *
 * Created on 1 octobre 2019, 21:17
 */

#ifndef BALL_H
#define BALL_H
#include "Types.h"
#include "Map.h"
#include "ClassicalMechanics.h"

class Ball {
public:
    //Ball();
    Ball(const Map& map);
    virtual ~Ball();

    enum class State { Staying, Moving, Jumping };

    enum class ActionRequest { GoStraightOn, TurnLeft, TurnRight, Jump};
    enum class AnswerRequest { Accepted, Rejected };

    AnswerRequest doAction ( ActionRequest action);

    using timePointMs = std::chrono::time_point<std::chrono::_V2::system_clock,
    std::chrono::duration<long int, std::ratio<1,1000> > > ;
    using durationMs = std::chrono::duration<long int,std::ratio<1,1000> > ;


    static timePointMs getTimePointMSNow ();

    std::array<float,3> get3DPos() const;
    float getRadius() const;
    std::array<float,3> lookTowardsThroughVector() const;

    JumperBallTypes::Direction  currentSide() const;
    JumperBallTypes::Direction  lookTowards() const;
    
private:
    unsigned int  _currentBlockX;
    unsigned int  _currentBlockY;
    unsigned int  _currentBlockZ;
    float         _3DPosX;
    float         _3DPosY;
    float         _3DPosZ;


    JumperBallTypes::Direction  _currentSide;
    JumperBallTypes::Direction  _lookTowards;
    Ball::State                 _state;

    ClassicalMechanics                                  _mechanicsPattern;
    std::chrono::time_point<std::chrono::system_clock>  _timeAction;

    const Map& _map;

    void turnLeft() noexcept;
    void turnRight() noexcept;
    void goStraightOn() noexcept;
    void stay() noexcept;
    void jump() noexcept;
    void updatePosition() noexcept;
    void setTimeActionNow() noexcept;
    AnswerRequest isFallingIntersectionBlock() noexcept;
    std::shared_ptr<const std::vector<int> >  intersectBlock (float x,
                                                              float y,
                                                              float z);
    timePointMs getTimeActionMs() noexcept;
    std::vector<float> P2DTo3D(ClassicalMechanics::physics2DVector p2D);

        
};

#endif /* BALL_H */

