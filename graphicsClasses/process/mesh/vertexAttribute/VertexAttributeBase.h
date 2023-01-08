//
// Created by Sebastien Morgenthaler on 04/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTEBASE_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTEBASE_H

#include "Utility.h"

class VertexAttributeBase;

using VertexAttributeBase_sptr = std::shared_ptr<VertexAttributeBase>;
using CstVertexAttributeBase_sptr = std::shared_ptr<const VertexAttributeBase>;
using vecCstVertexAttributeBase_sptr = std::vector<CstVertexAttributeBase_sptr>;
using vecVertexAttributeBase_sptr = std::vector<VertexAttributeBase_sptr>;

using VertexAttributeBase_uptr = std::unique_ptr<VertexAttributeBase>;
using CstVertexAttributeBase_uptr = std::unique_ptr<const VertexAttributeBase>;
using vecCstVertexAttributeBase_uptr = std::vector<CstVertexAttributeBase_uptr>;
using vecVertexAttributeBase_uptr = std::vector<VertexAttributeBase_uptr>;

class VertexAttributeBase {

public:
    explicit VertexAttributeBase(GLenum dataType);

    VertexAttributeBase(const VertexAttributeBase& vertexAttributeBase) = delete;
    VertexAttributeBase& operator=(const VertexAttributeBase& vertexAttributeBase) = delete;

    GLenum getDataType() const;

    virtual void createDataOnGpu() const = 0;

    virtual size_t dataLength() const = 0;

    virtual ~VertexAttributeBase() = default;

    static size_t getNumberOfVertices(const vecVertexAttributeBase_uptr& vertexAttributes);

    /**
     * Use vertex attribute generation functions to gen them and filter the unused ones
     */
    template<typename T>
    static std::vector<std::unique_ptr<T>> genAndFilter(
        const std::vector<std::function<std::unique_ptr<T>()> > &vertexAttributeGenerationFunctions
    );

private:

    /**
     * Binary operation function usually used in reduce function to filter the unused vertex attributes
     */
    template<typename T>
    static std::vector<std::unique_ptr<T>> filterUnused(
        std::vector<std::unique_ptr<T> > &current,
        const std::function<std::unique_ptr<T>()> &vertexAttributeGenerationFunction
    );

    const GLenum _dataType;

};

#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEBASE_H
