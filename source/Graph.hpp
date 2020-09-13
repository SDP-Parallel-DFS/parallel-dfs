#ifndef Graph_hpp
#define Graph_hpp


#include "SafeQueue.hpp"
#include "ThreadWorker.hpp"
#include "Semaphore.hpp"
#include "FastSemaphore.hpp"
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

#define GRAPH_DEBUG 1

using namespace std;


struct Node
{
        unsigned int id,
                inc_count = 0,
                inc_visited_count = 0,
                adj_visited_count = 0,
                prefix_subgraph_size = 0,
                subgraph_size = 1,
                pre = 0,
                post = 0;

        int parent = -1;
        vector<unsigned int > adj;
        vector<unsigned int > dt_adj;
        //unordered_map<unsigned int, bool> inc_visited, adj_visited;
        /*vector<unsigned int> inc;
          vector<bool> inc_visited;
        */
        bool subgraph_size_parent_visited = false;
        bool no_path = true;
        bool visited = false;
        vector<unsigned int> path;
    
};

class Graph
{
        unsigned int nNodes, pre, post;
        bool init_tw_done = false;
        Node main_parent;
        vector<mutex> muxes;
        vector<Node> nodes;
        vector<ThreadWorker> parent_workers;
        vector<ThreadWorker> child_workers;
        vector<FastSemaphore> worker_semaphores;
//        vector<Semaphore> worker_semaphores;
        unordered_set<unsigned int> roots;
        vector < unsigned int > roots_sorted;
        unordered_set<unsigned int> leafs;
        SafeQueue<unsigned int> P, C;
        SafeQueue<std::pair<unsigned int, unsigned int>>processChildQ;
        void init();
        void build(FILE * fp);
        void build_addEdges(unsigned int u, vector<unsigned int>& adj, unsigned int adj_size);
        void buildDT_processParent(const unsigned int p, unsigned int worker_id);
        void buildDT_processChild(unsigned int i, unsigned int p);
        void subDTSize_computePrefixSum(unsigned int p);
        void subDTSize_processChild(unsigned int i, unsigned int worker_id);
        void subDTSize_processParent(unsigned int p, unsigned int i);
        void computePrePost_processParent(const unsigned int p, unsigned int depth, unsigned int worker_id);
        void computePrePost_processChild(unsigned int child, unsigned int pre, unsigned int post);
        void sequentialDFS_r(unsigned int p);
        unsigned int hash(unsigned int x);
        void initThreadWorkers();
public:
        Graph(FILE * fp);
        Graph(unsigned int nodes);
        void addEdge(unsigned int u, unsigned int v);
        void printGraph();
        void printNodesStatus();
        void sortVectors();
        void sequentialDFS();
        void buildDT();
        void computeSubDTSize();
        void computePrePostOrder();
};


#endif /* Graph_hpp */

