#include "Page.h"
#include <math.h>
#include <algorithm>


/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

Page::Page(const Page_sptr &parent,
           NodeMessageAssociations&& nodeToMessage,
           const Page::PageFormat &pageFormat,
           const Page::EscapeAnswer &escapeAnswer,
           float height,
           bool visibleOnParent) :
    _pageFormat(pageFormat),
    _bridges{},
    _nodeToMessage(std::move(nodeToMessage)),
    _labels{},
    _cstLabels{},
    _children{},
    _parent(parent),
    _visibleOnParent(visibleOnParent),
    _height(height),
    _escapeAnswer(escapeAnswer),
    _localPosY(0.f),
    _localPressedPosY(0.f),
    _localReleasedPosY(0.f),
    _isPressed(false),
    _pressedScreenPosY(0.f),
    _lastUpdate{},
    _lastSwipeUpdates{},
    _releaseVelocity(0.f) {
}

std::weak_ptr<const Page> Page::parent() const {
    return _parent;
}

std::weak_ptr<Page> Page::parent() {
    return _parent;
}

float Page::localPosY() const {
    return _localPosY;
}

void Page::setBridges(std::unordered_map<Label_sptr, Page_sptr> &&bridges) {
    _bridges = std::move(bridges);
    _labels = createLabels();
    _cstLabels = createCstLabels();
    _children = createChildren();
}

bool Page::visibleOnParent() const {
    return _visibleOnParent;
}

Page_sptr Page::child(const Label_sptr &label) {
    if (_bridges.find(label) != _bridges.end()) {
        return _bridges.at(label);
    } else {
        return nullptr;
    }
}

const vecCstLabel_sptr &Page::labels() const {
    return _cstLabels;
}

float Page::height() const {
    return _height;
}

Label_sptr Page::matchedLabel(float x, float y) {
    // TODO
    return nullptr;
}

void Page::update(bool isPressed, float screenPosY) {

    if (JBTypesMethods::floatsEqual(_height, 1.f)) {
        return;
    }

    const auto now = JBTypesMethods::getTimePointMSNow();
    _lastUpdate = now;

    // Press cases
    if (isPressed && !_isPressed) {
        _pressedScreenPosY = screenPosY;
        _localPressedPosY = _localPosY;
        _isPressed = true;
        _lastSwipeUpdates.clear();
    }

    if (_isPressed) {
        constexpr float thresholdDeltaT = 0.1f; // 100 ms
        std::list<slideState>::iterator it;
        for (
            it = _lastSwipeUpdates.begin();
            it != _lastSwipeUpdates.end() &&
            JBTypesMethods::getFloatFromDurationMS(now - it->first) < thresholdDeltaT;
            ++it
            ) {
        }

        _lastSwipeUpdates.erase(it, _lastSwipeUpdates.end());
        _lastSwipeUpdates.push_front({now, screenPosY});
        _localPosY = _localPressedPosY + (screenPosY - _pressedScreenPosY);
    }

    // Release cases
    if (!isPressed && _isPressed && !_lastSwipeUpdates.empty()) {
        const slideState &lastSlideState = _lastSwipeUpdates.front();
        const slideState &olderSlideState = _lastSwipeUpdates.back();
        float deltaT = JBTypesMethods::getFloatFromDurationMS(
            lastSlideState.first - olderSlideState.first);

        // the velocity is the position derivative (pourcentagePage / ms)
        _releaseVelocity = (lastSlideState.second - olderSlideState.second) / deltaT;

        _localReleasedPosY = _localPosY;
        _isPressed = false;
    }
    if (!_isPressed && !_lastSwipeUpdates.empty()) {
        const slideState &lastSlideState = _lastSwipeUpdates.front();
        const float t = JBTypesMethods::getFloatFromDurationMS(
            now - lastSlideState.first);
        const float deceleration = decelerationCoefficient * powf(t, 2.f) / 2.f;

        if (_releaseVelocity > 0.f && t < -(_releaseVelocity) / (2.f * -decelerationCoefficient / 2.f)) {
            _localPosY = -deceleration + _releaseVelocity * t + _localReleasedPosY;
        } else if (
            _releaseVelocity < 0.f &&
            t < -(_releaseVelocity) / (decelerationCoefficient)
            ) {
            _localPosY = deceleration + _releaseVelocity * t + _localReleasedPosY;
        }

        if (_localPosY < 0.f) _localPosY = 0.f;
        else if (_localPosY > _height - 1.f) _localPosY = _height - 1.f;
    }

}

vecLabel_sptr Page::createLabels() const {
    vecLabel_sptr labels;
    for (const auto &bridge: _bridges) {
        labels.push_back(bridge.first);
    }
    return labels;
}

vecCstLabel_sptr Page::createCstLabels() const {
    vecCstLabel_sptr cstLabels;
    for (const auto &label: _labels) {
        cstLabels.push_back(label);
    }
    return cstLabels;
}


const Page::EscapeAnswer &Page::getEscapeAnswer() const {
    return _escapeAnswer;
}

std::vector<Page_sptr> Page::createChildren() const {
    std::vector<Page_sptr> children;
    for (const auto &bridge: _bridges) {
        if (bridge.second) {
            children.push_back(bridge.second);
        }
    }
    return children;
}


SceneElement::GlobalState Page::getGlobalState() const {
    return SceneElement::GlobalState::United;
}

void Page::resize(float screenRatio) {
    for (const auto &label: _labels) {
        // label->resize(screenRatio); // TODO
    }
}

const Page::NodeMessageAssociations &Page::nodeToMessage() const {
    return _nodeToMessage;
}

