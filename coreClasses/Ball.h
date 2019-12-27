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


    //---------CONSTANTS------------//
	  static constexpr float		  timeToGetNextBlock						           = 0.2f;


    //------------TYPES------------//
    enum class State            { Staying, Moving, Jumping, 
                                  TurningLeft, TurningRight };

    enum class ActionRequest    { GoStraightAhead, TurnLeft, TurnRight, Jump};
    enum class AnswerRequest    { Accepted, Rejected };
	  enum class NextBlockLocal	  { Above, InFrontOf, Same, None };

    using timePointMs =         std::chrono::time_point <
                                    std::chrono::_V2::system_clock,
                                    std::chrono::duration <
                                    long int, std::ratio<1,1000> 
                                                          > 
                                                        >;
    using durationMs =          std::chrono::duration<long int,
                                                      std::ratio<1,1000> > ;
    using shock     =           std::array<unsigned int, 3 > ; 
                                                    
	  struct nextBlockInformation { JumperBallTypes::Direction  nextSide;
								                  JumperBallTypes::Direction  nextLook;
								                  NextBlockLocal			        nextLocal;
								                  unsigned int				        poxX;
								                  unsigned int				        poxY;
								                  unsigned int				        poxZ;
								                };

    //-------CONST METHODS--------//
    std::array<float,3>         get3DPosition()                  const noexcept;
    float                       getRadius()                               const;
    std::array<float,3>         lookTowardsAsVector()                     const;

    JumperBallTypes::Direction  currentSide()                             const;
    JumperBallTypes::Direction  lookTowards()                             const;
    float                       distanceBehindBall()                      const;
    Ball::State                 state()                                   const;

    float                       getTimeSecondsSinceAction()      const noexcept;

	  struct nextBlockInformation getNextBlockInfo()					     const noexcept;



    //----------METHODS------------//
    void                        update()                               noexcept;
    AnswerRequest               doAction ( ActionRequest action);


    //--------STATIC METHODS-------//
    static timePointMs          getTimePointMSNow ()                   noexcept;
    static float                getTimeNowSecondsFloat()               noexcept;
    static float                getTimeSecondsSinceTimePoint(
                                    const timePointMs& timePoint)       noexcept;
    
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

    std::chrono::time_point<std::chrono::system_clock>  
                                _timeAction;

    //std::vector<shock>          _shocks; 

	
    //-------CONST METHODS--------//
    std::shared_ptr<const std::vector<int> >  
                                intersectBlock(float x, float y, float z) const;

    timePointMs                 getTimeActionMs()                const noexcept;
    float                       getTimeActionSecondsFloat()      const noexcept;

    std::array<float,3>         P2DTo3D(ClassicalMechanics::physics2DVector p2D)
                                                                          const;

    //----------METHODS------------//
    void                        turnLeft()                             noexcept;
    void                        turnRight()                            noexcept;
    void                        goStraightAhead()                      noexcept;
    void                        stay()                                 noexcept;
    void                        jump()                                 noexcept;
    void                        move()                                 noexcept;
    void                        setTimeActionNow()                     noexcept;
    AnswerRequest               isFallingIntersectionBlock()           noexcept;
    AnswerRequest               isGoingStraightAheadIntersectBlock()   noexcept; 




        
};

#endif /* BALL_H */

