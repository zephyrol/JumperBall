/*
 * File: Block.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef BLOCK_H
#define BLOCK_H
#include "system/Types.h"
#include "scene/objects/Clock.h"
#include "scene/objects/Coin.h"
#include "scene/objects/Key.h"

class Block {
public:


// --CONSTRUCTORS & DESTRUCTORS--//
Block(const JBTypes::vec3ui& position,
      bool hasInteraction = false,
      bool isFixed = true);
virtual ~Block() = default;

// ------------TYPES------------//
enum class Effect { Nothing, Burst, Burn, Slide, Jump };

// ----------METHODS-------------//
virtual Effect interaction(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime,
    const JBTypes::vec3f& posBall,
    const JBTypes::vec3ui& posBlock
 );
virtual Effect detectionEvent(const JBTypes::Dir& ballDir, const JBTypes::timePointMs& currentTime);
virtual void createObject(Object::CategoryOfObjects category, JBTypes::Dir dir);

static constexpr size_t objectsNumber = 6;

virtual std::array <bool, 6> faceInfo() const;
virtual bool isExists() const;

const JBTypes::vec3f& localScale() const;
const JBTypes::vec3f& localTranslation() const;

const std::shared_ptr <const Object> object(size_t number) const;
bool hasInteraction() const;
bool hasObjects() const;
void catchObject(
    const JBTypes::vec3ui&
    blockPosition,
    const JBTypes::vec3f& entityPosition,
    float radiusEntity);
virtual const bool& isFixed() const;
const std::array <std::shared_ptr <Object>, 6>& objects() const;
const JBTypes::vec3ui& position() const;

static JBTypes::vec3f objectPosition(const JBTypes::vec3ui& pos, unsigned int dirUint);

protected:
// --------ATTRIBUTES-----------//
const JBTypes::vec3ui _position;
const bool _hasInteraction;
const bool _isFixed;
JBTypes::vec3f _localScale;
JBTypes::vec3f _localTranslation;
std::array <std::shared_ptr <Object>, 6> _objects;
bool _hasObjects;
};

#endif /* BLOCK_H */
