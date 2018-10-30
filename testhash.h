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
#include <functional>
#include "Point.h"

using namespace std;

class mod_hash
{
public:
	size_t operator()( const long long & key ) const // <-- don't forget const
	{
		size_t hashVal = 0;
		return hashVal;
	}
};

class Hashtable{
private:
	forward_list<float> t;
	forward_list<int> r;
	forward_list<float *> v;
	unordered_multimap<long long,Point *,mod_hash> table;

public:
	static int w,k,d;
	
	Hashtable(float loadfactor);
	~Hashtable();	
	/*void print_params();
	float * get_v(int);
	long long phi_hash(Point&); // 1:pos,2:v_coordinate
	void fill(std::vector<Point *>&);
	void print_table();
	int get_bucketn();
	pair<Point *,double> NN(Point *,double(*metric)(Point *,Point *));*/

};

#endif