/*
 * File: Utility.cpp
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 15:45
 */

#include "Utility.h"
#include <cmath>
#include <fstream>


std::vector<glm::vec3> Utility::GLfloatListToGlmVec3(
    const std::vector<GLfloat> &list) {
    std::vector<glm::vec3> vecList;
    if ((list.size() % 3) != 0) {
        std::cerr << "Error ... Trying to convert a vector with a wrong size"
                  << std::endl;
    } else {
        for (size_t i = 0; i < list.size(); i += 3) {
            vecList.emplace_back(list.at(i), list.at(i + 1), list.at(i + 2));
        }
    }
    return vecList;
}

std::vector<glm::vec2> Utility::GLfloatListToGlmVec2(
    const std::vector<GLfloat> &list) {
    std::vector<glm::vec2> vecList;
    if ((list.size() % 2) != 0) {
        std::cerr << "Error ... Trying to convert a vector with a wrong size"
                  << std::endl;
    } else {
        for (size_t i = 0; i < list.size(); i += 2) {
            vecList.emplace_back(list.at(i), list.at(i + 1));
        }
    }
    return vecList;
}

glm::mat4 Utility::rotationUpToDir(JBTypes::Dir dir) {

    switch (dir) {
        case JBTypes::Dir::North:
            return glm::rotate(-JBTypes::pi2, glm::vec3(1.f, 0.f, 0.f));
        case JBTypes::Dir::South:
            return glm::rotate(JBTypes::pi2, glm::vec3(1.f, 0.f, 0.f));
        case JBTypes::Dir::East:
            return glm::rotate(-JBTypes::pi2, glm::vec3(0.f, 0.f, 1.f));
        case JBTypes::Dir::West:
            return glm::rotate(JBTypes::pi2, glm::vec3(0.f, 0.f, 1.f));
        case JBTypes::Dir::Up:
            return glm::mat4(1.f);
        case JBTypes::Dir::Down:
            return glm::rotate(JBTypes::pi, glm::vec3(1.f, 0.f, 0.f));
        default:
            return glm::mat4(1.f);
    }
}


float Utility::evalGauss1D(float x, float sigma) {
    return static_cast <float>(exp(
        (-pow(x, 2.)) / (2. * pow(sigma, 2.))) / (sqrt(2. * static_cast<double>(JBTypes::pi) * pow(sigma, 2.))
                               ));
}

std::vector<GLfloat> Utility::genGaussBuffer(size_t patchSize, float sigma) {
    std::vector<GLfloat> gaussBuffer;
    int patchSizeInteger = static_cast <int>(patchSize);
    for (int i = -patchSizeInteger / 2; i <= patchSizeInteger / 2; i++) {
        gaussBuffer.push_back(evalGauss1D(static_cast <float>(i), sigma));
    }
    return gaussBuffer;
}

glm::vec3 Utility::colorAsVec3(const JBTypes::Color &color) {
    switch (color) {
        case JBTypes::Color::None:
            return {0.f, 0.f, 0.f};
            break;
        case JBTypes::Color::Red:
            return {1.f, 0.f, 0.f};
            break;
        case JBTypes::Color::Green:
            return {0.f, 1.f, 0.f};
            break;
        case JBTypes::Color::Blue:
            return {0.f, 0.f, 1.f};
            break;
        case JBTypes::Color::Yellow:
            return {1.f, 1.f, 0.f};
            break;
        case JBTypes::Color::Orange:
            return {1.f, 0.4f, 0.f};
            break;
        case JBTypes::Color::Purple:
            return {0.4f, 0.f, 0.4f};
            break;
        case JBTypes::Color::White:
            return {1.f, 1.f, 1.f};
            break;
        case JBTypes::Color::Black:
            return {0.f, 0.f, 0.f};
            break;
        case JBTypes::Color::ShinyRed:
            return {1.f, 0.f, 0.f};
            break;
        case JBTypes::Color::ShinyGreen:
            return {0.f, 1.f, 0.f};
            break;
        case JBTypes::Color::ShinyBlue:
            return {0.f, 0.f, 1.f};
            break;
        default:
            return {0.f, 0.f, 0.f};
            break;
    }
    return {0.f, 0.f, 0.f};
}

GLubyte Utility::convertToOpenGLFormat(unsigned char uChar) {
    return static_cast <GLubyte>(uChar);
}

float Utility::menuPositionToOpenGLScreenFormat(float value) {
    return 2.f * value - 1.f;
}


