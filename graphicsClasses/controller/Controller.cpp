//
// Controller.cpp
// JumperBallApplication
//
// Created by S.Morgenthaler on 25/05/2020.
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
    _chronometer(std::make_shared<Chronometer>()),
    _player(std::make_shared<Player>(_chronometer)),
    _menu(Menu::getJumperBallMenu(
        _player,
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight)
    )),
    _filesContent(filesContent),
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
    )),
    _keyboardKey({
                     {KeyboardKey::Button::Up,       [this]() { _scene->setUp(); }},
                     {KeyboardKey::Button::Down,     [this]() { _scene->setDown(); }},
                     {KeyboardKey::Button::Left,     [this]() { _scene->setLeft(); }},
                     {KeyboardKey::Button::Right,    [this]() { _scene->setRight(); }},
                     {KeyboardKey::Button::Validate, [this]() { _scene->setValidate(); }},
                     {KeyboardKey::Button::Escape,   [this]() { escapeAction(); }},
                 }),
    _mouse(
        [this]() { _scene->setUp(); },
        [this]() { _scene->setDown(); },
        [this]() { _scene->setRight(); },
        [this]() { _scene->setLeft(); },
        [this](float mouseX, float mouseY) { setValidateMouse(mouseX, mouseY); },
        [this]() { _scene->mouseSetUp(); }
    ) {
}

void Controller::interactionButtons(const KeyboardKey::Button &button, const KeyboardKey::Status &status) {
    status == KeyboardKey::Status::Pressed
    ? _keyboardKey.press(button)
    : _keyboardKey.release(button);
}

void Controller::pressMouse(float posX, float posY) {
    _mouse.press(posX, posY);
}

void Controller::releaseMouse() {
    _mouse.release();
}

void Controller::runGame(size_t level) {
    // TODO: uncomment
    _chronometer->reset();
    _scene = std::make_shared<Scene>(
        _filesContent.at("map" + std::to_string(level) + ".txt"),
        _scene->getRatio(),
        _player,
        _scene->isUsingTouchScreen()
    );
    _viewer->setScene(_scene);
}

void Controller::setValidateMouse(float mouseX, float mouseY) {
    const auto currentStatus = _player->status();
    _scene->setValidateMouse();

    const auto &currentPage = _menu->currentPage();
    _menu->mouseClick(mouseX, mouseY);

    const auto &newPage = _menu->currentPage();
    if (newPage != currentPage) {
        _viewer->setPage(newPage);
    }

    if (_player->wantsToQuit()) {
        _requestToLeave = true;
        return;
    }
    const auto newStatus = _player->status();
    if (newStatus != currentStatus && newStatus == Player::Status::InTransition) {
        runGame(_player->getCurrentLevel());
        return;
    }
}

void Controller::escapeAction() {

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

bool Controller::requestToLeave() const {
    return _requestToLeave;
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

std::string Controller::update() {

    // 1. Update chronometer
    _chronometer->update();

    // 2. Update controls
    _keyboardKey.update();
    _mouse.update();

    // 3. Update scene and menu
    const auto &currentPage = _menu->currentPage();

    const auto sceneUpdateOutput = _scene->update();
    _menu->update(_mouse);

    // 4. Update viewer
    const auto &newPage = _menu->currentPage();
    if (newPage != currentPage) {
        _viewer->setPage(newPage);
    }

    _viewer->update();

    return sceneUpdateOutput + _player->genSaveContent();
}

void Controller::stop() {
    _chronometer->stop();
}

void Controller::resume() {
    _chronometer->resume();
}