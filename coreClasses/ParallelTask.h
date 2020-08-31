/* 
 * File:   ParallelTask.h
 * Author: Morgenthaler S
 *
 * Created on 31 aout 2020, 22:44
 */

#ifndef PARALLEL_TASK_H 
#define PARALLEL_TASK_H  
#include "Types.h"
#include <future>

class ParallelTask {
    ParallelTask (const std::function<void()> task);
    ~ParallelTask();
    //--------ATTRIBUTES-----------//
    bool                    _endOfTasksIsRequested;
    size_t                  _treatedComponents;
    std::mutex              _mutexComponentsInProgress;
    std::mutex              _mutexComponentsDone;
    std::vector<std::future<void> > _asyncTasksComponent;

    //----------METHODS-------------//
    void                    treatComponent();
    std::vector<std::future<void> >genAsyncTasks();
};

#endif /* PARALLEL_TASK_H */