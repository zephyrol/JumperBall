/*
 * File: Types.h
 * Author: Morgenthaler S
 *
 * Created on 6 octobre 2019, 09:49
 */

#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <string>
#include <cmath>
#include <iterator>
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <array>
#include <memory>
#include <chrono>
#include <numeric>

#define EPSILON_F 0.0001f
namespace JBTypes {

    constexpr auto pi = 3.14159265358979323846f;
    constexpr auto pi2 = pi / 2.f;

    using FileContent = std::map<std::string, std::string>;
    enum class Direction {
        North, South, East, West, Up, Down
    };

    enum class Color {
        None,
        Red,
        Green,
        Blue,
        Yellow,
        Orange,
        Purple,
        White,
        Black,
        ShinyRed,
        ShinyGreen,
        ShinyBlue
    };

    using Dir = Direction;

    struct vec3f {
        float x;
        float y;
        float z;
    };
    struct vec2f {
        float x;
        float y;
    };

    using vec3ui = std::array<unsigned int, 3>;
    using vec3i = std::array<int, 3>;

    struct Quaternion {
        JBTypes::vec3f v;
        float w;
    };
}

namespace JBTypesMethods {

    JBTypes::vec3f directionAsVector(JBTypes::Dir dir);

    JBTypes::vec3f rotationVectorUpToDir(const JBTypes::Dir &dir);

    JBTypes::Dir integerAsDirection(unsigned int number);

    JBTypes::Dir charAsDirection(unsigned char dirChar);

    JBTypes::Dir vectorAsDirection(const JBTypes::vec3f &vec);

    unsigned int directionAsInteger(JBTypes::Dir dir);

    std::array<bool, 6> strDirAsArray(const std::string &directions);

    float dot(const JBTypes::vec3f &a, const JBTypes::vec3f &b);

    JBTypes::vec3f cross(const JBTypes::vec3f &a, const JBTypes::vec3f &b);

    JBTypes::vec3f add(const JBTypes::vec3f &a, const JBTypes::vec3f &b);

    JBTypes::vec3f scalarApplication(float scalar, const JBTypes::vec3f &vec);

    float distance(const JBTypes::vec3f &a, const JBTypes::vec3f &b);

    float length(const JBTypes::vec3f &vec);

    JBTypes::vec3f normalize(const JBTypes::vec3f &vec);

    JBTypes::Quaternion createQuaternion(const JBTypes::vec3f &v, float w);

    JBTypes::Quaternion multiply(const JBTypes::Quaternion &q1, const JBTypes::Quaternion &q2);

    JBTypes::Quaternion q2q1(
        const JBTypes::Quaternion &q1q2,
        const JBTypes::Quaternion &q1,
        const JBTypes::Quaternion &q2
    );

    JBTypes::Quaternion inverse(const JBTypes::Quaternion &q);

    JBTypes::Quaternion createRotationQuaternion(const JBTypes::vec3f &axis, float angle);

    JBTypes::vec3f rotateVector(const JBTypes::vec3f &v, const JBTypes::Quaternion &q);

    void displayInstallError();

    std::string colorToString(const JBTypes::Color &color);

    JBTypes::Color colorToShiny(const JBTypes::Color &color);

    JBTypes::Color charAsColor(unsigned char charColor);

    JBTypes::vec3i directionAsVectorInt(JBTypes::Dir dir);

    bool floatsEqual(float a, float b);

}

#endif /* TYPES_H */
