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
    if (!_staticAttributes.positions.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::Positions] = genBufferObject();
    }
    if (!_staticAttributes.normals.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::Normals] = genBufferObject();
    }
    if (!_staticAttributes.colors.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::Colors] = genBufferObject();
    }
    if (!_staticAttributes.uvCoords.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::UvCoords] = genBufferObject();
    }
    if (!_staticAttributes.indices.empty()) {
        vertexBufferObjects[RenderPass::StaticAttributeType::Indices] = genBufferObject();
    }
    return vertexBufferObjects;
}
