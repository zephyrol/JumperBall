/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.cpp
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
        _mechanicsPatternJumping(),
        _mechanicsPatternLongJumping( 3.f,
                                      ClassicalMechanics::timeToStopWindBasic,
                                      4.2f),
        _mechanicsPatternFalling(0.f,0.f,0.f),
        _timeAction(std::chrono::system_clock::now()),
        _timeStateOfLife(std::chrono::system_clock::now()),
        _burnCoefficientTrigger(0.f),
        _burnCoefficientCurrent(0.f)
{
}

void Ball::turnLeft() noexcept {
    _lookTowards = turnLeftMovement.evaluate({_currentSide,_lookTowards});
    _state = Ball::State::TurningLeft;
    setTimeActionNow();
}

void Ball::turnRight() noexcept {
    _lookTowards = turnRightMovement.evaluate({_currentSide,_lookTowards});
    _state = Ball::State::TurningRight;
    setTimeActionNow();
}

JBTypes::vec3f Ball::get3DPosition() const noexcept {
    return JBTypes::vec3f {_3DPosX,_3DPosY,_3DPosZ};
}

Ball::nextBlockInformation Ball::getNextBlockInfo() const noexcept{

    Ball::nextBlockInformation nextBlock;

    const std::array<int,12> offsetsNextBlocks = nextBlockGetter.evaluate
        ({_currentSide,_lookTowards});
    
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
    
    const auto blockAbove = _map.getBlock(aboveX,aboveY,aboveZ);
    const auto blockInFrontOf = _map.getBlock(inFrontOfX,inFrontOfY,inFrontOfZ);
    const auto blockLeft = _map.getBlock(leftX,leftY,leftZ);
    const auto blockRight = _map.getBlock(rightX,rightY,rightZ);

    if (blockAbove && blockAbove->stillExists()) {
        nextBlock.poxX = aboveX;
        nextBlock.poxY = aboveY;
        nextBlock.poxZ = aboveZ;
        nextBlock.nextLocal = NextBlockLocal::Above;

        const JBTypes::Dir lookTowardsBeforeMovement = _lookTowards;
        nextBlock.nextLook = _currentSide;
        nextBlock.nextSide =
            turnBackMovement.evaluate({lookTowardsBeforeMovement});
    }
    else if (blockInFrontOf && blockInFrontOf->stillExists()) {
        nextBlock.poxX      = inFrontOfX;
        nextBlock.poxY      = inFrontOfY;
        nextBlock.poxZ      = inFrontOfZ;
        nextBlock.nextLocal = NextBlockLocal::InFrontOf;
        nextBlock.nextLook  = _lookTowards;
        nextBlock.nextSide  = _currentSide;
    }
    else {
        nextBlock.poxX      = _currentBlockX;
        nextBlock.poxY      = _currentBlockY;
        nextBlock.poxZ      = _currentBlockZ;
        
        if ((!blockLeft || !blockLeft ->stillExists())
            && (!blockRight || !blockRight ->stillExists())) {
            nextBlock.nextLocal = NextBlockLocal::Same;
            const JBTypes::Dir sideBeforeMovement = _currentSide;
            nextBlock.nextSide  = _lookTowards;
            nextBlock.nextLook = turnBackMovement.
                evaluate({sideBeforeMovement});
        }
        else {
            nextBlock.nextLocal = NextBlockLocal::None;
            nextBlock.nextSide  = _currentSide;
            nextBlock.nextLook  = _lookTowards;
        }
    }
    return nextBlock;
}

void Ball::goStraightAhead() noexcept {
    
    struct nextBlockInformation nextBlock = getNextBlockInfo(); 
    _currentBlockX  = nextBlock.poxX;
    _currentBlockY  = nextBlock.poxY;
    _currentBlockZ  = nextBlock.poxZ;
    _currentSide    = nextBlock.nextSide;
    _lookTowards    = nextBlock.nextLook; 
}

void Ball::jump() noexcept {
    _state = Ball::State::Jumping;
    setTimeActionNow();
    isGoingStraightAheadIntersectBlock();
}

void Ball::stay() noexcept {
    _state = Ball::State::Staying;
    _jumpingType = Ball::JumpingType::Short;
    setTimeActionNow();
}

void Ball::fall() noexcept {
    _state = Ball::State::Falling;
    setTimeActionNow();
}

void Ball::move() noexcept {
    
   const Ball::nextBlockInformation infos = getNextBlockInfo();
    if (infos.nextLocal != NextBlockLocal::None) {
        _state = Ball::State::Moving;
        setTimeActionNow();
    }
}

void Ball::setTimeActionNow() noexcept {
    _timeAction = std::chrono::system_clock::now();
}

void Ball::setTimeLifeNow() noexcept
{
    _timeStateOfLife = std::chrono::system_clock::now();
}

void Ball::doAction(Ball::ActionRequest action) {
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
            }
            break;
        default :
            break;
    }
}

void Ball::isGoingStraightAheadIntersectBlock()   noexcept {

    if ( _state == Ball::State::Jumping ) {
        constexpr float distanceNear      = 1.f;
        constexpr float distanceFar       = 2.f;
        constexpr float distanceVeryFar   = 3.f;
        constexpr float sizeBlock         = 1.f;

        const JBTypes::vec3f sideVec =
            JBTypesMethods::directionAsVector(_currentSide);
        const JBTypes::vec3f lookVec =
            JBTypesMethods::directionAsVector(_lookTowards);

        const int aboveNearX =  _currentBlockX +
                static_cast<int> (sideVec.x + lookVec.x);
        const int aboveNearY =  _currentBlockY +
                static_cast<int> (sideVec.y + lookVec.y);
        const int aboveNearZ =  _currentBlockZ +
                static_cast<int> (sideVec.z + lookVec.z);
        const int aboveFarX =   _currentBlockX +
                static_cast<int> (sideVec.x + 2.f * lookVec.x);
        const int aboveFarY =   _currentBlockY +
                static_cast<int> (sideVec.y + 2.f * lookVec.y);
        const int aboveFarZ =   _currentBlockZ +
                static_cast<int> (sideVec.z + 2.f * lookVec.z);
        const int aboveVeryFarX =   _currentBlockX +
                static_cast<int> (sideVec.x + 3.f * lookVec.x);
        const int aboveVeryFarY =   _currentBlockY +
                static_cast<int> (sideVec.y + 3.f * lookVec.y);
        const int aboveVeryFarZ =   _currentBlockZ +
                static_cast<int> (sideVec.z + 3.f * lookVec.z);

        const std::shared_ptr<const Block> blockNear  =
                _map.getBlock(aboveNearX,aboveNearY,aboveNearZ);
        
        const std::shared_ptr<const Block> blockFar   =
                _map.getBlock(aboveFarX,aboveFarY,aboveFarZ);

        const std::shared_ptr<const Block> blockVeryFar   =
                _map.getBlock(aboveVeryFarX,aboveVeryFarY,aboveVeryFarZ);
        
        ClassicalMechanics& refMechanicsJumping = getMechanicsJumping();
        if (blockNear && blockNear->stillExists()) {
            refMechanicsJumping.addShockFromPosition
            ( distanceNear-sizeBlock/2.f -getRadius());
        }
        else if (blockFar && blockFar->stillExists()) {
            refMechanicsJumping.addShockFromPosition
            ( distanceFar-sizeBlock/2.f -getRadius());
        }
        else if (_jumpingType == Ball::JumpingType::Long && blockVeryFar && 
                blockVeryFar->stillExists()) {
            refMechanicsJumping.addShockFromPosition
            ( distanceVeryFar-sizeBlock/2.f -getRadius());
        }
        else {
            refMechanicsJumping.timesShock({});
        }
    }
}

const ClassicalMechanics& Ball::getMechanicsJumping() const noexcept{
    if (_jumpingType == Ball::JumpingType::Short)
    return _mechanicsPatternJumping;
    else return _mechanicsPatternLongJumping;
}

const ClassicalMechanics& Ball::getMechanicsFalling() const noexcept{
    return _mechanicsPatternFalling;
}

void Ball::isFallingIntersectionBlock() noexcept {
    if ( _state == Ball::State::Jumping  || _state == Ball::State::Falling) {
        float fDifference = getTimeSecondsSinceAction();

        if ( _state == Ball::State::Falling || 
                _mechanicsPatternJumping.getVelocity(fDifference).y < 0 ) {
            auto positionBlockPtr = intersectBlock(_3DPosX,_3DPosY, _3DPosZ);
            if (positionBlockPtr) {
                getMechanicsJumping().timesShock({});
                _currentBlockX      = positionBlockPtr->at(0) ;
                _currentBlockY      = positionBlockPtr->at(1) ;
                _currentBlockZ      = positionBlockPtr->at(2) ;
                stay();
                blockEvent(_map.getBlock(
                        _currentBlockX,_currentBlockY,_currentBlockZ));
                update();
            }
        }
    }
}


std::shared_ptr<const std::vector<int> > Ball::intersectBlock(float x, 
                                                              float y, 
                                                              float z) const {
    std::shared_ptr<const std::vector<int> > blockIntersected = nullptr;
    
    const JBTypes::vec3f sideVec =
        JBTypesMethods::directionAsVector(_currentSide);

    const float offsetBlockPosition = ClassicalMechanics::radiusBall;
    const float xIntersectionUnder = x - sideVec.x * offsetBlockPosition;
    const float yIntersectionUnder = y - sideVec.y * offsetBlockPosition;
    const float zIntersectionUnder = z - sideVec.z * offsetBlockPosition;
    
    const int xInteger = static_cast<int> (xIntersectionUnder);
    const int yInteger = static_cast<int> (yIntersectionUnder);
    const int zInteger = static_cast<int> (zIntersectionUnder);
    
    const auto block = _map.getBlock(xInteger,yInteger,zInteger);
    if (block && block->stillExists()) 
        blockIntersected = std::make_shared<const std::vector<int> > (
                std::initializer_list<int> ({xInteger,yInteger,zInteger}));
    
    return blockIntersected;
}

JBTypes::timePointMs Ball::getTimeActionMs() const noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds>
                                                      (_timeAction);
}

JBTypes::timePointMs Ball::getTimeStateOfLifeMs() const noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds>
                                                      (_timeStateOfLife);
}

JBTypes::vec3f Ball::P2DTo3D(ClassicalMechanics::physics2DVector p2D) const
{

    const float offsetRealPosition = 0.5f + ClassicalMechanics::radiusBall;
    
    const JBTypes::vec3f sideVec =
        JBTypesMethods::directionAsVector(_currentSide);
    const JBTypes::vec3f lookVec =
        JBTypesMethods::directionAsVector(_lookTowards);

    const float x = static_cast<float> (_currentBlockX) + 0.5f +
        sideVec.x * (offsetRealPosition + p2D.y) + lookVec.x * p2D.x;
    const float y = static_cast<float> (_currentBlockY) + 0.5f +
        sideVec.y * (offsetRealPosition + p2D.y) + lookVec.y * p2D.x;
    const float z = static_cast<float> (_currentBlockZ) + 0.5f +
        sideVec.z * (offsetRealPosition + p2D.y) + lookVec.z * p2D.x;

    return JBTypes::vec3f {x,y,z};
}

float Ball::getRadius() const {
    return ClassicalMechanics::radiusBall;
}

JBTypes::Dir Ball::currentSide() const {
    return _currentSide;
}

JBTypes::Dir Ball::lookTowards() const {
    return _lookTowards;
}

float Ball::getTimeSecondsSinceAction() const noexcept{
    const JBTypes::timePointMs timeNowMs = JBTypesMethods::getTimePointMSNow();
    const JBTypes::timePointMs timeActionMs= getTimeActionMs();
    return JBTypesMethods::getFloatFromDurationMS(timeNowMs - timeActionMs);
}

float Ball::getTimeSecondsSinceStateOfLife() const noexcept{
    const JBTypes::timePointMs timeNowMs = JBTypesMethods::getTimePointMSNow();
    const JBTypes::timePointMs timeStateOfLifeMs= getTimeStateOfLifeMs();
    return JBTypesMethods::getFloatFromDurationMS(timeNowMs -
                                                  timeStateOfLifeMs);
}

JBTypes::vec3f Ball::lookTowardsAsVector() const {
    return JBTypesMethods::directionAsVector(_lookTowards);
}

JBTypes::vec3f Ball::currentSideAsVector() const {
    return JBTypesMethods::directionAsVector(_currentSide);
}

ClassicalMechanics& Ball::getMechanicsJumping() noexcept {
    //Scott Meyer's advice to avoid code duplication
    return const_cast<ClassicalMechanics&> (
            static_cast<const Ball&>(*this).getMechanicsJumping());
}

JBTypes::vec3f Ball::get3DPosStayingBall() const {
    constexpr float offsetPosition = 0.5f + ClassicalMechanics::radiusBall;
    
    const JBTypes::vec3f sideVec =
        JBTypesMethods::directionAsVector(_currentSide);
    const float x = static_cast<float> (_currentBlockX) + 0.5f +
        sideVec.x * offsetPosition;
    const float y = static_cast<float> (_currentBlockY) + 0.5f +
        sideVec.y * offsetPosition;
    const float z = static_cast<float> (_currentBlockZ) + 0.5f +
        sideVec.z * offsetPosition;
    
    return {x,y,z};
}

void Ball::blockEvent(std::shared_ptr<Block> block) noexcept{
    block->detectionEvent(_currentSide,
            JBTypesMethods::getTimePointMsFromTimePoint( _timeAction));
    if (_stateOfLife != StateOfLife::Bursting && block) {
        const Block::categoryOfBlocksInFile type = block->getType();
        if (type == Block::categoryOfBlocksInFile::Jump) {
            const unsigned int dir =
                JBTypesMethods::directionAsInteger(_currentSide);
            if (block->faceInfo().at(dir)){
                _jumpingType = Ball::JumpingType::Long;
                jump();
            }
        } else if (type == Block::categoryOfBlocksInFile::Fire) {
            _burnCoefficientCurrent += .2f;
            _burnCoefficientTrigger = _burnCoefficientCurrent;
            _stateOfLife = StateOfLife::Burning;
            setTimeLifeNow();
        } else if (type == Block::categoryOfBlocksInFile::Ice) {
            _burnCoefficientTrigger = 0.f;
        }
    }
 }


void Ball::jumpingUpdate() noexcept {
    const ClassicalMechanics::physics2DVector pos2D =
        getMechanicsJumping(). getPosition(getTimeSecondsSinceAction());

    const JBTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
    _3DPosX = relativePositionJump.x;
    _3DPosY = relativePositionJump.y;
    _3DPosZ = relativePositionJump.z;
    isFallingIntersectionBlock();
}

void Ball::fallingUpdate() noexcept {
    const ClassicalMechanics::physics2DVector pos2D =
        _mechanicsPatternFalling. getPosition(getTimeSecondsSinceAction());

    const JBTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
    _3DPosX = relativePositionJump.x;
    _3DPosY = relativePositionJump.y;
    _3DPosZ = relativePositionJump.z;
    isFallingIntersectionBlock();
}

void Ball::stayingUpdate() noexcept {
    const JBTypes::vec3f position3D = get3DPosStayingBall();
    const std::shared_ptr<Block> block =
        _map.getBlock(_currentBlockX,_currentBlockY,_currentBlockZ);
    if (block &&  block ->stillExists()) {
        _3DPosX = position3D.x;
        _3DPosY = position3D.y;
        _3DPosZ = position3D.z;
    } else {
        fall();
        update();
    }
}

void Ball::turningUpdate() noexcept {
    JBTypes::vec3f position3D = get3DPosStayingBall();
    float sSinceAction = getTimeSecondsSinceAction();
    if (sSinceAction  >= (timeToTurn)) {
        stay();
        update();
    } else {
        _3DPosX = position3D.x;
        _3DPosY = position3D.y;
        _3DPosZ = position3D.z;
    }
}

void Ball::movingUpdate() noexcept {
    JBTypes::vec3f position3D = get3DPosStayingBall();
    float sSinceAction = getTimeSecondsSinceAction();
    if (sSinceAction >= timeToGetNextBlock) {
        goStraightAhead();
        position3D  = get3DPosStayingBall();
        _3DPosX     = position3D.x;
        _3DPosY     = position3D.y;
        _3DPosZ     = position3D.z;
        stay();
        blockEvent(_map.getBlock
                   (_currentBlockX,_currentBlockY,_currentBlockZ));
    }
    else {
        struct nextBlockInformation infoTarget = getNextBlockInfo();
        if (infoTarget.nextLocal == NextBlockLocal::InFrontOf){
            _3DPosX = (sSinceAction * ( static_cast<float> (infoTarget.poxX)
                        - static_cast<float>(_currentBlockX) )
                       / timeToGetNextBlock) + position3D.x;
            _3DPosY = (sSinceAction * ( static_cast<float> (infoTarget.poxY)
                                       - static_cast<float>(_currentBlockY) )
                       / timeToGetNextBlock) + position3D.y;
            _3DPosZ = (sSinceAction * ( static_cast<float> (infoTarget.poxZ)
                                       - static_cast<float>(_currentBlockZ) )
                       / timeToGetNextBlock) + position3D.z;
        }
        else {
            _3DPosX     = position3D.x;
            _3DPosY     = position3D.y;
            _3DPosZ     = position3D.z;
            
            if ( infoTarget.nextLocal == NextBlockLocal::Same ||
                infoTarget.nextLocal == NextBlockLocal::Above ) {
                float distancePerStep;
                
                if ( infoTarget.nextLocal == NextBlockLocal::Same)
                    distancePerStep = 0.5f + ClassicalMechanics::radiusBall;
                else
                    distancePerStep = 0.5f - ClassicalMechanics::radiusBall;
                
                float timeStep1 = sSinceAction;
                if (timeStep1 > timeToGetNextBlock/2.f)
                    timeStep1 = timeToGetNextBlock/2.f;
                
                float timeStep2 = sSinceAction - timeToGetNextBlock/2.f;
                if (timeStep2 < 0)
                    timeStep2 = 0;

                const JBTypes::vec3f lookVec =
                JBTypesMethods::directionAsVector(_lookTowards);
                const JBTypes::vec3f nextLookVec =
                JBTypesMethods::directionAsVector(infoTarget.nextLook);
                
                const float distStep1 = distancePerStep * timeStep1 /
                (timeToGetNextBlock/2.f);
                const float distStep2 = distancePerStep * timeStep2 /
                (timeToGetNextBlock/2.f);
                
                _3DPosX += lookVec.x * distStep1 + nextLookVec.x * distStep2;
                _3DPosY += lookVec.y * distStep1 + nextLookVec.y * distStep2;
                _3DPosZ += lookVec.z * distStep1 + nextLookVec.z * distStep2;
            }
        }
    }
}

void Ball::burningUpdate() noexcept
{
    if (_stateOfLife != StateOfLife::Bursting) {
        const float time = getTimeSecondsSinceStateOfLife();
        if ( _stateOfLife == StateOfLife::Burning &&
             (_state == State::Staying || _state == State::TurningLeft ||
              _state == State::TurningRight)) {
            // the ball is burning
            _burnCoefficientCurrent =
                    _burnCoefficientTrigger + (time/timeToBurn);
            if( _burnCoefficientCurrent > 1.f ) {
                _stateOfLife = StateOfLife::Bursting;
                setTimeLifeNow();
            }
        }
        else {
            // the ball is cooldown
            if ( _stateOfLife == StateOfLife::Burning) {
                _stateOfLife = StateOfLife::Normal;
                setTimeLifeNow();
                _burnCoefficientTrigger = _burnCoefficientCurrent;
            }
            _burnCoefficientCurrent =
                    _burnCoefficientTrigger - (time/timeToBurn);
            if (_burnCoefficientCurrent < 0 ) _burnCoefficientCurrent = 0.f;
        }
    }
    std::cout << _burnCoefficientCurrent << std::endl;
}

void Ball::update() noexcept{
    switch (_state) {
        case Ball::State::Falling: fallingUpdate(); break;
        case Ball::State::Jumping: jumpingUpdate(); break;
        case Ball::State::TurningLeft: turningUpdate(); break;
        case Ball::State::TurningRight: turningUpdate(); break;
        case Ball::State::Staying: stayingUpdate(); break;
        case Ball::State::Moving: movingUpdate(); break;
        default: break;
    }
    mapInteraction();
    burningUpdate();
    if (isOutOfTheMap()) die();
}

void Ball::mapInteraction() noexcept{

    Map::EffectOnBall effect = _map.interaction(_currentSide,get3DPosition());

    switch ( effect ) {
        case Map::EffectOnBall::Nothing: 
            break;
        case Map::EffectOnBall::Burnt:
            if(_stateOfLife != StateOfLife::Burning) {
                _stateOfLife = StateOfLife::Burning;
                _timeStateOfLife = JBTypesMethods::getTimePointMSNow();
            }
            break;
        case Map::EffectOnBall::Burst: 
            if(_stateOfLife != StateOfLife::Bursting) {
                _stateOfLife = StateOfLife::Bursting;
                _timeStateOfLife = JBTypesMethods::getTimePointMSNow(); 
            }
            break;
        case Map::EffectOnBall::Slide: 
            break;
        default: break;
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
    if (_3DPosX < -thresholdOut ||
        _3DPosX > (static_cast<float>(_map.boundingBoxXMax()) + thresholdOut) ||
        _3DPosY < -thresholdOut ||
        _3DPosY > (static_cast<float>(_map.boundingBoxYMax()) + thresholdOut) ||
        _3DPosZ < -thresholdOut ||
        _3DPosZ > (static_cast<float>(_map.boundingBoxZMax()) + thresholdOut)) {
        return true;
    }
    else return false;
}


const TurnLeft Ball::turnLeftMovement;
const TurnRight Ball::turnRightMovement;
const TurnBack Ball::turnBackMovement;
const NextBlock Ball::nextBlockGetter;
