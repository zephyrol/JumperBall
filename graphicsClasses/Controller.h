//
//  Controller.hpp
//  JumperBallGraphic
//
//  Created by S Morgenthaler on 25/05/2020.
//

#ifndef Controller_h
#define Controller_h

#include <Player.h>
#include <Ball.h>

class Controller {
    
public:
    
    //------------TYPES------------//
    enum class Status {Pressed,Released};
    enum class Button {Up, Down, Left, Right, Escape, Validate};
    enum class ScreenDirection {North, South, East, West};
    
    
    //--CONSTRUCTORS & DESTRUCTORS--//
    Controller(Player& player);
    
    //----------METHODS------------//
    void            interactionButtons (const Button& button,
                                     const Status& status);
    void            interactionMouse (const Status& status,
                                     float posX, float posY);
    
    void            assignBall (const std::shared_ptr<Ball>& ball );
    
    
private:
    //-------CONST METHODS--------//
    ScreenDirection nearestDirection(float posX, float posY) const;
    
    //----------METHODS------------//
    void            manageUp (const Status& status);
    void            manageDown (const Status& status);
    void            manageRight (const Status& status);
    void            manageLeft (const Status& status);
    void            manageEscape (const Status& status);
    void            manageValidate (const Status& status);
    
    //Mouse/TouchPad/TouchScreen
    void            pressMouse (float posX, float posY);
    void            updateMouse (float posX, float posY);
    void            releaseMouse (float posX, float posY);

    //--------ATTRIBUTES-----------//
    Player&         _player;
    std::shared_ptr<Ball> _ball;
    std::map<Button, Status> _buttonsStatuts;
    
    float           _mousePressingXCoord;
    float           _mousePressingYCoord;
    bool            _mouseIsPressed;

};

#endif /* Controller_h */
