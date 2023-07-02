/*
 * file: genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "MeshGenerator.h"

MeshDynamicGroup_uptr MeshGenerator::genBall(const CstBall_sptr &ball, unsigned int ballSkin) {

    const auto getCustomColors = [&ballSkin]() -> std::vector<glm::vec3> {
        if (ballSkin == 0) {
            const glm::vec3 color1{32.0 / 255.0, 223.0 / 255.0, 123.0 / 255.0};
            return {color1, 1.f - color1};
        }
        if (ballSkin == 1) {
            const glm::vec3 color1{44.0 / 255.0, 211.0 / 255.0, 208.0 / 255.0};
            return {color1, 1.f - color1};
        }
        if (ballSkin == 2) {
            const glm::vec3 color1{4.0 / 255.0, 42.0 / 255.0, 213.0 / 255.0};
            return {color1, 1.f - color1};
        }
        if (ballSkin == 3) {
            return {
                {198.0 / 255.0, 218.0 / 255.0, 37.0 / 255.0},
                {37.0 / 255.0,  198.0 / 255.0, 218.0 / 255.0},
                {218.0 / 255.0, 37.0 / 255.0,  198.0 / 255.0}
            };
        }
        if (ballSkin == 4) {
            return {
                {223.0 / 255.0, 142.0 / 255.0, 32.0 / 255.0},
                {32.0 / 255.0,  223.0 / 255.0, 142.0 / 255.0},
                {142.0 / 255.0, 32.0 / 255.0,  223.0 / 255.0}
            };
        }
        const glm::vec3 tetradricFirstColor{34.0 / 255.0, 221.0 / 255.0, 213.0 / 255.0};
        const glm::vec3 tetradricSecondColor{119.0 / 255.0, 34.0 / 255.0, 221.0 / 255.0};
        return {
            tetradricFirstColor,
            tetradricSecondColor,
            1.f - tetradricFirstColor,
            1.f - tetradricSecondColor
        };
    };

    const CstGeometricShape_sptr sphere = std::make_shared<const Sphere>(getCustomColors());
    vecCstGeometricShape_sptr geometricShapes{sphere};
    Mesh_sptr sphereMesh = std::make_shared<Mesh>(ball, std::move(geometricShapes), 0);

    return std::unique_ptr<MeshDynamicGroup>(new MeshDynamicGroup(
        {{sphereMesh}}
    ));
}
