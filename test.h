#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random>
using namespace std; 

class Vector{
public:
	void * coords;

	Vector(void *);
	~Vector();
	void get_coord_int(int,float,float&);
};

class Hashtable{
private:
	forward_list<float> t;
	forward_list<int> r;
	forward_list<float *> v;
	unordered_multimap<string, Vector> table;

public:
	static int w,k,d;
	
	Hashtable();
	~Hashtable();	
	void print_params();
	string hash(Vector, void (*get_value)(int,float,float&)); // 1:pos,2:v_coordinate,3:result
};

class HashList{
private:
	forward_list<Hashtable> HL;

public:
	int L;

	HashList(int);
	~HashList();	
};
