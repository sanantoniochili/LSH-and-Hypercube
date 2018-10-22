#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random>
using namespace std; 

class Point{
public:
	static int d;

	Point();
	~Point();
	virtual float get_coord(const int& i,const float& v_i) {}
};

class Point_int : public Point{
private:
	int * coords;
public:
	Point_int(int *);
	~Point_int();
	float get_coord(const int& i,const float& v_i);	
};

class Hashtable{
private:
	forward_list<float> t;
	forward_list<int> r;
	forward_list<float *> v;
	unordered_multimap<string, Point> table;

public:
	static int w,k,d;
	
	Hashtable();
	~Hashtable();	
	void print_params();
	float * get_v(int);
	string hash(Point&); // 1:pos,2:v_coordinate
};

class HashList{
private:
	forward_list<Hashtable> HL;

public:
	int L;

	HashList(int);
	~HashList();	
};
