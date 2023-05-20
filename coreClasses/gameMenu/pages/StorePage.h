//
// Created by S.Morgenthaler on 14/05/2023.
//

#ifndef JUMPERBALLAPPLICATION_STOREPAGE_H
#define JUMPERBALLAPPLICATION_STOREPAGE_H

#include "Page.h"

class StorePage;

using StorePage_sptr = std::shared_ptr<StorePage>;

class StorePage : public Page {
private:
    static constexpr size_t numberOfSkins = 6;
    static constexpr size_t ballSkinNumberOfFields = 5;
    static const short backgroundId;
    struct BallSkin {
        Label_sptr ballLabel;
        Node_sptr priceNode;
        Node_sptr selectNode;
        Label_sptr coinSymbol;
        Node_sptr coveringNode; // Used to click on it
    };

public:
    static StorePage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );

    StorePage(
        Player_sptr &&player,
        Node_sptr &&storeNode,
        Node_sptr &&exitNode,
        std::array<BallSkin, numberOfSkins> &&ballSkins,
        Label_sptr &&backgroundLabel,
        const Page_sptr &parent
    );

    void update(const Mouse &mouse) override;

    Page_sptr click(float mouseX, float mouseY) override;

    void resize(float ratio) override;

    Displayable::DynamicNames getDynamicIntNames() const override;

    Displayable::DynamicValues<int> getDynamicIntValues() const override;

    Page_wptr parent() override;

    std::string getVertexShaderName() const override;

    vecCstTextNode_uptr genTextNodes() const override;

    std::vector<std::string> shaderDefines() const override;

    vecCstLabel_sptr labels() const override;

private:

    const Page_wptr _parent;

    Node_sptr _storeNode;
    Node_sptr _exitNode;
    std::array<BallSkin, numberOfSkins> _ballSkins;

    Label_sptr _backgroundLabel;

    static vecNode_sptr createNodes(float coveringNode, bool english);
    static std::array<BallSkin, numberOfSkins> createBallSkins(const vecNode_sptr& nodes);
};


#endif //JUMPERBALLAPPLICATION_STOREPAGE_H
