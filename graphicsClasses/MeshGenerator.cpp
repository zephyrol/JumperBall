/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeshGenerator.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 12 avril 2020, 19:24
 */

#include "MeshGenerator.h"
#include <iterator>
#include <algorithm>




std::vector<MeshComponent> MeshGenerator::genComponents(const Ball& ball) {

    const glm::mat4 scaleMatrix = glm::scale(glm::vec3(ball.getRadius()));
    const Sphere sphere;
     
    MeshComponent component ( std::make_shared<Sphere>(sphere,scaleMatrix),
                                std::make_shared<BallAnimation>(ball));
    return std::vector<MeshComponent> {component};
}

std::vector<MeshComponent> MeshGenerator::genJumpers( 
                                        const Block& block, 
                                        const glm::vec3& posWorld) {
    std::vector<MeshComponent> components;
    if (block.getType() == Block::categoryOfBlocksInFile::Jump) {
        
        if (commonShapes.find("jumperCylinder") == commonShapes.end()) {
            commonShapes["jumperCylinder"] = std::make_shared<Cylinder> (
                    glm::vec3(1.f,1.f,0.f),
                    glm::vec3(1.f,150.f/255.f,0.f));

        }

        for(size_t i = 0; i < block.faceInfo().size(); i++) {
            
            bool jumperPresent = block.faceInfo().at(i);
            if (jumperPresent) {
                
                constexpr float sizeBlock = 1.f;
                constexpr float offset = sizeBlock/2.f;
                
                const JBTypes::Dir currentDir =
                        JBTypesMethods::integerAsDirection(
                            static_cast<unsigned int>(i));
                const JBTypes::vec3f vecDir = JBTypesMethods::
                        directionAsVector(currentDir);
                
                const glm::mat4 rotationLocal = 
                        Utility::rotationUpToDir(currentDir);
                
                const glm::mat4 scaleLocal = 
                glm::scale(glm::vec3(0.7f, 0.05f, 0.7f));
                
                
                const glm::mat4 translationLocal =
                glm::translate( glm::vec3(
                posWorld.x + offset + vecDir.x * offset,
                        posWorld.y + offset + vecDir.y * offset,
                        posWorld.z + offset + vecDir.z * offset
                ));
                
                const glm::mat4 modelTranf= translationLocal * 
                rotationLocal * scaleLocal ;
                const glm::mat4 normalsTrans= rotationLocal; 
                
                MeshComponent component ( std::make_shared<Cylinder>
                (*commonShapes.at("jumperCylinder"),modelTranf,normalsTrans),
                        nullptr);
                components.push_back(std::move(component));
            }
        }
    }
    return components;

}

std::vector<MeshComponent> MeshGenerator::genSharps( 
                                        const Block& block, 
                                        const glm::vec3& posWorld) {
    
    std::vector<MeshComponent> components;
    if (block.getType() == Block::categoryOfBlocksInFile::Sharp) {
        
        if (commonShapes.find("pyramidSharp") == commonShapes.end()) {
            commonShapes["pyramidSharp"] = std::make_shared<Pyramid> ();
        }

        const std::array<float,7> scales {.2f,.1f,.05f,.1f,.075f,.15f,.175f};
        
        const std::array<glm::vec2,7> translationFloorFactor 
        {
            glm::vec2(0.f,0.f),glm::vec2(-0.6f,-0.4f), glm::vec2(0.6f,-0.6f),
            glm::vec2(0.2f,0.6f), glm::vec2(-0.2f,-0.6f), glm::vec2(0.6f,0.6f),
            glm::vec2(-0.6f,0.6f)
        };
        
        for(size_t i = 0; i < block.faceInfo().size(); i++) {
            
            const bool isSharp = block.faceInfo().at(i);
            if (isSharp) {
                
                constexpr float sizeBlock = 1.f;
                constexpr float offset = sizeBlock/2.f;
                
                const JBTypes::Dir currentDir =
                        JBTypesMethods::integerAsDirection(
                            static_cast<unsigned int>(i));
                const JBTypes::vec3f vecDir = 
                    JBTypesMethods:: directionAsVector(currentDir);
                
                const glm::mat4 translationOffset = glm::translate(
                        glm::vec3( -offset, 0 , -offset ));
                
                const glm::mat4 rotationLocal = 
                        Utility::rotationUpToDir(currentDir);
                
                
                for (size_t j = 0 ; j < scales.size() ; j++) {
                    
                    const glm::mat4 scaleLocal = 
                            glm::scale(glm::vec3(scales.at(j),0.5f,
                            scales.at(j)));
                    
                    
                    const glm::mat4 translationLocal =
                            glm::translate( glm::vec3(
                            posWorld.x + offset + vecDir.x * offset,
                            posWorld.y + offset + vecDir.y * offset,
                            posWorld.z + offset + vecDir.z * offset
                            ));
                    
                    const glm::mat4 translationFloor = glm::translate( 
                            glm::vec3(
                            offset * translationFloorFactor.at(j).x,
                            0.f,
                            offset * translationFloorFactor.at(j).y
                            ));
                    
                    const glm::mat4 modelTranf= translationLocal * 
                            rotationLocal * translationFloor * scaleLocal *
                            translationOffset;
                    const glm::mat4 normalsTrans= rotationLocal; 
                    
                    MeshComponent component ( std::make_shared<Pyramid>
                    (*commonShapes.at("pyramidSharp"),modelTranf,normalsTrans),
                            nullptr);
                    components.push_back(std::move(component));
                } 
            }
        }
    }
    return components;
}


std::vector<MeshComponent> MeshGenerator::sortComponents(
                                std::vector<MeshComponent>& components) {
    std::vector<MeshComponent> sortedComponents;
    bool hasToSwap = true;
    std::vector<size_t> indices (components.size());
    for (unsigned int i = 0; i < components.size(); ++i) {
        indices.at(i) = i;
    }
    while(hasToSwap) {
        hasToSwap = false;
        for (size_t i = 0; i < components.size()-1; ++i) {
            const MeshComponent& c1 = components.at(indices.at(i));
            const MeshComponent& c2 = components.at(indices.at(i+1));
            if (*c1.shape()->vertexArrayObject() > 
                    *c2.shape()->vertexArrayObject() ){
                const size_t intermediate = indices.at(i);
                indices.at(i) = indices.at(i+1);
                indices.at(i+1) = intermediate;
            }
        }
    }
    
    for (unsigned int i = 0; i < components.size(); ++i) {
        const size_t index = indices.at(i);
        sortedComponents.push_back(std::move(components.at(index)));
    }
    return sortedComponents;
}


std::vector<MeshComponent> MeshGenerator::genBlock
    (const Map& map, size_t index) {

    const std::array<unsigned int,3> position = map.getBlockCoords(index);

    const std::shared_ptr<const Block> block = map.getBlock(index);
    
    if (!block) return {};

    std::vector<MeshComponent> components;
    std::shared_ptr<GeometricShape> shape;

    std::string strSidesInfo {};
    std::array<bool,6> boolSidesInfo {}; // true <=> close, false <=> open
    const std::array<std::array<unsigned int, 3>,6 > positions {{
        {position.at(0),position.at(1),position.at(2) - 1},
        {position.at(0),position.at(1),position.at(2) + 1},
        {position.at(0) + 1,position.at(1),position.at(2)},
        {position.at(0) - 1,position.at(1),position.at(2)},
        {position.at(0),position.at(1) + 1,position.at(2)},
        {position.at(0),position.at(1) - 1,position.at(2)}
    }};
    
    for (size_t i = 0; i < 6; ++i) {
        const std::array<unsigned int, 3>& neighbourgPosition = positions.at(i);
        if (const std::shared_ptr<const Block> neighbourg =
            map.getBlock(neighbourgPosition.at(0),
                         neighbourgPosition.at(1),
                         neighbourgPosition.at(2))) {
            if (neighbourg->getType() != Block::categoryOfBlocksInFile::Brittle)
            {
                strSidesInfo.push_back('0');
                boolSidesInfo.at(i) = false;
            } else {
                strSidesInfo.push_back('1');
                boolSidesInfo.at(i) = true;
            }
        } else {
            strSidesInfo.push_back('1');
            boolSidesInfo.at(i) = true;
        }
    }

    if (block->getType() == Block::categoryOfBlocksInFile::Ice) {
        if (commonShapes.find("iceCube" + strSidesInfo) == commonShapes.end()) {
            commonShapes["iceCube" + strSidesInfo] = std::make_shared<Cube>
                (Cube::iceColorsCube,boolSidesInfo);
        }
        shape = commonShapes.at("iceCube" + strSidesInfo);
    } else if (block->getType() == Block::categoryOfBlocksInFile::Fire) {
        if (commonShapes.find("fireCube" + strSidesInfo) ==
            commonShapes.end()) {
            commonShapes["fireCube" + strSidesInfo] = std::make_shared<Cube>
                (Cube::fireColorsCube,boolSidesInfo);
        }
        shape = commonShapes.at("fireCube" + strSidesInfo);
    } else if (block->getType() == Block::categoryOfBlocksInFile::Brittle) {
        if (commonShapes.find("britlleCube") ==
            commonShapes.end()) {
            commonShapes["brittleCube"] = std::make_shared<Cube>();
        }
        shape = commonShapes.at("brittleCube");
    }
    else {
        if (commonShapes.find("basicCube" + strSidesInfo) ==
            commonShapes.end()) {
            commonShapes["basicCube" + strSidesInfo] =
            std::make_shared<Cube>(glm::mat4(1.f),glm::mat4(1.f)
                                   ,boolSidesInfo);
        }
        shape = commonShapes.at("basicCube" + strSidesInfo);
    }

    const glm::vec3 glmPosition { position.at(0), position.at(1),
                                  position.at(2)};
    const glm::mat4 transform (glm::translate(glmPosition));
    
    std::shared_ptr<BlockAnimation> blockAnim =
            std::make_shared<BlockAnimation>(*block);
    MeshComponent component
        (std::make_shared<Cube> (*shape,transform), blockAnim);
    components.push_back(component);
    
    std::vector<MeshComponent> sharpsComponents = genSharps(*block,glmPosition);
    for(MeshComponent& m : sharpsComponents) {
        components.push_back(std::move(m));
    }

    std::vector<MeshComponent> jumperComponents =
        genJumpers(*block,glmPosition);
    for(MeshComponent& m : jumperComponents) {
        components.push_back(std::move(m));
    }
    
    const std::array<std::shared_ptr<const Object>,6> objects =
        block->objects();

    for (size_t i = 0; i < objects.size() ; ++i) {
        if ( objects.at(i) ) {
            const JBTypes::Dir dir =
                JBTypesMethods::integerAsDirection(
                    static_cast<unsigned int>(i));
            std::vector<MeshComponent> v =
                genComponents(objects.at(i),glmPosition,dir);
            for(MeshComponent& m : v) {
                components.push_back(std::move(m));
            }
            //components.insert(components.end(),v.begin(),v.end()) ;
        }
    }

    return components;
}

std::vector<MeshComponent> MeshGenerator::genComponents(const Map& map) {

    std::vector<MeshComponent> components;
    const auto indices = map.validIndicesBlocks();
    for (unsigned int i = 0 ; i < indices.size(); ++i) {
        std::vector<MeshComponent> blockComponents =
                genBlock(map, indices.at(i));
        for(MeshComponent& m : blockComponents) {
            components.push_back(std::move(m));
        }
    }

    //We have better performances if the components are stored with objects
    //with the same VAO/VBO, we will not have to change the binding later
    std::vector<MeshComponent> sortedComponents = sortComponents(components);
    return sortedComponents;
}

std::vector<MeshComponent> MeshGenerator::genComponents(const Quad& ) {

    //The quad var is just used to specify that
    //we want to gen a MeshComponent from a Quad
    
    if (commonShapes.find("screenQuad") == commonShapes.end()) {
        commonShapes["screenQuad"] = std::make_shared<Quad> ();
    }
     
    MeshComponent component ( commonShapes.at("screenQuad"), nullptr);
    return std::vector<MeshComponent> {component};
}


std::vector<MeshComponent> MeshGenerator::genComponents(const Star& star) {

    if (commonShapes.find("starQuad") == commonShapes.end()) {
        commonShapes["starQuad"] = std::make_shared<Quad> ();
    }
    const MeshComponent component ( std::make_shared<Quad>(
                                    *commonShapes.at("starQuad"),
                                    star.initialTransform()),
                                nullptr);
    return std::vector<MeshComponent> {component};
}

std::vector<MeshComponent> MeshGenerator::genComponents(
                                const std::shared_ptr<const Object>& obj,
                                const glm::vec3& position,
                                const JBTypes::Dir& dir) {
    std::vector<MeshComponent> components;
    if (obj) {
        switch ( obj->getCategory() ) {
            case Object::CategoryOfObjects::Key: {

                constexpr size_t nbGeometriesToCreateAKey = 4;
                
                if (commonShapes.find("goldenSphere") == commonShapes.end()) {
                    commonShapes["goldenSphere"] = std::make_shared<Sphere>
                                              (glm::vec3(1.f,215.f/255.f,0.f));
                }
                if (commonShapes.find("goldenCube") == commonShapes.end()) {
                    commonShapes["goldenCube"] = std::make_shared<Cube>
                                              (glm::vec3(1.f,215.f/255.f,0.f));
                }

                const std::array<glm::vec3,nbGeometriesToCreateAKey> scales
                { glm::vec3(0.09f,0.075f,0.050f),glm::vec3(0.05f,0.3f,0.05f),
                  glm::vec3(0.1f,0.05f,0.05f), glm::vec3(0.1f,0.05f,0.05f) };

                const std::array<glm::vec3,nbGeometriesToCreateAKey>
                translations
                {   glm::vec3(0.f, 0.175f,0.f),
                    glm::vec3(-0.025f, -0.175f,-0.025f ),
                    glm::vec3(-0.025f, -0.075f, -0.025f ),
                    glm::vec3(-0.025f, -0.175f, -0.025f )
                };

                for (unsigned int i = 0; i < nbGeometriesToCreateAKey; ++i ) {
                    const glm::mat4 scaleMatrix = glm::scale(scales.at(i));
                    const glm::mat4 translationMatrix =
                                        glm::translate(translations.at(i));
                    const glm::mat4 tranform = translationMatrix * scaleMatrix;
                    if (i == 0){
                    const MeshComponent componentSphere = 
                      {std::make_shared<Sphere>(
                      *commonShapes.at("goldenSphere"),
                              tranform), 
                        std::make_shared<ObjectAnimation>(*obj,position,dir)};
                    components.push_back(componentSphere); 
                    } else {
                    const MeshComponent componentCube = 
                      {std::make_shared<Cube>(*commonShapes.at("goldenCube"), 
                              tranform), 
                        std::make_shared<ObjectAnimation>(*obj,position,dir)};
                    components.push_back(componentCube); 
                    }
               }
                break;
            }
            case Object::CategoryOfObjects::Coin: {
              
                if (commonShapes.find("coinCylinder") == commonShapes.end()) {
                    commonShapes["coinCylinder"] = std::make_shared<Cylinder>
                                        (glm::vec3(1.f,215.f/255.f,0.f),
                                        glm::vec3(150.f/255.f,75.f/255.f,0.f));
                }

                const glm::vec3 scale {0.3f, 0.05f, 0.3f};
                
                const glm::vec3 translation {0.f, 0.f, -0.025f };


                const glm::mat4 scaleMatrix = glm::scale(scale);
                const glm::mat4 translationMatrix = glm::translate(translation);
                const glm::mat4 rotationMatrix = 
                  glm::rotate(static_cast<float>(M_PI/2.),
                              glm::vec3(1.f,0.f,0.f));

                const glm::mat4 tranformLocal =
                  translationMatrix * rotationMatrix * scaleMatrix ;
                const glm::mat4 tranformNormals = rotationMatrix;

                const MeshComponent componentCube = 
                {std::make_shared<Cylinder>(*commonShapes.at("coinCylinder"), 
                        tranformLocal,
                        tranformNormals ), 
                std::make_shared<ObjectAnimation>(*obj,position,dir)};
                components.push_back(componentCube); 
                break;
            }
            case Object::CategoryOfObjects::Clock: {
                break;
            }
            default : break;
        }
    }
    return components;
}

std::map<std::string, std::shared_ptr<GeometricShape> > 
MeshGenerator::commonShapes;

