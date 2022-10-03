//
// Created by S.Morgenthaler on 20/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_INGAMEPAGE_H
#define JUMPERBALLAPPLICATION_INGAMEPAGE_H


#include "Page.h"

class InGamePage : public Page {

public:
    explicit InGamePage(Player_sptr player, const Page_sptr& parent);

    Page_wptr parent() override;

    Page_sptr click(float mouseX, float mouseY) override;

    vecCstTextNode_uptr genTextNodes() const override;

    void resize(float ratio) override;

private:

    Page_wptr _parent;
};


#endif //JUMPERBALLAPPLICATION_INGAMEPAGE_H
