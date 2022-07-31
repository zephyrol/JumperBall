/*
 * File: Menu.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#include "Menu.h"
#include "CenteredNode.h"
#include "UpNode.h"

Menu::Menu(
    Player &player,
    const Page_sptr &rootPage,
    const Page_sptr &pausePage,
    const Page_sptr &successPage,
    const Page_sptr &failurePage,
    const vecPage_sptr &pages) :
    _player(player),
    _rootPage(rootPage),
    _pausePage(pausePage),
    _successPage(successPage),
    _failurePage(failurePage),
    _pages(pages),
    _cstPages(createCstPages()),
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
    if (_currentPage) {
        _currentPage->update(isPressed, screenPosY);
    }
}

void Menu::rootPageAsCurrentPage() {
    _currentPage = _rootPage;
}


bool Menu::parentPageAsCurrentPage() {
    const std::shared_ptr<Page> parent = _currentPage->parent().lock();
    if (parent) {
        _currentPage = parent;
        return true;
    } else {
        return false;
    }
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

Menu::MenuAnswer Menu::mouseClick(float mouseX, float mouseY) {
    Menu::MenuAnswer menuAnswer;
    if (!_currentPage) {
        return menuAnswer;
    }

    /* TODO
    const auto& label = _currentPage->matchedLabel(mouseX, mouseY);
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
    return menuAnswer;
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
    return _cstPages;
}

std::shared_ptr<Menu> Menu::getJumperBallMenu(Player &player, float ratio) {
    Node_sptr mainNode = std::make_shared<CenteredNode>(
        nullptr,
        ratio
    );
    Node_sptr mainTitleNode = std::make_shared<CenteredNode>(
        mainNode,
        9.f / 16.f
    );
    Node_sptr jumperBallTitle = std::make_shared<UpNode>(
        mainTitleNode,
        4.f
    );

    Page::NodeMessageAssociations nodeToMessage = {
        {
            {jumperBallTitle, "Jumper Ball"}
        }
    };

    const Page_sptr page1 = std::make_shared<Page>(
        nullptr,
        std::move(nodeToMessage),
        Page::EscapeAnswer::QuitGame
    );
    return std::make_shared<Menu>(
        player,
        page1,
        nullptr,
        nullptr,
        nullptr,
        vecPage_sptr()
    );
}

/*std::shared_ptr<Menu> Menu::getJumperBallMenu(
    Player &player,
    size_t currentLevel,
    unsigned int sizeX,
    unsigned int sizeY
) {
    const float ratioX = static_cast <float>(sizeX) / static_cast <float>(sizeY);
    // Page 1
    const auto getFactor = [](float ratioX) {
        const bool isSmartPhoneFormat = ratioX < 1.f;
        const float ratioY = 1.f / ratioX;
        return isSmartPhoneFormat ? 1.f : ratioY;
    };
    auto label1Page1 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label1Page1Width = factor * 0.9f;
            const float label1Page1HeightDivide = 9.f;
            const float label1Page1Height = label1Page1Width * ratioX / label1Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label1Page1Width, label1Page1Height, 0.5f, 0.8f};
            return dimensions;
        },
        ratioX,
        "Jumper Ball");
    auto label2Page1 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);

            const float label2Page1Width = factor * 0.4f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label2Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label2Page1Width, label234Page1Height, 0.5f, 0.6f};
            return dimensions;
        },
        ratioX,
        "Play");
    auto label3Page1 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);

            const float label3Page1Width = factor * 0.6f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label3Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label3Page1Width, label234Page1Height, 0.5f, 0.4f};
            return dimensions;
        },
        ratioX,
        "Store");
    auto label4Page1 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label4Page1Width = factor * 0.4f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label4Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label4Page1Width, label234Page1Height, 0.5f, 0.2f};
            return dimensions;
        },
        ratioX,
        "Exit");

    const vecLabel_sptr labelsPage1{label1Page1, label2Page1, label3Page1, label4Page1};

    // Page 2
    vecLabel_sptr labelsPage2;

    auto labelLevelsTitle = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label1Page1Width = factor * 0.6f;
            const float label1Page1HeightDivide = 9.f;
            const float label1Page1Height = label1Page1Width * ratioX / label1Page1HeightDivide;
            const Label::LabelDimensions dimensions = {
                label1Page1Width,
                label1Page1Height,
                0.5f - label1Page1Width * .5f + label1Page1Width * (0.6f),
                1.f - 0.1f
            };
            return dimensions;
        },
        ratioX,
        "Levels"
    );

    Label::LabelAnswer arrowAction;
    arrowAction.typeOfAction = Label::TypeOfAction::PredefinedAction;
    arrowAction.predefinedAction = Label::PredefinedAction::GoBack;
    auto labelLevelsArrowBack = std::make_shared<ArrowLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float arrowBackWidth = factor * 0.15f;
            const float label1Page1Width = factor * 0.9f;
            const float label1Page1HeightDivide = 9.f;
            const float label1Page1Height = label1Page1Width * ratioX / label1Page1HeightDivide;
            const Label::LabelDimensions dimensions = {
                arrowBackWidth,
                label1Page1Height,
                0.5f - label1Page1Width * .5f + label1Page1Width * (0.1f),
                1.f - 0.1f
            };
            return dimensions;
        },
        ratioX,
        std::make_shared<Label::LabelAnswer>(arrowAction)
    );

    labelsPage2.push_back(labelLevelsTitle);
    labelsPage2.push_back(labelLevelsArrowBack);

    // constexpr float offsetBox = 0.02f;

    vecLabel_sptr labelLevels;
    for (size_t i = 0; i < 99; ++i) {
        std::string sNumber;
        if (i < 10) {
            sNumber.append("0");
        }
        sNumber.append(std::to_string(i + 1));

        Label::LabelAnswer associatedLevel;
        associatedLevel.typeOfAction = Label::TypeOfAction::GoLevel;
        associatedLevel.chooseLevel = i;

        std::shared_ptr<MessageLabel> labelLevel =
            std::make_shared<MessageLabel>(
                Label::WidthUnit::ShortestSide,
                [getFactor, i](float ratioX) {
                    const float factor = getFactor(ratioX);
                    const float labelNumberWidth = factor * 0.1f;
                    const float labelNumberHeight = labelNumberWidth * ratioX;
                    const float label1Page1Width = factor * 0.9f;
                    const Label::LabelDimensions dimensions = {
                        labelNumberWidth,
                        labelNumberHeight,
                        .5f - label1Page1Width * .5f + label1Page1Width * (.1f + (i % 3) * .4f),
                        1.f - (0.3f + i / 3 * 0.3f)
                    };
                    return dimensions;
                },
                ratioX,
                sNumber,
                std::make_shared<Label::LabelAnswer>(associatedLevel)
            );
        labelsPage2.push_back(labelLevel);
        labelLevels.push_back(labelLevel);
    }

    Label::updateLabelsLevels(labelLevels, currentLevel);

    // Page 3 (Failure page)
    auto label1Page3 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label1Page3Width = factor * 0.6f;
            const float label1Page1HeightDivide = 9.f;
            const float label1Page1Width = factor * 0.9f;
            const float label1Page1Height = label1Page1Width * ratioX / label1Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label1Page3Width, label1Page1Height, 0.5f, 0.8f};
            return dimensions;
        },
        ratioX,
        "You lost");
    auto label2Page3 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label4Page1Width = factor * 0.4f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label4Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {
                label4Page1Width,
                label234Page1Height,
                0.5f,
                0.6f
            };
            return dimensions;
        },
        ratioX,
        "Retry");
    auto label3Page3 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label4Page1Width = factor * 0.4f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label4Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {
                label4Page1Width,
                label234Page1Height,
                0.5f,
                0.3f
            };
            return dimensions;
        },
        ratioX,
        "Exit"
    );

    const vecLabel_sptr labelsPage3{label1Page3, label2Page3, label3Page3};


    // Page 4 (Pause page)
    auto label1Page4 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label2Page1Width = factor * 0.4f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label2Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {
                label2Page1Width,
                label234Page1Height,
                0.5f,
                0.4f
            };
            return dimensions;
        },
        ratioX,
        "Continue");
    auto label2Page4 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label3Page1Width = factor * 0.6f;
            const float label2Page1Width = factor * 0.4f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label2Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {
                label3Page1Width,
                label234Page1Height,
                0.5f,
                0.6f
            };
            return dimensions;
        },
        ratioX,
        "Main menu");
    const vecLabel_sptr labelsPage4{label1Page4, label2Page4};

    // Page 5 (Success page)
    auto label1Page5 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label1Page5Width = factor * 0.8f;
            const float label1Page1HeightDivide = 9.f;
            const float label1Page1Width = factor * 0.9f;
            const float label1Page1Height = label1Page1Width * ratioX / label1Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label1Page5Width, label1Page1Height, 0.5f, 0.8f};
            return dimensions;
        },
        ratioX,
        "Good game!");
    auto label2Page5 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label2Page5Width = factor * 0.7f;
            const float label1Page1HeightDivide = 9.f;
            const float label1Page1Width = factor * 0.9f;
            const float label1Page1Height = label1Page1Width * ratioX / label1Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label2Page5Width, label1Page1Height, 0.5f, 0.6f};
            return dimensions;
        },
        ratioX,
        "Continue");
    auto label3Page5 = std::make_shared<MessageLabel>(
        Label::WidthUnit::ShortestSide,
        [getFactor](float ratioX) {
            const float factor = getFactor(ratioX);
            const float label4Page1Width = factor * 0.4f;
            const float label234Page1HeightDivide = 8.f;
            const float label234Page1Height = label4Page1Width * ratioX / label234Page1HeightDivide;
            const Label::LabelDimensions dimensions = {label4Page1Width, label234Page1Height, 0.5f, 0.3f};
            return dimensions;
        },
        ratioX,
        "Exit");

    const vecLabel_sptr labelsPage5{label1Page5, label2Page5, label3Page5};

    const Page_sptr page1 = std::make_shared<Page>(
        nullptr, Page::PageFormat::Full, Page::EscapeAnswer::QuitGame
    );
    const Page_sptr page2 = std::make_shared<Page>(
        page1,
        Page::PageFormat::Scroll,
        Page::EscapeAnswer::GoToParent,
        10.f
    );
    const Page_sptr page3 = std::make_shared<Page>(
        page1, Page::PageFormat::Full, Page::EscapeAnswer::GoToParent
    );
    const Page_sptr page4 = std::make_shared<Page>(
        page1, Page::PageFormat::Full, Page::EscapeAnswer::GoToParent
    );
    const Page_sptr page5 = std::make_shared<Page>(
        page1, Page::PageFormat::Full, Page::EscapeAnswer::GoToParent
    );

    std::unordered_map<Label_sptr, Page_sptr> childrenPage2;
    for (const Label_sptr &label: labelsPage2) {
        childrenPage2[label] = nullptr;
    }

    std::unordered_map<Label_sptr, Page_sptr> childrenPage1;
    childrenPage1[labelsPage1.at(0)] = nullptr;
    childrenPage1[labelsPage1.at(1)] = page2;
    childrenPage1[labelsPage1.at(2)] = nullptr;
    childrenPage1[labelsPage1.at(3)] = nullptr;

    std::unordered_map<Label_sptr, Page_sptr> childrenPage3;
    childrenPage3[labelsPage3.at(0)] = page5;
    childrenPage3[labelsPage3.at(1)] = nullptr;
    childrenPage3[labelsPage3.at(2)] = page1;

    std::unordered_map<Label_sptr, Page_sptr> childrenPage4;
    childrenPage4[labelsPage4.at(0)] = nullptr;
    childrenPage4[labelsPage4.at(1)] = page1;

    std::unordered_map<Label_sptr, Page_sptr> childrenPage5;
    childrenPage5[labelsPage5.at(0)] = nullptr;
    childrenPage5[labelsPage5.at(1)] = page1;

    page1->setBridges(std::move(childrenPage1));
    page2->setBridges(std::move(childrenPage2));
    page3->setBridges(std::move(childrenPage3));
    page4->setBridges(std::move(childrenPage4));
    page5->setBridges(std::move(childrenPage5));

    const vecPage_sptr pages{page1, page2, page3, page4, page5};
    return std::make_shared<Menu>(player, page1, page4, page5, page3, pages);
} */

Menu::MenuAnswer Menu::escapeAction() {
    const Page::EscapeAnswer &escapeAnswer = _currentPage->getEscapeAnswer();
    Menu::MenuAnswer menuAnswer;
    if (escapeAnswer == Page::EscapeAnswer::QuitGame) {
        menuAnswer.action = Menu::Action::QuitGame;
    } else if (escapeAnswer == Page::EscapeAnswer::GoToParent) {
        menuAnswer.action = Menu::Action::GoBack;
        parentPageAsCurrentPage();
    } else {
        menuAnswer.action = Menu::Action::None;
    }
    return menuAnswer;
}

vecCstPage_sptr Menu::createCstPages() const {
    vecCstPage_sptr cstPages;
    for (const auto &pages: _pages) {
        cstPages.push_back(pages);
    }
    return cstPages;
}
