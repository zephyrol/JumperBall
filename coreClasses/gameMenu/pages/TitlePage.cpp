//
// Created by seb on 15/08/22.
//

#include "TitlePage.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/DownNode.h"

TitlePage::TitlePage(
    Player_sptr &&player,
    Node_sptr &&jumperBallTitle,
    Node_sptr &&play,
    Node_sptr &&store,
    Node_sptr &&exitNode,
    Node_sptr &&author
) : Page(
    std::move(player),
    nullptr
),
    _jumperBallTitle(std::move(jumperBallTitle)),
    _play(std::move(play)),
    _store(std::move(store)),
    _exitNode(std::move(exitNode)),
    _author(std::move(author)),
    _levelsPage() {
}

void TitlePage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _jumperBallTitle = nodes.at(0);
    _play = nodes.at(1);
    _store = nodes.at(2);
    _exitNode = nodes.at(3);
    _author = nodes.at(4);
}

TitlePage_sptr TitlePage::createInstance(Player_sptr player, float ratio) {
    auto nodes = createNodes(ratio);
    return std::make_shared<TitlePage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        std::move(nodes.at(3)),
        std::move(nodes.at(4))
    );
}

vecNode_sptr TitlePage::createNodes(float ratio) {
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

    const auto authorNode = std::make_shared<DownNode>(
        mainTitleNode,
        10.f
    );

    const auto optionsParentNode = std::make_shared<CenteredNode>(
        mainTitleNode,
        1.5f
    );
    constexpr float optionsNodeRatio = 7.f;
    const auto playNode = std::make_shared<UpNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    const auto storeNode = std::make_shared<CenteredNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    const auto exitNode = std::make_shared<DownNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    return {jumperBallTitle, playNode, storeNode, exitNode, authorNode};
}

Page::NodeMessageAssociations TitlePage::nodeToMessage() const {
    return {
        {_jumperBallTitle, "Jumper Ball"},
        {_play,            "Play"},
        {_store,           "Store"},
        {_exitNode,        "Exit"},
        {_author,          "Created by S.Morgenthaler"}
    };
}

Page_sptr TitlePage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_play)) {
        return _levelsPage;
    }
    if (intersectTest(_exitNode)) {
        _player->requestQuit();
    }
    return nullptr;
}

void TitlePage::setLevelsPage(Page_sptr levelsPage) {
    _levelsPage = std::move(levelsPage);
}

