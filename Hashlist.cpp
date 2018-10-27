#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random> 
#include <math.h>
#include <cmath>
#include <climits>
#include <utility>
#include <functional>
#include "Hashtable.h"
#include "Hashlist.h"

using namespace std;

Hashlist::Hashlist(int nL, float loadfactor) : L(nL) {
	std::forward_list<Hashtable *>::iterator iter = list.before_begin();
	for (int i = 0; i < L; ++i)
	{
		Hashtable * Ht = new Hashtable(loadfactor);
		iter = list.insert_after(iter, Ht);
	}
}
Hashlist::~Hashlist() {
	for ( auto iter = list.begin(); iter != list.end(); ++iter )
	{
		delete *iter;
	}
}

Point * Hashlist::NN(Point * query,double (*metric)(Point *,Point *)) {
	Point * nn = NULL;

	std::forward_list<Hashtable *>::iterator iter = list.before_begin();
	for (int i = 0; i < L; ++i)
	{
		//(*iter)->NN(query,metric);
	}

	return nn;
}