#ifndef __HASHLIST__
#define __HASHLIST__

#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random>
#include "Hashtable.h"
using namespace std; 

class Hashlist{
public:
	forward_list<Hashtable *> list;
	int L;

	Hashlist(int,float);
	~Hashlist();
	DPnt NN(Point *,string,double(*metric)(Point *,Point *),double,vector<DPnt>&);
};

#endif
