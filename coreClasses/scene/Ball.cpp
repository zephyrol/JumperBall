/*
 * File: Ball.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:17
 */

#include "Ball.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>

Ball::Ball(Map& map):
    _currentBlockX(map.beginX()),
    _currentBlockY(map.beginY()),
    _currentBlockZ(map.beginZ()),
    _3DPosX(0.f),
    _3DPosY(0.f),
    _3DPosZ(0.f),
    _currentSide(JBTypes::Dir::Up),
    _lookTowards(JBTypes::Dir::North),
    _state(Ball::State::Staying),
    _stateOfLife(Ball::StateOfLife::Normal),
    _jumpingType(Ball::JumpingType::Short),
    _map(map),
    _mechanicsPatternJumping(getRadius()),
    _mechanicsPatternLongJumping(getRadius(),
                                 3.f,
                                 4.2f),
    _mechanicsPatternFalling(getRadius(), 0.f, 0.f),
    _timeAction(std::chrono::system_clock::now()),
    _timeStateOfLife(std::chrono::system_clock::now()),
    _burnCoefficientTrigger(0.f),
    _burnCoefficientCurrent(0.f),
    _teleportationColor(JBTypes::Color::None),
    _teleportationCoefficient(0.f),
    _teleportationBlockDestination(0),
    _jumpRequest(false),
    _timeJumpRequest(),
    _currentCoveredRotation(JBTypesMethods::createQuaternion({ 0.f, 0.f, 0.f }, 1.f)),
    _currentCrushing(0.f) {
}

void Ball::turnLeft() noexcept{
    _lookTowards = turnLeftMovement.evaluate({ _currentSide, _lookTowards });
    _state = Ball::State::TurningLeft;
    setTimeActionNow();
}

void Ball::turnRight() noexcept{
    _lookTowards = turnRightMovement.evaluate({ _currentSide, _lookTowards });
    _state = Ball::State::TurningRight;
    setTimeActionNow();
}

JBTypes::vec3f Ball::get3DPosition() const noexcept{
    return JBTypes::vec3f { _3DPosX, _3DPosY, _3DPosZ };
}

Ball::nextBlockInformation Ball::getNextBlockInfo() const noexcept{

    const std::array <int, 12> offsetsNextBlocks = nextBlockGetter.evaluate({ _currentSide, _lookTowards });

    const int inFrontOfX = _currentBlockX + offsetsNextBlocks.at(0);
    const int inFrontOfY = _currentBlockY + offsetsNextBlocks.at(1);
    const int inFrontOfZ = _currentBlockZ + offsetsNextBlocks.at(2);

    const int leftX = _currentBlockX + offsetsNextBlocks.at(3);
    const int leftY = _currentBlockY + offsetsNextBlocks.at(4);
    const int leftZ = _currentBlockZ + offsetsNextBlocks.at(5);

    const int rightX = _currentBlockX + offsetsNextBlocks.at(6);
    const int rightY = _currentBlockY + offsetsNextBlocks.at(7);
    const int rightZ = _currentBlockZ + offsetsNextBlocks.at(8);

    const int aboveX = _currentBlockX + offsetsNextBlocks.at(9);
    const int aboveY = _currentBlockY + offsetsNextBlocks.at(10);
    const int aboveZ = _currentBlockZ + offsetsNextBlocks.at(11);

    const CstBlock_sptr& blockAbove = _map.getBlock(aboveX, aboveY, aboveZ);

    const CstBlock_sptr& blockInFrontOf = _map.getBlock(inFrontOfX, inFrontOfY, inFrontOfZ);

    const CstBlock_sptr& blockLeft = _map.getBlock(leftX, leftY, leftZ);

    const CstBlock_sptr& blockRight = _map.getBlock(rightX, rightY, rightZ);

    Ball::nextBlockInformation nextBlock;
    if (blockAbove && blockAbove->isExists()) {
        nextBlock.poxX = aboveX;
        nextBlock.poxY = aboveY;
        nextBlock.poxZ = aboveZ;
        nextBlock.nextLocal = NextBlockLocal::Above;

        const JBTypes::Dir lookTowardsBeforeMovement = _lookTowards;
        nextBlock.nextLook = _currentSide;
        nextBlock.nextSide =
            turnBackMovement.evaluate({ lookTowardsBeforeMovement });
    } else if (blockInFrontOf && blockInFrontOf->isExists()) {
        nextBlock.poxX = inFrontOfX;
        nextBlock.poxY = inFrontOfY;
        nextBlock.poxZ = inFrontOfZ;
        nextBlock.nextLocal = NextBlockLocal::InFrontOf;
        nextBlock.nextLook = _lookTowards;
        nextBlock.nextSide = _currentSide;
    } else if (
        (!blockLeft || !blockLeft->isExists()) &&
        (!blockRight || !blockRight->isExists())
        ) {
        nextBlock.poxX = _currentBlockX;
        nextBlock.poxY = _currentBlockY;
        nextBlock.poxZ = _currentBlockZ;
        nextBlock.nextLocal = NextBlockLocal::Same;
        const JBTypes::Dir sideBeforeMovement = _currentSide;
        nextBlock.nextSide = _lookTowards;
        nextBlock.nextLook = turnBackMovement.evaluate({ sideBeforeMovement });
    } else if (_stateOfLife == StateOfLife::Sliding) {
        nextBlock.poxX = inFrontOfX;
        nextBlock.poxY = inFrontOfY;
        nextBlock.poxZ = inFrontOfZ;
        nextBlock.nextLocal = NextBlockLocal::InFrontOf;
        nextBlock.nextLook = _lookTowards;
        nextBlock.nextSide = _currentSide;
    } else {
        nextBlock.poxX = _currentBlockX;
        nextBlock.poxY = _currentBlockY;
        nextBlock.poxZ = _currentBlockZ;
        nextBlock.nextLocal = NextBlockLocal::None;
        nextBlock.nextSide = _currentSide;
        nextBlock.nextLook = _lookTowards;
    }

    return nextBlock;
}

void Ball::goStraightAhead() noexcept{

    const Ball::nextBlockInformation nextBlock = getNextBlockInfo();
    _currentBlockX = nextBlock.poxX;
    _currentBlockY = nextBlock.poxY;
    _currentBlockZ = nextBlock.poxZ;
    _currentSide = nextBlock.nextSide;
    _lookTowards = nextBlock.nextLook;

    applyRotation();

}

void Ball::jump() noexcept{
    _state = Ball::State::Jumping;
    setTimeActionNow();
    isGoingStraightAheadIntersectBlock();
}

void Ball::stay() noexcept{
    _jumpingType = Ball::JumpingType::Short;
    _state = Ball::State::Staying;
    setTimeActionNow();
}

void Ball::fall() noexcept{
    _state = Ball::State::Falling;
    setTimeActionNow();
}

void Ball::teleport (const JBTypes::Color& col) noexcept{
    _state = Ball::State::Teleporting;
    _teleportationColor = col;
    setTimeActionNow();
}

void Ball::deteleport() noexcept{
    _state = Ball::State::Deteleporting;
    setTimeActionNow();
}

void Ball::move() noexcept{
    const Ball::nextBlockInformation infos = getNextBlockInfo();
    if (infos.nextLocal != NextBlockLocal::None) {
        _state = Ball::State::Moving;
        setTimeActionNow();
    }
}

void Ball::setTimeActionNow() noexcept{
    _timeAction = std::chrono::system_clock::now();
}

void Ball::setTimeLifeNow() noexcept{
    _timeStateOfLife = std::chrono::system_clock::now();
}

void Ball::doAction (Ball::ActionRequest action) {
    if (_stateOfLife == Ball::StateOfLife::Bursting) {
        return;
    }
    switch (action) {
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
    case Ball::ActionRequest::Jump:
        if (_state == Ball::State::Staying) {
            _state = Ball::State::Jumping;
            jump();
        } else {
            _jumpRequest = true;
            _timeJumpRequest = JBTypesMethods::getTimePointMSNow();
        }
        break;
    default:
        break;
    }
}

void Ball::isGoingStraightAheadIntersectBlock() noexcept{

    if (_state == Ball::State::Jumping) {
        constexpr float distanceNear = 1.f;
        constexpr float distanceFar = 2.f;
        constexpr float distanceVeryFar = 3.f;
        constexpr float sizeBlock = 1.f;

        const JBTypes::vec3f sideVec = JBTypesMethods::directionAsVector(_currentSide);
        const JBTypes::vec3f lookVec = JBTypesMethods::directionAsVector(_lookTowards);

        const int aboveNearX = _currentBlockX + static_cast <int>(sideVec.x + lookVec.x);
        const int aboveNearY = _currentBlockY + static_cast <int>(sideVec.y + lookVec.y);
        const int aboveNearZ = _currentBlockZ + static_cast <int>(sideVec.z + lookVec.z);
        const int aboveFarX = _currentBlockX + static_cast <int>(sideVec.x + 2.f * lookVec.x);
        const int aboveFarY = _currentBlockY + static_cast <int>(sideVec.y + 2.f * lookVec.y);
        const int aboveFarZ = _currentBlockZ + static_cast <int>(sideVec.z + 2.f * lookVec.z);
        const int aboveVeryFarX = _currentBlockX + static_cast <int>(sideVec.x + 3.f * lookVec.x);
        const int aboveVeryFarY = _currentBlockY + static_cast <int>(sideVec.y + 3.f * lookVec.y);
        const int aboveVeryFarZ = _currentBlockZ + static_cast <int>(sideVec.z + 3.f * lookVec.z);

        const CstBlock_sptr blockNear = _map.getBlock(aboveNearX, aboveNearY, aboveNearZ);

        const CstBlock_sptr blockFar = _map.getBlock(aboveFarX, aboveFarY, aboveFarZ);

        const CstBlock_sptr blockVeryFar = _map.getBlock(aboveVeryFarX, aboveVeryFarY, aboveVeryFarZ);

        ClassicalMechanics& refMechanicsJumping = getMechanicsJumping();
        if (blockNear && blockNear->isExists()) {
            refMechanicsJumping.addShockFromPosition(distanceNear - sizeBlock / 2.f - getRadius());
        } else if (blockFar && blockFar->isExists()) {
            refMechanicsJumping.addShockFromPosition(distanceFar - sizeBlock / 2.f - getRadius());
        } else if (
            _jumpingType == Ball::JumpingType::Long && blockVeryFar &&
            blockVeryFar->isExists()) {
            refMechanicsJumping.addShockFromPosition(distanceVeryFar - sizeBlock / 2.f - getRadius());
        } else {
            refMechanicsJumping.timesShock({});
        }
    }
}

const ClassicalMechanics& Ball::getMechanicsJumping() const noexcept{
    if (_jumpingType == Ball::JumpingType::Short) {
        return _mechanicsPatternJumping;
    } else {
        return _mechanicsPatternLongJumping;
    }
}

const ClassicalMechanics& Ball::getMechanicsFalling() const noexcept{
    return _mechanicsPatternFalling;
}

JBTypes::Quaternion Ball::getCoveredRotation() const noexcept{

    constexpr float sizeBlock = 1.f; // TODO: use a member in Block or map class

    const std::function <float()> getCoveredDistance =
        [this] ()->float {
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

float Ball::getCrushingCoefficient() const noexcept{

    if (_stateOfLife == Ball::StateOfLife::Sliding) {
        return 0.f;
    }

    const std::function <float()> stayCrushingCoeff =
        [this] ()->float {
            constexpr float durationWaitingCrushing = 0.7f;
            const float angleInCosinusFunc = getTimeSecondsSinceAction() * 2.f *
                                             static_cast <float>(M_PI) / durationWaitingCrushing;
            // -cos(0) = -1;
            // cos(0) = 1;
            return 0.5f - cosf(angleInCosinusFunc) / 2.f;
        };

    if (_state == Ball::State::Staying) {
        return stayCrushingCoeff();
    }

    const std::function <float(float)> movementCrushingCoeff =
        [this] (float timeToDoMovement)->float {
            const float tMax = getTimeSecondsSinceAction() / timeToDoMovement;
            const float t = tMax > 1.f
                            ? 1.f
                            : tMax;
            return (1.f - t) * _currentCrushing;
        };

    if ( _state == Ball::State::Moving || _state == Ball::State::Jumping) {
        return movementCrushingCoeff(timeToGetNextBlock);
    } 
    if ( _state == Ball::State::TurningLeft || _state == Ball::State::TurningRight) {
        return movementCrushingCoeff(timeToTurn);
    } 
    return 0.f;
}

float Ball::getTeleportationCoefficient() const noexcept{
    return _teleportationCoefficient;
}

const JBTypes::Color& Ball::getTeleportationColor() const noexcept{
    return _teleportationColor;
}

void Ball::isFallingIntersectionBlock() noexcept{
    const float fDifference = getTimeSecondsSinceAction();
    const bool descendingJumpPhase = _state == Ball::State::Jumping &&
                                     _mechanicsPatternJumping.getVelocity(fDifference).y < 0;
    const auto positionBlockPtr = intersectBlock(_3DPosX, _3DPosY, _3DPosZ);
    if (!positionBlockPtr || (!descendingJumpPhase && _state != Ball::State::Falling)) {
        return;
    }
    if (descendingJumpPhase) {
        const float coveredDistance = getMechanicsJumping().getPosition(
            getMechanicsJumping().getTimeToGetDestination()
            ).x;
        const float absCoveredDistance = fabs(coveredDistance);
        const bool inverseRotation = coveredDistance < 0.f;
        for (size_t i = 0; i < absCoveredDistance; ++i) {
            applyRotation(inverseRotation);
        }
    }
    getMechanicsJumping().timesShock({});
    _currentBlockX = positionBlockPtr->at(0);
    _currentBlockY = positionBlockPtr->at(1);
    _currentBlockZ = positionBlockPtr->at(2);
    stay();
    blockEvent({ _currentBlockX, _currentBlockY, _currentBlockZ });
    update();
}


std::shared_ptr <const std::vector <int> > Ball::intersectBlock (float x, float y, float z) const {
    const JBTypes::vec3f sideVec = JBTypesMethods::directionAsVector(_currentSide);

    const float offsetBlockPosition = getRadius();
    const float xIntersectionUnder = x - sideVec.x * offsetBlockPosition;
    const float yIntersectionUnder = y - sideVec.y * offsetBlockPosition;
    const float zIntersectionUnder = z - sideVec.z * offsetBlockPosition;

    const int xInteger = static_cast <int>(xIntersectionUnder);
    const int yInteger = static_cast <int>(yIntersectionUnder);
    const int zInteger = static_cast <int>(zIntersectionUnder);

    const CstBlock_sptr& block = _map.getBlock(xInteger, yInteger, zInteger);

    return (block && block->isExists())
           ? std::make_shared <const std::vector <int> >(
        std::initializer_list <int>({ xInteger, yInteger, zInteger }))
           : nullptr;
}

JBTypes::timePointMs Ball::getTimeActionMs() const noexcept{
    return std::chrono::time_point_cast <std::chrono::milliseconds>(_timeAction);
}

JBTypes::timePointMs Ball::getTimeStateOfLifeMs() const noexcept{
    return std::chrono::time_point_cast <std::chrono::milliseconds>(_timeStateOfLife);
}

JBTypes::vec3f Ball::P2DTo3D (ClassicalMechanics::physics2DVector p2D) const {
    const float offsetRealPosition = 0.5f + getRadius();

    const JBTypes::vec3f sideVec = JBTypesMethods::directionAsVector(_currentSide);
    const JBTypes::vec3f lookVec = JBTypesMethods::directionAsVector(_lookTowards);

    const float x = static_cast <float>(_currentBlockX) + 0.5f +
                    sideVec.x * (offsetRealPosition + p2D.y) + lookVec.x * p2D.x;
    const float y = static_cast <float>(_currentBlockY) + 0.5f +
                    sideVec.y * (offsetRealPosition + p2D.y) + lookVec.y * p2D.x;
    const float z = static_cast <float>(_currentBlockZ) + 0.5f +
                    sideVec.z * (offsetRealPosition + p2D.y) + lookVec.z * p2D.x;

    return JBTypes::vec3f { x, y, z };
}

float Ball::getRadius() const {
    return Ball::basicRadius;
}

JBTypes::Dir Ball::currentSide() const {
    return _currentSide;
}

JBTypes::Dir Ball::lookTowards() const {
    return _lookTowards;
}

float Ball::getTimeSecondsSinceAction() const noexcept{
    const JBTypes::timePointMs timeNowMs = JBTypesMethods::getTimePointMSNow();
    const JBTypes::timePointMs timeActionMs = getTimeActionMs();
    return JBTypesMethods::getFloatFromDurationMS(timeNowMs - timeActionMs);
}

float Ball::getTimeSecondsSinceStateOfLife() const noexcept{
    const JBTypes::timePointMs timeNowMs = JBTypesMethods::getTimePointMSNow();
    const JBTypes::timePointMs timeStateOfLifeMs = getTimeStateOfLifeMs();
    return JBTypesMethods::getFloatFromDurationMS(timeNowMs - timeStateOfLifeMs);
}

JBTypes::vec3f Ball::lookTowardsAsVector() const {
    return JBTypesMethods::directionAsVector(_lookTowards);
}

JBTypes::vec3f Ball::currentSideAsVector() const {
    return JBTypesMethods::directionAsVector(_currentSide);
}

ClassicalMechanics& Ball::getMechanicsJumping() noexcept{
    // Scott Meyer's advice to avoid code duplication
    return const_cast <ClassicalMechanics&>(static_cast <const Ball&>(*this).getMechanicsJumping());
}

JBTypes::vec3f Ball::get3DPosStayingBall() const {
    const float offsetPosition = 0.5f + getRadius();

    const JBTypes::vec3f sideVec = JBTypesMethods::directionAsVector(_currentSide);
    const float x = static_cast <float>(_currentBlockX) + 0.5f + sideVec.x * offsetPosition;
    const float y = static_cast <float>(_currentBlockY) + 0.5f + sideVec.y * offsetPosition;
    const float z = static_cast <float>(_currentBlockZ) + 0.5f + sideVec.z * offsetPosition;

    return { x, y, z };
}


void Ball::blockEvent (const JBTypes::vec3ui& blockPos) noexcept{

    const auto& block = _map.getBlock(blockPos.at(0), blockPos.at(1), blockPos.at(2));
    if (!block || _stateOfLife == StateOfLife::Bursting) {
        return;
    }

    const Block::Effect effect = block->detectionEvent(
        _currentSide,
        JBTypesMethods::getTimePointMsFromTimePoint(_timeAction)
        );
    if (effect == Block::Effect::Jump) {
        _stateOfLife = StateOfLife::Normal;
        _jumpingType = Ball::JumpingType::Long;
        jump();
    } else if (effect == Block::Effect::Burn) {
        _burnCoefficientCurrent += .2f;
        _burnCoefficientTrigger = _burnCoefficientCurrent;
        _stateOfLife = StateOfLife::Burning;
        setTimeLifeNow();
    } else if (effect == Block::Effect::Slide) {
        _burnCoefficientTrigger = 0.f;
        _stateOfLife = StateOfLife::Sliding;
        if (
            _jumpRequest && JBTypesMethods::getTimeSecondsSinceTimePoint(
                _timeJumpRequest) < timeToGetNextBlock
            ) {
            _jumpRequest = false;
            jump();
        } else {
            move();
        }
        update();
    } else {
        _stateOfLife = StateOfLife::Normal;
    }

    // Specials
    for (const Map::SpecialInfo& specialInfo : _map.getSpecialInfo()) {
        const std::shared_ptr <const Special>& special = specialInfo.special;
        if (special && blockPos == special->position() && _currentSide == special->direction()) {
            const JBTypes::Color& color = special->getColor();
            if (specialInfo.type == Map::SpecialTypes::Teleporter) {
                if (_map.getSpecialStates().at(color)) {
                    teleport(color);
                }
            } else { // Switch Button
                _map.switchColor(color);
            }
        }
    }

}

void Ball::jumpingUpdate() noexcept{
    const ClassicalMechanics::physics2DVector pos2D =
        getMechanicsJumping().getPosition(getTimeSecondsSinceAction());

    const JBTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
    _3DPosX = relativePositionJump.x;
    _3DPosY = relativePositionJump.y;
    _3DPosZ = relativePositionJump.z;
    isFallingIntersectionBlock();
}

void Ball::fallingUpdate() noexcept{
    const ClassicalMechanics::physics2DVector pos2D =
        _mechanicsPatternFalling.getPosition(getTimeSecondsSinceAction());

    const JBTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
    _3DPosX = relativePositionJump.x;
    _3DPosY = relativePositionJump.y;
    _3DPosZ = relativePositionJump.z;
    isFallingIntersectionBlock();
}

void Ball::stayingUpdate() noexcept{
    _currentCrushing = getCrushingCoefficient();
    const JBTypes::vec3f position3D = get3DPosStayingBall();
    const Block_sptr block = _map.getBlock(_currentBlockX, _currentBlockY, _currentBlockZ);
    if (block && block->isExists()) {
        _3DPosX = position3D.x;
        _3DPosY = position3D.y;
        _3DPosZ = position3D.z;
        return;
    }
    fall();
    update();
}

void Ball::turningUpdate() noexcept{
    const JBTypes::vec3f position3D = get3DPosStayingBall();
    const float sSinceAction = getTimeSecondsSinceAction();
    if (sSinceAction >= (timeToTurn)) {
        stay();
        update();
        return;
    }
    _3DPosX = position3D.x;
    _3DPosY = position3D.y;
    _3DPosZ = position3D.z;
}

void Ball::movingUpdate() noexcept{
    const float sSinceAction = getTimeSecondsSinceAction();
    if (sSinceAction >= timeToGetNextBlock) {
        goStraightAhead();
        const JBTypes::vec3f position3D = get3DPosStayingBall();
        _3DPosX = position3D.x;
        _3DPosY = position3D.y;
        _3DPosZ = position3D.z;
        stay();
        blockEvent({ _currentBlockX, _currentBlockY, _currentBlockZ });
        update();
        return;
    }
    const JBTypes::vec3f position3D = get3DPosStayingBall();
    const Ball::nextBlockInformation infoTarget = getNextBlockInfo();
    if (infoTarget.nextLocal == NextBlockLocal::InFrontOf) {
        _3DPosX = (sSinceAction * (static_cast <float>(infoTarget.poxX)
                                   - static_cast <float>(_currentBlockX))
                   / timeToGetNextBlock) + position3D.x;
        _3DPosY = (sSinceAction * (static_cast <float>(infoTarget.poxY)
                                   - static_cast <float>(_currentBlockY))
                   / timeToGetNextBlock) + position3D.y;
        _3DPosZ = (sSinceAction * (static_cast <float>(infoTarget.poxZ)
                                   - static_cast <float>(_currentBlockZ))
                   / timeToGetNextBlock) + position3D.z;
        return;
    }
    if (infoTarget.nextLocal == NextBlockLocal::Same || infoTarget.nextLocal == NextBlockLocal::Above) {

        const float distancePerStep = infoTarget.nextLocal == NextBlockLocal::Same
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
        const JBTypes::vec3f nextLookVec = JBTypesMethods::directionAsVector(infoTarget.nextLook);

        const float distStep1 = distancePerStep * timeStep1 / halfTimeToGetNextBlock;
        const float distStep2 = distancePerStep * timeStep2 / halfTimeToGetNextBlock;

        _3DPosX = position3D.x + lookVec.x * distStep1 + nextLookVec.x * distStep2;
        _3DPosY = position3D.y + lookVec.y * distStep1 + nextLookVec.y * distStep2;
        _3DPosZ = position3D.z + lookVec.z * distStep1 + nextLookVec.z * distStep2;
        return;
    }
    _3DPosX = position3D.x;
    _3DPosY = position3D.y;
    _3DPosZ = position3D.z;
}

void Ball::burningUpdate() noexcept{
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
            setTimeLifeNow();
        }
        return;
    }
    // the ball is cooldown
    if (_stateOfLife == StateOfLife::Burning) {
        _stateOfLife = StateOfLife::Normal;
        setTimeLifeNow();
        _burnCoefficientTrigger = _burnCoefficientCurrent;
    }
    _burnCoefficientCurrent = _burnCoefficientTrigger - (time / timeToBurn);
    if (_burnCoefficientCurrent < 0.f) {
        _burnCoefficientCurrent = 0.f;
    }

}

void Ball::teleportingUpdate() noexcept{

    const float timeSinceAction = getTimeSecondsSinceAction();
    if (timeSinceAction < Ball::halfTimeTeleportationDuration) {
        _teleportationCoefficient = timeSinceAction / Ball::halfTimeTeleportationDuration;
        return;
    }
    const size_t blockIndex = _map.getIndex({ _currentBlockX, _currentBlockY, _currentBlockZ });

    const auto& blockTeleporter = _map.getBlocksTeleporters();
    const Map::TeleportersInfo teleporterInfo = blockTeleporter.at(_teleportationColor);

    const size_t firstTeleporterIndex = teleporterInfo.coupleIndices.first;
    const size_t secondTeleporterIndex = teleporterInfo.coupleIndices.second;

    const JBTypes::Dir firstTeleporterDir = teleporterInfo.coupleDirections.first;
    const JBTypes::Dir secondTeleporterDir = teleporterInfo.coupleDirections.second;

    const size_t destinationIndex = blockIndex == firstTeleporterIndex
                                    ? secondTeleporterIndex
                                    : firstTeleporterIndex;

    const JBTypes::Dir destinationDir = blockIndex == firstTeleporterIndex
                                        ? secondTeleporterDir
                                        : firstTeleporterDir;

    const JBTypes::vec3ui& destinationPosition = _map.getBlockCoords(destinationIndex);

    _currentBlockX = destinationPosition.at(0);
    _currentBlockY = destinationPosition.at(1);
    _currentBlockZ = destinationPosition.at(2);
    if (_currentSide != destinationDir) {
        const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(destinationDir);
        _lookTowards = JBTypesMethods::vectorAsDirection(
            JBTypesMethods::cross(vecDir, { vecDir.y, -vecDir.x, 0.f })
            );
    }
    _currentSide = destinationDir;
    _teleportationCoefficient = 1.f;

    stay();
    update();
    deteleport();
    update();
}

void Ball::deteleportingUpdate() noexcept{

    const float timeSinceAction = getTimeSecondsSinceAction();
    if (timeSinceAction > Ball::halfTimeTeleportationDuration) {
        _teleportationCoefficient = 0.f;
        _teleportationColor = JBTypes::Color::None;
        stay();
        update();
    } else {
        _teleportationCoefficient = 1.f - (timeSinceAction / halfTimeTeleportationDuration);
    }
}

void Ball::applyRotation (bool inverse) {

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

JBTypes::vec3f Ball::getRotationAxis() const noexcept{
    return JBTypesMethods::cross(
        JBTypesMethods::directionAsVector(_currentSide),
        JBTypesMethods::directionAsVector(_lookTowards)
        );
}

JBTypes::vec3f Ball::getInverseRotationAxis() const noexcept{
    return JBTypesMethods::cross(
        JBTypesMethods::directionAsVector(_lookTowards),
        JBTypesMethods::directionAsVector(_currentSide)
        );
}

float Ball::getJumpingPosX() const noexcept{
    return getMechanicsJumping().getPosition(getTimeSecondsSinceAction()).x;
}

float Ball::getFallingPosX() const noexcept{
    return getMechanicsFalling().getPosition(getTimeSecondsSinceAction()).x;
}

void Ball::update() noexcept{
    if (_stateOfLife == Ball::StateOfLife::Dead) {
        return;
    }
    switch (_state) {
    case Ball::State::Falling: fallingUpdate(); break;
    case Ball::State::Jumping: jumpingUpdate(); break;
    case Ball::State::TurningLeft: turningUpdate(); break;
    case Ball::State::TurningRight: turningUpdate(); break;
    case Ball::State::Staying: stayingUpdate(); break;
    case Ball::State::Moving: movingUpdate(); break;
    case Ball::State::Teleporting: teleportingUpdate(); break;
    case Ball::State::Deteleporting: deteleportingUpdate(); break;
    default: break;
    }
    mapInteraction();
    burningUpdate();
    if (isOutOfTheMap() || isBurstingFinished()) {
        die();
    }
}

bool Ball::isBurstingFinished() const {
    return (_stateOfLife == Ball::StateOfLife::Bursting) && (getTimeSecondsSinceStateOfLife() > timeToBurst);
}

void Ball::mapInteraction() noexcept{
    const Map::Effect effect = _map.interaction(_currentSide, get3DPosition(), getRadius());
    if (effect == Map::Effect::Burst) {
        if (_stateOfLife != StateOfLife::Bursting) {
            _stateOfLife = StateOfLife::Bursting;
            setTimeLifeNow();
        }
    }
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

void Ball::die() noexcept{
    _stateOfLife = Ball::StateOfLife::Dead;
}

bool Ball::isOutOfTheMap() const {
    constexpr float thresholdOut = 5.f;
    if (
        _3DPosX < -thresholdOut || _3DPosX > (static_cast <float>(_map.width()) + thresholdOut) ||
        _3DPosY < -thresholdOut || _3DPosY > (static_cast <float>(_map.height()) + thresholdOut) ||
        _3DPosZ < -thresholdOut || _3DPosZ > (static_cast <float>(_map.depth()) + thresholdOut)
        ) {
        return true;
    } else return false;
}

float Ball::getTimeToGetDestination() const {
    return getMechanicsJumping().getTimeToGetDestination();
}

JBTypes::vec3f Ball::getNextLook() const {
    return JBTypesMethods::directionAsVector(getNextBlockInfo().nextLook);
}

SceneElement::DynamicValues <float> Ball::getDynamicFloats() const {

    const auto getStateOfLifeStatus = [this]() {
        if (
            _stateOfLife == Ball::StateOfLife::Normal ||
            _stateOfLife == Ball::StateOfLife::Burning ||
            _stateOfLife == Ball::StateOfLife::Sliding) {
            return 0.f;
        }
        if (_stateOfLife == Ball::StateOfLife::Bursting) { 
            return 1.f;
        }
        return 2.f;
    };

    return {
        { "ballRadius", getRadius()},
        { "crushingCoeff", getCrushingCoefficient()},
        { "status", getStateOfLifeStatus()},
        { "timeStateOfLife", getTimeSecondsSinceStateOfLife()},
        { "burningCoeff", burnCoefficient()}
    };
}

SceneElement::DynamicValues <JBTypes::vec3f> Ball::getDynamicVec3fs() const {
    return {
        { "sideDir", currentSideAsVector() },
        { "position", get3DPosition() }
    };
}

SceneElement::DynamicValues <JBTypes::Quaternion> Ball::getDynamicQuaternions() const {
    return {
        { "quaternion", getCoveredRotation()}
    };
}

SceneElement::GlobalState Ball::getGlobalState() const {
    if (_stateOfLife == Ball::StateOfLife::Bursting) {
        return SceneElement::GlobalState::Separate;
    }
    if (_stateOfLife == Ball::StateOfLife::Dead) {
        return SceneElement::GlobalState::Dead;
    }
    return SceneElement::GlobalState::United;
}


const TurnLeft Ball::turnLeftMovement;
const TurnRight Ball::turnRightMovement;
const TurnBack Ball::turnBackMovement;
const NextBlock Ball::nextBlockGetter;
