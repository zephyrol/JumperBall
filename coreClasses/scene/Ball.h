/*
 * File: Ball.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:17
 */

#ifndef BALL_H
#define BALL_H
#include "ClassicalMechanics.h"
#include "movements/TurnLeft.h"
#include "movements/TurnRight.h"
#include "movements/TurnBack.h"
#include "movements/NextBlock.h"
#include <scene/blocks/Block.h>

class Ball;
using Ball_sptr = std::shared_ptr <Ball>;
using Ball_wptr = std::weak_ptr<Ball>;
using CstBall_sptr = std::shared_ptr <const Ball>;
using CstBall_wptr = std::weak_ptr<const Ball>;

class Ball: public SceneElement {
public:

Ball(unsigned int x, unsigned int y, unsigned int z);

static constexpr float timeToGetNextBlock = 0.25f;
static constexpr float timeToTurn = 0.3f;
static constexpr float timeToBurn = 2.f;
static constexpr float timeToBurst = 0.8f;
static constexpr float basicRadius = 0.2f;
static constexpr float teleportationDuration = 1.f;
static constexpr float halfTimeTeleportationDuration = teleportationDuration / 2.f;

enum class State {
    Staying,
    Moving,
    Jumping,
    TurningLeft,
    TurningRight,
    Falling,
    Teleporting,
    Deteleporting
};

enum class StateOfLife { Normal, Bursting, Burning, Sliding, Dead };

enum class JumpingType { Short, Long };

enum class ActionRequest { GoStraightAhead, TurnLeft, TurnRight, Jump, Nothing};


enum class NextDestination { Above, InFrontOf, Same, None };
struct MovementDestination {
    JBTypes::Dir nextSide;
    JBTypes::Dir nextLook;
    NextDestination nextLocal;
    JBTypes::vec3ui pos;
};

using shock = std::array <unsigned int, 3>;

const JBTypes::vec3f& get3DPosition() const noexcept;
const JBTypes::vec3ui& getPosition() const noexcept;
static float getRadius() ;
JBTypes::vec3f lookTowardsAsVector() const;
JBTypes::vec3f currentSideAsVector() const;

JBTypes::Dir currentSide() const;
JBTypes::Dir lookTowards() const;
Ball::State state() const;
Ball::StateOfLife stateOfLife() const;
float burnCoefficient() const;

float getTimeSecondsSinceAction() const noexcept;
JBTypes::timePointMs getTimeActionMs() const noexcept;

float getTimeSecondsSinceStateOfLife() const noexcept;
JBTypes::timePointMs getTimeStateOfLifeMs() const noexcept;

const ClassicalMechanics& getMechanicsJumping() const noexcept;
const ClassicalMechanics& getMechanicsFalling() const noexcept;

JBTypes::Quaternion getCoveredRotation() const noexcept;
float getCrushingCoefficient() const noexcept;
float getTeleportationCoefficient() const noexcept;
const JBTypes::Color& getTeleportationColor() const noexcept;

Ball::MovementDestination getNextBlockInfo() const;
float getTimeToGetDestination() const;

SceneElement::DynamicValues <float> getDynamicFloats() const override;
SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
SceneElement::DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const override;

SceneElement::GlobalState getGlobalState() const override;
CstBlock_sptr getBlock(const JBTypes::vec3ui& pos) const;

void update(const JBTypes::timePointMs& updatingTime, const Ball::ActionRequest& action) noexcept;

void setBlockPositions(const std::shared_ptr<const std::map<std::string, Block_sptr> >& blocksPositions);
void setBlockWithInteractions(const std::shared_ptr<const vecBlock_sptr>& blocksWithInterraction);
void setBlockTeleportations(
    const std::shared_ptr<const std::map<BlockDir , BlockDir> >& blocksTeleportations
);

void updateMovements();

JBTypes::vec3f getNextLook() const;

void die() noexcept;

void teleport(const JBTypes::Color& col) noexcept;

private:

JBTypes::vec3ui _pos;

JBTypes::Dir _currentSide;
JBTypes::Dir _lookTowards;

JBTypes::vec3f _3DPos;

Ball::State _state;
Ball::StateOfLife _stateOfLife;
Ball::JumpingType _jumpingType;

const ClassicalMechanics _mechanicsPatternJumping;
const ClassicalMechanics _mechanicsPatternLongJumping;
const ClassicalMechanics _mechanicsPatternFalling;

std::chrono::time_point <std::chrono::system_clock> _timeAction;
std::chrono::time_point <std::chrono::system_clock> _timeStateOfLife;

// BurnCoefficient at the last state change
float _burnCoefficientTrigger;

// current burnCoefficient
float _burnCoefficientCurrent;

JBTypes::Color _teleportationColor;
float _teleportationCoefficient;

JBTypes::timePointMs _updatingTime;

bool _jumpRequest;
JBTypes::timePointMs _timeJumpRequest;

JBTypes::Quaternion _currentCoveredRotation;
float _currentCrushing;

const TurnLeft _turnLeftMovement;
const TurnRight _turnRightMovement;
const NextBlock _nextBlockGetter;
const TurnBack _turnBackMovement;

Block_sptr getCurrentBlock();

MovementDestination _movementDestination;

std::shared_ptr<const std::map<std::string, Block_sptr> > _blocksPositions;
std::shared_ptr<const vecBlock_sptr> _blockWithInteractions;
std::shared_ptr<const std::map<BlockDir , BlockDir> > _blocksTeleportations;

JBTypes::vec3f P2DTo3D(ClassicalMechanics::physics2DVector p2D) const;
JBTypes::vec3f get3DPosStayingBall() const;
bool isBurstingFinished() const;
JBTypes::vec3f getRotationAxis() const noexcept;
float getJumpingPosX() const noexcept;
float getFallingPosX() const noexcept;


void turnLeft() noexcept;
void turnRight() noexcept;
void goStraightAhead() noexcept;
void stay() noexcept;
void jump() noexcept;
void move() noexcept;
void fall() noexcept;
void deteleport() noexcept;
void setTimeActionNow() noexcept;
void setTimeLifeNow() noexcept;
void blockEvent() noexcept;
ClassicalMechanics& getMechanicsJumping() noexcept;
void isGoingStraightAheadIntersectBlock() noexcept;

void internalUpdate() noexcept;

void fallingUpdate() noexcept;
void stayingUpdate() noexcept;
void movingUpdate() noexcept;
void turningUpdate() noexcept;
void jumpingUpdate() noexcept;
void burningUpdate() noexcept;
void teleportingUpdate() noexcept;
void deteleportingUpdate() noexcept;

void doAction(ActionRequest action);


std::shared_ptr<const JBTypes::vec3ui> intersectBlock() const;
void applyRotation(bool inverse = false);
JBTypes::vec3f getInverseRotationAxis() const noexcept;

    void isFallingIntersectionBlock() noexcept;

    void interaction();
};

#endif /* BALL_H */
