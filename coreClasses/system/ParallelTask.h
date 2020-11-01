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

template<typename T>
class ParallelTask {
public:
    ParallelTask (std::function<T(size_t)>&& taskFunction,
                  size_t numberOfTasks  = 1, bool forceAsync = false);
    ParallelTask (const ParallelTask& parallelTask)                    = delete;
    ParallelTask& operator=(const ParallelTask& parallelTask)          = delete;
    ParallelTask (ParallelTask&& parallelTask)                         = delete;
    ParallelTask& operator=(ParallelTask&& parallelTask)               = delete;
    ~ParallelTask()                                                   = default;

    void runTasks();
    const std::shared_ptr<std::vector<T> >& waitTasks();

private:
    //--------ATTRIBUTES-----------//
    bool                             _forceAsync;
    size_t                           _numberOfThreads;
    size_t                           _numberOfTasks;
    std::function<T(size_t)>         _taskFunction;
    std::vector<std::future<void> >  _asyncTasks;
    std::shared_ptr<std::vector<T> > _returnValues;

    //----------METHODS-------------//
    void                 threadFunction(const std::function<T(size_t)>& task,
                                        size_t threadnumber,
                                        size_t nbOfThreads,
                                        size_t nbOfTasks);
    void                 runTask(const std::function<T(size_t)>& task,
                                 size_t taskNumber);
    std::shared_ptr<std::vector<T> > allocateReturnValues();

    //------ STATIC METHODS---------//
    static size_t        getNumberOfThreads(size_t numberOfTasks);
};

template<typename T>
ParallelTask<T>::ParallelTask(std::function<T(size_t)>&& taskFunction,
    size_t numberOfTasks,
    bool forceAsync) :
    _forceAsync(forceAsync),
    _numberOfThreads(getNumberOfThreads(numberOfTasks)),
    _numberOfTasks(numberOfTasks),
    _taskFunction(taskFunction),
    _asyncTasks(),
    _returnValues(allocateReturnValues())
{
}

template<typename T>
void ParallelTask<T>::runTasks()
{
    _asyncTasks.clear();
    for (size_t i = 0; i < _numberOfThreads; ++i) {
        if (_forceAsync) {
            _asyncTasks.push_back( std::async(std::launch::async,[this,i](){
                threadFunction(_taskFunction,i,_numberOfThreads,_numberOfTasks);}));
        } else {
            _asyncTasks.push_back( std::async([this,i](){
                threadFunction(_taskFunction,i,_numberOfThreads,_numberOfTasks);}));
        }
    }
}

template<typename T>
const std::shared_ptr<std::vector<T> >& ParallelTask<T>::waitTasks()
{
    for ( const std::future<void>& asyncTask : _asyncTasks ) {
        asyncTask.wait();
    }
    return _returnValues;
}

template<typename T>
void ParallelTask<T>::threadFunction(const std::function<T(size_t)> &task,
                                     size_t threadnumber,
                                     size_t nbOfThreads,
                                     size_t nbOfTasks) {

    for (size_t i = (threadnumber * nbOfTasks / nbOfThreads);
         i < ((threadnumber + 1) * nbOfTasks / nbOfThreads); ++i)
    {
        runTask(task, i);
    }
}

template<typename T>
void ParallelTask<T>::runTask(
        const std::function<T (size_t)> &task, size_t taskNumber)
{
    _returnValues->at(taskNumber) = task(taskNumber);
}

template<>
void ParallelTask<void>::runTask(const std::function<void (size_t)>& task,
                                 size_t taskNumber);


template<typename T>
std::shared_ptr<std::vector<T> > ParallelTask<T>::allocateReturnValues()
{
    return std::make_shared< std::vector<T> >(_numberOfTasks);
}

template<>
std::shared_ptr<std::vector<void> > ParallelTask<void>::allocateReturnValues();

template<typename T>
size_t ParallelTask<T>::getNumberOfThreads(size_t numberOfTasks)
{
    size_t numberOfThreads;
    const size_t numberOfThreadsMax = std::thread::hardware_concurrency();
    if (numberOfThreadsMax == 0) {
        numberOfThreads = 1;
    } else if (numberOfTasks < numberOfThreadsMax) {
        numberOfThreads = numberOfTasks;
    } else {
        numberOfThreads = numberOfThreadsMax;
    }
    return numberOfThreads;
}

#endif /* PARALLEL_TASK_H */
