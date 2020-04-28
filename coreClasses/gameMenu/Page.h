
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

#ifndef PAGE_H
#define PAGE_H
#include "Label.h"

class Page
{
public:
    Page(const std::vector<std::shared_ptr<Label> >& labels,
         const std::shared_ptr<Page>& parent = nullptr,
         bool visibleOnParent  = false);

private:
    std::vector<std::shared_ptr<Label> > _labels;
    std::shared_ptr<Page>                _parent;
    bool                                 _visibleOnParent;
};

#endif // PAGE_H
