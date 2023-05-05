//
// Created by S.Morgenthaler on 25/03/2023.
//

#ifndef JUMPERBALLAPPLICATION_CREDITSPAGE_H
#define JUMPERBALLAPPLICATION_CREDITSPAGE_H

#include "Page.h"

class CreditsPage;

using CreditsPage_sptr = std::shared_ptr<CreditsPage>;

class CreditsPage : public Page {
public:

    static CreditsPage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );

    void resize(float ratio) override;

    Page_wptr parent() override;

    explicit CreditsPage(
        Player_sptr &&player,
        Node_sptr &&credits,
        Node_sptr &&developmentAndDesign,
        Node_sptr &&mainAuthor,
        Node_sptr &&musicAndSoundsEffects,
        Node_sptr &&secondAuthor,
        Node_sptr &&exitNode,
        Label_sptr &&backgroundLabel,
        const Page_sptr &parent
    );

    vecCstTextNode_uptr genTextNodes() const override;

    Page_sptr click(float mouseX, float mouseY) override;

    void update(const Mouse &mouse) override;

    vecCstLabel_sptr labels() const override;

    std::string getVertexShaderName() const override;

    std::vector<std::string> shaderDefines() const override;

private:
    const Page_wptr _parent;
    Node_sptr _credits;
    Node_sptr _developmentAndDesign;
    Node_sptr _selectNameToGo;
    Node_sptr _toPersonalPage;
    Node_sptr _mainAuthor;
    Node_sptr _musicAndSoundsEffects;
    Node_sptr _secondAuthor;
    Node_sptr _exitNode;
    Label_sptr _backgroundLabel;

    static constexpr int developmentAndDesignLabelId = 1;
    static constexpr int mainAuthorLabelId = 2;
    static constexpr int musicAndSoundsEffectsLabelId = 3;
    static constexpr int secondAuthorLabelId = 4;
    static constexpr int exitNodeLabelId = 5;

    static vecNode_sptr createNodes(float ratio, bool english);

};


#endif //JUMPERBALLAPPLICATION_CREDITSPAGE_H
