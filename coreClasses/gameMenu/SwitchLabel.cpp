#include "SwitchLabel.h"
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SwitchLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

SwitchLabel::SwitchLabel(float width, float height, bool switchButton):
    Label(width,height),
    _switch(switchButton)
{
}
