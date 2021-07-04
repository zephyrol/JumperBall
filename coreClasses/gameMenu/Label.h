/*
 * File: Label.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#ifndef LABEL_H
#define LABEL_H
#include "system/Types.h"
#include "system/Geometry.h"
// TODO clean those include
#include "scene/items/Key.h"
#include "scene/items/Coin.h"
#include "scene/items/Clock.h"
#include "scene/SceneElement.h"
#include <fstream>

class Label;
using Label_sptr = std::shared_ptr <Label>;
using CstLabel_sptr = std::shared_ptr <const Label>;
using vecCstLabel_sptr = std::vector <CstLabel_sptr>;
using vecLabel_sptr = std::vector <Label_sptr>;

class Label {
public:

enum class TypeOfAction { PredefinedAction, GoLevel };
enum class PredefinedAction {
    ExitGame,
    RetryLevel,
    RunNextLevel,
    GoBack,
    ChooseEnglish,
    ChooseFrench
};

struct LabelAnswer {
    Label::TypeOfAction typeOfAction;
    union {
        size_t chooseLevel;
        // bool switchMusic;
        // bool switchSound;
        PredefinedAction predefinedAction;
    };
};

enum class WidthUnit { ScreenWidth, ShortestSide };

Label(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    bool activated = false,
    const std::shared_ptr <LabelAnswer>& action = nullptr,
    bool fixed = false);
virtual ~Label() = 0;

float width() const;
float height() const;

virtual vecGeometry genGeometries() const;

const vecCstLabel_sptr& children() const;
bool isFixed() const;
bool isActivated() const;
const std::shared_ptr <LabelAnswer>& action() const;
const Label::WidthUnit& widthUnit() const;

const JBTypes::vec2f& position() const;
virtual std::string message() const;

void activate();
void deactivate();

static void updateLabelsLevels(vecLabel_sptr& labels,
                               size_t end);


private:

const Label::WidthUnit _widthUnit;
const float _width;
const float _height;
const vecCstLabel_sptr _children;
const JBTypes::vec2f _position;
const bool _fixed;
bool _activated;
const std::shared_ptr <LabelAnswer> _action;

};

#endif /* LABEL_H */
