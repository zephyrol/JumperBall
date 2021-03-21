#include "QuadState.h"

QuadState::QuadState(const Quad&) {
}

State::GlobalState QuadState::update() {
    return State::GlobalState::United;
}
