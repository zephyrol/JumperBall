//
// Created by S.Morgenthaler on 20/09/2022.
//

#include "InGamePage.h"

Page_wptr InGamePage::parent() {
    return _parent;
}

void InGamePage::setParent(Page_sptr parent) {
    _parent = std::move(parent);
}

InGamePage::InGamePage(Player_sptr player) :
    Page(std::move(player)),
    _parent(nullptr) {

}

Page_sptr InGamePage::click(float, float) {
    return nullptr;
}

Page::NodeMessageAssociations InGamePage::nodeToMessage() const {
    return {};
}

void InGamePage::resize(float) {
}
