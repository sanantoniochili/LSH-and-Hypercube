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
	forward_list<float> t;
	forward_list<int> r;
	forward_list<float *> v;
	unordered_multimap<long long, Point *> table;

public:
	static int w,k,d;
	
	Hashtable(float loadfactor);
	~Hashtable();	
	void print_params();
	float * get_v(int);
	long long phi_hash(Point&); // 1:pos,2:v_coordinate
	void fill(std::vector<Point *>&);
	void print_table();
	DPnt NN(Point *,double(*metric)(Point *,Point *),DPnt,double,vector<DPnt>&);

};

class Cube : public Hashtable{
private:
	forward_list<float> f;
	unordered_multimap<string, Point *> cube;
public:
	Cube(float);
	~Cube();
	string cube_hash(Point&);
	void fill(std::vector<Point *>&);
	DPnt NN(Point * query,double(*metric)(Point *,Point *),int probes,int M,double R,vector<DPnt>& nns);

	
};
#endif