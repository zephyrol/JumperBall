//
// Created by S.Morgenthaler on 20/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_SUCCESSPAGE_H
#define JUMPERBALLAPPLICATION_SUCCESSPAGE_H

#include "Page.h"

class SuccessPage;

using SuccessPage_sptr = std::shared_ptr<SuccessPage>;

class SuccessPage : public Page {
public:
    static SuccessPage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );

    SuccessPage(
        Player_sptr &&player,
        Node_sptr &&goodGameNode,
        Node_sptr &&continueNode,
        Node_sptr &&exitNode,
        Label_sptr &&backgroundLabel,
        const Page_sptr &parent
    );

    void update(const Mouse &mouse) override;

    Page_sptr click(float mouseX, float mouseY) override;

    void resize(float ratio) override;

    Page_wptr parent() override;

    void setInGamePage(Page_sptr inGamePage);

    std::string getVertexShaderName() const override;

    vecCstTextNode_uptr genTextNodes() const override;

    std::vector<std::string> shaderDefines() const override;

    vecCstLabel_sptr labels() const override;

private:
    const Page_wptr _parent;
    Node_sptr _goodGameNode;
    Node_sptr _continueNode;
    Node_sptr _exitNode;

    Label_sptr _backgroundLabel;

    Page_sptr _inGamePage;

    static constexpr int continueLabelId = 1;
    static constexpr int exitLabelId = 2;

    static vecNode_sptr createNodes(float ratio);
};


#endif //JUMPERBALLAPPLICATION_SUCCESSPAGE_H
