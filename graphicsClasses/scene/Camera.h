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

    static constexpr float defaultZNear = 0.1f;

    const glm::vec3 &pos() const noexcept;

    glm::mat4 viewProjection() const noexcept;

    void update(
        const Player::Status &status,
        bool goAbove
    ) noexcept;

    const Movement &getMovement() const noexcept;

    void setRatio(float ratio);

    static glm::mat4 genVPMatrixFromStar(const Star &star);

private:

    struct CameraLocalInformation {
        float rearOffset;
        float frontOffset;
        glm::mat4 perspectiveMatrix;
    };

    void turningAroundMapUpdate() noexcept;

    void followingBallUpdate() noexcept;

    bool approachingBallUpdate() noexcept;

    CameraLocalInformation getCameraLocalInformation(float ratio) const noexcept;

    const Map &_map;
    const CstChronometer_sptr _chronometer;
    const float _defaultZFar;
    CameraLocalInformation _cameraLocalInformation;

    Movement _movement;
    glm::vec3 _pos;
    glm::vec3 _center;
    glm::vec3 _up;
    float _timePointComeBack;
    float _timePointGoAbove;

    static constexpr float distBehindBall = 1.8f;
    static constexpr float distAbove = 1.7f;
    static constexpr float distDirPoint = 2.f;

};

#endif /* CAMERA_H */
