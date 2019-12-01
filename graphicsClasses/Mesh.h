/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mesh.h
 * Author: seb
 *
 * Created on 13 novembre 2019, 21:05
 */

#ifndef MESH_H
#define MESH_H
#include "Shader.h"
#include <Ball.h>
#include <Map.h>
#include <glm/glm.hpp>

class Mesh {
public:
    //Mesh();
    Mesh(const Ball& ball);
    Mesh(const Map& map);
    const glm::mat4& local() const ;
    const glm::mat4& world() const ;

    void local(const glm::mat4& l);
    void world(const glm::mat4& w);
    
    virtual ~Mesh();

    void render() const;

    void updateMatrices (const Ball& b);

private:
    std::vector<glm::vec3>  _positions;
    std::vector<glm::vec3>  _normals;
    std::vector<glm::vec3>  _colors;
    std::vector<glm::vec2>  _uvCoords;

    bool                    _useIndexing;
    std::vector<GLushort>   _indices;
    GLuint                  _idElementBuffer;

    GLuint                  _idVertexArray;
    std::array<GLuint,2>    _idVertexBuffer;

    glm::mat4               _local;
    glm::mat4               _world;
};

#endif /* MESH_H */

