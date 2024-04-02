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
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/detail/type_quat.hpp>


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
}

namespace JBTypesMethods {

    glm::vec3 directionAsVector(JBTypes::Dir dir);

    glm::vec3 rotationVectorUpToDir(const JBTypes::Dir &dir);

    JBTypes::Dir integerAsDirection(unsigned int number);

    JBTypes::Dir charAsDirection(unsigned char dirChar);

    JBTypes::Dir vectorAsDirection(const glm::vec3 &vec);

    unsigned int directionAsInteger(JBTypes::Dir dir);

    std::array<bool, 6> strDirAsArray(const std::string &directions);

    void displayInstallError();

    std::string colorToString(const JBTypes::Color &color);

    JBTypes::Color colorToShiny(const JBTypes::Color &color);

    JBTypes::Color charAsColor(unsigned char charColor);

    glm::i32vec3 directionAsVectorInt(JBTypes::Dir dir);

    bool floatsEqual(float a, float b);

}

#endif /* TYPES_H */
