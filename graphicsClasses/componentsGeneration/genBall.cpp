/*
 * file:   genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "../MeshGenerator.h"

std::vector<MeshComponent> MeshGenerator::genComponents(const Ball& ball) {

    const glm::mat4 scaleMatrix = glm::scale(glm::vec3(ball.getRadius()));
    const Sphere sphere;

    MeshComponent component ( std::make_shared<Sphere>(sphere,scaleMatrix),
                                std::make_shared<BallAnimation>(ball));
    return std::vector<MeshComponent> {component};
}
