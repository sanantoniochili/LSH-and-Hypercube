#ifndef __POINT__
#define __POINT__

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

#endif