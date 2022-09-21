//
// Created by S.Morgenthaler on 20/09/2022.
//

#include "SuccessPage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"

SuccessPage::SuccessPage(
    Player_sptr &&player,
    Node_sptr &&goodGameNode,
    Node_sptr &&continueNode,
    Node_sptr &&exitNode,
    const Page_sptr &parent
) : Page(std::move(player)),
    _parent(parent),
    _goodGameNode(goodGameNode),
    _continueNode(continueNode),
    _exitNode(exitNode),
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

void SuccessPage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _goodGameNode = nodes.at(0);
    _continueNode = nodes.at(1);
    _exitNode = nodes.at(2);
}

Page_wptr SuccessPage::parent() {
    return _parent;
}

void SuccessPage::setInGamePage(Page_sptr inGamePage) {
    _inGamePage = std::move(inGamePage);
}

Page::NodeMessageAssociations SuccessPage::nodeToMessage() const {
    return {
        {_goodGameNode, "Good game!"},
        {_continueNode, "Next level"},
        {_exitNode,     "Exit"},
    };
}

Page_sptr SuccessPage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_exitNode)) {
        return _parent.lock();
    }
    if (intersectTest(_continueNode)) {
        _player->setCurrentLevel(_player->getCurrentLevel() + 1);
        return _inGamePage;
    }
    return nullptr;
}
