/*
 * File: Menu.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#include "Menu.h"

#include <utility>
#include "gameMenu/pages/TitlePage.h"
#include "gameMenu/pages/LevelsPage.h"
#include "gameMenu/pages/InGamePage.h"
#include "gameMenu/pages/SuccessPage.h"
#include "gameMenu/pages/FailurePage.h"
#include "gameMenu/pages/PausePage.h"

Menu::Menu(
    Player_sptr player,
    Page_sptr successPage,
    Page_sptr failurePage,
    vecPage_sptr pages
) :
    _player(std::move(player)),
    _pages(std::move(pages)),
    _successPage(std::move(successPage)),
    _failurePage(std::move(failurePage)),
    _currentPage(_pages.at(0)) {
}

Page_sptr Menu::currentPage() {
    return _currentPage;
}

void Menu::update(bool isPressed, float screenPosY) {
    if (_player->status() != Player::Status::InMenu) {
        return;
    }
    if (_player->isAWinner()) {
        _currentPage = _successPage;
        _player->resetGameStatus();
    } else if (_player->isALoser()) {
        _currentPage = _failurePage;
        _player->resetGameStatus();
    }
    _currentPage->update(isPressed, screenPosY);
}

void Menu::mouseClick(float mouseX, float mouseY) {
    const auto matchedPage = _currentPage->click(
        // Positions have to be centered
        mouseX - 0.5f,
        mouseY - 0.5f
    );
    if (matchedPage != nullptr) {
        _currentPage = matchedPage;
    }
}

std::shared_ptr<Menu> Menu::getJumperBallMenu(const Player_sptr &player, float ratio) {

    const auto titlePage = TitlePage::createInstance(player, ratio);
    const auto levelsPage = LevelsPage::createInstance(player, titlePage, ratio);
    const auto pausePage = PausePage::createInstance(player, titlePage, ratio);
    const auto inGamePage = std::make_shared<InGamePage>(player, pausePage);
    const auto successPage = SuccessPage::createInstance(player, titlePage, ratio);
    const auto failurePage = FailurePage::createInstance(player, titlePage, ratio);

    titlePage->setLevelsPage(levelsPage);
    levelsPage->setInGamePage(inGamePage);
    successPage->setInGamePage(inGamePage);
    failurePage->setInGamePage(inGamePage);
    pausePage->setInGamePage(inGamePage);


    const vecPage_sptr pages{titlePage, levelsPage, pausePage, inGamePage, successPage, failurePage};
    return std::make_shared<Menu>(
        player,
        successPage,
        failurePage,
        pages
    );
}

bool Menu::escapeAction() {
    _player->escapeAction();
    const auto &parent = _currentPage->parent().lock();
    if (parent == nullptr) {
        return true;
    }
    _currentPage = parent;
    return false;
}

void Menu::resize(float ratio) {
    for (const auto &page: _pages) {
        page->resize(ratio);
    }
}
