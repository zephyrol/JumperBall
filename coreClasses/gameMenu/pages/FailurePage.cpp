//
// Created by S.Morgenthaler on 21/09/22.
//

#include "FailurePage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"

FailurePage::FailurePage(
    Player_sptr &&player,
    Node_sptr &&failureNode,
    Node_sptr &&retryNode,
    Node_sptr &&exitNode,
    Label_sptr &&backgroundLabel,
    const Page_sptr &parent
) : Page(std::move(player)),
    _parent(parent),
    _failureNode(std::move(failureNode)),
    _retryNode(std::move(retryNode)),
    _exitNode(std::move(exitNode)),
    _backgroundLabel(std::move(backgroundLabel)),
    _inGamePage(nullptr) {
}

FailurePage_sptr FailurePage::createInstance(
    Player_sptr player,
    const Page_sptr &parent,
    float ratio
) {
    auto nodes = createNodes(ratio);
    return std::make_shared<FailurePage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        createBackgroundLabel(nodes.at(3)),
        parent
    );
}

vecNode_sptr FailurePage::createNodes(float ratio) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainTitleNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    const auto youLostTitle = std::make_shared<UpNode>(
        mainTitleNode,
        4.f
    );

    const auto optionsParentNode = std::make_shared<CenteredNode>(
        mainTitleNode,
        1.5f
    );
    constexpr float optionsNodeRatio = 7.f;

    const auto continueNode = std::make_shared<UpNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    const auto exitNode = std::make_shared<DownNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    return {youLostTitle, continueNode, exitNode, optionsParentNode};

}

void FailurePage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _failureNode = nodes.at(0);
    _retryNode = nodes.at(1);
    _exitNode = nodes.at(2);
    _backgroundLabel = createBackgroundLabel(nodes.at(3));
}

Page_wptr FailurePage::parent() {
    return _parent;
}

void FailurePage::setInGamePage(Page_sptr inGamePage) {
    _inGamePage = std::move(inGamePage);
}

Page_sptr FailurePage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_exitNode)) {
        _player->addValidationSound();
        return _parent.lock();
    }
    if (intersectTest(_retryNode)) {
        _player->setCurrentLevel(_player->getCurrentLevel());
        return _inGamePage;
    }
    return nullptr;
}

vecCstTextNode_uptr FailurePage::genTextNodes() const {
    vecCstTextNode_uptr textNodes;
    const auto english = _player->isUsingEnglishLanguage();
    textNodes.emplace_back(new TextNode(_failureNode, english ? "You lost!" : "Perdu!", 0));
    textNodes.emplace_back(new TextNode(_retryNode, english ? "Retry" : "R;essayer", retryLabelId));
    textNodes.emplace_back(new TextNode(_exitNode, english ? "Exit" : "Sortir", exitLabelId));
    return textNodes;
}

std::string FailurePage::getVertexShaderName() const {
    return "titlePageVs.vs";
}

std::vector<std::string> FailurePage::shaderDefines() const {
    return {"TRANSPARENT_BACKGROUND"};
}

void FailurePage::update(const Mouse &mouse) {

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
    if (intersectTest(_retryNode)) {
        _currentSelectedLabel = retryLabelId;
    } else if (intersectTest(_exitNode)) {
        _currentSelectedLabel = exitLabelId;
    } else {
        _currentSelectedLabel = 0;
    }
}

vecCstLabel_sptr FailurePage::labels() const {
    return {_backgroundLabel};
}
