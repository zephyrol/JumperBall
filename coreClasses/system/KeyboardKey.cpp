//
// Created by S.Morgenthaler on 08/10/22.
//

#include "KeyboardKey.h"

KeyboardKey::KeyboardKey(std::vector<std::function<void()>> &&actionFunctions) :
    _actionFunctions(std::move(actionFunctions)),
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
    for(size_t i = 0; i < _currentStatus.size(); ++i) {
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
    // for (const auto &keyState: _currentStatus) {
    //     const auto &key = keyState.first;
    //     const auto &status = keyState.second;
    //     if (key != KeyboardKey::Button::Escape) {
    //         if (status == KeyboardKey::Status::Pressed) {
    //             _actionFunctions.at(static_cast<size_t>(key))();
    //         }
    //     } else if (_previousStatus.at(KeyboardKey::Button::Escape) == KeyboardKey::Status::Pressed
    //                && status == KeyboardKey::Status::Released) {
    //         _actionFunctions.at(static_cast<size_t>(key))();
    //     }
    // }
    _previousStatus = _currentStatus;
}
