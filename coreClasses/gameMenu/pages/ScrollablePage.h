//
// Created by S.Morgenthaler on 24/09/2022.
//

#ifndef JUMPERBALLAPPLICATION_SCROLLABLEPAGE_H
#define JUMPERBALLAPPLICATION_SCROLLABLEPAGE_H

#include "Page.h"
#include "scene/Map.h"


class ScrollablePage : public Page {

public:
    // Slide state => time point and yScreenPosition
    using slideState = std::pair<float, float>;

    explicit ScrollablePage(
        Player_sptr &&player,
        float height
    );


    void update(const Mouse &mouse) override;

    std::string shaderDefine() const override;

    std::vector<std::string> getUniformNames() const override;

    std::vector<float> getUniformValues(const CstMap_sptr& map) const override;

protected:
    float getOffsetY() const;

private:
    constexpr static float decelerationCoefficient = 10.f; // pagePourcentage /s^2
    const CstChronometer_sptr _chronometer;
    const float _height;
    float _localPosY; // Page position
    float _localPressedPosY;
    float _localReleasedPosY;
    bool _isPressed;
    float _pressedScreenPosY;
    std::list<slideState> _lastSwipeUpdates;
    float _releaseVelocity;
};


#endif //JUMPERBALLAPPLICATION_SCROLLABLEPAGE_H
