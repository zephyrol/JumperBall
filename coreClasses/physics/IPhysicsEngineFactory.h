/*
 * File: IPhysicsEngineFactory.h
 * Author: Morgenthaler S
 * Description: Physics engine factory interface (DIP - Abstract Factory pattern)
 */

#ifndef IPHYSICSENGINEFACTORY_H
#define IPHYSICSENGINEFACTORY_H

#include "IPhysicsEngine.h"
#include <memory>

/**
 * @brief Abstract factory for creating physics engine instances
 *
 * This factory interface allows the core game logic to create physics
 * engines without knowing the concrete implementation. The main.cpp
 * will inject a concrete factory implementation.
 */
class IPhysicsEngineFactory {
public:
    virtual ~IPhysicsEngineFactory() = default;

    /**
     * @brief Create a physics engine with basic parameters
     * @param ballRadius The radius of the ball
     * @param speedCoefficient The speed coefficient
     * @return A unique pointer to a physics engine instance
     */
    virtual IPhysicsEngine_uptr createPhysicsEngine(
        float ballRadius,
        float speedCoefficient
    ) const = 0;

    /**
     * @brief Create a physics engine with custom jump parameters
     * @param ballRadius The radius of the ball
     * @param jumpDistance The jump distance
     * @param v0y Initial vertical velocity
     * @param speedCoefficient The speed coefficient
     * @return A unique pointer to a physics engine instance
     */
    virtual IPhysicsEngine_uptr createPhysicsEngine(
        float ballRadius,
        float jumpDistance,
        float v0y,
        float speedCoefficient
    ) const = 0;
};

using IPhysicsEngineFactory_sptr = std::shared_ptr<IPhysicsEngineFactory>;
using CstIPhysicsEngineFactory_sptr = std::shared_ptr<const IPhysicsEngineFactory>;

#endif /* IPHYSICSENGINEFACTORY_H */

