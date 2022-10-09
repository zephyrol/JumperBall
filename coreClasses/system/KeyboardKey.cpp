//
// Created by S.Morgenthaler on 08/10/22.
//

#include "KeyboardKey.h"

KeyboardKey::KeyboardKey(std::unordered_map<KeyboardKey::Button, std::function<void()>> &&actionFunctions) :
    _actionFunctions(std::move(actionFunctions)),
    _currentStatus{
        {KeyboardKey::Button::Up,       KeyboardKey::Status::Released},
        {KeyboardKey::Button::Down,     KeyboardKey::Status::Released},
        {KeyboardKey::Button::Right,    KeyboardKey::Status::Released},
        {KeyboardKey::Button::Left,     KeyboardKey::Status::Released},
        {KeyboardKey::Button::Escape,   KeyboardKey::Status::Released},
        {KeyboardKey::Button::Validate, KeyboardKey::Status::Released}
    },
    _previousStatus(_currentStatus) {
}

void KeyboardKey::press(const KeyboardKey::Button &button) {
    _currentStatus.at(button) = Status::Pressed;
}

void KeyboardKey::release(const KeyboardKey::Button &button) {
    _currentStatus.at(button) = Status::Released;
}

void KeyboardKey::update() {
    for (const auto &keyState: _currentStatus) {
        const auto &key = keyState.first;
        const auto &status = keyState.second;
        if (key != KeyboardKey::Button::Escape) {
            if (status == KeyboardKey::Status::Pressed) {
                _actionFunctions.at(key)();
            }
        } else if (_previousStatus.at(KeyboardKey::Button::Escape) == KeyboardKey::Status::Pressed
                   && status == KeyboardKey::Status::Released) {
            _actionFunctions.at(key)();
        }
    }
    _previousStatus = _currentStatus;
}
