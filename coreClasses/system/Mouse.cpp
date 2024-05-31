//
// Created by S.Morgenthaler on 08/10/22.
//
#include "Mouse.h"

#include <utility>

Mouse::Mouse(float screenRatio,
             const std::function<void()>& northActionFunc,
             const std::function<void()>& southActionFunc,
             const std::function<void()>& eastActionFunc,
             const std::function<void()>& westActionFunc,
             float northActionRepeatingDuration,
             float southActionRepeatingDuration,
             float eastActionRepeatingDuration,
             float westActionRepeatingDuration,
             std::function<void(float mouseX, float mouseY)> validateActionFunc,
             std::function<void()> releaseFunction)
    : _directionActionFunctions{northActionFunc, southActionFunc, eastActionFunc, westActionFunc},
      _directionActionRepeatingDurations{northActionRepeatingDuration, southActionRepeatingDuration,
                                         eastActionRepeatingDuration, westActionRepeatingDuration},
      _validateActionFunction(std::move(validateActionFunc)),
      _releaseFunction(std::move(releaseFunction)),
      _screenRatio(screenRatio),
      _mouseCoords(nullptr),
      _movementCircle(nullptr),
      _directionActionTimePoint(nullptr),
      _isPressed(false) {}

void Mouse::press(float posX, float posY) {
    const Mouse::MouseCoords mouseCoords{posX, posY};
    _mouseCoords = std::make_shared<MouseCoords>(mouseCoords);
    _isPressed = true;
}

void Mouse::release() {
    _isPressed = false;
}

void Mouse::setScreenRatio(float screenRatio) {
    _screenRatio = screenRatio;
}

void Mouse::update(const Chronometer::TimePointMs& updatingTime) {
    !_isPressed ? releasedMouseUpdate(updatingTime) : pressedMouseUpdate(updatingTime);
}

void Mouse::pressedMouseUpdate(const Chronometer::TimePointMs& updatingTime) {
    if (!_movementCircle) {
        _movementCircle =
            std::unique_ptr<MovementCircle>(new MovementCircle{nullptr, *_mouseCoords, updatingTime});
    }

    const auto& directionStateCoords = _movementCircle->mouseCoords;
    const auto movementLength = computeDistance(directionStateCoords.xCoord, directionStateCoords.yCoord,
                                                _mouseCoords->xCoord, _mouseCoords->yCoord);

    const auto computeCardinalDistance = [this, directionStateCoords, movementLength](
                                             const Mouse::CardinalPoint& cardinalPoint) -> CardinalDistance {
        const auto& point = cardinalPoint.point;
        return {cardinalPoint.direction,
                computeDistance(directionStateCoords.xCoord + point.first * movementLength,
                                directionStateCoords.yCoord + point.second * movementLength,
                                _mouseCoords->xCoord, _mouseCoords->yCoord) /
                    cardinalPoint.weight};
    };

    std::vector<Mouse::CardinalDistance> cardinalDistances(cardinalsPoints.size());
    std::transform(cardinalsPoints.begin(), cardinalsPoints.end(), cardinalDistances.begin(),
                   computeCardinalDistance);

    Mouse::CardinalDistance nearestCardinal = cardinalDistances.front();
    for (auto iter = cardinalDistances.begin() + 1; iter != cardinalDistances.end(); ++iter) {
        const auto& cardinal = *iter;
        if (cardinal.distance < nearestCardinal.distance) {
            nearestCardinal = cardinal;
        }
    }
    constexpr auto movingThreshold = 0.05f;
    if (computeDistance(_mouseCoords->xCoord, _mouseCoords->yCoord, _movementCircle->mouseCoords.xCoord,
                        _movementCircle->mouseCoords.yCoord) > movingThreshold) {
        _movementCircle = std::unique_ptr<MovementCircle>(new MovementCircle{
            std::unique_ptr<ScreenDirection>(new ScreenDirection(nearestCardinal.direction)), *_mouseCoords,
            updatingTime});
        executeDirectionActionFunction(updatingTime);
        return;
    }

    const auto& movement = _movementCircle->movement;
    constexpr float updatingDirectionDetectionThreshold = 0.3f;  // 0.3 seconds
    const float timeSinceMovementCircleCreation =
        Chronometer::getFloatFromDurationMS(updatingTime - _movementCircle->creationTime);

    if (movement == nullptr) {
        return;
    }

    if (timeSinceMovementCircleCreation > updatingDirectionDetectionThreshold) {
        _movementCircle = std::unique_ptr<MovementCircle>(new MovementCircle{
            std::unique_ptr<ScreenDirection>(new ScreenDirection(*movement)), *_mouseCoords, updatingTime});
    }

    if (_directionActionTimePoint == nullptr) {
        return;
    }

    const float timeSinceMovementStarting =
        Chronometer::getFloatFromDurationMS(updatingTime - (*_directionActionTimePoint));

    if (timeSinceMovementStarting >
        _directionActionRepeatingDurations.at(static_cast<size_t>(*(_movementCircle->movement)))) {
        executeDirectionActionFunction(updatingTime);
        return;
    }
    _releaseFunction();
}

void Mouse::executeDirectionActionFunction(const Chronometer::TimePointMs& updatingTime) {
    _directionActionTimePoint =
        std::unique_ptr<Chronometer::TimePointMs>(new Chronometer::TimePointMs(updatingTime));
    _directionActionFunctions.at(static_cast<size_t>(*(_movementCircle->movement)))();
}

void Mouse::releasedMouseUpdate(const Chronometer::TimePointMs& updatingTime) {
    if (_movementCircle == nullptr) {
        _releaseFunction();
        return;
    }
    if (_movementCircle->movement != nullptr) {
        _movementCircle = nullptr;
        _releaseFunction();
        return;
    }

    const float timeSinceMovementCircleCreation =
        Chronometer::getFloatFromDurationMS(updatingTime - _movementCircle->creationTime);

    const float distance =
        computeDistance(_movementCircle->mouseCoords.xCoord, _movementCircle->mouseCoords.yCoord,
                        _mouseCoords->xCoord, _mouseCoords->yCoord);
    constexpr float pressTimeThreshold = 0.3f;  // in seconds
    constexpr float thresholdMoving = 0.03f;
    if (distance < thresholdMoving && timeSinceMovementCircleCreation < pressTimeThreshold) {
        _validateActionFunction(_mouseCoords->xCoord, _mouseCoords->yCoord);
    } else {
        _releaseFunction();
    }
    _movementCircle = nullptr;
}

const std::vector<Mouse::CardinalPoint> Mouse::cardinalsPoints{
    {Mouse::ScreenDirection::North, {0.f, 1.f}, 1.f},
    {Mouse::ScreenDirection::South, {0.f, -1.f}, 0.7f},
    {Mouse::ScreenDirection::East, {1.f, 0.f}, 1.f},
    {Mouse::ScreenDirection::West, {-1.f, 0.f}, 1.f}};

float Mouse::computeDistance(float x0, float y0, float x1, float y1) const {
    // Screen is rarely a square, we use the ratio to avoid distance scaling and to get orthonormal base.
    // So length could be > sqrt 2
    const auto getLengths = [this, x0, y0, x1, y1]() {
        const auto x1MinusX0 = x1 - x0;
        const auto y1MinusY0 = y1 - y0;

        return _screenRatio > 1.f ? std::pair<float, float>{x1MinusX0 * _screenRatio, y1MinusY0}
                                  : std::pair<float, float>{x1MinusX0, y1MinusY0 / _screenRatio};
    };
    const auto lengths = getLengths();
    const auto& horizontalLength = lengths.first;
    const auto& verticalLength = lengths.second;
    return sqrtf(horizontalLength * horizontalLength + verticalLength * verticalLength);
}

float Mouse::currentXCoord() const {
    return _mouseCoords->xCoord;
}

float Mouse::currentYCoord() const {
    return _mouseCoords->yCoord;
}

bool Mouse::isPressed() const {
    return _isPressed;
}

std::shared_ptr<const Mouse::MouseCoords> Mouse::getMouseCoords() const {
    return _mouseCoords;
}
