//
// Created by S Morgenthaler on 15/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_EXITBLOCK_H
#define JUMPERBALLAPPLICATION_EXITBLOCK_H

#include "InteractiveBlock.h"

class ExitBlock : public InteractiveBlock {
public:
    ExitBlock(
        const JBTypes::vec3ui &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr &ball,
        const JBTypes::Direction &exitDir,
        bool isUnlockModel
    );

    Effect detectionEvent() override;

    JBTypes::Color getColor() const override;

    vecCstShape_sptr getExtraShapes() const override;

    void unlockExit(bool makeSound) override;

    std::string getDynamicGroupHash() const override;

    DynamicValues <JBTypes::vec3f> getDynamicVec3fValues() const override;

private:

    const JBTypes::Direction _exitDir;
    const bool _isUnlockModel;
    JBTypes::vec3f _scale;
    bool _isUnlocked;
};

#endif //JUMPERBALLAPPLICATION_EXITBLOCK_H
