//
// Created by Sebastien Morgenthaler on 10/10/2022.
//

#ifndef JUMPERBALLAPPLICATION_CHRONOMETER_H
#define JUMPERBALLAPPLICATION_CHRONOMETER_H

#include "system/Types.h"

class Chronometer;

using Chronometer_sptr = std::shared_ptr<Chronometer>;
using CstChronometer_sptr = std::shared_ptr<const Chronometer>;

class Chronometer {

public:
    Chronometer();

    float timeSinceCreation() const;

    void reset();

    void update();

    bool isStopped() const;

    void stop();

    void resume();

    using TimePointMs = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::duration<long int, std::ratio<1, 1000> >
    >;

    static TimePointMs getTimePointMSNow() noexcept;

private:

    using DurationMs = std::chrono::duration<long int, std::ratio<1, 1000> >;

    enum class TimeSession {
        Run, Stop
    };

    /**
     * Current session (Run if the chronometer is running, else Stop)
     */
    TimeSession _currentSession;

    /**
     * Updating time point.
     */
    TimePointMs _updatingTimePoint;
    TimePointMs _currentSessionBeginningTimePoint;

    /**
     * Sum of the cumulated previous run sessions since creation.
     * A session if over when the chronometer is stopped.
     */
    float _msPreviousRunSessionsCreation;

    /**
     * Current session time.
     * A session if started when the chronometer is triggered.
     */
    float _msCurrentSession;

    /*
     * Sum of previous sessions and the current one.
     */
    float _msCreation;

    bool _requestStop;
    bool _requestResume;
    bool _isStopped;

    static float getFloatFromDurationMS(const DurationMs &dms) noexcept;

};

#endif //JUMPERBALLAPPLICATION_CHRONOMETER_H