/*
 * File: Label.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#ifndef LABEL_H
#define LABEL_H
#include "system/Geometry.h"
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

struct LabelDimensions {
    float width;
    float height;
    float positionX;
    float positionY;
};

enum class WidthUnit { ScreenWidth, ShortestSide };

Label(
    const Label::WidthUnit& widthUnit,
    const std::function<Label::LabelDimensions(float)>& updateLabelSizesFct,
    float ratio,
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

float positionX() const;
float positionY() const;

virtual std::string message() const;

void activate();
void deactivate();
void resize(float screenRatio);

static void updateLabelsLevels(vecLabel_sptr& labels,
                               size_t end);


private:

const Label::WidthUnit _widthUnit;
const std::function<Label::LabelDimensions(float)> _updateLabelSizes;
Label::LabelDimensions _dimensions;
const vecCstLabel_sptr _children;
const bool _fixed;
bool _activated;
const std::shared_ptr <LabelAnswer> _action;

};

#endif /* LABEL_H */
