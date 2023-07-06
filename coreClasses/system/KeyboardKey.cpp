//
// Created by S.Morgenthaler on 08/10/22.
//

#include "KeyboardKey.h"

KeyboardKey::KeyboardKey(
    std::vector<std::function<void()>> &&actionFunctions,
    std::function<void()> &&releaseFunction
) :
    _actionFunctions(std::move(actionFunctions)),
    _releaseFunction(std::move(releaseFunction)),
    _currentStatus(6, KeyboardKey::Status::Released),
    _previousStatus(_currentStatus) {
}

void KeyboardKey::press(const KeyboardKey::Button &button) {
    _currentStatus.at(static_cast<size_t>(button)) = Status::Pressed;
}

void KeyboardKey::release(const KeyboardKey::Button &button) {
    _currentStatus.at(static_cast<size_t>(button)) = Status::Released;
}

void KeyboardKey::update() {
    for (size_t i = 0; i < _currentStatus.size(); ++i) {
        constexpr auto escapeButtonIndex = static_cast<size_t>(KeyboardKey::Button::Escape);
        const auto &status = _currentStatus.at(i);
        if (i != escapeButtonIndex) {
            if (status == KeyboardKey::Status::Pressed) {
                _actionFunctions.at(i)();
            }
        } else if (_previousStatus.at(escapeButtonIndex) == KeyboardKey::Status::Pressed
                   && status == KeyboardKey::Status::Released) {
            _actionFunctions.at(i)();
        }
    }
    if (std::all_of(_currentStatus.begin(), _currentStatus.end(), [](const KeyboardKey::Status &status) {
        return status == KeyboardKey::Status::Released;
    })) {
        _releaseFunction();
    }
    _previousStatus = _currentStatus;
}
