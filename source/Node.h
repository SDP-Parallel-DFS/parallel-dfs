//
// Created by lire on 10/25/20.
//

#ifndef SDP_PARALLELSOLUTION_NODE_H
#define SDP_PARALLELSOLUTION_NODE_H

#include <boost/multiprecision/cpp_int.hpp>
#include "BusySemaphore.h"

struct Node {
    boost::multiprecision::cpp_int nodeWeight = 1;
    boost::multiprecision::cpp_int time;

    int start = INT32_MAX;
    int end = -1;


    std::vector<int> adj;
    int adjSize = 0;

    std::vector<int> ancestors;
    int ancSize = 0;

    int ancNumber = 0;

    bool operator<(const Node &n) const {
        return (nodeWeight < n.nodeWeight);
    }

    BusySemaphore *bSem = new BusySemaphore;

    ~Node() { delete bSem; };
};

#endif //SDP_PARALLELSOLUTION_NODE_H
