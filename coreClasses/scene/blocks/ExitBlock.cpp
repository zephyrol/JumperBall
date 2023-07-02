//
// Created by S.Morgenthaler on 15/01/2022.
//

#include "ExitBlock.h"
#include "system/SoundOutput.h"

ExitBlock::ExitBlock(const JBTypes::vec3ui &position,
                     const vecItem_sptr &items,
                     const vecEnemy_sptr &enemies,
                     const vecSpecial_sptr &specials,
                     const Ball_sptr& ball,
                     const JBTypes::Direction& exitDir,
                     bool isUnlockModel

):
        InteractiveBlock(position, items, enemies, specials, ball),
        _exitDir(exitDir),
        _isUnlockModel(isUnlockModel),
        _isUnlocked(false)
{
    if (!isUnlockModel) {
       return;
    }
    _localScale.x = 0.f;
    _localScale.y = 0.f;
    _localScale.z = 0.f;
}
vecCstShape_sptr ExitBlock::getExtraShapes() const {

    vecCstShape_sptr shapes {};
    constexpr float sizeBlock = 1.f; // TODO specify it elsewhere
    constexpr float offset = sizeBlock / 2.f;

    const JBTypes::vec3f localScale { 0.7f, 0.05f, 0.7f };

    const JBTypes::vec3f translationOnBlock {
        0.f,
        offset,
        0.f
    };

    const JBTypes::vec3f translationPosition {
            static_cast<float>(_position.at(0)),
            static_cast<float>(_position.at(1)),
            static_cast<float>(_position.at(2))
    };

    const auto directionRotation = JBTypesMethods::rotationVectorUpToDir(_exitDir);

    shapes.push_back(std::make_shared<const Shape>(
        Shape::Aspect::Cylinder,
        _isUnlockModel ? JBTypes::Color::Green : JBTypes::Color::Red,
        std::initializer_list<Transformation>({
            Transformation(Transformation::Type::Scale, localScale),
            Transformation(Transformation::Type::Translation, translationOnBlock),
            Transformation(Transformation::Type::Rotation, directionRotation),
            Transformation(Transformation::Type::Translation, translationPosition)
        })
    ));

    return shapes;
}

void ExitBlock::unlockExit() {
    _isUnlocked = true;
    if (_isUnlockModel) {
        _localScale.x = 1.f;
        _localScale.y = 1.f;
        _localScale.z = 1.f;
        return;
    }
    _localScale.x = 0.f;
    _localScale.y = 0.f;
    _localScale.z = 0.f;
    _ball.lock()->addUpdateOutput(std::make_shared<SoundOutput>("exitIsUnlocked"));
}

Block::Effect ExitBlock::detectionEvent() {
    const auto ball = _ball.lock();
    if(_isUnlockModel) {
        ball->addUpdateOutput(std::make_shared<SoundOutput>("hasWon"));
    }
    return _isUnlocked && ball->currentSide() == _exitDir
        ? Block::Effect::FinishLevel
        : Block::Effect::Nothing;
}

JBTypes::Color ExitBlock::getColor() const {
    return _isUnlockModel ? JBTypes::Color::Green : JBTypes::Color::Red;
}

std::string ExitBlock::getDynamicGroupHash() const {
    return "exit;" + std::string(_isUnlockModel ? "lock" : "unlock");
}
