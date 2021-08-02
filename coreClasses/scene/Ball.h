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
#include "SceneElement.h"

class Ball: public SceneElement {
public:

Ball(int x, int y, int z);

static constexpr float timeToGetNextBlock = 0.25f;
static constexpr float timeToTurn = 0.3f;
static constexpr float timeToBurn = 2.f;
static constexpr float timeToBurst = 0.8f;
static constexpr float basicRadius = 0.2f;
static constexpr float teleportationDuration = 1.f;
static constexpr float halfTimeTeleportationDuration = teleportationDuration / 2.f;

enum class State { Staying, Moving, Jumping,
                   TurningLeft, TurningRight, Falling,
                   Teleporting, Deteleporting };

enum class StateOfLife { Normal, Bursting, Burning, Sliding, Dead };

enum class JumpingType { Short, Long };

enum class ActionRequest { GoStraightAhead, TurnLeft, TurnRight, Jump };

using shock = std::array <unsigned int, 3>;

JBTypes::vec3f get3DPosition() const noexcept;
float getRadius() const;
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

float getTimeToGetDestination() const;
JBTypes::vec3f getNextLook() const;

SceneElement::DynamicValues <float> getDynamicFloats() const override;
SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
SceneElement::DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const override;

SceneElement::GlobalState getGlobalState() const override;

void update() noexcept;
void doAction(ActionRequest action);

private:

unsigned int _posX;
unsigned int _posY;
unsigned int _posZ;

float _3DPosX;
float _3DPosY;
float _3DPosZ;

JBTypes::Dir _currentSide;
JBTypes::Dir _lookTowards;
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
size_t _teleportationBlockDestination;

bool _jumpRequest;
JBTypes::timePointMs _timeJumpRequest;

JBTypes::Quaternion _currentCoveredRotation;
float _currentCrushing;

std::shared_ptr <const std::vector <int> >
intersectBlock(float x, float y, float z) const;
JBTypes::vec3f P2DTo3D(ClassicalMechanics::physics2DVector p2D) const;
JBTypes::vec3f get3DPosStayingBall() const;
bool isOutOfTheMap() const;
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
void teleport(const JBTypes::Color& col) noexcept;
void deteleport() noexcept;
void setTimeActionNow() noexcept;
void setTimeLifeNow() noexcept;
void mapInteraction() noexcept;
void blockEvent(const JBTypes::vec3ui& blockPos) noexcept;
void die() noexcept;
ClassicalMechanics& getMechanicsJumping() noexcept;
void isFallingIntersectionBlock() noexcept;
void isGoingStraightAheadIntersectBlock() noexcept;

void setPosition(int x, int y, int z);

void fallingUpdate() noexcept;
void stayingUpdate() noexcept;
void movingUpdate() noexcept;
void turningUpdate() noexcept;
void jumpingUpdate() noexcept;
void burningUpdate() noexcept;
void teleportingUpdate() noexcept;
void deteleportingUpdate() noexcept;

void applyRotation(bool inverse = false);
JBTypes::vec3f getInverseRotationAxis() const noexcept;

static const TurnLeft turnLeftMovement;
static const TurnRight turnRightMovement;
static const TurnBack turnBackMovement;
static const NextBlock nextBlockGetter;
};

#endif /* BALL_H */
