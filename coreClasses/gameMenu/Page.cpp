#include "Page.h"

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Map.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

Page::Page(const std::vector<std::shared_ptr<const Label> >& labels,
           const std::shared_ptr<const Page>& parent,
           bool  visibleOnParent):
    _labels(labels),
    _parent(parent),
    _visibleOnParent(visibleOnParent)
{
}
