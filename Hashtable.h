#ifndef __HASHTABLE__
#define __HASHTABLE__

#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random>
#include <utility>
#include "Point.h"

using namespace std;

typedef pair<Point *,double> DPnt;

class Hashtable{
protected:
	forward_list<float> t; // lists of k parameters
	forward_list<int> r;
	forward_list<float *> v;
private:
	unordered_multimap<long long, Point *> table;
	long long phi_hash(Point&);
	long long bit_hash(Point&); 

public:
	static int w,k,d;
	
	Hashtable(float loadfactor); // loadfactor to define tablesize
	~Hashtable();	
	void print_params();
	float * get_v(int);
	void fill(std::vector<Point *>&,string); // init hashtables
	void print_table();
	DPnt NN(Point *,string,double(*metric)(Point *,Point *),DPnt,double,vector<DPnt>&);

};

class Cube : public Hashtable{
private:
	forward_list<float> f;
	unordered_multimap<string, Point *> cube;
	string cube_hash(Point&,string);

public:
	Cube(float);
	~Cube();
	void fill(std::vector<Point *>&,string);
	DPnt NN(Point * query,string,double(*metric)(Point *,Point *),int probes,int M,double R,vector<DPnt>& nns); // metric function passed as parameter

	
};
#endif