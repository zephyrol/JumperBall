/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

RenderPass::RenderPass(const ShaderProgram& shaderProgram, const vecCstMesh_sptr& meshes):
    _shaderProgram(shaderProgram),
    _vertexArrayObject(genVertexArrayObject()),
    _meshes(meshes),
    _staticAttributes(createStaticAttributes()),
    _vertexBufferObjects(createVertexBufferObjects()),
    _elementBufferObject(_vertexBufferObjects.find(
                             RenderPass::StaticAttributeType::Indices) == _vertexBufferObjects.end()
                         ? genBufferObject()
                         : 0),
    // Use smartpointer, when the uniform is changed outside this class, every renderpass will be updated
    _uniformMatrix4{},
    _uniformVec4{},
    _uniformVec3{},
    _uniformVec2{},
    _uniformFloat{},
    _uniformBool{},
    _uniformTextures{} {
}

GLuint RenderPass::genVertexArrayObject() const {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

GLuint RenderPass::genBufferObject() const {
    GLuint bo;
    glGenBuffers(1, &bo);
    return bo;
}

void RenderPass::update() {

}

void RenderPass::upsertUniform (const std::string& name, const glm::mat4& value) {
    _uniformMatrix4[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const glm::vec4& value) {
    _uniformVec4[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const glm::vec3& value) {
    _uniformVec3[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const glm::vec2& value) {
    _uniformVec2[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const bool& value) {
    _uniformBool[name] = value;
}

void RenderPass::upsertUniformTexture (const std::string& name, const GLuint value) {
    _uniformTextures[name] = value;
}

void RenderPass::render() const {
    glBindVertexArray(_vertexArrayObject);
}

Mesh::StaticAttributes RenderPass::createStaticAttributes() const {
    Mesh::StaticAttributes staticAttributes;
    for (const CstMesh_sptr& mesh : _meshes) {
        staticAttributes = Mesh::concatStaticAttributes(staticAttributes, mesh->genStaticAttributes());
    }
    return staticAttributes;
}

std::map <RenderPass::StaticAttributeType, GLuint> RenderPass::createVertexBufferObjects() const {


    // TODO take setverticesdate from geometricshape.cpp and use hashmap for the sizes and accesses
    std::map <RenderPass::StaticAttributeType, GLuint> vertexBufferObjects;
    const std::vector<glm::vec3>& positions = _staticAttributes.positions;
    if (!positions.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::Positions] = genBufferObject();
        initializeVBO(vertexBufferObjects[RenderPass::StaticAttributeType::Positions], 3 * sizeof(GLfloat), positions);
    }

    const std::vector<glm::vec3>& normals = _staticAttributes.normals;
    if (!normals.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::Normals] = genBufferObject();
        initializeVBO(vertexBufferObjects[RenderPass::StaticAttributeType::Normals], 3 * sizeof(GLfloat), normals);
    }

    const std::vector<glm::vec3>& colors = _staticAttributes.colors;
    if (!colors.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::Colors] = genBufferObject();
        initializeVBO(vertexBufferObjects[RenderPass::StaticAttributeType::Colors], 3 * sizeof(GLfloat), colors);
    }

    const std::vector<glm::vec2>& uvCoords= _staticAttributes.uvCoords;
    if (!uvCoords.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::UvCoords] = genBufferObject();
        initializeVBO(vertexBufferObjects[RenderPass::StaticAttributeType::UvCoords], 2 * sizeof(GLfloat), uvCoords);
    }

    return vertexBufferObjects;
}

template<typename T>
void RenderPass::initializeVBO(const GLuint& vbo, size_t sizeOfElement, const std::vector<T> bufferData) const
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               bufferData.size() * sizeOfElement,
               bufferData.data(),
               GL_STATIC_DRAW
               );
}
