//
// Created by S.Morgenthaler on 08/10/22.
//

#ifndef JUMPERBALLAPPLICATION_KEYBOARDKEY_H
#define JUMPERBALLAPPLICATION_KEYBOARDKEY_H

#include "system/Types.h"

class KeyboardKey {
public :
    enum class Status {
        Pressed, Released
    };

    enum class Button {
        Up, Down, Left, Right, Escape, Validate
    };

    explicit KeyboardKey(std::unordered_map<Button, std::function<void()> > &&actionFunctions);

    void press(const Button& button);
    void release(const Button& button);

    void update();

private:
    using KeyStatus = std::unordered_map<Button, Status>;

    const std::unordered_map<Button, std::function<void()> > _actionFunctions;
    KeyStatus _currentStatus;
    KeyStatus _previousStatus;
};


#endif //JUMPERBALLAPPLICATION_KEYBOARDKEY_H
