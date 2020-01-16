/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Utility.cpp
 * Author: seb
 * 
 * Created on 3 novembre 2019, 15:45
 */

#include "Utility.h"
#include "glm/gtx/transform.hpp"
#include <math.h>
#include <fstream>
#include <glm/glm.hpp>



std::string Utility::readFileSrc(const std::string& filePath) {
    // precondition
    if (filePath.size() == 0) 
        std::cerr << "Invalid parameter filePath : size() must not be 0." 
                << std::endl;
    //--------------------------------------------------------------------------

    std::cout << "Opening file " <<  filePath << std::endl;

    std::string strContent;


    std::ifstream file(filePath.c_str());
    if (!file) {
    std::cerr << "Could not open file " << filePath << std::endl;
    }

    file.seekg(0, std::ios::end);
    strContent.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    strContent.assign((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());


    // postcondition
    if (strContent.size() == 0)  
        std::cout << "Invalid content read strContent(\"" << strContent 
                << "\") : size() must not be 0."  << std::endl;
    //--------------------------------------------------------------------------

    return strContent;

}


const std::vector<GLfloat> Utility::positionsCube {
    //Face 1 
    0.f,1.f,0.f, 1.f,0.f,0.f,  0.f,0.f,0.f,
    0.f,1.f,0.f, 1.f,1.f,0.f,  1.f,0.f,0.f,
    //Face 2
    0.f,0.f,1.f, 1.f,0.f,1.f, 0.f,1.f,1.f,
    1.f,0.f,1.f, 1.f,1.f,1.f, 0.f,1.f,1.f,
    //Face 3
    0.f,0.f,0.f, 1.f,0.f,0.f, 0.f,0.f,1.f,
    1.f,0.f,0.f, 1.f,0.f,1.f, 0.f,0.f,1.f,
    //Face 4
    0.f,1.f,1.f, 1.f,1.f,0.f,  0.f,1.f,0.f, 
    0.f,1.f,1.f, 1.f,1.f,1.f,  1.f,1.f,0.f, 
    //Face 5
    0.f,0.f,1.f, 0.f,1.f,0.f,  0.f,0.f,0.f, 
    0.f,0.f,1.f, 0.f,1.f,1.f,  0.f,1.f,0.f, 
    //Face 6
    1.f,0.f,0.f, 1.f,1.f,0.f, 1.f,0.f,1.f,
    1.f,1.f,0.f, 1.f,1.f,1.f, 1.f,0.f,1.f
};

const std::vector<GLfloat> Utility::normalsCube {
    //Face 1 
    0.f,0.f,-1.f, 0.f,0.f,-1.f, 0.f,0.f,-1.f,
    0.f,0.f,-1.f, 0.f,0.f,-1.f, 0.f,0.f,-1.f,
    //Face 2
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    //Face 3
    0.f,-1.f,0.f, 0.f,-1.f,0.f, 0.f,-1.f,0.f,
    0.f,-1.f,0.f, 0.f,-1.f,0.f, 0.f,-1.f,0.f,
    //Face 4 
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    //Face 5
    -1.f,0.f,0.f, -1.f,0.f,0.f, -1.f,0.f,0.f,
    -1.f,0.f,0.f, -1.f,0.f,0.f, -1.f,0.f,0.f,
    //Face 6
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
};

const std::vector<GLfloat> Utility::colorsCube {

    //Face 1 
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
    //Face 2
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    //Face 3
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    //Face 4 
    1.f,1.f,0.f, 1.f,1.f,0.f, 1.f,1.f,0.f,
    1.f,1.f,0.f, 1.f,1.f,0.f, 1.f,1.f,0.f,
    //Face 5
    0.f,1.f,1.f, 0.f,1.f,1.f, 0.f,1.f,1.f,
    0.f,1.f,1.f, 0.f,1.f,1.f, 0.f,1.f,1.f,
    //Face 6
    1.f,0.f,1.f, 1.f,0.f,1.f, 1.f,0.f,1.f,
    1.f,0.f,1.f, 1.f,0.f,1.f, 1.f,0.f,1.f
};

const std::vector<GLfloat> Utility::positionsPike {
    //Base
    0.f,0.f,1.f,       0.f,0.f,0.f, 1.f,0.f,0.f, 
    0.f,0.f,1.f,       1.f,0.f,0.f, 1.f,0.f,1.f, 
    //Face 1
    0.f,0.f,0.f,    0.5f,1.f,0.5f,  0.5f,0.f,0.f, 
    0.5f,0.f,0.f,   0.5f,1.f,0.5f,  1.f,0.f,0.f, 
    //Face 2
    0.5f,0.f,1.f,   0.5f,1.f,0.5f,  0.f,0.f,1.f, 
    1.f,0.f,1.f,    0.5f,1.f,0.5f,  0.5f,0.f,1.f,
    //Face 3
    0.f,0.f,0.f,    0.f,0.f,0.5f,   0.5f,1.f,0.5f,
    0.f,0.f,0.5f,   0.f,0.f,1.f,    0.5f,1.f,0.5f,
    //Face 4
    1.f,0.f,0.5f,   1.f,0.f,0.f,    0.5f,1.f,0.5f,
    1.f,0.f,1.f,    1.f,0.f,0.5f,   0.5f,1.f,0.5f,
};

const std::vector<GLfloat> Utility::colorsPike {
    //Base
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    //Face 1
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.1f,0.1f,0.1f, 
    //Face 2
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.1f,0.1f,0.1f, 
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    //Face 3
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  
    0.5f,0.5f,0.5f, 0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f, 
    //Face 4
    0.5f,0.5f,0.5f, 0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f,
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,
};

/*const std::vector<GLfloat> Utility::colorsPike {
    //Base
    0.f,0.f,0.f, 0.f,0.f,0.f,  0.f,0.f,0.f, 
    0.f,0.f,0.f, 0.f,0.f,0.f,  0.f,0.f,0.f, 
    //Face 1
    0.f,0.f,0.f, 0.f,0.f,0.f,  0.f,0.f,0.f, 
    0.f,0.f,0.f, 0.f,0.f,0.f,  0.f,0.f,0.f, 
    //Face 2
    0.f,0.f,0.f, 0.f,0.f,0.f,  0.f,0.f,0.f, 
    0.f,0.f,0.f, 0.f,0.f,0.f,  0.f,0.f,0.f, 
    //Face 3
    0.f,0.f,0.f, 0.f,0.f,0.f, 0.f,0.f,0.f,  
    0.f,0.f,0.f, 0.f,0.f,0.f, 0.f,0.f,0.f, 
    //Face 4
    0.f,0.f,0.f, 0.f,0.f,0.f, 0.f,0.f,0.f,
    0.f,0.f,0.f, 0.f,0.f,0.f, 0.f,0.f,0.f,
};*/

const std::vector<GLfloat> Utility::normalsPike = 
                                        Utility::computeNormals(positionsPike);


std::vector<GLfloat> Utility::computeNormals(const std::vector<GLfloat>& 
                                                positions) {
    std::vector<GLfloat> normals;
    constexpr unsigned int offsetPointA       = 0;
    constexpr unsigned int offsetPointB       = 3;
    constexpr unsigned int offsetPointC       = 6;
    
    constexpr unsigned int offsetCoordX       = 0;
    constexpr unsigned int offsetCoordY       = 1;
    constexpr unsigned int offsetCoordZ       = 2;
    
    constexpr unsigned int coordsPerTriangle  = 9;
    constexpr unsigned int pointsPerTriangles = 3;
    
    for ( size_t i = 0; i < positions.size(); i+=coordsPerTriangle) {
        glm::vec3 normal  (
            glm::cross( 
                glm::vec3(  
                            positionsPike.at(offsetPointB + offsetCoordX) -
                            positionsPike.at(offsetPointA + offsetCoordX),
                            positionsPike.at(offsetPointB + offsetCoordY) -
                            positionsPike.at(offsetPointA + offsetCoordY),
                            positionsPike.at(offsetPointB + offsetCoordZ) -
                            positionsPike.at(offsetPointA + offsetCoordZ)
                          ),
                glm::vec3(  
                            positionsPike.at(offsetPointC + offsetCoordX) -
                            positionsPike.at(offsetPointB + offsetCoordX),
                            positionsPike.at(offsetPointC + offsetCoordY) -
                            positionsPike.at(offsetPointB + offsetCoordY),
                            positionsPike.at(offsetPointC + offsetCoordZ) -
                            positionsPike.at(offsetPointB + offsetCoordZ)
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

glm::mat4 Utility::rotationUpToDir( JumperBallTypes::Direction dir) {

    glm::mat4 rotationMatrix;
    constexpr float fPI   = static_cast<float>(M_PI);
    constexpr float fPI2  = static_cast<float>(M_PI_2);

    switch (dir) {
        case JumperBallTypes::Direction::North:
            rotationMatrix = glm::rotate(-fPI2,glm::vec3(1.f,0.f,0.f));
            break;
        case JumperBallTypes::Direction::South:
            rotationMatrix = glm::rotate(+fPI2,glm::vec3(1.f,0.f,0.f));
            break;
        case JumperBallTypes::Direction::East:
            rotationMatrix = glm::rotate(-fPI2,glm::vec3(0.f,0.f,1.f));
            break;
        case JumperBallTypes::Direction::West:
            rotationMatrix = glm::rotate(fPI2,glm::vec3(0.f,0.f,1.f));
            break;
        case JumperBallTypes::Direction::Up:
            rotationMatrix = glm::mat4(1.f);
            break;
        case JumperBallTypes::Direction::Down:
            rotationMatrix = glm::rotate(fPI,glm::vec3(1.f,0.f,0.f));
            break;
        default :
            break;
    }

    return rotationMatrix;
}


void Utility::printMatrix(const glm::mat4& m) {

    std::cout << 
            m[0][0] << " " << m[0][1] << " " <<  m[0][2] << 
            " " <<  m[0][3]  << std::endl <<
            m[1][0] << " " << m[1][1] << " " << m[1][2] <<  
            " " << m[1][3]  <<  std::endl <<
            m[2][0] << " " << m[2][1] << " " << m[2][2] << 
            " " <<  m[2][3]  <<  std::endl <<
            m[3][0] << " " << m[3][1] << " " << m[3][2] << 
            " " <<  m[3][3]  <<  std::endl <<
            std::endl;

}