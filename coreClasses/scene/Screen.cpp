/*
 * File: Screen.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 juillet 2020, 8:41
 */

#include "Screen.h"

Screen::Screen() {
}

SceneElement::GlobalState Screen::getGlobalState() const {
    return SceneElement::GlobalState::United;
}
