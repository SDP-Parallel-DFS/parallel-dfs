#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>

#include "OPTIONS.h"

#include "Graph.h"
#include "Worker.h"

using namespace std;

void nodeSize(Worker *worker) {
    worker->nodeSize();
}

void nodeWeights(Worker *worker, bool works_on_roots) {
    worker->nodeWeights(works_on_roots);
}

void nodeTimes(Worker *worker) {
    worker->nodeTimes();
}


void setupTime(Graph *g) {
    boost::multiprecision::cpp_int max = 0;
    for(int i=0; i<g->rootsSize; i++) {
        max = max + g->nodes.at(g->roots.at(i)).nodeWeight;
    }
    for(int i=0; i<g->nNodes; i++) {
        g->nodes.at(i).time = max;
    }
}


template<typename T>
vector<size_t> sort_indexes(const vector<T> &v) {

    // initialize original index locations
    vector<size_t> idx(v.size()-1);
    iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values
    stable_sort(idx.begin(), idx.end(),
                [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

    return idx;
}


void reset(sharedData *sd) {
    sd->sq.reset();
    sd->nodeRead = 0;
    fill(sd->visitedNeighbours.begin(), sd->visitedNeighbours.end(), 0);
    sd->g->posIntoPreLeaves = 0;
}

void start(int nWorkers, Graph *g) {

    sharedData sd(g, nWorkers);

    vector<Worker> allWorkers(nWorkers);
    for (int i = 0; i < nWorkers; i++) {
        allWorkers.at(i).initialize(&sd);
    }



    //pre phase
    vector<thread> t_workers(nWorkers-1);

    for (int i = 0; i < nWorkers-1; i++) {
        t_workers[i] = thread(nodeSize, &allWorkers.at(i));
    }
    nodeSize(&allWorkers.at(nWorkers-1));

    for (int i = 0; i < nWorkers-1; i++) {
        t_workers[i].join();
    }

    reset(&sd);

    //first phase
    setupTime(g);

    t_workers[0] = thread(nodeWeights, &allWorkers.at(0), true);
    for (int i = 1; i < nWorkers-1; i++) {
        t_workers[i] = thread(nodeWeights, &allWorkers.at(i), false);
    }
    nodeWeights(&allWorkers.at(nWorkers-1), false);

    for (int i = 0; i < nWorkers-1; i++) {
        t_workers[i].join();
    }

    reset(&sd);



    //second phase
    vector<size_t> tmp = sort_indexes(g->nodes);
    for (int i=0; i<tmp.size(); i++) {
        g->nodes.at(static_cast<int> (tmp.at(i))).end = i+1;
    }

    //third phase
    for (int i = 0; i < nWorkers-1; i++) {
        t_workers[i] = thread(nodeTimes, &allWorkers.at(i));
    }
    nodeTimes(&allWorkers.at(nWorkers-1));

    for (int i = 0; i < nWorkers-1; i++) {
        t_workers[i].join();
    }
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

    start(N_THREADS, &g);

    if ((fp = fopen(outname.c_str(), "w")) == NULL) {
        cout << "Error: File doesn't exist." << endl;
        return -1;
    }

    for (int i = 0; i < g.nNodes; i++) {
        fprintf(fp, "%d %d %d\n", i, g.nodes.at(i).start, g.nodes.at(i).end);
    }

    fclose(fp);

    return 0;

}
