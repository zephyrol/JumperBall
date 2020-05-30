//
//  Controller.cpp
//  JumperBallApplication
//
//  Created by Sebastien Morgenthaler on 25/05/2020.
//

#include "Controller.h"
#include "cmath"

Controller::Controller(Player& player):
_player(player),
_ball(nullptr),
_buttonsStatuts{
    { Controller::Button::Up,       Controller::Status::Released },
    { Controller::Button::Down,     Controller::Status::Released },
    { Controller::Button::Right,    Controller::Status::Released },
    { Controller::Button::Left,     Controller::Status::Released },
    { Controller::Button::Escape,   Controller::Status::Released },
    { Controller::Button::Validate, Controller::Status::Released }},
_mousePressingXCoord(0.f),
_mousePressingYCoord(0.f),
_mouseIsPressed(false)
{
}

void Controller::interactionButtons(const Controller::Button& button,
                             const Controller::Status& status) {

    switch ( button ) {
        case Controller::Button::Up :
            manageUp(status);
            break;
        case Controller::Button::Down :
            manageDown(status);
            break;
        case Controller::Button::Left :
            manageLeft(status);
            break;
        case Controller::Button::Right :
            manageRight(status);
            break;
        case Controller::Button::Validate :
            manageValidate(status);
            break;
        case Controller::Button::Escape :
            manageEscape(status);
            break;
    }
    _buttonsStatuts[button] = status;
}


void Controller::interactionMouse(const Status& status, float posX, float posY){
    if( status == Controller::Status::Released) {
        if (_mouseIsPressed) {
            releaseMouse(posX,posY);
            _mouseIsPressed = false;
        }
    } else {
        if(!_mouseIsPressed) {
            pressMouse(posX,posY);
            _mouseIsPressed = true;
        }
    }
}

void Controller::manageValidate(const Controller::Status &status) {
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::Jump);
        }
    }
}


void Controller::manageEscape(const Controller::Status &status) {
    static_cast<void>(status);
}


void Controller::manageRight(const Controller::Status &status) {
    
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed &&
            _buttonsStatuts.at(Controller::Button::Right)
                == Controller::Status::Released &&
            _ball) {
            _ball->doAction(Ball::ActionRequest::TurnRight);
        }
    }
}

void Controller::manageLeft(const Status& status) {
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed &&
            _buttonsStatuts.at(Controller::Button::Right)
            == Controller::Status::Released &&
            _ball) {
            _ball->doAction(Ball::ActionRequest::TurnLeft);
        }
    }
}


void Controller::manageDown(const Controller::Status &status) {
    static_cast<void>(status);
}


void Controller::manageUp(const Controller::Status &status) {
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::GoStraightAhead);
        }
    }
}


void Controller::assignBall ( const std::shared_ptr<Ball>& ball ) {
    _ball = ball;
}

void Controller::pressMouse ( float posX, float posY ) {
    _mousePressingXCoord = posX;
    _mousePressingYCoord = posY;
}

void Controller::updateMouse ( float posX, float posY ) {
    
}

void Controller::releaseMouse ( float posX, float posY ) {
    
    constexpr float thresholdMoving = 0.1;
    const float distance = sqrtf(pow(posX - _mousePressingXCoord,2) +
                                 pow(posY - _mousePressingYCoord, 2)) ;
    if (distance < thresholdMoving) {
        manageValidate(Controller::Status::Pressed);
    }
}
