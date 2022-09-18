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
        vecPage_sptr pages
    );

    Page_sptr currentPage();

    CstPage_sptr currentPage() const;

    void currentPage(const Page_sptr &page);

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
    Page_sptr _currentPage;

};

#endif /* MENU_H */
