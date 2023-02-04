/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

#include "Page.h"
#include <algorithm>


Page::Page(Player_sptr &&player) : _player(std::move(player)) {}

Page_wptr Page::parent() {
    return {};
}

void Page::update(const Mouse &) {
}

vecCstLabel_sptr Page::labels() const {
    return {};
}

std::vector<std::string> Page::shaderDefines() const {
    return {};
}

std::vector<float> Page::getUniformFloatValues(const CstMap_sptr &) const {
    return {};
}

std::vector<int> Page::getUniformIntValues(const CstMap_sptr &) const {
    return {};
}

Displayable::GlobalState Page::getGlobalState() const {
    return GlobalState::United;
}

