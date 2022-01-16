/*
 * File: Player.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */

#ifndef PLAYER_H
#define PLAYER_H
#include "scene/Ball.h"

class Player {
public:
Player();

enum class Status { InGame, InMenu, InTransition };

size_t levelProgression() const;
unsigned int getMoney() const;

void unlockNewLevel();

void increaseMoney(unsigned int value);
void decreaseMoney(unsigned int value);

void decreaseDiamonds(unsigned int value);
void addDiamond();

void speedLevelUp();
void gravityLevelUp();
void fireResistanceLevelUp();

void timeLevelUp();
void clockItemLevelUp();
void bonusLevelUp();

Status status() const;
void status(const Status& s);

private:

Status _status;
size_t _levelProgression;

unsigned int _money;
std::vector <bool> _diamonds;

unsigned int _diamondsCounter;

unsigned int _speedLevel;
unsigned int _gravityLevel;
unsigned int _fireResistanceLevel;

unsigned int _timeLevel;
unsigned int _clockItemLevel;
unsigned int _bonusLevel;

};

#endif /* PLAYER_H */
