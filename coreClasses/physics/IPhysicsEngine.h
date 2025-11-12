/*
 * File: IPhysicsEngine.h
 * Author: Morgenthaler S
 * Description: Physics engine interface (DIP - Domain layer abstraction)
 */

#ifndef IPHYSICSENGINE_H
#define IPHYSICSENGINE_H

#include <memory>
#include <vector>

/**
 * @brief Abstract interface for physics calculations
 *
 * This interface defines the contract for physics calculations without
 * depending on any concrete implementation. This follows the Dependency
 * Inversion Principle (DIP) where high-level modules (core game logic)
 * depend on abstractions rather than concrete implementations.
 */
class IPhysicsEngine {
public:
    struct Vector2D {
        float x;
        float y;
    };

    virtual ~IPhysicsEngine() = default;

    /**
     * @brief Get the jump distance
     */
    virtual float getJumpDistance() const = 0;

    /**
     * @brief Get velocity at time t
     */
    virtual Vector2D getVelocity(float t) const = 0;

    /**
     * @brief Get position at time t
     */
    virtual Vector2D getPosition(float t) const = 0;

    /**
     * @brief Get time to reach destination
     */
    virtual float getTimeToGetDestination() const = 0;

    /**
     * @brief Set shock times
     */
    virtual void timesShock(const std::vector<float>& v) = 0;

    /**
     * @brief Add shock from position
     */
    virtual void addShockFromPosition(float pos) = 0;
};

using IPhysicsEngine_uptr = std::unique_ptr<IPhysicsEngine>;
using IPhysicsEngine_sptr = std::shared_ptr<IPhysicsEngine>;
using CstIPhysicsEngine_sptr = std::shared_ptr<const IPhysicsEngine>;

#endif /* IPHYSICSENGINE_H */

