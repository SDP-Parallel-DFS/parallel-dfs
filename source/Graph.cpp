#include "Graph.hpp"

using namespace std;

void Graph::init()
{
	roots.reserve(nNodes);
	nodes.resize(nNodes + 1);
	if (roots.size() > 0)
		return;
	for (unsigned int i = 0; i < nNodes; i++)
	{
		nodes[i].id = i;
		roots.insert(i);
	}
}

// should only be used by build
void Graph::build_addEdges(unsigned int u, vector<unsigned int> &adj, unsigned int adj_size)
{
	nodes[u].adj.resize(adj_size);
	for (unsigned int i = 0; i < adj_size; i++)
	{
		nodes[u].adj[i] = adj[i];
	}
}
Graph::Graph(FILE *fp) : nodes(), roots()
{
	unsigned int u;
	vector<unsigned> buf;
	unsigned val;

	fscanf(fp, "%d\n", &nNodes);
	init();
	buf = vector<unsigned int>(nNodes + 1);
	while (fscanf(fp, "%d: ", &u) != EOF)
	{
		while (fscanf(fp, "%d ", &val))
		{
			buf.push_back(val);
			roots.erase(val);
		}
		build_addEdges(u, buf, buf.size());
		buf.clear();
		fscanf(fp, "#\n");
	}

	// hidden parent of all roots
	nodes[nNodes].adj = vector<unsigned int>(roots.begin(), roots.end());
	sort(nodes[nNodes].adj.begin(), nodes[nNodes].adj.end(), std::less<unsigned int>());
}

Graph::Graph(ifstream &input_stream) : nodes(), roots()
{
	unsigned int u, v;
	string node_adj;
	vector<unsigned int> buf;
	if (getline(input_stream, node_adj))
	{
		istringstream iss(node_adj);
		iss >> nNodes;
		init();
		buf = vector<unsigned int>(nNodes + 1);
	}
	while (getline(input_stream, node_adj))
	{

		unsigned int i = 0;
		string dontcare;
		node_adj.pop_back();
		istringstream iss(node_adj);
		iss >> u;
		iss >> dontcare;
		// get the first token
		// walk through other tokens
		while (iss >> v)
		{
			roots.erase(v);
			buf.at(i++) = v;
		}
		this->build_addEdges(u, buf, i);
	}

	// hidden parent of all roots
	nodes[nNodes].adj = vector<unsigned int>(roots.begin(), roots.end());
	sort(nodes[nNodes].adj.begin(), nodes[nNodes].adj.end(), std::less<unsigned int>());
}

Graph::Graph(unsigned int nNodes) : nodes(), roots()
{
	this->nNodes = nNodes;
	init();
}

void Graph::printGraph()
{

	for (int v = 0; v < nNodes; ++v)
	{
		cout << v << ": " << (nodes[v].parent == nNodes ? -1 : nodes[v].parent);
		cout << endl;
	}

	cout << "Roots (" << roots.size() << "): " << endl;
	for (unsigned int x : roots)
		cout << x << " ";
	cout << endl;
}

void Graph::printNodesStatus()
{
	for (int v = 0; v < nNodes; ++v)
		cout << v << " " << nodes[v].s << " " << nodes[v].e << endl;
}

// meant to be used after building the graph.
void Graph::sortVectors()
{
	for (int v = 0; v < nNodes; v++)
	{
		sort(nodes[v].adj.begin(), nodes[v].adj.end(), std::less<int>());
	}
}

void Graph::addEdge(unsigned int u, unsigned int v)
{
	nodes[u].adj.push_back(v);
}

void Graph::sequentialDFS()
{
	pre = post = 0;
	for (unsigned int root : nodes[nNodes].adj)
		sequentialDFS_r(root);

	pre = post = 0;
}

void Graph::sequentialDFS_r(unsigned int p)
{
	nodes[p].visited = true; // node visited
	nodes[p].pre = pre++;
	for (unsigned int child : nodes[p].adj)
	{
		if (!nodes[child].visited)
		{
			nodes[child].parent = p;
			sequentialDFS_r(child);
		}
		if (nodes[p].s > nodes[child].s)
			nodes[p].s = nodes[child].s;
	}
	nodes[p].post = post++;
	nodes[p].e = nodes[p].post + 1;
	if (nodes[p].s > nodes[p].e)
		nodes[p].s = nodes[p].e;
}
