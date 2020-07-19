
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

#ifndef PAGE_H
#define PAGE_H
#include "Label.h"

class Page
{
public:
    
    // Slide state => timepoint and yScreenPosition
    using slideState = std::pair<JumperBallTypes::timePointMs, float>;


    //--CONSTRUCTORS & DESTRUCTORS--//
    Page( const std::vector<std::shared_ptr<const Label> >& labels,
          const std::shared_ptr<const Page>& parent = nullptr,
          bool visibleOnParent = false,
          float height = 1.f);

    
    //-------CONST METHODS--------//
    const std::vector<std::shared_ptr<const Label> >& labels()            const;
    const std::map<std::shared_ptr<const Label>, std::shared_ptr<Page> >&
                                      bridges();
    const std::weak_ptr<const Page> & parent()                            const;
    bool                              visibleOnParent()                   const;


    std::shared_ptr<const Page>       child(float x, float y)             const;

    std::shared_ptr<const Label>      matchedLabel(float x, float y)      const;

    float                             height()                            const;
    float                             localPosY()                         const;

    //----------METHODS-----------//
    void                              addBridge(
                                    const std::shared_ptr<const Label> label,
                                    const std::shared_ptr<Page> page);
    std::shared_ptr<Page>             child(
                                    const std::shared_ptr<const Label>& label);
    void                              updateTimeSlide();
    void                              pressOnPage();
    void                              release();
    void                              update(bool isPressed,
                                             float screenPosY = 0.f);
    
private:

    //-------CONST METHODS--------//
    //--------ATTRIBUTES-----------//

    constexpr static float decelerationCoefficient = 10.f; //    pagePourcentage
                                                          //    / s^2

    const std::vector<std::shared_ptr<const Label> > _labels;
    std::map<std::shared_ptr<const Label>, std::shared_ptr<Page> >
                                      _bridges;
    const std::weak_ptr<const Page>   _parent;
    const bool                        _visibleOnParent;
    const float                       _height;
    float                             _localPosY; // Page position
    float                             _localPressedPosY;
    float                             _localReleasedPosY;
    bool                              _isPressed;
    float                             _pressedScreenPosY;
    JumperBallTypes::timePointMs      _lastUpdate;
    std::array<slideState,2>          _lastSwipeUpdates;
    unsigned int                      _countingUpdates;
    float                             _releaseVelocity;
};

#endif // PAGE_H
