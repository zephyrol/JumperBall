//
// Created by seb on 11/09/22.
//

#ifndef JUMPERBALLAPPLICATION_LEVELSPAGE_H
#define JUMPERBALLAPPLICATION_LEVELSPAGE_H

#include "Page.h"


class LevelsPage;
using LevelsPage_sptr = std::shared_ptr<LevelsPage>;
class LevelsPage : public Page {
public:
    static LevelsPage_sptr createInstance(
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );

    void resize(float ratio) override;

    Page_sptr click(float mouseX, float mouseY) override;

    LevelsPage(
        Player_sptr&& player,
        Node_sptr &&levelsTitle,
        std::vector<Node_sptr> &&levels,
        const Page_sptr &parent
    );

    NodeMessageAssociations nodeToMessage() const override;

private:

    Node_sptr _levelsTitle;
    std::vector<Node_sptr> _levels;

    static vecNode_sptr createLevelsNodes(float ratio);
    static Node_sptr createLevelsTitleNode(float ratio);
    static Node_sptr getCommonNode(float ratio);
    static const size_t numberOfLevels;
};


#endif //JUMPERBALLAPPLICATION_LEVELSPAGE_H
