//
//  Graph.hpp
//  ParallelDFS
//
//  Created by Enrico on 09/08/2020.
//  Copyright © 2020 Enrico. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <unordered_set>
#include <sstream>

#define GRAPH_DEBUG 0

using namespace std;

struct Node
{
    unsigned int id;
    unsigned int pre = 0,
                 post = 0,
                 s = numeric_limits<unsigned int>::max(),
                 e = 0;
    int parent = -1;
    vector<unsigned int> adj;
    bool visited;
};

class Graph
{
    unsigned int pre, post, nNodes;
    vector<Node> nodes;
    unordered_set<unsigned int> roots;
    void init();
    void build(FILE *fp);
    void build_addEdges(unsigned int u, vector<unsigned int> &adj, unsigned int adj_size);

    void sequentialDFS_r(unsigned int p);

public:
    Graph(FILE *fp);
    Graph(ifstream &input_stream);
    Graph(unsigned int nodes);
    void addEdge(unsigned int u, unsigned int v);
    void printGraph();
    void sortVectors();
    void printNodesStatus();
    void sequentialDFS();
};

#endif /* Graph_hpp */
