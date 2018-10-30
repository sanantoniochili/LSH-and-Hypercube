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
#include "Point.h"

using namespace std;

Point::Point(string nname) : name(nname) { this->g = ""; }
Point::~Point(void) {}

void Point::add_h2g(long  h) {
	this->g += to_string(h);
}
void Point::get_g(void) {
	cout << "Concatenation of h's for point: " << this->g << endl;
}
string Point::get_name() { return this->name; }

Point_int::Point_int(int * array, string nname) : Point(nname),coords(array) {}
Point_int::~Point_int(void) { delete[] coords; }

float Point_int::get_multcoord(const int& i, const float& v_i) { // res is result of mult := (coordinate i of Vector)*v_i 
	return (float)v_i*coords[i];	
}

void Point_int::print_coords(void) {
	for (int i = 0; i < d; ++i)
	{
		cout << '\t' << this->coords[i];
	}
	cout << endl;
}

void* Point_int::get_coords() {
	return coords;
}

double Point_int::abs() {
	double sum = 0;
	for (int i = 0; i < d; ++i)
	{
		sum += coords[i]*coords[i];
	}
	return sqrt(sum);
}
