//
// Controller.cpp
// JumperBallApplication
//
// Created by Sebastien Morgenthaler on 25/05/2020.
//

#include "Controller.h"
#include "cmath"

Controller::Controller(
        const size_t& screenWidth,
        const size_t& screenHeight,
        const JBTypes::FileContent& filesContent,
        const unsigned char* fontData,
        size_t fontDataSize
        ) :
        _ftContent(FontTexturesGenerator::initFreeTypeAndFont(fontData, fontDataSize)),
        _player(),
        _menu(Menu::getJumperBallMenu(
            _player,
            1,
            static_cast<unsigned int>(screenWidth),
            static_cast<unsigned int>(screenHeight)
        )),
        _buttonsStatus{
        {Controller::Button::Up, Controller::Status::Released},
        {Controller::Button::Down, Controller::Status::Released},
        {Controller::Button::Right, Controller::Status::Released},
        {Controller::Button::Left, Controller::Status::Released},
        {Controller::Button::Escape, Controller::Status::Released},
        {Controller::Button::Validate, Controller::Status::Released}},
        _currentKey(Scene::ActionKey::Nothing),
        _filesContent(filesContent),
        _screenWidth(screenWidth),
        _screenHeight(screenHeight),
        _ratio(static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight)),
        _mousePressingXCoord(0.f),
        _mousePressingYCoord(0.f),
        _mouseCurrentXCoord(0.f),
        _mouseCurrentYCoord(0.f),
        _mousePreviousXCoord(0.f),
        _mousePreviousYCoord(0.f),
        _mouseUpdatingTime(),
        _currentMovementDir(nullptr),
        _mouseIsPressed(false),
        _requestToLeave(false),
        _scene(std::make_shared<Scene>(
                filesContent.at("map" + std::to_string(_player.levelProgression()) + ".txt"),
                _ratio
                )),
        _viewer(createViewer())
{
    updateSceneMenu();
}

void Controller::interactionButtons (const Controller::Button& button, const Controller::Status& status) {

    switch (button) {
        case Controller::Button::Up:
            setUp(status);
            break;
        case Controller::Button::Down:
            setDown(status);
            break;
        case Controller::Button::Left:
            setLeft(status);
            break;
        case Controller::Button::Right:
            setRight(status);
            break;
        case Controller::Button::Validate:
            setValidateButton(status);
            break;
        case Controller::Button::Escape:
            setEscape(status);
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
        return;
    }

    if (!_mouseIsPressed) {
        pressMouse(posX, posY);
        _mouseIsPressed = true;
        return;
    }
    updateMouse(posX, posY);
}
void Controller::setValidateButton (const Controller::Status& status) {
    if (_player.status() == Player::Status::InGame) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Validate;
        }
    }
}

void Controller::runGame (size_t level) {
    _scene = std::make_shared<Scene>(
        _filesContent.at("map" + std::to_string(level) + ".txt"),
        _ratio
    );
    refreshViewer();
    updateSceneMenu();
}

void Controller::setValidateMouse() {
    if (_player.status() == Player::Status::InGame) {
        _currentKey = Scene::ActionKey::Validate;
        return;
    }
    if (_player.status() == Player::Status::InMenu) {
        const Menu::MenuAnswer menuAnswer =
            _menu->mouseClick(_mousePreviousXCoord, _mousePreviousYCoord);

        if (menuAnswer.action == Menu::Action::GoLevel) {
            runGame(menuAnswer.newLevel);
        }
    }
}

std::shared_ptr<Viewer> Controller::createViewer() const {
    return std::make_shared<Viewer>(
        _screenWidth,
        _screenHeight,
        *_scene,
        *_menu,
        _ftContent,
        _filesContent
    );
}

void Controller::setEscape (const Controller::Status& status) {
    if (_player.status() == Player::Status::InMenu) {
        if (
            status == Controller::Status::Released &&
                _buttonsStatus.at(Button::Escape) == Status::Pressed
            ) {
            if (_menu->escapeAction().action == Menu::Action::QuitGame) {
                _requestToLeave = true;
            }
        }
    } else if (_player.status() == Player::Status::InGame) {
        if (
            status == Controller::Status::Released &&
                _buttonsStatus.at(Button::Escape) == Status::Pressed
            ) {
            _menu->pausePageAsCurrentPage();
            _player.status(Player::Status::InMenu);
        }
    }
}

void Controller::setRight (const Controller::Status& status) {
    if (_player.status() == Player::Status::InGame) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Right;
        }
    }
}

void Controller::setLeft (const Status& status) {
    if (_player.status() == Player::Status::InGame) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Left;
        }
    }
}

void Controller::setDown (const Controller::Status& status) {
    if (_player.status() == Player::Status::InGame) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Down;
        }
    }
}

void Controller::setUp (const Controller::Status& status) {
    if (_player.status() == Player::Status::InGame) {
        if (status == Controller::Status::Pressed) {
            _currentKey = Scene::ActionKey::Up;
        }
    }
}

Controller::ScreenDirection Controller::nearestDirection (float posX, float posY) const {

    Controller::ScreenDirection nearestDir = Controller::ScreenDirection::North;
    float computedDistance;
    float nearestDistance = computeDistance(_mousePreviousXCoord,
                                            _mousePreviousYCoord + 1.f,
                                            posX, posY);
    if (
        (computedDistance = computeDistance(_mousePreviousXCoord,
                                            _mousePreviousYCoord - 1.f,
                                            posX, posY))
        < nearestDistance
        ) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::South;
    }
    if (
        (computedDistance = computeDistance(_mousePreviousXCoord + 1.f,
                                            _mousePreviousYCoord,
                                            posX, posY))
        < nearestDistance
        ) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::East;
    }
    if (
        (computedDistance = computeDistance(_mousePreviousXCoord - 1.f,
                                            _mousePreviousYCoord,
                                            posX, posY))
        < nearestDistance
        ) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::West;
    }
    return nearestDir;
}

void Controller::pressMouse (float posX, float posY) {
    _currentMovementDir = nullptr;
    _mousePressingXCoord = posX;
    _mousePressingYCoord = posY;
    _mousePreviousXCoord = posX;
    _mousePreviousYCoord = posY;
    _mouseCurrentXCoord = posX;
    _mouseCurrentYCoord = posY;
    _mouseUpdatingTime = JBTypesMethods::getTimePointMSNow();
}

void Controller::updateMouse (float posX, float posY) {

    const auto now = JBTypesMethods::getTimePointMSNow();
    if( !JBTypesMethods::floatsEqual(posX, _mouseCurrentXCoord)
    || !JBTypesMethods::floatsEqual(posY, _mouseCurrentYCoord)) {
        _mouseUpdatingTime = now;
    }

    _mouseCurrentXCoord = posX;
    _mouseCurrentYCoord = posY;

    constexpr float thresholdMoving = 0.05f;
    const float distance = computeDistance(_mousePreviousXCoord, _mousePreviousYCoord, posX, posY);

    const auto movementIsDetected = distance > thresholdMoving;
    if (movementIsDetected) {
        _currentMovementDir = std::make_shared<const Controller::ScreenDirection>(nearestDirection(posX, posY));
    }

    constexpr float updatingMouseThreshold = 300.f; // 0.3 secondes
    if(JBTypesMethods::getFloatFromDurationMS(now - _mouseUpdatingTime) > updatingMouseThreshold
    || movementIsDetected ) {
        _mouseUpdatingTime = now;
        _mousePreviousXCoord = posX;
        _mousePreviousYCoord = posY;
    }

    if(!_currentMovementDir) {
        return;
    }
    if (*_currentMovementDir == Controller::ScreenDirection::North) {
        setUp(Controller::Status::Pressed);
        return;
    }
    if (*_currentMovementDir == Controller::ScreenDirection::South) {
        setDown(Controller::Status::Pressed);
        return;
    }
    if (*_currentMovementDir == Controller::ScreenDirection::East) {
        setRight(Controller::Status::Pressed);
        return;
    }
    if (*_currentMovementDir == Controller::ScreenDirection::West) {
        setLeft(Controller::Status::Pressed);
        return;
    }
}

void Controller::releaseMouse (float posX, float posY) {

    constexpr float thresholdMoving = 0.05f;

    const float distance = computeDistance(_mousePressingXCoord, _mousePressingYCoord, posX, posY);
    if (distance < thresholdMoving) {
        setValidateMouse();
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

void Controller::refreshViewer() {
    _viewer->freeGPUMemory();
    _viewer = createViewer();
}

void Controller::resize(size_t screenWidth, size_t screenHeight) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _ratio = static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight);
    _scene->updateScreenRatio(_ratio);
    //_menu = Menu::getJumperBallMenu(_player, 1, _screenWidth, _screenHeight),
    _menu->resize(_ratio);
    refreshViewer();
}

void Controller::updateSceneMenu() {

    _player.status(_scene->update(_player.status(), _currentKey));
    _currentKey = Scene::ActionKey::Nothing;
    if (_player.status() == Player::Status::InMenu) {
        _menu->update(_mouseIsPressed, _mouseCurrentYCoord);
    }
    else if (_player.status() == Player::Status::InGame) {
        if (_scene->gameIsWon()) {
            _player.status(Player::Status::InMenu);
            _menu->successPageAsCurrentPage();
            return;
        }
        if (_scene->gameIsLost()) {
            _player.status(Player::Status::InMenu);
            _menu->failurePageAsCurrentPage();
        }
    }
}

void Controller::updateViewer() {
    _viewer->update();
}

void Controller::render() const {
    _viewer->render();
}

void Controller::update() {
    updateSceneMenu();
    updateViewer();
}


