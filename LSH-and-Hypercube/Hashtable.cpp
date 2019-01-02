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
#include <bitset>
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

		// random device class instance, source of 'true' randomness for initializing random seed
	    std::random_device rd; 

	    // Mersenne twister PRNG, initialized with seed from previous random device instance
	    std::mt19937 generator(rd()); 

		normal_distribution<float> ndistr (0.0,1.0); // Gaussian distribution
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
		long long temp = 0.0;
		for (int i = 0; i < Point::d; ++i)
		{
			temp += (p.get_multcoord(i,(*it_v)[i])/(float)w);
		}

		// adding two fractions
		temp += (*it_t / (float)w);
		long long h = floor (temp);
		p.add_h2g(abs(h));
		
		long a = mod(*it_r,M);
		long b = mod(h,M);
		// multiply with r and apply mod M
		if( multOvf(a,b) ) { // check for overflow
			cout << "Overflow detected" << endl;
			exit (-1);
		}
		long res_i = mod(a*b,M);

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


void Hashtable::fill(std::vector<Point *>& points, string metric) {
	if( metric[0]=='c' ) {
		for (int i = 0; i < points.size(); ++i)
		{
			table.insert(make_pair(bit_hash(*(points[i])),points[i]));
		}
	} else {
		for (int i = 0; i < points.size(); ++i)
		{
			table.insert(make_pair(phi_hash(*(points[i])),points[i]));
		}
	}

}

long long Hashtable::bit_hash(Point& p) { // applying hash function 
	std::forward_list<float *>::iterator it_v = v.begin();
	
	// creating bit string
	char res[k+1];
	for (int i = 0; i < k; ++i) // for every h_i()
	{
		// calculating r_i * p
		long long temp = 0.0;
		for (int i = 0; i < Point::d; ++i)
		{
			temp += p.get_multcoord(i,(*it_v)[i]);
		}
		int h = (temp >= 0);
		p.add_h2g(h);
		if( h==0 ) res[i] = '0';
		else res[i] = '1';		
		// proceed with next h()
		it_v++;
	}
	res[k] = '\0';
	unsigned long long number =  strtoull (res, NULL, 2);
	return number;
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


DPnt Hashtable::NN(Point * query,string metric_name,double(*metric)(Point *,Point *),DPnt lastnn,double R,vector<DPnt>& nns) {
	unsigned long buc = 0;
	if( metric_name[0]=='c' ){
		buc = table.bucket(bit_hash(*query));
	}else{
		buc = table.bucket(phi_hash(*query)); // getting bucket number of query
	}

	Point * neighbour = NULL;
	DPnt min(lastnn); // init with previous nearest neighbour

	for ( auto local_it = table.begin(buc); local_it!= table.end(buc); ++local_it ){
	    neighbour = local_it->second;

	    double dist = metric(query,neighbour);
	    if( min.first==NULL && min.second<0 ) {
	    	min.first = neighbour;
	    	min.second = dist;
	    } else {
	    	// using g values to determine neighbourhood
	    	if( query->get_g().compare(neighbour->get_g())==0 ) {
			    if( dist<min.second ) {
			    	min.first = neighbour;
			    	min.second = dist;
			    }
			    if( dist<=R ) {
			    	nns.push_back(make_pair(neighbour,dist));
			    }
			}
		}
	}
	return min;

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

Cube::Cube(float loadfactor) : Hashtable(loadfactor) {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	srand( time( NULL ) );

	std::forward_list<float>::iterator it_f = f.before_begin();
	for (int i = 0; i < k; ++i) // for every v vector of k hashtables
	{
		// k parameters of f
		float value = (rand() / (RAND_MAX + 1.0));
		it_f = f.insert_after(it_f, value);
	}

}
Cube::~Cube() {}

string Cube::cube_hash(Point& p, string metric) { // applying hash function 
	std::forward_list<float *>::iterator it_v = v.begin();
	std::forward_list<float>::iterator it_t = t.begin();
	std::forward_list<int>::iterator it_r = r.begin();
	std::forward_list<float>::iterator it_f = f.begin();
	
	string res = "";
	// calculating h()
	for (int i = 0; i < k; ++i) // for every h_i()
	{
		if( metric[0]=='c' ){
			// cosine similarity
			// calculating r_i * p
			long long temp = 0.0;
			for (int i = 0; i < Point::d; ++i)
			{
				temp += p.get_multcoord(i,(*it_v)[i]);
			}

			int h = (temp >= 0);
			p.add_h2g(h);		
			string bit = std::to_string((*it_f)*temp >= 0.5);
			res += bit;
		} else {

			// euclidean
			long long temp = 0.0;
			for (int i = 0; i < Point::d; ++i)
			{
				temp += (p.get_multcoord(i,(*it_v)[i])/(float)w);
			}

			// adding two fractions
			temp += (*it_t / (float)w);
			long long h = floor (temp);
			p.add_h2g(abs(h));
			string bit = std::to_string(abs((*it_f)*h) >= 0.5);

			res += bit;
		}
		// proceed with next h()
		it_v++;
		it_t++;
		it_r++;
		it_f++;

	}
	return res;
}

void Cube::fill(std::vector<Point *>& points, string metric) {
	for (int i = 0; i < points.size(); ++i)
	{
		cube.insert(make_pair(cube_hash(*(points[i]),metric),points[i]));
	}

}

DPnt Cube::NN(Point * query,string metric_name,double(*metric)(Point *,Point *),int probes, int M, double R,vector<DPnt>& nns) {
	unsigned long buc = cube.bucket(cube_hash(*query,metric_name));
	/*cout << "Bucket of " << query->get_name();
	query->print_coords();*/

	Point * neighbour = NULL;
	DPnt min(NULL,-1);

	for ( auto local_it = cube.begin(buc); local_it!= cube.end(buc); ++local_it ){
	    neighbour = local_it->second;
	    double dist = metric(query,neighbour);
	    if( min.first==NULL && min.second<0 ) {
	    	min.first = neighbour;
	    	min.second = dist;
	    } else {
	    	if( query->get_g().compare(neighbour->get_g())==0 ) {
cout << "here" << endl;
			    if( dist<min.second ) {
			    	min.first = neighbour;
			    	min.second = dist;
			    }
			    if( dist<=R ) {
			    	nns.push_back(make_pair(neighbour,dist));
			    }
			}
		}
		if( --M==0 ) return min;
	}
	probes--;
	float half = (float)probes /(float) 2;
	int maxb = ceil(buc+half);
	for (int i = buc; i < maxb; ++i)
	{
		for ( auto local_it = cube.begin(i); local_it!= cube.end(i); ++local_it ){
		    neighbour = local_it->second;

		    double dist = metric(query,neighbour);
		    if( min.first==NULL && min.second<0 ) {
		    	min.first = neighbour;
		    	min.second = dist;
		    } else {
		    	if( query->get_g().compare(neighbour->get_g())==0 ) {
				    if( dist<min.second ) {
				    	min.first = neighbour;
				    	min.second = dist;
				    }
				    if( dist<=R ) {
				    	nns.push_back(make_pair(neighbour,dist));
				    }
				}
			}
			if( --M==0 ) return min;
		}
	}
	int minb = floor(buc-half);
	for (int i = minb; i < buc; ++i)
	{
		for ( auto local_it = cube.begin(i); local_it!= cube.end(i); ++local_it ){
		    neighbour = local_it->second;

		    double dist = metric(query,neighbour);
		    if( min.first==NULL && min.second<0 ) {
		    	min.first = neighbour;
		    	min.second = dist;
		    } else {
		    	if( query->get_g().compare(neighbour->get_g())==0 ) {
				    if( dist<min.second ) {
				    	min.first = neighbour;
				    	min.second = dist;
				    }
				    if( dist<=R ) {
				    	nns.push_back(make_pair(neighbour,dist));
				    }
				}
			}
			if( --M==0 ) return min;
		}
	}


	return min;

}
