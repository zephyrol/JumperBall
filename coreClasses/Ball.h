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

class Ball {
public:
    Ball();
    Ball(const Ball& orig);
    virtual ~Ball();

    enum class State { Staying, Moving, Jumping };

    enum class ActionRequest { GoStraightOn, TurnLeft, TurnRight, Jump};
    enum class AnswerRequest { Accepted, Rejected };

    AnswerRequest doAction ( ActionRequest action);
    
private:
    unsigned int _currentBlockX;
    unsigned int _currentBlockY;
    unsigned int _currentBlockZ;
    float _3DPosX;
    float _3DPosY;
    float _3DPosZ;


    JumperBallTypes::Direction _currentSide;
    JumperBallTypes::Direction _lookTowards;
    Ball::State _state;

    std::chrono::time_point<std::chrono::system_clock> _timeAction;

    std::shared_ptr<const Map> _map;

    void turnLeft() noexcept;
    void turnRight() noexcept;
    void goStraightOn() noexcept;
    void stay() noexcept;
    void jump() noexcept;
    void setTimeActionNow() noexcept;
        
};

#endif /* BALL_H */

