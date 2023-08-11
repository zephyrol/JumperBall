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
        bool soundsAreActivated,
        float initialAdvertisementTime
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

    void setCurrentSkin(size_t skinNumber);

    void unlockSkin(size_t skinNumber);

    void switchLanguage();

    bool isUsingEnglishLanguage() const;

    void switchMusicsStatus();

    bool areMusicsActivated() const;

    void switchSoundsStatus();

    bool areSoundsActivated() const;

    void addValidationSound();

    void addQuitRequest();

    void addNotEnoughMoneySound();

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

    void updateAdvertisementChronometer(const Chronometer::TimePointMs& updatingTime);

    void stopChronometer();

    void resumeChronometer();

    void checkAdvertisement();

    /**
     * True if a skin has been bought.
     * @param skinNumber between [0-4]
     */
    bool hasBoughtSkin(size_t skinNumber) const;

    static constexpr unsigned int maxLevel = 29;

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

    float _initialAdvertisementTime;
    Chronometer _advertisementChronometer;

    Chronometer::TimePointMs _timePointSavedFile;

    size_t _currentLevel;
    float _remainingTime;
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
