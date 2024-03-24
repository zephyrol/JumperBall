/*
 * File: Camera.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <scene/Star.h>
#include <scene/Map.h>
#include "player/Player.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Utility.h"

class Camera : public Displayable {
public:
    enum class Movement {
        TurningAroundMap, FollowingBall, ApproachingBall
    };

    Camera(const Map &map, float ratio);

    const glm::vec3 &pos() const noexcept;

    glm::mat4 viewProjection() const noexcept;

    void update(
        const Player::Status &status,
        bool goAbove
    ) noexcept;

    const Movement &getMovement() noexcept;

    void setRatio(float ratio);

    static glm::mat4 genVPMatrixFromStar(const Star &star);

private:
    struct Offset {
        float behind;
        float above;
        float zNear;
        float halfMinFov;
    };

    void turningAroundMapUpdate() noexcept;

    void followingBallUpdate() noexcept;

    bool approachingBallUpdate() noexcept;

    static float getFovY(float ratio) noexcept;

    static Offset getOffset(float ratio);

    const Map &_map;
    const CstChronometer_sptr _chronometer;
    const float _zFar;
    Movement _movement;
    float _fovY;
    Offset _offset;
    glm::vec3 _pos;
    glm::vec3 _center;
    glm::vec3 _up;
    float _timePointComeBack;
    float _timePointGoAbove;
    glm::mat4 _perspectiveMatrix;

    static constexpr float targetDistance = 2.f;
};

#endif /* CAMERA_H */
