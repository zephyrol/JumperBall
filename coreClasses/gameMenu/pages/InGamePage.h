//
// Created by S.Morgenthaler on 20/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_INGAMEPAGE_H
#define JUMPERBALLAPPLICATION_INGAMEPAGE_H


#include "Page.h"
#include "gameMenu/labels/ArrowLabel.h"
#include "scene/Map.h"

class InGamePage;

using InGamePage_sptr = std::shared_ptr<InGamePage>;

class InGamePage : public Page {


public:
    explicit InGamePage(
        Player_sptr &&player,
        ArrowLabel_sptr arrowLabel,
        Node_sptr &&leftDigitNode,
        Node_sptr &&middleDigitNode,
        Node_sptr &&rightDigitNode,
        const Page_sptr &parent
    );

    static InGamePage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );


    static std::shared_ptr<ArrowLabel> createInGameArrowLabel(const Node_sptr &headerNode);

    Page_wptr parent() override;

    vecCstLabel_sptr labels() const override;

    Page_sptr click(float mouseX, float mouseY) override;

    vecCstTextNode_uptr genTextNodes() const override;

    void update(const Mouse &mouse) override;

    void resize(float ratio) override;

    std::vector<std::string> shaderDefines() const override;

    DynamicNames getDynamicIntNames() const override;

    DynamicValues<int> getDynamicIntValues() const override;

    std::string getVertexShaderName() const override;

private:

    Page_wptr _parent;
    ArrowLabel_sptr _arrowLabel;
    Node_sptr _leftDigitNode;
    Node_sptr _middleDigitNode;
    Node_sptr _rightDigitNode;

    static const int arrowLabelId;

    static vecNode_sptr createNodes(float ratio);
};


#endif //JUMPERBALLAPPLICATION_INGAMEPAGE_H
