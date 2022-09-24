//
// Created by S.Morgenthaler on 24/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_SCROLLABLEPAGE_H
#define JUMPERBALLAPPLICATION_SCROLLABLEPAGE_H
#include "Page.h"


class ScrollablePage: public Page {

public:

    explicit ScrollablePage(
        Player_sptr &&player,
        float height
    );

    float localPosY() const override;

    void update(bool isPressed, float screenPosY) override;


private:
    constexpr static float decelerationCoefficient = 10.f; // pagePourcentage /s^2
    const float _height;
    float _localPosY; // Page position
    float _localPressedPosY;
    float _localReleasedPosY;
    bool _isPressed;
    float _pressedScreenPosY;
    JBTypes::timePointMs _lastUpdate;
    std::list<slideState> _lastSwipeUpdates;
    float _releaseVelocity;
};


#endif //JUMPERBALLAPPLICATION_SCROLLABLEPAGE_H
