/*
 * File: Menu.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#include "Menu.h"

#include "gameMenu/pages/CreditsPage.h"
#include "gameMenu/pages/FailurePage.h"
#include "gameMenu/pages/LevelsPage.h"
#include "gameMenu/pages/PausePage.h"
#include "gameMenu/pages/StorePage.h"
#include "gameMenu/pages/SuccessPage.h"
#include "gameMenu/pages/TitlePage.h"
#include "gameMenu/pages/ValidationPage.h"
#include "pages/tutorials/FacesTutorial.h"
#include "pages/tutorials/JumpTutorial.h"
#include "pages/tutorials/KeysTutorial.h"
#include "pages/tutorials/LookDownTutorial.h"
#include "pages/tutorials/MovementTutorial.h"
#include "pages/tutorials/Tutorial.h"

Menu::Menu(Player_sptr player,
           Page_sptr successPage,
           Page_sptr failurePage,
           InGamePage_sptr inGamePage,
           vecPage_sptr pages,
           bool isUsingTouchScreen)
    : _isUsingTouchScreen(isUsingTouchScreen),
      _player(std::move(player)),
      _pages(std::move(pages)),
      _successPage(std::move(successPage)),
      _failurePage(std::move(failurePage)),
      _inGamePage(std::move(inGamePage)),
      _currentPage(_pages.at(0)) {}

Page_sptr Menu::currentPage() {
    return _currentPage;
}

void Menu::update(const Mouse& mouse) {
    if (_player->isAWinner()) {
        _currentPage = _successPage;
        _player->resetGameStatus();
    } else if (_player->isALoser()) {
        _currentPage = _failurePage;
        _player->resetGameStatus();
    }
    _currentPage->update(mouse);
}

void Menu::mouseClick(float mouseX, float mouseY) {
    const auto matchedPage = _currentPage->click(
        // Positions have to be centered
        mouseX - 0.5f, mouseY - 0.5f);
    if (matchedPage != nullptr) {
        _currentPage = matchedPage;
    }
}

std::shared_ptr<Menu> Menu::getJumperBallMenu(const Player_sptr& player,
                                              CstItemsContainer_sptr itemsContainer,
                                              const CstMovableObject_sptr& movableObject,
                                              const CstAboveMovingCamera_sptr& camera,
                                              float ratio,
                                              bool isUsingTouchScreen) {
    const auto titlePage = TitlePage::createInstance(player, ratio);
    const auto levelsPage = LevelsPage::createInstance(player, titlePage, ratio);
    const auto pausePage = PausePage::createInstance(player, titlePage, ratio);
    const auto inGamePage = InGamePage::createInstance(
        player, pausePage, ratio, std::move(itemsContainer),
        createTutorial(player->getCurrentLevel(), movableObject, camera, player->isUsingEnglishLanguage(),
                       isUsingTouchScreen, player->isLeftRightReversed()));
    const auto successPage = SuccessPage::createInstance(player, titlePage, ratio);
    const auto failurePage = FailurePage::createInstance(player, titlePage, ratio);
    const auto creditsPage = CreditsPage::createInstance(player, titlePage, ratio);
    const auto storePage = StorePage::createInstance(player, titlePage, ratio);

    std::array<Page_sptr, StorePage::numberOfSkins> validationPages;
    for (size_t i = 0; i < StorePage::numberOfSkins; ++i) {
        validationPages[i] = ValidationPage::createInstance(i, player, storePage, ratio);
    }

    titlePage->setLevelsPage(levelsPage);
    titlePage->setCreditsPage(creditsPage);
    titlePage->setStorePage(storePage);
    levelsPage->setInGamePage(inGamePage);
    successPage->setInGamePage(inGamePage);
    failurePage->setInGamePage(inGamePage);
    pausePage->setInGamePage(inGamePage);
    storePage->setValidationPages(validationPages);

    vecPage_sptr pages{titlePage,   levelsPage,  pausePage,   inGamePage,
                       successPage, failurePage, creditsPage, storePage};
    pages.insert(pages.end(), validationPages.begin(), validationPages.end());

    return std::make_shared<Menu>(player, successPage, failurePage, inGamePage, pages, isUsingTouchScreen);
}

bool Menu::escapeAction() {
    _player->escapeAction();
    const auto& parent = _currentPage->parent().lock();
    if (parent == nullptr) {
        return true;
    }
    _currentPage = parent;
    return false;
}

void Menu::resize(float ratio) {
    for (const auto& page : _pages) {
        page->resize(ratio);
    }
}

Tutorial_uptr Menu::createTutorial(size_t level,
                                   const CstMovableObject_sptr& movableObject,
                                   const CstAboveMovingCamera_sptr& camera,
                                   bool isUsingEnglish,
                                   bool isUsingTouchScreen,
                                   bool isLeftRightReverted) {
    std::map<size_t, std::function<Tutorial_uptr()>> tutorialFactory{
        {1,
         [movableObject, isUsingEnglish, isUsingTouchScreen, isLeftRightReverted]() {
             return std::unique_ptr<Tutorial>(new MovementTutorial(movableObject, isUsingEnglish,
                                                                   isUsingTouchScreen, isLeftRightReverted));
         }},
        {2,
         [movableObject, isUsingEnglish]() {
             return std::unique_ptr<Tutorial>(new KeysTutorial(movableObject, isUsingEnglish));
         }},
        {3,
         [movableObject, isUsingEnglish, isUsingTouchScreen]() {
             return std::unique_ptr<Tutorial>(
                 new JumpTutorial(movableObject, isUsingEnglish, isUsingTouchScreen));
         }},
        {5,
         [movableObject, isUsingEnglish]() {
             return std::unique_ptr<Tutorial>(new FacesTutorial(movableObject, isUsingEnglish));
         }},
        {8, [movableObject, isUsingEnglish, isUsingTouchScreen, camera]() {
             return std::unique_ptr<Tutorial>(
                 new LookDownTutorial(movableObject, camera, isUsingEnglish, isUsingTouchScreen));
         }}};
    const auto tutorialIterator = tutorialFactory.find(level);
    if (tutorialIterator == tutorialFactory.cend()) {
        return nullptr;
    }
    return tutorialIterator->second();
}

void Menu::setBackgroundMap(CstItemsContainer_sptr itemsContainer,
                            const CstMovableObject_sptr& movableObject,
                            const CstAboveMovingCamera_sptr& camera) {
    _inGamePage->setItemsContainer(std::move(itemsContainer));
    _inGamePage->setTutorial(createTutorial(_player->getCurrentLevel(), movableObject, camera,
                                            _player->isUsingEnglishLanguage(), _isUsingTouchScreen,
                                            _player->isLeftRightReversed()));
}
