
/* 
 * File:   ArrowRendering.h
 * Author: Morgenthaler S
 * 
 * Created on 17 janvier 2021, 11:20
 */

#ifndef ARROWRENDERING_H
#define ARROWRENDERING_H
#include <map>
#include <gameMenu/ArrowLabel.h>
#include <system/ParallelTask.h>
#include <geometry/Triangle.h>
#include "LabelRendering.h"

class ArrowRendering;
using ArrowRendering_sptr = std::shared_ptr<ArrowRendering>;
using CstArrowRendering_sptr = std::shared_ptr<const ArrowRendering>;
using vecCstArrowRendering_sptr = std::vector<CstArrowRendering_sptr>;
using vecArrowRendering_sptr = std::vector<ArrowRendering_sptr>;

class ArrowRendering: public LabelRendering {
public:
    ArrowRendering                       (const Label& label);
    virtual ~ArrowRendering() = default;
    struct CharacterTransform           { glm::vec2 localScale;
                                          glm::vec2 localTranslate; };

    void                                render() const override;
    void                                update(float offset) override;
    const glm::vec4&                    getArrowColor() const;
    
private:
    glm::mat4                           _transformQuad;
    glm::mat4                           _transformTriangle;
    
    static void                         updateGeometry();

    static std::shared_ptr<const Quad>  displayQuad;
    static std::shared_ptr<const Triangle> displayTriangle;
    static const glm::vec4              arrowColor;

    static const std::string vsshaderArrow;
    static const std::string fsshaderArrow;
    static std::shared_ptr<const ShaderProgram> spArrow;
    static const glm::mat4 leftTriangleTransform;
    static const glm::mat4 rightTriangleTransform;
    static const glm::mat4 leftQuadTransform;
    static const glm::mat4 rightQuadTransform;
};

#endif // ARROWRENDERING_H
