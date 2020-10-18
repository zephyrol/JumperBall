//
//  Controller.hpp
//  JumperBallGraphic
//
//  Created by S Morgenthaler on 25/05/2020.
//

#ifndef Controller_h
#define Controller_h

#include <player/Player.h>
#include "scene/SceneRendering.h"
#include "graphicMenu/MenuRendering.h"

class Controller {
    
public:
    
    //------------TYPES------------//
    enum class Status {Pressed,Released};
    enum class Button {Up, Down, Left, Right, Escape, Validate};
    enum class ScreenDirection {North, South, East, West};
    enum class CurrentFrame { FrameA, FrameB };
    
    
    //--CONSTRUCTORS & DESTRUCTORS--//
    Controller();
    
    //----------METHODS------------//
    void            interactionButtons (const Button& button,
                                     const Status& status);
    void            interactionMouse (const Status& status,
                                     float posX, float posY);

    void            runController();
    void            waitController();

    void            currentMap(const std::shared_ptr<Map>& currentMap);
    const std::shared_ptr<Map> &currentMap() const;

    void currentBall(const std::shared_ptr<Ball>& currentBall);
    const std::shared_ptr<Ball> &currentBall() const;

    const std::shared_ptr<Menu>&  menu() const;
    bool requestToLeave() const;

    
private:
    //-------CONST METHODS--------//
    ScreenDirection nearestDirection(float posX, float posY) const;
    const std::shared_ptr<SceneRendering> &sceneRendering() const;
    const std::shared_ptr<MenuRendering> &menuRendedering() const;

    //----------METHODS------------//
    void            manageUp (const Status& status);
    void            manageDown (const Status& status);
    void            manageRight (const Status& status);
    void            manageLeft (const Status& status);
    void            manageEscape (const Status& status);
    void            manageValidateButton (const Status& status);
    void            runGame(size_t level);
    void            updateRenderingEngine();
    void            updateMenuRendering();
    void            renderRenderingEngine() const;
    void            renderMenuRendering() const;

    //Mouse/TouchPad/TouchScreen
    void            pressMouse (float posX, float posY);
    void            updateMouse (float posX, float posY);
    void            releaseMouse (float posX, float posY);
    void            manageValidateMouse ();

    //--------ATTRIBUTES-----------//
    Player                         _player;
    std::shared_ptr<Menu>          _menu;
    std::map<Button, Status>       _buttonsStatuts;

    
    float                          _mouseCurrentXCoord;
    float                          _mouseCurrentYCoord;
    float                          _mousePressingXCoord;
    float                          _mousePressingYCoord;
    bool                           _mouseIsPressed;
    bool                           _requestToLeave;

    std::shared_ptr<Map>           _map;
    std::shared_ptr<Ball>          _ball;
    std::shared_ptr<Camera>        _camera;
    std::shared_ptr<Star>          _star;

    CurrentFrame _currentFrame;
    std::shared_ptr<SceneRendering> _sceneRenderingFrameA;
    std::shared_ptr<SceneRendering> _sceneRenderingFrameB;
    std::shared_ptr<MenuRendering> _menuRenderingFrameA;
    std::shared_ptr<MenuRendering> _menuRenderingFrameB;

    ParallelTask<void>             _updatingScene;
    ParallelTask<void>             _updatingMenu;

};

#endif /* Controller_h */
