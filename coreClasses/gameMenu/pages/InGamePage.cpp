//
// Created by S.Morgenthaler on 20/09/2022.
//

#include "InGamePage.h"

Page_wptr InGamePage::parent() {
    return _parent;
}

InGamePage::InGamePage(Player_sptr player, Page_sptr parent) :
    Page(std::move(player)),
    _parent(std::move(parent)) {

}

Page_sptr InGamePage::click(float, float) {
    return nullptr;
}

void InGamePage::resize(float) {
}

vecCstTextNode_uptr InGamePage::genTextNodes() const {
    return {};
}
