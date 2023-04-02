/*
 * File: Cube.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:18
 */

#include "Cube.h"

Cube::Cube(
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform,
    const std::array<bool, 6> &sides
) :
    GeometricShape(modelTransform, normalsTransform, {}, {}),
    _sides(sides) {
}

Cube::Cube(
    const JBTypes::Color &color,
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform,
    const std::array<bool, 6> &sides
) :
    GeometricShape(modelTransform, normalsTransform, genColors(color), {}),
    _sides(sides) {

}

Cube::Cube(
    const glm::vec3 &customColor,
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform,
    const std::array<bool, 6> &sides
) :
    GeometricShape(modelTransform, normalsTransform, {customColor}, {}),
    _sides(sides) {
}


Cube::Cube(std::vector<glm::vec3> &&customColors,
           const glm::mat4 &modelTransform,
           const glm::mat4 &normalsTransform,
           const std::array<bool, 6> &sides
) :
    GeometricShape(modelTransform, normalsTransform, std::move(customColors), {}),
    _sides(sides) {
}

std::vector<glm::vec3> Cube::getBasicPosCube(const std::array<bool, 6> &sides) {
    return getBasicElements(getBasicPosFullyCube(), sides);
}

std::vector<glm::vec3> Cube::getBasicNormalsCube(const std::array<bool, 6> &sides) {
    return getBasicElements(getBasicNormalsFullyCube(), sides);
}

std::vector<glm::vec3> Cube::getBasicColorsCube(const std::array<bool, 6> &sides) {
    return getBasicElements(getBasicColorsFullyCube(), sides);
}

template<typename T>
std::vector<T> Cube::getBasicElements
    (const std::vector<T> &basicElements,
     const std::array<bool, 6> &sides) {
    constexpr size_t vecsPerFace = 6;
    constexpr size_t nbFaces = 6;
    bool isValid;
    if (basicElements.size() != vecsPerFace * nbFaces) {
        std::cerr << "Error: your buffer containing cube information " <<
                  "not the good size (yours : " << basicElements.size() <<
                  " vecs, required : " << vecsPerFace * nbFaces << std::endl <<
                  "returns vector of 0.f ..." << std::endl;
        isValid = false;
    } else {
        isValid = true;
    }
    std::vector<T> elements{};
    for (unsigned int i = 0; i < sides.size(); ++i) {
        if (sides.at(i) && isValid) {
            elements.insert(
                elements.end(),
                basicElements.begin() + vecsPerFace * i,
                basicElements.begin() + vecsPerFace * (i + 1)
            );
        } else if (sides.at(i) && !isValid) {
            for (unsigned int j = 0; j < vecsPerFace; ++j) {
                elements.push_back(T(0.f));
            }
        }
    }
    return elements;
}

std::vector<glm::vec3> Cube::genPositions() const {
    return getBasicPosCube(_sides);
}

std::vector<glm::vec3> Cube::genNormals() const {
    return getBasicNormalsCube(_sides);
}

std::vector<glm::vec3> Cube::genColors(const std::vector<glm::vec3> &colors) const {
    if (colors.size() > 1) {
        return getBasicElements(colors, _sides);
    }
    if (colors.size() == 1) {
        constexpr size_t numberOfFloats = 108;
        return getBasicElements(
            GeometricShape::createCustomColorBuffer(colors.at(0), numberOfFloats),
            _sides
        );
    }
    return getBasicColorsCube(_sides);
}

std::vector<glm::vec3> Cube::genColors(const JBTypes::Color &color) {
    if (color == JBTypes::Color::Blue) {
        return Utility::GLfloatListToGlmVec3(
            {
                // Face 1
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f,
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                // Face 2
                0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                // Face 6
                0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                // Face 5
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f,
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                // Face 4
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f,
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                // Face 3
                0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f,
                0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.f, 1.f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f
            }
        );
    }
    if (color == JBTypes::Color::Orange) {
        return Utility::GLfloatListToGlmVec3(
            {
                // Face 1
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                // Face 2
                1.f * 1.5f, 0.f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                // Face 6
                1.f * 1.5f, 0.f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                // Face 5
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                // Face 4
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                // Face 3
                1.f * 1.5f, 0.f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f, 1.f * 1.5f, 0.f, 0.f,
                0.8f * 1.5f, 0.8f * 1.5f, 0.f
            }
        );
    }
    if (color == JBTypes::Color::Red) {
        return Utility::GLfloatListToGlmVec3(
            {
                // Face 1
                0.6f, 0.5f, 0.4f, 0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f,
                0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f,
                // Face 2
                1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f, 0.6f, 0.5f, 0.4f,
                0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f,
                // Face 6
                1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f, 0.6f, 0.5f, 0.4f,
                0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f,
                // Face 5
                0.6f, 0.5f, 0.4f, 0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f,
                0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f,
                // Face 4
                0.6f, 0.5f, 0.4f, 0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f,
                0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f,
                // Face 3
                1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f, 0.6f, 0.5f, 0.4f,
                0.6f, 0.5f, 0.4f, 1.f, 0.f, 0.f, 0.6f, 0.5f, 0.4f
            }
        );
    }
    if (color == JBTypes::Color::Green) {
        return Utility::GLfloatListToGlmVec3(
            {
                // Face 1
                0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f,
                0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f,
                // Face 2
                0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f,
                0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f,
                // Face 6
                0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f,
                0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f,
                // Face 5
                0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f,
                0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f,
                // Face 4
                0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f,
                0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f,
                // Face 3
                0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f,
                0.f, 0.8f, 0.f, 0.f, 0.4f, 0.f, 0.f, 0.8f, 0.f
            }
        );
    }
    if (color == JBTypes::Color::Purple) {
        return Utility::GLfloatListToGlmVec3(
            {

                // Face 1
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f,
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                // Face 2
                0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                // Face 6
                0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                // Face 5
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f,
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                // Face 4
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f,
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                // Face 3
                0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f,
                0.8f * 1.5f, 0.f, 0.8f * 1.5f, 0.3f, 0.f, 0.5f, 0.8f * 1.5f, 0.f, 0.8f * 1.5f
            }
        );
    }
    const auto getColorVec = [](const std::vector<GLfloat> &color) {
        std::vector<GLfloat> colorFloats{};
        for (size_t i = 0; i < 36; ++i) {
            colorFloats.insert(colorFloats.end(), color.begin(), color.end());
        }
        return Utility::GLfloatListToGlmVec3(colorFloats);
    };

    if (color == JBTypes::Color::Yellow) {
        return getColorVec({1.f, 215.f / 255.f, 0.f});
    }

    if (color == JBTypes::Color::Black) {
        return getColorVec({0.f, 0.f, 0.f});
    }

    return getBasicColorsFullyCube();
}

std::vector<glm::vec3> Cube::getBasicPosFullyCube() {
    // order : 1 2 6 5 4 3
    return Utility::GLfloatListToGlmVec3(
        {
            // Face 1
            -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
            // Face 2
            -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
            // Face 6
            0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
            // Face 5
            -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
            // Face 4
            -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
            // Face 3
            -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f
        }
    );
}

std::vector<glm::vec3> Cube::getBasicNormalsFullyCube() {
    return Utility::GLfloatListToGlmVec3(
        {
            // Face 1
            0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f,
            0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f,
            // Face 2
            0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
            0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
            // Face 6
            1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
            1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
            // Face 5
            -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f,
            -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f,
            // Face 4
            0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
            0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
            // Face 3
            0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f,
            0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f
        }
    );
}

std::vector<glm::vec3> Cube::getBasicColorsFullyCube() {
    return Utility::GLfloatListToGlmVec3(
        {

            // Face 1
            0.5f, 0.f, 0.f, 0.5f, 0.f, 0.f, 0.5f, 0.f, 0.f,
            0.5f, 0.f, 0.f, 0.5f, 0.f, 0.f, 0.5f, 0.f, 0.f,
            // Face 2
            0.f, 0.5f, 0.f, 0.f, 0.5f, 0.f, 0.f, 0.5f, 0.f,
            0.f, 0.5f, 0.f, 0.f, 0.5f, 0.f, 0.f, 0.5f, 0.f,
            // Face 6
            0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f,
            0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f,
            // Face 5
            0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f,
            0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f,
            // Face 4
            0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f,
            0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f,
            // Face 3
            0.f, 0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.8f,
            0.f, 0.f, 0.8f, 0.f, 0.f, 0.8f, 0.f, 0.f, 0.8f
        }
    );
}

