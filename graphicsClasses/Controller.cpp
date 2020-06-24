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
_menu(nullptr),
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
        default: break;
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
        } else {
            updateMouse(posX, posY);
        }
    }
}

void Controller::manageValidate(const Controller::Status &status) {
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::Jump);
        }
    }
    else if (_player.statut() == Player::Statut::INMENU) {
        if(_menu->currentPage()) {
            const std::shared_ptr<const Page> newPage =
                    _menu->currentPage()->child(_mousePressingXCoord,
                                                 _mousePressingYCoord);
            if (newPage) {
                _menu->currentPage(newPage);
            }
        }
    }
}


void Controller::manageEscape(const Controller::Status &status) {
    static_cast<void>(status);
}


void Controller::manageRight(const Controller::Status &status) {
    
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::TurnRight);
        }
    }
}

void Controller::manageLeft(const Status& status) {
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed && _ball) {
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

void Controller::assignMenu(const std::shared_ptr<Menu> &menu)
{
   _menu = menu;
}

Controller::ScreenDirection Controller::nearestDirection
                                            (float posX, float posY) const {
    
    auto computeDistance = [] ( float x0, float y0, float x1, float y1) {
        return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2)) ;
    };
    
    Controller::ScreenDirection nearestDir = Controller::ScreenDirection::North;
    float computedDistance;
    float nearestDistance = computeDistance(_mousePressingXCoord,
                                            _mousePressingYCoord - 1.f,
                                            posX,posY);
    if ((computedDistance = computeDistance(_mousePressingXCoord,
                                            _mousePressingYCoord + 1.f,
                                            posX, posY))
                                            <  nearestDistance ){
        nearestDistance= computedDistance;
        nearestDir = Controller::ScreenDirection::South;
    }
    if ((computedDistance = computeDistance(_mousePressingXCoord + 1.f,
                                                  _mousePressingYCoord,
                                                  posX, posY))
                                                  <  nearestDistance ){
        nearestDistance= computedDistance;
        nearestDir = Controller::ScreenDirection::East;
    }
    if ((computedDistance = computeDistance(_mousePressingXCoord - 1.f,
                                                 _mousePressingYCoord,
                                                 posX, posY))
                                                 <  nearestDistance ){
        nearestDistance= computedDistance;
        nearestDir = Controller::ScreenDirection::West;
    }
                            
    return nearestDir;
}

void Controller::pressMouse ( float posX, float posY ) {
    
    _mousePressingXCoord = posX;
    _mousePressingYCoord = posY;
}

void Controller::updateMouse ( float posX, float posY ) {
    
    constexpr float thresholdMoving = 0.05f;
    
    auto computeDistance = [] ( float x0, float y0, float x1, float y1) {
        return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2)) ;
    };
    
    const float distance = computeDistance(_mousePressingXCoord,
                                           _mousePressingYCoord, posX, posY);
    if (distance > thresholdMoving) {
        Controller::ScreenDirection sDir = nearestDirection(posX, posY);
        
        if (sDir == Controller::ScreenDirection::North) {
            manageUp(Controller::Status::Pressed);
        }
        else if (sDir == Controller::ScreenDirection::South) {
            manageDown(Controller::Status::Pressed);
        }
        if (sDir == Controller::ScreenDirection::East) {
            manageRight(Controller::Status::Pressed);
        }
        else if (sDir == Controller::ScreenDirection::West) {
            manageLeft(Controller::Status::Pressed);
        }
    }
    
}

void Controller::releaseMouse ( float posX, float posY ) {
    
    constexpr float thresholdMoving = 0.05f;
    
    auto computeDistance = [] ( float x0, float y0, float x1, float y1) {
        return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2)) ;
    };
    
    const float distance = computeDistance(_mousePressingXCoord,
                                           _mousePressingYCoord, posX, posY);
    if (distance < thresholdMoving) {
        manageValidate(Controller::Status::Pressed);
    }
}
