/*
 * File: Page.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

#ifndef PAGE_H
#define PAGE_H

#include "Label.h"
#include <list>

class Page;

using Page_sptr = std::shared_ptr<Page>;
using CstPage_sptr = std::shared_ptr<const Page>;
using vecCstPage_sptr = std::vector<CstPage_sptr>;
using vecPage_sptr = std::vector<Page_sptr>;

class Page : public SceneElement {
public:

    using NodeMessageAssociations = std::unordered_map<Node_sptr, std::string>;

    // Slide state => timepoint and yScreenPosition
    using slideState = std::pair<JBTypes::timePointMs, float>;
    enum class EscapeAnswer {
        QuitGame, GoToParent, GoToPause
    };

    Page(
        const Page_sptr &parent,
        // TODO add no message labels ( shapes )
        NodeMessageAssociations &&nodeToMessage,
        const Page::EscapeAnswer &escapeAnswer,
        float height = 1.f,
        bool visibleOnParent = false
    );

    std::weak_ptr<const Page> parent() const;

    std::weak_ptr<Page> parent();

    bool visibleOnParent() const;

    Label_sptr matchedLabel(float x, float y);

    float height() const;

    float localPosY() const;

    void setBridges(std::unordered_map<Label_sptr, Page_sptr> &&bridges);

    const Page::EscapeAnswer &getEscapeAnswer() const;

    SceneElement::GlobalState getGlobalState() const override;

    Page_sptr child(const Label_sptr &label);

    const vecCstLabel_sptr &labels() const;

    const NodeMessageAssociations &nodeToMessage() const;

    void pressOnPage();
    void release();

    void update(bool isPressed, float screenPosY = 0.f);

    void resize(float ratio);

private:

    constexpr static float decelerationCoefficient = 10.f; // pagePourcentage /s^2

    vecLabel_sptr createLabels() const;

    vecCstLabel_sptr createCstLabels() const;

    std::vector<Page_sptr> createChildren() const;

    std::unordered_map<Label_sptr, Page_sptr> _bridges;

    const NodeMessageAssociations _nodeToMessage;
    const vecNode_sptr _nodes;

    vecLabel_sptr _labels;
    vecCstLabel_sptr _cstLabels;

    std::vector<Page_sptr> _children;
    const std::weak_ptr<Page> _parent;
    const bool _visibleOnParent;
    const float _height;
    const Page::EscapeAnswer _escapeAnswer;
    float _localPosY; // Page position
    float _localPressedPosY;
    float _localReleasedPosY;
    bool _isPressed;
    float _pressedScreenPosY;
    JBTypes::timePointMs _lastUpdate;
    std::list<slideState> _lastSwipeUpdates;
    float _releaseVelocity;
};

#endif // PAGE_H
