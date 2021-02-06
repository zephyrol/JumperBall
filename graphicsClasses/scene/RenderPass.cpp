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
    _numberOfVertices(computeNumberOfVertices()),
    _staticAttributes(createStaticAttributes()),
    _dynamicAttributes(createDynamicAttributes()),
    _vertexStaticBufferObjects(createVertexStaticBufferObjects()),
    _vertexDynamicBufferObjects(createVertexDynamicBufferObjects()),
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
    for (const auto& mesh : _meshes) {
        const Mesh::Uniforms uniforms = mesh->genUniformsValues();
        upsertUniforms(uniforms.uniformFloats);
        upsertUniforms(uniforms.uniformVec2s);
        upsertUniforms(uniforms.uniformVec3s);
        _dynamicAttributes = createDynamicAttributes();
    }
}

template<typename T> void RenderPass::upsertUniforms (const std::map <std::string,
                                                                      T> uniformsData) {
    for (const auto& uniform : uniformsData) {
        upsertUniform(uniform.first, uniform.second);
    }
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

void RenderPass::upsertUniform (const std::string& name, const GLfloat& value) {
    _uniformFloat[name] = value;
}

void RenderPass::upsertUniformTexture (const std::string& name, const GLuint value) {
    _uniformTextures[name] = value;
}

void RenderPass::render() const {
    glBindVertexArray(_vertexArrayObject);
}

template<typename T> void RenderPass::createAttributes (T& attributes) const {
    for (const CstMesh_sptr& mesh : _meshes) {
        attributes = Mesh::concatAttributes(attributes, mesh->genAttributes <T>());
    }
}

Mesh::StaticAttributes RenderPass::createStaticAttributes() const {
    Mesh::StaticAttributes staticAttributes;
    createAttributes(staticAttributes);
    return staticAttributes;
}

Mesh::DynamicAttributes RenderPass::createDynamicAttributes() const {
    Mesh::DynamicAttributes dynamicAttributes;
    createAttributes(dynamicAttributes);
    return dynamicAttributes;
}

size_t RenderPass::computeNumberOfVertices() const {
    size_t numberOfVertices = 0;
    for (const auto& mesh : _meshes) {
        numberOfVertices += mesh->numberOfVertices();
    }
    return numberOfVertices;
}

template<typename T> std::vector <GLuint> RenderPass::createDynamicAttributesBufferObject (
    const std::vector <std::vector <T> >& attributes) const {
    std::vector <GLuint> vertexBufferObjects;
    for (const auto& dynamicAttribute : attributes) {
        const auto vertexBuffer = initializeVBO(dynamicAttribute);
        if (vertexBuffer) {
            vertexBufferObjects.push_back(*vertexBuffer);
        }
    }
    return vertexBufferObjects;
}

std::vector <GLuint> RenderPass::createVertexDynamicBufferObjects() const {

    std::vector <GLuint> vertexBufferObjects;
    const std::vector <GLuint> vbosFloatsAttributes = createDynamicAttributesBufferObject(
        _dynamicAttributes.dynamicFloats);
    const std::vector <GLuint> vbosVec2sAttributes = createDynamicAttributesBufferObject(
        _dynamicAttributes.dynamicsVec2s);
    const std::vector <GLuint> vbosVec3sAttributes = createDynamicAttributesBufferObject(
        _dynamicAttributes.dynamicsVec3s);
    const std::vector <GLuint> vbosUbytesAttributes = createDynamicAttributesBufferObject(
        _dynamicAttributes.dynamicUbytes);
    vertexBufferObjects.insert(vertexBufferObjects.begin(),
                               vbosFloatsAttributes.begin(), vbosFloatsAttributes.end());
    vertexBufferObjects.insert(vertexBufferObjects.begin(),
                               vbosVec2sAttributes.begin(), vbosVec2sAttributes.end());
    vertexBufferObjects.insert(vertexBufferObjects.begin(),
                               vbosVec3sAttributes.begin(), vbosVec3sAttributes.end());
    vertexBufferObjects.insert(vertexBufferObjects.begin(),
                               vbosUbytesAttributes.begin(), vbosUbytesAttributes.end());
    return vertexBufferObjects;
}

std::map <RenderPass::StaticAttributeType, GLuint> RenderPass::createVertexStaticBufferObjects() const {

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
    const std::vector <T> attributeData) const {
    std::shared_ptr <GLuint> vbo = nullptr;
    if (!attributeData.empty()) {
        const std::shared_ptr <GLuint> vbo = std::make_shared <GLuint>(genBufferObject());
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     attributeData.size() * sizeof(T),
                     attributeData.data(),
                     GL_STATIC_DRAW);
    }
    return vbo;
}
