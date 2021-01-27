
/*
 * File:   SwitchLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#ifndef SWITCHLABEL_H
#define SWITCHLABEL_H
#include "Label.h"

class SwitchLabel: public Label
{
public:
    SwitchLabel(
        const Label::WidthUnit& widthUnit,
        float width,
        float height,
        const JBTypes::vec2f& position,
        bool switchButton = false);

    void turnOn();
    void turnOff();

private:
    bool _switch;
};

#endif // SWITCHLABEL_H
