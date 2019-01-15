#ifndef __CLUSTER__
#define __CLUSTER__

#include <iostream>
#include <string>
#include "../LSH-and-Hypercube/Point.h"
#include "../LSH-and-Hypercube/Hashlist.h"

using namespace std;

typedef pair<int,double> DCnt;

class Clusters{
private:
	int k;
	vector<int> centrs; // each element-centroid is the index to "match" vector
	vector<pair<Point *, int>> match; // vector with the input points
	vector<double> obf; // terms (one for each point) of objective function's sum
public:
	int N; // number of input points

	Clusters(int); // init number of clusters
	~Clusters();
	// initialization
	void kmeanspp(double (*metric)(Point *,Point *));
	DCnt closest_centr(Point *,double (*metric)(Point *,Point *));
	//assignment
	void lloyds_assign(double (*metric)(Point *,Point *));
	void LSH_assign(Hashlist *, string, double (*metric_ptr)(Point *,Point *));
	// update
	void lloyds_update(double (*metric)(Point *,Point *)); // centroids as points existent in initial input
	void lloyds_update_virtual(double (*metric)(Point *,Point *)); // centroids as fictional points 
	void PAM_update(double (*metric)(Point *,Point *));
	// misc
	void rand_centr();
	void add_point(Point *);
	void print_points();
	void print_centroids();
	DCnt closest_to_virtual(Point * p, int cluster, double (*metric)(Point *,Point *));
	double compute_objective(int,int,double (*metric)(Point *,Point *));
	void swap(int centroid, int point);
	void iter1(int count, double (*metric)(Point *,Point *));
	void iter2(int count, double (*metric)(Point *,Point *));
	void lloyds_virtual_clean();
	Point * get_centr(int i) { return match[centrs[i]].first; }
	int find_point_by_name(Point * p);
	Hashlist * LSH_fill(std::vector<Point *> *, int, string);

};

int choose_range(int,double[],double);

class Range{
private:
	double R;
public:
	int assigned;
	int centroid;

	Range(int, Clusters *, double (*metric)(Point *,Point *)); // init R with min distance from other centroids
	~Range() {};
	void doubleR() { this->R = this->R * 2; }
	double getR() { return this->R; }
	
};

#endif