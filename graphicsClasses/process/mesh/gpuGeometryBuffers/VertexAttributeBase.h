//
// Created by S.Morgenthaler on 04/01/2023.
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
    VertexAttributeBase() = default;

    VertexAttributeBase(const VertexAttributeBase& vertexAttributeBase) = delete;

    VertexAttributeBase& operator=(const VertexAttributeBase& vertexAttributeBase) = delete;

    virtual void createDataOnGpu() const = 0;

    virtual size_t dataLength() const = 0;

    /**
     * Get a function that defines a vertex attribute data information on GPU.
     * The parameter of this output function is the index (vertex attribute number)
     */
    virtual std::function<void(GLuint)> getVertexAttribPointerFunc() const = 0;

    virtual ~VertexAttributeBase() = default;

    /**
     * Use vertex attribute generation functions to gen them and filter the unused ones
     */
    template <typename T>
    static std::vector<std::unique_ptr<T>> genAndFilter(
        const std::vector<std::function<std::unique_ptr<T>()>>& vertexAttributeGenerationFunctions);
};

template <typename T>
std::vector<std::unique_ptr<T>> VertexAttributeBase::genAndFilter(
    const std::vector<std::function<std::unique_ptr<T>()>>& vertexAttributeGenerationFunctions) {
    std::vector<std::unique_ptr<T>> usefulVertexAttributes{};
    for (const auto& vertexAttributeGenerationFunction : vertexAttributeGenerationFunctions) {
        auto vertexAttribute = vertexAttributeGenerationFunction();
        if (vertexAttribute != nullptr) {
            usefulVertexAttributes.emplace_back(std::move(vertexAttribute));
        }
    }
    return usefulVertexAttributes;
}

#endif  // JUMPERBALLAPPLICATION_VERTEXATTRIBUTEBASE_H
