/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Menu.h
 * Author: Morgenthaler S
 *
 * Created on 10 mai 2020, 15:47
 */

#ifndef MENU_H
#define MENU_H
#include <gameMenu/Page.h>
#include "TextRendering.h"

class Menu
{
public:
    Menu(const std::shared_ptr<const Page>& rootPage);
    void render() const;


private:
    std::map<unsigned int, TextRendering> initTextRendering(
                                      const std::shared_ptr<const Page>& root);
    const std::shared_ptr<const Page> _rootPage;
    const std::map<unsigned int, TextRendering> _textRendering;
};

#endif /* MENU_H */

