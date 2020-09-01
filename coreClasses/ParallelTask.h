/* 
 * File:   ParallelTask.h
 * Author: Morgenthaler S
 *
 * Created on 31 aout 2020, 22:44
 */

#ifndef PARALLEL_TASK_H 
#define PARALLEL_TASK_H  
#include "Types.h"
#include "ClassicalMechanics.h"
#include <future>

class ParallelTask {
    ParallelTask (std::function<void()>&& taskFunction,
                  size_t numberOfTasks );
    ~ParallelTask();

public:
    void runTasks();
    void waitTasks();

private:
    //--------ATTRIBUTES-----------//
    bool                    _endOfTasksIsRequested;
    size_t                  _numberOfThreads;
    size_t                  _numberOfTasks;
    size_t                  _treatedTasks;
    std::mutex              _mutexInProgress;
    std::mutex              _mutexDone;
    std::function<void()>   _taskFunction;
    std::vector<std::future<void> > _asyncTasks;

    //----------METHODS-------------//
    void                 threadFunction(const std::function<void()>& task);
    std::vector<std::future<void> >genAsyncTasks();

    //------ STATIC METHODS---------//
    static size_t getNumberOfThreads(size_t numberOfTasks);
};

#endif /* PARALLEL_TASK_H */
