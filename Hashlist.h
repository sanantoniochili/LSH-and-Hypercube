#ifndef __HASHLIST__
#define __HASHLIST__

#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random>
using namespace std; 

class Hashlist{
public:
	forward_list<Hashtable *> list;
	int L;

	Hashlist(int,float);
	~Hashlist();
	Point * NN(Point *,double(*metric)(Point *,Point *));
};

#endif