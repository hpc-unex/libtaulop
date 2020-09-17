//
//  allgather_rda.cpp
//  Recursive Doubling Algather algorithm
//
//  Created by jarico on 17/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "graph.hpp"

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;




Graph::Graph () {
	this->max_depth      = 0;
}

Graph::~Graph () {
}


void Graph::insert (Edge edge) {
	
	this->edges.push_back(edge);
	
	if (edge.depth > this->max_depth) {
		this->max_depth = edge.depth;
		this->edges_in_depth.push_back(1);
	} else {
		this->edges_in_depth[edge.depth-1] += 1;
	}
}


void Graph::insert (int src, int dst, int depth) {
	Edge edge = {src, dst, depth};
	this->insert(edge);
}


int Graph::maxDepth () {
	return this->max_depth;
}


int Graph::numEdgesInDepth (int depth) {
	return this->edges_in_depth[depth-1];
}


void Graph::getEdgesInDepth (int depth, vector<Edge> &edges) {
	
	bool end = false;
	int  i   = 0;
	
	for (Edge e : this->edges) {
		
		if (e.depth == depth) {
			edges.push_back(e);
		}

		if (e.depth > depth)   break;

	}
}


int Graph::numEdgesFromSrc (int src) {
	
	int num = 0;
	
	for (Edge e : this->edges) {
		if (e.src == src) {
			num++;
		}
	}
	
	return num;
}


void Graph::getEdgesFromSrc (int src, vector<Edge> &edges) {
	
	int  i = 0;

	for (Edge e : this->edges) {
		if (e.src == src) {
			edges.push_back(e);
		}
	}
}


int Graph::numEdgesToDst (int dst) {
	
	int num = 0;
	
	for (Edge e : this->edges) {
		if (e.dst == dst) {
			num++;
		}
	}
	
	return num;
}


void Graph::getEdgesToDst (int dst, vector<Edge> &edges) {
	
	int  i = 0;
	
	for (Edge e : this->edges) {
		if (e.dst == dst) {
			edges.push_back(e);
		}
	}
}


void Graph::show () {

	string sep = ",";
	
	cerr << "[";
	for (auto it = this->edges.begin(); it != this->edges.end(); ++it) {
		
		cerr << "(" << it->src << "," << it->dst << "," << it->depth << ")";
	
		if (it == this->edges.end() - 1) {
			sep = "";
		}
		
		cerr << sep;
	}
	cerr << "]" << endl;
	cerr << "Max. depth: " << this->max_depth << endl;
}
