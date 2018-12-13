#include <iostream>
#include <string>
#include <random>
#include <algorithm>       
#include "Cluster.h"
using namespace std;

Clusters::Clusters(int k) {
	this->k = k;
}
Clusters::~Clusters() { 
	match.clear(); 
	centrs.clear(); 
	obf.clear();
}

void Clusters::add_point(Point * np) {
	match.push_back(make_pair(np,-1));	// init pair of point with non existent cluster
}

void Clusters::print_points() {
	cout << "Printing points with cluster number.." << endl;
	for (int i = 0; i < match.size(); ++i)
	{//match[i].first->print_coords();
		if( match[i].second<0 )
			cout << "Name: " << match[i].first->get_name() << " [Centroid] " << endl;
		else
			cout << "Name: " << match[i].first->get_name() << " Index of centroid assigned: " << match[i].second << endl;
	}
}

void Clusters::print_centroids() {
	cout << "Printing cluster centroids.." << endl;
	for (int i = 0; i < centrs.size(); ++i)
	{
		cout << "Name: " << match[centrs[i]].first->get_name() << endl;
		//centrs[i]->print_coords();
	}
}

void Clusters::rand_centr() {
	srand (time(NULL));

	centrs.clear();
	for (int i = 0; i < k; ++i)
	{
		int index = rand() % match.size();	// select random points for cluster centroids
		centrs.push_back(index);
	}
}

void Clusters::kmeanspp(double (*metric)(Point *,Point *)) {
	std::default_random_engine generator;
  	std::uniform_int_distribution<int> distribution(0,match.size());

  	centrs.clear();
  	centrs.push_back(distribution(generator)); // choose random first centroid

  	int count = 1; // counting centroids
  	double dists[match.size()];
  	do {
		for (int i = 0; i < match.size(); ++i)
		{
			if (std::find(centrs.begin(), centrs.end(), i) == centrs.end()) { // not a centroid
				Point * p = match[i].first;
				dists[i] = closest_centr(p,metric).second; // min distance to some centroid
			} else {
				dists[i] = 0;
			}
		}
		double maxD = *max_element(dists,dists+match.size()); // find max Di for normalisation
		double part_sums[match.size()+1]; // create array of Dis partial sums
		part_sums[0] = 0; // first value is zero
  		int index = 1;
		for (int i = 0; i < match.size(); ++i)
		{
			part_sums[index] = part_sums[index-1] + pow(dists[i],2.0) / maxD; // new partial sum
			index++;
		}

		std::uniform_real_distribution<double> distribution(0.0,part_sums[match.size()]);
		double random = distribution(generator);
		centrs.push_back(choose_range(match.size()-1,part_sums,random)); // add choosen point
		
  		count++;
  	}while( count!=k );
}

DCnt Clusters::closest_centr(Point * p,double (*metric)(Point *,Point *)) { // return position of nearest centroid and distance
	double minD = metric(p,match[centrs[0]].first);
	int minC = 0;
	for (int i = 1; i < centrs.size(); ++i) // calculate min distance among centroids
	{
		double dist = metric(p,match[centrs[i]].first); // find distance of point (arg 1) from centroid of cluster i (arg 2)
		if( dist<minD ){
			minD = dist;
			minC = i; // index of centroid in "centrs" vector
		}
	}
	return make_pair(minC,minD);

} 

int choose_range(int max, double array[], double random) {
	int min = 0;
    while (min < max) {
        int middle = min + (max - min) / 2;
        if (array[middle] < random)
            min = middle + 1;
        else
            max = middle;
    }
    return min;

}

void Clusters::lloyds_assign(double (*metric)(Point *,Point *)) {
	obf.clear(); // clear terms of objective function

	for (int i = 0; i < match.size(); ++i)
	{
		vector<int>::iterator it;
		if ((it = std::find(centrs.begin(), centrs.end(), i)) != centrs.end()) { // if centroid
			match[i].second = (*it); // assign same centroid
			obf.push_back(0); // keep min distance 
		} else {
			DCnt cnt_dist = closest_centr(match[i].first,metric); // find closest centroid
			match[i].second = cnt_dist.first; // assign closest centroid
			obf.push_back(pow(cnt_dist.second,2.0)); // keep min distance 
		}
	}
}

void Clusters::lloyds_update(double (*metric)(Point *,Point *)) {
	double means[k][Point::d];

	for (int i = 0; i < k; ++i)
	{
		for (int c = 0; c < Point::d; ++c)
		{
			means[i][c] = 0;
		}
	}
	for (int i = 0; i < k; ++i)
	{
		int count=0;
		for (int j = 0; j < match.size(); ++j)
		{
			if( match[j].second==i ) {
				double * coords = (double *)(match[j].first)->get_coords();
				for (int c= 0; c < Point::d; ++c)
				{
					means[i][c] += coords[c]; // calculate mean values of coordinates
				}
				count++;
			}
		}

		for (int c = 0; c < Point::d; ++c)
		{
			means[i][c] /= count;
		}
		
	}


	for (int i = 0; i < k; ++i)
	{
		double * array = new double[Point::d];
		for (int c = 0; c < Point::d; ++c)
		{
			array[c] = means[i][c];
		}
		Point_double * p = new Point_double(array,"mean"); // create a point with mean values
		DCnt dcnt = closest_to_virtual(p,i,metric);
		centrs[i] = dcnt.first;

		delete p;
	}

}

void Clusters::lloyds_update_virtual(double (*metric)(Point *,Point *)) {
	//remove virtual points
	if( match.size()==N+k ) {
		for (int i = 0; i < k; ++i)
		{
			match.pop_back();
		}
	}

	double means[k][Point::d];
	for (int i = 0; i < k; ++i)
	{
		for (int c = 0; c < Point::d; ++c)
		{
			means[i][c] = 0;
		}
	}

	// calculate mean values of coordinates
	for (int i = 0; i < k; ++i)
	{
		int count = 0;
		for (int j = 0; j < match.size(); ++j)
		{
			if( match[j].second==i ) { // if it belongs to cluster i
				double * coords = (double *)(match[j].first)->get_coords();
				for (int c= 0; c < Point::d; ++c)
				{
					means[i][c] += coords[c]; 
				}
				count++;
			}
		}

		for (int c = 0; c < Point::d; ++c)
		{
			means[i][c] /= count;
		}
	}

	//add virtual points to vector (not existent in original input)
	for (int i = 0; i < k; ++i)
	{
		double * array = new double[Point::d];
		for (int c = 0; c < Point::d; ++c)
		{
			array[c] = means[i][c];
		}
		Point_double * p = new Point_double(array,"mean"); // create a point with mean values
		add_point(p);
	}



	//centrs.clear(); // clear last centroids and add new
	for (int i = 0; i < k; ++i)
	{
		centrs[i] = N+i;
	} 

}

void Clusters::lloyds_virtual_clean() {
	//remove virtual points
	if( match.size()==N+k ) {
		for (int i = 0; i < k; ++i)
		{
			match.pop_back();
		}
	}
}


/*void Clusters::PAM_update(double (*metric)(Point *,Point *)) {
	for (int i = 0; i < k; ++i)
	{
		
	}
}*/


DCnt Clusters::closest_to_virtual(Point * p, int cluster, double (*metric)(Point *,Point *)) {
	double minD = metric(p,match[0].first);
	int minC = 0;
	int first = 0;
	for (int i = 0; i < match.size(); ++i)
	{
		if( match[i].second==cluster ) { // if it belongs to cluster "cluster"
			double dist = metric(p,match[i].first); // find distance of points 
			if( first==0 && dist!=0 ){
				minC = i;
				minD = dist;
				first++;
			} else {
				if( dist<minD && dist!=0 ){
					minD = dist;
					minC = i; // index of point in "match" vector
				}
			}
		}
	}
	return make_pair(minC,minD);
}


void Clusters::iter1(int count, double (*metric)(Point *,Point *)) {
	rand_centr();
	lloyds_assign(metric);
	for (int i = 0; i < count; ++i)
	{
		lloyds_update(metric);
		lloyds_assign(metric);

	}
}

void Clusters::iter2(int count, double (*metric)(Point *,Point *)) {
	kmeanspp(metric);
	lloyds_assign(metric);
	for (int i = 0; i < count; ++i)
	{
		lloyds_update_virtual(metric);
		lloyds_assign(metric);
	}
	lloyds_virtual_clean();

}