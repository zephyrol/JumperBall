//
// Controller.cpp
// JumperBallApplication
//
// Created by Sebastien Morgenthaler on 25/05/2020.
//

#include "Controller.h"
#include "cmath"

Controller::Controller(
    const size_t &screenWidth,
    const size_t &screenHeight,
    const JBTypes::FileContent &filesContent,
    const unsigned char *fontData,
    size_t fontDataSize,
    bool isUsingTouchScreen
) :
    _player(std::make_shared<Player>()),
    _menu(Menu::getJumperBallMenu(
        _player,
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight)
    )),
    _buttonsStatus{
        {Controller::Button::Up,       Controller::Status::Released},
        {Controller::Button::Down,     Controller::Status::Released},
        {Controller::Button::Right,    Controller::Status::Released},
        {Controller::Button::Left,     Controller::Status::Released},
        {Controller::Button::Escape,   Controller::Status::Released},
        {Controller::Button::Validate, Controller::Status::Released}
    },
    _currentEscapeStatus(Controller::Status::Released),
    _actionsWhenPressed{
        {Controller::Button::Up, [this]() { _scene->setUp(); }},
        {Controller::Button::Down, [this]() { _scene->setDown(); }},
        {Controller::Button::Left, [this]() { _scene->setLeft(); }},
        {Controller::Button::Right, [this]() { _scene->setRight(); }},
        {Controller::Button::Validate, [this]() { _scene->setValidate(); }},
    },
    _actionsMouseDirection{
        {Controller::ScreenDirection::North, _actionsWhenPressed.at(Controller::Button::Up)},
        {Controller::ScreenDirection::South, _actionsWhenPressed.at(Controller::Button::Down)},
        {Controller::ScreenDirection::West, _actionsWhenPressed.at(Controller::Button::Left)},
        {Controller::ScreenDirection::East, _actionsWhenPressed.at(Controller::Button::Right)},
    },
    _filesContent(filesContent),
    _mousePressingXCoord(0.f),
    _mousePressingYCoord(0.f),
    _mouseCurrentXCoord(0.f),
    _mouseCurrentYCoord(0.f),
    _mousePreviousXCoord(0.f),
    _mousePreviousYCoord(0.f),
    _mouseUpdatingTime(),
    _mousePressTime(),
    _currentMovementDir(nullptr),
    _mouseIsPressed(false),
    _requestToLeave(false),
    _scene(std::make_shared<Scene>(
        filesContent.at("map" + std::to_string(_player->levelProgression()) + ".txt"),
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
        _player,
        isUsingTouchScreen
    )),
    _viewer(std::make_shared<Viewer>(
        screenWidth,
        screenHeight,
        _scene,
        _menu->currentPage(),
        _filesContent,
        fontData,
        fontDataSize
    )) {
}

void Controller::interactionButtons(const Controller::Button &button, const Controller::Status &status) {
    _buttonsStatus[button] = status;
}

void Controller::interactionMouse(const Status &status, float posX, float posY) {
    if (status == Controller::Status::Released) {
        if (_mouseIsPressed) {
            releaseMouse(posX, posY);
        }
        return;
    }

    if (!_mouseIsPressed) {
        pressMouse(posX, posY);
        return;
    }
    updateMouse(posX, posY);
}

void Controller::runGame(size_t level) {
    _scene = std::make_shared<Scene>(
        _filesContent.at("map" + std::to_string(level) + ".txt"),
        _scene->getRatio(),
        _player,
        _scene->isUsingTouchScreen()
    );
    _viewer->setScene(_scene);
}

void Controller::setValidateMouse() {
    _scene->setValidateMouse();

    const auto &currentPage = _menu->currentPage();
    _menu->mouseClick(_mousePreviousXCoord, _mousePreviousYCoord);

    const auto &newPage = _menu->currentPage();
    if (newPage != currentPage) {
        _viewer->setPage(newPage);
    }

    if (_player->wantsToQuit()) {
        _requestToLeave = true;
        return;
    }
    if (_player->status() == Player::Status::InTransition) {
        runGame(_player->getCurrentLevel());
        return;
    }
}

void Controller::escapeAction() {

    const auto& escapeStatus = _buttonsStatus.at(Button::Escape);
    if (escapeStatus == Controller::Status::Released && _currentEscapeStatus == Status::Pressed ) {

        const auto &currentPage = _menu->currentPage();
        if (_menu->escapeAction()) {
            _requestToLeave = true;
            return;
        }

        const auto &newPage = _menu->currentPage();

        if (newPage != currentPage) {
            _viewer->setPage(newPage);
        }
    }
    _currentEscapeStatus = escapeStatus;
}

Controller::ScreenDirection Controller::nearestDirection(float posX, float posY) const {

    Controller::ScreenDirection nearestDir = Controller::ScreenDirection::North;
    float computedDistance;
    float nearestDistance = computeDistance(
        _mousePreviousXCoord,
        _mousePreviousYCoord + 1.f,
        posX,
        posY
    );
    if ((computedDistance = computeDistance(
        _mousePreviousXCoord,
        _mousePreviousYCoord - 1.f,
        posX,
        posY)) < nearestDistance) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::South;
    }
    if (
        (computedDistance = computeDistance(
            _mousePreviousXCoord + 1.f,
            _mousePreviousYCoord,
            posX,
            posY)) < nearestDistance) {
        nearestDistance = computedDistance;
        nearestDir = Controller::ScreenDirection::East;
    }
    if (computeDistance(
        _mousePreviousXCoord - 1.f,
        _mousePreviousYCoord,
        posX,
        posY
    ) < nearestDistance) {
        nearestDir = Controller::ScreenDirection::West;
    }
    return nearestDir;
}

void Controller::pressMouse(float posX, float posY) {
    _currentMovementDir = nullptr;
    _mousePressingXCoord = posX;
    _mousePressingYCoord = posY;
    _mousePreviousXCoord = posX;
    _mousePreviousYCoord = posY;
    _mouseCurrentXCoord = posX;
    _mouseCurrentYCoord = posY;
    _mouseUpdatingTime = JBTypesMethods::getTimePointMSNow();
    _mousePressTime = _mouseUpdatingTime;
    _mouseIsPressed = true;
}

void Controller::updateMouse(float posX, float posY) {

    const auto now = JBTypesMethods::getTimePointMSNow();
    if (!JBTypesMethods::floatsEqual(posX, _mouseCurrentXCoord)
        || !JBTypesMethods::floatsEqual(posY, _mouseCurrentYCoord)) {
        _mouseUpdatingTime = now;
    }

    _mouseCurrentXCoord = posX;
    _mouseCurrentYCoord = posY;

    constexpr float thresholdMoving = 0.05f;
    const float distance = computeDistance(_mousePreviousXCoord, _mousePreviousYCoord, posX, posY);

    const auto movementIsDetected = distance > thresholdMoving;
    if (movementIsDetected) {
        _currentMovementDir = std::make_shared<const Controller::ScreenDirection>(
            nearestDirection(posX, posY));
    }

    constexpr float updatingMouseThreshold = 300.f; // 0.3 seconds
    if (JBTypesMethods::getFloatFromDurationMS(now - _mouseUpdatingTime) > updatingMouseThreshold
        || movementIsDetected) {
        _mouseUpdatingTime = now;
        _mousePreviousXCoord = posX;
        _mousePreviousYCoord = posY;
    }

    if (!_currentMovementDir) {
        constexpr float goAheadDelay = 0.1f;
        if (JBTypesMethods::getFloatFromDurationMS(now - _mousePressTime) > goAheadDelay) {
            _scene->mouseSetUp();
        }
        return;
    }
    _actionsMouseDirection.at(*_currentMovementDir)();
}

void Controller::releaseMouse(float posX, float posY) {

    constexpr float thresholdMoving = 0.05f;

    const float distance = computeDistance(_mousePressingXCoord, _mousePressingYCoord, posX, posY);
    constexpr float pressTimeThreshold = 0.3f;
    if (distance < thresholdMoving
        && JBTypesMethods::getTimeSecondsSinceTimePoint(_mousePressTime) < pressTimeThreshold
        ) {
        setValidateMouse();
    }
    _mouseIsPressed = false;
}

bool Controller::requestToLeave() const {
    return _requestToLeave;
}

float Controller::computeDistance(float x0, float y0, float x1, float y1) {
    const auto x1MinusX0 = x1 - x0;
    const auto y1MinusY0 = y1 - y0;
    return sqrtf(x1MinusX0 * x1MinusX0 + y1MinusY0 * y1MinusY0);
}

void Controller::resize(size_t screenWidth, size_t screenHeight) {
    float ratio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    _scene->updateScreenRatio(ratio);
    _menu->resize(ratio);
    _viewer->resize(screenWidth, screenHeight);
}

void Controller::render() const {
    _viewer->render();
}

void Controller::update() {

    // 1. Update controls
    for (const auto &buttonFunction: _actionsWhenPressed) {
        if(_buttonsStatus[buttonFunction.first] == Controller::Status::Pressed) {
            buttonFunction.second();
        }
    }
    escapeAction();

    // 2. Update scene and menu
    const auto &currentPage = _menu->currentPage();
    _scene->update();
    _menu->update(_mouseIsPressed, _mouseCurrentYCoord);

    // 3. Update viewer
    const auto &newPage = _menu->currentPage();
    if (newPage != currentPage) {
        _viewer->setPage(newPage);
    }

    _viewer->update();
}
