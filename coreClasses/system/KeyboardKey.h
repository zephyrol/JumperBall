//
// Created by S.Morgenthaler on 08/10/22.
//

#ifndef JUMPERBALLAPPLICATION_KEYBOARDKEY_H
#define JUMPERBALLAPPLICATION_KEYBOARDKEY_H
#include "scene/Scene.h"

class KeyboardKey {
public :
    enum class Status {
        Pressed, Released
    };

    enum class Button {
        Up, Down, Left, Right, Escape, Validate
    };

    KeyboardKey();

private:

    using KeyStatus = std::unordered_map<Button, Status>;
    KeyStatus _currentStatus;
    KeyStatus _previousStatus;
};


#endif //JUMPERBALLAPPLICATION_KEYBOARDKEY_H
