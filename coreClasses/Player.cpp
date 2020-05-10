/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 10 mai 2020, 11:40
 */

#include "Player.h"

Player::Player() :
    _currentMap (nullptr),
    _currentPage(nullptr),
    _statut(Player::Statut::INMENU),
    _levelProgression(1),
    _money(0),
    _diamonds(false),
    _diamondsCounter(0),
    _speedLevel(1),
    _gravityLevel(1),
    _fireResistanceLevel(1),
    _timeLevel(1),
    _clockItemLevel(1),
    _bonusLevel(1)
{ }

void Player::currentMap(const std::shared_ptr<Map>& _currentMap)
{
    _currentMap = _currentMap;
}

const std::shared_ptr<Map>& Player::currentMap() const
{
    return _currentMap;
}

size_t Player::levelProgression() const {
    return levelProgression;
}

void Player::unlockNewLevel() {
    ++_levelProgression;
}

void Player::decreaseMoney(unsigned int value) {
    if ( _money < value ) {
        std::cerr << "Error ... negative value ! ... Cropped to 0";
        _money = 0;
    } else {
        _money -= value;
    }
}

void Player::increaseMoney(unsigned int value) {
    _money += value;
}

unsigned int Player::getMoney() const {
    return _money;
}

Player::Statut Player::statut() const {
    return _statut;
}

void Player::statut(const Player::Statut& s) {
    _statut = s;
    if (_statut == Player::Statut::INMENU) {
      _currentMap = nullptr;
    }
 }
