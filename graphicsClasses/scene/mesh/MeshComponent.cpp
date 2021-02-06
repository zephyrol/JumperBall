/*
 * File: MeshComponent.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 mars 2020, 00:35
 */

#include "MeshComponent.h"

MeshComponent::MeshComponent(const std::shared_ptr <const GeometricShape>& elem,
                             const std::shared_ptr <Animation>& anim):
    _shape(elem),
    _animation(anim),
    _cstAnimation(std::const_pointer_cast <const Animation>(_animation)) {

}

/*void MeshComponent::bindShape() const {
   _shape->bind();
   }

   const glm::mat4& MeshComponent::getShapeModelTransform() const {
   return _shape->modelTransform();
   }

   const glm::mat4& MeshComponent::getShapeNormalsTransform() const {
   return _shape->normalsTransform();
   }*/

/*glm::mat4 MeshComponent::getAnimationModel() const {
   return _animation->model();
   }

   glm::mat4 MeshComponent::getAnimationScaleRotation() const {
   return _animation->scaleRotation();
   }

   glm::mat4 MeshComponent::getAnimationTranslation() const {
   return _animation->translation();
   }

   const std::shared_ptr <GLuint>& MeshComponent::getShapeVAO() const {
   return _shape->vertexArrayObject();
   }

   void MeshComponent::drawShape() const {
   _shape->draw();
   }*/

const std::shared_ptr <Animation>& MeshComponent::animation() {
    return _animation;
}

const std::shared_ptr <const Animation>& MeshComponent::animation() const {
    return _cstAnimation;
}
