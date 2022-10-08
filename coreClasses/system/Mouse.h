//
// Created by S.Morgenthaler on 08/10/22.
//

#ifndef JUMPERBALLAPPLICATION_MOUSE_H
#define JUMPERBALLAPPLICATION_MOUSE_H

#include "system/Types.h"

class Mouse {

    enum class ScreenDirection {
        North, South, East, West
    };
    using CstScreenDirection_sptr = std::shared_ptr<const ScreenDirection>;

    explicit Mouse(
        const std::function<void()> &northActionFunc,
        const std::function<void()> &southActionFunc,
        const std::function<void()> &eastActionFunc,
        const std::function<void()> &westActionFunc,
        std::function<void(float mouseX, float mouseY)> validateActionFunc,
        std::function<void()> longPressActionFunc
    );

    void press(float posX, float posY);

    void release();

    void update(const JBTypes::timePointMs &updatingTime);

private:

    void pressedMouseUpdate();
    void releasedMouseUpdate();

    struct MouseCoords {
        float xCoord;
        float yCoord;
    };

    struct MouseState {
        std::shared_ptr<MouseCoords> mouseCoords;
        JBTypes::timePointMs updatingTime;
    };

    struct CardinalDistance {
        ScreenDirection direction;
        float distance;
    };

    struct CardinalPoint {
        ScreenDirection direction;
        std::pair<float, float> point;
    };

    const std::unordered_map<ScreenDirection, std::function<void()> > _directionActionFunctions;
    const std::function<void(float mouseX, float mouseY)> _validateActionFunction;
    const std::function<void()> _longPressActionFunction;

    std::shared_ptr<MouseCoords> _mouseCoords;
    MouseState _currentState;
    MouseState _pressingState;
    MouseState _directionDetectionState;
    MouseState _previousState;
    CstScreenDirection_sptr _currentMovementDir;

    static float computeDistance(float x0, float y0, float x1, float y1);
    static const std::vector<CardinalPoint> cardinalsPoints;

};


#endif //JUMPERBALLAPPLICATION_MOUSE_H
