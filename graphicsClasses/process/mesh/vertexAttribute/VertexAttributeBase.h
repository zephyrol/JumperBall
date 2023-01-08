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

    /**
     * Merge a vertex attribute geometry into the current one moving it.
     * @param other The other that will be moved into the current one.
     */
    // virtual void merge(VertexAttributeBase_uptr &&other) = 0;

    virtual void createDataOnGpu() const = 0;

    virtual size_t dataLength() const = 0;

    virtual ~VertexAttributeBase() = default;

    virtual void getMergeData(std::vector<void>&& other) = 0;

    /**
     * Use vertex attribute generation functions to gen them and filter the unused ones
     */
    static vecVertexAttributeBase_uptr genAndFilterVertexAttributes(
        const std::vector<std::function<VertexAttributeBase_uptr()> > &vertexAttributeGenerationFunction
    );

    static size_t getNumberOfVertices(const vecVertexAttributeBase_uptr& vertexAttributes);

    template<typename T1, typename T2>
    static void merge(const std::unique_ptr<T1>& vertexAttributeA , std::unique_ptr<T2>&& vertexAttributeB);

    template<typename T>
    static void merge(const std::unique_ptr<T>& vertexAttributeA , std::unique_ptr<T>&& vertexAttributeB);

private:

    const GLenum _dataType;

    /**
     * Binary operation function usually used in reduce function to filter the unused vertex attributes
     */
    static vecVertexAttributeBase_uptr filterUnusedAttributes(
        vecVertexAttributeBase_uptr &current,
        const std::function<VertexAttributeBase_uptr()> &vertexAttributeGenerationFunction
    );

};

template<typename T>
void VertexAttributeBase::merge(const std::unique_ptr<T> &vertexAttributeA,
                                std::unique_ptr<T> &&vertexAttributeB) {
    auto &dataA = vertexAttributeA->getDataRef();
    auto &dataB = vertexAttributeB->getDataRef();
    dataA.insert(
        dataA.end(),
        std::make_move_iterator(dataB.begin()),
        std::make_move_iterator(dataB.end())
    );
}

template<typename T1, typename T2>
void VertexAttributeBase::merge(const std::unique_ptr<T1> &,
                                std::unique_ptr<T2> &&) {
    std::cerr << "Error: Both vertex attribute don't contain the same attribute type" << std::endl;
}

#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEBASE_H
