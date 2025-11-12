/*
 * File: ClassicalMechanicsFactory.cpp
 * Author: Morgenthaler S
 * Description: Implementation of ClassicalMechanicsFactory
 */

#include "ClassicalMechanicsFactory.h"
#include "ClassicalMechanicsAdapter.h"

IPhysicsEngine_uptr ClassicalMechanicsFactory::createPhysicsEngine(
    float ballRadius,
    float speedCoefficient
) const {
    return IPhysicsEngine_uptr(new ClassicalMechanicsAdapter(ballRadius, speedCoefficient));
}

IPhysicsEngine_uptr ClassicalMechanicsFactory::createPhysicsEngine(
    float ballRadius,
    float jumpDistance,
    float v0y,
    float speedCoefficient
) const {
    return IPhysicsEngine_uptr(new ClassicalMechanicsAdapter(
        ballRadius,
        jumpDistance,
        v0y,
        speedCoefficient
    ));
}

