/*
 * File: Camera.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <scene/Map.h>
#include <scene/Star.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "AboveMovingCamera.h"
#include "Utility.h"
#include "player/Player.h"

class Camera : public Displayable, public AboveMovingCamera {
   public:
    enum class Movement { TurningAroundMap, FollowingBall, ApproachingBall };

    Camera(const Map& map, float ratio);

    const glm::vec3& pos() const noexcept;

    glm::mat4 viewProjection() const noexcept;

    bool isMovingAbove() const override;

    void update(const Player::Status& status, bool goAbove) noexcept;

    const Movement& getMovement() noexcept;

    void setRatio(float ratio);

    static glm::mat4 genVPMatrixFromStar(const Star& star);

   private:
    struct Offset {
        float above;
        float targetDistance;
        float zNear;
        float halfMinFov;
    };

    float getAboveWay() const;

    void turningAroundMapUpdate() noexcept;

    void followingBallUpdate() noexcept;

    bool approachingBallUpdate() noexcept;

    static float getFovY(float ratio) noexcept;

    static Offset getOffset(float ratio);

    const Map& _map;
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

    static constexpr float behindCameraDistance = 1.8f;
};

#endif /* CAMERA_H */
