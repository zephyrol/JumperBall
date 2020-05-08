/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Label.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#ifndef LABEL_H
#define LABEL_H
#include "../Types.h"
#include "../objects/Key.h"
#include "../objects/Coin.h"
#include "../objects/Clock.h"
#include <fstream>

class Label{
public:

    //------------TYPES------------//
    struct Child {
        std::shared_ptr<Label> childLabel;
        std::pair<float,float> position;
    };

    //---------CONSTANTS------------//

    //--CONSTRUCTORS & DESTRUCTORS--//
    Label           (float width,float height);
    virtual ~Label  () = default;

    //-------CONST METHODS----------//
    float width() const;
    float height() const;
    std::vector<std::shared_ptr<Child> > children() const;

    //----------METHODS------------//
    
    //--------STATIC METHODS-------//

private:
    float                                _width;
    float                                _height;
    std::vector<std::shared_ptr<Child> > _children;
};

#endif /* LABEL_H */

