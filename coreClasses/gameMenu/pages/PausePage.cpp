//
// Created by S.Morgenthaler on 21/09/22.
//

#include "PausePage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/RightNode.h"
#include "gameMenu/nodes/LeftNode.h"

PausePage::PausePage(
    Player_sptr &&player,
    Node_sptr &&levelNode,
    Node_sptr &&leftDigitNode,
    Node_sptr &&middleDigitNode,
    Node_sptr &&rightDigitNode,
    Node_sptr &&resumeNode,
    Node_sptr &&exitNode,
    Label_sptr &&backgroundLabel,
    const Page_sptr &parent
)
    : Page(std::move(player)),
      _parent(parent),
      _inGamePage(nullptr),
      _levelNode(std::move(levelNode)),
      _leftDigitNode(std::move(leftDigitNode)),
      _middleDigitNode(std::move(middleDigitNode)),
      _rightDigitNode(std::move(rightDigitNode)),
      _resumeNode(std::move(resumeNode)),
      _exitNode(std::move(exitNode)),
      _backgroundLabel(std::move(backgroundLabel)) {
}

PausePage_sptr PausePage::createInstance(
    Player_sptr player,
    const Page_sptr &parent,
    float ratio
) {
    auto nodes = createNodes(ratio);
    return std::make_shared<PausePage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        std::move(nodes.at(3)),
        std::move(nodes.at(4)),
        std::move(nodes.at(5)),
        createBackgroundLabel(std::move(nodes.at(6))),
        parent
    );
}

void PausePage::setInGamePage(Page_sptr inGamePage) {
    _inGamePage = std::move(inGamePage);
}

vecNode_sptr PausePage::createNodes(float ratio) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainTitleNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    const auto headerNode = std::make_shared<UpNode>(
        mainTitleNode,
        6.f
    );

    const auto digitsNode = std::make_shared<RightNode>(headerNode, 2.f);
    constexpr auto digitNodesRatio = 1.f;
    const auto leftDigitNode = std::make_shared<LeftNode>(digitsNode, digitNodesRatio);
    const auto middleDigitNode = std::make_shared<CenteredNode>(digitsNode, digitNodesRatio);
    const auto rightDigitNode = std::make_shared<RightNode>(digitsNode, digitNodesRatio);

    const auto levelNode = std::make_shared<LeftNode>(
        headerNode,
        4.f
    );

    const auto optionsParentNode = std::make_shared<CenteredNode>(
        mainTitleNode,
        1.5f
    );
    constexpr float optionsNodeRatio = 7.f;

    const auto resumeNode = std::make_shared<UpNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    const auto exitNode = std::make_shared<DownNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    return {
        levelNode,
        leftDigitNode,
        middleDigitNode,
        rightDigitNode,
        resumeNode,
        exitNode,
        optionsParentNode
    };

}

void PausePage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _levelNode = nodes.at(0);
    _leftDigitNode = nodes.at(1);
    _middleDigitNode = nodes.at(2);
    _rightDigitNode = nodes.at(3);
    _resumeNode = nodes.at(4);
    _exitNode = nodes.at(5);
    _backgroundLabel = createBackgroundLabel(nodes.at(6));
}

Page_wptr PausePage::parent() {
    return _parent;
}

Page_sptr PausePage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY, 2.f);
    };
    if (intersectTest(_exitNode)) {
        _player->addValidationSound();
        return _parent.lock();
    }
    if (intersectTest(_resumeNode)) {
        _player->setAsInGame();
        return _inGamePage;
    }
    return nullptr;
}

vecCstTextNode_uptr PausePage::genTextNodes() const {
    vecCstTextNode_uptr textNodes;
    const auto english = _player->isUsingEnglishLanguage();
    textNodes.emplace_back(new TextNode(_levelNode, english ? "Level" : "Niveau", levelLabelId));
    textNodes.emplace_back(new TextNode(_resumeNode, english ? "Resume" : "Reprendre", resumeLabelId));
    textNodes.emplace_back(new TextNode(_exitNode, english ? "Exit" : "Sortir", exitLabelId));

    short nodeCount = 0;
    for (const auto &node: std::vector<Node_sptr>{
        _leftDigitNode,
        _middleDigitNode,
        _rightDigitNode
    }) {
        for (unsigned int i = 0; i < 10; ++i) {
            textNodes.emplace_back(new TextNode(
                node,
                std::to_string(i),
                nodeCount
            ));
            ++nodeCount;
        }
    }
    return textNodes;
}

std::vector<std::string> PausePage::shaderDefines() const {
    return {"TRANSPARENT_BACKGROUND"};
}

std::string PausePage::getVertexShaderName() const {
    return "pausePageVs.vs";
}

Displayable::DynamicNames PausePage::getDynamicIntNames() const {
    decltype(getDynamicIntNames()) dynamicNames{
        "leftDigit",
        "middleDigit",
        "rightDigit",
    };
    auto pageDynamicNames = Page::getDynamicIntNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(pageDynamicNames.begin()),
        std::make_move_iterator(pageDynamicNames.end())
    );
    return dynamicNames;
}

Displayable::DynamicValues<int> PausePage::getDynamicIntValues() const {

    const auto currentLevel = static_cast<int>(_player->getCurrentLevel());
    const auto leftDigit = currentLevel / 100;
    const auto middleDigit = (currentLevel % 100) / 10;
    const auto rightDigit = currentLevel % 10;

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


void PausePage::update(const Mouse &mouse) {

    if (!mouse.isPressed()) {
        _currentSelectedLabel = -1;
        return;
    }

    // Positions have to be centered
    const auto mouseX = mouse.currentXCoord() - 0.5f;
    const auto mouseY = mouse.currentYCoord() - 0.5f;
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY, 2.f);
    };
    if (intersectTest(_resumeNode)) {
        _currentSelectedLabel = resumeLabelId;
    } else if (intersectTest(_exitNode)) {
        _currentSelectedLabel = exitLabelId;
    } else {
        _currentSelectedLabel = -1;
    }
}

vecCstLabel_sptr PausePage::labels() const {
    return {_backgroundLabel};
}
