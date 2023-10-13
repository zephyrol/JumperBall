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
        Node_sptr &&levelNode,
        Node_sptr &&leftDigitNode,
        Node_sptr &&middleDigitNode,
        Node_sptr &&rightDigitNode,
        Node_sptr &&resumeNode,
        Node_sptr &&exitNode,
        Label_sptr &&backgroundLabel,
        const Page_sptr &parent
    );

    Page_sptr click(float mouseX, float mouseY) override;

    void resize(float ratio) override;

    Page_wptr parent() override;

    void setInGamePage(Page_sptr inGamePage);

    vecCstTextNode_uptr genTextNodes() const override;

    std::string getVertexShaderName() const override;

    std::vector<std::string> shaderDefines() const override;

    void update(const Mouse &mouse) override;

    vecCstLabel_sptr labels() const override;

    DynamicNames getDynamicIntNames() const override;

    DynamicValues<int> getDynamicIntValues() const override;

private:
    const Page_wptr _parent;
    Page_sptr _inGamePage;
    Node_sptr _levelNode;
    Node_sptr _leftDigitNode;
    Node_sptr _middleDigitNode;
    Node_sptr _rightDigitNode;
    Node_sptr _resumeNode;
    Node_sptr _exitNode;

    Label_sptr _backgroundLabel;

    static constexpr int levelLabelId = 500;
    static constexpr int resumeLabelId = 501;
    static constexpr int exitLabelId = 502;

    static vecNode_sptr createNodes(float ratio);
};


#endif //JUMPERBALLAPPLICATION_PAUSEPAGE_H
