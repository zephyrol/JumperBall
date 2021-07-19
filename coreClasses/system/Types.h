/*
 * File: Types.h
 * Author: Morgenthaler S
 *
 * Created on 6 octobre 2019, 09:49
 */

#ifndef TYPES_H
#define TYPES_H
#define __STDC_WANT_LIB_EXT1__ 1
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <array>
#include <memory>
#include <chrono>
#include <mutex>
#define EPSILON_F 0.0001f
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 3.14159265358979323846 / 2.
#endif


namespace JBTypes {

enum class Direction { North, South, East, West, Up, Down };

enum class Color { None, Red, Green, Blue, Yellow };

using Dir = Direction;

struct vec3f { float x; float y; float z; };
struct vec2f { float x; float y; };

using vec3ui = std::array <unsigned int, 3>;
using timePointMs = std::chrono::time_point <
    std::chrono::system_clock,
    std::chrono::duration <
        long int, std::ratio <1, 1000>
        >
    >;
using durationMs = std::chrono::duration <long int, std::ratio <1, 1000> >;
struct Quaternion { JBTypes::vec3f v; float w; };
}

namespace JBTypesMethods {
JBTypes::vec3f directionAsVector(JBTypes::Dir dir);
JBTypes::Dir integerAsDirection(unsigned int number);
JBTypes::Dir charAsDirection(unsigned char dirChar);
JBTypes::Dir vectorAsDirection(const JBTypes::vec3f& vec);
unsigned int directionAsInteger(JBTypes::Dir dir);

JBTypes::timePointMs getTimePointMSNow() noexcept;

float getTimeSecondsSinceTimePoint(const JBTypes::timePointMs& timePoint) noexcept;

float getFloatFromDurationMS(const JBTypes::durationMs& dms);

JBTypes::timePointMs getTimePointMsFromTimePoint(
    const std::chrono::time_point <std::chrono::system_clock>& timePoint
    ) noexcept;


float dot(const JBTypes::vec3f& a, const JBTypes::vec3f& b);
JBTypes::vec3f cross(const JBTypes::vec3f& a, const JBTypes::vec3f& b);
JBTypes::vec3f add(const JBTypes::vec3f& a, const JBTypes::vec3f& b);
JBTypes::vec3f scalarApplication(float scalar, const JBTypes::vec3f& vec);
float distance(const JBTypes::vec3f& a, const JBTypes::vec3f& b);
float length(const JBTypes::vec3f& vec);
JBTypes::vec3f normalize(const JBTypes::vec3f& vec);

JBTypes::Quaternion createQuaternion(const JBTypes::vec3f& v, float w);
JBTypes::Quaternion multiply(const JBTypes::Quaternion& q1, const JBTypes::Quaternion& q2);
JBTypes::Quaternion q2q1(
    const JBTypes::Quaternion& q1q2,
    const JBTypes::Quaternion& q1,
    const JBTypes::Quaternion& q2
    );
JBTypes::Quaternion inverse(const JBTypes::Quaternion& q);
JBTypes::Quaternion createRotationQuaternion(const JBTypes::vec3f& axis, float angle);

JBTypes::vec3f rotateVector(const JBTypes::vec3f& v, const JBTypes::Quaternion& q);

void displayInstallError();

std::string colorToString(const JBTypes::Color color);

template<typename K, typename V> std::map <K, V> createMap(std::vector <K> keys, std::vector <V> values);
}

#endif /* TYPES_H */
