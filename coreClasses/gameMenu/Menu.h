/*
 * File: Menu.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#ifndef MENU_H
#define MENU_H
#include <gameMenu/Page.h>
#include <gameMenu/MessageLabel.h>
#include <gameMenu/ItemLabel.h>
#include <gameMenu/ArrowLabel.h>
#include <gameMenu/BoxLabel.h>
#include <player/Player.h>

class Menu {
public:

enum class Action { None, QuitGame, GoBack, GoLevel };
struct MenuAnswer {
    Menu::Action action = Menu::Action::None;
    size_t newLevel = 0;
};

Menu(
    Player& player,
    const Page_sptr& rootPage,
    const Page_sptr& pausePage,
    const Page_sptr& successPage,
    const Page_sptr& failurePage,
    const vecCstPage_sptr& pages
    );

Page_sptr currentPage();
CstPage_sptr currentPage() const;
void currentPage(const Page_sptr& page);

void update(bool isPressed, float screenPosY);

void rootPageAsCurrentPage();
void pausePageAsCurrentPage();
void successPageAsCurrentPage();
void failurePageAsCurrentPage();
void noPageAsCurrentPage();
bool parentPageAsCurrentPage();
Menu::MenuAnswer escapeAction();

Menu::MenuAnswer mouseClick(float mouseX, float mouseY);

CstPage_sptr rootPage() const;
CstPage_sptr pausePage() const;
CstPage_sptr successPage() const;
CstPage_sptr failurePage() const;
const vecCstPage_sptr& pages() const;

static std::shared_ptr <Menu> getJumperBallMenu(Player& player,
                                                size_t currentLevel,
                                                unsigned int sizeX,
                                                unsigned int sizeY
                                                );

private:
Player& _player;
const Page_sptr _rootPage;
const Page_sptr _pausePage;
const Page_sptr _successPage;
const Page_sptr _failurePage;
const vecCstPage_sptr _pages;
Page_sptr _currentPage;

};

#endif /* MENU_H */
