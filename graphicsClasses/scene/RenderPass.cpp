/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

RenderPass::RenderPass(const ShaderProgram& shaderProgram, const vecMesh_sptr& meshes):
    _shaderProgram(shaderProgram),
    _vertexArrayObject(genVertexArrayObject()),
    _meshes(meshes),
    _meshesVerticesInfo(createMeshesVerticesInfo()),
    _bufferObjects(createBufferObjects()),
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

Mesh::MeshVerticesInfo RenderPass::createMeshesVerticesInfo() const {
    Mesh::MeshVerticesInfo meshesVerticesInfo;
    for (const CstMesh_sptr& mesh : _meshes) {
        Mesh::concatMeshVerticesInfo(meshesVerticesInfo, mesh->genMeshVerticesInfo());
    }
    return meshesVerticesInfo;
}

void RenderPass::update() {
    for (const auto& mesh : _meshes) {
        mesh->update();
        const Mesh::Uniforms uniforms = mesh->genUniformsValues();
        upsertUniforms(uniforms.uniformFloats);
        upsertUniforms(uniforms.uniformVec2s);
        upsertUniforms(uniforms.uniformVec3s);
        // _stateVertexAttributes = createStateVertexAttributes();
    }
}

template<typename T> void RenderPass::upsertUniforms (const std::map <std::string, T>& uniformsData) {
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

void RenderPass::bindUniforms() const {

    int textureNumber = 0;
    for (const auto& uniformTexture : _uniformTextures) {
        _shaderProgram.bindUniformTexture(uniformTexture.first, textureNumber, uniformTexture.second);
        ++textureNumber;
    }
    bindUniforms(_uniformFloat);
    bindUniforms(_uniformVec2);
    bindUniforms(_uniformVec3);
    bindUniforms(_uniformMatrix4);
}

void RenderPass::render() const {
    _shaderProgram.use();
    glBindVertexArray(_vertexArrayObject);
    bindUniforms();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferObjects.elementBufferObject);

    glDrawElements(GL_TRIANGLES, static_cast <GLsizei>(_meshesVerticesInfo.shapeVerticesInfo.indices.size()),
                   GL_UNSIGNED_SHORT, nullptr);
}

template<typename T> void RenderPass::bindUniforms (UniformVariable <T> uniforms) const {
    for (const auto& uniform : uniforms) {
        _shaderProgram.bindUniform(uniform.first, uniform.second);
    }
}

template<typename T> std::vector <GLuint> RenderPass::createStateVertexAttributesBufferObject (
    const std::vector <std::vector <T> >& attributes) const {
    std::vector <GLuint> vertexBufferObjects;
    for (const auto& stateVertexAttribute : attributes) {
        const auto vertexBuffer = initializeVBO(stateVertexAttribute);
        if (vertexBuffer) {
            vertexBufferObjects.push_back(*vertexBuffer);
        }
    }
    return vertexBufferObjects;
}

template<typename T> void RenderPass::fillVBOsList (
    std::vector <GLuint>& vboList,
    const std::vector <T>& attributeData,
    size_t attributesOffset
    ) const {
    const std::shared_ptr <GLuint> vertexBufferObject = initializeVBO(attributeData);
    if (vertexBufferObject) {
        const size_t attributeNumber = vboList.size() + attributesOffset;
        const size_t numberOfGLfloats = sizeof(T) / sizeof(GLfloat);

        glEnableVertexAttribArray(attributeNumber);
        glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferObject);
        glVertexAttribPointer(attributeNumber, numberOfGLfloats, GL_FLOAT, GL_FALSE, 0, nullptr);
        vboList.push_back(*vertexBufferObject);
    }
}

template<typename T> void RenderPass::fillStateVertexAttributesVBOsList (std::vector <GLuint>& vboList,
                                                                         const std::vector <std::vector <T> >& stateVertexAttributes,
                                                                         size_t attributesOffset) const {
    for (const std::vector <T>& stateVertexAttribute : stateVertexAttributes) {
        fillVBOsList(vboList, stateVertexAttribute, attributesOffset);
    }
}

RenderPass::BufferObjects RenderPass::createBufferObjects() const {

    glBindVertexArray(_vertexArrayObject);
    RenderPass::BufferObjects bufferObjects;

    std::vector <GLuint>& shapeVBOs = bufferObjects.shapeVertexBufferObjects;
    const auto& shapeVerticesInfo = _meshesVerticesInfo.shapeVerticesInfo;
    const auto& shapeVertexAttributes = shapeVerticesInfo.shapeVertexAttributes;
    fillVBOsList(shapeVBOs, shapeVertexAttributes.positions);
    fillVBOsList(shapeVBOs, shapeVertexAttributes.colors);
    fillVBOsList(shapeVBOs, shapeVertexAttributes.normals);
    fillVBOsList(shapeVBOs, shapeVertexAttributes.uvCoords);

    const size_t attributesOffset = shapeVBOs.size();
    std::vector <GLuint>& stateVBOs = bufferObjects.stateVertexBufferObjects;
    const auto& stateVertexAttributes = _meshesVerticesInfo.stateVertexAttributes;

    fillStateVertexAttributesVBOsList(stateVBOs, stateVertexAttributes.staticFloats, attributesOffset);
    fillStateVertexAttributesVBOsList(stateVBOs, stateVertexAttributes.staticVec2s, attributesOffset);
    fillStateVertexAttributesVBOsList(stateVBOs, stateVertexAttributes.staticVec3s, attributesOffset);

    const auto ebo = initializeEBO(shapeVerticesInfo.indices);
    bufferObjects.elementBufferObject = ebo
                                        ? *ebo
                                        : 0;

    return bufferObjects;
}

template<typename T> std::shared_ptr <GLuint> RenderPass::initializeBO (
    const std::vector <T>& attributeData,
    GLenum target
    ) const {
    std::shared_ptr <GLuint> bo = nullptr;
    if (!attributeData.empty()) {
        bo = std::make_shared <GLuint>(genBufferObject());
        glBindBuffer(target, *bo);
        glBufferData(target, attributeData.size() * sizeof(T), attributeData.data(), GL_STATIC_DRAW);
    }
    return bo;
}

template<typename T> std::shared_ptr <GLuint> RenderPass::initializeVBO (
    const std::vector <T>& attributeData
    ) const {
    return initializeBO(attributeData, GL_ARRAY_BUFFER);
}

std::shared_ptr <GLuint> RenderPass::initializeEBO (const std::vector <GLushort>& indicesData) const {
    return initializeBO(indicesData, GL_ELEMENT_ARRAY_BUFFER);
}
