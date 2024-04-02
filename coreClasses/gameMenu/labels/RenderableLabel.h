//
// Created by S.Morgenthaler on 16/02/2023.
//

#ifndef JUMPERBALLAPPLICATION_RENDERABLELABEL_H
#define JUMPERBALLAPPLICATION_RENDERABLELABEL_H


#include "scene/Displayable.h"

class RenderableLabel : public Displayable {
public:

    /**
     * @param label Base label
     * @param page Page containing label
     */
    RenderableLabel(CstDisplayable_sptr label, CstDisplayable_sptr page);

private:
    /**
     * Label contain the static properties.
     */
    const CstDisplayable_sptr _label;

    /**
    * Every label in a page shared the same dynamic uniform values, so the parent page
    * is used to access them.
    */
    const CstDisplayable_sptr _page;

public:

    std::vector<unsigned char> getStaticUnsignedByteValues() const override;

    std::vector<short> getStaticShortValues() const override;

    StaticValues<float> getStaticFloatValues() const override;

    StaticValues <glm::vec2> getStaticVec2fValues() const override;

    StaticValues <glm::vec3> getStaticVec3fValues() const override;

    DynamicNames getDynamicIntNames() const override;

    DynamicValues<int> getDynamicIntValues() const override;

    DynamicNames getDynamicFloatNames() const override;

    DynamicValues<float> getDynamicFloatValues() const override;

    DynamicNames getDynamicVec2fNames() const override;

    DynamicValues <glm::vec2> getDynamicVec2fValues() const override;

    DynamicNames getDynamicVec3fNames() const override;

    DynamicValues <glm::vec3> getDynamicVec3fValues() const override;

    DynamicNames getDynamicQuaternionNames() const override;

    DynamicValues <glm::quat> getDynamicQuaternionValues() const override;
};


#endif //JUMPERBALLAPPLICATION_RENDERABLELABEL_H
