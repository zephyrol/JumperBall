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

ParallelTask::ParallelTask (std::function<void()>&& taskFunction,
                            size_t numberOfTasks) :
    _endOfTasksIsRequested(false),
    _numberOfThreads(getNumberOfThreads(numberOfTasks)),
    _numberOfTasks(numberOfTasks),
    _treatedTasks(0),
    _mutexInProgress(),
    _mutexDone(),
    _taskFunction(taskFunction),
    _asyncTasks()
{
    _mutexInProgress.lock();
    for (size_t i = 0; i < _numberOfThreads; ++i) {
        _asyncTasks.push_back( std::async(std::launch::async, [this](){
            threadFunction(_taskFunction);}));
    }
}

void ParallelTask::runTasks()
{
    _mutexDone.lock();
    _treatedTasks = 0;
    _mutexInProgress.unlock();
}

void ParallelTask::waitTasks()
{
    _mutexDone.lock();
    _mutexDone.unlock();
}

void ParallelTask::threadFunction(const std::function<void()>& task) {

    while(!_endOfTasksIsRequested) {
        _mutexInProgress.lock();
        ++_treatedTasks;
        if (_treatedTasks < _numberOfTasks) {
            _mutexInProgress.unlock();
            task();
        } else {
           _mutexDone.unlock();
        }
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
    for ( size_t i = 0; i < _numberOfThreads; ++i) {
        _mutexInProgress.unlock();
    }
    for ( size_t i = 0; i < _numberOfThreads; ++i) {
        _asyncTasks.at(i).wait();
    }
}
