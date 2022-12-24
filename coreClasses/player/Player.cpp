/*
 * File: Player.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */
#include "Player.h"
#include "system/SaveFileOutput.h"

Player::Player(DoubleChronometer_sptr doubleChronometer) :
    _doubleChronometer(std::move(doubleChronometer)),
    _status(Player::Status::InMenu),
    _gameStatus(Player::GameStatus::None),
    _levelProgression(2),
    _currentLevel(_levelProgression),
    _updateOutputs{},
    _money(0),
    _diamonds(false),
    _diamondsCounter(0),
    _speedLevel(1),
    _gravityLevel(1),
    _fireResistanceLevel(1),
    _timeLevel(1),
    _clockItemLevel(1),
    _bonusLevel(1),
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

void Player::fireResistanceLevelUp() {
    _fireResistanceLevel++;
}

void Player::gravityLevelUp() {
    _gravityLevel++;
}

void Player::speedLevelUp() {
    _speedLevel++;
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
    return SaveFileOutput(
        std::to_string(_money) + "." + std::to_string(_levelProgression)
    ).getOutput();
}

vecCstUpdateOutput_sptr &&Player::retrieveUpdateOutput() {
    return std::move(_updateOutputs);
}

CstChronometer_sptr Player::getCreationChronometer() const {
    return _doubleChronometer->first();
}

void Player::setAsInGame() {
    _status = Player::Status::InGame;
    _doubleChronometer->resumeSecond();
}

void Player::setAsInMenu() {
    _status = Player::Status::InMenu;
    _doubleChronometer->stopSecond();
}
