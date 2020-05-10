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
  _textRendering(initTextRendering(rootPage))
{

}

std::map<unsigned int, TextRendering> Menu::initTextRendering(
                                      const std::shared_ptr<const Page>& root) {
    std::map<unsigned int, TextRendering> textRendering;
    std::shared_ptr<const Page> currentPage = root;
    for( const std::shared_ptr<const Label>& label : currentPage->labels()) {

        if (currentPage->bridges().find(label) != currentPage->bridges().end()
                && currentPage->bridges().at(label)){

            std::map<unsigned int, TextRendering> child =
                initTextRendering(currentPage->bridges().at(label));

            for (std::map<unsigned int, TextRendering>::iterator it
                    = child.begin(); it != child.end() ; ++it) {
                textRendering[it->first] = std::move(it->second);
            }

        }
    }

    return textRendering;
 }


