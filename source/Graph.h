//
// Created by lire on 8/19/20.
//

#ifndef SDP_PIPELINERESOLUTION_GRAPH_H
#define SDP_PIPELINERESOLUTION_GRAPH_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <boost/multiprecision/cpp_int.hpp>

struct Node {
    int id;
    boost::multiprecision::cpp_int nodeWeight = 1;
    boost::multiprecision::cpp_int prefix;
    int start = INT32_MAX;
    int end = -1;
    std::vector<int> adj;
    int adjSize = 0;
    std::vector<int> ancestors;
    int ancSize = 0;
    int ancNumber = 0;
    bool root = true;

    bool operator<(const Node &n) const {
        return (nodeWeight < n.nodeWeight);
    }
};

class Graph {

public:
    int nNodes;
    int nEdges = 0;
    std::vector<Node> nodes;

    explicit Graph(FILE *fp);

    void build_addEdges(unsigned u, std::vector<unsigned> &adj, unsigned adj_size);

    void sortVectors();

    void build(FILE *fp);

    boost::multiprecision::cpp_int preorderVetSize = 0;

    void reBuild(FILE *fp);

    int preLeavesPos = 0;
    int rootsSize = 0;
    int rootsPos = 0;
    std::vector<int> preLeaves;
    std::vector<int> roots;
};

#endif //SDP_PIPELINERESOLUTION_GRAPH_H