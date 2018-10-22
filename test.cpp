#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random> 
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

string Hashtable::hash(Vector vec, void (*get_value)(int,float,float&)) {
	for (int i = 0; i < k; ++i) // for every h_i()
	{
		
	}
}

Vector::Vector(void * array) : coords(array) {}
Vector::~Vector(void) {}
void Vector::get_coord_int(int i, float v_i, float& res) { // res is result of mult := (coordinate i of Vector)*v_i 
	if( i<Hashtable::k ){
		int * t_coords = (int *)coords;
		res = (float)v_i*t_coords[i];
		cout << res << " " << v_i << " " << t_coords[i] << endl;
	}
}

HashList::HashList(int nL) : L(nL) {}
