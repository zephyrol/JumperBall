#ifndef QUADSTATE_H
#define QUADSTATE_H
#include "geometry/Quad.h"
#include <scene/State.h>

class QuadState:public State {
public:
QuadState(const Quad& quad);
virtual State::GlobalState update() override;

private:
};

#endif // QUADSTATE_H
