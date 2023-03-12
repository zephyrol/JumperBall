//
// Created by sebastien on 21/09/22.
//

#ifndef JUMPERBALLAPPLICATION_FAILUREPAGE_H
#define JUMPERBALLAPPLICATION_FAILUREPAGE_H

#include "Page.h"

class FailurePage;

using FailurePage_sptr = std::shared_ptr<FailurePage>;

class FailurePage : public Page {
public:
    static FailurePage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );

    FailurePage(
        Player_sptr &&player,
        Node_sptr &&failureNode,
        Node_sptr &&retryNode,
        Node_sptr &&exitNode,
        const Page_sptr &parent
    );

    Page_sptr click(float mouseX, float mouseY) override;

    void resize(float ratio) override;

    Page_wptr parent() override;

    void setInGamePage(Page_sptr inGamePage);

    vecCstTextNode_uptr genTextNodes() const override;

    std::vector<std::string> shaderDefines() const override;

    std::string getVertexShaderName() const override;

    void update(const Mouse &mouse) override;

private:
    const Page_wptr _parent;
    Node_sptr _failureNode;
    Node_sptr _retryNode;
    Node_sptr _exitNode;

    Page_sptr _inGamePage;

    static constexpr int retryLabelId = 1;
    static constexpr int exitLabelId = 2;

    static vecNode_sptr createNodes(float ratio);
};


#endif //JUMPERBALLAPPLICATION_FAILUREPAGE_H
