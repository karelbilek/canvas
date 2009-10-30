#include "point.h"
#include <cmath>

using namespace glib;

//-------------------------CONSTRUCTORS
point::point(glib_float new_x, glib_float new_y):x(new_x),y(new_y) {}
point::point(glib_int new_x, glib_int new_y):x(new_x),y(new_y) {}
point::point(int new_x, int new_y):x(new_x),y(new_y) {}

point::point(double new_x, double new_y):x(static_cast<glib_float>(new_x)),y(static_cast<glib_float>(new_y)) {}

point::point():x(0), y(0) { }

//-------------------------OTHER
point
point::move(const glib_float distance, glib_float degrees) const{
	while (degrees<0) {
		degrees+=360;
	}
	while (degrees>=360) {
		degrees-=360;
	}	//modulo, ale na cislech, kde mi to c++ nedovoli
	
	glib_float rad = __DEG2RAD(degrees);
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
point::operator*(const glib_float n) const {
	return point(x*n, y*n);
}

point 
point::operator*(const glib_int n) const {
	return point(x*n, y*n);
}

point 
point::operator*(const int n) const {
	return point(x*n, y*n);
}

point 
point::operator*(const double n) const {
	return point(x*n, y*n);
}

point& 
point::operator=(const point& other) {
	x=other.x;
	y=other.y; 
	return *this;
}

bool 
point::operator==(const point& other) const {
	return (__abs2(x - other.x)<0.01 && __abs2(y - other.y)<0.01);
}

bool 
point::operator!=(const point& other) const {
	return (__abs2(x - other.x)>=0.01 || __abs2(y - other.y)>=0.01);
}

// bool 
// point::smaller_deviation(const point& other) const {
// 	glib_float my_x_dev = x-static_cast<glib_int> (x);
// 	glib_float my_y_dev = y-static_cast<glib_int> (y);
// 	glib_float other_x_dev = other.x-static_cast<glib_int> (other.x);
// 	glib_float other_y_dev = other.y-static_cast<glib_int> (other.y);
// 	if (__abs2(my_x_dev-other_x_dev)<0.01 && __abs2(my_y_dev-other_y_dev)<0.01 ) 
// 		return false;
// 	
// 	if (my_x_dev<other_x_dev) return true;
// 	if (my_x_dev>other_x_dev) return false;
// 	if (my_y_dev<other_y_dev) return true;
// 	if (my_y_dev>other_y_dev) return false;
// 	return false;
// }

point 
point::distance_from(const point& other) const {
	return point(other.x-x,other.y-y);
}

// point 
// curve_def::start() const {
// 	return _a;
// }
// 
// point 
// curve_def::end() const {
// 	if (_is_bezier)
// 		return _d;
// 	else
// 		return _b;
// }
// 
// curve_def::curve_def(const point& a, const point& b, const point& c, const point& d) : _a(a), _b(b), _c(c), _d(d), _is_bezier(true) { }
// 
// curve_def::curve_def(const point& a, const point& b):_a(a), _b(b), _c(), _d(), _is_bezier(false) { }
