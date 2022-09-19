//
// Created by seb on 11/09/22.
//

#include "LevelsPage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/VerticalNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/RightNode.h"

LevelsPage::LevelsPage(
    Player_sptr &&player,
    Node_sptr &&levelsTitle,
    std::vector<Node_sptr> &&levels,
    std::shared_ptr<ArrowLabel>&& arrowLabel,
    const Page_sptr &parent
    ) : Page(
    std::move(player),
    parent
),
    _levelsTitle(std::move(levelsTitle)),
    _levels(std::move(levels)),
    _arrowLabel(std::move(arrowLabel)){
}

LevelsPage_sptr LevelsPage::createInstance(
    Player_sptr player,
    const Page_sptr &parent,
    float ratio
) {
    const auto levelsPageNode = getCommonNode(ratio);
    auto levelsNode = createLevelsNodes(ratio, levelsPageNode);
    const auto headerNode = createHeaderNode(levelsPageNode);
    auto levelsTitleNode = createLevelsTitleNode(headerNode);
    auto arrowLabel = createArrowLabel(headerNode);

    return std::make_shared<LevelsPage>(
        std::move(player),
        std::move(levelsTitleNode),
        std::move(levelsNode),
        std::move(arrowLabel),
        parent
    );
}

Node_sptr LevelsPage::createHeaderNode(const Node_sptr &commonNode) {
    return std::make_shared<UpNode>(commonNode, 4.f);
}

vecNode_sptr LevelsPage::createLevelsNodes(float ratio, const Node_sptr &commonNode) {
    vecNode_sptr levelsNodes;
    const auto levelsMainNode = std::make_shared<DownNode>(commonNode, 9.f / 18.f);
    const auto numberOfLevelsFloat = static_cast<float>(LevelsPage::numberOfLevels);
    for (size_t i = 0; i < LevelsPage::numberOfLevels; i += 3) {
        constexpr auto positionFactor = 4.f;
        const auto positionY = static_cast<float>(i) / numberOfLevelsFloat;
        const auto verticalNode = std::make_shared<VerticalNode>(
            levelsMainNode,
            5.f,
            1.f - positionY * positionFactor
        );

        auto leftLevel = LevelsPage::createLevelNode<LeftNode>(verticalNode);
        levelsNodes.push_back(std::move(leftLevel));

        auto middleLevel = LevelsPage::createLevelNode<CenteredNode>(verticalNode);
        levelsNodes.push_back(std::move(middleLevel));

        auto rightLevel = LevelsPage::createLevelNode<RightNode>(verticalNode);
        levelsNodes.push_back(std::move(rightLevel));
    }

    return levelsNodes;
}

void LevelsPage::resize(float ratio) {
    const auto levelsPageNode = getCommonNode(ratio);
    auto levelsNode = createLevelsNodes(ratio, levelsPageNode);
    const auto headerNode = createHeaderNode(levelsPageNode);
    auto levelsTitleNode = createLevelsTitleNode(headerNode);
    auto arrowLabel = createArrowLabel(headerNode);
    _levelsTitle = std::move(levelsTitleNode);
    _levels = std::move(levelsNode);
    _arrowLabel = std::move(arrowLabel);
}

Node_sptr LevelsPage::getCommonNode(float ratio) {
    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto levelsPageNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 22.f
    );
    return levelsPageNode;
}


Page::NodeMessageAssociations LevelsPage::nodeToMessage() const {
    Page::NodeMessageAssociations nodeMessageAssociations;

    for (size_t i = 0; i < LevelsPage::numberOfLevels; ++i) {
        const auto levelNumber = i + 1;
        nodeMessageAssociations[_levels[i]] = (levelNumber < 10 ? "0" : "") + std::to_string(levelNumber);
    }

    nodeMessageAssociations[_levelsTitle] = "Levels";
    return nodeMessageAssociations;
}

Page_sptr LevelsPage::click(float mouseX, float mouseY) {
    return nullptr;
}

vecCstLabel_sptr LevelsPage::labels() const {
    return {_arrowLabel};
}

Node_sptr LevelsPage::createLevelsTitleNode(const Node_sptr &headerNode) {
    return std::make_shared<RightNode>(headerNode, 2.5f);
}

std::shared_ptr<ArrowLabel> LevelsPage::createArrowLabel(const Node_sptr &headerNode) {
    const auto leftNode = std::make_shared<LeftNode>(headerNode, 1.f);
    auto arrowLabel = std::make_shared<ArrowLabel>(leftNode);
    return arrowLabel;
}
