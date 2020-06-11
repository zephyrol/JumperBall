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

    //---------CONSTANTS------------//

    //--CONSTRUCTORS & DESTRUCTORS--//
    Label           (float width,float height,
                     const JumperBallTypes::vec2f& position);
    virtual ~Label  () = default;

    //-------CONST METHODS----------//
    float width() const;
    float height() const;
    const std::vector<std::shared_ptr<const Label> >& children() const;
    JumperBallTypes::vec2f position() const;
    virtual TypeOfLabel typeOfLabel() const = 0;
    virtual std::string message() const;

    //----------METHODS------------//
    
    //--------STATIC METHODS-------//

private:
    const float                                       _width;
    const float                                       _height;
    const std::vector<std::shared_ptr<const Label> >  _children;
    const JumperBallTypes::vec2f                      _position;

};

#endif /* LABEL_H */

