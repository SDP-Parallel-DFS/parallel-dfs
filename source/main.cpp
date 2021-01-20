#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>

#include "EmptierManager.h"
#include "FeederManager.h"
#include "Graph.h"
#include "Worker.h"
#include "FastSemaphore.h"

using namespace std;

void preGraphSizeWorker(Worker *worker) {
    worker->preGraphSize();
}

void preGraphSizeEManager(emptierManager eManager) {
    eManager.preGraphSize();
}

void preGraphSizeFManager(feederManager fManager) {
    fManager.preGraphSize();
}

//weight+prefix
void startWorker(Worker *worker) {
    worker->weightsAndPrefixes();
}

void startEManager(emptierManager eManager) {
    eManager.weightsAndPrefixes();
}

void startFManager(feederManager fManager) {
    fManager.weightsAndPrefixes();
}

//father end
void startEndWorker(Worker *worker) {
    worker->startEndTimes();
}

void startEndEManager(emptierManager eManager) {
    eManager.startEndTimes();
}

void startEndFManager(feederManager fManager) {
    fManager.startEndTimes();
}

template<typename T>
vector<size_t> sort_indexes(const vector<T> &v) {

    // initialize original index locations
    vector<size_t> idx(v.size());
    iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values
    stable_sort(idx.begin(), idx.end(),
                [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

    return idx;
}

void start(int nWorkers, Graph *g) {


    vector<Worker> allWorkers(nWorkers);
    for (int i = 0; i < nWorkers; i++) {
        allWorkers.at(i).initialize(i, g, nWorkers);
    }
    FastSemaphore commonSemQueueFull(0);
    FastSemaphore commonSemQueueEmpty(g->nNodes);

    std::vector<intintint> commonQueue(g->nNodes);
    emptierManager eManager(&allWorkers, nWorkers, &commonSemQueueFull,
                            &commonSemQueueEmpty, &commonQueue, g);
    feederManager fManager(&allWorkers, nWorkers, &commonSemQueueFull,
                           &commonSemQueueEmpty, &commonQueue, g);


    //pre phase

    vector<thread> tPreGraphSizeWorkers(nWorkers);
    for (int i = 0; i < nWorkers; i++) {
        tPreGraphSizeWorkers[i] = thread(preGraphSizeWorker, &allWorkers.at(i));
    }
    thread tPreGraphSizeEManager(preGraphSizeEManager, eManager);
    thread tPreGraphSizeFManager(preGraphSizeFManager, fManager);

    for (int i = 0; i < nWorkers; i++) {
        tPreGraphSizeWorkers[i].join();
    }
    tPreGraphSizeEManager.join();
    tPreGraphSizeFManager.join();


    for (int i = 0; i < nWorkers; i++) {
        allWorkers.at(i).resetSemaphores();
    }
    commonSemQueueEmpty.reset();
    commonSemQueueFull.reset();


    //first phase


    vector<thread> tWorkers(nWorkers);
    for (int i = 0; i < nWorkers; i++) {
        tWorkers[i] = thread(startWorker, &allWorkers.at(i));
    }
    thread tEManager(startEManager, eManager);
    thread tFManager(startFManager, fManager);

    for (int i = 0; i < nWorkers; i++) {
        tWorkers[i].join();
    }
    tEManager.join();
    tFManager.join();


    for (int i = 0; i < nWorkers; i++) {
        allWorkers.at(i).resetSemaphores();
    }
    commonSemQueueEmpty.reset();
    commonSemQueueFull.reset();



    //second phase

    vector<size_t> tmp = sort_indexes(g->nodes);
    for (int i = 0; auto x : tmp) {
        g->nodes.at(static_cast<int> (x)).end = ++i;
    }


    //third phase

    vector<thread> seWorkers(nWorkers);
    for (int i = 0; i < nWorkers; i++) {
        seWorkers[i] = thread(startEndWorker, &allWorkers.at(i));
    }
    thread seEManager(startEndEManager, eManager);
    thread seFManager(startEndFManager, fManager);

    for (int i = 0; i < nWorkers; i++) {
        seWorkers[i].join();
    }
    seEManager.join();
    seFManager.join();
}


int main(int argc, const char *argv[]) {
    FILE *fp;

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " input_file output_file" << endl;
        return -1;
    }
    string graname(argv[1]);
    string outname(argv[2]);

    if ((fp = fopen(graname.c_str(), "r")) == NULL) {
        cout << "Error: File doesn't exist." << endl;
        return -1;
    }

    Graph g(fp);
    fclose(fp);

    g.sortVectors();

    start(2, &g);

    if ((fp = fopen(outname.c_str(), "w")) == NULL) {
        cout << "Error: File doesn't exist." << endl;
        return -1;
    }

    for(int i=0; i<g.nNodes; i++) {
        fprintf(fp, "%d %d %d\n", i, g.nodes.at(i).start, g.nodes.at(i).end);
    }

    fclose(fp);


    return 0;

}
