//
// Controller.cpp
// JumperBallApplication
//
// Created by Sebastien Morgenthaler on 25/05/2020.
//

#include "Controller.h"
#include "cmath"

Controller::Controller():
    _ftContent(FontTexturesGenerator::initFreeTypeAndFont()),
    _player(),
    _menu(Menu::getJumperBallMenu(
              _player,
              1,
              Utility::windowResolutionX,
              Utility::windowResolutionY)),
    _buttonsStatuts{
                    { Controller::Button::Up, Controller::Status::Released },
                    { Controller::Button::Down, Controller::Status::Released },
                    { Controller::Button::Right, Controller::Status::Released },
                    { Controller::Button::Left, Controller::Status::Released },
                    { Controller::Button::Escape, Controller::Status::Released },
                    { Controller::Button::Validate, Controller::Status::Released }},
    _mouseCurrentXCoord(0.f),
    _mouseCurrentYCoord(0.f),
    _mousePressingXCoord(0.f),
    _mousePressingYCoord(0.f),
    _mouseIsPressed(false),
    _requestToLeave(false),
    _map(MapGenerator::loadMap(_player.levelProgression())),
    _ball(std::make_shared <Ball>(*_map)),
    _camera(std::make_shared <Camera>(*_map, *_ball)),
    _star(Star::createBlurStar(*_map)),
    _sceneRendering(std::make_shared <SceneRendering>(*_map, *_ball, *_star, *_camera)),
    _menuRendering(std::make_shared <MenuRendering>(*_menu, _ftContent)),
    _updatingScene([this] (size_t) {
                       _ball->update();
                       if (_player.statut() == Player::Statut::INMENU) {
                           _camera->turnAroundMap();
                       } else if (_player.statut() == Player::Statut::INGAME) {
                           _camera->followBall();
                       } else if (_player.statut() == Player::Statut::INTRANSITION) {
                           _camera->approachBall();
                       }
                       _camera->update();
                       if (_camera->getMovement() == Camera::Movement::FollowingBall) {
                           _player.statut(Player::Statut::INGAME);
                       }
                       _sceneRendering->update();
                   }),
    _updatingMenu([this] (size_t) {
                      if (_player.statut() == Player::Statut::INMENU) {
                          _menu->update(_mouseIsPressed, _mouseCurrentYCoord);
                      } else if (_player.statut() == Player::Statut::INGAME) {
                          if (_ball->stateOfLife() == Ball::StateOfLife::Dead) {
                              _player.statut(Player::Statut::INMENU);
                              _menu->failurePageAsCurrentPage();
                          }
                      }
                      // TODO: Swap it !
                      _menuRendering->update();
                  }),
    _updating([this] (size_t) {
                  _updatingScene.runTasks();
                  _updatingMenu.runTasks();
                  _updatingMenu.waitTasks();
                  _updatingScene.waitTasks();
              }, 1, true) {
    _updating.runTasks();
    _updating.waitTasks();
    swapFrames();
    _updating.runTasks();
    _updating.waitTasks();
    swapFrames();
}

void Controller::swapFrames() {
    _sceneRendering->swapFrames();
}

void Controller::interactionButtons (const Controller::Button& button,
                                     const Controller::Status& status) {

    switch (button) {
    case Controller::Button::Up:
        manageUp(status);
        break;
    case Controller::Button::Down:
        manageDown(status);
        break;
    case Controller::Button::Left:
        manageLeft(status);
        break;
    case Controller::Button::Right:
        manageRight(status);
        break;
    case Controller::Button::Validate:
        manageValidateButton(status);
        break;
    case Controller::Button::Escape:
        manageEscape(status);
        break;
    default: break;
    }
    _buttonsStatuts[button] = status;
}


void Controller::interactionMouse (const Status& status, float posX, float posY) {
    if (status == Controller::Status::Released) {
        if (_mouseIsPressed) {
            releaseMouse(posX, posY);
            _mouseIsPressed = false;
        }
    } else {
        if (!_mouseIsPressed) {
            pressMouse(posX, posY);
            _mouseIsPressed = true;
        } else {
            updateMouse(posX, posY);
        }
    }
}

void Controller::runController() {
    // Scene and Menu updating
    // std::cout << "run tasks !" << std::endl;
    _updating.runTasks();

    // Launch rendering
    _sceneRendering->render();
    _menuRendering->render();
}

void Controller::waitController() {
    // std::cout << "wait tasks !" << std::endl;
    _updating.waitTasks();
    swapFrames();
}

void Controller::manageValidateButton (const Controller::Status& status) {
    if (_player.statut() == Player::Statut::INGAME) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::Jump);
        }
    }
}

void Controller::runGame (size_t level) {
    _map = MapGenerator::loadMap(level);
    _ball = std::make_shared <Ball>(*_map);
    _camera = std::make_shared <Camera>(*_map, *_ball);
    _star = Star::createBlurStar(*_map);
    _sceneRendering = std::make_shared <SceneRendering>(*_map, *_ball, *_star, *_camera);
    // TODO: check if that is necessary
    _updating.runTasks();
    _updating.waitTasks();
    swapFrames();
    _updating.runTasks();
    _updating.waitTasks();
    swapFrames();
}

void Controller::manageValidateMouse() {
    if (_player.statut() == Player::Statut::INGAME && _ball) {
        _ball->doAction(Ball::ActionRequest::Jump);
    } else if (_player.statut() == Player::Statut::INMENU) {
        const Menu::MenuAnswer menuAnswer =
            _menu->mouseClick(_mousePressingXCoord, _mousePressingYCoord);

        if (menuAnswer.action == Menu::Action::GoLevel) {
            runGame(menuAnswer.newLevel);
        }
    }
}

void Controller::manageEscape (const Controller::Status& status) {
    if (_player.statut() == Player::Statut::INMENU) {
        if (
            status == Controller::Status::Released &&
            _buttonsStatuts.at(Button::Escape) == Status::Pressed
            ) {
            if (_menu->escapeAction().action == Menu::Action::QuitGame) {
                _requestToLeave = true;
            }
        }
    } else if (_player.statut() == Player::Statut::INGAME) {
        if (
            status == Controller::Status::Released &&
            _buttonsStatuts.at(Button::Escape) == Status::Pressed
            ) {
            _menu->pausePageAsCurrentPage();
            _player.statut(Player::Statut::INMENU);
        }
    }
}

void Controller::manageRight (const Controller::Status& status) {
    if (_player.statut() == Player::Statut::INGAME) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::TurnRight);
        }
    }
}

void Controller::manageLeft (const Status& status) {
    if (_player.statut() == Player::Statut::INGAME) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::TurnLeft);
        }
    }
}

void Controller::manageDown (const Controller::Status&) {
}

void Controller::manageUp (const Controller::Status& status) {
    if (_player.statut() == Player::Statut::INGAME) {
        if (status == Controller::Status::Pressed && _ball) {
            _ball->doAction(Ball::ActionRequest::GoStraightAhead);
        }
    }
}

Controller::ScreenDirection Controller::nearestDirection
    (float posX, float posY) const {

    const auto computeDistance = [] ( float x0, float y0, float x1, float y1) {
                                     return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2));
                                 };

    Controller::ScreenDirection nearestDir = Controller::ScreenDirection::North;
    float computedDistance;
    float nearestDistance = computeDistance(_mousePressingXCoord,
                                            _mousePressingYCoord + 1.f,
                                            posX, posY);
    if (
        (computedDistance = computeDistance(_mousePressingXCoord,
                                            _mousePressingYCoord - 1.f,
                                            posX, posY))
        < nearestDistance
        ) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::South;
    }
    if (
        (computedDistance = computeDistance(_mousePressingXCoord + 1.f,
                                            _mousePressingYCoord,
                                            posX, posY))
        < nearestDistance
        ) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::East;
    }
    if (
        (computedDistance = computeDistance(_mousePressingXCoord - 1.f,
                                            _mousePressingYCoord,
                                            posX, posY))
        < nearestDistance
        ) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::West;
    }
    return nearestDir;
}

void Controller::pressMouse (float posX, float posY) {
    _mousePressingXCoord = posX;
    _mousePressingYCoord = posY;
    _mouseCurrentXCoord = posX;
    _mouseCurrentYCoord = posY;
}

void Controller::updateMouse (float posX, float posY) {

    constexpr float thresholdMoving = 0.05f;

    auto computeDistance = [] ( float x0, float y0, float x1, float y1) {
                               return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2));
                           };

    const float distance = computeDistance(_mousePressingXCoord,
                                           _mousePressingYCoord, posX, posY);
    if (distance > thresholdMoving) {
        Controller::ScreenDirection sDir = nearestDirection(posX, posY);

        if (sDir == Controller::ScreenDirection::North) {
            manageUp(Controller::Status::Pressed);
        } else if (sDir == Controller::ScreenDirection::South) {
            manageDown(Controller::Status::Pressed);
        }
        if (sDir == Controller::ScreenDirection::East) {
            manageRight(Controller::Status::Pressed);
        } else if (sDir == Controller::ScreenDirection::West) {
            manageLeft(Controller::Status::Pressed);
        }
        _mouseCurrentXCoord = posX;
        _mouseCurrentYCoord = posY;
    }
}

void Controller::releaseMouse (float posX, float posY) {

    constexpr float thresholdMoving = 0.05f;

    const auto computeDistance = [] (float x0, float y0, float x1, float y1) {
                                     return sqrtf(pow(x1 - x0, 2) + pow(y1 - y0, 2));
                                 };

    const float distance = computeDistance(_mousePressingXCoord,
                                           _mousePressingYCoord, posX, posY);
    if (distance < thresholdMoving) {
        manageValidateMouse();
    }
}


const std::shared_ptr <Menu>& Controller::menu() const {
    return _menu;
}

void Controller::currentMap (const std::shared_ptr <Map>& currentMap) {
    _map = currentMap;
}

const std::shared_ptr <Map>& Controller::currentMap() const {
    return _map;
}

void Controller::currentBall (const std::shared_ptr <Ball>& currentBall) {
    _ball = currentBall;
}

const std::shared_ptr <Ball>& Controller::currentBall() const {
    return _ball;
}

bool Controller::requestToLeave() const {
    return _requestToLeave;
}

Controller::~Controller() {
    FontTexturesGenerator::clearFreeTypeRessources(_ftContent);
}
