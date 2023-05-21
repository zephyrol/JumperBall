//
// Created by S.Morgenthaler on 20/09/2022.
//

#include "SuccessPage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/RightNode.h"
#include "gameMenu/nodes/HorizontalNode.h"

SuccessPage::SuccessPage(
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
) : Page(std::move(player)),
    _parent(parent),
    _goodGameNode(std::move(goodGameNode)),
    _continueNode(std::move(continueNode)),
    _exitNode(std::move(exitNode)),
    _previousDigitOne(std::move(previousDigitOne)),
    _previousDigitTwo(std::move(previousDigitTwo)),
    _previousDigitThree(std::move(previousDigitThree)),
    _previousDigitFour(std::move(previousDigitFour)),
    _obtainedDigitOne(std::move(obtainedDigitOne)),
    _obtainedDigitTwo(std::move(obtainedDigitTwo)),
    _plusNode(std::move(plusNode)),
    _sumDigitOne(std::move(sumDigitOne)),
    _sumDigitTwo(std::move(sumDigitTwo)),
    _sumDigitThree(std::move(sumDigitThree)),
    _sumDigitFour(std::move(sumDigitFour)),
    _backgroundLabel(std::move(backgroundLabel)),
    _operationLine(std::move(operationLine)),
    _coinSymbol(std::move(coinSymbol)),
    _inGamePage(nullptr) {
}

SuccessPage_sptr SuccessPage::createInstance(
    Player_sptr player,
    const Page_sptr &parent,
    float ratio
) {
    auto nodes = createNodes(ratio);

    return std::make_shared<SuccessPage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        std::move(nodes.at(3)),
        std::move(nodes.at(4)),
        std::move(nodes.at(5)),
        std::move(nodes.at(6)),
        std::move(nodes.at(7)),
        std::move(nodes.at(8)),
        std::move(nodes.at(9)),
        std::move(nodes.at(10)),
        std::move(nodes.at(11)),
        std::move(nodes.at(12)),
        std::move(nodes.at(13)),
        createBackgroundLabel(std::move(nodes.at(14))),
        createOperationLine(std::move(nodes.at(15))),
        std::make_shared<Label>(std::move(nodes.at(16)), JBTypes::Color::Yellow, coinSymbolLabelId),
        parent
    );
}

vecNode_sptr SuccessPage::createNodes(float ratio) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainTitleNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    const auto youDidItNode = std::make_shared<UpNode>(
        mainTitleNode,
        4.f
    );

    const auto optionsNode = std::make_shared<DownNode>(
        mainTitleNode,
        9.f / 6.f
    );

    constexpr float optionsNodeRatio = 7.f;
    const auto continueNode = std::make_shared<UpNode>(
        optionsNode,
        optionsNodeRatio
    );

    const auto exitNode = std::make_shared<DownNode>(
        optionsNode,
        optionsNodeRatio
    );

    const auto scoreBodyNode = std::make_shared<UpNode>(
        mainTitleNode,
        9.f / 8.f
    );

    const auto scoreNode = std::make_shared<DownNode>(
        scoreBodyNode,
        9.f / 4.f
    );

    const auto scoreNodeChildRatio = scoreNode->ratio() * 2.f;

    const auto previousObtainedNode = std::make_shared<UpNode>(
        scoreNode,
        scoreNodeChildRatio
    );

    const auto previousObtainedNodeChildRatio = previousObtainedNode->ratio() * 2.f;

    const auto createDigitNode = [](const Node_sptr &parent, float positionX) {
        constexpr auto digitRatio = 1.f;
        constexpr auto width = 9.f;
        return std::make_shared<HorizontalNode>(
            parent,
            digitRatio,
            positionX / width
        );
    };

    const auto createFirstDigitNode = [&createDigitNode](const Node_sptr &parent) {
        return createDigitNode(parent, 8.f);
    };

    const auto createSecondDigitNode = [&createDigitNode](const Node_sptr &parent) {
        return createDigitNode(parent, 6.5f);
    };

    const auto createThirdDigitNode = [&createDigitNode](const Node_sptr &parent) {
        return createDigitNode(parent, 5.f);
    };

    const auto createFourthDigitNode = [&createDigitNode](const Node_sptr &parent) {
        return createDigitNode(parent, 3.5f);
    };

    const auto createLeftNode = [&createDigitNode](const Node_sptr &parent) {
        return createDigitNode(parent, 1.f);
    };

    const auto previousNode = std::make_shared<UpNode>(
        previousObtainedNode,
        previousObtainedNodeChildRatio
    );

    const auto digitOneNode = createFirstDigitNode(previousNode);
    const auto digitTwoNode = createSecondDigitNode(previousNode);
    const auto digitThreeNode = createThirdDigitNode(previousNode);
    const auto digitFourNode = createFourthDigitNode(previousNode);

    const auto obtainedNode = std::make_shared<DownNode>(
        previousObtainedNode,
        previousObtainedNodeChildRatio
    );

    const auto unitDigit = createFirstDigitNode(obtainedNode);
    const auto tensDigit = createSecondDigitNode(obtainedNode);
    const auto plusNode = createLeftNode(obtainedNode);

    const auto lineAndSumNode = std::make_shared<DownNode>(
        scoreNode,
        scoreNodeChildRatio
    );

    const auto lineNode = std::make_shared<CenteredNode>(
        lineAndSumNode,
        previousObtainedNodeChildRatio * 12.f
    );

    const auto sumNode = std::make_shared<DownNode>(
        lineAndSumNode,
        previousObtainedNodeChildRatio
    );

    const auto sumDigitOneNode = createFirstDigitNode(sumNode);
    const auto sumDigitTwoNode = createSecondDigitNode(sumNode);
    const auto sumDigitThreeNode = createThirdDigitNode(sumNode);
    const auto sumDigitFourNode = createFourthDigitNode(sumNode);

    const auto coinNodeBase = createLeftNode(sumNode);
    const auto scaledCoinNode = std::make_shared<DownNode>(
        coinNodeBase,
        1.4f
    );
    const auto coinNode = std::make_shared<CenteredNode>(
        scaledCoinNode,
        1.f
    );

    return {
        youDidItNode,
        continueNode,
        exitNode,
        digitOneNode,
        digitTwoNode,
        digitThreeNode,
        digitFourNode,
        unitDigit,
        tensDigit,
        plusNode,
        sumDigitOneNode,
        sumDigitTwoNode,
        sumDigitThreeNode,
        sumDigitFourNode,
        mainTitleNode,
        lineNode,
        coinNode
    };

}

void SuccessPage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _goodGameNode = nodes.at(0);
    _continueNode = nodes.at(1);
    _exitNode = nodes.at(2);
    _previousDigitOne = nodes.at(3);
    _previousDigitTwo = nodes.at(4);
    _previousDigitThree = nodes.at(5);
    _previousDigitFour = nodes.at(6);
    _obtainedDigitOne = nodes.at(7);
    _obtainedDigitTwo = nodes.at(8);
    _plusNode = nodes.at(9);
    _sumDigitOne = nodes.at(10);
    _sumDigitTwo = nodes.at(11);
    _sumDigitThree = nodes.at(12);
    _sumDigitFour = nodes.at(13);
    _backgroundLabel = createBackgroundLabel(nodes.at(14));
    _operationLine = createOperationLine(nodes.at(15));
    _coinSymbol = std::make_shared<Label>(nodes.at(16), JBTypes::Color::Yellow, coinSymbolLabelId);
}

Page_wptr SuccessPage::parent() {
    return _parent;
}

void SuccessPage::setInGamePage(Page_sptr inGamePage) {
    _inGamePage = std::move(inGamePage);
}

Page_sptr SuccessPage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_exitNode)) {
        _player->addValidationSound();
        return _parent.lock();
    }
    if (intersectTest(_continueNode)) {
        const auto nextLevel = _player->getCurrentLevel() + 1;
        if (nextLevel > Player::maxLevel) {
            return nullptr;
        }
        _player->setCurrentLevel(nextLevel);
        return _inGamePage;
    }
    return nullptr;
}

vecCstTextNode_uptr SuccessPage::genTextNodes() const {
    vecCstTextNode_uptr textNodes;
    const auto english = _player->isUsingEnglishLanguage();
    textNodes.emplace_back(new TextNode(_goodGameNode, english ? "You did it!" : "Bien jou;!", 0));
    textNodes.emplace_back(new TextNode(
        _continueNode,
        english ? "Next level" : "Niveau suivant",
        continueLabelId
    ));
    textNodes.emplace_back(new TextNode(_exitNode, english ? "Exit" : "Sortir", exitLabelId));

    auto nodeCount = static_cast<short>(SuccessPage::digitsIdOffset);
    for (const auto &node: std::vector<Node_sptr>{
        _previousDigitOne,
        _previousDigitTwo,
        _previousDigitThree,
        _previousDigitFour,
        _obtainedDigitOne,
        _obtainedDigitTwo,
        _sumDigitOne,
        _sumDigitTwo,
        _sumDigitThree,
        _sumDigitFour
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
    textNodes.emplace_back(new TextNode(_plusNode, "+", 0));
    return textNodes;
}

Displayable::DynamicNames SuccessPage::getDynamicIntNames() const {
    decltype(getDynamicIntNames()) dynamicNames{
        "previousDigitOne",
        "previousDigitTwo",
        "previousDigitThree",
        "previousDigitFour",
        "obtainedDigitOne",
        "obtainedDigitTwo",
        "sumDigitOne",
        "sumDigitTwo",
        "sumDigitThree",
        "sumDigitFour"
    };
    auto pageDynamicNames = Page::getDynamicIntNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(pageDynamicNames.begin()),
        std::make_move_iterator(pageDynamicNames.end())
    );
    return dynamicNames;
}

Displayable::DynamicValues<int> SuccessPage::getDynamicIntValues() const {

    const auto previousMoney = static_cast<int>(_player->getPreviousMoney());
    const auto sumMoney = static_cast<int>(_player->getMoney());
    const auto diffMoney = sumMoney - previousMoney;

    const auto previousDigitOne = previousMoney % 10;
    const auto previousDigitTwo = (previousMoney % 100) / 10;
    const auto previousDigitThree = (previousMoney % 1000) / 100;
    const auto previousDigitFour = previousMoney / 1000;

    const auto obtainedDigitOne = diffMoney % 10;
    const auto obtainedDigitTwo = diffMoney / 10;

    const auto sumDigitOne = sumMoney % 10;
    const auto sumDigitTwo = (sumMoney % 100) / 10;
    const auto sumDigitThree = (sumMoney % 1000) / 100;
    const auto sumDigitFour = sumMoney / 1000;

    constexpr auto previousDigitOneOffset = digitsIdOffset;
    constexpr auto previousDigitTwoOffset = previousDigitOneOffset + 10;
    constexpr auto previousDigitThreeOffset = previousDigitTwoOffset + 10;
    constexpr auto previousDigitFourOffset = previousDigitThreeOffset + 10;
    constexpr auto obtainedDigitOneOffset = previousDigitFourOffset + 10;
    constexpr auto obtainedDigitTwoOffset = obtainedDigitOneOffset + 10;
    constexpr auto sumDigitOneOffset = obtainedDigitTwoOffset + 10;
    constexpr auto sumDigitTwoOffset = sumDigitOneOffset + 10;
    constexpr auto sumDigitThreeOffset = sumDigitTwoOffset + 10;
    constexpr auto sumDigitFourOffset = sumDigitThreeOffset + 10;

    decltype(getDynamicIntValues()) dynamicInts{
        previousDigitOneOffset + previousDigitOne,
        previousDigitTwoOffset + previousDigitTwo,
        previousDigitThreeOffset + previousDigitThree,
        previousDigitFourOffset + previousDigitFour,
        obtainedDigitOneOffset + obtainedDigitOne,
        obtainedDigitTwoOffset + obtainedDigitTwo,
        sumDigitOneOffset + sumDigitOne,
        sumDigitTwoOffset + sumDigitTwo,
        sumDigitThreeOffset + sumDigitThree,
        sumDigitFourOffset + sumDigitFour
    };

    auto pageDynamicInts = Page::getDynamicIntValues();
    dynamicInts.insert(
        dynamicInts.end(),
        std::make_move_iterator(pageDynamicInts.begin()),
        std::make_move_iterator(pageDynamicInts.end())
    );
    return dynamicInts;
}

std::vector<std::string> SuccessPage::shaderDefines() const {
    return {"TRANSPARENT_BACKGROUND", "DISCARDING", "TEST_COIN"};
}

std::string SuccessPage::getVertexShaderName() const {
    return "successPageVs.vs";
}

void SuccessPage::update(const Mouse &mouse) {

    if (!mouse.isPressed()) {
        _currentSelectedLabel = 0;
        return;
    }

    // Positions have to be centered
    const auto mouseX = mouse.currentXCoord() - 0.5f;
    const auto mouseY = mouse.currentYCoord() - 0.5f;
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_continueNode)) {
        _currentSelectedLabel = continueLabelId;
    } else if (intersectTest(_exitNode)) {
        _currentSelectedLabel = exitLabelId;
    } else {
        _currentSelectedLabel = 0;
    }
}

vecCstLabel_sptr SuccessPage::labels() const {
    return {_backgroundLabel, _operationLine, _coinSymbol};
}

const int SuccessPage::operationLineLabelId = 3;

Label_sptr SuccessPage::createOperationLine(Node_sptr node) {
    return std::make_shared<Label>(
        std::move(node),
        JBTypes::Color::White,
        SuccessPage::operationLineLabelId
    );
}

const int SuccessPage::coinSymbolLabelId = 400;
