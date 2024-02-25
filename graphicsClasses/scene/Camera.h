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

    const glm::vec3& pos() const noexcept;

    glm::mat4 viewProjection() const noexcept;

    void update(
        const Player::Status &status,
        bool goAbove
    ) noexcept;

    const Movement& getMovement() const noexcept;

    void setRatio(float ratio);

    static glm::mat4 genVPMatrixFromStar(const Star &star);

private:
    struct CameraLocalInformation {
        /**
         * \brief Initial distance behind the ball where the camera is placed.
         */
        float distanceBehind;

        /**
         * \brief Initial distance above the ball where the camera is placed.
         */
        float distanceAbove;

        /**
         * \brief Additional offset behind the initial position to make a good visibility.
         */
        float rearOffset;

        /**
         * \brief Additional offset above the initial position to make a good visibility.
         */
        float frontOffset;

        /**
         * \brief Perpective matrix of the camera to apply in shaders.
         */
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

    /**
     * \brief Distance in front of the ball where the camera is looking at.
     */
    static constexpr float targetDistance = 2.f;

    static constexpr float defaultZNear = 0.1f;
};

#endif /* CAMERA_H */
