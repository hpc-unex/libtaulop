//
//  graph_hpp
//  Representing of a Communication Graph (Virtual topology)
//    between ranks.
//
//  Created by jarico on 16/Sep/20.
//  Copyright © 2020 Juan A. Rico. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include <vector>
#include <iostream>
using namespace std;


// Represent an edge of the graph composed of: (src,dst,depth)
class Edge {
public:
	int src;
	int dst;
	int depth;
	
	void show () {
		cout << "(" << this->src << "," << this->dst << "," << this->depth << ")" << endl;
	}
};

// Graph represented as a set of edges
class Graph {
    
private:
	
	vector<Edge>  edges;
	vector<int>   edges_in_depth;
	int           max_depth;
	
public:
    
     Graph ();
    ~Graph ();
	
	void  insert          (Edge edge);
	void  insert          (int src, int dst, int depth);
	
	int   maxDepth        ();
	
	int   numEdgesInDepth (int depth);
	void  getEdgesInDepth (int depth, vector<Edge> &edges);

	int   numEdgesFromSrc (int src);
	void  getEdgesFromSrc (int src,   vector<Edge> &edges);

	int   numEdgesToDst   (int dst);
	void  getEdgesToDst   (int dst,   vector<Edge> &edges);
	
	void  show ();
};


#endif /* graph_hpp */
