/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Menu.cpp
 * Author: Morgenthaler S
 * 
 * Created on 10 mai 2020, 15:47
 */

#include "Menu.h"

Menu::Menu(
           Player& player,
           const Page_sptr &rootPage,
           const Page_sptr &pausePage,
           const Page_sptr &successPage,
           const Page_sptr &failurePage,
           const vecCstPage_sptr &pages):
    _player(player),
    _rootPage(rootPage),
    _pausePage(pausePage),
    _successPage(successPage),
    _failurePage(failurePage),
    _pages(pages),
    _currentPage(rootPage)
{
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
    if (_currentPage) {
        _currentPage->update(isPressed, screenPosY);
    }
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

void Menu::noPageAsCurrentPage() {
    _currentPage = nullptr; 
}

Menu::Event Menu::mouseClick(float mouseX, float mouseY) {
    Menu::Event event;
    if (_currentPage) {
        const CstLabel_sptr label =
            _currentPage->matchedLabel(mouseX, mouseY);
        if (label) {
            const Page_sptr newPage = _currentPage->child(label);
            if (newPage) {
                _currentPage = newPage;
            }
            else if (const std::shared_ptr<const Label::LabelAnswer> &action =
                label->action()) {
                _player.treatAction(*action);
                //New level case
                if (_player.statut() == Player::Statut::INTRANSITION) {
                    noPageAsCurrentPage();
                    _currentPage = nullptr;
                    event.newLevel = action->chooseLevel + 1;
                }
            }
        }
    }
    return event;
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

const vecCstPage_sptr &Menu::pages() const {
    return _pages;
}

std::shared_ptr<Menu> Menu::getJumperBallMenu(
    Player& player,
    size_t currentLevel,
    unsigned int sizeX,
    unsigned int sizeY)
{


    const bool isSmartPhoneFormat = sizeX < sizeY;
    const float ratioX = static_cast<float>(sizeX) / static_cast<float>(sizeY);
    const float ratioY = 1.f / ratioX;
    const float ratio = ratioX < ratioY
      ? ratioX
      : ratioY;
    std::cout <<(ratio) << std::endl;
    //Page 1
    const float factor = isSmartPhoneFormat
      ? 1.f
      : ratioY;

    const float label1Page1Width = factor * 0.9f;
    const float label2Page1Width = factor * 0.4f;
    const float label3Page1Width = factor * 0.6f;
    const float label4Page1Width = factor * 0.4f;

    const float label1Page1HeightDivide = 9.f;
    const float label234Page1HeightDivide = 8.f;

    const float label1Page1Height =
      label1Page1Width * ratioX / label1Page1HeightDivide;
    const float label234Page1Height =
      label2Page1Width * ratioX / label234Page1HeightDivide;
    std::shared_ptr<const MessageLabel> label1Page1 =
        std::make_shared<const MessageLabel>(
            Label::WidthUnit::ShortestSide,
            label1Page1Width, label1Page1Height,
            JBTypes::vec2f{0.5f,0.8f},
            "Jumper Ball");
    std::shared_ptr<const MessageLabel> label2Page1 =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label2Page1Width, label234Page1Height,
            JBTypes::vec2f{0.5f,0.6f},
            "Play");
    std::shared_ptr<const MessageLabel> label3Page1 =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label3Page1Width, label234Page1Height,
            JBTypes::vec2f{0.5f,0.4f},
            "Store");
    std::shared_ptr<const MessageLabel> label4Page1 =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label4Page1Width, label234Page1Height,
            JBTypes::vec2f{0.5f,0.2f},
            "Exit");

    const vecCstLabel_sptr labelsPage1 {
        label1Page1, label2Page1, label3Page1, label4Page1
    };

    //Page 2
    vecCstLabel_sptr labelsPage2;

    const float label1Page2Width = factor * 0.7f;
    std::shared_ptr<const MessageLabel> labelLevelsTitle =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label1Page2Width, label1Page1Height, 
            JBTypes::vec2f{0.5f, 1.f - 0.1f}, "Levels"
        );
    labelsPage2.push_back(labelLevelsTitle);

    //constexpr float offsetBox = 0.02f;

   vecLabel_sptr labelLevels;
    for (size_t i = 0; i < 99; ++i) {


      const float labelNumberWidth = factor * 0.1f;

      const float labelNumberHeight =
          labelNumberWidth * ratioX;

      std::string sNumber;
      if (i < 10)
      {
        sNumber.append("0");
        }
        sNumber.append(std::to_string(i+1));


        /*std::shared_ptr<const BoxLabel> boxLabelLevels =
        std::make_shared<const BoxLabel>
        (   Utility::xScreenToPortrait(.2f), 0.1f,
         JBTypes::vec2f{.5f - Utility::xScreenToPortrait(.5f)
            + Utility::xScreenToPortrait(.1f + (i%3) * .4f), 1.f-(0.3f + i/3 * 0.3f)-offsetBox});
        labelsPage2.push_back(boxLabelLevels);*/

        Label::LabelAnswer associatedLevel;
        associatedLevel.typeOfAction = Label::TypeOfAction::GoLevel;
        associatedLevel.chooseLevel = i;


        std::shared_ptr<MessageLabel> labelLevel =
        std::make_shared<MessageLabel>(
            Label::WidthUnit::ShortestSide,
            labelNumberWidth, labelNumberHeight,
            JBTypes::vec2f{ .5f - label1Page1Width * .5f
                + label1Page1Width* (.1f + (i%3) * .4f),
                1.f-(0.3f + i/3 * 0.3f)},
            sNumber,
            std::make_shared<Label::LabelAnswer> (associatedLevel)
         );
        labelsPage2.push_back(labelLevel);
        labelLevels.push_back(labelLevel);
    }

    Label::updateLabelsLevels(labelLevels, currentLevel
                              /*_player.levelProgression()*/);

    /*std::shared_ptr<const BoxLabel> labelBox =
    std::make_shared<const BoxLabel>(0.5f, 0.1f,
                                     JBTypes::vec2f{0.5f,0.8f} );*/


    //Page 3 (Failure page)

    const float label1Page3Width = factor * 0.6f;
    std::shared_ptr<const MessageLabel> label1Page3 =
        std::make_shared<const MessageLabel>(
            Label::WidthUnit::ShortestSide,
            label1Page3Width, label1Page1Height,
            JBTypes::vec2f{0.5f,0.8f},
            "You lost");
    std::shared_ptr<const MessageLabel> label2Page3 =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label3Page1Width, label234Page1Height,
            JBTypes::vec2f{0.5f,0.6f},
            "Retry");
    std::shared_ptr<const MessageLabel> label3Page3 =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label4Page1Width, label234Page1Height,
            JBTypes::vec2f{0.5f,0.3f},
            "Exit");

    const vecCstLabel_sptr labelsPage3
    {label1Page3, label2Page3, label3Page3};

    
    //Page 4 (Pause page)
    std::shared_ptr<const MessageLabel> label1Page4 =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label2Page1Width, label234Page1Height,
            JBTypes::vec2f{0.5f,0.4f},
            "Continue");
    std::shared_ptr<const MessageLabel> label2Page4 =
        std::make_shared<const MessageLabel> (
            Label::WidthUnit::ShortestSide,
            label3Page1Width, label234Page1Height,
            JBTypes::vec2f{0.5f,0.6f},
            "Main menu");

    const vecCstLabel_sptr labelsPage4 { label1Page4, label2Page4 };

    const Page_sptr page1 = std::make_shared<Page> (
        nullptr, Page::PageFormat::Full
    );
    const Page_sptr page2 = std::make_shared<Page> (
        page1, Page::PageFormat::Scroll, 10.f
    );
    const Page_sptr page3 = std::make_shared<Page> (
        nullptr, Page::PageFormat::Full
    );
    const Page_sptr page4 = std::make_shared<Page> (
        nullptr, Page::PageFormat::Full
    );

    std::map<CstLabel_sptr, Page::TypeOfLabel> typesPage2;
    std::map<CstLabel_sptr, Page_sptr> childrenPage2;
    for ( CstLabel_sptr& label : labelsPage2) {
        typesPage2[label] = Page::TypeOfLabel::Message;
        childrenPage2[label] = nullptr;
    }

    std::map<CstLabel_sptr, Page_sptr> childrenPage1;
    std::map<CstLabel_sptr, Page::TypeOfLabel> typesPage1;
    childrenPage1[labelsPage1.at(0)] = nullptr;
    childrenPage1[labelsPage1.at(1)] = page2;
    childrenPage1[labelsPage1.at(2)] = nullptr;
    childrenPage1[labelsPage1.at(3)] = nullptr;
    typesPage1[labelsPage1.at(0)] = Page::TypeOfLabel::Message;
    typesPage1[labelsPage1.at(1)] = Page::TypeOfLabel::Message;
    typesPage1[labelsPage1.at(2)] = Page::TypeOfLabel::Message;
    typesPage1[labelsPage1.at(3)] = Page::TypeOfLabel::Message;

    std::map<CstLabel_sptr, Page_sptr> childrenPage3;
    std::map<CstLabel_sptr, Page::TypeOfLabel> typesPage3;
    childrenPage3[labelsPage3.at(0)] = nullptr;
    childrenPage3[labelsPage3.at(1)] = nullptr;
    childrenPage3[labelsPage3.at(2)] = page1;
    typesPage3[labelsPage3.at(0)] = Page::TypeOfLabel::Message;
    typesPage3[labelsPage3.at(1)] = Page::TypeOfLabel::Message;
    typesPage3[labelsPage3.at(2)] = Page::TypeOfLabel::Message;

    std::map<CstLabel_sptr, Page_sptr> childrenPage4;
    std::map<CstLabel_sptr, Page::TypeOfLabel> typesPage4;
    childrenPage4[labelsPage4.at(0)] = nullptr;
    childrenPage4[labelsPage4.at(1)] = page1;
    typesPage4[labelsPage4.at(0)] = Page::TypeOfLabel::Message;
    typesPage4[labelsPage4.at(1)] = Page::TypeOfLabel::Message;

    page1->setBridges(std::move(childrenPage1));
    page2->setBridges(std::move(childrenPage2));
    page3->setBridges(std::move(childrenPage3));
    page4->setBridges(std::move(childrenPage4));
    page1->setTypes(std::move(typesPage1));
    page2->setTypes(std::move(typesPage2));
    page3->setTypes(std::move(typesPage3));
    page4->setTypes(std::move(typesPage4));

    const vecCstPage_sptr pages { page1,page2,page3,page4 };
    return std::make_shared<Menu> (player,page1,page4,nullptr,page3,pages);
}
