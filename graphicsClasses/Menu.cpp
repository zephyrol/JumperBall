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

void Menu::render(const ShaderProgram& sp) const
{
    renderPage(sp, _rootPage);
}


void Menu::renderPage(const ShaderProgram& sp,
    const std::shared_ptr<const Page>& page) const {
    
    if (page->parent() && page->visibleOnParent()) {
        renderPage(sp,page->parent());
    }

    for( const std::shared_ptr<const Label>& label :_rootPage->labels()) {
        _textRendering.render(sp, *label, glm::vec3(0, 0, 1));
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
//TextRendering Menu::initTextRendering(
//                                      const std::shared_ptr<const Page>& root) {
//    TextRendering textRendering;
//    /*std::shared_ptr<const Page> currentPage = root;
//    for( const std::shared_ptr<const Label>& label : currentPage->labels()) {
//
//		if (currentPage->bridges().find(label) != currentPage->bridges().end()
//			&& currentPage->bridges().at(label)) {
//            TextRendering t 
//
//            textRendering[size] = 
//		}
//
//		std::map<unsigned int, TextRendering> child =
//			initTextRendering(currentPage->bridges().at(label));
//		for (std::map<unsigned int, TextRendering>::iterator it
//			= child.begin(); it != child.end(); ++it) {
//			textRendering[it->first] = std::move(it->second);
//		}
//    }*/
//
//    return textRendering;
// }

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
    return static_cast<unsigned int> (RESOLUTION_Y * height);
}


