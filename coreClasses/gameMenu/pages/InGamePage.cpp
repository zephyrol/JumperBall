//
// Created by S.Morgenthaler on 20/09/2022.
//

#include "InGamePage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/RightNode.h"

Page_wptr InGamePage::parent() {
    return _parent;
}

InGamePage::InGamePage(
    Player_sptr &&player,
    ArrowLabel_sptr &&arrowLabel,
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
    auto arrowLabel = std::make_shared<ArrowLabel>(nodes[0], JBTypes::Color::Blue);
    return std::make_shared<InGamePage>(
        std::move(player),
        std::move(arrowLabel),
        std::move(nodes[1]),
        std::move(nodes[2]),
        std::move(nodes[3]),
        parent
    );
}

Page_sptr InGamePage::click(float, float) {
    return nullptr;
}

void InGamePage::resize(float ratio) {
    const auto nodes = createNodes(ratio);
    _arrowLabel = std::make_shared<ArrowLabel>(nodes[0], JBTypes::Color::Blue);
    _leftDigitNode = nodes[1];
    _middleDigitNode = nodes[2];
    _rightDigitNode = nodes[3];
}

vecCstTextNode_uptr InGamePage::genTextNodes() const {
    decltype(genTextNodes()) textNodes;

    auto digitId = 1.f;
    for (const auto &node: std::vector<Node_sptr>{_leftDigitNode, _middleDigitNode, _rightDigitNode}) {
        for (unsigned int i = 0; i < 10; ++i) {
            textNodes.push_back(CstTextNode_uptr(new TextNode(
                node,
                std::to_string(i),
                {1.f, digitId}
            )));
        }
        digitId += 1.f;
    }

    return textNodes;
}

vecNode_sptr InGamePage::createNodes(float ratio) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    // const auto mainTitleNode = std::make_shared<CenteredNode>(
    //     resizedScreenNode,
    //     9.f / 16.f
    // );
    const auto notchNode = std::make_shared<UpNode>(resizedScreenNode, 8.f);

    constexpr auto sideNodesRatio = 1.f;
    const auto arrowNode = std::make_shared<LeftNode>(notchNode, sideNodesRatio);

    const auto timerNode = std::make_shared<RightNode>(notchNode, sideNodesRatio);
    const auto digitsNode = std::make_shared<CenteredNode>(notchNode, 3.f);
    constexpr auto digitNodesRatio = 1.f;
    const auto leftDigitNode = std::make_shared<LeftNode>(digitsNode, digitNodesRatio);
    const auto middleDigitNode = std::make_shared<CenteredNode>(digitsNode, digitNodesRatio);
    const auto rightDigitNode = std::make_shared<RightNode>(digitsNode, digitNodesRatio);

    return {arrowNode, leftDigitNode, middleDigitNode, rightDigitNode};

}

std::string InGamePage::shaderDefine() const {
    return "TIMER";
}

std::vector<std::string> InGamePage::getUniformNames() const {
    return {"leftDigit", "middleDigit", "rightDigit"};
}

std::vector<float> InGamePage::getUniformValues(const CstMap_sptr &map) const {
    const auto remainingTime = static_cast<unsigned int> (std::ceilf(map->remainingTime()));
    const auto leftDigit = remainingTime / 100;
    const auto middleDigit = (remainingTime % 100) / 10;
    const auto rightDigit = remainingTime % 10;
    return {static_cast<float>(leftDigit), static_cast<float>(middleDigit), static_cast<float>(rightDigit)};
}

