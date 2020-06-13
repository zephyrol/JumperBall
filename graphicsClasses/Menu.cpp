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
      getNumberOfPixelsHeight(getHeight(rootPage)))
{
}

void Menu::render() const
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    renderPage(_rootPage);
}


void Menu::renderPage( const std::shared_ptr<const Page>& page) const {
    
    if (page->parent() && page->visibleOnParent()) {
        renderPage(page->parent());
    }

    for( const std::shared_ptr<const Label>& label :_rootPage->labels()) {
        if (label->typeOfLabel() == Label::TypeOfLabel::Message){
            _textRendering.render(*label, glm::vec3(0, 1.f, 1.f));
        }
    }
    
}

const std::shared_ptr<const Page>& Menu::currentPage() const
{
    return _currentPage;
}


void Menu::currentPage(std::shared_ptr<const Page>& page)
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


