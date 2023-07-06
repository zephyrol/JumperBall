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

    explicit KeyboardKey(
        std::vector<std::function<void()> > &&actionFunctions,
        std::function<void()> &&releaseFunction
    );

    void press(const Button &button);

    void release(const Button &button);

    void update();

private:
    // For each Button (respecting the order up, down, left ...), its status
    using KeyStatus = std::vector<Status>;

    const std::vector<std::function<void()> > _actionFunctions;
    const std::function<void()> _releaseFunction;
    KeyStatus _currentStatus;
    KeyStatus _previousStatus;
};


#endif //JUMPERBALLAPPLICATION_KEYBOARDKEY_H
