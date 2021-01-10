//
// Created by lire on 8/19/20.
//

#include "Graph.h"

using namespace std;

Graph::Graph(FILE *fp) {

    fscanf(fp, "%d\n", &nNodes);
    startingNode = nNodes;
    nodes.resize(nNodes+1);

#if PRINT_MAT
    int size = nNodes*2;
    mat.resize(size);
    for(int i=0; i<size; i++) {
        mat.at(i).resize(nNodes, -5);
    }
#endif

    rootsSize = nNodes;

#if READ_TYPE == 0
    this->build(fp);
#elif READ_TYPE == 1
    this->build2(fp);
#elif READ_TYPE == 2
    this->build3(fp);
#else
    printf("Read option not supported");
    exit(-1);
#endif

    roots.resize(rootsSize);

    for (int i = 0; i < nNodes; i++) {

        if (nodes[i].root) {
            roots.at(rootsPos++) = i;
        }

    }

}

void Graph::printTrueLabelsPreWeights() {
    for (int v = 0; v < nNodes; ++v) {
        cout << "\n Adjacency list of vertex " << v << "\n head ";
        printf("\nStart-end time: %d -> %d\n", nodes[v].start, nodes[v].end);
    }
}

void Graph::sortVectors() {
    for (int v = 0; v < nNodes; ++v) {
        sort(nodes[v].adj.begin(), nodes[v].adj.end(), std::less<int>());
    }
}

void Graph::build_addEdges(unsigned u, vector<unsigned> &adj, unsigned adj_size) {
    if (adj_size > 0) {
        nodes[u].adj.resize(adj_size);
        for (int i = 0; i < adj_size; i++) {
            nodes[u].adj.at(i) = adj[i];
            if (nodes.at(adj[i]).root) {
                nodes.at(adj[i]).root = false;
                rootsSize--;
            }
        }
        nodes[u].adjSize = nodes[u].adj.size();
    }
}

#if GRAPH_DOUBLE_READ
void Graph::addAncestor(unsigned int u, std::vector<unsigned int> &adj, unsigned int adj_size) {
    for (int i = 0; i < adj_size; i++) {
        nodes[adj[i]].ancestors->at(nodes[adj[i]].ancNumber++) = u;
    }
}

void Graph::reBuild(FILE * fp) {
    unsigned u, v;
    unsigned max_line_size = (log10(nNodes) + 2) * (nNodes + 1) + 3;
    char str[max_line_size];
    char dontcare[3];
    vector<unsigned> buf = vector<unsigned> (nNodes + 1);

    while(fscanf(fp, "%[^#]s", str) != EOF) {
        fscanf(fp, "%s", dontcare);
        char *token;
        unsigned i = 0;

        /* get the first token */
        token = strtok(str, " ");
#if GRAPH_DEBUG
        printf( " %s\n", token );
#endif
        sscanf(token, "%d", &u);
        token = strtok(NULL, " ");
        /* walk through other tokens */
        while(token != NULL){

            sscanf(token, "%d", &v);
            buf[i++] = v;

            token = strtok(NULL, " ");
        }

        this->addAncestor(u, buf, i);

    }

}
#endif

#if READ_TYPE == 0
void Graph::build(FILE *fp) {
    unsigned u;
    unsigned max_line_size = (log10(nNodes) + 2) * (nNodes + 1) + 3;
    char str[max_line_size];
    char dontcare[3];
    vector<unsigned> buf = vector<unsigned>(nNodes + 1);

    while (fscanf(fp, "%[^#]s", str) != EOF) {
        fscanf(fp, "%s\n", dontcare);
        unsigned i = 0, j = 0;
        u = 0;
        while(str[i] != ':') {
            u = u * 10 + (str[i++] & 0x0f);
        }
        i = i+2;
        while (str[i] != '\0') {
            buf.at(j) = 0;
            while(str[i] != ' ') {
                buf.at(j) = buf.at(j) * 10 + (str[i++] & 0x0f);
            }
            j++; i++;
        }
        build_addEdges(u, buf, j);
    }
}
#elif READ_TYPE == 1
void Graph::build2(FILE *fp) {
    unsigned u;
    char str;
    vector<unsigned> buf;

    while (fscanf(fp, "%c", &str) != EOF) {
        u = 0;
        while(str != ':') {
            u = u * 10 + (str & 0x0f);
            fscanf(fp, "%c", &str);
        }
        fscanf(fp, "%c", &str);
        fscanf(fp, "%c", &str);

        while(str != '\n') {
            while(str != '#') {
                buf.push_back(0);
                while(str != ' ') {
                    buf.back() = buf.back() * 10 + (str & 0x0f);
                    fscanf(fp, "%c", &str);
                }
                fscanf(fp, "%c", &str);
            }
            build_addEdges(u, buf, buf.size());
            fscanf(fp, "%c", &str);
        }
        buf.clear();
    }
}
#elif READ_TYPE == 2
void Graph::build3(FILE *fp) {
    unsigned u;
    char str;
    vector<unsigned> buf;
    unsigned val;

    while (fscanf(fp, "%d: ", &u) != EOF) {
        while(fscanf(fp, "%d ", &val)) {
            buf.push_back(val);
        }
        build_addEdges(u, buf, buf.size());
        buf.clear();
        fscanf(fp, "#\n");
    }
}
#endif