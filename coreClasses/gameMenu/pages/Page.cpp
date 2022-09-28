#include "Page.h"
#include <cmath>
#include <algorithm>


/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

Page::Page(Player_sptr &&player) : _player(std::move(player)) {}

Page_wptr Page::parent() {
    return {};
}

float Page::localPosY() const {
    return 0.f;
}


void Page::update(bool, float) {
}

vecCstLabel_sptr Page::labels() const {
    return {};
}

const Player_sptr &Page::player() const {
    return _player;
}

