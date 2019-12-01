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

    //--CONSTRUCTORS & DESTRUCTORS--//
    Ball                        (const Map& map);
    virtual                     ~Ball();

    //------------TYPES------------//
    enum class State            { Staying, Moving, Jumping };

    enum class ActionRequest    { GoStraightOn, TurnLeft, TurnRight, Jump};
    enum class AnswerRequest    { Accepted, Rejected };

    using timePointMs =         std::chrono::time_point <
                                    std::chrono::_V2::system_clock,
                                    std::chrono::duration <
                                    long int, std::ratio<1,1000> 
                                                          > 
                                                        >;
    using durationMs =          std::chrono::duration<long int,
                                                      std::ratio<1,1000> > ;



    //-------CONST METHODS--------//
    std::array<float,3>         get3DPosition()                  const noexcept;
    float                       getRadius()                               const;
    std::array<float,3>         lookTowardsThroughVector()                const;

    JumperBallTypes::Direction  currentSide()                             const;
    JumperBallTypes::Direction  lookTowards()                             const;

    void update() noexcept;


    //----------METHODS------------//
    AnswerRequest               doAction ( ActionRequest action);


    //--------STATIC METHODS-------//
    static timePointMs          getTimePointMSNow ()                   noexcept;
    static float                getTimeNowSecondsFloat()               noexcept;
    
private:
    
    //--------ATTRIBUTES-----------//
    unsigned int                _currentBlockX;
    unsigned int                _currentBlockY;
    unsigned int                _currentBlockZ;
    
    float                       _3DPosX;
    float                       _3DPosY;
    float                       _3DPosZ;

    JumperBallTypes::Direction  _currentSide;
    JumperBallTypes::Direction  _lookTowards;
    Ball::State                 _state;

    const Map&                  _map;

    ClassicalMechanics          _mechanicsPattern;

    std::chrono::time_point<std::chrono::system_clock>  _timeAction;


    //----------METHODS------------//
    void                        turnLeft()                             noexcept;
    void                        turnRight()                            noexcept;
    void                        goStraightOn()                         noexcept;
    void                        stay()                                 noexcept;
    void                        jump()                                 noexcept;
    void                        setTimeActionNow()                     noexcept;
    AnswerRequest               isFallingIntersectionBlock()           noexcept;


    //-------CONST METHODS--------//
    std::shared_ptr<const std::vector<int> >  
                                intersectBlock(float x, float y, float z) const;

    timePointMs                 getTimeActionMs()                const noexcept;
    float                       getTimeActionSecondsFloat()      const noexcept;
    float                       getTimeSecondsSinceAction()      const noexcept;

    std::array<float,3>         P2DTo3D(ClassicalMechanics::physics2DVector p2D)
                                                                          const;

        
};

#endif /* BALL_H */

