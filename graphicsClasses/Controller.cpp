//
// Controller.cpp
// JumperBallApplication
//
// Created by Sebastien Morgenthaler on 25/05/2020.
//

#include "Controller.h"
#include "cmath"

Controller::Controller() : _ftContent(FontTexturesGenerator::initFreeTypeAndFont()),
                           _player(),
                           _menu(Menu::getJumperBallMenu(
                               _player,
                               1,
                               Utility::windowResolutionX,
                               Utility::windowResolutionY)),
                           _buttonsStatus{
                               {Controller::Button::Up, Controller::Status::Released},
                               {Controller::Button::Down, Controller::Status::Released},
                               {Controller::Button::Right, Controller::Status::Released},
                               {Controller::Button::Left, Controller::Status::Released},
                               {Controller::Button::Escape, Controller::Status::Released},
                               {Controller::Button::Validate, Controller::Status::Released}},
                           _currentKey(Scene::ActionKey::Nothing),
                           _mouseCurrentXCoord(0.f),
                           _mouseCurrentYCoord(0.f),
                           _mousePressingXCoord(0.f),
                           _mousePressingYCoord(0.f),
                           _mouseIsPressed(false),
                           _requestToLeave(false),
                           _scene(std::make_shared<Scene>(_player.levelProgression())),
                           _viewer(createViewer()),
                           _updatingSceneMenu([this](size_t)
                                {
                                    _player.status(_scene->update(_player.status(), _currentKey));
                                    _currentKey = Scene::ActionKey::Nothing;
                                    if (_player.status() == Player::Status::INMENU) {
                                        _menu->update(_mouseIsPressed, _mouseCurrentYCoord);
                                    }
                                    else if (_player.status() == Player::Status::INGAME) {
                                        if (_scene->gameIsFinished())
                                        {
                                            _player.status(Player::Status::INMENU);
                                            _menu->failurePageAsCurrentPage();
                                        }
                                    }
                                }),
                           _updating([this](size_t)
                                {
                                    _updatingSceneMenu.runTasks();
                                    _updatingSceneMenu.waitTasks();
                                    _viewer->update();
                                },
                                1, true)
{
    _updating.runTasks();
    _updating.waitTasks();
}

void Controller::interactionButtons (const Controller::Button& button, const Controller::Status& status) {

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
    _buttonsStatus[button] = status;
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
    _updating.runTasks();

    // Launch rendering
    _viewer->render();
}

void Controller::waitController() {
    _updating.waitTasks();
}

void Controller::manageValidateButton (const Controller::Status& status) {
    if (_player.status() == Player::Status::INGAME) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Validate;
        }
    }
}

void Controller::runGame (size_t level) {
    _scene = std::make_shared<Scene>(level);

    _viewer->freeGPUMemory();
    _viewer = createViewer();

    _updating.runTasks();
    _updating.waitTasks();
}

void Controller::manageValidateMouse() {
    if (_player.status() == Player::Status::INGAME) {
        _currentKey = Scene::ActionKey::Validate;
        return;
    }
    if (_player.status() == Player::Status::INMENU) {
        const Menu::MenuAnswer menuAnswer =
            _menu->mouseClick(_mousePressingXCoord, _mousePressingYCoord);

        if (menuAnswer.action == Menu::Action::GoLevel) {
            runGame(menuAnswer.newLevel);
        }
    }
}

std::shared_ptr<Viewer> Controller::createViewer() const {
    return std::make_shared<Viewer>(
        Utility::windowResolutionX,
        Utility::windowResolutionY,
        *_scene,
        *_menu,
        _ftContent);
}

void Controller::manageEscape (const Controller::Status& status) {
    if (_player.status() == Player::Status::INMENU) {
        if (
            status == Controller::Status::Released &&
                _buttonsStatus.at(Button::Escape) == Status::Pressed
            ) {
            if (_menu->escapeAction().action == Menu::Action::QuitGame) {
                _requestToLeave = true;
            }
        }
    } else if (_player.status() == Player::Status::INGAME) {
        if (
            status == Controller::Status::Released &&
                _buttonsStatus.at(Button::Escape) == Status::Pressed
            ) {
            _menu->pausePageAsCurrentPage();
            _player.status(Player::Status::INMENU);
        }
    }
}

void Controller::manageRight (const Controller::Status& status) {
    if (_player.status() == Player::Status::INGAME) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Right;
        }
    }
}

void Controller::manageLeft (const Status& status) {
    if (_player.status() == Player::Status::INGAME) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Left;
        }
    }
}

void Controller::manageDown (const Controller::Status&) {
}

void Controller::manageUp (const Controller::Status& status) {
    if (_player.status() == Player::Status::INGAME) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Up;
        }
    }
}

Controller::ScreenDirection Controller::nearestDirection (float posX, float posY) const {

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

    const float distance = computeDistance(_mousePressingXCoord, _mousePressingYCoord, posX, posY);
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

    const float distance = computeDistance(_mousePressingXCoord, _mousePressingYCoord, posX, posY);
    if (distance < thresholdMoving) {
        manageValidateMouse();
    }
}

bool Controller::requestToLeave() const {
    return _requestToLeave;
}

Controller::~Controller() {
    FontTexturesGenerator::clearFreeTypeRessources(_ftContent);
}

float Controller::computeDistance(float x0, float y0, float x1, float y1) {
    return sqrtf(powf(x1 - x0, 2.f) + powf(y1 - y0, 2.f));
}
