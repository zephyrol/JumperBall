//
// Created by S.Morgenthaler on 24/12/2022.
//

#ifndef JUMPERBALLAPPLICATION_DOUBLECHRONOMETER_H
#define JUMPERBALLAPPLICATION_DOUBLECHRONOMETER_H

#include "Chronometer.h"

class DoubleChronometer;

using DoubleChronometer_sptr = std::shared_ptr<DoubleChronometer>;
using CstDoubleChronometer_sptr = std::shared_ptr<const DoubleChronometer>;

class DoubleChronometer {

public:
    DoubleChronometer(bool firstAutoStart, bool secondAutoStart);

    /**
     * Update both chronometers.
     */
    void update();

    /**
     * Reset both chronometers to 0
     */
    void reset();

    /**
     * Stop the first chronometer
     */
    void stopFirst();

    /**
     * Stop the second chronometer
     */
    void stopSecond();

    /**
     * Stop both chronometers
     */
    void stopBoth();

    /**
     * Resume the first chronometer
     */
    void resumeFirst();

    /**
     * Resume the second chronometer
     */
    void resumeSecond();

    /**
     * Resume both chronometers
     */
    void resumeBoth();

    /**
     * Get time on the first timer.
     */
    float getFirstTimer() const;

    /**
     * Get time on the second timer.
     */
    float getSecondTimer() const;

    const CstChronometer_sptr &first() const;

    const CstChronometer_sptr &second() const;

    const Chronometer_sptr &first();

    const Chronometer_sptr &second();

private:

    const std::array<Chronometer_sptr, 2> _chronometers;
    const Chronometer_sptr &_first;
    const Chronometer_sptr &_second;
    const CstChronometer_sptr _cstFirst;
    const CstChronometer_sptr _cstSecond;

};


#endif //JUMPERBALLAPPLICATION_DOUBLECHRONOMETER_H
