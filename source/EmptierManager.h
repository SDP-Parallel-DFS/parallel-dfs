//
// Created by lire on 8/19/20.
//

#ifndef SDP_PIPELINERESOLUTION_EMPTIERMANAGER_H
#define SDP_PIPELINERESOLUTION_EMPTIERMANAGER_H

#include <queue>
#include <vector>

#include "FeederManager.h"
#include "Worker.h"

class emptierManager {
    std::vector<Worker> *workers;
    Semaphore *commonSemQueueFull;
    Semaphore *commonSemQueueEmpty;
    std::vector<struct intintint> *commonQueue;
    int nWorkers;
    int graphSize;
    Graph *g;
    std::vector<int> *roots;
    std::vector<int> *preLeaves;


public:
    emptierManager(std::vector<Worker> *allWorkers, int nWorkers,
                   Semaphore *commonSemQueueFull, Semaphore *commonSemQueueEmpty,
                   std::vector<intintint> *commonQueue,/* Node *separator,*/
                   Graph *g); //commonQueue must have size equal to graphSize


    void weightsAndPrefixes();
    void startEndTimes();
    void preGraphSize();
};

#endif //SDP_PIPELINERESOLUTION_EMPTIERMANAGER_H