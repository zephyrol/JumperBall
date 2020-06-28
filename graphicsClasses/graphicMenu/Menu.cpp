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

Menu::Menu(const std::shared_ptr<const Page>& rootPage):
  _rootPage(rootPage),
  _textRendering(getCharacters(rootPage), 
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


void Menu::renderPage( const std::shared_ptr<const Page>& page) const {
    
    if (page->parent() && page->visibleOnParent()) {
        renderPage(page->parent());
    }

    for( const std::shared_ptr<const Label>& label :page->labels()) {
        if (label->typeOfLabel() == Label::TypeOfLabel::Message){
            if (label->isActivated()) {
                _textRendering.render(*label, glm::vec3(0, 1.f, 1.f));
            } else {
                _textRendering.render(*label, glm::vec3(.2f, .2f, .2f));
            }
        } else if (label->typeOfLabel() == Label::TypeOfLabel::Box) {
            _boxRendering.render(*label);
        }
    }
    
}

const std::shared_ptr<const Page>& Menu::currentPage() const
{
    return _currentPage;
}


void Menu::currentPage(const std::shared_ptr<const Page> &page)
{
    _currentPage = page;
}

std::vector<unsigned char> Menu::getCharacters(
    const std::shared_ptr<const Page>& page)
{
    std::vector<unsigned char> characters;

    for( const std::shared_ptr<const Label>& label : page->labels()) {

		if (page->bridges().find(label) != page->bridges().end()
			&& page->bridges().at(label)) {

			const std::vector<unsigned char> childCaracters =
				getCharacters(page->bridges().at(label));
			characters.insert(characters.end(), childCaracters.begin(),
				childCaracters.end());
		}

        if (label->typeOfLabel() == Label::TypeOfLabel::Message) {
            for (const char& c : label->message()) {
                characters.push_back(c);
            }
        }
    }

    return characters;
}

float Menu::getHeight(const std::shared_ptr<const Page>& page)
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
    //Menu 1
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

    //Menu 2
    std::vector<std::shared_ptr<const Label> > labelsPage2;

    std::shared_ptr<const MessageLabel> labelLevelsTitle =
    std::make_shared<const MessageLabel>
    (Utility::xScreenToPortrait(1.  ),
     0.2f, JumperBallTypes::vec2f{0.5f, 1.f - 0.1f},
     "Levels");
    labelsPage2.push_back(labelLevelsTitle);

    //constexpr float offsetBox = 0.02f;

    std::vector<std::shared_ptr<Label> > labelLevels;
    for (size_t i = 0; i < 15; ++i) {

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

    //Menu 2
    std::shared_ptr<const BoxLabel> labelBox =
    std::make_shared<const BoxLabel>(0.5f, 0.1f,
                                     JumperBallTypes::vec2f{0.5f,0.8f} );


    const std::vector<std::shared_ptr<const Label> > labelsPage1
    {label, label2, label3, label4};


    std::map<std::shared_ptr<const Label>,
        std::shared_ptr<const Page> > bridgesPage1 ;

    std::map<std::shared_ptr<const Label>,
        std::shared_ptr<const Page> > bridgesPage2 ;

    const std::shared_ptr<Page> page1 =
        std::make_shared<Page> (labelsPage1, nullptr, false);

    const std::shared_ptr<Page> page2 =
        std::make_shared<Page> (labelsPage2, page1, false);

    page1->addBridge(label2, page2);

    return std::make_shared<Menu> (page1);
}

