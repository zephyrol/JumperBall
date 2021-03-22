/*
 * File: RenderGroup.h
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */

#ifndef RENDERGROUP_H
#define RENDERGROUP_H
#include "Mesh.h"

class RenderGroup {

public:

RenderGroup(const vecMesh_sptr& meshes, const State::GlobalState& globalState);
RenderGroup(const RenderGroup& renderGroup, const State::GlobalState& globalState);
RenderGroup(const RenderGroup& renderGroup) = delete;
RenderGroup& operator= (const RenderGroup& renderGroup) = delete;
RenderGroup(RenderGroup&& renderGroup) = default;
RenderGroup& operator= (RenderGroup&& renderGroup) = default;

CstMesh_sptr getHeadMesh() const;
vecMesh_sptr update(const vecMesh_sptr& meshesToAdd = {});
void render() const;
State::GlobalState _globalState;

private:

struct BufferObjects {
    std::vector <GLuint> shapeVertexBufferObjects;
    std::vector <GLuint> stateVertexBufferObjects;
    GLuint elementBufferObject;
};

template<typename T> std::vector <GLuint> createStateVertexAttributesBufferObject(
    const std::vector <std::vector <T> >& attributes) const;

template<typename T> void fillBufferObjectData(
    GLuint bo,
    const std::vector <T>& bufferObjectData,
    GLenum target
    ) const;

template<typename T> std::shared_ptr <GLuint> initializeBO(
    const std::vector <T>& bufferObjectData,
    GLenum target
    ) const;

template<typename T> std::shared_ptr <GLuint> initializeVBO(const std::vector <T>& attributeData) const;
std::shared_ptr <GLuint> initializeEBO(const std::vector <GLushort>& indicesData) const;

template<typename T> void fillVBOsList(
    std::vector <GLuint>& vboList,
    const std::vector <T>& attributeData,
    size_t attributesOffset = 0
    ) const;

template<typename T> void fillStateVertexAttributesVBOsList(
    std::vector <GLuint>& vboList,
    const std::vector <std::vector <T> >& stateVertexAttributes,
    size_t attributesOffset
    ) const;

BufferObjects createBufferObjects() const;
BufferObjects createBufferObjects(const RenderGroup& renderGroup) const;

template<typename T> void activateVertexAttribute(
  const std::vector <T>& attributeData,
  GLuint attributeNumber) const;

template<typename T> size_t updateStateVBOsData(
    const std::vector <GLuint>& vbosList,
    const std::vector <std::vector <T> >& bufferObjectsData,
    size_t nbOfShapeVbos,
    size_t attributesOffset
    ) const;
template<typename T> size_t updateShapeVBOsData(
    const std::vector <GLuint>& vbosList,
    const std::vector <T>& bufferObjectsData,
    size_t attributesOffset
    ) const;

template<typename T> size_t updateBufferObjectData(
    GLuint bo,
    const std::vector <T>& bufferObjectData,
    GLenum target
    ) const;

  


void updateBufferObjectsData() const;

GLuint genVertexArrayObject() const;
GLuint genBufferObject() const;
Mesh::MeshVerticesInfo createMeshesVerticesInfo() const;
const BufferObjects& bufferObjects() const;

vecMesh_sptr _meshes;
Mesh::MeshVerticesInfo _meshesVerticesInfo;
public:
const GLuint _vertexArrayObject;
const BufferObjects _bufferObjects;
bool _needBuffersRebinding;
};


#endif // RENDERGROUP_H
