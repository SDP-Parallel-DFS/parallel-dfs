//
// Created by lire on 10/25/20.
//

#ifndef SDP_PARALLELSOLUTION_NODE_H
#define SDP_PARALLELSOLUTION_NODE_H

#include <vector>

struct Node {

    int start = INT32_MAX;
    int end = -1;


    std::vector<int> adj;
    int adjSize = 0;

    bool root = true;

    Node() {
        nextNode.resize(N_THREADS+1, -1);
        visitato.resize(N_THREADS+1, false);
        visitatoInQuestoLivello.resize(N_THREADS+1, -1);
        pronto.resize(N_THREADS+1, false);
    }

    std::vector<int> nextNode;
    std::vector<char> pronto;
    std::vector<char> visitato;
    std::vector<int> visitatoInQuestoLivello;
};

#endif //SDP_PARALLELSOLUTION_NODE_H
