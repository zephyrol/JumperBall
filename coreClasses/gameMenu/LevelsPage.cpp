//
// Created by seb on 11/09/22.
//

#include "LevelsPage.h"
#include "ScreenNode.h"
#include "CenteredNode.h"
#include "ScaledNode.h"
#include "UpNode.h"

Page_sptr LevelsPage::createInstance(
    const Page_sptr &parent,
    float ratio
) {
    auto levelsNode = createLevelsNodes(ratio);
    auto levelsTitleNode = createLevelsTitleNode(ratio);
    return std::make_shared<LevelsPage>(
        std::move(levelsTitleNode),
        std::move(levelsNode),
        parent
    );
}


LevelsPage::LevelsPage(
    Node_sptr &&levelsTitle,
    std::vector<Node_sptr> &&levels,
    const Page_sptr &parent
) : Page(
    parent
),
    _levelsTitle(std::move(levelsTitle)),
    _levels(std::move(levels)) {
}

Node_sptr LevelsPage::createLevelsTitleNode(float ratio) {
    const auto levelsPageNode = getCommonNode(ratio);
    const auto levelsTitle = std::make_shared<UpNode>(
        levelsPageNode,
        4.f
    );
    return levelsTitle;
}

vecNode_sptr LevelsPage::createLevelsNodes(float ratio) {
    const auto levelsPageNode = getCommonNode(ratio);
    return {};
}

void LevelsPage::resize(float ratio) {
    auto levelsNode = createLevelsNodes(ratio);
    auto levelsTitleNode = createLevelsTitleNode(ratio);
    _levels = std::move(levelsNode);
    _levelsTitle = std::move(levelsTitleNode);
}

Node_sptr LevelsPage::getCommonNode(float ratio) {
    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto levelsPageNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );
}

const size_t LevelsPage::numberOfLevels = 100;

Page::NodeMessageAssociations LevelsPage::nodeToMessage() const {
    Page::NodeMessageAssociations nodeMessageAssociations;

    for(size_t i = 0; i < LevelsPage::numberOfLevels; ++i) {
        // nodeMessageAssociations[_levels[i]] = (i < 10 ? "0" : "") + std::to_string(i);
    }

    nodeMessageAssociations[_levelsTitle] = "Levels";
    return nodeMessageAssociations;
}
