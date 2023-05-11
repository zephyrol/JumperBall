/*
 * File: Player.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */
#include <sstream>
#include "Player.h"
#include "system/SaveFileOutput.h"
#include "system/SoundStatusOutput.h"
#include "system/MusicStatusOutput.h"

Player::Player(
    DoubleChronometer_sptr doubleChronometer,
    unsigned int money,
    size_t levelProgression,
    std::vector<bool> ballSkins,
    unsigned int currentBallSkin,
    bool frenchLanguageIsActivated,
    bool musicsAreActivated,
    bool soundsAreActivated
) :
    _doubleChronometer(std::move(doubleChronometer)),
    _status(Player::Status::InMenu),
    _gameStatus(Player::GameStatus::None),
    _updateOutputs{},
    _money(money),
    _previousMoney(_money),
    _levelProgression(levelProgression),
    _ballSkins(std::move(ballSkins)),
    _currentBallSkin(currentBallSkin),
    _frenchLanguageIsActivated(frenchLanguageIsActivated),
    _musicsAreActivated(musicsAreActivated),
    _soundsAreActivated(soundsAreActivated),
    _currentLevel(_levelProgression),
    _remainingTime(0.f),
    _wantsToQuit(false),
    _needsSaveFile(false) {
}

size_t Player::levelProgression() const {
    return _levelProgression;
}

void Player::unlockNewLevel() {
}

void Player::decreaseMoney(unsigned int value) {
    _money -= value;
}

void Player::increaseMoney(unsigned int value) {
    _money += value;
}

unsigned int Player::getCurrentBallSkin() const {
    return _currentBallSkin;
}

unsigned int Player::getMoney() const {
    return _money;
}

unsigned int Player::getPreviousMoney() const {
    return _previousMoney;
}

Player::Status Player::status() const {
    return _status;
}

void Player::setCurrentLevel(size_t levelNumber) {
    _currentLevel = levelNumber;
    _status = Player::Status::InTransition;
}

size_t Player::getCurrentLevel() const {
    return _currentLevel;
}

bool Player::wantsToQuit() const {
    return _wantsToQuit;
}

void Player::requestQuit() {
    _wantsToQuit = true;
}

void Player::escapeAction() {
    if (_status != Player::Status::InMenu) {
        setAsInMenu();
    }
}

void Player::setAsWinner(unsigned int earnedMoney) {
    _needsSaveFile = true;
    setAsInMenu();
    if (_currentLevel == _levelProgression) {
        ++_levelProgression;
    }
    _gameStatus = GameStatus::Winner;
    _previousMoney = _money;
    _money += earnedMoney;
    constexpr unsigned int maxMoney = 9999;
    if (_money > maxMoney) {
        _money = maxMoney;
    }
}

bool Player::isAWinner() const {
    return _gameStatus == GameStatus::Winner;
}

void Player::setAsLoser() {
    _needsSaveFile = true;
    setAsInMenu();
    _gameStatus = GameStatus::Loser;
}

bool Player::isALoser() const {
    return _gameStatus == GameStatus::Loser;
}

void Player::resetGameStatus() {
    _gameStatus = GameStatus::None;
}

CstDoubleChronometer_sptr Player::getDoubleChronometer() const {
    return _doubleChronometer;
}

std::string Player::genSaveContent() {
    if (!_needsSaveFile) {
        return "";
    }
    _needsSaveFile = false;

    const auto boolVectorToString = [](const std::vector<bool> &boolVector) {
        return std::accumulate(
            boolVector.cbegin(),
            boolVector.cend(),
            std::string(),
            [](std::string &acc, bool booleanValue) {
                return acc + (booleanValue ? std::string("1") : std::string("0"));
            });
    };

    std::string saveContent =
        std::to_string(_money) + " "
        + std::to_string(_levelProgression) + " "
        + boolVectorToString(_ballSkins) + " "
        + std::to_string(_currentBallSkin) + " "
        + (_frenchLanguageIsActivated ? "1" : "0") + " "
        + (_musicsAreActivated ? "1" : "0") + " "
        + (_soundsAreActivated ? "1" : "0");

    return SaveFileOutput(std::move(saveContent)).getOutput();
}

Player_sptr Player::createInstance(DoubleChronometer_sptr doubleChronometer, const std::string &saveFile) {
    std::istringstream iss(saveFile);

    const auto readBooleanVector = [&iss]() {
        std::vector<bool> values;
        std::string w;
        iss >> w;
        for (const auto c: w) {
            values.push_back(c == '1');
        }
        return values;
    };

    const auto readBoolean = [&iss]() {
        std::vector<bool> values;
        std::string w;
        iss >> w;
        return w.front() == '1';
    };

    return std::make_shared<Player>(
        std::move(doubleChronometer),
        Player::readValue<unsigned int>(iss), // money
        Player::readValue<size_t>(iss), // levelProgression
        readBooleanVector(), //ballSkins
        Player::readValue<unsigned int>(iss), // currentBallSkin,
        readBoolean(), // frenchLanguageIsActivated
        readBoolean(), // musicsAreActivated
        readBoolean() //soundsAreActivated
    );
}


CstChronometer_sptr Player::getCreationChronometer() const {
    return _doubleChronometer->first();
}

void Player::setAsInGame() {
    if (_status != Player::Status::InGame) {
        _status = Player::Status::InGame;
        _doubleChronometer->resumeSecond();
    }
}

void Player::setAsInMenu() {
    if (_status != Player::Status::InMenu) {
        _status = Player::Status::InMenu;
        _doubleChronometer->stopSecond();
    }
}

void Player::setRemainingTime(float remainingTime) {
    _remainingTime = remainingTime;
}

float Player::getRemainingTime() const {
    return _remainingTime;
}

void Player::switchLangage() {
    _needsSaveFile = true;
    _frenchLanguageIsActivated = !_frenchLanguageIsActivated;
}

bool Player::isUsingEnglishLanguage() const {
    return !_frenchLanguageIsActivated;
}

void Player::switchMusicsStatus() {
    _needsSaveFile = true;
    _musicsAreActivated = !_musicsAreActivated;
    _updateOutputs.emplace_back(new MusicStatusOutput(_musicsAreActivated? "on": "off"));
}

bool Player::areMusicsActivated() const {
    return _musicsAreActivated;
}

void Player::switchSoundsStatus() {
    _needsSaveFile = true;
    _soundsAreActivated = !_soundsAreActivated;
    _updateOutputs.emplace_back(new SoundStatusOutput(_soundsAreActivated ? "on": "off"));
}

bool Player::areSoundsActivated() const {
    return _soundsAreActivated;
}

std::string Player::genOutputs() {
    return genSaveContent() + UpdateOutput::combineUpdateOutputs(std::move(_updateOutputs));
}
