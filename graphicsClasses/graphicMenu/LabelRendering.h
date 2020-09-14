/*
 * File:   LabelRendering.h
 * Author: Morgenthaler S
 *
 * Created on 6 septembre 2020, 17:15
 */

#ifndef LABELRENDERING_H
#define LABELRENDERING_H
#include <gameMenu/Label.h>
#include "Utility.h"

class LabelRendering;
using LabelRendering_sptr = std::shared_ptr<LabelRendering>;
using CstLabelRendering_sptr = std::shared_ptr<const LabelRendering>;
using vecCstLabelRendering_sptr = std::vector<CstLabelRendering_sptr>;
using vecLabelRendering_sptr = std::vector<LabelRendering_sptr>;

class LabelRendering
{
public:
    LabelRendering(const Label& label);
    virtual void update() = 0;
    virtual void render() const = 0;

protected:
    const Label& _label;
    static const glm::mat4 biasMatrix;

private:
    static constexpr float biasScalar = 2.f; //To multiply the translation by 2

    glm::mat4 _transform;

};

#endif // LABELRENDERING_H