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

Menu::Menu(const std::shared_ptr<Page>& rootPage,
           const std::shared_ptr<Page>& pausePage,
           const std::shared_ptr<Page>& successPage,
           const std::shared_ptr<Page>& failurePage):
  _rootPage(rootPage),
  _pausePage(pausePage),
  _successPage(successPage),
  _failurePage(failurePage),
  _textRendering(getCharacters({pausePage,successPage,failurePage}),
      getNumberOfPixelsHeight(getHeight(rootPage))),
  _boxRendering(glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,1.f,1.f)),
  _currentPage(rootPage)
{
}

void Menu::render() const
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    renderPage(_currentPage);
}


void Menu::renderPage( const std::weak_ptr<const Page>& page) const {

    std::shared_ptr<const Page> spPage = page.lock();
    if (spPage->parent().lock() && spPage->visibleOnParent()) {
        renderPage(spPage->parent());
    }

    for( const std::shared_ptr<const Label>& label : page.lock()->labels()) {
        if (label->typeOfLabel() == Label::TypeOfLabel::Message){
            if (label->isActivated()) {
                _textRendering.render(*label, glm::vec3(0, 1.f, 1.f),
                                      spPage->localPosY());
            } else {
                _textRendering.render(*label, glm::vec3(.2f, .2f, .2f),
                                      spPage->localPosY());
            }
        } else if (label->typeOfLabel() == Label::TypeOfLabel::Box) {
            _boxRendering.render(*label);
        }
    }
    
}

const std::shared_ptr<Page>& Menu::currentPage()
{
    return _currentPage;
}


void Menu::currentPage(const std::shared_ptr<Page> &page)
{
    _currentPage = page;
}

void Menu::update(bool isPressed, float screenPosY)
{
    _currentPage->update(isPressed, screenPosY);
}

std::vector<unsigned char> Menu::getCharacters(
    const std::vector<std::shared_ptr<Page> >& pages)
{
    std::vector<unsigned char> characters;

    for( const std::shared_ptr<Page>& page : pages) {
        if (page)
        for( const std::shared_ptr<const Label>& label : page->labels()) {
            
            if (page->bridges().find(label) != page->bridges().end()
                && page->bridges().at(label)) {
                
                const std::vector<unsigned char> childCaracters =
                getCharacters({page->bridges().at(label)});
                characters.insert(characters.end(), childCaracters.begin(),
                                  childCaracters.end());
            }
            
            if (label->typeOfLabel() == Label::TypeOfLabel::Message) {
                for (const char& c : label->message()) {
                    characters.push_back(c);
                }
            }
        }
    }

    return characters;
}

float Menu::getHeight(const std::shared_ptr<Page>& page)
{
    float height = 0.f;

    for( const std::shared_ptr<const Label>& label : page->labels()) {

		if (page->bridges().find(label) != page->bridges().end()
			&& page->bridges().at(label)) {

            if (getHeight(page->bridges().at(label)) > height) {
                height = getHeight(page->bridges().at(label));
            }
		}

        if (label->height() > height) {
            height = label->height();
        };
    }

    return height;
}

unsigned int Menu::getNumberOfPixelsHeight(float height)
{
    return static_cast<unsigned int> (Utility::windowResolutionY * height);
}

std::shared_ptr<Menu> Menu::getJumperBallMenu(size_t currentLevel)
{
    //Page 1
    std::shared_ptr<const MessageLabel> label =
        std::make_shared<const MessageLabel>(
            Utility::xScreenToPortrait(1.f), 0.1f,
            JumperBallTypes::vec2f{0.5f,0.8f},
            "Jumper Ball");
    std::shared_ptr<const MessageLabel> label2 =
        std::make_shared<const MessageLabel> (
            Utility::xScreenToPortrait(0.4f), 0.05f,
            JumperBallTypes::vec2f{0.5f,0.6f},
            "Play");
    std::shared_ptr<const MessageLabel> label3 =
        std::make_shared<const MessageLabel> (
            Utility::xScreenToPortrait(0.6f), 0.05f,
            JumperBallTypes::vec2f{0.5f,0.4f},
            "Store");
    std::shared_ptr<const MessageLabel> label4 =
        std::make_shared<const MessageLabel> (
            Utility::xScreenToPortrait(0.4f), 0.05f,
            JumperBallTypes::vec2f{0.5f,0.2f},
            "Exit");

    const std::vector<std::shared_ptr<const Label> > labelsPage1
    {label, label2, label3, label4};

    //Page 2
    std::vector<std::shared_ptr<const Label> > labelsPage2;

    std::shared_ptr<const MessageLabel> labelLevelsTitle =
    std::make_shared<const MessageLabel>
    (Utility::xScreenToPortrait(1.  ),
     0.2f, JumperBallTypes::vec2f{0.5f, 1.f - 0.1f},
     "Levels");
    labelsPage2.push_back(labelLevelsTitle);

    //constexpr float offsetBox = 0.02f;

    std::vector<std::shared_ptr<Label> > labelLevels;
    for (size_t i = 0; i < 99; ++i) {

        std::string sNumber;
        if( i < 10 ) {
            sNumber.append("0");
        }
        sNumber.append(std::to_string(i+1));


        /*std::shared_ptr<const BoxLabel> boxLabelLevels =
        std::make_shared<const BoxLabel>
        (   Utility::xScreenToPortrait(.2f), 0.1f,
         JumperBallTypes::vec2f{.5f - Utility::xScreenToPortrait(.5f)
            + Utility::xScreenToPortrait(.1f + (i%3) * .4f), 1.f-(0.3f + i/3 * 0.3f)-offsetBox});
        labelsPage2.push_back(boxLabelLevels);*/

        Label::LabelAnswer associatedLevel;
        associatedLevel.typeOfAction = Label::TypeOfAction::GoLevel;
        associatedLevel.chooseLevel = i;


        std::shared_ptr<MessageLabel> labelLevel =
        std::make_shared<MessageLabel> (
            Utility::xScreenToPortrait(.2f), 0.1f,
            JumperBallTypes::vec2f{ .5f - Utility::xScreenToPortrait(.5f)
                + Utility::xScreenToPortrait(.1f + (i%3) * .4f),
                1.f-(0.3f + i/3 * 0.3f)},
            sNumber,
            std::make_shared<Label::LabelAnswer> (associatedLevel)
         );
        labelsPage2.push_back(labelLevel);
        labelLevels.push_back(labelLevel);
    }

    Label::updateLabelsLevels(labelLevels, currentLevel
                              /*_player.levelProgression()*/);

    std::shared_ptr<const BoxLabel> labelBox =
    std::make_shared<const BoxLabel>(0.5f, 0.1f,
                                     JumperBallTypes::vec2f{0.5f,0.8f} );


    //Page 3 (Failure page)

    std::shared_ptr<const MessageLabel> label1Page3 =
        std::make_shared<const MessageLabel>(
            Utility::xScreenToPortrait(1.f), 0.1f,
            JumperBallTypes::vec2f{0.5f,0.8f},
            "You lost");
    std::shared_ptr<const MessageLabel> label2Page3 =
        std::make_shared<const MessageLabel> (
            Utility::xScreenToPortrait(0.4f), 0.05f,
            JumperBallTypes::vec2f{0.5f,0.6f},
            "Retry");
    std::shared_ptr<const MessageLabel> label3Page3 =
        std::make_shared<const MessageLabel> (
            Utility::xScreenToPortrait(0.4f), 0.05f,
            JumperBallTypes::vec2f{0.5f,0.3f},
            "Exit");

    const std::vector<std::shared_ptr<const Label> > labelsPage3
    {label1Page3, label2Page3, label3Page3};

    
    //Page 4 (Pause page)
    /*std::shared_ptr<const MessageLabel> label1Page3 =
        std::make_shared<const MessageLabel>(
            Utility::xScreenToPortrait(1.f), 0.1f,
            JumperBallTypes::vec2f{0.5f,0.8f},
            "");*/
    std::shared_ptr<const MessageLabel> label1Page4 =
        std::make_shared<const MessageLabel> (
            Utility::xScreenToPortrait(0.4f), 0.05f,
            JumperBallTypes::vec2f{0.5f,0.4f},
            "Continue");
    std::shared_ptr<const MessageLabel> label2Page4 =
        std::make_shared<const MessageLabel> (
            Utility::xScreenToPortrait(0.7f), 0.05f,
            JumperBallTypes::vec2f{0.5f,0.6f},
            "Main menu");

    const std::vector<std::shared_ptr<const Label> > labelsPage4
    {label1Page4, label2Page4};


    /*std::map<std::shared_ptr<const Label>,
        std::shared_ptr<const Page> > bridgesPage1 ;

    std::map<std::shared_ptr<const Label>,
        std::shared_ptr<const Page> > bridgesPage2 ;*/

    const std::shared_ptr<Page> page1 =
        std::make_shared<Page> (labelsPage1, nullptr, false);

    const std::shared_ptr<Page> page2 =
        std::make_shared<Page> (labelsPage2, page1, false, 10.f);

    const std::shared_ptr<Page> page3 =
        std::make_shared<Page> (labelsPage3, nullptr, false);

    const std::shared_ptr<Page> page4 =
        std::make_shared<Page> (labelsPage4, nullptr, false);

    page1->addBridge(label2, page2);
    page3->addBridge(label3Page3, page1);
    page4->addBridge(label2Page4, page1);

    return std::make_shared<Menu> (page1,page4,nullptr,page3);
}

const std::shared_ptr<Page> &Menu::pausePage(){
    return _pausePage;
}

const std::shared_ptr<Page> &Menu::failurePage() {
    return _failurePage;
}

const std::shared_ptr<Page> &Menu::successPage() {
    return _successPage;
}

const std::shared_ptr<Page> &Menu::rootPage() {
    return _rootPage;
}


