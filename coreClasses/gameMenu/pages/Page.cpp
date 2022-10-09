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

void Page::update(const Mouse &, const JBTypes::timePointMs &) {
}

vecCstLabel_sptr Page::labels() const {
    return {};
}

std::string Page::shaderDefine() const {
    return "";
}

std::vector<std::string> Page::getUniformNames() const {
    return {};
}

std::vector<float> Page::getUniformValues() const {
    return {};
}

