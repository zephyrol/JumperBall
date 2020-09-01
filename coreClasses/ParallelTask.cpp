/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParallelTask.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 31 aout 2020, 19:30
 */

#include "ParallelTask.h"
    #include <unistd.h>

ParallelTask::ParallelTask (std::function<void(size_t)> &&taskFunction,
                            size_t numberOfTasks) :
    _endOfTasksIsRequested(false),
    _numberOfThreads(getNumberOfThreads(numberOfTasks)),
    _numberOfTasks(numberOfTasks),
    _mutexInProgress(),
    _mutexTaskCounter(),
    _mutexDone(),
    _taskFunction(taskFunction),
    _asyncTasks(),
    _finishedTasksThreads(_numberOfThreads,false)
{
    _mutexInProgress.lock();
    for (size_t i = 0; i < _numberOfThreads; ++i) {
        std::cout << _numberOfThreads << " threads are created" << std::endl;
        _asyncTasks.push_back( std::async(std::launch::async, [this,i](){
            threadFunction(_taskFunction,i,_numberOfThreads,_numberOfTasks);}));
    }
}

void ParallelTask::runTasks()
{
    for(size_t i = 0; i < _finishedTasksThreads.size(); ++i){
        _finishedTasksThreads.at(i) = false;
        _mutexInProgress.unlock();
    }
}

void ParallelTask::waitTasks()
{
    bool allfinished;
    do {
        _mutexDone.lock();
        allfinished = true;
        for ( bool finishedTaskThread : _finishedTasksThreads) {
            if (finishedTaskThread == false) {
                allfinished = false;
            }
        }
    } while (allfinished == false);

}

void ParallelTask::threadFunction(const std::function<void(size_t)> &task,
                                  size_t threadnumber,
                                  size_t nbOfThreads,
                                  size_t nbOfTasks) {

    while(!_endOfTasksIsRequested) {
        _mutexInProgress.lock();
        for(size_t i = threadnumber * nbOfTasks / nbOfThreads ;
            i < (threadnumber+1) * nbOfTasks / nbOfThreads ; ++i){
            std::cout << "thread " << threadnumber << " does task " << i
                      << std::endl;
            task(i);
        }
        _finishedTasksThreads.at(threadnumber) = true;
        _mutexDone.unlock();
    }
}


size_t ParallelTask::getNumberOfThreads(size_t numberOfTasks)
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

ParallelTask::~ParallelTask() {
    _endOfTasksIsRequested = true;
    _mutexInProgress.unlock();
    for ( size_t i = 0; i < _numberOfThreads; ++i) {
        _asyncTasks.at(i).wait();
    }
}
