/*
 * File: Screen.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 juillet 2020, 8:41
 */

#include "Screen.h"

Screen::Screen() {
}

Displayable::GlobalState Screen::getGlobalState() const {
    return Displayable::GlobalState::United;
}
