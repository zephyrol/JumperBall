//
// Created by S.Morgenthaler on 14/05/2023.
//

#ifndef JUMPERBALLAPPLICATION_STOREPAGE_H
#define JUMPERBALLAPPLICATION_STOREPAGE_H

#include "Page.h"

class StorePage;

using StorePage_sptr = std::shared_ptr<StorePage>;

class StorePage : public Page {
public:
    static constexpr size_t numberOfSkins = 6;
private:
    static constexpr size_t ballSkinNumberOfFields = 5;
    static const short backgroundId;
    struct BallSkin {
        Label_sptr ballLabel;
        Node_sptr priceNode;
        Node_sptr selectNode;
        Label_sptr coinSymbol;
        Label_sptr background; // Also used to click on it
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
        Node_sptr &&sumDigitOne,
        Node_sptr &&sumDigitTwo,
        Node_sptr &&sumDigitThree,
        Node_sptr &&sumDigitFour,
        std::array<BallSkin, numberOfSkins> &&ballSkins,
        Label_sptr &&coinSymbol,
        ArrowLabel_sptr &&arrowLabel,
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

    void setValidationPages(const std::array<Page_sptr, numberOfSkins> &validationPages);

private:

    const Page_wptr _parent;

    Node_sptr _storeNode;
    std::array<BallSkin, numberOfSkins> _ballSkins;

    Node_sptr _sumDigitOne;
    Node_sptr _sumDigitTwo;
    Node_sptr _sumDigitThree;
    Node_sptr _sumDigitFour;
    Label_sptr _coinSymbol;
    ArrowLabel_sptr _arrowLabel;

    std::array<Page_wptr, numberOfSkins> _validationPages;

    static vecNode_sptr createNodes(float coveringNode);

    static std::array<BallSkin, numberOfSkins> createBallSkins(const vecNode_sptr &nodes);

    static ArrowLabel_sptr createStoreArrowLabel(const Node_sptr &headerNode);

    static constexpr int digitsIdOffset = 100;
};


#endif //JUMPERBALLAPPLICATION_STOREPAGE_H
