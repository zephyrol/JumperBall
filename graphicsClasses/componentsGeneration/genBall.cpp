/*
 * file: genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "scene/mesh/MeshGenerator.h"
#include "animations/BallAnimation.h"

/*vecMeshComponent_sptr MeshGenerator::genComponents (const BallState& ball) {

    const glm::mat4 scaleMatrix = glm::scale(glm::vec3(ball.getRadius()));
    const Sphere sphere;

    MeshComponent_sptr component = std::make_shared <MeshComponent>
                                       (std::make_shared <Sphere>(sphere, scaleMatrix),
                                       std::make_shared <BallAnimation>(ball));
    return vecMeshComponent_sptr { std::move(component) };
}*/
