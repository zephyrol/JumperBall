/*
 * File: Page.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

#ifndef PAGE_H
#define PAGE_H

#include "gameMenu/labels/Label.h"
#include "player/Player.h"
#include <list>

class Page;

using Page_sptr = std::shared_ptr<Page>;
using Page_wptr= std::weak_ptr<Page>;
using CstPage_sptr = std::shared_ptr<const Page>;
using vecCstPage_sptr = std::vector<CstPage_sptr>;
using vecPage_sptr = std::vector<Page_sptr>;

class Page : public SceneElement {
public:


    // Slide state => timepoint and yScreenPosition
    using slideState = std::pair<JBTypes::timePointMs, float>;

    explicit Page(
        Player_sptr&& player,
        const Page_sptr &parent,
        float height = 1.f
    );

    std::weak_ptr<const Page> parent() const;

    std::weak_ptr<Page> parent();

    /**
     * Click on the page. Check if the mouse is on a node and activate the node action.
     * Return a target page linked to this node if it exists else nullptr
     * @param mouseX Screen mouse x coordinate
     * @param mouseY Screen mouse y coordinate
     * @return Target page or nullptr
     */
    virtual Page_sptr click(float mouseX, float mouseY) = 0;

    float height() const;

    float localPosY() const;

    SceneElement::GlobalState getGlobalState() const override;


    virtual vecCstLabel_sptr labels() const;

    using NodeMessageAssociations = std::unordered_map<Node_sptr, std::string>;
    virtual NodeMessageAssociations nodeToMessage() const = 0;

    void update(bool isPressed, float screenPosY = 0.f);

    virtual void resize(float ratio) = 0;


private:

    constexpr static float decelerationCoefficient = 10.f; // pagePourcentage /s^2


protected:
    const Player_sptr _player;

private:

    const std::weak_ptr<Page> _parent;
    const float _height;
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
