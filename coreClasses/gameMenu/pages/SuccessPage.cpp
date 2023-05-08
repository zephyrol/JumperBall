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
    Node_sptr &&sumDigitOne,
    Node_sptr &&sumDigitTwo,
    Node_sptr &&sumDigitThree,
    Node_sptr &&sumDigitFour,
    Label_sptr &&backgroundLabel,
    Label_sptr &&operationLine,
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
    _sumDigitOne(std::move(sumDigitOne)),
    _sumDigitTwo(std::move(sumDigitTwo)),
    _sumDigitThree(std::move(sumDigitThree)),
    _sumDigitFour(std::move(sumDigitFour)),
    _backgroundLabel(std::move(backgroundLabel)),
    _operationLine(std::move(operationLine)),
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
        createBackgroundLabel(std::move(nodes.at(13))),
        createOperationLine(std::move(nodes.at(14))),
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

    const auto goodGameTitle = std::make_shared<UpNode>(
        mainTitleNode,
        4.f
    );

    const auto bodyNode = std::make_shared<DownNode>(
        mainTitleNode,
        9.f / 16.f
    );

    const auto scoreNode = std::make_shared<UpNode>(
        mainTitleNode,
        9.f / 10.f
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

    const auto scoreNodeChildRatio = scoreNode->ratio() * 2.f;

    const auto previousObtainedNode = std::make_shared<UpNode>(
        scoreNode,
        scoreNodeChildRatio
    );

    const auto previousObtainedNodeChildRatio = previousObtainedNode->ratio() * 2.f;
    const auto previousNode = std::make_shared<UpNode>(
        previousObtainedNode,
        previousObtainedNodeChildRatio
    );
    const auto threeDigitsRatio = previousObtainedNode->ratio() / 2.f;
    const auto threeDigitNode = std::make_shared<RightNode>(
        previousNode,
        threeDigitsRatio
    );
    const auto digitRatio = threeDigitsRatio / 3.f;
    const auto digitOneNode = std::make_shared<RightNode>(
        threeDigitNode,
        digitRatio
    );
    const auto digitTwoNode = std::make_shared<CenteredNode>(
        threeDigitNode,
        digitRatio
    );
    const auto digitThreeNode = std::make_shared<RightNode>(
        threeDigitNode,
        digitRatio
    );

    const auto leftDigitsNode = std::make_shared<LeftNode>(
        previousNode,
        threeDigitsRatio
    );

    const auto digitFourNode = std::make_shared<RightNode>(
        leftDigitsNode,
        digitRatio
    );

    const auto obtainedNode = std::make_shared<DownNode>(
        previousObtainedNode,
        previousObtainedNodeChildRatio
    );

    const auto obtainedDigits = std::make_shared<RightNode>(
        obtainedNode,
        threeDigitsRatio
    );

    const auto unitDigit = std::make_shared<RightNode>(
        obtainedDigits,
        digitRatio
    );

    const auto tensDigit = std::make_shared<CenteredNode>(
        obtainedDigits,
        digitRatio
    );

    const auto plusAndSpaceNode = std::make_shared<LeftNode>(
        obtainedNode,
        threeDigitsRatio
    );

    const auto plusNode = std::make_shared<LeftNode>(
        plusAndSpaceNode,
        digitRatio
    );

    const auto lineAndSumNode = std::make_shared<DownNode>(
        scoreNode,
        scoreNodeChildRatio
    );

    const auto lineNodeBase = std::make_shared<UpNode>(
        lineAndSumNode,
        previousObtainedNodeChildRatio
    );

    const auto lineNode = std::make_shared<CenteredNode>(
        lineNodeBase,
        previousObtainedNodeChildRatio * 4.f
    );

    const auto sumNode = std::make_shared<DownNode>(
        lineAndSumNode,
        scoreNodeChildRatio
    );

    const auto sumThreeDigitNode = std::make_shared<RightNode>(
        sumNode,
        threeDigitsRatio
    );
    const auto sumDigitOneNode = std::make_shared<RightNode>(
        sumThreeDigitNode,
        digitRatio
    );
    const auto sumDigitTwoNode = std::make_shared<CenteredNode>(
        sumThreeDigitNode,
        digitRatio
    );
    const auto sumDigitThreeNode = std::make_shared<RightNode>(
        sumThreeDigitNode,
        digitRatio
    );

    const auto sumLeftDigitsNode = std::make_shared<LeftNode>(
        sumNode,
        threeDigitsRatio
    );

    const auto sumDigitFourNode = std::make_shared<RightNode>(
        sumLeftDigitsNode,
        digitRatio
    );

    const auto coinNode = std::make_shared<LeftNode>(
        sumLeftDigitsNode,
        digitRatio
    );

    return {
        goodGameTitle,
        continueNode,
        exitNode,
        digitOneNode,
        digitTwoNode,
        digitThreeNode,
        digitFourNode,
        unitDigit,
        tensDigit,
        sumDigitOneNode,
        sumDigitTwoNode,
        sumDigitThreeNode,
        sumDigitFourNode,
        mainTitleNode,
        lineNode
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
    _sumDigitOne = nodes.at(9);
    _sumDigitTwo = nodes.at(10);
    _sumDigitThree = nodes.at(11);
    _sumDigitFour = nodes.at(12);
    _backgroundLabel = createBackgroundLabel(nodes.at(13));
    _operationLine = createOperationLine(nodes.at(14));
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

    short nodeCount = 100;
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

std::vector<std::string> SuccessPage::shaderDefines() const {
    return {"TRANSPARENT_BACKGROUND"};
}

std::string SuccessPage::getVertexShaderName() const {
    return "titlePageVs.vs";
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
    return {_backgroundLabel, _operationLine};
}

int SuccessPage::operationLineLabelId = 3;

Label_sptr SuccessPage::createOperationLine(Node_sptr node) {
    return std::make_shared<Label>(
        std::move(node),
        JBTypes::Color::White,
        SuccessPage::operationLineLabelId
    );
}
