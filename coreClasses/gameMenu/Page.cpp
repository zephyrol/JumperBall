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
	bool visibleOnParent,
    float height) :
    _labels(labels),
    _bridges{},
    _parent(parent),
    _visibleOnParent(visibleOnParent),
    _height(height),
    _posY(0.f)
{ }

const std::vector<std::shared_ptr<const Label> >& Page::labels() const{
    return _labels;
}

const std::map<std::shared_ptr<const Label>, std::shared_ptr<const Page> >& 
Page::bridges() const {
    return _bridges;
}

const std::shared_ptr<const Page>& Page::parent() const {
    return _parent;
}

bool Page::visibleOnParent() const {
    return _visibleOnParent;
}

std::shared_ptr<const Page> Page::child(float x, float y) const{

    for (const std::shared_ptr<const Label>& label : _labels) {
        if( (_bridges.find(label) != _bridges.end()) && _bridges.at(label) &&
           x > label->position().x - label->width()/2.f &&
           x < label->position().x + label->width()/2.f &&
           y > label->position().y - label->height()/2.f &&
           y < label->position().y + label->height()/2.f) {
            return bridges().at(label);
        }
    }
    return nullptr;
}

float Page::height() const {
    return _height;
}

void Page::addBridge(const std::shared_ptr<const Label> label,
                     const std::shared_ptr<const Page> page) {
    if (std::find(_labels.begin(), _labels.end(), label) == _labels.end()) {
        std::cout << "Trying to associate a label that does not exist in the" <<
            "current page ... the operation is skipped ..." << std::endl;
    } else {
        _bridges[label] = page;
    }
}
