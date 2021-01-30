/*
 * File: ObjectRendering.h
 * Author: Morgenthaler S
 *
 * Created on 17 janvier 2021, 11:20
 */

#ifndef OBJECTRENDERING_H
#define OBJECTRENDERING_H
#include <map>
#include <gameMenu/ObjectLabel.h>
#include <system/ParallelTask.h>
#include <geometry/Triangle.h>
#include "LabelRendering.h"
#include "scene/mesh/MeshGenerator.h"

class ObjectRendering;
using ObjectRendering_sptr = std::shared_ptr <ObjectRendering>;
using CstObjectRendering_sptr = std::shared_ptr <const ObjectRendering>;
using vecCstObjectRendering_sptr = std::vector <CstObjectRendering_sptr>;
using vecObjectRendering_sptr = std::vector <ObjectRendering_sptr>;

class ObjectRendering:public LabelRendering {
public:
ObjectRendering(const Label& label);
virtual ~ObjectRendering() = default;

void render() const override;
void update(float offset) override;

private:
glm::mat4 _transformObject;
static const std::string vsshaderObject;
static const std::string fsshaderObject;
static std::shared_ptr <const ShaderProgram> spObject;
vecMeshComponent_sptr _components;
};

#endif // OBJECTRENDERING_H
