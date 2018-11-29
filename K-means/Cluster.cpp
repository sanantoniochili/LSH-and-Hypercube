#include <iostream>
#include <string>
#include <random>
#include <algorithm>       
#include "Cluster.h"
using namespace std;

Clusters::Clusters(int k) {
	this->k = k;
}
Clusters::~Clusters() {}

void Clusters::add_point(Point * np) {
	match.push_back(make_pair(np,-1));	// init pair of point with non existent cluster
}

void Clusters::print_points() {
	cout << "Printing points with cluster number.." << endl;
	for (int i = 0; i < match.size(); ++i)
	{
		cout << match[i].first->get_name() << " " << match[i].second << endl;
	}
}

void Clusters::print_centroids() {
	cout << "Printing cluster centroids.." << endl;
	for (int i = 0; i < centrs.size(); ++i)
	{
		cout << match[centrs[i]].first->get_name() << endl;
		//centrs[i]->print_coords();
	}
}

void Clusters::rand_centr() {
	srand (time(NULL));

	for (int i = 0; i < k; ++i)
	{
		int index = rand() % match.size();	// select random points for cluster centroids
		centrs.push_back(index);
	}
}

void Clusters::kmeanspp() {
	std::default_random_engine generator;
  	std::uniform_int_distribution<int> distribution(0,match.size());

  	int count = 1;
  	do {
	for (int i = 0; i < match.size(); ++i)
	{
		if (std::find(centrs.begin(), centrs.end(), i) == centrs.end()) { // not a centroid
			for (int i = 0; i < centrs.size(); ++i) // calculate min distance among centroids
			{
				
			}

		}
	}
  	}while( count!=k );

}