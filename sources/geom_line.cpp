#include "geom_line.h"
#include <math.h>


using namespace libcan;
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
geom_line::parallel(const bool left, const libcan_float distance) const {
	return geom_line(right_angle_a(left, distance), right_angle_b(!left,distance));
}

point
geom_line::intersection(const geom_line& another)const {
	
	
	
	if (__abs(b.y - a.y)<0.1) {
		
		libcan_float another_dxy = (another.b.x - another.a.x)/(another.b.y - another.a.y);
		libcan_float diff = b.y - another.b.y;
		return point(another.b.x + diff*another_dxy, another.b.y + diff);
		
	} else if (__abs(another.b.y - another.a.y)<0.1) {
		
		libcan_float me_dxy = (b.x - a.x)/(b.y - a.y);
		libcan_float diff = another.b.y - b.y;
		return point(b.x + diff*me_dxy, b.y + diff);
		
	} else {
		
		//vezmu si caru jako funkci podle y, f(y)=ky+c
		
		libcan_float k = (b.x-a.x)/(b.y-a.y);
		libcan_float c = a.x-k*(a.y);
		libcan_float another_k = (another.b.x-another.a.x)/(another.b.y-another.a.y);
		libcan_float another_c = another.a.x-another_k*(another.a.y);
		
		libcan_float y=(another_c-c)/(k-another_k);
		libcan_float x=k*y+c;
		
		return point(x,y);
		
	}
	
}

geom_line 
geom_line::reverted() const{
	return geom_line(b,a);
}

libcan_float 
geom_line::count_rev_angle(const geom_line another) const{

	libcan_float first = another.reverted().angle_from_x();

	libcan_float second = angle_from_x();


	libcan_float res = first-second;

	if (__abs(res)>180) {
		libcan_float res2=360-__abs(res);
		if (res>0){res=-res2;} else {res=res2;}
	}
	return res;
}

libcan_float 
geom_line::angle_from_x() const{
	if (__abs(a.x-b.x)<0.1) {
		return (a.y<b.y)?(270):(90);
	}
	
	if (__abs(a.y-b.y)<0.1) {
		return (a.x<b.x)?(0):(180);
	}
	
		//ty Y jsou "obracene", protoze Y je v souradicich na druhou stranu nez X
	libcan_float res;
	if (b.x > a.x) {
		
		res=__RAD2DEG(atan((a.y-b.y) / (b.x-a.x)));
	} else {
		res = 180-__RAD2DEG(atan((b.y-a.y) / (b.x-a.x)));
	}
	while (res<0) res+=360;
	while (res>360) res-=360;
	return res;
}

geom_line
geom_line::thick_cover(const geom_line& another, const libcan_float distance, bool second_hint) const{
	geom_line my_right_parallel = parallel(false, distance);
	geom_line another_right_parallel = another.parallel(false,distance);
	
	
	geom_line my_left_parallel =  parallel(true, distance);
	geom_line another_left_parallel = another.parallel(true,distance);
	
	libcan_float angle = count_rev_angle(another);
	
	point left_intersection;
	point right_intersection;
	
	if (__abs(angle)<25){
			//useknute
		left_intersection = my_left_parallel.intersection(another_right_parallel);
		right_intersection = my_right_parallel.intersection(another_left_parallel);
		if (!second_hint) {
			return geom_line(right_intersection, left_intersection);
		}
	} else {
		left_intersection = my_left_parallel.intersection(another_left_parallel);
		right_intersection = my_right_parallel.intersection(another_right_parallel);
	}

	
	return geom_line(left_intersection, right_intersection);
}

geom_line 
geom_line::normalised(const libcan_float length) const {
	libcan_float my_width = (b.x-a.x);
	libcan_float my_height = (b.y-a.y);
	
	libcan_float my_length = static_cast<libcan_float>( sqrt(static_cast<double>(my_width*my_width + my_height*my_height)));
	
	libcan_float quoc = (length/my_length);
	
	return geom_line(a, point(a.x+quoc*my_width, a.y+quoc*my_height));
}

point
geom_line::right_angle_a(bool clockwise, const libcan_float length) const {
	libcan_float my_width = b.x - a.x;
	libcan_float my_height = b.y - a.y;
	geom_line res;
	if (!clockwise) {
		res= geom_line(a, point(a.x-my_height, a.y + my_width));
	} else {
		res= geom_line(a, point(a.x + my_height, a.y - my_width));
	}
	return res.normalised(length).b;
}


point
geom_line::right_angle_b(bool clockwise, const libcan_float length) const {
	libcan_float my_width = b.x - a.x;
	libcan_float my_height = b.y - a.y;
	geom_line res;
	
	if (clockwise) {
		res= geom_line(b, point(b.x-my_height, b.y + my_width));
	} else {
		res= geom_line(b, point(b.x + my_height, b.y - my_width));
	}
	return res.normalised(length).b;
	
}

