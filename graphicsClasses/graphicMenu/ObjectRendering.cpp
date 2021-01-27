
/* 
 * File:   ObjectRendering.h
 * Author: Morgenthaler S
 * 
 * Created on 17 janvier 2021, 11:20
 */

#include "ObjectRendering.h"
#include "ShaderProgram.h"
#include <iterator>

ObjectRendering::ObjectRendering(const Label &label):
    LabelRendering(label),
    _transformObject(0.f)
{
    if(!spObject) {
        spObject = std::make_shared<ShaderProgram> (
            Shader (GL_VERTEX_SHADER,   vsshaderObject),
            Shader (GL_FRAGMENT_SHADER, fsshaderObject)
        );
    }
    update(0.f);
}


void ObjectRendering::render() const {
    spObject->use();
    spObject->bindUniform("isActivated",_label.isActivated());
    spObject->bindUniform("M",_transformObject); 

    GLuint currentVAO = 0;
    for(const MeshComponent_sptr& component :_components) {
        if(currentVAO != *component->getShapeVAO()){
            component->bindShape();
            currentVAO = *component->getShapeVAO();
        }
        component->drawShape();
    }
}

void ObjectRendering::update(float offset) {

    const auto getPosition = [this](float offset) {
        JBTypes::vec2f position = _label.position();
        if (!_label.isFixed()) {
            position.y += offset;
        }
        return position;
    };
    const glm::mat4 biasMatrix  = glm::mat4{ 1.f, 0.f,  0.f, 0.f,
                                             0.f,  1.f, 0.f, 0.f,
                                             0.f,  0.f,  1.f, 0.f,
                                             -1.f, -1.f, 0.f, 1.f} ;

    constexpr float biasScalar = 2.f; //To multiply the translation by 2
    const glm::vec3 scale = glm::vec3{_label.width(),_label.height(),0.f};

    const glm::mat4 scaleMatrix = glm::scale(scale);

    const JBTypes::vec2f position = getPosition(offset);

    const glm::mat4 translate = glm::translate( biasScalar *
                                    glm::vec3{ position.x,
                                               position.y,
                                               0.f
                                              });
    _transformObject = biasMatrix * translate * scaleMatrix;
}

const std::string ObjectRendering::vsshaderObject = "shaders/objectVs.vs";
const std::string ObjectRendering::fsshaderObject = "shaders/objectFs.fs";
std::shared_ptr<const ShaderProgram> ObjectRendering::spObject = nullptr;
