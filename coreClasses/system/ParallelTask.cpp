
/* 
 * File:   ParallelTask.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 31 aout 2020, 19:30
 */

#include "ParallelTask.h"

template<>
void ParallelTask<void>::runTask(
        const std::function<void (size_t)> & task, size_t taskNumber)
{
    task(taskNumber);
}

template<>
std::shared_ptr<std::vector<void> > ParallelTask<void>::allocateReturnValues()
{
    return nullptr;
}
