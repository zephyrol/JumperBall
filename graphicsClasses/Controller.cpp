//
//  Controller.cpp
//  JumperBallApplication
//
//  Created by Sebastien Morgenthaler on 25/05/2020.
//

#include "Controller.h"
#include "cmath"

Controller::Controller():
_player(),
_menu(Menu::getJumperBallMenu(1,1.f/3.f)),
_buttonsStatuts{
    { Controller::Button::Up,       Controller::Status::Released },
    { Controller::Button::Down,     Controller::Status::Released },
    { Controller::Button::Right,    Controller::Status::Released },
    { Controller::Button::Left,     Controller::Status::Released },
    { Controller::Button::Escape,   Controller::Status::Released },
    { Controller::Button::Validate, Controller::Status::Released }},
_mouseCurrentXCoord(0.f),
_mouseCurrentYCoord(0.f),
_mousePressingXCoord(0.f),
_mousePressingYCoord(0.f),
_mouseIsPressed(false),
_requestToLeave(false),

_map(Map::loadMap(_player.levelProgression())),
_ball(std::make_shared<Ball>(*_map)),
_camera(std::make_shared<Camera>()),
_star(std::make_shared<Star>(glm::vec3(5.f,5.f,5.f),
                                    glm::vec3(0.f,3.f,3.f),
                                    0.3f,0.5f,50.f,5.f)),
_currentFrame(Controller::CurrentFrame::FrameA),
_sceneRenderingFrameA( std::make_shared<SceneRendering>
                  (*_map,*_ball,*_star,*_camera)),
_sceneRenderingFrameB( std::make_shared<SceneRendering>
                  (*_map,*_ball,*_star,*_camera)),
_menuRenderingFrameA( std::make_shared<MenuRendering>(*_menu)),
_menuRenderingFrameB( std::make_shared<MenuRendering>(*_menu)),
_updatingScene([this](size_t) {
    _ball->update();
    if (_player.statut() == Player::Statut::INMENU) {
        _camera->follow(*_map);
    } else if (_player.statut() == Player::Statut::INGAME) {
        _camera->follow(*_ball);
        if (_ball->stateOfLife() == Ball::StateOfLife::Dead) {
            _player.statut(Player::Statut::INMENU);
        }

    } else if (_player.statut() == Player::Statut::INTRANSITION){
        if (_camera->transitionEffect(*_ball, *_map)) {
            _player.statut(Player::Statut::INGAME);
        }
    }

    const std::shared_ptr<SceneRendering> currentSceneRendering = 
        _currentFrame == Controller::CurrentFrame::FrameA 
        ? _sceneRenderingFrameA
        : _sceneRenderingFrameB;
    currentSceneRendering->update();
}),
_updatingMenu([this](size_t) {
    if (_player.statut() == Player::Statut::INMENU) {
        _menu->update(_mouseIsPressed, _mouseCurrentYCoord);
    } else if (_player.statut() == Player::Statut::INGAME) {
        if (_ball->stateOfLife() == Ball::StateOfLife::Dead) {
            _player.statut(Player::Statut::INMENU);
            _menu->failurePageAsCurrentPage();
        }
    }
    const std::shared_ptr<MenuRendering> currentMenuRendering = 
        _currentFrame == Controller::CurrentFrame::FrameA 
        ? _menuRenderingFrameA
        : _menuRenderingFrameB;
    currentMenuRendering->update();
})
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
            manageValidateButton(status);
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

void Controller::runController()
{
    /*// Rendering updating
    if (_player.statut() == Player::Statut::INMENU) {
        _updatingMenuRendering.runTasks();
    }
    _sceneRendering->update();
    
    if (_player.statut() == Player::Statut::INMENU) {
        _updatingMenuRendering.waitTasks();
    }*/
    // Scene and Menu updating
    _updatingScene.runTasks();
    _updatingMenu.runTasks();

    // Launch rendering
    _currentFrame == Controller::CurrentFrame::FrameA
        ? _sceneRenderingFrameA->render()
        : _sceneRenderingFrameB->render();
    if (_player.statut() == Player::Statut::INMENU) {
        _currentFrame == Controller::CurrentFrame::FrameA
            ? _menuRenderingFrameA->render()
            : _menuRenderingFrameB->render();
    }
}

void Controller::waitController()
{
    _updatingScene.waitTasks();
    _updatingMenu.waitTasks();
    _currentFrame = _currentFrame == Controller::CurrentFrame::FrameA
        ? Controller::CurrentFrame::FrameB
        : Controller::CurrentFrame::FrameA;
}

void Controller::manageValidateButton(const Controller::Status &status) {
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::Jump);
        }
    }
}

void Controller::runGame(size_t level)
{
_map = Map::loadMap(level);
_ball = std::make_shared<Ball>(*_map);
_camera = std::make_shared<Camera>();
_star = std::make_shared<Star>(
            glm::vec3(1.f,1.f,1.f),glm::vec3(0.f,1.f,1.f) ,0.3f,0.5f,50.f,5.f);
_sceneRenderingFrameA = std::make_shared<SceneRendering>
        (*_map,*_ball,*_star,*_camera);
_sceneRenderingFrameB = std::make_shared<SceneRendering>
        (*_map,*_ball,*_star,*_camera);
}
void Controller::manageValidateMouse()
{
    if ( _player.statut() == Player::Statut::INGAME && _ball) {
        _ball->doAction(Ball::ActionRequest::Jump);
    }
    else if (_player.statut() == Player::Statut::INMENU ) {
        if(_menu->currentPage()) {
            const CstLabel_sptr label =
                _menu->currentPage()->matchedLabel(
                    _mousePressingXCoord,
                    _mousePressingYCoord);
            if (label) {
                Page_sptr newPage = _menu->currentPage()->child(label);
                if (newPage) {
                    _menu->currentPage(newPage);
                } else if  ( const std::shared_ptr< const
                        Label::LabelAnswer>& action = label->action()) {
                    _player.treatAction(*action);

                    //New level case
                    if (_player.statut() == Player::Statut::INTRANSITION ) {
                        runGame(action->chooseLevel+1);
                    }
                }
            }
        }
    }
}

void Controller::manageEscape(const Controller::Status &status) {
    if ( _player.statut() == Player::Statut::INMENU ) {
        if (status == Controller::Status::Released &&
            _buttonsStatuts.at(Button::Escape) == Status::Pressed) {
            _requestToLeave = true;
        }
    } else if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Released &&
            _buttonsStatuts.at(Button::Escape) == Status::Pressed) {
            _menu->pausePageAsCurrentPage();
            _player.statut(Player::Statut::INMENU);
        }
    }
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

void Controller::manageDown(const Controller::Status &) {
}

void Controller::manageUp(const Controller::Status &status) {
    if ( _player.statut() == Player::Statut::INGAME ) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::GoStraightAhead);
        }
    }
}

Controller::ScreenDirection Controller::nearestDirection
                                            (float posX, float posY) const {
    
    auto computeDistance = [] ( float x0, float y0, float x1, float y1) {
        return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2)) ;
    };
    
    Controller::ScreenDirection nearestDir = Controller::ScreenDirection::North;
    float computedDistance;
    float nearestDistance = computeDistance(_mousePressingXCoord,
                                            _mousePressingYCoord + 1.f,
                                            posX,posY);
    if ((computedDistance = computeDistance(_mousePressingXCoord,
                                            _mousePressingYCoord - 1.f,
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
    _mouseCurrentXCoord = posX;
    _mouseCurrentYCoord = posY;
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

        _mouseCurrentXCoord = posX;
        _mouseCurrentYCoord = posY;
    }
    
}

void Controller::releaseMouse (float posX, float posY) {
    
    constexpr float thresholdMoving = 0.05f;
    
    auto computeDistance = [] (float x0, float y0, float x1, float y1) {
        return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2)) ;
    };
    
    const float distance = computeDistance(_mousePressingXCoord,
                                           _mousePressingYCoord, posX, posY);
    if (distance < thresholdMoving) {
        manageValidateMouse();
    }
}


const std::shared_ptr<Menu>&  Controller::menu() const {
    return _menu;
}

void Controller::currentMap(const std::shared_ptr<Map> &currentMap)
{
    _map = currentMap;
}

const std::shared_ptr<Map> &Controller::currentMap() const
{
    return _map;
}

void Controller::currentBall(const std::shared_ptr<Ball> &currentBall)
{
    _ball = currentBall;
}

const std::shared_ptr<Ball> &Controller::currentBall() const
{
    return _ball;
}

bool Controller::requestToLeave() const {
    return _requestToLeave;
}
