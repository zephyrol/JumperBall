//
// Created by S.Morgenthaler on 10/10/2022.
//

#ifndef JUMPERBALLAPPLICATION_CHRONOMETER_H
#define JUMPERBALLAPPLICATION_CHRONOMETER_H

#include "system/Types.h"

class Chronometer;
using Chronometer_sptr = std::shared_ptr<Chronometer>;
using CstChronometer_sptr = std::shared_ptr<const Chronometer>;

/**
 * Ms Chronometer
 */
class Chronometer {

public:
    /**
     * @param autoStart If true, the chronometer is running directly after its creation
     * and after each reset.
     */
    explicit Chronometer(bool autoStart);

    /**
     * Get chronometer time in s.
     * @return Passed time in s.
     */
    float getTime() const;

    /**
     * Reset the chronometer to 0
     */
    void reset();

    using TimePointMs = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::duration<long int, std::ratio<1, 1000> >
    >;

    /**
     * Update the chronometer.
     * @param updatingTime Time point representing the updating time
     */
    void update(const Chronometer::TimePointMs& updatingTime);

    /**
     * Freeze the chronometer.
     */
    void stop();

    /**
     * Resume the chronometer.
     */
    void resume();

    static TimePointMs getTimePointMSNow() noexcept;

    using DurationMs = std::chrono::duration<long int, std::ratio<1, 1000> >;

    static float getFloatFromDurationMS(const DurationMs &dms) noexcept;

private:

    enum class TimeSession {
        Run, Stop
    };

    enum class Request {
        Resume, Stop, None
    };

    const bool _autoStart;

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
     * A session is over when the chronometer is stopped.
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
    float _msTime;

    Request _request;

};

#endif //JUMPERBALLAPPLICATION_CHRONOMETER_H
