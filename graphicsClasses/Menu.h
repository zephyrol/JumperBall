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
    //TextRendering initTextRendering( const std::shared_ptr<const Page>& root);
    const std::shared_ptr<const Page> _rootPage;
    TextRendering _textRendering;
    std::shared_ptr<const Page> _currentPage;


    void renderPage( const std::shared_ptr<const Page>& page) const;

    const std::shared_ptr<const Page>& currentPage() const;
    void currentPage(std::shared_ptr<const Page>& page);
    

    static std::vector<unsigned char> getCharacters
        (const std::shared_ptr<const Page>& page);
    static float getHeight (const std::shared_ptr<const Page>& page);
    static unsigned int getNumberOfPixelsHeight(float height);
};

#endif /* MENU_H */

