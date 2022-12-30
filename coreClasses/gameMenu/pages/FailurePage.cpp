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
    const Page_sptr &parent
) : Page(std::move(player)),
    _parent(parent),
    _failureNode(std::move(failureNode)),
    _retryNode(std::move(retryNode)),
    _exitNode(std::move(exitNode)),
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

    const auto goodGameTitle = std::make_shared<UpNode>(
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

    return {goodGameTitle, continueNode, exitNode};

}

void FailurePage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _failureNode = nodes.at(0);
    _retryNode = nodes.at(1);
    _exitNode = nodes.at(2);
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
    textNodes.emplace_back(new TextNode(_failureNode, "You lost!", 0));
    textNodes.emplace_back(new TextNode(_retryNode, "Retry", 0));
    textNodes.emplace_back(new TextNode(_exitNode, "Exit", 0));
    return textNodes;
}

std::string FailurePage::getVertexShaderName() const {
    return "failurePageVs.vs";
}
