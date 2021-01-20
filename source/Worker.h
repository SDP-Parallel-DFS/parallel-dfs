//
// Created by lire on 8/19/20.
//

#ifndef SDP_PIPELINERESOLUTION_WORKER_H
#define SDP_PIPELINERESOLUTION_WORKER_H

#include <queue>
#include <vector>

#include "Graph.h"
#include "FastSemaphore.h"


struct boostIntVectBoostVect {
    boost::multiprecision::uint1024_t father;
    std::vector<int> *adj;
    std::vector<boost::multiprecision::uint1024_t> *adjWeights;
};


class Worker {

    int id;

public:
    FastSemaphore *askManagerToEmpty = new FastSemaphore();
    FastSemaphore *managerHasEmptied;
    FastSemaphore *askManagerToFeed = new FastSemaphore(1);
    FastSemaphore *managerHasFed = new FastSemaphore();

    int graphSize;
    std::vector<boostIntVectBoostVect> results;
    Node *next;


    void initialize(int id, Graph *g, int nWorkers);

    void preGraphSize();

    void resetSemaphores();

    void weightsAndPrefixes();

    void startEndTimes();
};

#endif //SDP_PIPELINERESOLUTION_WORKER_H
