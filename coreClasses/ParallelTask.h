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

template<typename T>
class ParallelTask {
public:
    ParallelTask (std::function<T(size_t)>&& taskFunction,
                  size_t numberOfTasks  = 1);
    ParallelTask (const ParallelTask& parallelTask)                    = delete;
    ParallelTask& operator=(const ParallelTask& parallelTask)          = delete;
    ParallelTask (ParallelTask&& parallelTask)                         = delete;
    ParallelTask& operator=(ParallelTask&& parallelTask)               = delete;
    ~ParallelTask();

    void runTasks();
    const std::shared_ptr<std::vector<T> >& waitTasks();

private:
    //--------ATTRIBUTES-----------//
    bool                             _tasksAreInitialized;
    bool                             _endOfTasksIsRequested;
    size_t                           _numberOfThreads;
    size_t                           _numberOfTasks;
    std::vector<std::mutex>          _mutexesStart;
    std::vector<std::mutex>          _mutexesRunningTasks;
    std::vector<std::mutex>          _mutexesWaitingTasks;
    std::vector<std::mutex>          _mutexesDone;
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
    void                 initTasks();
    std::shared_ptr<std::vector<T> > allocateReturnValues();

    //------ STATIC METHODS---------//
    static size_t        getNumberOfThreads(size_t numberOfTasks);
};

template<typename T>
ParallelTask<T>::ParallelTask(std::function<T(size_t)>&& taskFunction,
    size_t numberOfTasks) :
    _tasksAreInitialized(false),
    _endOfTasksIsRequested(false),
    _numberOfThreads(getNumberOfThreads(numberOfTasks)),
    _numberOfTasks(numberOfTasks),
    _mutexesStart(_numberOfThreads),
    _mutexesRunningTasks(_numberOfThreads),
    _mutexesWaitingTasks(_numberOfThreads),
    _mutexesDone(_numberOfThreads),
    _taskFunction(taskFunction),
    _asyncTasks(),
    _returnValues(allocateReturnValues())
{
}

template<typename T>
void ParallelTask<T>::runTasks()
{
    if (!_tasksAreInitialized) {
        initTasks();
        _tasksAreInitialized = true;
    }

    for ( std::mutex& mutexRunningTasks : _mutexesRunningTasks) {
        mutexRunningTasks.unlock();
    }

    for ( std::mutex& mutexStart : _mutexesStart) {
        mutexStart.lock();
        mutexStart.unlock();
    }
}

template<typename T>
const std::shared_ptr<std::vector<T> >& ParallelTask<T>::waitTasks()
{

    for (size_t i = 0; i < _numberOfThreads; ++i) {
        _mutexesRunningTasks.at(i).lock();
        _mutexesWaitingTasks.at(i).unlock();
    }

    for (size_t i = 0; i < _numberOfThreads; ++i) {
        _mutexesDone.at(i).lock();
        _mutexesWaitingTasks.at(i).lock();
        _mutexesDone.at(i).unlock();
    }

    for ( std::mutex& mutexDone : _mutexesDone ) {
        mutexDone.lock();
        mutexDone.unlock();
    }

    for ( std::mutex& mutexDone : _mutexesDone ) {
        mutexDone.lock();
        mutexDone.unlock();
    }
    return _returnValues;
}

template<typename T>
void ParallelTask<T>::threadFunction(const std::function<T(size_t)> &task,
                                     size_t threadnumber,
                                     size_t nbOfThreads,
                                     size_t nbOfTasks) {

    _mutexesStart.at(threadnumber).lock();
    while(!_endOfTasksIsRequested) {
	    _mutexesRunningTasks.at(threadnumber).lock();
        if (!_endOfTasksIsRequested) {
			_mutexesDone.at(threadnumber).lock();
			_mutexesStart.at(threadnumber).unlock();
            for(size_t i = (threadnumber * nbOfTasks / nbOfThreads);
                i < ((threadnumber+1) * nbOfTasks / nbOfThreads); ++i){
                runTask(task,i);
            }
            _mutexesRunningTasks.at(threadnumber).unlock();
            _mutexesWaitingTasks.at(threadnumber).lock();
            _mutexesWaitingTasks.at(threadnumber).unlock();
            _mutexesStart.at(threadnumber).lock();
            _mutexesDone.at(threadnumber).unlock();
        }
    }
}

template<typename T>
void ParallelTask<T>::runTask(
        const std::function<T (size_t)> &task, size_t taskNumber)
{
    _returnValues->at(taskNumber) = task(taskNumber);
}

template<typename T>
inline void ParallelTask<T>::initTasks()
{
    for (size_t i = 0; i < _numberOfThreads; ++i) {
        _mutexesWaitingTasks.at(i).lock();
        _mutexesRunningTasks.at(i).lock();
        _asyncTasks.push_back( std::async(std::launch::async, [this,i](){
            threadFunction(_taskFunction,i,_numberOfThreads,_numberOfTasks);}));
    }
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

template<typename T>
ParallelTask<T>::~ParallelTask() {
    _endOfTasksIsRequested = true;
    for ( std::mutex& mutexRunningTasks: _mutexesRunningTasks ) {
        std::unique_lock<std::mutex> lock(mutexRunningTasks, std::adopt_lock);
        lock.unlock();
    }
    for ( auto& asyncTask : _asyncTasks) {
        asyncTask.wait();
    }
}

#endif /* PARALLEL_TASK_H */
