//
// Created by S.Morgenthaler on 08/10/22.
//

#include "KeyboardKey.h"

KeyboardKey::KeyboardKey():
_currentStatus{
    {KeyboardKey::Button::Up,       KeyboardKey::Status::Released},
    {KeyboardKey::Button::Down,     KeyboardKey::Status::Released},
    {KeyboardKey::Button::Right,    KeyboardKey::Status::Released},
    {KeyboardKey::Button::Left,     KeyboardKey::Status::Released},
    {KeyboardKey::Button::Escape,   KeyboardKey::Status::Released},
    {KeyboardKey::Button::Validate, KeyboardKey::Status::Released}
},
_previousStatus(_currentStatus){

}
