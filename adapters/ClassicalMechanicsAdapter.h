/*
 * File: ClassicalMechanicsAdapter.h
 * Author: Morgenthaler S
 * Description: Adapter for ClassicalMechanics implementing IPhysicsEngine (DIP - Adapter pattern)
 */

#ifndef CLASSICALMECHANICSADAPTER_H
#define CLASSICALMECHANICSADAPTER_H

#include "physics/IPhysicsEngine.h"
#include "ClassicalMechanics.h"
#include <memory>

/**
 * @brief Adapter that wraps ClassicalMechanics to implement IPhysicsEngine
 *
 * This adapter allows the concrete ClassicalMechanics implementation
 * to be used through the IPhysicsEngine interface, inverting the dependency.
 */
class ClassicalMechanicsAdapter : public IPhysicsEngine {
public:
    /**
     * @brief Construct adapter with basic parameters
     */
    explicit ClassicalMechanicsAdapter(float ballRadius, float speedCoefficient);

    /**
     * @brief Construct adapter with custom jump parameters
     */
    ClassicalMechanicsAdapter(
        float ballRadius,
        float jumpDistance,
        float v0y,
        float speedCoefficient
    );

    ~ClassicalMechanicsAdapter() override = default;

    // IPhysicsEngine interface implementation
    float getJumpDistance() const override;
    Vector2D getVelocity(float t) const override;
    Vector2D getPosition(float t) const override;
    float getTimeToGetDestination() const override;
    void timesShock(const std::vector<float>& v) override;
    void addShockFromPosition(float pos) override;

private:
    ClassicalMechanics _mechanics;
};

#endif /* CLASSICALMECHANICSADAPTER_H */

