/*
 * File: Player.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "system/DoubleChronometer.h"
#include "system/UpdateOutput.h"

class Player;

using Player_sptr = std::shared_ptr<Player>;

class Player {
   public:
    explicit Player(DoubleChronometer_sptr doubleChronometer,
                    unsigned int money,
                    size_t levelProgression,
                    std::vector<bool> ballSkins,
                    unsigned int currentBallSkin,
                    bool frenchLanguageIsActivated,
                    bool musicsAreActivated,
                    bool soundsAreActivated,
                    bool leftRightIsReversed,
                    float initialAdvertisementTime);

    static Player_sptr createInstance(DoubleChronometer_sptr doubleChronometer, const std::string& saveFile);

    enum class Status { InGame, InMenu, InTransition };

    enum class GameStatus { None, Winner, Loser };

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

    void switchLeftRightStatus();

    bool isLeftRightReversed() const;

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
    static constexpr unsigned int maxLevel = 120;

    template <typename T>
    static T readValue(std::istringstream& stream);

    template <typename T>
    static std::string compressTransformAttribute(const T& value, T factor);

    template <typename T>
    static T uncompressTransformAttribute(std::string value, T factor);

   private:

    const DoubleChronometer_sptr _doubleChronometer;
    Status _status;
    GameStatus _gameStatus;
    vecCstUpdateOutput_sptr _updateOutputs;

    static constexpr unsigned int moneyTransformFactor = 2;
    unsigned int _money;
    unsigned int _previousMoney;

    static constexpr size_t levelTransformFactor = 3;
    size_t _levelProgression;

    std::vector<bool> _ballSkins;
    unsigned int _currentBallSkin;

    bool _frenchLanguageIsActivated;
    bool _musicsAreActivated;
    bool _soundsAreActivated;
    bool _leftRightIsReversed;

    float _initialAdvertisementTime;
    Chronometer _advertisementChronometer;

    Chronometer::TimePointMs _timePointSavedFile;

    size_t _currentLevel;
    float _remainingTime;
    bool _needsSaveFile;

    std::string genSaveContent();
};

template <typename T>
T Player::readValue(std::istringstream& stream) {
    T value;
    stream >> value;
    return value;
}

template <typename T>
std::string Player::compressTransformAttribute(const T& value, T factor) {
    auto levelProgressionStr = std::to_string(value * factor);
    std::reverse(levelProgressionStr.begin(), levelProgressionStr.end());
    return levelProgressionStr;
}
template <typename T>
T Player::uncompressTransformAttribute(std::string value, T factor) {
    std::reverse(value.begin(), value.end());
    return static_cast<T>(std::stoi(value) / factor);
}

#endif /* PLAYER_H */
