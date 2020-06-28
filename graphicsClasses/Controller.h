//
//  Controller.hpp
//  JumperBallGraphic
//
//  Created by S Morgenthaler on 25/05/2020.
//

#ifndef Controller_h
#define Controller_h

#include <Player.h>
#include <Rendering.h>
#include "graphicMenu/Menu.h"

class Controller {
    
public:
    
    //------------TYPES------------//
    enum class Status {Pressed,Released};
    enum class Button {Up, Down, Left, Right, Escape, Validate};
    enum class ScreenDirection {North, South, East, West};
    
    
    //--CONSTRUCTORS & DESTRUCTORS--//
    Controller();
    
    //----------METHODS------------//
    void            interactionButtons (const Button& button,
                                     const Status& status);
    void            interactionMouse (const Status& status,
                                     float posX, float posY);

    void            run();

    void currentMap(const std::shared_ptr<Map>& currentMap);
    const std::shared_ptr<Map> &currentMap() const;

    void currentBall(const std::shared_ptr<Ball>& currentBall);
    const std::shared_ptr<Ball> &currentBall() const;

    const std::shared_ptr<Menu>&  menu() const;

    
private:
    //-------CONST METHODS--------//
    ScreenDirection nearestDirection(float posX, float posY) const;
    
    //----------METHODS------------//
    void            manageUp (const Status& status);
    void            manageDown (const Status& status);
    void            manageRight (const Status& status);
    void            manageLeft (const Status& status);
    void            manageEscape (const Status& status);
    void            manageValidateButton (const Status& status);
    
    //Mouse/TouchPad/TouchScreen
    void            pressMouse (float posX, float posY);
    void            updateMouse (float posX, float posY);
    void            releaseMouse (float posX, float posY);
    void            manageValidateMouse ();

    //--------ATTRIBUTES-----------//
    Player                   _player;
    std::shared_ptr<Menu>    _menu;
    std::map<Button, Status> _buttonsStatuts;

    
    float           _mousePressingXCoord;
    float           _mousePressingYCoord;
    bool            _mouseIsPressed;

    std::shared_ptr<Map>    _currentMap;
    std::shared_ptr<Ball>   _currentBall;
    std::shared_ptr<Camera> _currentCamera;
    std::shared_ptr<Star>   _currentStar;
    Rendering _renderingEngine;

    static std::shared_ptr<Map> loadMap(size_t mapNumber);
};

#endif /* Controller_h */
