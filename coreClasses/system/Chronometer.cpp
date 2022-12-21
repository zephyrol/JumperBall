//
// Created by Sebastien Morgenthaler on 10/10/2022.
//

#include "Chronometer.h"

Chronometer::Chronometer():
    _currentSession(Chronometer::TimeSession::Run),
    _updatingTimePoint(),
    _currentSessionBeginningTimePoint(getTimePointMSNow()),
    _msPreviousRunSessionsCreation(0.f),
    _msCurrentSession(0.f),
    _msCreation(0.f),
    _requestStop(false),
    _requestResume(false),
    _isStopped(false)
{
}

Chronometer::TimePointMs Chronometer::getTimePointMSNow() noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
}

float Chronometer::getFloatFromDurationMS(const Chronometer::DurationMs &dms) noexcept {
    const std::chrono::duration<float> durationFloatDifference = dms;
    const float fDifference = durationFloatDifference.count();
    return fDifference;
}

void Chronometer::reset() {
    _currentSessionBeginningTimePoint = _updatingTimePoint;
    _msCreation = 0.f;
    _msPreviousRunSessionsCreation = 0.f;
    _msCurrentSession = 0.f;
}

void Chronometer::update() {
    _updatingTimePoint = getTimePointMSNow();
    _msCurrentSession = getFloatFromDurationMS(_updatingTimePoint - _currentSessionBeginningTimePoint);

    if(_requestStop && _currentSession == TimeSession::Run) {
        _currentSession = TimeSession::Stop;
        _msPreviousRunSessionsCreation += _msCurrentSession;
        _currentSessionBeginningTimePoint = _updatingTimePoint;
        _requestStop = false;
        _isStopped = true;
        return;
    }
    if(_requestResume && _currentSession == TimeSession::Stop) {
        _currentSession = TimeSession::Run;
        _requestResume = false;
        _currentSessionBeginningTimePoint = _updatingTimePoint;
        _isStopped = false;
        return;
    }
    if(_currentSession == TimeSession::Run) {
        _msCreation = _msPreviousRunSessionsCreation + _msCurrentSession;
    }
}

void Chronometer::stop() {
    _requestStop = true;
}

void Chronometer::resume() {
    _requestResume = true;
}

float Chronometer::timeSinceCreation() const {
    return _msCreation;
}

bool Chronometer::isStopped() const {
    return _isStopped;
}
