//
// Created by S.Morgenthaler on 24/09/2022.
//

#include "ScrollablePage.h"

ScrollablePage::ScrollablePage(Player_sptr &&player, float height) :
    Page(std::move(player)),
    _chronometer(_player->getCreationChronometer()),
    _height(height),
    _localPosY(0.f),
    _localPressedPosY(0.f),
    _localReleasedPosY(0.f),
    _isPressed(false),
    _pressedScreenPosY(0.f),
    _previousMouseCoords(nullptr),
    _lastSwipeUpdates{},
    _releaseVelocity(0.f) {
}

void ScrollablePage::update(const Mouse &mouse) {

    const auto isPressed = mouse.isPressed();
    auto screenPosY = 0.f;
    if (isPressed) {
        screenPosY = mouse.currentYCoord();
    } else if (_previousMouseCoords != nullptr) {
        screenPosY = _previousMouseCoords->yCoord;
    }

    // Press cases
    if (isPressed && !_isPressed) {
        _pressedScreenPosY = screenPosY;
        _localPressedPosY = _localPosY;
        _isPressed = true;
        _lastSwipeUpdates.clear();
    }

    if (_isPressed) {
        constexpr float thresholdDeltaT = 0.1f; // 100 ms
        std::list<slideState>::iterator it;
        for (
            it = _lastSwipeUpdates.begin();
            it != _lastSwipeUpdates.end() &&
            (_chronometer->getTime() - it->first) < thresholdDeltaT;
            ++it
            ) {
        }

        _lastSwipeUpdates.erase(it, _lastSwipeUpdates.end());
        _lastSwipeUpdates.emplace_front(_chronometer->getTime(), screenPosY);
        _localPosY = _localPressedPosY + (screenPosY - _pressedScreenPosY);
    }

    // Release cases
    if (!isPressed && _isPressed && !_lastSwipeUpdates.empty()) {
        const slideState &lastSlideState = _lastSwipeUpdates.front();
        const slideState &olderSlideState = _lastSwipeUpdates.back();
        float deltaT = lastSlideState.first - olderSlideState.first;

        // the velocity is the position derivative (% page / ms)
        _releaseVelocity = (lastSlideState.second - olderSlideState.second) / deltaT;

        _localReleasedPosY = _localPosY;
        _isPressed = false;
    }
    if (!_isPressed && !_lastSwipeUpdates.empty()) {
        const slideState &lastSlideState = _lastSwipeUpdates.front();
        const float t = _chronometer->getTime() - lastSlideState.first;
        const float deceleration = decelerationCoefficient * t * t / 2.f;

        if (_releaseVelocity > 0.f && t < -(_releaseVelocity) / (2.f * -decelerationCoefficient / 2.f)) {
            _localPosY = -deceleration + _releaseVelocity * t + _localReleasedPosY;
        } else if (_releaseVelocity < 0.f && t < -(_releaseVelocity) / (decelerationCoefficient)) {
            _localPosY = deceleration + _releaseVelocity * t + _localReleasedPosY;
        }

        if (_localPosY < 0.f) {
            _localPosY = 0.f;
        } else if (_localPosY > _height - 1.f) {
            _localPosY = _height - 1.f;
        }
    }
    _previousMouseCoords = mouse.getMouseCoords();
}

float ScrollablePage::getOffsetY() const {
    return _localPosY;
}

Displayable::DynamicNames ScrollablePage::getDynamicFloatNames() const {
    return {"positionY"};
}

Displayable::DynamicValues<float> ScrollablePage::getDynamicFloatValues() const {
    return {
        _localPosY * 2.f // Convert to OpenGL format
    };
}
