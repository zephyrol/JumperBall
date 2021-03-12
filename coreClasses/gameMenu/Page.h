/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

#ifndef PAGE_H
#define PAGE_H
#include <list>
#include "Label.h"

class Page;
using Page_sptr = std::shared_ptr <Page>;
using CstPage_sptr = std::shared_ptr <const Page>;
using vecCstPage_sptr = std::vector <CstPage_sptr>;
using vecPage_sptr = std::vector <Page_sptr>;

class Page {
public:

// ------------TYPES------------//
// Slide state => timepoint and yScreenPosition
using slideState = std::pair <JBTypes::timePointMs, float>;
enum class TypeOfLabel { Unknown, Object, Message, Switch, Box, Arrow };
enum class PageFormat { Full, Scroll };
enum class EscapeAnswer { QuitGame, GoToParent, GoToPause };

// --CONSTRUCTORS & DESTRUCTORS--//
Page(const Page_sptr& parent,
     const Page::PageFormat& pageFormat,
     const Page::EscapeAnswer& escapeAnswer,
     float height = 1.f,
     bool visibleOnParent = false);


// -------CONST METHODS--------//
std::weak_ptr <const Page> parent() const;
std::weak_ptr <Page> parent();
bool visibleOnParent() const;
// CstPage_sptr child(float x, float y) const;
CstLabel_sptr matchedLabel(float x, float y) const;
float height() const;
float localPosY() const;
void setBridges(std::map <CstLabel_sptr, Page_sptr>&& bridges);
void setTypes(std::map <CstLabel_sptr, TypeOfLabel>&& labelsTypes);
const Page::EscapeAnswer& getEscapeAnswer() const;

// ----------METHODS-----------//
CstPage_sptr child(const CstLabel_sptr& label) const;
Page_sptr child(const CstLabel_sptr& label);
TypeOfLabel type(const CstLabel_sptr& label) const;
const vecCstLabel_sptr& labels() const;
void pressOnPage();
void release();
void update(bool isPressed, float screenPosY = 0.f);

private:

// --------ATTRIBUTES-----------//
constexpr static float decelerationCoefficient = 10.f; // pagePourcentage /s^2

std::vector <CstLabel_sptr> createLabels() const;
std::vector <Page_sptr> createChildren() const;

const PageFormat _pageFormat;
std::map <CstLabel_sptr, Page_sptr> _bridges;
std::map <CstLabel_sptr, TypeOfLabel> _labelsTypes;
std::vector <CstLabel_sptr> _labels;
std::vector <Page_sptr> _children;
const std::weak_ptr <Page> _parent;
const bool _visibleOnParent;
const float _height;
const Page::EscapeAnswer _escapeAnswer;
float _localPosY; // Page position
float _localPressedPosY;
float _localReleasedPosY;
bool _isPressed;
float _pressedScreenPosY;
JBTypes::timePointMs _lastUpdate;
std::list <slideState> _lastSwipeUpdates;
float _releaseVelocity;
};

#endif // PAGE_H
