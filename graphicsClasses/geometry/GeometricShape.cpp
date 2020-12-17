/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeometricShape.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 29 mars 2020, 09:07
 */

#include "GeometricShape.h"

GeometricShape::GeometricShape(const std::vector<glm::vec3>& positions,
                               const std::vector<glm::vec3>& normals,
                               const std::vector<glm::vec3>& colors,
                               const std::vector<glm::vec2>& uvCoords,
                               const glm::mat4&              modelTransform,
                               const glm::mat4&              normalsTransform,
                               const std::vector<GLushort>&  indices
                              ):
_positions            (std::make_shared<std::vector<glm::vec3> >(positions)),
_normals              (std::make_shared<std::vector<glm::vec3> >(normals)),
_colors               (std::make_shared<std::vector<glm::vec3> >(colors)),
_uvCoords             (std::make_shared<std::vector<glm::vec2> >(uvCoords)),
_vertexArrayObject    (std::make_shared<GLuint>()),
_vertexBufferObjects  (std::make_shared<std::array<GLuint,4> >()),
_elementBufferObject  (!indices.empty()? std::make_shared<GLuint>() : nullptr),
_indices              (!indices.empty()? 
                          std::make_shared<std::vector<GLushort> >(indices) 
                          : nullptr ),
_modelTransform       (modelTransform),
_normalsTransform     (normalsTransform)
{
    glGenVertexArrays(1, _vertexArrayObject.get());
    glBindVertexArray(*_vertexArrayObject);
    glGenBuffers(4, _vertexBufferObjects->data());
    if (_elementBufferObject && _indices) {
        glGenBuffers(1, _elementBufferObject.get());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,*_elementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,_indices->size()* sizeof(GLushort),
                      _indices->data(),GL_STATIC_DRAW);
    }
    setVerticesData();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(0));
    
    glVertexAttribPointer ( 
            0,
            3, // 3 GL_FLOAT per vertex
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
            );
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(1));
    glVertexAttribPointer ( 
            1,
            3, // 3 GL_FLOAT per vertex
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
            );
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(2));
    glVertexAttribPointer ( 
            2,
            3, // 3 GL_FLOAT per vertex
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
            );
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(3));
    glVertexAttribPointer ( 
            3,
            2, // 2 GL_FLOAT per vertex
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
            );

}

GeometricShape::GeometricShape(const GeometricShape& geometricShape, 
                               const glm::mat4& modelTransform,
                               const glm::mat4& normalsTransform):
_positions            (geometricShape.positions()),
_normals              (geometricShape.normals()),
_colors               (geometricShape.colors()),
_uvCoords             (geometricShape.uvCoords()),
_vertexArrayObject    (geometricShape.vertexArrayObject()),
_vertexBufferObjects  (geometricShape.vertexBufferObjects()),
_elementBufferObject  (geometricShape.elementBufferObject()),
_indices              (geometricShape.indices()),
_modelTransform       (modelTransform),
_normalsTransform     (normalsTransform)
{

}

const std::shared_ptr<const std::vector<glm::vec3> >& 
                                            GeometricShape::positions() const {
    return _positions;
}

const std::shared_ptr<const std::vector<glm::vec3> >& 
                                            GeometricShape::normals() const {
    return _normals;
}

const std::shared_ptr<const std::vector<glm::vec3> >& 
                                            GeometricShape::colors() const {
    return _colors;
}

const std::shared_ptr<const std::vector<glm::vec2> >& 
                                            GeometricShape::uvCoords() const {
    return _uvCoords;
}

const std::shared_ptr<GLuint>& GeometricShape::vertexArrayObject() const {

    return _vertexArrayObject;
}

const std::shared_ptr<std::array<GLuint, 4> >& 
                                  GeometricShape::vertexBufferObjects() const {
    return _vertexBufferObjects;
}

const std::shared_ptr<GLuint>& GeometricShape::elementBufferObject() const {

    return _elementBufferObject;
}

const std::shared_ptr<const std::vector<GLushort> >& 
                                          GeometricShape::indices() const {
    return _indices;
}

const glm::mat4& GeometricShape::modelTransform() const{
    return _modelTransform;
}

const glm::mat4& GeometricShape::normalsTransform() const {
    return _normalsTransform;
}

size_t GeometricShape::levelOfDetail() const {
    return 0; 
}


std::vector<glm::vec3> GeometricShape::createCustomColorBuffer(
                                          const glm::vec3& customColor,
                                          size_t size) {
    std::vector<glm::vec3> customColorCube (size);
    for (glm::vec3& color : customColorCube) {
        color = customColor;
    }
    return customColorCube;
}

void GeometricShape::setVerticesData() const {

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(0));
    glBufferData(GL_ARRAY_BUFFER, _positions->size() * 3 * sizeof(GLfloat), 
            _positions->data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(1));
    glBufferData(GL_ARRAY_BUFFER, _colors->size() * 3 * sizeof(GLfloat), 
            _colors->data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(2));
    glBufferData(GL_ARRAY_BUFFER, _normals->size() * 3 * sizeof(GLfloat), 
            _normals->data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(3));
    glBufferData(GL_ARRAY_BUFFER, _uvCoords->size() * 2 * sizeof(GLfloat), 
            _uvCoords->data(), GL_STATIC_DRAW);

}

void GeometricShape::bind() const {
    glBindVertexArray(*_vertexArrayObject);
}

void GeometricShape::draw() const {

    if (_elementBufferObject && _indices) {
        glBindBuffer  (GL_ELEMENT_ARRAY_BUFFER,*_elementBufferObject);
        glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(_indices->size()),
            GL_UNSIGNED_SHORT,nullptr);
    }
    else {
        constexpr int offset = 0;
        glDrawArrays(GL_TRIANGLES,offset,
            static_cast<GLsizei>(_positions->size()));
    }
    

}
