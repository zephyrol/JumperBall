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
#include "gameMenu/nodes/TextNode.h"
#include <list>

class Page;

using Page_sptr = std::shared_ptr<Page>;
using Page_wptr = std::weak_ptr<Page>;
using CstPage_sptr = std::shared_ptr<const Page>;
using vecCstPage_sptr = std::vector<CstPage_sptr>;
using vecPage_sptr = std::vector<Page_sptr>;

class Page {
public:


    // Slide state => timepoint and yScreenPosition
    using slideState = std::pair<JBTypes::timePointMs, float>;

    explicit Page(Player_sptr &&player);

    virtual ~Page() = default;

    virtual Page_wptr parent();

    /**
     * Click on the page. Check if the mouse is on a node and activate the node action.
     * Return a target page linked to this node if it exists else nullptr
     * @param mouseX Screen mouse x coordinate
     * @param mouseY Screen mouse y coordinate
     * @return Target page or nullptr
     */
    virtual Page_sptr click(float mouseX, float mouseY) = 0;

    virtual float localPosY() const;

    virtual vecCstLabel_sptr labels() const;

    virtual vecCstTextNode_uptr genTextNodes() const = 0;

    const Player_sptr& player() const;

    virtual void update(bool isPressed, float screenPosY);

    virtual void resize(float ratio) = 0;

protected:
    const Player_sptr _player;
};

#endif // PAGE_H
