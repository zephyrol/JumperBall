/*
 * File: Menu.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#ifndef MENU_H
#define MENU_H

#include "gameMenu/pages/Page.h"
#include "gameMenu/labels/TextLabel.h"
#include "gameMenu/labels/ArrowLabel.h"
#include <player/Player.h>

class Menu {
public:

    explicit Menu(
        Player_sptr player,
        Page_sptr successPage,
        Page_sptr failurePage,
        vecPage_sptr pages
    );

    Page_sptr currentPage();

    void update(bool isPressed, float screenPosY);

    /**
     * Action escape operation.
     * @return true if the game is the game is ended.
     */
    bool escapeAction();

    void mouseClick(float mouseX, float mouseY);

    static std::shared_ptr<Menu> getJumperBallMenu(const Player_sptr &player, float ratio);

    void resize(float screenRatio);

private:
    const Player_sptr _player;
    const vecPage_sptr _pages;
    const Page_sptr _successPage;
    const Page_sptr _failurePage;
    Page_sptr _currentPage;

};

#endif /* MENU_H */
