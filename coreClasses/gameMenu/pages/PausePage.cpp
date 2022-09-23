//
// Created by S.Morgenthaler on 21/09/22.
//

#include "PausePage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"

PausePage::PausePage(
    Player_sptr &&player,
    Node_sptr &&jumperBallTitleNode,
    Node_sptr &&resumeNode,
    Node_sptr &&exitNode,
    const Page_sptr &parent
) : Page(std::move(player)),
    _parent(parent),
    _inGamePage(nullptr),
    _jumperBallTitleNode(std::move(jumperBallTitleNode)),
    _resumeNode(std::move(resumeNode)),
    _exitNode(std::move(exitNode)) {
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

    const auto jumperBallTitle = std::make_shared<UpNode>(
        mainTitleNode,
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

    return {jumperBallTitle, resumeNode, exitNode};

}

void PausePage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _jumperBallTitleNode = nodes.at(0);
    _resumeNode = nodes.at(1);
    _exitNode = nodes.at(2);
}

Page_wptr PausePage::parent() {
    return _parent;
}

Page_sptr PausePage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_exitNode)) {
        return _parent.lock();
    }
    if (intersectTest(_resumeNode)) {
        _player->status(Player::Status::InGame);
        return _inGamePage;
    }
    return nullptr;
}

vecCstTextNode_uptr PausePage::genTextNodes() const {
    vecCstTextNode_uptr textNodes;
    textNodes.emplace_back(new TextNode(_jumperBallTitleNode, "Jumper Ball"));
    textNodes.emplace_back(new TextNode(_resumeNode, "Resume"));
    textNodes.emplace_back(new TextNode(_exitNode, "Exit"));
    return textNodes;
}
