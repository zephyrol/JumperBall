/*
 * File: Player.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "system/UpdateOutput.h"
#include "system/DoubleChronometer.h"


class Player;

using Player_sptr = std::shared_ptr<Player>;

class Player {
public:
    explicit Player(
        DoubleChronometer_sptr doubleChronometer,
        unsigned int money,
        size_t levelProgression,
        std::vector<bool> ballSkins,
        unsigned int currentBallSkin,
        bool frenchLanguageIsActivated,
        bool musicsAreActivated,
        bool soundsAreActivated
    );

    static Player_sptr createInstance(
        DoubleChronometer_sptr doubleChronometer,
        const std::string &saveFile
    );

    enum class Status {
        InGame, InMenu, InTransition
    };

    enum class GameStatus {
        None, Winner, Loser
    };

    CstDoubleChronometer_sptr getDoubleChronometer() const;

    CstChronometer_sptr getCreationChronometer() const;

    size_t levelProgression() const;

    void setCurrentLevel(size_t levelNumber);

    size_t getCurrentLevel() const;

    unsigned int getMoney() const;

    unsigned int getPreviousMoney() const;

    void unlockNewLevel();

    void increaseMoney(unsigned int value);

    void decreaseMoney(unsigned int value);

    void switchLangage();

    bool isUsingEnglishLanguage() const;

    void switchMusicsStatus();

    bool areMusicsActivated() const;

    void switchSoundsStatus();

    bool areSoundsActivated() const;

    void addValidationSound();

    bool wantsToQuit() const;

    void requestQuit();

    void escapeAction();

    void setRemainingTime(float remainingTime);

    float getRemainingTime() const;

    Status status() const;

    void setAsInGame();

    void setAsWinner(unsigned int earnedMoney);

    bool isAWinner() const;

    unsigned int getCurrentBallSkin() const;

    void setAsLoser();

    bool isALoser() const;

    void resetGameStatus();

    void setAsInMenu();

    std::string genOutputs();

    void requestDeveloperPage();

    void requestMusicianPage();

    static constexpr unsigned int maxLevel = 2;

    template<typename T>
    static T readValue(std::istringstream& stream);

private:

    const DoubleChronometer_sptr _doubleChronometer;
    Status _status;
    GameStatus _gameStatus;
    vecCstUpdateOutput_sptr _updateOutputs;

    unsigned int _money;
    unsigned int _previousMoney;
    size_t _levelProgression;

    std::vector<bool> _ballSkins;
    unsigned int _currentBallSkin;

    bool _frenchLanguageIsActivated;
    bool _musicsAreActivated;
    bool _soundsAreActivated;

    size_t _currentLevel;
    float _remainingTime;
    bool _wantsToQuit;
    bool _needsSaveFile;

    std::string genSaveContent();
};

template<typename T>
T Player::readValue(std::istringstream& stream) {
    T value;
    stream >> value;
    return value;
}


#endif /* PLAYER_H */
