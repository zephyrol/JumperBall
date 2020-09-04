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
public:
    ParallelTask (std::function<void(size_t)>&& taskFunction,
                  size_t numberOfTasks  = 1);
    ~ParallelTask();

    void runTasks();
    void waitTasks();

private:
    //--------ATTRIBUTES-----------//
    bool                    _endOfTasksIsRequested;
    size_t                  _numberOfThreads;
    size_t                  _numberOfTasks;
    std::vector<std::mutex> _mutexesStart;
    std::vector<std::mutex> _mutexesDone;
    std::function<void(size_t)>   _taskFunction;
    std::vector<std::future<void> > _asyncTasks;

    //----------METHODS-------------//
    void                 threadFunction(const std::function<void(size_t)>& task,
                                        size_t threadnumber,
                                        size_t nbOfThreads,
                                        size_t nbOfTasks);
    std::vector<std::future<void> >genAsyncTasks();

    //------ STATIC METHODS---------//
    static size_t getNumberOfThreads(size_t numberOfTasks);
};

#endif /* PARALLEL_TASK_H */
