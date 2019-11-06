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
#include <fstream>

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


std::vector<GLfloat> Utility::getPositionsLocalCube(){
    const std::vector<GLfloat> positions  {
        //Face 1 
        0.f,0.f,0.f, 1.f,0.f,0.f, 0.f,1.f,0.f,
        1.f,0.f,0.f, 1.f,1.f,0.f, 0.f,1.f,0.f,
        //Face 2
        0.f,0.f,1.f, 1.f,0.f,1.f, 0.f,1.f,1.f,
        1.f,0.f,1.f, 1.f,1.f,1.f, 0.f,1.f,1.f,
        //Face 3
        0.f,0.f,0.f, 1.f,0.f,0.f, 0.f,0.f,1.f,
        1.f,0.f,0.f, 1.f,0.f,1.f, 0.f,0.f,1.f,
        //Face 4
        0.f,1.f,0.f, 1.f,1.f,0.f, 0.f,1.f,1.f,
        1.f,1.f,0.f, 1.f,1.f,1.f, 0.f,1.f,1.f,
        //Face 5
        0.f,0.f,0.f, 0.f,1.f,0.f, 0.f,0.f,1.f,
        0.f,1.f,0.f, 0.f,1.f,1.f, 0.f,0.f,1.f,
        //Face 6
        1.f,0.f,0.f, 1.f,1.f,0.f, 1.f,0.f,1.f,
        1.f,1.f,0.f, 1.f,1.f,1.f, 1.f,0.f,1.f,
    };
   return positions;
}

std::vector<GLfloat> Utility::getNormalsLocalCube() {

    const std::vector<GLfloat> normals;
    return normals;
}
std::vector<GLfloat> Utility::getColorsLocalCube() {
    const std::vector<GLfloat> colors;
    return colors;
}

