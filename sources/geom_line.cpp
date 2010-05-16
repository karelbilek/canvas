#include "geom_line.h"


#define __abs(x) (fabs(x))

using namespace libcan;

geom_line::geom_line(const point& my_a, const point& my_b):
  a(my_a),
  b(my_b) {	
}

geom_line::geom_line():
  a(0,0),
  b(0,0) {	
}

geom_line
geom_line::parallel(const bool left, const double distance) const {
	return geom_line(right_angle_a(left, distance), right_angle_b(!left,distance));
}

point
geom_line::intersection(const geom_line& another)const {



	if (__abs(b.y - a.y)<0.1) {

		double another_dxy = (another.b.x - another.a.x)/(another.b.y - another.a.y);
		double diff = b.y - another.b.y;
		return point(another.b.x + diff*another_dxy, another.b.y + diff);

	} else if (__abs(another.b.y - another.a.y)<0.1) {

		double me_dxy = (b.x - a.x)/(b.y - a.y);
		double diff = another.b.y - b.y;
		return point(b.x + diff*me_dxy, b.y + diff);

	} else {

		//vezmu si caru jako funkci podle y, f(y)=ky+c

		double k = (b.x-a.x)/(b.y-a.y);
		double c = a.x-k*(a.y);
		double another_k = (another.b.x-another.a.x)/(another.b.y-another.a.y);
		double another_c = another.a.x-another_k*(another.a.y);

		double y=(another_c-c)/(k-another_k);
		double x=k*y+c;

		return point(x,y);

	}

}

geom_line 
geom_line::reverted() const{
	return geom_line(b,a);
}

double 
geom_line::count_rev_angle(const geom_line another) const{

	double first = another.reverted().angle_from_x();

	double second = angle_from_x();


	double res = first-second;

	if (__abs(res)>180) {
		double res2=360-__abs(res);
		if (res>0){res=-res2;} else {res=res2;}
	}
	return res;
}

double 
geom_line::angle_from_x() const{
	if (__abs(a.x-b.x)<0.1) {
		return (a.y<b.y)?(270):(90);
	}

	if (__abs(a.y-b.y)<0.1) {
		return (a.x<b.x)?(0):(180);
	}

		//ty Y jsou "obracene", protoze Y je v souradicich na druhou stranu nez X
	double res;
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
geom_line::thick_cover(const geom_line& another, const double distance, bool second_hint) const{
	geom_line my_right_parallel = parallel(false, distance);
	geom_line another_right_parallel = another.parallel(false,distance);


	geom_line my_left_parallel =  parallel(true, distance);
	geom_line another_left_parallel = another.parallel(true,distance);

	double angle = count_rev_angle(another);

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
geom_line::line_from_rev_angle(const double angle, const double length) const {
	double my_angle = angle_from_x();

	return line_from_point_angle(b, 180+my_angle+angle, length);

}

double 
geom_line::distance(const point& p) const {
	return geom_line(p, intersection(right_angle(1,p,1))).length();
}

geom_line 
geom_line::line_from_point_angle(const point& f, double angle, const double length) {
	while (angle>360) {angle-=360;}
	while (angle<0) {angle+=360;}

	if (__abs(angle-90)<0.1){
		return geom_line(f, point(f.x, f.y-length));
	}

	if (__abs(angle-180)<0.1){
		return geom_line(f, point(f.x-length, f.y));
	}

	if (__abs(angle-270)<0.1){
		return geom_line(f, point(f.x, f.y+length));
	}

	if (__abs(angle)<0.1){
		return geom_line(f, point(f.x+length, f.y));
	}

	if (angle>270 || angle < 90){
		return (geom_line(f,point(f.x+10.0, f.y-10.0*tan(__DEG2RAD(angle)))).normalised(length));
	} else {
		return (geom_line(f,point(f.x-10.0, f.y-10.0*tan(__DEG2RAD(180-angle)))).normalised(length));
	}

}

double
geom_line::length() const {
	double my_width = (b.x-a.x);
	double my_height = (b.y-a.y);

	double my_length = static_cast<double>( sqrt(static_cast<double>(my_width*my_width + my_height*my_height)));
	return my_length;
}

geom_line 
geom_line::normalised(const double new_length) const {

	return resize(new_length/length());
}

geom_line 
geom_line::resize(double quoc) const{
	return geom_line(a, point(a.x+quoc*(b.x-a.x), a.y+quoc*(b.y-a.y)));
}




geom_line 
geom_line::rotate_fixed_a(const double angle) const {
	return line_from_rev_angle((180-angle)/2.0,length()*sin(__DEG2RAD(angle/2.0))*2.0);
		//vypadá jako magie, ale stačí nakreslit na papír :)
}

geom_line 
geom_line::right_angle(const bool clockwise, const point& start, const double length) const {
	double my_width = b.x - a.x;
	double my_height = b.y - a.y;
	geom_line res;

	if (clockwise) {
		res= geom_line(start, point(start.x-my_height, start.y + my_width));
	} else {
		res= geom_line(start, point(start.x + my_height, start.y - my_width));
	}
	return res.normalised(length);
}

point
geom_line::right_angle_a(bool clockwise, const double length) const {
	return right_angle(!clockwise, a, length).b;
}


point
geom_line::right_angle_b(bool clockwise, const double length) const {
	return right_angle(clockwise, b, length).b;
}

point
geom_line::right_angle_b(const point& c) const {
	double angle = count_rev_angle(geom_line(b,c));

	return right_angle(angle>0, b, distance(c)).b;
}

point 
geom_line::move_point(const point& p) const{
	return point(p.x+(b.x-a.x), p.y+(b.y-a.y));
}