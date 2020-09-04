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
    _numberOfThreads(getNumberOfThreads(numberOfTasks)-1),
    _numberOfTasks(numberOfTasks),
    _mutexesStart(_numberOfThreads),
    _mutexesDone(_numberOfThreads),
    _taskFunction(taskFunction),
    _asyncTasks()
{
    for (size_t i = 0; i < _numberOfThreads; ++i) {
        std::cout << _numberOfThreads << " threads are created" << std::endl;
        _mutexesStart.at(i).lock();
        _asyncTasks.push_back( std::async(std::launch::async, [this,i](){
            threadFunction(_taskFunction,i,_numberOfThreads,_numberOfTasks);}));
    }
}

void ParallelTask::runTasks()
{
    for ( std::mutex& mutexDone : _mutexesDone ) {
        mutexDone.lock();
    }
    for ( std::mutex& mutexStart : _mutexesStart ) {
        mutexStart.unlock();
    }
}

void ParallelTask::waitTasks()
{
    int i = 0;
    for ( std::mutex& mutexDone : _mutexesDone ) {
        //std::cout << "Waiting for " << i << std::endl;
        mutexDone.lock();
        mutexDone.unlock();
        i++;
    }
}

void ParallelTask::threadFunction(const std::function<void(size_t)> &task,
                                  size_t threadnumber,
                                  size_t nbOfThreads,
                                  size_t nbOfTasks) {

    while(!_endOfTasksIsRequested) {
        _mutexesStart.at(threadnumber).lock();
        if (!_endOfTasksIsRequested) {
            //std::cout<< "I started : " << threadnumber << std::endl;
            for(size_t i = (threadnumber * nbOfTasks / nbOfThreads);
                i < ((threadnumber+1) * nbOfTasks / nbOfThreads); ++i){
                for (size_t j = 0 ; j < 10 ; j++)
                    task(i);
                //std::cout << "thread " << threadnumber << " does task " << i
                          //<< std::endl;
            }
            _mutexesDone.at(threadnumber).unlock();
            //std::cout<< "DONE : " << threadnumber << std::endl;
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
    for ( std::mutex& mutexStart : _mutexesStart ) {
        mutexStart.unlock();
    }
    for ( auto& asyncTask : _asyncTasks) {
        asyncTask.wait();
    }
}
