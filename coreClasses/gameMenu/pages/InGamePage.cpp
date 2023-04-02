//
// Created by S.Morgenthaler on 20/09/2022.
//

#include "InGamePage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/RightNode.h"

InGamePage::InGamePage(
    Player_sptr &&player,
    ArrowLabel_sptr arrowLabel,
    Node_sptr &&leftDigitNode,
    Node_sptr &&middleDigitNode,
    Node_sptr &&rightDigitNode,
    const Page_sptr &parent
) : Page(std::move(player)),
    _parent(parent),
    _arrowLabel(std::move(arrowLabel)),
    _leftDigitNode(std::move(leftDigitNode)),
    _middleDigitNode(std::move(middleDigitNode)),
    _rightDigitNode(std::move(rightDigitNode)) {
}

InGamePage_sptr InGamePage::createInstance(Player_sptr player, const Page_sptr &parent, float ratio) {
    auto nodes = createNodes(ratio);
    auto arrowLabel = createInGameArrowLabel(nodes[0]);
    auto inGamePage = std::make_shared<InGamePage>(
        std::move(player),
        arrowLabel,
        std::move(nodes[1]),
        std::move(nodes[2]),
        std::move(nodes[3]),
        parent
    );
    return inGamePage;
}

Page_wptr InGamePage::parent() {
    return _parent;
}

Page_sptr InGamePage::click(float mouseX, float mouseY) {
    if (_arrowLabel->getNode()->intersect(mouseX, mouseY)) {
        _player->setAsInMenu();
        return _parent.lock();
    }
    return nullptr;
}

void InGamePage::resize(float ratio) {
    const auto nodes = createNodes(ratio);
    _arrowLabel = createInGameArrowLabel(nodes[0]);
    _leftDigitNode = nodes[1];
    _middleDigitNode = nodes[2];
    _rightDigitNode = nodes[3];
}

vecCstTextNode_uptr InGamePage::genTextNodes() const {
    decltype(genTextNodes()) textNodes;

    short nodeCount = 0;
    for (const auto &node: std::vector<Node_sptr>{_leftDigitNode, _middleDigitNode, _rightDigitNode}) {
        for (unsigned int i = 0; i < 10; ++i) {
            textNodes.push_back(CstTextNode_uptr(new TextNode(
                node,
                std::to_string(i),
                nodeCount
            )));
            ++nodeCount;
        }
    }

    return textNodes;
}

vecNode_sptr InGamePage::createNodes(float ratio) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto notchNode = std::make_shared<UpNode>(resizedScreenNode, 6.f);

    constexpr auto sideNodesRatio = 1.f;
    const auto arrowNode = std::make_shared<LeftNode>(notchNode, sideNodesRatio);

    const auto timerNode = std::make_shared<RightNode>(notchNode, sideNodesRatio);
    const auto digitsNode = std::make_shared<UpNode>(timerNode, 2.f);
    constexpr auto digitNodesRatio = 1.f;
    const auto leftDigitNode = std::make_shared<LeftNode>(digitsNode, digitNodesRatio);
    const auto middleDigitNode = std::make_shared<CenteredNode>(digitsNode, digitNodesRatio);
    const auto rightDigitNode = std::make_shared<RightNode>(digitsNode, digitNodesRatio);

    return {arrowNode, leftDigitNode, middleDigitNode, rightDigitNode};

}

std::vector<std::string> InGamePage::shaderDefines() const {
    return {"DISCARDING", "TEST_ALPHA_TEXTURE"};
}

const int InGamePage::arrowLabelId = -1;

std::string InGamePage::getVertexShaderName() const {
    return "inGamePageVs.vs";
}

Displayable::DynamicNames InGamePage::getDynamicIntNames() const {
    decltype(getDynamicIntNames()) dynamicNames{"leftDigit", "middleDigit", "rightDigit"};
    auto pageDynamicNames = Page::getDynamicIntNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(pageDynamicNames.begin()),
        std::make_move_iterator(pageDynamicNames.end())
    );
    return dynamicNames;
}

Displayable::DynamicValues<int> InGamePage::getDynamicIntValues() const {

    const auto remainingTime = static_cast<int> (std::ceilf(_player->getRemainingTime()));
    const auto leftDigit = remainingTime / 100;
    const auto middleDigit = (remainingTime % 100) / 10;
    const auto rightDigit = remainingTime % 10;
    constexpr auto middleDigitIdOffset = 10;
    constexpr auto rightDigitIdOffset = middleDigitIdOffset + 10;
    decltype(getDynamicIntValues()) dynamicInts{
        leftDigit,
        middleDigit + middleDigitIdOffset,
        rightDigit + rightDigitIdOffset
    };

    auto pageDynamicInts = Page::getDynamicIntValues();
    dynamicInts.insert(
        dynamicInts.end(),
        std::make_move_iterator(pageDynamicInts.begin()),
        std::make_move_iterator(pageDynamicInts.end())
    );
    return dynamicInts;
}

vecCstLabel_sptr InGamePage::labels() const {
    return {_arrowLabel};
}

void InGamePage::update(const Mouse &mouse) {
    if (!mouse.isPressed()) {
        _currentSelectedLabel = 0;
        return;
    }

    // Positions have to be centered
    const auto mouseX = mouse.currentXCoord() - 0.5f;
    const auto mouseY = mouse.currentYCoord() - 0.5f;

    if (_arrowLabel->getNode()->intersect(mouseX, mouseY)) {
        _currentSelectedLabel = InGamePage::arrowLabelId;
    }
}

std::shared_ptr<ArrowLabel> InGamePage::createInGameArrowLabel(const Node_sptr &headerNode) {
    return createArrowLabel(headerNode, InGamePage::arrowLabelId, true, 1.5f);
}
