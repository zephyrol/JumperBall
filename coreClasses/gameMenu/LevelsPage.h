//
// Created by seb on 11/09/22.
//

#ifndef JUMPERBALLAPPLICATION_LEVELSPAGE_H
#define JUMPERBALLAPPLICATION_LEVELSPAGE_H

#include "Page.h"


class LevelsPage : public Page {
public:
    static Page_sptr createInstance(
        const Page_sptr &parent,
        float ratio
    );

    void resize(float ratio) override;

    LevelsPage(
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
