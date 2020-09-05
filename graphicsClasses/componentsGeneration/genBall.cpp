/*
 * file:   genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "../MeshGenerator.h"

VecMeshComponentSptr MeshGenerator::genComponents(const Ball& ball) {

    const glm::mat4 scaleMatrix = glm::scale(glm::vec3(ball.getRadius()));
    const Sphere sphere;

    MeshComponentSptr component = std::make_shared<MeshComponent>
            (std::make_shared<Sphere>(sphere,scaleMatrix),
             std::make_shared<BallAnimation>(ball));
    return VecMeshComponentSptr {component};
}
