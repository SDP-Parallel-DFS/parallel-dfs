//
// Created by lire on 8/19/20.
//

#include "Graph.h"

using namespace std;

Graph::Graph(FILE *fp) {
    fscanf(fp, "%d\n", &nNodes);
    nodes.resize(nNodes);
    preLeaves.resize(nNodes);
    rootsSize = nNodes;
    for (int i = 0; i < nNodes; i++) {
        nodes[i].id = i;
        nodes[i].adj = new vector<int>;
    }
    this->build(fp);
    preLeaves.resize(preLeavesPos);
    roots.resize(rootsSize);

    for (int i = 0; i < nNodes; i++) {
        nodes[i].ancestors = new vector<int>(nodes[i].ancSize);
        if (nodes[i].root) {
            roots.at(rootsPos++) = i;
        }
    }
    this->reBuild(fp);
}

void Graph::sortVectors() {
    for (int v = 0; v < nNodes; ++v) {
        sort(nodes[v].adj->begin(), nodes[v].adj->end(), std::less<int>());
    }
}

void Graph::build_addEdges(unsigned u, vector<unsigned> &adj, unsigned adj_size) {
    nEdges += adj_size;
    if (adj_size > 0) {
        if (nodes[u].adjSize == 0) {
            nodes[u].adj->resize(adj_size);
            for (int i = 0; i < adj_size; i++) {
                nodes[u].adj->at(i) = adj[i];
                if (nodes.at(adj[i]).root) {
                    nodes.at(adj[i]).root = false;
                    rootsSize--;
                }
                nodes[adj[i]].ancSize++;
            }
            nodes[u].adjSize = nodes[u].adj->size();
        } else {
            printf("REALLY??\n");
            nodes[u].adj->insert(nodes[u].adj->end(), &adj[0], &adj[adj_size]);
            nodes[u].adjSize += adj_size;
            for (int i = 0; i < adj_size; i++) {
                nodes[adj[i]].root = false;
                nodes[adj[i]].ancSize++;
            }
        }
    }
    else {
        preLeaves.at(preLeavesPos++) = u;
    }
}

void Graph::reBuild(FILE *fp) {

    for (int i = 0; i < nNodes; i++) {
        for (int j = 0; j < nodes[i].adjSize; j++) {
            nodes[nodes[i].adj->at(j)].ancestors->at(nodes[nodes[i].adj->at(j)].ancNumber++) = i;
        }
    }

}

void Graph::build(FILE *fp) {
    unsigned u, v;
    unsigned max_line_size = (log10(nNodes) + 2) * (nNodes + 1) + 3;
    char str[max_line_size];
    char dontcare[3];
    vector<unsigned> buf = vector<unsigned>(nNodes + 1);

    while (fscanf(fp, "%[^#]s", str) != EOF) {
        fscanf(fp, "%s\n", dontcare);
        char *token;
        unsigned i = 0;

        /* get the first token */
        token = strtok(str, " ");
        sscanf(token, "%d", &u);
        token = strtok(NULL, " ");

        /* walk through other tokens */
        while (token != NULL) {
            sscanf(token, "%d", &v);
            buf[i++] = v;

            token = strtok(NULL, " ");
        }

        this->build_addEdges(u, buf, i);

    }

}