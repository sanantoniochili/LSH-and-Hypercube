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

using namespace std;
unsigned long M = pow(2,32)-5;

bool addOvf(long long,long long);
bool multOvf(long,long);
long long mod(long long,unsigned long);

Hashtable::Hashtable(float loadfactor) {

		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		std::forward_list<float>::iterator it_t = t.before_begin();
		std::forward_list<int>::iterator it_r = r.before_begin();
		std::forward_list<float *>::iterator it_v = v.before_begin();
  		
  		default_random_engine generator (seed);
		normal_distribution<double> ndistr (0.0,1.0); // Gaussian distribution
		srand( time( NULL ) );
		
		for (int i = 0; i < k; ++i) // for every v vector of k hashtables
		{
			// k parameters of t
			float value = w * (rand() / (RAND_MAX + 1.0));
			it_t = t.insert_after(it_t, value);

			// k parameters of r
			int rv = k * (rand() / (RAND_MAX + 1.0));
			it_r = r.insert_after(it_r, rv);

			// k pamameters of v = v1,v2,..,vd
			float * coords = new float[d];
			for (int i = 0; i < d; ++i) // single-precision real coordinates
			{
				coords[i] = ndistr(generator);
			}
			it_v = v.insert_after(it_v, coords);
		}

		this->table.max_load_factor ( loadfactor );
	}

Hashtable::~Hashtable() {
		for ( auto it_v = v.begin(); it_v != v.end(); ++it_v )
		{
			delete[] (*it_v);
		}
	}

void Hashtable::print_params(void) {
	cout << "Printing parameters of hashtable.." << endl;
	std::forward_list<float>::iterator it_t = t.begin();
	std::forward_list<int>::iterator it_r = r.begin();
	std::forward_list<float *>::iterator it_v = v.begin();
		
	cout << "t: ";
	for (int i = 0; i < k; ++i) // for every v vector of k hashtables
	{
		cout << "\t" << *it_t++; 
	}
	cout << endl << "r: ";
	for (int i = 0; i < k; ++i) // for every v vector of k hashtables
	{
		cout << "\t" << *it_r++; 
	}
	cout << endl << "v: " << endl;
	for (int i = 0; i < k; ++i) 
	{
		for (int i = 0; i < d; ++i) // single-precision real coordinates
		{
			cout << "\t" << (*it_v)[i]; 
		}
		cout << endl;
		it_v++;
	}
}

long long Hashtable::phi_hash(Point& p) { // applying hash function 
	std::forward_list<float *>::iterator it_v = v.begin();
	std::forward_list<float>::iterator it_t = t.begin();
	std::forward_list<int>::iterator it_r = r.begin();
	
	long long res = 0;

	for (int i = 0; i < k; ++i) // for every h_i()
	{
		// calculating h()
		float temp = 0.0;
		for (int i = 0; i < Point::d; ++i)
		{
			temp += p.get_coord(i,(*it_v)[i]);
		}

		// adding two fractions
		temp /= (float)w;
		temp += (*it_t / (float)w);
		long h = floor (temp);
		p.add_h2g(h);
		
		// multiply with r and apply mod M
		if( multOvf(*it_r,h) ) { // check for overflow
			cout << "Overflow detected" << endl;
			exit (-1);
		}
		long res_i = mod(((*it_r)*h),M);

		// add to result 
		if( addOvf(res_i,res) ) { // check for overflow
			cout << "Overflow detected" << endl;
			exit (-1);
		}
		res += res_i;

		// proceed with next h()
		it_v++;
		it_t++;
		it_r++;

	}

	res = mod(res,M);
	return res;
}

void Hashtable::fill(std::vector<Point *>& points) {
	for (int i = 0; i < points.size(); ++i)
	{
		table.insert(make_pair(phi_hash(*(points[i])),points[i]));
	}

}

void Hashtable::print_table(void) {
  	std::cout << "New hashtable. Buckets contain:\n";
  	for ( unsigned i = 0; i < table.bucket_count(); ++i) {
    	std::cout << "bucket #" << i << " contains:\n";
	    for ( auto local_it = table.begin(i); local_it!= table.end(i); ++local_it )
	    {
	    	std::cout << " " << local_it->second->get_name() << ": (" << local_it->first << ")";
			local_it->second->print_coords();
	    }
	    std::cout << std::endl;
	}
}


pair<Point *,double> Hashtable::NN(Point * query,double(*metric)(Point *,Point *)) {
	unsigned long buc = table.bucket(phi_hash(*query));
	cout << "For query:";
	query->print_coords();
	for ( auto local_it = table.begin(buc); local_it!= table.end(buc); ++local_it )
	    {
	    	std::cout << " " << local_it->second->get_name() << endl;
	    }
}





/* Fuction to check overflow on addition.
   credits: https://www.geeksforgeeks.org */
bool addOvf(long long a, long long b) 
{ 
   if( a > LLONG_MAX - b) 
     return true; 
   else
      return false; 
} 

/* Fuction to check overflow on multiplication.
   credits: https://www.geeksforgeeks.org */
bool multOvf(long a, long b) 
{ 
	// Check if either of them is zero 
	if (a == 0 || b == 0) 
		return false; 
	
	long result = a * b; 
	if (a == result / b) 
		return false; 
	else
		return true; 
} 

long long mod(long long a, unsigned long b) {
	return (abs(a) % b + b) % b; // absolute value to restrict size of
}