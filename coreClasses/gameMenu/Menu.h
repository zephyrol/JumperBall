/*
 * File: Menu.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#ifndef MENU_H
#define MENU_H

#include <player/Player.h>
#include "gameMenu/labels/ArrowLabel.h"
#include "gameMenu/labels/TextLabel.h"
#include "gameMenu/pages/InGamePage.h"
#include "gameMenu/pages/Page.h"
#include "system/Mouse.h"

#include "pages/tutorials/Tutorial.h"
#include "scene/AboveMovingCamera.h"

class Menu {
   public:
    explicit Menu(Player_sptr player,
                  Page_sptr successPage,
                  Page_sptr failurePage,
                  InGamePage_sptr inGamePage,
                  vecPage_sptr pages,
                  bool isUsingTouchScreen);

    Page_sptr currentPage();

    void update(const Mouse& mouse);

    /**
     * Action escape operation.
     * @return true if the game is the game is ended.
     */
    bool escapeAction();

    void mouseClick(float mouseX, float mouseY);

    static std::shared_ptr<Menu> getJumperBallMenu(const Player_sptr& player,
                                                   CstItemsContainer_sptr itemsContainer,
                                                   const CstMovableObject_sptr& movableObject,
                                                   const CstAboveMovingCamera_sptr& camera,
                                                   float ratio,
                                                   bool isUsingTouchScreen);

    void setBackgroundMap(CstItemsContainer_sptr itemsContainer,
                          const CstMovableObject_sptr& movableObject,
                          const CstAboveMovingCamera_sptr& camera);
    void resize(float screenRatio);

   private:
    const bool _isUsingTouchScreen;
    const Player_sptr _player;
    const vecPage_sptr _pages;
    const Page_sptr _successPage;
    const Page_sptr _failurePage;
    const InGamePage_sptr _inGamePage;
    Page_sptr _currentPage;

    static Tutorial_uptr createTutorial(size_t level,
                                        const CstMovableObject_sptr& movableObject,
                                        const CstAboveMovingCamera_sptr& camera,
                                        bool isUsingEnglish,
                                        bool isUsingTouchScreen,
                                        bool isLeftRightReverted);
};

#endif /* MENU_H */
