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
    enum class TypeOfLabel { Object, Message, Switch};
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
    virtual TypeOfLabel typeOfLabel() const = 0;

    //----------METHODS------------//
    
    //--------STATIC METHODS-------//

private:
    const float                                 _width;
    const float                                 _height;
    const std::vector<std::shared_ptr<Child> >  _children;
};

#endif /* LABEL_H */

