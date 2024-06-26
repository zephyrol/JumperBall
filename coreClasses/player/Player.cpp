/*
 * File: Player.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 11:40
 */
#include "Player.h"
#include <sstream>
#include "system/GoToAuthorPageOutput.h"
#include "system/MusicStatusOutput.h"
#include "system/RequestQuitOutput.h"
#include "system/RunAdOutput.h"
#include "system/SaveFileOutput.h"
#include "system/ShowPrivacyPolicyOutput.h"
#include "system/SoundOutput.h"
#include "system/SoundStatusOutput.h"

Player::Player(DoubleChronometer_sptr doubleChronometer,
               unsigned int money,
               size_t levelProgression,
               std::vector<bool> ballSkins,
               unsigned int currentBallSkin,
               bool frenchLanguageIsActivated,
               bool musicsAreActivated,
               bool soundsAreActivated,
               bool leftRightIsReversed,
               float initialAdvertisementTime)
    : _doubleChronometer(std::move(doubleChronometer)),
      _status(Player::Status::InMenu),
      _gameStatus(Player::GameStatus::None),
      _updateOutputs{},
      _money(money),
      _previousMoney(_money),
      _levelProgression(levelProgression),
      _ballSkins(std::move(ballSkins)),
      _currentBallSkin(currentBallSkin),
      _frenchLanguageIsActivated(frenchLanguageIsActivated),
      _musicsAreActivated(musicsAreActivated),
      _soundsAreActivated(soundsAreActivated),
      _leftRightIsReversed(leftRightIsReversed),
      _initialAdvertisementTime(initialAdvertisementTime),
      _advertisementChronometer(true),
      _timePointSavedFile(Chronometer::getTimePointMSNow()),
      _currentLevel(_levelProgression),
      _remainingTime(0.f),
      _needsSaveFile(false) {}

size_t Player::levelProgression() const {
    return _levelProgression;
}

void Player::requestDeveloperPage() {
    _updateOutputs.emplace_back(new GoToAuthorPageOutput("developer"));
}

void Player::requestMusicianPage() {
    _updateOutputs.emplace_back(new GoToAuthorPageOutput("musician"));
}

unsigned int Player::getCurrentBallSkin() const {
    return _currentBallSkin;
}

unsigned int Player::getMoney() const {
    return _money;
}

unsigned int Player::getPreviousMoney() const {
    return _previousMoney;
}

Player::Status Player::status() const {
    return _status;
}

void Player::setCurrentLevel(size_t levelNumber) {
    _currentLevel = levelNumber;
    _status = Player::Status::InTransition;
    _updateOutputs.emplace_back(new SoundOutput("startLevel"));
}

size_t Player::getCurrentLevel() const {
    return _currentLevel;
}

void Player::escapeAction() {
    if (_status != Player::Status::InMenu) {
        setAsInMenu();
    }
}

void Player::setCurrentSkin(size_t skinNumber) {
    _currentBallSkin = static_cast<decltype(_currentBallSkin)>(skinNumber);
    addValidationSound();
}

void Player::unlockSkin(size_t skinNumber) {
    _needsSaveFile = true;
    _money -= static_cast<decltype(_money)>(skinNumber) * 100;
    _ballSkins.at(skinNumber) = true;
    _updateOutputs.emplace_back(new SoundOutput("bought"));
    _currentBallSkin = static_cast<decltype(_currentBallSkin)>(skinNumber);
}

void Player::setAsWinner(unsigned int earnedMoney) {
    _needsSaveFile = true;
    setAsInMenu();
    if (_currentLevel == _levelProgression && _currentLevel != maxLevel) {
        ++_levelProgression;
    }
    _gameStatus = GameStatus::Winner;
    _previousMoney = _money;
    _money += earnedMoney;
    constexpr unsigned int maxMoney = 9999;
    if (_money > maxMoney) {
        _money = maxMoney;
    }
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

    const auto boolVectorToString = [](const std::vector<bool>& boolVector) {
        return std::accumulate(boolVector.cbegin(), boolVector.cend(), std::string(),
                               [](const std::string& acc, bool booleanValue) {
                                   return acc + (booleanValue ? std::string("1") : std::string("0"));
                               });
    };

    const auto initialAdvertisementTimeFloat =
        _initialAdvertisementTime + _advertisementChronometer.getTime();
    constexpr auto currentSaveVersion = 0;
    std::string saveContent = std::to_string(currentSaveVersion) + " " + std::to_string(_money) + " " +
                              std::to_string(_levelProgression) + " " + boolVectorToString(_ballSkins) + " " +
                              std::to_string(_currentBallSkin) + " " +
                              (_frenchLanguageIsActivated ? "1" : "0") + " " +
                              (_musicsAreActivated ? "1" : "0") + " " + (_soundsAreActivated ? "1" : "0") +
                              " " + (_leftRightIsReversed ? "1" : "0") + " " +
                              std::to_string(static_cast<unsigned int>(initialAdvertisementTimeFloat));

    return SaveFileOutput(std::move(saveContent)).getOutput();
}

Player_sptr Player::createInstance(DoubleChronometer_sptr doubleChronometer, const std::string& saveFile) {
    std::istringstream iss(saveFile);

    const auto readBooleanVector = [&iss]() {
        std::vector<bool> values;
        std::string w;
        iss >> w;
        for (const auto c : w) {
            values.push_back(c == '1');
        }
        return values;
    };

    const auto readBoolean = [&iss]() {
        std::vector<bool> values;
        std::string w;
        iss >> w;
        return w.front() == '1';
    };

    // Read current version
    Player::readValue<unsigned int>(iss);

    const auto money = Player::readValue<unsigned int>(iss);
    const auto levelProgression = Player::readValue<size_t>(iss);
    const auto ballSkins = readBooleanVector();
    const auto currentBallSkin = Player::readValue<unsigned int>(iss);
    const auto frenchLanguageIsActivated = readBoolean();
    const auto musicsAreActivated = readBoolean();
    const auto soundsAreActivated = readBoolean();
    const auto leftRightIsReversed = readBoolean();

    const auto initialAdvertisementTimeUint = Player::readValue<unsigned int>(iss);
    const auto initialAdvertisementTime = static_cast<float>(initialAdvertisementTimeUint);

    return std::make_shared<Player>(std::move(doubleChronometer), money, levelProgression, ballSkins,
                                    currentBallSkin, frenchLanguageIsActivated, musicsAreActivated,
                                    soundsAreActivated, leftRightIsReversed, initialAdvertisementTime);
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

void Player::switchLanguage() {
    _needsSaveFile = true;
    _frenchLanguageIsActivated = !_frenchLanguageIsActivated;
    addValidationSound();
}

bool Player::isUsingEnglishLanguage() const {
    return !_frenchLanguageIsActivated;
}

void Player::switchMusicsStatus() {
    _needsSaveFile = true;
    _musicsAreActivated = !_musicsAreActivated;
    _updateOutputs.emplace_back(new MusicStatusOutput(_musicsAreActivated ? "on" : "off"));
    addValidationSound();
}

bool Player::areMusicsActivated() const {
    return _musicsAreActivated;
}

void Player::switchSoundsStatus() {
    addValidationSound();
    _needsSaveFile = true;
    _soundsAreActivated = !_soundsAreActivated;
    _updateOutputs.emplace_back(new SoundStatusOutput(_soundsAreActivated ? "on" : "off"));
    if (_soundsAreActivated) {
        addValidationSound();
    }
}

bool Player::areSoundsActivated() const {
    return _soundsAreActivated;
}

void Player::switchLeftRightStatus() {
    addValidationSound();
    _needsSaveFile = true;
    _leftRightIsReversed = !_leftRightIsReversed;
}

bool Player::isLeftRightReversed() const {
    return _leftRightIsReversed;
}

std::string Player::genOutputs() {
    return genSaveContent() + UpdateOutput::combineUpdateOutputs(std::move(_updateOutputs));
}

void Player::addValidationSound() {
    _updateOutputs.emplace_back(new SoundOutput("validation"));
}

bool Player::hasBoughtSkin(size_t skinNumber) const {
    return _ballSkins.at(skinNumber);
}

void Player::addNotEnoughMoneySound() {
    _updateOutputs.emplace_back(new SoundOutput("notEnoughMoney"));
}

void Player::updateAdvertisementChronometer(const Chronometer::TimePointMs& updatingTime) {
    // 1 Update save file frequently
    constexpr auto saveFileDelay = 20.f;
    if (Chronometer::getFloatFromDurationMS(updatingTime - _timePointSavedFile) > saveFileDelay) {
        _needsSaveFile = true;
        _timePointSavedFile = updatingTime;
    }

    _advertisementChronometer.update(updatingTime);
}

void Player::stopChronometer() {
    _advertisementChronometer.stop();
}

void Player::resumeChronometer() {
    _advertisementChronometer.resume();
}

void Player::checkAdvertisement() {
    constexpr decltype(_levelProgression) firstAdLevels = 6;
    if(_levelProgression < firstAdLevels) {
        return;
    }
    static constexpr float timeToRunAd = 180.f;  // 3 minutes
    const float spentTime = _initialAdvertisementTime + _advertisementChronometer.getTime();
    if (spentTime > timeToRunAd) {
        _advertisementChronometer.reset();
        _initialAdvertisementTime = 0.f;
        _updateOutputs.emplace_back(new RunAdOutput());
    }
}

void Player::addQuitRequest() {
    _updateOutputs.emplace_back(new RequestQuitOutput());
}

void Player::addPrivacyPolicyRequest() {
    _updateOutputs.emplace_back(new ShowPrivacyPolicyOutput());
}
