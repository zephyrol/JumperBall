//
// Created by S.Morgenthaler on 21/09/22.
//

#ifndef JUMPERBALLAPPLICATION_PAUSEPAGE_H
#define JUMPERBALLAPPLICATION_PAUSEPAGE_H


#include "Page.h"

class PausePage;

using PausePage_sptr = std::shared_ptr<PausePage>;

class PausePage : public Page {
public:
    static PausePage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );

    PausePage(
        Player_sptr &&player,
        Node_sptr &&jumperBallTitleNode,
        Node_sptr &&resumeNode,
        Node_sptr &&exitNode,
        const Page_sptr &parent
    );

    Page_sptr click(float mouseX, float mouseY) override;

    void resize(float ratio) override;

    Page_wptr parent() override;

    void setInGamePage(Page_sptr inGamePage);

    vecCstTextNode_uptr genTextNodes() const override;

private:
    const Page_wptr _parent;
    Page_sptr _inGamePage;
    Node_sptr _jumperBallTitleNode;
    Node_sptr _resumeNode;
    Node_sptr _exitNode;

    static vecNode_sptr createNodes(float ratio);
};


#endif //JUMPERBALLAPPLICATION_PAUSEPAGE_H
