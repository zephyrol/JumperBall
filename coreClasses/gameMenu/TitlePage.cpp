//
// Created by seb on 15/08/22.
//

#include "TitlePage.h"
#include "CenteredNode.h"
#include "UpNode.h"
#include "ScreenNode.h"
#include "ScaledNode.h"
#include "DownNode.h"

TitlePage::TitlePage(
    Node_sptr &&jumperBallTitle,
    Node_sptr &&play,
    Node_sptr &&store,
    Node_sptr &&exit,
    Node_sptr &&author
) : Page(
    nullptr
),
    _jumperBallTitle(std::move(jumperBallTitle)),
    _play(std::move(play)),
    _store(std::move(store)),
    _exit(std::move(exit)),
    _author(std::move(author)){
}

void TitlePage::resize(float ratio) {
    const auto& nodes = createNodes(ratio);
    _jumperBallTitle = nodes.at(0);
    _play = nodes.at(1);
    _store = nodes.at(2);
    _exit = nodes.at(3);
    _author = nodes.at(4);
}

Page_sptr TitlePage::createInstance(float ratio) {
    auto nodes = createNodes(ratio);
    return std::make_shared<TitlePage>(
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
        {_play, "Play"},
        {_store, "Store"},
        {_exit, "Exit"},
        {_author, "Created by S.Morgenthaler"}
    };
}

