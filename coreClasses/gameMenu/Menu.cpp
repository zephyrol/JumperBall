/*
 * File: Menu.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#include "Menu.h"

#include <utility>
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/pages/TitlePage.h"

Menu::Menu(
    Player_sptr player,
    const Page_sptr &rootPage,
    Page_sptr pausePage,
    Page_sptr successPage,
    Page_sptr failurePage,
    vecPage_sptr pages
    ) :
    _player(std::move(player)),
    _rootPage(rootPage),
    _pausePage(std::move(pausePage)),
    _successPage(std::move(successPage)),
    _failurePage(std::move(failurePage)),
    _pages(std::move(pages)),
    _currentPage(rootPage) {
}

CstPage_sptr Menu::currentPage() const {
    return _currentPage;
}

Page_sptr Menu::currentPage() {
    return _currentPage;
}

void Menu::currentPage(const Page_sptr &page) {
    _currentPage = page;
}

void Menu::update(bool isPressed, float screenPosY) {
    if(_player->status() != Player::Status::InMenu) {
        return;
    }
    _currentPage->update(isPressed, screenPosY);
}

void Menu::rootPageAsCurrentPage() {
    _currentPage = _rootPage;
}

void Menu::pausePageAsCurrentPage() {
    _currentPage = _pausePage;
}

void Menu::successPageAsCurrentPage() {
    _currentPage = _successPage;
}

void Menu::failurePageAsCurrentPage() {
    _currentPage = _failurePage;
}

std::unique_ptr<size_t> Menu::mouseClick(float mouseX, float mouseY) {
    const auto matchedPage = _currentPage->click(
        // Positions have to be centered
        mouseX - 0.5f,
        mouseY - 0.5f
        );
    if(matchedPage != nullptr) {
        _currentPage = matchedPage;
    }

    return nullptr;
    /* TODO
    const auto& label = _currentPage->matchNode(mouseX, mouseY);
    if (label) {
        const Page_sptr newPage = _currentPage->child(label);
        if (newPage) {
            _currentPage = newPage;
        } else if (
            const std::shared_ptr <const Label::LabelAnswer>& action =
                label->action()
            ) {
            if (action->typeOfAction == Label::TypeOfAction::GoLevel) {
                noPageAsCurrentPage();
                _currentPage = nullptr;
                menuAnswer.action = Menu::Action::GoLevel;
                menuAnswer.newLevel = action->chooseLevel + 1;
                _player.status(Player::Status::InTransition);
            } else if (
                action->typeOfAction == Label::TypeOfAction::PredefinedAction) {
                if (
                    action->predefinedAction == Label::PredefinedAction::GoBack) {
                    parentPageAsCurrentPage();
                    menuAnswer.action = Menu::Action::GoBack;
                }
            }
        }
    }
     */
}

CstPage_sptr Menu::rootPage() const {
    return _rootPage;
}

CstPage_sptr Menu::pausePage() const {
    return _pausePage;
}

CstPage_sptr Menu::successPage() const {
    return _successPage;
}

CstPage_sptr Menu::failurePage() const {
    return _failurePage;
}

std::shared_ptr<Menu> Menu::getJumperBallMenu(const Player_sptr &player, float ratio) {

    const Page_sptr page1 = TitlePage::createInstance(player, ratio);

    const vecPage_sptr pages{page1};
    return std::make_shared<Menu>(
        player,
        page1,
        nullptr,
        nullptr,
        nullptr,
        pages
    );
}

bool Menu::escapeAction() {
    if(_player->status() != Player::Status::InMenu) {
        return false;
    }
    const auto& parent = _currentPage->parent().lock();
    if(parent == nullptr) {
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
