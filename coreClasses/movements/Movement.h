//
//  Movement.h
//  JumperBallCore
//
//  Created by Sebastien Morgenthaler on 29/07/2020.
//

#ifndef Movement_h
#define Movement_h
#include <functional>
#include "Map.h"

template<typename T, size_t N>
class Movement {
public:
    
    Movement(const std::function<T(const std::array<JBTypes::Dir,N>)>&& fct);
    
    Movement(const Movement& movement) = delete;
    Movement& operator = (const Movement& movement) = delete;
    
    Movement(Movement&& movement) = delete;
    Movement& operator = (Movement&& movement) = delete;
    
    virtual ~Movement() = 0;
    
    const std::function<T(JBTypes::Dir)> _fct;
    T evaluate(const std::array<JBTypes::Dir,N>& inputs);
};

template<typename T, size_t N>
Movement<T,N>::Movement(const std::function<T
    (const std::array<JBTypes::Dir,N>)>&& fct):
_fct(fct)
{ }

template<typename T, size_t N>
T Movement<T,N>::evaluate(const std::array<JBTypes::Dir,N>& inputs) {
    return _fct(inputs);
}
#endif /* Movement_h */
