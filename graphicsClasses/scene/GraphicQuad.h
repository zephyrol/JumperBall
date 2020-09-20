#ifndef GRAPHICQUAD_H
#define GRAPHICQUAD_H
#include "geometry/Quad.h"

class GraphicQuad
{
public:
    GraphicQuad(const Quad & quad);
    void update();

private:
    const Quad& _quad;
};

#endif // GRAPHICQUAD_H
