/*
 * File: Player.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */
#include <sstream>
#include "Player.h"
#include "system/SaveFileOutput.h"

Player::Player(
    DoubleChronometer_sptr doubleChronometer,
    unsigned int money,
    size_t levelProgression,
    std::vector<bool> diamonds,
    unsigned int diamondsCounter,
    std::vector<bool> crystals,
    unsigned int crystalsCounter,
    unsigned int rollSpeedLevel,
    unsigned int currentRollSpeed,
    unsigned int jumpSpeedLevel,
    unsigned int currentJumpSpeed,
    unsigned int turningSpeedLevel,
    unsigned int currentTurningSpeed,
    unsigned int timeLevel,
    unsigned int clockItemLevel,
    unsigned int bonusLevel,
    std::vector<bool> ballSkins,
    unsigned int currentBallSkin,
    bool frenchLangageIsActivated
) :
    _doubleChronometer(std::move(doubleChronometer)),
    _status(Player::Status::InMenu),
    _gameStatus(Player::GameStatus::None),
    _updateOutputs{},
    _money(money),
    _levelProgression(levelProgression),
    _diamonds(std::move(diamonds)),
    _diamondsCounter(diamondsCounter),
    _crystals(std::move(crystals)),
    _crystalsCounter(crystalsCounter),
    _rollSpeedLevel(currentRollSpeed),
    _currentRollSpeed(currentRollSpeed),
    _jumpSpeedLevel(jumpSpeedLevel),
    _currentJumpSpeed(currentJumpSpeed),
    _turningSpeedLevel(turningSpeedLevel),
    _currentTurningSpeed(currentTurningSpeed),
    _timeLevel(timeLevel),
    _clockItemLevel(clockItemLevel),
    _bonusLevel(bonusLevel),
    _ballSkins(std::move(ballSkins)),
    _currentBallSkin(currentBallSkin),
    _frenchLangageIsActivated(frenchLangageIsActivated),
    _currentLevel(_levelProgression),
    _remainingTime(0.f),
    _wantsToQuit(false),
    _needsSaveFile(false) {
}

size_t Player::levelProgression() const {
    return _levelProgression;
}

void Player::unlockNewLevel() {
    ++_levelProgression;
}

void Player::decreaseMoney(unsigned int value) {
    _money -= value;
}

void Player::decreaseDiamonds(unsigned int value) {
    _diamondsCounter -= value;
}

void Player::addDiamond() {
    _diamondsCounter++;
}

void Player::increaseMoney(unsigned int value) {
    _money += value;
}

unsigned int Player::getMoney() const {
    return _money;
}

Player::Status Player::status() const {
    return _status;
}

void Player::bonusLevelUp() {
    _bonusLevel++;
}

void Player::clockItemLevelUp() {
    _clockItemLevel++;
}

void Player::timeLevelUp() {
    _timeLevel++;
}

void Player::gravityLevelUp() {
    _jumpSpeedLevel++;
}

void Player::speedLevelUp() {
    _rollSpeedLevel++;
}

void Player::setCurrentLevel(size_t levelNumber) {
    _currentLevel = levelNumber;
    _status = Player::Status::InTransition;

    // Update level number
    if (levelNumber > _levelProgression) {
        _levelProgression = levelNumber;
    }
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

void Player::setAsWinner() {
    _needsSaveFile = true;
    setAsInMenu();
    _gameStatus = GameStatus::Winner;
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
        std::to_string(_money)
        + boolVectorToString(_diamonds)
        + std::to_string(_diamondsCounter)
        + boolVectorToString(_crystals)
        + std::to_string(_crystalsCounter)
        + std::to_string(_rollSpeedLevel)
        + std::to_string(_currentRollSpeed)
        + std::to_string(_jumpSpeedLevel)
        + std::to_string(_currentJumpSpeed)
        + std::to_string(_jumpSpeedLevel)
        + std::to_string(_currentJumpSpeed)
        + std::to_string(_turningSpeedLevel)
        + std::to_string(_currentTurningSpeed)
        + std::to_string(_timeLevel)
        + std::to_string(_clockItemLevel)
        + std::to_string(_bonusLevel)
        + boolVectorToString(_ballSkins)
        + std::to_string(_currentJumpSpeed)
        + (_frenchLangageIsActivated ? "1" : "0");

    return SaveFileOutput(std::move(saveContent)).getOutput();
}

vecCstUpdateOutput_sptr &&Player::retrieveUpdateOutput() {
    return std::move(_updateOutputs);
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

    return std::make_shared<Player>(
        std::move(doubleChronometer),
        Player::readValue<unsigned int>(iss), // money
        Player::readValue<size_t>(iss), // levelProgression
        readBooleanVector(), // diamonds
        Player::readValue<unsigned int>(iss), // diamondsCounter
        readBooleanVector(), // crystals
        Player::readValue<unsigned int>(iss), // crystalsCounter
        Player::readValue<unsigned int>(iss), // rollSpeedLevel,
        Player::readValue<unsigned int>(iss), // currentRollSpeed,
        Player::readValue<unsigned int>(iss), // jumpSpeedLevel,
        Player::readValue<unsigned int>(iss), // currentJumpSpeed,
        Player::readValue<unsigned int>(iss), // turningSpeedLevel,
        Player::readValue<unsigned int>(iss), // currentTurningSpeed,
        Player::readValue<unsigned int>(iss), // timeLevel,
        Player::readValue<unsigned int>(iss), // clockItemLevel,
        Player::readValue<unsigned int>(iss), // bonusLevel,
        readBooleanVector(), //ballSkins
        Player::readValue<unsigned int>(iss), // currentBallSkin,
        [&iss]() {
            std::vector<bool> values;
            std::string w;
            iss >> w;
            return w.front() == '1';
        }() // frenchLangageIsActivated
    );
}
