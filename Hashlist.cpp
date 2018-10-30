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

DPnt Hashlist::NN(Point * query,double (*metric)(Point *,Point *)) {
	DPnt min(NULL,-1);
	int count = 1;
	for ( auto iter = list.begin(); iter != list.end(); ++iter ) // for each hashtable
	{
		cout << "--Hashtable " << count++ << "--" << endl; 
		min = (*iter)->NN(query,metric,min);
	}

	if( min.first ){
		cout << query->get_name();
		query->print_coords();
		cout << "Final NN: " << min.first->get_name();
		min.first->print_coords();
	} else{
		cout << "**Found no neighbours.\n" << endl;
	}
	return min;
}