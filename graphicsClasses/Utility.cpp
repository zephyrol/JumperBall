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


std::vector<GLfloat> Utility::getPositionsLocalCube(){
    const std::vector<GLfloat> positions  {
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
   return positions;
}

std::vector<GLfloat> Utility::getNormalsLocalCube() {

    const std::vector<GLfloat> normals;
    return normals;
}

std::vector<GLfloat> Utility::getColorsLocalCube() {
    const std::vector<GLfloat> colors {
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

    return colors;
}

std::array<std::vector<GLfloat>,3> Utility::getLocalSphere(){
 
    uint     iParaCount  = 40;
    uint     iMeriCount  = 60;
    float   r           = 0.5f;
    
    // Create a sphere --------------------------------------------------------------------------------
    GLuint iVertexCount = iParaCount * iMeriCount;
    
    /*glm::vec3* afPositions  = new glm::vec3[ iVertexCount ];
    glm::vec2* afTexCoords  = new glm::vec2[ iVertexCount ];*/

    std::vector<GLfloat> afPositions (iVertexCount*3);
    std::vector<GLfloat> afTexCoords(iVertexCount*2);
    
    float a1 = ( 180.0 / ( iParaCount + 1 ) ) * M_PI / 180.0;
    float a2 = ( 360.0 / ( iMeriCount - 1 ) ) * M_PI / 180.0;
    
    // parallels ---------------------------------------------------------------------------------------
    uint k = 0;
    for( uint i = 0; i < iParaCount; ++i )
    {
        float fAngle    = - static_cast<float>(M_PI) / 2.0f + a1 * ( i + 1 );
        float z         = r * sin( fAngle );
        float fRadius   = r * cos( fAngle );
        
        for( uint j = 0; j < iMeriCount; ++j )
        {

            glm::vec3 afPos ( fRadius * cos( a2 * j ), 
                                          fRadius * sin( a2 * j ), z );

            afPositions.push_back(afPos.x);
            afPositions.push_back(afPos.y);
            afPositions.push_back(afPos.z);

            glm::vec2 afTexC( static_cast<float>(j)/ iMeriCount, 
                              static_cast<float>( iParaCount - i ) 
                              / iParaCount);
            afPositions.push_back(afTexC.x);
            afPositions.push_back(afTexC.y);

            k++;
        }
    }
    
        // compute normals -----------------------------------------------------
        // on a 0 centered sphere : you just need to normalise the position!
        std::vector<GLfloat> afNormals(iVertexCount*3);

        for( uint i = 0; i < iVertexCount*3; i += 3 )
        {
            glm::vec3 normal (afPositions[i],afPositions[i+1],afPositions[i+2]);
            glm::normalize(normal);
            afNormals[ i ]    = normal.x;
            afNormals[ i+1 ]  = normal.y;
            afNormals[ i+2 ]  = normal.z;
        }

        GLuint iElementsCount = ( iMeriCount - 1 ) * ( iParaCount - 1 ) * 2 * 3;
        // for quads split in 2 
 
        GLuint* auiIndices = new GLuint[ iElementsCount ]; 
 
        k=0; 
        for( uint i = 0; i < ( iParaCount - 1 ); ++i ) 
        { 
            for( uint j = 0; j < ( iMeriCount - 1 ); ++j ) 
            { 
                auiIndices[ k++ ] = iMeriCount * i + j; 
                auiIndices[ k++ ] = iMeriCount * i + ( j + 1 ); 
                auiIndices[ k++ ] = iMeriCount * ( i + 1 ) + ( j + 1 ); 
                auiIndices[ k++ ] = iMeriCount * ( i + 1 ) + ( j + 1 ); 
                auiIndices[ k++ ] = iMeriCount * ( i + 1 ) + j; 
                auiIndices[ k++ ] = iMeriCount * i + j; 
            } 
        } 
 
        /*m_MeshSphere.createFrom( GL_TRIANGLES, iVertexCount, afPositions, afNormals, afTexCoords, iElementsCount, auiIndices ); */
        return std::array<std::vector<GLfloat>,3 > {afPositions,afNormals,
                                                  afTexCoords};
        /*delete [] afPositions; 
        delete [] afNormals; 
        delete [] afTexCoords; */


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