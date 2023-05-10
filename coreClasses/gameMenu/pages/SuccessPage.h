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
        Node_sptr &&previousDigitOne,
        Node_sptr &&previousDigitTwo,
        Node_sptr &&previousDigitThree,
        Node_sptr &&previousDigitFour,
        Node_sptr &&obtainedDigitOne,
        Node_sptr &&obtainedDigitTwo,
        Node_sptr &&plusNode,
        Node_sptr &&sumDigitOne,
        Node_sptr &&sumDigitTwo,
        Node_sptr &&sumDigitThree,
        Node_sptr &&sumDigitFour,
        Label_sptr &&backgroundLabel,
        Label_sptr &&operationLine,
        Label_sptr &&coinSymbol,
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
    Node_sptr _previousDigitOne;
    Node_sptr _previousDigitTwo;
    Node_sptr _previousDigitThree;
    Node_sptr _previousDigitFour;
    Node_sptr _obtainedDigitOne;
    Node_sptr _obtainedDigitTwo;
    Node_sptr _plusNode;
    Node_sptr _sumDigitOne;
    Node_sptr _sumDigitTwo;
    Node_sptr _sumDigitThree;
    Node_sptr _sumDigitFour;

    Label_sptr _backgroundLabel;
    Label_sptr _operationLine;
    Label_sptr _coinSymbol;

    Page_sptr _inGamePage;

    static constexpr int continueLabelId = 1;
    static constexpr int exitLabelId = 2;
    static const int operationLineLabelId;
    static const int coinSymbolLabelId;

    static vecNode_sptr createNodes(float ratio);

    static Label_sptr createOperationLine(Node_sptr node);
};


#endif //JUMPERBALLAPPLICATION_SUCCESSPAGE_H
