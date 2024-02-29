#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#define INFINITY numeric_limits<int>::infinity();

#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

class Vertex {
public:
	bool visited;
	int distance;
	int previous;
	int finish;
	std::vector<int> adj;
};

class Graph {
public:
	int time;

	Graph(int V, int E, std::pair<int, int> *edges) {
		_V = V;
		_E = E;
		vertices = new Vertex[_V];
		for (int i = 0; i < _V; i++) {
                    vertices[i].visited = false;
                    vertices[i].distance = INFINITY;
                    vertices[i].previous = -1;
		}
		for (int i = 0; i < _E; i++) {
		    addEdge(edges[i].first, edges[i].second);
		}
		time = 0;
	}

	virtual ~Graph() {
		for (int i=0; i<_V; ++i) {
		    auto adj = vertices[i].adj;
		    adj.clear(); // clear list
		}

		delete[] vertices; // delete array of vertices
	}

	int V() {
		return _V;
	}

	int E() {
		return _E;
	}

	void addEdge(int u, int v) {
		vertices[u].adj.push_back(v);
	}

	std::vector<int> getEdges(int u) {
		return vertices[u].adj;
	}

	int degree(int u) {
		return vertices[u].adj.size();
	}

	void bfs(int s) {
		queue<int> Q;
		vertices[s].distance = 0;
		vertices[s].visited = true;
		Q.push(s);
		while(!Q.empty()){
			int u = Q.front();
			for (int i = 0; i < vertices[u].adj.size(); i++){
				int a = vertices[u].adj.at(i);
				if (!vertices[a].visited){
					vertices[a].visited = true;
					vertices[a].distance = vertices[u].distance + 1;
					vertices[a].previous = u;
					Q.push(a);
				}
			}
			Q.pop(); //node is black
		}
	}

	void dfs() {
		time = 0;
		for (int i = 0; i < _V; i++){
			if(!vertices[i].visited){
				dfs_visit(i);
			}
		}
	}

	void dfs_visit(int u) {
		time++;
		vertices[u].distance = time;
		vertices[u].visited = true;
		for (int i = 0; i < vertices[u].adj.size(); i++){
				int a = vertices[u].adj.at(i);
				if (!vertices[a].visited){
					vertices[a].previous = u;
					dfs_visit(a);
				}
			}
		cout << u << " ";    //print node when it turns black
		time++;
		vertices[u].finish = time;
	}

	void reset(){ //reset node values to test new graph search
		for (int i = 0; i < _V; i++) {  
                    vertices[i].visited = false;
                    vertices[i].distance = INFINITY;
                    vertices[i].previous = -1;
		}
	}

	void print_path(int s, int v) {
		if (v == s)
		   std::cout << s;
		else if (vertices[v].previous == -1)
		   std::cout << "not connected";
		else {
		   print_path(s,vertices[v].previous);
		   std::cout << "->" << v;
		}
	}

	std::string toString() {
		std::stringbuf buffer;
		std::ostream os(&buffer);
		os << "Vertices = " << _V << ", Edges = " << _E << std::endl;
		for (int i = 0; i < _V; ++i) {
		    os << i << "(" << degree(i) << "): ";
		    for (const auto& l : vertices[i].adj) 
			os << l << " ";
		    os << std::endl;
		}

		return buffer.str();
	}
private:
	int _V; // no. of vertices
	int _E; // no. of edges
	Vertex *vertices; // array of vertices
};

#endif
