#include "QuadState.h"

QuadState::QuadState(const Quad&) {
}

ObjectState::GlobalState QuadState::update() {
    return ObjectState::GlobalState::United;
}
