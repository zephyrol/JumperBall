//
// Created by S.Morgenthaler on 08/10/22.
//

#ifndef JUMPERBALLAPPLICATION_MOUSE_H
#define JUMPERBALLAPPLICATION_MOUSE_H

#include "Chronometer.h"
#include "system/Types.h"

class Mouse {
   public:
    enum class ScreenDirection { North, South, East, West };

    struct MouseCoords {
        const float xCoord;
        const float yCoord;
    };

    explicit Mouse(const std::function<void()>& northActionFunc,
                   const std::function<void()>& southActionFunc,
                   const std::function<void()>& eastActionFunc,
                   const std::function<void()>& westActionFunc,
                   float northActionRepeatingDuration,
                   float southActionRepeatingDuration,
                   float eastActionRepeatingDuration,
                   float westActionRepeatingDuration,
                   std::function<void(float mouseX, float mouseY)> validateActionFunc,
                   std::function<void()> releaseFunction);

    void press(float posX, float posY);

    void release();

    bool isPressed() const;

    float currentXCoord() const;

    float currentYCoord() const;

    void update(const Chronometer::TimePointMs& updatingTime);

    std::shared_ptr<const Mouse::MouseCoords> getMouseCoords() const;

   private:
    void pressedMouseUpdate(const Chronometer::TimePointMs& updatingTime);

    void releasedMouseUpdate(const Chronometer::TimePointMs& updatingTime);

    void executeDirectionActionFunction(const Chronometer::TimePointMs& updatingTime);

    struct CardinalDistance {
        ScreenDirection direction;
        float distance;
    };

    struct CardinalPoint {
        ScreenDirection direction;
        std::pair<float, float> point;
        float weight;
    };

    struct MovementCircle {
        const std::unique_ptr<ScreenDirection> movement;
        const MouseCoords mouseCoords;
        const Chronometer::TimePointMs creationTime;
    };

    const std::vector<std::function<void()> > _directionActionFunctions;
    const std::vector<float> _directionActionRepeatingDurations;
    const std::function<void(float mouseX, float mouseY)> _validateActionFunction;
    const std::function<void()> _releaseFunction;

    std::shared_ptr<MouseCoords> _mouseCoords;
    std::unique_ptr<MovementCircle> _movementCircle;
    std::unique_ptr<Chronometer::TimePointMs> _directionActionTimePoint;
    bool _isPressed;

    static float computeDistance(float x0, float y0, float x1, float y1);

    static const std::vector<CardinalPoint> cardinalsPoints;
};

#endif  // JUMPERBALLAPPLICATION_MOUSE_H
