//
// Controller.cpp
// JumperBallApplication
//
// Created by S.Morgenthaler on 25/05/2020.
//

#include "Controller.h"

Controller::Controller(const size_t& screenWidth,
                       const size_t& screenHeight,
                       const JBTypes::FileContent& filesContent,
                       const unsigned char* fontData,
                       size_t fontDataSize,
                       bool isUsingTouchScreen)
    : _doubleChronometer(std::make_shared<DoubleChronometer>(
          // The chronometer tracking creation time needs to be started directly
          true,
          // The chronometer tracking in game time needs to be started later
          false)),
      _player(Player::createInstance(_doubleChronometer, filesContent.at("save.txt"))),
      _filesContent(filesContent),
      _scene(std::make_shared<Scene>(
          filesContent.at("map" + std::to_string(_player->levelProgression()) + ".txt"),
          static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
          _player)),
      _menu(Menu::getJumperBallMenu(_player,
                                    _scene->getMap(),
                                    _scene->getBall(),
                                    _scene->getCamera(),
                                    static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
                                    isUsingTouchScreen)),
      _viewer(std::make_shared<Viewer>(static_cast<GLsizei>(screenWidth),
                                       static_cast<GLsizei>(screenHeight),
                                       _scene,
                                       _menu->currentPage(),
                                       _filesContent,
                                       fontData,
                                       fontDataSize)),
      _keyboardKey({
                       [this]() { _scene->setUp(); },
                       [this]() { _scene->setDown(); },
                       [this]() { _scene->setLeft(); },
                       [this]() { _scene->setRight(); },
                       [this]() { escapeAction(); },
                       [this]() { _scene->setValidate(); },
                   },
                   [this]() { _scene->setNoAction(); }),
      _mouse([this]() { _scene->setUp(); },
             [this]() { _scene->setDown(); },
             [this]() { _player->isLeftRightReversed() ? _scene->setRight() : _scene->setLeft(); },
             [this]() { _player->isLeftRightReversed() ? _scene->setLeft() : _scene->setRight(); },
             0.5f,
             0.f,  // South direction action function is always executed
             0.7f,
             0.7f,
             [this](float mouseX, float mouseY) { setValidateMouse(mouseX, mouseY); },
             [this]() { _scene->setNoAction(); }),
      _outputs{},
      _input(Controller::Input::Keyboard) {}

void Controller::interactionButtons(const KeyboardKey::Button& button, const KeyboardKey::Status& status) {
    if (status == KeyboardKey::Status::Pressed) {
        _keyboardKey.press(button);
        _input = Controller::Input::Keyboard;
        return;
    }
    _keyboardKey.release(button);
}

void Controller::pressMouse(float posX, float posY) {
    _input = Controller::Input::Mouse;
    _mouse.press(posX, posY);
}

void Controller::releaseMouse() {
    _mouse.release();
}

void Controller::runGame(size_t level) {
    _doubleChronometer->reset();
    _scene = std::make_shared<Scene>(_filesContent.at("map" + std::to_string(level) + ".txt"),
                                     _scene->getRatio(), _player);
    CstMovableObject_sptr movableObject = _scene->getBall();
    _menu->setBackgroundMap(_scene->getMap(), _scene->getBall(), _scene->getCamera());
    _viewer->setScene(_scene);
}

void Controller::setValidateMouse(float mouseX, float mouseY) {
    _input = Controller::Input::Mouse;
    const auto currentStatus = _player->status();
    _scene->setValidateMouse();

    const auto& currentPage = _menu->currentPage();
    const auto& currentLanguage = _player->isUsingEnglishLanguage();
    const auto& currentLeftRightStatus = _player->isLeftRightReversed();
    const auto& currentMusicsStatus = _player->areMusicsActivated();
    const auto& currentSoundsStatus = _player->areSoundsActivated();
    _menu->mouseClick(mouseX, mouseY);

    // Reset the viewer if the language has changed.
    const auto& newPage = _menu->currentPage();
    const auto newLanguage = _player->isUsingEnglishLanguage();
    const auto newLeftRightStatus = _player->isLeftRightReversed();
    const auto newMusicsStatus = _player->areMusicsActivated();
    const auto newSoundsStatus = _player->areSoundsActivated();

    if (newPage != currentPage || newLanguage != currentLanguage || newMusicsStatus != currentMusicsStatus ||
        newSoundsStatus != currentSoundsStatus || newLeftRightStatus != currentLeftRightStatus) {
        _viewer->setPage(newPage);
        _scene->setNoAction();
    }

    const auto newStatus = _player->status();
    if (newStatus != currentStatus && newStatus == Player::Status::InTransition) {
        runGame(_player->getCurrentLevel());
        return;
    }
}

void Controller::escapeAction() {
    const auto& currentPage = _menu->currentPage();
    if (_menu->escapeAction()) {
        _player->addQuitRequest();
        return;
    }
    const auto& newPage = _menu->currentPage();

    if (newPage != currentPage) {
        _viewer->setPage(newPage);
    }
}

void Controller::resize(int screenWidth, int screenHeight) {
    float ratio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    _scene->updateScreenRatio(ratio);
    _menu->resize(ratio);
    _viewer->resize(screenWidth, screenHeight);
}

void Controller::render() const {
    _viewer->render();
}

std::string Controller::update() {
    _viewer->resetRendering();
    // 1. Update chronometers
    const auto updatingTime = Chronometer::getTimePointMSNow();
    _doubleChronometer->update(updatingTime);
    _player->updateAdvertisementChronometer(updatingTime);

    // 2. Update controls
    if (_input == Controller::Input::Keyboard) {
        _keyboardKey.update();
    } else {
        _mouse.update(updatingTime);
    }

    // 3. Update scene and menu
    const auto& currentPage = _menu->currentPage();

    const auto sceneUpdateOutput = _scene->update();
    _menu->update(_mouse);

    const auto& newPage = _menu->currentPage();
    if (newPage != currentPage) {
        _viewer->setPage(newPage);
        if (newPage->isCompatibleWithAdvertisements()) {
            _player->checkAdvertisement();
        }
    }

    // 4. Update viewer
    _viewer->update();

    return sceneUpdateOutput + _player->genOutputs();
}

bool Controller::areMusicsActivated() const {
    return _player->areMusicsActivated();
}

bool Controller::areSoundsActivated() const {
    return _player->areSoundsActivated();
}

void Controller::stop() {
    _doubleChronometer->stopBoth();
    _player->stopChronometer();
}

void Controller::resume() {
    _player->resumeChronometer();
    if (_player->status() != Player::Status::InGame) {
        _doubleChronometer->resumeFirst();
        return;
    }
    _doubleChronometer->resumeBoth();
}
