/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeometricShape.h
 * Author: Morgenthaler S 
 *
 * Created on 29 mars 2020, 09:07
 */

#ifndef GEOMETRICSHAPE_H
#define GEOMETRICSHAPE_H
#include "Shader.h"
#include "Utility.h"

class GeometricShape {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    GeometricShape(
            const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec3>& normals,
            const std::vector<glm::vec3>& colors,
            const std::vector<glm::vec2>& uvCoords,
            const glm::mat4&              modelTransform    = glm::mat4(1.f),
            const glm::mat4&              normalsTransform  = glm::mat4(1.f),
            const std::vector<GLushort>&  indices = {}
            );

    //It's useless to have a copy of a shape with exactly the same transform
    //and vertices, every attributes are const...
    GeometricShape(const GeometricShape& geometricShape)               = delete;
    GeometricShape& operator= (const GeometricShape& geometricShape)   = delete;

    GeometricShape(GeometricShape&& geometricShape)                   = default;
    GeometricShape& operator= (GeometricShape&& geometricShape)       = default;


    //Use this constructor to make a copy with a different transform
    GeometricShape(const GeometricShape& geometricShape, 
                    const glm::mat4& modelTransform,
                    const glm::mat4& normalsTransform);

    virtual ~GeometricShape                                ()          =default;


    //-------CONST METHODS------ --//
    void                                                   setVerticesData() 
                                                                          const;

    void                                                   bind()         const;
    void                                                   draw()         const;


    const std::shared_ptr<const std::vector<glm::vec3> >&  positions()    const;
    const std::shared_ptr<const std::vector<glm::vec3> >&  normals()      const;
    const std::shared_ptr<const std::vector<glm::vec3> >&  colors()       const;
    const std::shared_ptr<const std::vector<glm::vec2> >&  uvCoords()     const;

    const std::shared_ptr<GLuint>&                         vertexArrayObject()  
                                                                          const;
    const std::shared_ptr<std::array<GLuint,4> >&          vertexBufferObjects() 
                                                                          const;
    const std::shared_ptr<GLuint>&                         elementBufferObject()
                                                                          const;
    const std::shared_ptr<const std::vector<GLushort> >&   indices()      const;

    const glm::mat4&                                       modelTransform()  
                                                                          const;
    const glm::mat4&                                       normalsTransform()                                                                               const;
    //--------STATIC METHODS-------//
    static std::vector<glm::vec3>   createCustomColorBuffer(
                                          const glm::vec3& customColor,
                                          size_t size);
private:

    //--------ATTRIBUTES-----------//
     std::shared_ptr<const std::vector<glm::vec3> >  _positions;
     std::shared_ptr<const std::vector<glm::vec3> >  _normals;
     std::shared_ptr<const std::vector<glm::vec3> >  _colors;
     std::shared_ptr<const std::vector<glm::vec2> >  _uvCoords;

     std::shared_ptr<GLuint>                         _vertexArrayObject;
     std::shared_ptr<std::array<GLuint,4> >          _vertexBufferObjects;

     std::shared_ptr<GLuint>                         _elementBufferObject;
     std::shared_ptr<const std::vector<GLushort> >   _indices;

     glm::mat4                                       _modelTransform;
     glm::mat4                                       _normalsTransform;

};

#endif /* GEOMETRICSHAPE_H */

