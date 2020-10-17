/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */

#ifndef PLAYER_H
#define PLAYER_H
#include "scene/Ball.h"
#include "gameMenu/Page.h"

class Player
{
public:
    Player();

    enum class Statut { INGAME, INMENU, INTRANSITION };

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

    Statut statut() const;
    void statut(const Statut& s);

    void treatAction(const Label::LabelAnswer& action);

private:
   
    Statut _statut;
    size_t _levelProgression;

    unsigned int _money;
    std::vector<bool> _diamonds;

    unsigned int _diamondsCounter;

    unsigned int _speedLevel;
    unsigned int _gravityLevel;
    unsigned int _fireResistanceLevel;

    unsigned int _timeLevel;
    unsigned int _clockItemLevel;
    unsigned int _bonusLevel;

};

#endif /* PLAYER_H */

