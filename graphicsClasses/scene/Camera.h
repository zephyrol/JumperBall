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
    struct IntrinsicProperties {
        const float above;
        const float behind;
        const float targetDistance;
        const float halfMinFov;
        const float zNear;
        const glm::mat4 perspectiveMatrix;
    };

    float getAboveWay() const;

    void turningAroundMapUpdate() noexcept;

    void followingBallUpdate() noexcept;

    bool approachingBallUpdate() noexcept;

    static IntrinsicProperties getIntrinsicProperties(float ratio, float zFar);

    struct ControlPoint {
        const float t;
        const std::vector<float> values;
    };

    static std::vector<float> catmullRomSpline(const std::vector<ControlPoint>& controlPoints, float t);

    const Map& _map;
    const CstChronometer_sptr _chronometer;
    const float _zFar;
    Movement _movement;
    std::unique_ptr<const IntrinsicProperties> _intrinsicProperties;
    glm::vec3 _pos;
    glm::vec3 _center;
    glm::vec3 _up;
    float _timePointComeBack;
    float _timePointGoAbove;
};

#endif /* CAMERA_H */
