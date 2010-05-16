#include "point.h"

using namespace libcan;

//-------------------------CONSTRUCTORS
point::point(double new_x, double new_y):x(new_x),y(new_y) {}
point::point(long new_x, long new_y):x(new_x),y(new_y) {}
point::point(int new_x, int new_y):x(new_x),y(new_y) {}


point::point():x(0), y(0) { }

//-------------------------OTHER
point
point::move(const double distance, double degrees) const{
	while (degrees<0) {
		degrees+=360;
	}
	while (degrees>=360) {
		degrees-=360;
	}	//modulo, ale na cislech, kde mi to c++ nedovoli
	
	double rad = __DEG2RAD(degrees);
	return point(x+distance*cos(rad), y+distance*sin(rad));
}



point
point::operator+(const point& other) const{
	return point(x+other.x, y+other.y);
}

point 
point::operator-(const point& other) const {
	return point(x-other.x, y-other.y);
}

point 
point::operator*(const double n) const {
	return point(x*n, y*n);
}

point 
point::operator*(const long n) const {
	return point(x*n, y*n);
}

point 
point::operator*(const int n) const {
	return point(x*n, y*n);
}
point
point::trunc() const {
	return point(static_cast<long>(x), static_cast<long>(y));
}

point& 
point::operator=(const point& other) {
	x=other.x;
	y=other.y; 
	return *this;
}

bool 
point::operator==(const point& other) const {
	return (abs(x - other.x)<0.01 && abs(y - other.y)<0.01);
}

bool 
point::operator!=(const point& other) const {
	return (abs(x - other.x)>=0.01 || abs(y - other.y)>=0.01);
}


point 
point::distance_from(const point& other) const {
	return point(other.x-x,other.y-y);
}

