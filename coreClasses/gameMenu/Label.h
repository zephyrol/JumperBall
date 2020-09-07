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

class Label;
using Label_sptr = std::shared_ptr<Label>;
using CstLabel_sptr = std::shared_ptr<const Label>;
using vecCstLabel_sptr = std::vector<CstLabel_sptr>;
using vecLabel_sptr = std::vector<Label_sptr>;

class Label{
public:

    //------------TYPES------------//
    enum class TypeOfLabel { Object, Message, Switch, Box};

    enum class TypeOfAction { PredefinedAction, GoLevel };
    enum class PredefinedAction { ExitGame, ChooseEnglish, ChooseFrench };

    struct LabelAnswer {
        Label::TypeOfAction typeOfAction;
        union {
            size_t chooseLevel;
            //bool switchMusic;
            //bool switchSound;
            PredefinedAction predefinedAction;
        };
    };


    //---------CONSTANTS------------//

    //--CONSTRUCTORS & DESTRUCTORS--//
    Label                   (float width,
                             float height,
                             const JBTypes::vec2f& position,
                             bool activated = false,
                             const std::shared_ptr<LabelAnswer> action
                                = nullptr,
                             bool fixed = false);
    virtual ~Label          ()                                        = default;

    //-------CONST METHODS----------//
    float                   width()                                       const;
    float                   height()                                      const;
    
    const vecCstLabel_sptr& children()          const;
    bool                    isFixed()                                     const;
    bool                    isActivated()                                 const;
    const std::shared_ptr<LabelAnswer>& action()                          const;

    JBTypes::vec2f          position()                                    const;
    virtual TypeOfLabel     typeOfLabel()                             const = 0;
    virtual std::string     message()                                     const;

    //----------METHODS-------------//
    void                    activate();
    void                    deactivate();

    //--------STATIC METHODS-------//
    static void             updateLabelsLevels(vecLabel_sptr& labels,
                                               size_t end);

private:
    
    //--------ATTRIBUTES-----------//
    const float                        _width;
    const float                        _height;
    const vecCstLabel_sptr             _children;
    const JBTypes::vec2f               _position;
    const bool                         _fixed;
    bool                               _activated;
    const std::shared_ptr<LabelAnswer> _action;

};


#endif /* LABEL_H */

