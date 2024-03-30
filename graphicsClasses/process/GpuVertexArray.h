//
// Created by S.Morgenthaler on 30/03/2024.
//

#ifndef GPUVERTEXARRAY_H
#define GPUVERTEXARRAY_H

#include "Utility.h"


class GpuVertexArray;
using CstGpuVertexArray_uptr = std::unique_ptr<const GpuVertexArray>;

class GpuVertexArray {
public:
    GpuVertexArray();

    /**
     * Copy constructor and assignment are deleted because a OpenGL vertex array id is unique.
     */
    GpuVertexArray(const GpuVertexArray &gpuVertexArray) = delete;
    GpuVertexArray &operator=(const GpuVertexArray &gpuVertexArray) = delete;

    /**
     * Move constructor and assignment are deleted because the destructor free the GPU memory.
     */
    GpuVertexArray(GpuVertexArray &&gpuVertexArray) = delete;
    GpuVertexArray &operator=(GpuVertexArray &&gpuVertexArray) = delete;

    /**
     * Bind a vertex array from its id.
     */
    void bind() const;

    ~GpuVertexArray();

private:
    const GLuint _vertexArrayId;
};



#endif //GPUVERTEXARRAY_H
