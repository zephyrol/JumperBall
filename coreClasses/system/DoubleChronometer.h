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
    DoubleChronometer();

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

private:

    const Chronometer &getFirst() const;

    const Chronometer &getSecond() const;

    const DoubleChronometer &getConstThis();

    Chronometer &getFirst();

    Chronometer &getSecond();

    std::array<Chronometer, 2> _chronometers;

};


#endif //JUMPERBALLAPPLICATION_DOUBLECHRONOMETER_H
