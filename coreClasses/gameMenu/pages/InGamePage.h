//
// Created by S.Morgenthaler on 20/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_INGAMEPAGE_H
#define JUMPERBALLAPPLICATION_INGAMEPAGE_H


#include "Page.h"
#include "gameMenu/labels/ArrowLabel.h"
#include "scene/ItemsContainer.h"
#include "scene/MovableObject.h"
#include "tutorials/Tutorial.h"

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
        Node_sptr &&coinsTensDigit,
        Node_sptr &&coinsUnitsDigit,
        Node_sptr &&topTutorialNode,
        Node_sptr &&bottomTutorialNode,
        Label_sptr key1,
        Label_sptr key2,
        Label_sptr key3,
        Label_sptr key4,
        Label_sptr coinSymbol,
        const Page_sptr &parent,
        CstItemsContainer_sptr itemsContainer,
        Tutorial_uptr tutorial,
        glm::vec2 tutorialCenter,
        glm::vec2 ratioAndInverse
    );

    static InGamePage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio,
        CstItemsContainer_sptr itemsContainer,
        Tutorial_uptr tutorial
    );


    static std::shared_ptr<ArrowLabel> createInGameArrowLabel(const Node_sptr &headerNode);

    Page_wptr parent() override;

    vecCstLabel_sptr labels() const override;

    Page_sptr click(float mouseX, float mouseY) override;

    vecCstTextNode_uptr genTextNodes() const override;

    void update(const Mouse &mouse) override;

    void resize(float ratio) override;

    void setItemsContainer(CstItemsContainer_sptr itemsContainer);

    void setTutorial(Tutorial_uptr tutorial);

    std::vector<std::string> shaderDefines() const override;

    DynamicNames getDynamicIntNames() const override;

    DynamicNames getDynamicFloatNames() const override;

    DynamicValues<float> getDynamicFloatValues() const override;

    DynamicNames getDynamicVec2fNames() const override;

    DynamicValues<glm::vec2> getDynamicVec2fValues() const override;

    DynamicValues<int> getDynamicIntValues() const override;

    std::string getVertexShaderName() const override;

private:
    Page_wptr _parent;
    ArrowLabel_sptr _arrowLabel;
    Node_sptr _leftDigitNode;
    Node_sptr _middleDigitNode;
    Node_sptr _rightDigitNode;
    Node_sptr _coinsTensDigit;
    Node_sptr _coinsUnitsDigit;
    Node_sptr _topTutorialNode;
    Node_sptr _bottomTutorialNode;
    Label_sptr _key1;
    Label_sptr _key2;
    Label_sptr _key3;
    Label_sptr _key4;
    Label_sptr _coinSymbol;
    Tutorial_uptr _tutorial;
    glm::vec2 _tutorialCenter;
    glm::vec2 _ratioAndInverse;
    CstItemsContainer_sptr _itemsContainer;

    /**
     * Get the tutorial center from top and bottom nodes.
     * @return The position in the projected [-1,1] space
     */
    static glm::vec2 getTutorialCenter(const Node_sptr& top, const Node_sptr& bottom);

    /**
     * Get the ratio and its inverse
     */
    static glm::vec2 getRatioAndInverse(float ratio);

    static const int arrowLabelId;
    static const int key1LabelId;
    static const int key2LabelId;
    static const int key3LabelId;
    static const int key4LabelId;
    static const int coinSymbolLabelId;
    static const short tutorialFirstMessageTopLabelId;
    static const short tutorialFirstMessageBottomLabelId;

    static vecNode_sptr createNodes(float ratio);
};


#endif //JUMPERBALLAPPLICATION_INGAMEPAGE_H
