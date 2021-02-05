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
    _elementBufferObject((!_staticAttributes.indices.empty()) ? genBufferObject()
                         : 0),
    // Use smartpointer, when the uniform is changed outside this class, every renderpass will be updated
    _uniformMatrix4{},
    _uniformVec4{},
    _uniformVec3{},
    _uniformVec2{},
    _uniformFloat{},
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

    std::map <RenderPass::StaticAttributeType, GLuint> vertexBufferObjects;

    const auto fillVertexBufferObject =
        [&vertexBufferObjects] (const RenderPass::StaticAttributeType& type,
                                const std::shared_ptr <GLuint>& vertexBufferObject) {
            if (vertexBufferObject) {
                vertexBufferObjects[type] = *vertexBufferObject;
            }
        };

    fillVertexBufferObject(RenderPass::StaticAttributeType::Positions,
                           initializeVBO(_staticAttributes.positions));
    fillVertexBufferObject(RenderPass::StaticAttributeType::Normals,
                           initializeVBO(_staticAttributes.normals));
    fillVertexBufferObject(RenderPass::StaticAttributeType::Colors,
                           initializeVBO(_staticAttributes.colors));
    fillVertexBufferObject(RenderPass::StaticAttributeType::UvCoords,
                           initializeVBO(_staticAttributes.uvCoords));

    return vertexBufferObjects;
}

template<typename T> std::shared_ptr <GLuint> RenderPass::initializeVBO (
    const std::vector <T> staticAttributeData) const {
    std::shared_ptr <GLuint> vbo = nullptr;
    if (!staticAttributeData.empty()) {
        const std::shared_ptr <GLuint> vbo = std::make_shared <GLuint>(genBufferObject());
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     staticAttributeData.size() * sizeof(T),
                     staticAttributeData.data(),
                     GL_STATIC_DRAW);
    }
    return vbo;
}
