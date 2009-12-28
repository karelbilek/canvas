#include "curves/line.h"
#include "all_shapes.h"
#include "geom_line.h"

using namespace glib;
using namespace std;

line::line(point a, point b) : 
  _a(a),
  _b(b) {
}


list<moved_arrays>
line::get_arrays() {
	//algoritmus nekde odsud http://cgg.ms.mff.cuni.cz/~pepca/, zrovna to nebezi
	
	bool width_bigger = (__abs(_a.x-_b.x) > __abs(_a.y-_b.y));
	
	bool switch_points = ((width_bigger)?(_b.x>_a.x):(_b.y>_a.y));
	
	point begin_p = (switch_points)?(_a.trunc()):(_b.trunc());
	point end_p = (switch_points)?(_b.trunc()):(_a.trunc());
	
	bool increasing = (width_bigger ? ((end_p.y-begin_p.y)>0) : ((end_p.x-begin_p.x)>0));
	
	moved_arrays res(__minimum(begin_p.y,end_p.y),__maximum(begin_p.y, end_p.y));
	
	glib_int lower_size = static_cast<glib_int>(width_bigger?__abs(begin_p.y-end_p.y):__abs(begin_p.x-end_p.x));
	
	glib_int bigger_size = static_cast<glib_int>(width_bigger?__abs(begin_p.x-end_p.x):__abs(begin_p.y-end_p.y));
	
	
	glib_int D = 2*lower_size - bigger_size;
	glib_int inc0 = 2*lower_size;
	glib_int inc1 = 2*(lower_size - bigger_size);
	
	
	point moving = begin_p;
	res.set(moving.x, moving.y);
	
	glib_float& moving_bigger = width_bigger?moving.x:moving.y;
	glib_float& moving_lower = width_bigger?moving.y:moving.x;
	

	
	for (glib_int i = 0; i < bigger_size; ++i) {
		if (D<=0){
			D+=inc0;
		} else {
			D+=inc1;
			moving_lower += increasing?(1):(-1);
		}
		++moving_bigger;
		res.set(moving.x, moving.y);
	}
	res.set(end_p.x, end_p.y);
	
	list<moved_arrays> res2;
	res2.push_back(res);
	
	return res2;
	
}


glib_int line::get_min_y() const {return __minimum(_a.y,_b.y);}
glib_int line::get_max_y() const {return __maximum(_a.y,_b.y)+2;}

glib_int line::get_min_x() const {return __minimum(_a.x,_b.x);}
glib_int line::get_max_x() const {return __maximum(_a.x,_b.x)+2;}

shape_type 
line::get_thick_line(const glib_float thickness, const curve* const previous, const curve* const next) const{
	point a;
	point b;
	point c;
	point d;
	
	if (const line* previous_line = dynamic_cast<const line*>(previous)) {
		geom_line res = (geom_line(_a,_b)).parallel_intersection(geom_line(previous_line->_a, previous_line->_b),thickness/2);
		//prvni vrati VLEVO, pak VPRAVO
		a=res.a;
		b=res.b;
	} else {
		geom_line this_gl = geom_line(_a, _b);
		a = this_gl.right_angle_a(1,thickness/2);
		b = this_gl.right_angle_a(0,thickness/2);
	}
	
	if (const line* next_line = dynamic_cast<const line*>(next)) {
		std::cout<<"WELCOME TO THE maSChiENEEEEEE\n========\n";
		geom_line res = (geom_line(_a,_b)).parallel_intersection(geom_line(next_line->_a, next_line->_b),thickness/2);
		std::cout<<"zde vznika c jako intersection paralelu usecky, dane body _a:"<<_a.x<<","<<_a.y<<" a _b:"<<_b.x<<","<<_b.y<<"; a dalsi usecka je dana body _a:"<<(next_line->_a).x<<","<<(next_line->_a).y<<" a _b:"<<(next_line->_b).x<<","<<(next_line->_b).y<<" s tloustkou "<<thickness/2<<", resp. jako jeden z bodu.\n=====\n";
		c = res.a;
		d = res.b;
	} else {
		geom_line this_gl = geom_line(_a, _b);
		d = this_gl.right_angle_b(1,thickness/2);
		c = this_gl.right_angle_b(0,thickness/2);
	}
	
	return polygon(a,b,c,d);
}


