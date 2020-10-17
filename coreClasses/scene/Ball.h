/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:17
 */

#ifndef BALL_H
#define BALL_H
#include "ClassicalMechanics.h"
#include "movements/TurnLeft.h"
#include "movements/TurnRight.h"
#include "movements/TurnBack.h"
#include "movements/NextBlock.h"

class Ball {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Ball                          (Map& map);


    //---------CONSTANTS------------//
    static constexpr float        timeToGetNextBlock                    = 0.25f;
    static constexpr float        timeToTurn                             = 0.3f;
    static constexpr float        timeToBurn                              = 2.f;

    //------------TYPES------------//
    enum class State              { Staying, Moving, Jumping, 
                                    TurningLeft, TurningRight, Falling };

    enum class StateOfLife        { Normal, Bursting, Burning, Sliding, Dead };

    enum class JumpingType        { Short, Long };

    enum class ActionRequest      { GoStraightAhead, TurnLeft, TurnRight, Jump};
    enum class NextBlockLocal     { Above, InFrontOf, Same, None };

    using shock     =             std::array<unsigned int, 3 > ; 
                                                      
    struct nextBlockInformation   { JBTypes::Dir                nextSide;
                                    JBTypes::Dir                nextLook;
                                    NextBlockLocal              nextLocal;
                                    unsigned int                poxX;
                                    unsigned int                poxY;
                                    unsigned int                poxZ;
                                  };

    //-------CONST METHODS--------//
    JBTypes::vec3f                get3DPosition()                const noexcept;
    float                         getRadius()                             const;
    JBTypes::vec3f                lookTowardsAsVector()                   const;
    JBTypes::vec3f                currentSideAsVector()                   const;

    JBTypes::Dir                  currentSide()                           const;
    JBTypes::Dir                  lookTowards()                           const;
    Ball::State                   state()                                 const;
    Ball::StateOfLife             stateOfLife()                           const;
    float                         burnCoefficient()                       const;

    float                         getTimeSecondsSinceAction()    const noexcept;
    JBTypes::timePointMs          getTimeActionMs()              const noexcept;

    float                         getTimeSecondsSinceStateOfLife()
                                                                 const noexcept;
    JBTypes::timePointMs          getTimeStateOfLifeMs()         const noexcept;

    struct nextBlockInformation   getNextBlockInfo()             const noexcept;
    const ClassicalMechanics&     getMechanicsJumping()          const noexcept;
    const ClassicalMechanics&     getMechanicsFalling()          const noexcept;

    //----------METHODS------------//
    void                          update()                             noexcept;
    void                          doAction ( ActionRequest action);

private:
    
    //--------ATTRIBUTES-----------//
    unsigned int                  _currentBlockX;
    unsigned int                  _currentBlockY;
    unsigned int                  _currentBlockZ;
    
    float                         _3DPosX;
    float                         _3DPosY;
    float                         _3DPosZ;

    JBTypes::Dir                  _currentSide;
    JBTypes::Dir                  _lookTowards;
    Ball::State                   _state;
    Ball::StateOfLife             _stateOfLife;
    Ball::JumpingType             _jumpingType;

    //Through the interactions, a ball may modify a map
    Map&                          _map;

    ClassicalMechanics            _mechanicsPatternJumping;
    ClassicalMechanics            _mechanicsPatternLongJumping;
    ClassicalMechanics            _mechanicsPatternFalling;


    std::chrono::time_point<std::chrono::system_clock>  
                                  _timeAction;
    std::chrono::time_point<std::chrono::system_clock>  
                                  _timeStateOfLife;
    
    //BurnCoefficient at the last state change
    float                         _burnCoefficientTrigger;

    //current burnCoefficient
    float                         _burnCoefficientCurrent;

    bool                          _jumpRequest;
    JBTypes::timePointMs          _timeJumpRequest;

    //-------CONST METHODS--------//
    std::shared_ptr<const std::vector<int> >  
                                  intersectBlock(float x, float y, float z) 
                                                                          const;

    JBTypes::vec3f                P2DTo3D(
                                      ClassicalMechanics::physics2DVector p2D)
                                                                          const;
    JBTypes::vec3f                get3DPosStayingBall()                   const;
    bool                          isOutOfTheMap()                         const;

    //----------METHODS------------//
    void                          turnLeft()                           noexcept;
    void                          turnRight()                          noexcept;
    void                          goStraightAhead()                    noexcept;
    void                          stay()                               noexcept;
    void                          jump()                               noexcept;
    void                          move()                               noexcept;
    void                          fall()                               noexcept;
    void                          setTimeActionNow()                   noexcept;
    void                          setTimeLifeNow()                     noexcept;
    void                          mapInteraction()                     noexcept;
    void                          blockEvent(std::shared_ptr<Block> block)
                                                                       noexcept;
    void                          die()                                noexcept;
    ClassicalMechanics&           getMechanicsJumping()                noexcept;
    void                          isFallingIntersectionBlock()         noexcept;
    void                          isGoingStraightAheadIntersectBlock() noexcept;
    
    void                          fallingUpdate()                      noexcept;
    void                          stayingUpdate()                      noexcept;
    void                          movingUpdate()                       noexcept;
    void                          turningUpdate()                      noexcept;
    void                          jumpingUpdate()                      noexcept;
    void                          burningUpdate()                      noexcept;
    
    static const TurnLeft         turnLeftMovement;
    static const TurnRight        turnRightMovement;
    static const TurnBack         turnBackMovement;
    static const NextBlock        nextBlockGetter;
};

#endif /* BALL_H */

