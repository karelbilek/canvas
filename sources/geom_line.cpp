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
		
		//vezmu si caru jako funkci podle y, f(y)=ky+c
		
		glib_float k = (b.x-a.x)/(b.y-a.y);
		glib_float c = a.x-k*(a.y);
		glib_float another_k = (another.b.x-another.a.x)/(another.b.y-another.a.y);
		glib_float another_c = another.a.x-another_k*(another.a.y);
		
		glib_float y=(another_c-c)/(k-another_k);
		if (__abs(k - another_k)<0.1) {throw 1;}
		glib_float x=k*y+c;
		
		return point(x,y);
		
	}
	
}

geom_line
geom_line::parallel_intersection(const geom_line& another, const glib_float distance) const{
	geom_line my_right_parallel = parallel(false, distance);
	geom_line another_right_parallel = another.parallel(false,distance);
	point first_intersection = my_right_parallel.intersection(another_right_parallel);
	
	geom_line my_left_parallel =  parallel(true, distance);
	geom_line another_left_parallel = another.parallel(true,distance);
	point second_intersection = my_left_parallel.intersection(another_left_parallel);
	
	return geom_line(first_intersection, second_intersection);
}

geom_line 
geom_line::normalised(const glib_float length) const {
	glib_float my_width = (b.x-a.x);
	glib_float my_height = (b.y-a.y);
	
	glib_float my_length = static_cast<glib_float>( sqrt(static_cast<double>(my_width*my_width + my_height*my_height)));
	
	glib_float quoc = (length/my_length);
	
	return geom_line(a, point(a.x+quoc*my_width, a.y+quoc*my_height));
}

point
geom_line::right_angle_a(bool clockwise, const glib_float length) const {
	glib_float my_width = b.x - a.x;
	glib_float my_height = b.y - a.y;
	geom_line res;
	if (!clockwise) {
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
	point respoint = res.normalised(length).b;
	return respoint;
	
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

