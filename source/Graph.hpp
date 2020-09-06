#ifndef Graph_hpp
#define Graph_hpp


#include "SafeQueue.hpp"
#include "ThreadWorker.hpp"
#include "Semaphore.hpp"
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <future>
#include <vector>
#include <future>
#include <algorithm>
#include <chrono>
#include <random>
#include <mutex>

#define GRAPH_DEBUG 0

using namespace std;


struct Node
{
    unsigned id,
    inc_count = 0,
    inc_visited_count = 0,
    adj_visited_count = 0,
    subgraph_size = 1,
    pre = 0,
    post = 0;

    int parent = -1;
    vector<unsigned > adj;
    vector<unsigned > dt_adj;
    //unordered_map<unsigned, bool> inc_visited, adj_visited;
    /*vector<unsigned> inc;
    vector<bool> inc_visited;
     */
    bool subgraph_size_parent_visited = false;
    bool no_path = true;
    bool visited = false;
    vector<unsigned> path;
    
};

class Graph
{
    unsigned nNodes, pre, post;
    bool init_tw_done = false;
    vector<mutex> muxes;
    vector<Node> nodes;
    vector<ThreadWorker> parent_workers;
    vector<ThreadWorker> child_workers;
    vector<Semaphore> worker_semaphores;
    unordered_set<unsigned> roots;
    unordered_set<unsigned> leafs;
    SafeQueue<unsigned> P, C;
    SafeQueue<std::pair<unsigned, unsigned>>processChildQ;
    void init();
    void build(FILE * fp);
    void build_addEdges(unsigned u, vector<unsigned>& adj, unsigned adj_size);
    void buildDT_processParent(const unsigned p, unsigned worker_id);
    void buildDT_processChild(unsigned i, unsigned p);
    void subGraphSize_computePrefixSum(unsigned p);
    void subGraphSize_processChild(unsigned i, unsigned worker_id);
    void subGraphSize_processParent(unsigned p, unsigned i);
    void sequentialDFS_r(unsigned p);
    unsigned int hash(unsigned int x);
    void initThreadWorkers();
public:
    Graph(FILE * fp);
    Graph(unsigned nodes);
    void addEdge(unsigned u, unsigned v);
    void printGraph();
    void printNodesStatus();
    void sortVectors();
    void sequentialDFS();
    void buildDT();
    void computeSubGraphSize();
    void computePrePostOrder();
};


#endif /* Graph_hpp */

