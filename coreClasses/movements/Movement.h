//
// Movement.h
// JumperBallCore
//
// Created by Sebastien Morgenthaler on 29/07/2020.
//

#ifndef Movement_h
#define Movement_h
#include <system/Types.h>

template<typename T, size_t N> class Movement {

public:
explicit Movement(const std::function <T(const std::array <JBTypes::Dir, N>&)>&& fct);

Movement(const Movement& movement) = delete;
Movement& operator= (const Movement& movement) = delete;

Movement(Movement&& movement) = delete;
Movement& operator= (Movement&& movement) = delete;

virtual ~Movement() = default;

T evaluate(const std::array <JBTypes::Dir, N>& inputs) const;


private:
const std::function <T(const std::array <JBTypes::Dir, N>&)> _fct;
};

template<typename T, size_t N> Movement <T, N>::Movement(
    const std::function <T(const std::array <JBTypes::Dir, N>&)>&& fct
    ):
    _fct(fct) {
}

template<typename T, size_t N> T Movement <T, N>::evaluate (
    const std::array <JBTypes::Dir, N>& inputs
    ) const {
    return _fct(inputs);
}
#endif /* Movement_h */
