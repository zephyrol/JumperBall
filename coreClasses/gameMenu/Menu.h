/*
 * File: Menu.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#ifndef MENU_H
#define MENU_H

#include "gameMenu/pages/Page.h"
#include "gameMenu/labels/MessageLabel.h"
#include "gameMenu/labels/ArrowLabel.h"
#include <player/Player.h>

class Menu {
public:

    Menu(
        Player_sptr player,
        const Page_sptr &rootPage,
        Page_sptr pausePage,
        Page_sptr successPage,
        Page_sptr failurePage,
        vecPage_sptr pages
    );

    Page_sptr currentPage();

    CstPage_sptr currentPage() const;

    void currentPage(const Page_sptr &page);

    void update(bool isPressed, float screenPosY);

    void rootPageAsCurrentPage();

    void pausePageAsCurrentPage();

    void successPageAsCurrentPage();

    void failurePageAsCurrentPage();

    /**
     * Action escape operation.
     * @return true if the game is the game is ended.
     */
    bool escapeAction();

    void mouseClick(float mouseX, float mouseY);

    CstPage_sptr rootPage() const;

    CstPage_sptr pausePage() const;

    CstPage_sptr successPage() const;

    CstPage_sptr failurePage() const;

    static std::shared_ptr<Menu> getJumperBallMenu(const Player_sptr &player, float ratio);

    void resize(float screenRatio);

private:
    const Player_sptr _player;
    const Page_sptr _rootPage;
    const Page_sptr _pausePage;
    const Page_sptr _successPage;
    const Page_sptr _failurePage;
    const vecPage_sptr _pages;
    Page_sptr _currentPage;

};

#endif /* MENU_H */
