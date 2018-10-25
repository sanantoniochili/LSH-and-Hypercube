#include <iostream>
#include <string>
#include <unordered_map>
#include <forward_list>  
#include <vector>  
#include <chrono>
#include <random>
using namespace std; 


class Point{
private:
	string name;
	string g;
public:
	static int d;

	Point(string);
	~Point();
	virtual float get_coord(const int& i,const float& v_i) {}
	virtual void print_coords() {}
	void add_h2g(long);
	void get_g();
	string get_name();
};

class Point_int : public Point{
private:
	int * coords;
public:
	Point_int(int *,string);
	~Point_int();
	float get_coord(const int& i,const float& v_i);	
	void print_coords();
};

class Hashtable{
private:
	forward_list<float> t;
	forward_list<int> r;
	forward_list<float *> v;
	unordered_multimap<long long, Point *> table;

public:
	static int w,k,d;
	
	Hashtable(float loadfactor);
	~Hashtable();	
	void print_params();
	float * get_v(int);
	long long hash(Point&); // 1:pos,2:v_coordinate
	void fill(std::vector<Point *>&);
	void print_table();
	int get_bucketn();
};

class Hashlist{
public:
	forward_list<Hashtable *> list;
	int L;

	Hashlist(int,float);
	~Hashlist();
};
