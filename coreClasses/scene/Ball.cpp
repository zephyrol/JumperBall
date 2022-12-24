/*
 * File: Ball.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:17
 */

#include "Ball.h"
#include "system/SoundOutput.h"

Ball::Ball(unsigned int x, unsigned int y, unsigned int z, const CstDoubleChronometer_sptr &doubleChronometer):
    _creationChronometer(doubleChronometer->first()),
    _inGameChronometer(doubleChronometer->second()),
    _pos({x, y, z}),
    _currentSide(JBTypes::Dir::Up),
    _lookTowards(JBTypes::Dir::North),
    _3DPos(get3DPosStayingBall()),
    _state(Ball::State::Staying),
    _stateOfLife(Ball::StateOfLife::Normal),
    _jumpingType(Ball::JumpingType::Short),
    _mechanicsPatternJumping(getRadius()),
    _mechanicsPatternLongJumping(getRadius(), 3.f, 4.2f),
    _mechanicsPatternFalling(getRadius(), 0.f, 0.f),
    _actionTime(0.f),
    _stateOfLifeTime(0.f),
    _burnCoefficientTrigger(0.f),
    _burnCoefficientCurrent(0.f),
    _teleportationColor(JBTypes::Color::None),
    _teleportationCoefficient(0.f),
    _jumpRequest(false),
    _jumpRequestTime(),
    _currentCoveredRotation(JBTypesMethods::createQuaternion({0.f, 0.f, 0.f}, 1.f)),
    _currentCrushing(0.f),
    _turnLeftMovement(),
    _turnRightMovement(),
    _nextBlockGetter(),
    _turnBackMovement(),
    _movementDestination(),
    _blocksPositions(nullptr),
    _blockWithInteractions(nullptr),
    _blocksTeleportations(nullptr),
    _nbOfKeys(0),
    _nbOfCoins(0),
    _updateOutputs{}
    {
}

void Ball::turnLeft() noexcept {
    _lookTowards = _turnLeftMovement.evaluate({_currentSide, _lookTowards});
    _state = Ball::State::TurningLeft;
    setActionTimeNow();
}

void Ball::turnRight() noexcept {
    _lookTowards = _turnRightMovement.evaluate({_currentSide, _lookTowards});
    _state = Ball::State::TurningRight;
    setActionTimeNow();
}

const JBTypes::vec3f &Ball::get3DPosition() const noexcept {
    return _3DPos;
}

void Ball::goStraightAhead() noexcept {
    _pos = _movementDestination.pos;
    _currentSide = _movementDestination.nextSide;
    _lookTowards = _movementDestination.nextLook;

    applyRotation();
}

void Ball::jump() noexcept {
    _state = Ball::State::Jumping;
    setActionTimeNow();
    isGoingStraightAheadIntersectBlock();
}

void Ball::stay() noexcept {
    _jumpingType = Ball::JumpingType::Short;
    _state = Ball::State::Staying;
    _3DPos = get3DPosStayingBall();
    setActionTimeNow();
}

void Ball::fall() noexcept {
    _state = Ball::State::Falling;
    setActionTimeNow();
}

void Ball::teleport(const JBTypes::Color &col) noexcept {
    _state = Ball::State::Teleporting;
    _teleportationColor = col;
    setActionTimeNow();
}

void Ball::deteleport() noexcept {
    _state = Ball::State::Deteleporting;
    setActionTimeNow();
}

void Ball::move() noexcept {

    updateMovements();
    if (_movementDestination.nextLocal != Ball::NextDestination::None) {
        _state = Ball::State::Moving;
        setActionTimeNow();
    }
}

void Ball::doAction(Ball::ActionRequest action) {
    if (_stateOfLife == Ball::StateOfLife::Bursting) {
        return;
    }
    switch (action) {
        case Ball::ActionRequest::Nothing:
            break;
        case Ball::ActionRequest::GoStraightAhead:
            if (_state == Ball::State::Staying) {
                move();
            }
            break;
        case Ball::ActionRequest::TurnLeft:
            if (_state == Ball::State::Staying) {
                turnLeft();
            }
            break;
        case Ball::ActionRequest::TurnRight:
            if (_state == Ball::State::Staying) {
                turnRight();
            }
            break;
        case Ball::ActionRequest::MoveCamera:
            break;
        case Ball::ActionRequest::Jump:
            if (_state == Ball::State::Staying) {
                _state = Ball::State::Jumping;
                jump();
            } else {
                _jumpRequest = true;
                _jumpRequestTime = _inGameChronometer->getTime();
            }
            break;
        default:
            break;
    }
}

void Ball::isGoingStraightAheadIntersectBlock() noexcept {

    if (_state != Ball::State::Jumping) {
        return;
    }
    const auto sideVec = JBTypesMethods::directionAsVectorInt(_currentSide);
    const auto lookVec = JBTypesMethods::directionAsVectorInt(_lookTowards);
    const auto offsetPosX = _pos.at(0) + sideVec.at(0);
    const auto offsetPosY = _pos.at(1) + sideVec.at(1);
    const auto offsetPosZ = _pos.at(2) + sideVec.at(2);

    const auto aboveNearX = offsetPosX + lookVec.at(0);
    const auto aboveNearY = offsetPosY + lookVec.at(1);
    const auto aboveNearZ = offsetPosZ + lookVec.at(2);
    const CstBlock_sptr blockNear = getBlock({aboveNearX, aboveNearY, aboveNearZ});

    ClassicalMechanics &refMechanicsJumping = getMechanicsJumping();
    constexpr float sizeBlock = 1.f;
    const float sizeBlock2MinusRadius = sizeBlock / 2.f - getRadius();

    if (blockNear && blockNear->isExists()) {
        refMechanicsJumping.addShockFromPosition(sizeBlock2MinusRadius);
        return;
    }

    const auto aboveFarX = offsetPosX + 2 * lookVec.at(0);
    const auto aboveFarY = offsetPosY + 2 * lookVec.at(1);
    const auto aboveFarZ = offsetPosZ + 2 * lookVec.at(2);
    const CstBlock_sptr blockFar = getBlock({aboveFarX, aboveFarY, aboveFarZ});

    if (blockFar && blockFar->isExists()) {
        constexpr float offsetFar = 1.f;
        refMechanicsJumping.addShockFromPosition(sizeBlock2MinusRadius + offsetFar);
        return;
    }

    const auto aboveVeryFarX = offsetPosX + 3 * lookVec.at(0);
    const auto aboveVeryFarY = offsetPosY + 3 * lookVec.at(1);
    const auto aboveVeryFarZ = offsetPosZ + 3 * lookVec.at(2);
    const CstBlock_sptr blockVeryFar = getBlock({aboveVeryFarX, aboveVeryFarY, aboveVeryFarZ});

    if (_jumpingType == Ball::JumpingType::Long && blockVeryFar && blockVeryFar->isExists()) {
        constexpr float offsetVeryFar = 2.f;
        refMechanicsJumping.addShockFromPosition(sizeBlock2MinusRadius + offsetVeryFar);
        return;
    }
    refMechanicsJumping.timesShock({});
}

const ClassicalMechanics &Ball::getMechanicsJumping() const noexcept {
    if (_jumpingType == Ball::JumpingType::Short) {
        return _mechanicsPatternJumping;
    } else {
        return _mechanicsPatternLongJumping;
    }
}

const ClassicalMechanics &Ball::getMechanicsFalling() const noexcept {
    return _mechanicsPatternFalling;
}

JBTypes::Quaternion Ball::getCoveredRotation() const noexcept {

    constexpr float sizeBlock = 1.f; // TODO: use a member in Block or map class

    const std::function<float()> getCoveredDistance =
        [this]() -> float {
            if (_state == Ball::State::Moving) {
                const float timeSecondsSinceAction = getTimeSecondsSinceAction();
                return timeSecondsSinceAction / Ball::timeToGetNextBlock;
            } else if (_state == Ball::State::Jumping) {
                return getJumpingPosX();
            } else if (_state == Ball::State::Falling) {
                return getFallingPosX();
            } else {
                return 0.f;
            }
        };

    const float coveredDistance = getCoveredDistance();
    const float angle = sizeBlock * coveredDistance / getRadius();

    const JBTypes::Quaternion movementRotation = JBTypesMethods::createRotationQuaternion(
        getRotationAxis(),
        angle
    );
    return JBTypesMethods::multiply(movementRotation, _currentCoveredRotation);
}

float Ball::getCrushingCoefficient() const noexcept {

    if (_stateOfLife == Ball::StateOfLife::Sliding) {
        return 0.f;
    }

    if (_state == Ball::State::Staying) {

        const std::function<float()> stayCrushingCoeff =
            [this]() -> float {
                constexpr float durationWaitingCrushing = 0.7f;
                const float angleInCosinusFunc = getTimeSecondsSinceAction() * 2.f *
                                                 static_cast <float>(M_PI) / durationWaitingCrushing;
                return 0.5f - cosf(angleInCosinusFunc) / 2.f;
            };

        return stayCrushingCoeff();
    }

    const std::function<float(float)> movementCrushingCoeff =
        [this](float timeToDoMovement) -> float {
            const float tMax = getTimeSecondsSinceAction() / timeToDoMovement;
            const float t = tMax > 1.f
                            ? 1.f
                            : tMax;
            return (1.f - t) * _currentCrushing;
        };

    if (_state == Ball::State::Moving || _state == Ball::State::Jumping) {
        return movementCrushingCoeff(timeToGetNextBlock);
    }
    if (_state == Ball::State::TurningLeft || _state == Ball::State::TurningRight) {
        return movementCrushingCoeff(timeToTurn);
    }
    return 0.f;
}

float Ball::getTeleportationCoefficient() const noexcept {
    return _teleportationCoefficient;
}

const JBTypes::Color &Ball::getTeleportationColor() const noexcept {
    return _teleportationColor;
}

void Ball::isFallingIntersectionBlock() noexcept {
    // TODO Get time since action should be used only one at the beginning of the update
    const float fDifference = getTimeSecondsSinceAction();
    const bool descendingJumpPhase = _state == Ball::State::Jumping &&
                                     _mechanicsPatternJumping.getVelocity(fDifference).y < 0;
    // TODO update it
    const auto positionBlockPtr = intersectBlock();
    if (!positionBlockPtr || (!descendingJumpPhase && _state != Ball::State::Falling)) {
        return;
    }
    if (descendingJumpPhase) {
        const float coveredDistance = roundf(getMechanicsJumping().getPosition(
            getMechanicsJumping().getTimeToGetDestination()
        ).x);
        const float absCoveredDistance = fabsf(coveredDistance);
        const bool inverseRotation = coveredDistance < 0.f;
        const auto distance = static_cast<size_t>(absCoveredDistance);
        for (size_t i = 0; i < distance; ++i) {
            applyRotation(inverseRotation);
        }
    }
    getMechanicsJumping().timesShock({});
    _pos = {positionBlockPtr->at(0), positionBlockPtr->at(1), positionBlockPtr->at(2)};
    stay();
    blockEvent();
    internalUpdate();
}

JBTypes::vec3f Ball::P2DTo3D(ClassicalMechanics::physics2DVector p2D) const {
    const float offsetRealPosition = 0.5f + getRadius();

    const JBTypes::vec3f sideVec = JBTypesMethods::directionAsVector(_currentSide);
    const JBTypes::vec3f lookVec = JBTypesMethods::directionAsVector(_lookTowards);

    const auto x = static_cast <float>(_pos.at(0)) +
                    sideVec.x * (offsetRealPosition + p2D.y) + lookVec.x * p2D.x;
    const auto y = static_cast <float>(_pos.at(1)) +
                    sideVec.y * (offsetRealPosition + p2D.y) + lookVec.y * p2D.x;
    const auto z = static_cast <float>(_pos.at(2)) +
                   sideVec.z * (offsetRealPosition + p2D.y) + lookVec.z * p2D.x;

    return JBTypes::vec3f{x, y, z};
}

float Ball::getRadius() {
    return Ball::basicRadius;
}

JBTypes::Dir Ball::currentSide() const {
    return _currentSide;
}

JBTypes::Dir Ball::lookTowards() const {
    return _lookTowards;
}

JBTypes::vec3f Ball::lookTowardsAsVector() const {
    return JBTypesMethods::directionAsVector(_lookTowards);
}

JBTypes::vec3f Ball::currentSideAsVector() const {
    return JBTypesMethods::directionAsVector(_currentSide);
}

ClassicalMechanics &Ball::getMechanicsJumping() noexcept {
    // Scott Meyer's advice to avoid code duplication
    return const_cast <ClassicalMechanics &>(static_cast <const Ball &>(*this).getMechanicsJumping());
}

JBTypes::vec3f Ball::get3DPosStayingBall() const {
    const float offsetPosition = 0.5f + getRadius();

    const JBTypes::vec3f sideVec = JBTypesMethods::directionAsVector(_currentSide);
    const auto x = static_cast <float>(_pos.at(0)) + sideVec.x * offsetPosition;
    const auto y = static_cast <float>(_pos.at(1)) + sideVec.y * offsetPosition;
    const auto z = static_cast <float>(_pos.at(2)) + sideVec.z * offsetPosition;

    return {x, y, z};
}


void Ball::blockEvent() noexcept {

    if (_stateOfLife == StateOfLife::Bursting) {
        return;
    }

    // TODO: create attribute strPosition if used more that once
    const std::string blockPosition = Block::positionToString(_pos);
    if (_blocksPositions->find(blockPosition) == _blocksPositions->end()) {
        return;
    }
    const auto &block = _blocksPositions->at(blockPosition);

    const Block::Effect effect = block->detectionEvent();

    if (effect == Block::Effect::FinishLevel) {
        _stateOfLife = StateOfLife::Winner;
        return;
    }
    if (effect == Block::Effect::Jump) {
        _stateOfLife = StateOfLife::Normal;
        _jumpingType = Ball::JumpingType::Long;
        jump();
        return;
    }
    if (effect == Block::Effect::Burn) {
        _burnCoefficientCurrent += .2f;
        _burnCoefficientTrigger = _burnCoefficientCurrent;
        _stateOfLife = StateOfLife::Burning;
        setLifeTimeNow();
        return;
    }
    const auto hasToJump = _jumpRequest
        && (_inGameChronometer->getTime() - _jumpRequestTime) < timeToGetNextBlock;
    if (hasToJump) {
        _jumpRequest = false;
        jump();
    }
    if (effect == Block::Effect::Slide) {
        _burnCoefficientTrigger = 0.f;
        _stateOfLife = StateOfLife::Sliding;
        setLifeTimeNow();
        if (!hasToJump) {
            move();
        }
        internalUpdate();
        return;
    }
    _stateOfLife = StateOfLife::Normal;
}

void Ball::jumpingUpdate() noexcept {
    const ClassicalMechanics::physics2DVector pos2D =
        getMechanicsJumping().getPosition(getTimeSecondsSinceAction());

    const JBTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
    _3DPos = relativePositionJump;
    isFallingIntersectionBlock();
}

void Ball::fallingUpdate() noexcept {
    const ClassicalMechanics::physics2DVector pos2D = _mechanicsPatternFalling.getPosition(
        getTimeSecondsSinceAction()
    );

    const JBTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
    _3DPos = relativePositionJump;
    isFallingIntersectionBlock();
}

void Ball::stayingUpdate() noexcept {
    const auto block = getBlock(_pos);
    if (block && block->isExists()) {
        return;
    }
    fall();
    internalUpdate();
}

void Ball::turningUpdate() noexcept {
    const float sSinceAction = getTimeSecondsSinceAction();
    if (sSinceAction >= (timeToTurn)) {
        stay();
        internalUpdate();
        return;
    }
}

void Ball::movingUpdate() noexcept {
    const float sSinceAction = getTimeSecondsSinceAction();
    if (sSinceAction >= timeToGetNextBlock) {
        goStraightAhead();
        stay();
        blockEvent();
        internalUpdate();
        return;
    }

    const JBTypes::vec3f position3D = get3DPosStayingBall();
    if (_movementDestination.nextLocal == Ball::NextDestination::InFrontOf) {
        _3DPos.x = (sSinceAction * (static_cast <float>(_movementDestination.pos.at(0))
                                    - static_cast <float>(_pos.at(0)))
                    / timeToGetNextBlock) + position3D.x;
        _3DPos.y = (sSinceAction * (static_cast <float>(_movementDestination.pos.at(1))
                                    - static_cast <float>(_pos.at(1)))
                    / timeToGetNextBlock) + position3D.y;
        _3DPos.z = (sSinceAction * (static_cast <float>(_movementDestination.pos.at(2))
                                    - static_cast <float>(_pos.at(2)))
                    / timeToGetNextBlock) + position3D.z;
        return;
    }
    if (_movementDestination.nextLocal == Ball::NextDestination::Same ||
        _movementDestination.nextLocal == Ball::NextDestination::Above) {

        const float distancePerStep = _movementDestination.nextLocal == Ball::NextDestination::Same
                                      ? 0.5f + getRadius()
                                      : 0.5f - getRadius();

        const float halfTimeToGetNextBlock = timeToGetNextBlock / 2.f;

        const float timeStep1 = sSinceAction > halfTimeToGetNextBlock
                                ? halfTimeToGetNextBlock
                                : sSinceAction;

        const float timeStep2 = sSinceAction - halfTimeToGetNextBlock > 0.f
                                ? sSinceAction - halfTimeToGetNextBlock
                                : 0.f;

        const JBTypes::vec3f lookVec = JBTypesMethods::directionAsVector(_lookTowards);
        const JBTypes::vec3f nextLookVec = JBTypesMethods::directionAsVector(_movementDestination.nextLook);

        const float distStep1 = distancePerStep * timeStep1 / halfTimeToGetNextBlock;
        const float distStep2 = distancePerStep * timeStep2 / halfTimeToGetNextBlock;
        _3DPos = {
            position3D.x + lookVec.x * distStep1 + nextLookVec.x * distStep2,
            position3D.y + lookVec.y * distStep1 + nextLookVec.y * distStep2,
            position3D.z + lookVec.z * distStep1 + nextLookVec.z * distStep2
        };
        return;
    }
    _3DPos = position3D;
}

void Ball::burningUpdate() noexcept {
    if (_stateOfLife == StateOfLife::Bursting) {
        return;
    }

    const float time = getTimeSecondsSinceStateOfLife();
    if (
        _stateOfLife == StateOfLife::Burning &&
        (_state == State::Staying || _state == State::TurningLeft ||
         _state == State::TurningRight)
        ) {
        // the ball is burning
        _burnCoefficientCurrent = _burnCoefficientTrigger + (time / timeToBurn);
        if (_burnCoefficientCurrent > 1.f) {
            _stateOfLife = StateOfLife::Bursting;
            setLifeTimeNow();
        }
        return;
    }
    // the ball is cooling down
    if (_stateOfLife == StateOfLife::Burning) {
        _stateOfLife = StateOfLife::Normal;
        setLifeTimeNow();
        _burnCoefficientTrigger = _burnCoefficientCurrent;
    }
    _burnCoefficientCurrent = _burnCoefficientTrigger - (time / timeToBurn);
    if (_burnCoefficientCurrent < 0.f) {
        _burnCoefficientCurrent = 0.f;
    }

}

Ball::MovementDestination Ball::getNextBlockInfo() const {

    const JBTypes::Dir lookTowards = _lookTowards;
    const JBTypes::Dir currentSide = _currentSide;
    const std::array<int, 12> offsetsNextBlocks = _nextBlockGetter.evaluate(
        {currentSide, lookTowards}
    );

    const auto getNeighbor = [this, offsetsNextBlocks](size_t n) -> JBTypes::vec3ui {
        const size_t offset = 3 * n;
        return {
            _pos.at(0) + offsetsNextBlocks.at(offset),
            _pos.at(1) + offsetsNextBlocks.at(offset + 1),
            _pos.at(2) + offsetsNextBlocks.at(offset + 2)
        };
    };

    const auto inFrontOf = getNeighbor(0);
    const auto left = getNeighbor(1);
    const auto right = getNeighbor(2);
    const auto above = getNeighbor(3);

    const CstBlock_sptr &blockAbove = getBlock(above);
    const CstBlock_sptr &blockInFrontOf = getBlock(inFrontOf);
    const CstBlock_sptr &blockLeft = getBlock(left);
    const CstBlock_sptr &blockRight = getBlock(right);

    Ball::MovementDestination nextBlock{};
    if (blockAbove && blockAbove->isExists()) {
        nextBlock.pos = above;
        nextBlock.nextLocal = Ball::NextDestination::Above;

        const JBTypes::Dir lookTowardsBeforeMovement = lookTowards;
        nextBlock.nextLook = currentSide;
        nextBlock.nextSide = _turnBackMovement.evaluate({lookTowardsBeforeMovement});
    } else if (blockInFrontOf && blockInFrontOf->isExists()) {
        nextBlock.pos = inFrontOf;
        nextBlock.nextLocal = Ball::NextDestination::InFrontOf;
        nextBlock.nextLook = lookTowards;
        nextBlock.nextSide = currentSide;
    } else if ((!blockLeft || !blockLeft->isExists()) && (!blockRight || !blockRight->isExists())) {
        nextBlock.pos = _pos;
        nextBlock.nextLocal = Ball::NextDestination::Same;
        const JBTypes::Dir sideBeforeMovement = currentSide;
        nextBlock.nextSide = lookTowards;
        nextBlock.nextLook = _turnBackMovement.evaluate({sideBeforeMovement});
    } else if (_stateOfLife == Ball::StateOfLife::Sliding) {
        nextBlock.pos = inFrontOf;
        nextBlock.nextLocal = Ball::NextDestination::InFrontOf;
        nextBlock.nextLook = lookTowards;
        nextBlock.nextSide = currentSide;
    } else {
        nextBlock.pos = _pos;
        nextBlock.nextLocal = Ball::NextDestination::None;
        nextBlock.nextSide = currentSide;;
        nextBlock.nextLook = lookTowards;
    }
    return nextBlock;
}

void Ball::teleportingUpdate() noexcept {

    const float timeSinceAction = getTimeSecondsSinceAction();
    if (timeSinceAction < Ball::halfTimeTeleportationDuration) {
        _teleportationCoefficient = timeSinceAction / Ball::halfTimeTeleportationDuration;
        return;
    }

    const auto currentBlock = getCurrentBlock();
    const auto destination = _blocksTeleportations->at(
        BlockDir(currentBlock, _currentSide)
    );
    _pos = destination.first->position();
    const auto destinationDir = destination.second;

    if (_currentSide != destinationDir) {
        const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(destinationDir);
        _lookTowards = JBTypesMethods::vectorAsDirection(
            JBTypesMethods::cross(vecDir, {vecDir.y, -vecDir.x, 0.f})
        );
    }
    _currentSide = destinationDir;
    _teleportationCoefficient = 1.f;

    stay();
    internalUpdate();
    deteleport();
    // internalUpdate();
}

void Ball::deteleportingUpdate() noexcept {

    const float timeSinceAction = getTimeSecondsSinceAction();
    if (timeSinceAction > Ball::halfTimeTeleportationDuration) {
        _teleportationCoefficient = 0.f;
        _teleportationColor = JBTypes::Color::None;
        stay();
        internalUpdate();
        return;
    }
    _teleportationCoefficient = 1.f - (timeSinceAction / halfTimeTeleportationDuration);
}

void Ball::applyRotation(bool inverse) {

    constexpr float sizeBlock = 1.f; // TODO: use a member in Block or map class
    const float angleToGetBlock = sizeBlock / getRadius();

    _currentCoveredRotation = JBTypesMethods::multiply(
        JBTypesMethods::createRotationQuaternion(
            inverse ? getInverseRotationAxis() : getRotationAxis(),
            angleToGetBlock
        ),
        _currentCoveredRotation
    );
}

JBTypes::vec3f Ball::getRotationAxis() const noexcept {
    return JBTypesMethods::cross(
        JBTypesMethods::directionAsVector(_currentSide),
        JBTypesMethods::directionAsVector(_lookTowards)
    );
}

JBTypes::vec3f Ball::getInverseRotationAxis() const noexcept {
    return JBTypesMethods::cross(
        JBTypesMethods::directionAsVector(_lookTowards),
        JBTypesMethods::directionAsVector(_currentSide)
    );
}

float Ball::getJumpingPosX() const noexcept {
    return getMechanicsJumping().getPosition(getTimeSecondsSinceAction()).x;
}

float Ball::getFallingPosX() const noexcept {
    return getMechanicsFalling().getPosition(getTimeSecondsSinceAction()).x;
}

void Ball::update(const Ball::ActionRequest &action) noexcept {
    doAction(action);
    internalUpdate();
}


bool Ball::isBurstingFinished() const {
    return (_stateOfLife == Ball::StateOfLife::Bursting) && (getTimeSecondsSinceStateOfLife() > timeToBurst);
}


Ball::State Ball::state() const {
    return _state;
}

Ball::StateOfLife Ball::stateOfLife() const {
    return _stateOfLife;
}

float Ball::burnCoefficient() const {
    return _burnCoefficientCurrent;
}

void Ball::die() noexcept {
    _stateOfLife = Ball::StateOfLife::Dead;
}

float Ball::getTimeToGetDestination() const {
    return getMechanicsJumping().getTimeToGetDestination();
}

Displayable::DynamicValues<float> Ball::getDynamicFloats() const {

    const auto getStateOfLifeStatus = [this]() {
        if (
            _stateOfLife == Ball::StateOfLife::Normal ||
            _stateOfLife == Ball::StateOfLife::Burning ||
            _stateOfLife == Ball::StateOfLife::Sliding
            ) {
            return 0.f;
        }
        if (_stateOfLife == Ball::StateOfLife::Bursting) {
            return 1.f;
        }
        return 2.f;
    };

    return {
        {"ballRadius",      getRadius()},
        {"crushingCoeff",   _currentCrushing},
        {"status",          getStateOfLifeStatus()},
        {"timeStateOfLife", getTimeSecondsSinceStateOfLife()},
        {"burningCoeff",    burnCoefficient()}
    };
}

Displayable::DynamicValues<JBTypes::vec3f> Ball::getDynamicVec3fs() const {
    return {
        {"sideDir",  currentSideAsVector()},
        {"position", get3DPosition()}
    };
}

Displayable::DynamicValues<JBTypes::Quaternion> Ball::getDynamicQuaternions() const {
    return {
        {"quaternion", getCoveredRotation()}
    };
}

Displayable::GlobalState Ball::getGlobalState() const {
    if (_stateOfLife == Ball::StateOfLife::Bursting) {
        return Displayable::GlobalState::Separate;
    }
    if (_stateOfLife == Ball::StateOfLife::Dead) {
        return Displayable::GlobalState::Dead;
    }
    return Displayable::GlobalState::United;
}

const JBTypes::vec3ui &Ball::getPosition() const noexcept {
    return _pos;
}

void Ball::interaction() {

    if (_stateOfLife == Ball::StateOfLife::Bursting || _stateOfLife == Ball::StateOfLife::Dead) {
        return;
    }
    Block::Effect finalBlockEffect = Block::Effect::Nothing;

    for (const auto &block: *_blockWithInteractions) {
        const auto blockEffect = block->interaction();
        if (blockEffect != Block::Effect::Nothing) {
            finalBlockEffect = blockEffect;
        }
    }
    if (finalBlockEffect == Block::Effect::Burst) {
        _stateOfLife = Ball::StateOfLife::Bursting;
        addUpdateOutput(std::make_shared<SoundOutput>("ballIsBursting"));
        setLifeTimeNow();
        internalUpdate();
    }
}

void Ball::internalUpdate() noexcept {

    if (_stateOfLife == Ball::StateOfLife::Dead || _stateOfLife == Ball::StateOfLife::Winner) {
        return;
    }

    switch (_state) {
        case Ball::State::Falling:
            fallingUpdate();
            break;
        case Ball::State::Jumping:
            jumpingUpdate();
            break;
        case Ball::State::TurningLeft:
            turningUpdate();
            break;
        case Ball::State::Staying:
            stayingUpdate();
            break;
        case Ball::State::TurningRight:
            turningUpdate();
            break;
        case Ball::State::Moving:
            movingUpdate();
            break;
        case Ball::State::Teleporting:
            teleportingUpdate();
            break;
        case Ball::State::Deteleporting:
            deteleportingUpdate();
            break;
        default:
            break;
    }
    interaction();

    _currentCrushing = getCrushingCoefficient();
    burningUpdate();
    if (isBurstingFinished()) {
        die();
    }

}

void Ball::setBlockPositions(
    const std::shared_ptr<const std::unordered_map<std::string, Block_sptr>> &blocksPositions
) {
    _blocksPositions = blocksPositions;
}

CstBlock_sptr Ball::getBlock(const JBTypes::vec3ui &pos) const {
    const std::string strPos = Block::positionToString(pos);
    const auto blockPositionIterator = _blocksPositions->find(strPos);
    return blockPositionIterator != _blocksPositions->end()
           ? blockPositionIterator->second
           : nullptr;
}

JBTypes::vec3f Ball::getNextLook() const {
    return JBTypesMethods::directionAsVector(_movementDestination.nextLook);
}

std::shared_ptr<const JBTypes::vec3ui> Ball::intersectBlock() const {

    const JBTypes::vec3f sideVec = currentSideAsVector();
    const float offsetBlockPosition = getRadius();

    const float xIntersectionUnder = _3DPos.x - sideVec.x * offsetBlockPosition;
    const float yIntersectionUnder = _3DPos.y - sideVec.y * offsetBlockPosition;
    const float zIntersectionUnder = _3DPos.z - sideVec.z * offsetBlockPosition;

    constexpr float offset = 0.5f;
    const auto xInteger = static_cast <unsigned int>(xIntersectionUnder + offset);
    const auto yInteger = static_cast <unsigned int>(yIntersectionUnder + offset);
    const auto zInteger = static_cast <unsigned int>(zIntersectionUnder + offset);

    const CstBlock_sptr &block = getBlock({xInteger, yInteger, zInteger});

    return (block && block->isExists())
           ? std::make_shared<const JBTypes::vec3ui>(block->position())
           : nullptr;
}

void Ball::updateMovements() {
    _movementDestination = getNextBlockInfo();
}

void Ball::setBlockWithInteractions(const std::shared_ptr<const vecBlock_sptr> &blocksWithInterraction) {
    _blockWithInteractions = blocksWithInterraction;
}

void Ball::setBlockTeleportations(
    const std::shared_ptr<const std::map<BlockDir, BlockDir>> &blocksTeleportations
) {
    _blocksTeleportations = blocksTeleportations;
}

Block_sptr Ball::getCurrentBlock() {
    const std::string strPos = Block::positionToString(_pos);
    return _blocksPositions->at(strPos);
}

const std::shared_ptr<const vecBlock_sptr> &Ball::getBlocksWithInteraction() {
    return _blockWithInteractions;
}

void Ball::obtainKey() {
    _updateOutputs.push_back(std::make_shared<SoundOutput>("keyIsObtained"));
    ++_nbOfKeys;
}

void Ball::obtainCoin() {
    _updateOutputs.push_back(std::make_shared<SoundOutput>("coinIsObtained"));
    ++_nbOfCoins;
}

unsigned int Ball::numberOfKeys() const {
    return _nbOfKeys;
}

unsigned int Ball::numberOfCoins() const {
    return _nbOfCoins;
}

const CstChronometer_sptr &Ball::getCreationChronometer() const {
    return _creationChronometer;
}

const CstChronometer_sptr &Ball::getInGameChronometer() const {
    return _inGameChronometer;
}

float Ball::getTimeSecondsSinceStateOfLife() const {
    return _inGameChronometer->getTime() - _stateOfLifeTime;
}

float Ball::getTimeSecondsSinceAction() const {
    return _inGameChronometer->getTime() - _actionTime;
}

float Ball::getActionTime() const {
    return _actionTime;
}

void Ball::setActionTimeNow() noexcept {
    _actionTime = _inGameChronometer->getTime();
}

void Ball::setLifeTimeNow() noexcept {
    _stateOfLifeTime = _inGameChronometer->getTime();
}

void Ball::addUpdateOutput(CstUpdateOutput_sptr &&updateOutput) {
    _updateOutputs.push_back(std::move(updateOutput));
}

vecCstUpdateOutput_sptr &&Ball::retrieveUpdateOutput() {
    return std::move(_updateOutputs);
}
