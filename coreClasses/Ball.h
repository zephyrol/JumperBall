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
    Ball                          (Map& map);


    //---------CONSTANTS------------//
    static constexpr float        timeToGetNextBlock                     = 0.2f;


    //------------TYPES------------//
    enum class State              { Staying, Moving, Jumping, 
                                    TurningLeft, TurningRight, Falling };

    enum class StateOfLife        { Normal, Bursting, Burning, Dead};

    enum class ActionRequest      { GoStraightAhead, TurnLeft, TurnRight, Jump};
    enum class AnswerRequest      { Accepted, Rejected };
    enum class NextBlockLocal     { Above, InFrontOf, Same, None };

    using shock     =             std::array<unsigned int, 3 > ; 
                                                      
    struct nextBlockInformation   { JumperBallTypes::Direction  nextSide;
                                    JumperBallTypes::Direction  nextLook;
                                    NextBlockLocal              nextLocal;
                                    unsigned int                poxX;
                                    unsigned int                poxY;
                                    unsigned int                poxZ;
                                  };

    //-------CONST METHODS------  --//
    JumperBallTypes::vec3f        get3DPosition()                const noexcept;
    float                         getRadius()                             const;
    JumperBallTypes::vec3f        lookTowardsAsVector()                   const;
    JumperBallTypes::vec3f        currentSideAsVector()                   const;

    JumperBallTypes::Direction    currentSide()                           const;
    JumperBallTypes::Direction    lookTowards()                           const;
    float                         distanceBehindBall()                    const;
    Ball::State                   state()                                 const;
    Ball::StateOfLife             stateOfLife()                           const;

    float                         getTimeSecondsSinceAction()    const noexcept;
    JumperBallTypes::timePointMs  getTimeActionMs()              const noexcept;

    float                         getTimeSecondsSinceStateOfLife()    
                                                                 const noexcept;
    JumperBallTypes::timePointMs  getTimeStateOfLifeMs()         const noexcept;

    struct nextBlockInformation   getNextBlockInfo()             const noexcept;
    const ClassicalMechanics&     getMechanicsJumping()          const noexcept;
    const ClassicalMechanics&     getMechanicsFalling()          const noexcept;



    //----------METHODS------------//
    void                          update()                             noexcept;
    AnswerRequest                 doAction ( ActionRequest action);


    //--------STATIC METHODS-------//
    static float                  getTimeNowSecondsFloat()             noexcept;
    
private:
    
    //--------ATTRIBUTES-----------//
    unsigned int                  _currentBlockX;
    unsigned int                  _currentBlockY;
    unsigned int                  _currentBlockZ;
    
    float                         _3DPosX;
    float                         _3DPosY;
    float                         _3DPosZ;

    JumperBallTypes::Direction    _currentSide;
    JumperBallTypes::Direction    _lookTowards;
    Ball::State                   _state;
    Ball::StateOfLife             _stateOfLife;

    //Through the interactions, a ball may modify a map
    Map&                          _map;

    ClassicalMechanics            _mechanicsPatternJumping;
    ClassicalMechanics            _mechanicsPatternFalling;

    std::chrono::time_point<std::chrono::system_clock>  
                                  _timeAction;

    std::chrono::time_point<std::chrono::system_clock>  
                                  _timeStateOfLife;

    //-------CONST METHODS--------//
    std::shared_ptr<const std::vector<int> >  
                                  intersectBlock(float x, float y, float z) 
                                                                          const;

    float                         getTimeActionSecondsFloat()    const noexcept;

    JumperBallTypes::vec3f        P2DTo3D(
                                      ClassicalMechanics::physics2DVector p2D)
                                                                          const;
    JumperBallTypes::vec3f        get3DPosStayingBall()                   const;

    //----------METHODS------------//
    void                          turnLeft()                           noexcept;
    void                          turnRight()                          noexcept;
    void                          goStraightAhead()                    noexcept;
    void                          stay()                               noexcept;
    void                          jump()                               noexcept;
    void                          move()                               noexcept;
    void                          fall()                               noexcept;
    void                          setTimeActionNow()                   noexcept;
    void                          mapInteraction()                     noexcept;
    AnswerRequest                 isFallingIntersectionBlock()         noexcept;
    AnswerRequest                 isGoingStraightAheadIntersectBlock() noexcept; 

};

#endif /* BALL_H */

