#ifndef __CLUSTER__
#define __CLUSTER__

#include <iostream>
#include <string>
#include "../LSH-and-Hypercube/Point.h"
using namespace std;

class Clusters{
private:
	int k;
	vector<int> centrs;
	vector<pair<Point *, int>> match;
public:
	Clusters(int); // init number of clusters
	~Clusters();
	void rand_centr();
	void add_point(Point *);
	void print_points();
	void print_centroids();
	void kmeanspp();
};

#endif