/*
 * File: Utility.cpp
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 15:45
 */

#include "Utility.h"
#include <math.h>
#include <fstream>


std::string Utility::readFileSrc (const std::string& filePath) {
    // precondition
    if (filePath.size() == 0)
        std::cerr << "Invalid parameter filePath : size() must not be 0."
                  << std::endl;
    // --------------------------------------------------------------------------


    std::string strContent;

    std::string filePathV2 = "bin/" + filePath;
    const std::vector <std::string> fileNames {
        filePath, std::move(filePathV2) };

    bool foundFile = false;
    for (size_t i = 0; i < fileNames.size() && !foundFile; ++i) {
        std::ifstream shaderFile(fileNames.at(i));
        if (shaderFile) {
            std::cout << "Opening file " << fileNames.at(i) << std::endl;
            foundFile = true;

            shaderFile.seekg(0, std::ios::end);
            strContent.reserve(shaderFile.tellg());
            shaderFile.seekg(0, std::ios::beg);
            strContent.assign((std::istreambuf_iterator <char>(shaderFile)),
                              std::istreambuf_iterator <char>());

            // postcondition
            if (strContent.size() == 0)
                std::cout << "Invalid content read strContent(\"" << strContent
                          << "\") : size() must not be 0." << std::endl;
            // ------------------------------------------------------------------
            shaderFile.close();
        }
    }
    if (!foundFile) {
        std::cerr << "Could not open file " << filePath << std::endl;
    }

    return strContent;
}


const glm::mat3 Utility::RGBToXYZ { 2.7689, 1.7517, 1.1302,
                                    1.0000, 4.5907, 0.060100,
                                    0.0000, 0.056508, 5.5943 };

const glm::mat3 Utility::XYZToRGB = glm::inverse(Utility::XYZToRGB);


std::vector <GLfloat> Utility::uniColorsCube (const glm::vec3& color) {
    std::vector <GLfloat> colors;
    for (unsigned int i = 0; i < 36; ++i) {
        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }
    return colors;
}

std::vector <GLfloat> Utility::computeNormals (const std::vector <GLfloat>&
                                               positions) {
    std::vector <GLfloat> normals;
    constexpr unsigned int offsetPointA = 0;
    constexpr unsigned int offsetPointB = 3;
    constexpr unsigned int offsetPointC = 6;

    constexpr unsigned int offsetCoordX = 0;
    constexpr unsigned int offsetCoordY = 1;
    constexpr unsigned int offsetCoordZ = 2;

    constexpr unsigned int coordsPerTriangle = 9;
    constexpr unsigned int pointsPerTriangles = 3;
    for (size_t i = 0; i < positions.size(); i += coordsPerTriangle) {
        glm::vec3 normal(
            glm::cross(
                glm::vec3(
                    positions.at(offsetPointB + offsetCoordX) -
                    positions.at(offsetPointA + offsetCoordX),
                    positions.at(offsetPointB + offsetCoordY) -
                    positions.at(offsetPointA + offsetCoordY),
                    positions.at(offsetPointB + offsetCoordZ) -
                    positions.at(offsetPointA + offsetCoordZ)
                    ),
                glm::vec3(
                    positions.at(offsetPointC + offsetCoordX) -
                    positions.at(offsetPointB + offsetCoordX),
                    positions.at(offsetPointC + offsetCoordY) -
                    positions.at(offsetPointB + offsetCoordY),
                    positions.at(offsetPointC + offsetCoordZ) -
                    positions.at(offsetPointB + offsetCoordZ)
                    )));

        glm::normalize(normal);
        for (unsigned int j = 0; j < pointsPerTriangles; j++) {

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
        }
    }
    return normals;
}



std::vector <glm::vec3> Utility::GLfloatListToGlmVec3 (
    const std::vector <GLfloat>& list) {
    std::vector <glm::vec3> vecList;
    if ((list.size() % 3) != 0) {
        std::cerr << "Error ... Trying to convert a vector with a wrong size"
                  << std::endl;
    } else {
        for (size_t i = 0; i < list.size(); i += 3) {
            vecList.push_back(glm::vec3(list.at(i), list.at(i + 1), list.at(i + 2)));
        }
    }
    return vecList;
}

std::vector <glm::vec2> Utility::GLfloatListToGlmVec2 (
    const std::vector <GLfloat>& list) {
    std::vector <glm::vec2> vecList;
    if ((list.size() % 2) != 0) {
        std::cerr << "Error ... Trying to convert a vector with a wrong size"
                  << std::endl;
    } else {
        for (size_t i = 0; i < list.size(); i += 2) {
            vecList.push_back(glm::vec2(list.at(i), list.at(i + 1)));
        }
    }
    return vecList;
}



glm::mat4 Utility::rotationUpToDir (JBTypes::Dir dir) {

    glm::mat4 rotationMatrix;
    constexpr float fPI = static_cast <float>(M_PI);
    constexpr float fPI2 = static_cast <float>(M_PI_2);

    switch (dir) {
    case JBTypes::Dir::North:
        rotationMatrix = glm::rotate(-fPI2, glm::vec3(1.f, 0.f, 0.f));
        break;
    case JBTypes::Dir::South:
        rotationMatrix = glm::rotate(+fPI2, glm::vec3(1.f, 0.f, 0.f));
        break;
    case JBTypes::Dir::East:
        rotationMatrix = glm::rotate(-fPI2, glm::vec3(0.f, 0.f, 1.f));
        break;
    case JBTypes::Dir::West:
        rotationMatrix = glm::rotate(fPI2, glm::vec3(0.f, 0.f, 1.f));
        break;
    case JBTypes::Dir::Up:
        rotationMatrix = glm::mat4(1.f);
        break;
    case JBTypes::Dir::Down:
        rotationMatrix = glm::rotate(fPI, glm::vec3(1.f, 0.f, 0.f));
        break;
    default:
        rotationMatrix = glm::mat4(1.f);
        break;
    }

    return rotationMatrix;
}


float Utility::evalGauss1D (float x, float sigma) {
    return static_cast <float>(
        exp((-pow(x, 2.)) / (2. * pow(sigma, 2.))) / (sqrt(2. * M_PI * pow(sigma, 2.))));
}


std::vector <GLfloat> Utility::genGaussBuffer (size_t patchSize, float sigma) {
    std::vector <GLfloat> gaussBuffer;
    int patchSizeInteger = static_cast <int>(patchSize);
    for (int i = -patchSizeInteger / 2; i <= patchSizeInteger / 2; i++) {
        gaussBuffer.push_back(evalGauss1D(static_cast <float>(i), sigma));
    }
    return gaussBuffer;
}

void Utility::printMatrix (const glm::mat4& m) {

    std::cout <<
        m[0][0] << " " << m[0][1] << " " << m[0][2] <<
        " " << m[0][3] << std::endl <<
        m[1][0] << " " << m[1][1] << " " << m[1][2] <<
        " " << m[1][3] << std::endl <<
        m[2][0] << " " << m[2][1] << " " << m[2][2] <<
        " " << m[2][3] << std::endl <<
        m[3][0] << " " << m[3][1] << " " << m[3][2] <<
        " " << m[3][3] << std::endl <<
        std::endl;

}

glm::vec3 Utility::convertCIExyYToRGB (const glm::vec3& CIExyYColor) {
    const float scalar = CIExyYColor.z / CIExyYColor.y;
    const glm::vec3 CIEXYZ { scalar*CIExyYColor.x,
                             CIExyYColor.z,
                             scalar*(1.f - CIExyYColor.x - CIExyYColor.y) };
    return Utility::XYZToRGB * CIEXYZ;
}

glm::vec3 Utility::convertRBGToCIExyY (const glm::vec3& rbgColor) {
    const glm::vec3 CIEXYZ = Utility::RGBToXYZ * rbgColor;
    const float sumXYZ = CIEXYZ.x + CIEXYZ.y + CIEXYZ.z;
    return glm::vec3(CIEXYZ.x / sumXYZ, CIEXYZ.y / sumXYZ, CIEXYZ.y);
}

unsigned int Utility::windowResolutionX = 0;
unsigned int Utility::windowResolutionY = 0;

float Utility::getLuminance (const glm::vec3& rgbColor) {
    const glm::vec3 CIExyYColor = convertRBGToCIExyY(rgbColor);

    return CIExyYColor.z;
}


float Utility::getWindowRatio() {
    return static_cast <float>(Utility::windowResolutionX) /
           static_cast <float>(Utility::windowResolutionY);
}


float Utility::xScreenToPortrait (float x) {
    constexpr float maxRatio = 2.75f;
    return x / (static_cast <float>(windowResolutionX) /
                (static_cast <float>(windowResolutionY) / maxRatio));
}

GLsizei Utility::getWidthFromHeight (unsigned int resolutionY) {
    return static_cast <GLsizei>(static_cast <float>(resolutionY) *
                                 static_cast <float>(windowResolutionX) /
                                 static_cast <float>(windowResolutionY));
}

glm::vec3 Utility::colorAsVec3 (const JBTypes::Color& color) {
    switch (color) {
    case JBTypes::Color::Red: return glm::vec3(1.f, 0.f, 0.f); break;
    case JBTypes::Color::Green: return glm::vec3(0.f, 1.f, 0.f); break;
    case JBTypes::Color::Blue: return glm::vec3(0.f, 0.f, 1.f); break;
    case JBTypes::Color::Yellow: return glm::vec3(1.f, 1.f, 0.f); break;
    case JBTypes::Color::None: return glm::vec3(0.f, 0.f, 0.f); break;
    default: return glm::vec3(0.f, 0.f, 0.f); break;
    }
    return glm::vec3(0.f, 0.f, 0.f);
}
