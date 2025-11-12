/*
 * File: ClassicalMechanicsFactory.h
 * Author: Morgenthaler S
 * Description: Concrete factory for creating ClassicalMechanics-based physics engines
 */

#ifndef CLASSICALMECHANICSFACTORY_H
#define CLASSICALMECHANICSFACTORY_H

#include "physics/IPhysicsEngineFactory.h"

/**
 * @brief Concrete factory that creates ClassicalMechanicsAdapter instances
 *
 * This factory is instantiated in main.cpp and injected into the core
 * game logic, completing the dependency inversion.
 */
class ClassicalMechanicsFactory : public IPhysicsEngineFactory {
public:
    ClassicalMechanicsFactory() = default;
    ~ClassicalMechanicsFactory() override = default;

    IPhysicsEngine_uptr createPhysicsEngine(
        float ballRadius,
        float speedCoefficient
    ) const override;

    IPhysicsEngine_uptr createPhysicsEngine(
        float ballRadius,
        float jumpDistance,
        float v0y,
        float speedCoefficient
    ) const override;
};

#endif /* CLASSICALMECHANICSFACTORY_H */

