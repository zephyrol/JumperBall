/*
 * File: Player.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */
#include "Player.h"

Player::Player() :
    _status(Player::Status::InMenu),
    _gameStatus(Player::GameStatus::None),
    _levelProgression(1),
    _currentLevel(_levelProgression),
    _money(0),
    _diamonds(false),
    _diamondsCounter(0),
    _speedLevel(1),
    _gravityLevel(1),
    _fireResistanceLevel(1),
    _timeLevel(1),
    _clockItemLevel(1),
    _bonusLevel(1),
    _wantsToQuit(false) {
}


size_t Player::levelProgression() const {
    return _levelProgression;
}

void Player::unlockNewLevel() {
    ++_levelProgression;
}

void Player::decreaseMoney(unsigned int value) {
    if (_money < value) {
        std::cerr << "Error ... negative value ! ... Cropped to 0";
        _money = 0;
    } else {
        _money -= value;
    }
}

void Player::decreaseDiamonds(unsigned int value) {
    if (_diamondsCounter < value) {
        std::cerr << "Error ... negative value ! ... Cropped to 0";
        _diamondsCounter = 0;
    } else {
        _diamondsCounter -= value;
    }
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

void Player::status(const Player::Status &s) {
    _status = s;
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
    if(levelNumber > _levelProgression) {
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
    if(_status != Player::Status::InMenu)  {
        _status = Player::Status::InMenu;
    }
}

void Player::setAsWinner() {
    _status = Player::Status::InMenu;
    _gameStatus = GameStatus::Winner;
}
bool Player::isAWinner() const {
    return _gameStatus == GameStatus::Winner;
}
void Player::setAsLoser() {
    _status = Player::Status::InMenu;
    _gameStatus = GameStatus::Loser;
}

bool Player::isALoser() const {
    return _gameStatus == GameStatus::Loser;
}

void Player::resetGameStatus() {
    _gameStatus = GameStatus::None;
}
