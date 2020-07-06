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
#include "BoxRendering.h"

class Menu
{
public:
    Menu(const std::shared_ptr<const Page>& rootPage,
         const std::shared_ptr<const Page>& pausePage,
         const std::shared_ptr<const Page>& successPage,
         const std::shared_ptr<const Page>& failurePage);

    void render() const;
    const std::shared_ptr<const Page>& currentPage() const;
    void currentPage(const std::shared_ptr<const Page>& page);

    
    const std::shared_ptr<const Page>& rootPage() const;
    const std::shared_ptr<const Page>& pausePage() const;
    const std::shared_ptr<const Page>& successPage() const;
    const std::shared_ptr<const Page>& failurePage() const;
    
    static std::shared_ptr<Menu> getJumperBallMenu(size_t currentLevel);

private:
    
    const std::shared_ptr<const Page> _rootPage;
    const std::shared_ptr<const Page> _pausePage;
    const std::shared_ptr<const Page> _successPage;
    const std::shared_ptr<const Page> _failurePage;
    
    TextRendering _textRendering;
    BoxRendering _boxRendering;
    std::shared_ptr<const Page> _currentPage;


    void renderPage( const std::weak_ptr<const Page>& page) const;

    static std::vector<unsigned char> getCharacters
        (const std::vector<std::shared_ptr<const Page> >& pages);
    static float getHeight (const std::shared_ptr<const Page>& page);
    static unsigned int getNumberOfPixelsHeight(float height);
};

#endif /* MENU_H */

