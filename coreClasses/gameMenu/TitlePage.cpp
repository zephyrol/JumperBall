//
// Created by seb on 15/08/22.
//

#include "TitlePage.h"
#include "CenteredNode.h"
#include "UpNode.h"

TitlePage::TitlePage(
    const Node_sptr &jumperBallTitle
):
Page(
    nullptr,
    Page::EscapeAnswer::QuitGame),
    _jumperBallTitle(jumperBallTitle)
{
}

void TitlePage::resize(float ratio) {
    _jumperBallTitle = createNodes(ratio).at(0);
}

Page_sptr TitlePage::createInstance(float ratio) {
    const auto nodes = createNodes(ratio);
    return std::make_shared<TitlePage>(nodes.at(0));
}

vecNode_sptr TitlePage::createNodes(float ratio) {
    Node_sptr mainTitleNode = std::make_shared<CenteredNode>(
        nullptr,
        9.f / 16.f
    );

    Node_sptr jumperBallTitle = std::make_shared<UpNode>(
        mainTitleNode,
        4.f
    );

    return {jumperBallTitle};
}

Page::NodeMessageAssociations TitlePage::nodeToMessage() const {
    return {
        {_jumperBallTitle, "Jumper Ball"}
    };
}

