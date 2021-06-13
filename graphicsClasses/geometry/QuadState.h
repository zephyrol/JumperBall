#ifndef QUADSTATE_H
#define QUADSTATE_H
#include "geometry/Quad.h"
#include <scene/ObjectState.h>

class QuadState:public ObjectState {
public:
QuadState(const Quad& quad);
virtual ObjectState::GlobalState update() override;

private:
};

#endif // QUADSTATE_H
