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
    
    enum class Status {Pressed,Released};
    enum class Button {Up, Down, Left, Right, Escape, Validate};
    
    Controller(Player& player);
    void interaction(const Button& button, const Status& status);
    void assignBall ( const std::shared_ptr<Ball>& ball );
    
private:

    void manageUp(const Status& status);
    void manageDown(const Status& status);
    void manageRight(const Status& status);
    void manageLeft(const Status& status);
    void manageEscape(const Status& status);
    void manageValidate(const Status& status);
    
    //Mouse/TouchPad/TouchScreen
    void pressMouse ( float posX, float posY );
    void updateMouse ( float posX, float posY );
    void releaseMouse ( float posX, float posY );
    
    Player&   _player;
    std::shared_ptr<Ball>  _ball;
    
    std::map<Button, Status> _buttonsStatuts;

};

#endif /* Controller_h */
