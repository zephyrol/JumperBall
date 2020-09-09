
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

class Page;
using Page_sptr = std::shared_ptr<Page>;
using CstPage_sptr = std::shared_ptr<const Page>;
using vecCstPage_sptr = std::vector<CstPage_sptr>;
using vecPage_sptr = std::vector<Page_sptr>;

class Page
{
public:
    
    // Slide state => timepoint and yScreenPosition
    using slideState = std::pair<JBTypes::timePointMs, float>;


    //--CONSTRUCTORS & DESTRUCTORS--//
    Page(const CstPage_sptr& parent,
         float height = 1.f,
         bool visibleOnParent = false);

    
    //-------CONST METHODS--------//
    //const vecCstLabel_sptr&           labels()                            const;
    //const std::map<CstLabel_sptr, Page_sptr >& bridges()                  const;
    const std::weak_ptr<const Page> & parent()                            const;
    bool                              visibleOnParent()                   const;
    CstPage_sptr                      child(float x, float y)             const;
    CstLabel_sptr                     matchedLabel(float x, float y)      const;
    float                             height()                            const;
    float                             localPosY()                         const;
    void                              setBridges(
            std::map<CstLabel_sptr, Page_sptr>&& bridges);

    //----------METHODS-----------//
    //void                              addBridge(const CstLabel_sptr& label,
                                                //const Page_sptr& page);
    CstPage_sptr                      child(const CstLabel_sptr& label) const;
    Page_sptr                         child(const CstLabel_sptr& label);
    const vecCstLabel_sptr &          labels() const;
    //const std::map<CstLabel_sptr, const CstPage_sptr& > children
    void                              pressOnPage();
    void                              release();
    void                              update(bool isPressed,
                                             float screenPosY = 0.f);
    
private:

    //--------ATTRIBUTES-----------//
    constexpr static float decelerationCoefficient = 10.f; //    pagePourcentage
                                                          //    / s^2

    std::vector<CstLabel_sptr>        createLabels() const;
    std::vector<Page_sptr>            createChildren() const;

    std::map<CstLabel_sptr, Page_sptr > _bridges;
    std::vector<CstLabel_sptr>        _labels;
    std::vector<Page_sptr>            _children;
    const std::weak_ptr<const Page>   _parent;
    const bool                        _visibleOnParent;
    const float                       _height;
    float                             _localPosY; // Page position
    float                             _localPressedPosY;
    float                             _localReleasedPosY;
    bool                              _isPressed;
    float                             _pressedScreenPosY;
    JBTypes::timePointMs              _lastUpdate;
    std::array<slideState,2>          _lastSwipeUpdates;
    unsigned int                      _countingUpdates;
    float                             _releaseVelocity;
};

#endif // PAGE_H
