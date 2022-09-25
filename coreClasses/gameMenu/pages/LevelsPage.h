//
// Created by seb on 11/09/22.
//

#ifndef JUMPERBALLAPPLICATION_LEVELSPAGE_H
#define JUMPERBALLAPPLICATION_LEVELSPAGE_H

#include "Page.h"
#include "gameMenu/labels/ArrowLabel.h"


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
        Player_sptr &&player,
        Node_sptr &&levelsTitle,
        std::vector<Node_sptr> &&levels,
        std::shared_ptr<ArrowLabel> &&arrowLabel,
        const Page_sptr &parent
    );

    vecCstTextNode_uptr genTextNodes() const override;

    Page_wptr parent() override;

    vecCstLabel_sptr labels() const override;

    void setInGamePage(Page_sptr inGamePage);

private:

    const Page_wptr _parent;
    Node_sptr _levelsTitle;

    std::vector<Node_sptr> _levels;
    std::shared_ptr<ArrowLabel> _arrowLabel;
    Page_sptr _inGamePage;

    template<class T>
    static std::shared_ptr<T> createLevelNode(const Node_sptr &parent);

    static vecNode_sptr createLevelsNodes(const Node_sptr &commonNode);

    static Node_sptr createHeaderNode(const Node_sptr &commonNode);

    static Node_sptr createLevelsTitleNode(const Node_sptr &headerNode);

    static std::shared_ptr<ArrowLabel> createArrowLabel(const Node_sptr &headerNode);

    static Node_sptr getCommonNode(float ratio);

    static constexpr size_t numberOfLevels = 100;
};

template<class T>
std::shared_ptr<T> LevelsPage::createLevelNode(const Node_sptr &parent) {
    constexpr float levelNodeRatio = 1.f;
    return std::make_shared<T>(parent, levelNodeRatio);
}


#endif //JUMPERBALLAPPLICATION_LEVELSPAGE_H
