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
        _burnCoefficient(0.f){
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

    struct nextBlockInformation nextBlock;

    const std::array<int,12> offsetsNextBlocks = nextBlockGetter.evaluate
        ({_currentSide,_lookTowards});
    
    int inFrontOfX = _currentBlockX + offsetsNextBlocks.at(0);
    int inFrontOfY = _currentBlockY + offsetsNextBlocks.at(1);
    int inFrontOfZ = _currentBlockZ + offsetsNextBlocks.at(2);
    
    int leftX = _currentBlockX + offsetsNextBlocks.at(3);
    int leftY = _currentBlockY + offsetsNextBlocks.at(4);
    int leftZ = _currentBlockZ + offsetsNextBlocks.at(5);
    
    int rightX = _currentBlockX + offsetsNextBlocks.at(6);
    int rightY = _currentBlockY + offsetsNextBlocks.at(7);
    int rightZ = _currentBlockZ + offsetsNextBlocks.at(8);
    
    int aboveX = _currentBlockX + offsetsNextBlocks.at(9);
    int aboveY = _currentBlockY + offsetsNextBlocks.at(10);
    int aboveZ = _currentBlockZ + offsetsNextBlocks.at(11);
    
    auto blockAbove = _map.getBlock(aboveX,aboveY,aboveZ);
    auto blockInFrontOf = _map.getBlock(inFrontOfX,inFrontOfY,inFrontOfZ);
    auto blockLeft = _map.getBlock(leftX,leftY,leftZ);
    auto blockRight = _map.getBlock(rightX,rightY,rightZ);

    if (blockAbove && blockAbove->stillExists()) {
        nextBlock.poxX = aboveX;
        nextBlock.poxY = aboveY;
        nextBlock.poxZ = aboveZ;
        nextBlock.nextLocal = NextBlockLocal::Above;

        JBTypes::Dir lookTowardsBeforeMovement = _lookTowards;

        nextBlock.nextLook = _currentSide;
        
        nextBlock.nextSide= turnBackMovement.evaluate({lookTowardsBeforeMovement});
    }
    else if (blockInFrontOf && blockInFrontOf->stillExists()) {
        nextBlock.poxX      = inFrontOfX;
        nextBlock.poxY      = inFrontOfY;
        nextBlock.poxZ      = inFrontOfZ;
        nextBlock.nextLocal = NextBlockLocal::InFrontOf;
        nextBlock.nextLook  = _lookTowards;
        nextBlock.nextSide  = _currentSide;
    }
    else if ((!blockLeft || !blockLeft ->stillExists())
            && (!blockRight || !blockRight ->stillExists())) {
        nextBlock.poxX      = _currentBlockX;
        nextBlock.poxY      = _currentBlockY;
        nextBlock.poxZ      = _currentBlockZ;
        nextBlock.nextLocal = NextBlockLocal::Same;

        JBTypes::Dir sideBeforeMovement = _currentSide;

        nextBlock.nextSide  = _lookTowards;
        
        nextBlock.nextLook = turnBackMovement.evaluate({sideBeforeMovement});
    }
    else {
        nextBlock.nextLocal = NextBlockLocal::None;
        nextBlock.nextSide  = _currentSide;
        nextBlock.nextLook  = _lookTowards;
        nextBlock.poxX      = _currentBlockX;
        nextBlock.poxY      = _currentBlockY;
        nextBlock.poxZ      = _currentBlockZ;
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
    
    struct nextBlockInformation infos = getNextBlockInfo();
    if (infos.nextLocal != NextBlockLocal::None)
    {
        _state = Ball::State::Moving;
        setTimeActionNow();
    }
}

void Ball::setTimeActionNow() noexcept {
   _timeAction = std::chrono::system_clock::now();
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
        
        const JBTypes::vec3f sideVec =
            JBTypesMethods::directionAsVector(_currentSide);
        const JBTypes::vec3f lookVec =
            JBTypesMethods::directionAsVector(_lookTowards);
        
        int aboveNearX =  _currentBlockX + sideVec.x + lookVec.x;
        int aboveNearY =  _currentBlockY + sideVec.y + lookVec.y;
        int aboveNearZ =  _currentBlockZ + sideVec.z + lookVec.z;
        int aboveFarX =   _currentBlockX + sideVec.x + 2 * lookVec.x;
        int aboveFarY =   _currentBlockY + sideVec.y + 2 * lookVec.y;
        int aboveFarZ =   _currentBlockZ + sideVec.z + 2 * lookVec.z;
        int aboveVeryFarX =   _currentBlockX + sideVec.x + 3 * lookVec.x;
        int aboveVeryFarY =   _currentBlockY + sideVec.y + 3 * lookVec.y;
        int aboveVeryFarZ =   _currentBlockZ + sideVec.z + 3 * lookVec.z;

        std::shared_ptr<const Block> blockNear  = 
                _map.getBlock(aboveNearX,aboveNearY,aboveNearZ);
        
        std::shared_ptr<const Block> blockFar   = 
                _map.getBlock(aboveFarX,aboveFarY,aboveFarZ);

        std::shared_ptr<const Block> blockVeryFar   = 
                _map.getBlock(aboveVeryFarX,aboveVeryFarY,aboveVeryFarZ);
        
        constexpr float distanceNear      = 1.f;
        constexpr float distanceFar       = 2.f;
        constexpr float distanceVeryFar   = 3.f;
        constexpr float sizeBlock         = 1.f;
       
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

float Ball::distanceBehindBall() const 
{
    constexpr float offsetCenterBlock = 0.5f;
    float distance;
    switch (_lookTowards) {
        case JBTypes::Dir::North:
            distance = _3DPosZ- (static_cast<float>(_currentBlockZ)
                              + offsetCenterBlock);
            break;
        case JBTypes::Dir::South:
            distance = _3DPosZ- (static_cast<float>(_currentBlockZ)
                              + offsetCenterBlock);
            break;
        case JBTypes::Dir::East:
            distance = _3DPosX- (static_cast<float>(_currentBlockX)
                              + offsetCenterBlock);
            break;
        case JBTypes::Dir::West:
            distance = _3DPosX- (static_cast<float>(_currentBlockX)
                              + offsetCenterBlock);
            break;
        case JBTypes::Dir::Up:
            distance = _3DPosY- (static_cast<float>(_currentBlockY)
                              + offsetCenterBlock);
            break;
        case JBTypes::Dir::Down:
            distance = _3DPosY- (static_cast<float>(_currentBlockY)
                              + offsetCenterBlock);
            break;
        default :
            distance = 0.f;
            break;
    }

    if (distance < 0 ) distance = -distance;

    distance = offsetCenterBlock + distance ;
    if (_state== Ball::State::TurningLeft ||_state== Ball::State::TurningRight)
        distance *= 10;
    return distance;
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

float Ball::getTimeActionSecondsFloat() const noexcept {

    const JBTypes::timePointMs timeActionMs        = getTimeActionMs(); 
    
    const JBTypes::durationMs timeActionSinceEpoch =
                                                timeActionMs.time_since_epoch();
    
    const std::chrono::duration<float> durationFloat = timeActionSinceEpoch;
    return durationFloat.count() / 1000.f ;
}

float Ball::getTimeNowSecondsFloat() noexcept {

    const JBTypes::timePointMs timeNowMs = JBTypesMethods::
                                                            getTimePointMSNow();
    
    const JBTypes::durationMs timeNowSinceEpoch  = 
                                                  timeNowMs.time_since_epoch();
    
    const std::chrono::duration<float> durationFloat = timeNowSinceEpoch;
    
   
    return durationFloat.count() / 1000.f ;
}

float Ball::getTimeSecondsSinceAction() const noexcept{

    const JBTypes::timePointMs timeNowMs = 
                                    JBTypesMethods::getTimePointMSNow();
    const JBTypes::durationMs timeNowSinceEpoch = 
                                                  timeNowMs.time_since_epoch();
    const JBTypes::timePointMs timeActionMs = getTimeActionMs(); 
    const JBTypes::durationMs timeActionSinceEpoch    = 
                                                timeActionMs.time_since_epoch();
    const JBTypes::durationMs difference = 
                                      timeNowSinceEpoch - timeActionSinceEpoch;
    const std::chrono::duration<float> durationFloatDifference = difference;
    const float fDifference = durationFloatDifference.count();

    return fDifference;
}

float Ball::getTimeSecondsSinceStateOfLife() const noexcept{

    const JBTypes::timePointMs timeNowMs = 
                                    JBTypesMethods::getTimePointMSNow();
    const JBTypes::durationMs timeNowSinceEpoch = 
                                                  timeNowMs.time_since_epoch();
    const JBTypes::timePointMs timeStateOfLifeMs = 
                                                  getTimeStateOfLifeMs(); 
    const JBTypes::durationMs timeStateOfLifeSinceEpoch    = 
                                          timeStateOfLifeMs.time_since_epoch();
    const JBTypes::durationMs difference = 
                                  timeNowSinceEpoch - timeStateOfLifeSinceEpoch;
    const std::chrono::duration<float> durationFloatDifference = difference;
    const float fDifference = durationFloatDifference.count();

    return fDifference;
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

    float x = static_cast<float> (_currentBlockX + 0.5f);
    float y = static_cast<float> (_currentBlockY + 0.5f);
    float z = static_cast<float> (_currentBlockZ + 0.5f);
        
    switch (_currentSide) {
        case JBTypes::Dir::North:
            z -= offsetPosition ;
            break;
        case JBTypes::Dir::South:
            z += offsetPosition ;
            break;
        case JBTypes::Dir::East:
            x += offsetPosition ;
            break;
        case JBTypes::Dir::West:
            x -= offsetPosition ;
            break;
        case JBTypes::Dir::Up:
            y += offsetPosition ;
            break;
        case JBTypes::Dir::Down:
            y -= offsetPosition ;
            break;
        default :
            break;
    }

    const JBTypes::vec3f position3D {x,y,z};

    return position3D;
}

void Ball::blockEvent(std::shared_ptr<Block> block) noexcept{

    block->detectionEvent(_currentSide,
            JBTypesMethods::getTimePointMsFromTimePoint(
            _timeAction));
    
    if (block) {
        if ((block->getType() == Block::categoryOfBlocksInFile::Jump)) {
            const unsigned int dir =
                JBTypesMethods::directionAsInteger(_currentSide);
            if (block->faceInfo().at(dir)){
                _jumpingType = Ball::JumpingType::Long;
                jump();
            }
        } else if ((block->getType() == Block::categoryOfBlocksInFile::Fire)) {
            _burnCoefficient += .2f;
        }
    }
 }


void Ball::update() noexcept{

    JBTypes::vec3f position3D {0.f,0.f,0.f};

    if (_state == Ball::State::Staying || _state == Ball::State::Moving ||
            _state == Ball::State::TurningLeft || 
            _state == Ball::State::TurningRight) {

        position3D = get3DPosStayingBall();
        if ( _state == Ball::State::Staying ){
            const std::shared_ptr<Block> block =_map.getBlock(_currentBlockX,_currentBlockY,_currentBlockZ);
            if (block &&  block ->stillExists()) {
                _3DPosX = position3D.x;
                _3DPosY = position3D.y;
                _3DPosZ = position3D.z;
            }
            else {
                fall();
                update();
            }
            if (block && block->getType() ==
                Block::categoryOfBlocksInFile::Fire) {
                const auto now = JBTypesMethods::getTimePointMSNow();
                const JBTypes::durationMs duration =
                    now - getTimeActionMs();
                const float time =
                    JBTypesMethods::getFloatFromDurationMS(duration);
                if( _burnCoefficient + time > timeToBurn  ) {
                    
                }

            }
            
            

        } else if ( _state == Ball::State::Moving){
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
                    _3DPosX = (sSinceAction * (
                            static_cast<float> (infoTarget.poxX)
                            - static_cast<float>(_currentBlockX) )
                            / timeToGetNextBlock) + position3D.x;
                    _3DPosY = (sSinceAction * (
                            static_cast<float> (infoTarget.poxY)
                            - static_cast<float>(_currentBlockY) )
                            / timeToGetNextBlock) + position3D.y;
                    _3DPosZ = (sSinceAction * (
                            static_cast<float> (infoTarget.poxZ)
                            - static_cast<float>(_currentBlockZ) )
                            / timeToGetNextBlock) + position3D.z;
                }
                else if ( infoTarget.nextLocal == NextBlockLocal::Same ||
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
                    
                    _3DPosX = position3D.x;
                    _3DPosY = position3D.y;
                    _3DPosZ = position3D.z;
                    
                    switch (_lookTowards) {
                        case JBTypes::Dir::North:
                            _3DPosZ = static_cast<float>(position3D.z) 
                                    - distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::South:
                            _3DPosZ = static_cast<float>(position3D.z)
                                    + distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::East:
                            _3DPosX = static_cast<float>(position3D.x)
                                    + distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::West:
                            _3DPosX = static_cast<float>(position3D.x)
                                    - distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::Up:
                            _3DPosY = static_cast<float>(position3D.y)
                                    + distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::Down:
                            _3DPosY = static_cast<float>(position3D.y)
                                    - distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        default :
                            break;
                    }
                    switch (infoTarget.nextLook) {
                        case JBTypes::Dir::North:
                            _3DPosZ = static_cast<float>(_3DPosZ) 
                                    - distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::South:
                            _3DPosZ = static_cast<float>(_3DPosZ)
                                    + distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::East:
                            _3DPosX = static_cast<float>(_3DPosX)
                                    + distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::West:
                            _3DPosX = static_cast<float>(_3DPosX)
                                    - distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::Up:
                            _3DPosY = static_cast<float>(_3DPosY)
                                    + distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JBTypes::Dir::Down:
                            _3DPosY = static_cast<float>(_3DPosY)
                                    - distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        default :
                            break;
                    }
                    
                } else {
                    _3DPosX = position3D.x;
                    _3DPosY = position3D.y;
                    _3DPosZ = position3D.z;
                }
                
            }
        } else if ( _state == Ball::State::TurningLeft ||
                _state == Ball::State::TurningRight) {
            
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
    } 
    else if ( _state == Ball::State::Jumping || 
            _state == Ball::State::Falling ) {

        ClassicalMechanics::physics2DVector pos2D;
        if (_state == Ball::State::Jumping) {
            pos2D = getMechanicsJumping().
                    getPosition(getTimeSecondsSinceAction());
        } else {
            pos2D = _mechanicsPatternFalling.
                    getPosition(getTimeSecondsSinceAction());
        }

        JBTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
        
        position3D = {  relativePositionJump.x,
                        relativePositionJump.y,
                        relativePositionJump.z };
        _3DPosX = position3D.x;
        _3DPosY = position3D.y;
        _3DPosZ = position3D.z;
        
        isFallingIntersectionBlock();
    }
    mapInteraction();
    
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

void Ball::die() noexcept{
    _stateOfLife = Ball::StateOfLife::Dead;
}

bool Ball::isOutOfTheMap() const {
    constexpr float thresholdOut = 5.f;
    if ( _3DPosX < -thresholdOut ||
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
