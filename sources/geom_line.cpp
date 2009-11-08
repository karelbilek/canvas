#include "geom_line.h"
#include <math.h>


using namespace glib;
//using namespace std;

geom_line::geom_line(const point& my_a, const point& my_b):
  a(my_a),
  b(my_b) {	
}

geom_line::geom_line():
  a(0,0),
  b(0,0) {	
}

geom_line
geom_line::parallel(const bool left, const glib_float distance) const {
	return geom_line(right_angle_a(left, distance), right_angle_b(!left,distance));
}

point
geom_line::intersection(const geom_line& another)const {
	
	//a jsem ja, y je ten druhy
	//ay2 = ja.b.y
	//ay1 = ja.a.y
	//ax1 atd
	if (__abs(b.y - a.y)<0.1) {
		
		glib_float another_dxy = (another.b.x - another.a.x)/(another.b.y - another.a.y);
		glib_float diff = b.y - another.b.y;
		return point(another.b.x + diff*another_dxy, another.b.y + diff);
		
	} else if (__abs(another.b.y - another.a.y)<0.1) {
		
		glib_float me_dxy = (b.x - a.x)/(b.y - a.y);
		glib_float diff = another.b.y - b.y;
		return point(b.x + diff*me_dxy, b.y + diff);
		
	} else {
		
		glib_float me_dxy = (b.x - a.x)/(b.y - a.y);
		glib_float another_dxy = (another.b.x - another.a.x)/(another.b.y - another.a.y);
		glib_float h = (another_dxy*(b.y - another.b.y) - (b.x - another.b.x))/(me_dxy - another_dxy);
		return point(b.x + h*me_dxy, b.y + h);
		
	}
	
}

geom_line
geom_line::parallel_intersection(const geom_line& another, const glib_float distance) const{
	return geom_line(parallel(true, distance).intersection(another.parallel(true,distance)), parallel(false, distance).intersection(another.parallel(false,distance)));
}

geom_line 
geom_line::normalised(const glib_float length) const {
	glib_float my_width = __abs(a.x-b.x);
	glib_float my_height = __abs(a.y-b.y);
	
	glib_float my_length = static_cast<glib_float>( sqrt(static_cast<double>(my_width*my_width + my_height*my_height)));
	
	return geom_line(a, point(a.x+(length/my_length)*my_width, b.x+(length/my_length)*my_height));
}

point
geom_line::right_angle_a(bool clockwise, const glib_float length) const {
	glib_float my_width = b.x - a.x;
	glib_float my_height = b.y - a.y;
	geom_line res;
	if (clockwise) {
		res= geom_line(a, point(a.x-my_height, a.y + my_width));
	} else {
		res= geom_line(a, point(a.x + my_height, a.y - my_width));
	}
	return res.normalised(length).b;
}


point
geom_line::right_angle_b(bool clockwise, const glib_float length) const {
	glib_float my_width = b.x - a.x;
	glib_float my_height = b.y - a.y;
	geom_line res;
	
	if (clockwise) {
		res= geom_line(b, point(b.x-my_height, b.y + my_width));
	} else {
		res= geom_line(b, point(b.x + my_height, b.y - my_width));
	}
	return res.normalised(length).b;
	
	/*
	
	
	80
	---------
10	|\
	|		\
		   10
		 |---
	 80	 |  /
		 |
		 |
		 |
		 |/
	
	
	
	
	
	*/
	
}

