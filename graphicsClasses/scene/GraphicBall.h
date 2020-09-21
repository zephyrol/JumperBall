/*
 * File:   GraphicBall.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef GRAPHICBALL_H
#define GRAPHICBALL_H
#include <Ball.h>


class GraphicBall
{
public:
    GraphicBall(const Ball& ball);

    void update();
    const JBTypes::vec3f&         get3DPosition()                const noexcept;
    float                         burnCoefficient()                       const;
    const JBTypes::vec3f&         currentSideAsVector()                   const;
    float                         getTimeSecondsSinceStateOfLife()        const;
    float                         getTimeSecondsSinceAction()    const noexcept;
    Ball::State                   state()                                 const;
    Ball::StateOfLife             stateOfLife()                           const;
    float                         getRadius()                             const;
    float                         jumpingPosX()                           const;
    float                         fallingPosX()                           const;
    const JBTypes::vec3f&         lookTowardsAsVector()                   const;

private:

    const Ball&                   _ball;
    JBTypes::vec3f                _position;
    float                         _burnCoefficient;
    JBTypes::vec3f                _currentSideAsVector;
    float                         _timeSecondsSinceAction;
    float                         _timeSecondsSinceStateOfLife;
    Ball::State                   _state;
    Ball::StateOfLife             _stateOfLife;
    float                         _radius;
    float                         _jumpingPosX;
    float                         _fallingPosX;
    JBTypes::vec3f                _lookTowardsAsVector;
};

#endif // GRAPHICBALL_H
