/*
 * File: Player.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "scene/Ball.h"
#include "system/UpdateOutput.h"
#include "system/DoubleChronometer.h"


class Player;

using Player_sptr = std::shared_ptr<Player>;

class Player {
public:
    explicit Player(CstDoubleChronometer_sptr doubleChronometer);

    enum class Status {
        InGame, InMenu, InTransition
    };

    enum class GameStatus {
        None, Winner, Loser
    };

    const CstDoubleChronometer_sptr &getDoubleChronometer() const;

    const CstChronometer_sptr &getCreationChronometer() const;

    vecCstUpdateOutput_sptr &&retrieveUpdateOutput();

    size_t levelProgression() const;

    void setCurrentLevel(size_t levelNumber);

    size_t getCurrentLevel() const;

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

    bool wantsToQuit() const;

    void requestQuit();

    void escapeAction();

    Status status() const;

    void status(const Status &s);

    void setAsWinner();

    bool isAWinner() const;

    void setAsLoser();

    bool isALoser() const;

    void resetGameStatus();

    std::string genSaveContent();

    static constexpr unsigned int maxLevel = 2;

private:


    const CstDoubleChronometer_sptr _doubleChronometer;
    Status _status;
    GameStatus _gameStatus;
    size_t _levelProgression;
    size_t _currentLevel;
    vecCstUpdateOutput_sptr _updateOutputs;

    unsigned int _money;
    std::vector<bool> _diamonds;

    unsigned int _diamondsCounter;

    unsigned int _speedLevel;
    unsigned int _gravityLevel;
    unsigned int _fireResistanceLevel;

    unsigned int _timeLevel;
    unsigned int _clockItemLevel;
    unsigned int _bonusLevel;
    bool _wantsToQuit;
    bool _needsSaveFile;
};

#endif /* PLAYER_H */
