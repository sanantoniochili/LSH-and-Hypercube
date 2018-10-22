#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random> 
#include <math.h>
#include <cmath>
#include "test.h"

using namespace std;

Hashtable::Hashtable() {

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
	}
Hashtable::~Hashtable() {

		std::forward_list<float *>::iterator it_v = v.before_begin();
		for (int i = 0; i < k; ++i)
		{
			delete(*it_v);
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

string Hashtable::hash(Point& p) { // applying hash function
	std::forward_list<float *>::iterator it_v = v.begin();
	std::forward_list<float>::iterator it_t = t.begin();
	
	string res = "";
	for (int i = 0; i < k; ++i) // for every h_i()
	{
	cout << "iter: " << i << endl;
		// calculating h()
		float fr = 0.0;
		for (int i = 0; i < Point::d; ++i)
		{
			fr += p.get_coord(i,(*it_v)[i]);
		}
		
		// adding two fractions
		fr /= (float)w;
	cout << fr << "+";

		fr += (*it_t / (float)w);
	cout << (*it_t / (float)w);

		// convert floor to string
		int res_i;
		if( i==0 ) {
			res_i = (int)floor (fr);
		} else {
			res_i = abs ((int)floor (fr));
		}
		res += to_string(res_i);
	cout << "=" << res_i << endl;

		// proceed with next h()
		it_v++;
		it_t++;

	}
cout << res << endl;
	return res;
}

Point::Point(void) {}
Point::~Point(void) {}

Point_int::Point_int(int * array) : coords(array) {}
Point_int::~Point_int(void) { /*delete coords;*/ }

float Point_int::get_coord(const int& i, const float& v_i) { // res is result of mult := (coordinate i of Vector)*v_i 
	return (float)v_i*coords[i];	
}

HashList::HashList(int nL) : L(nL) {}
