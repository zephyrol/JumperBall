/*
 * File: Screen.h
 * Author: Morgenthaler S
 *
 * Created on 5 juillet 2020, 8:41
 */

#ifndef SCREEN_H  
#define SCREEN_H 
#include <scene/Displayable.h>

class Screen : public Displayable {

public : 

Screen();

Displayable::GlobalState getGlobalState() const override;
};


#endif 