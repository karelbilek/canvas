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
	
		//kreslim vzdycky podel delsiho rozmeru, a od mensiho cisla k vetsimu
	bool is_width_bigger = (__abs(_a.x-_b.x) > __abs(_a.y-_b.y));
	bool is_switch_points = ((is_width_bigger)?(_b.x>_a.x):(_b.y>_a.y));
	
	
			//kde zacinam a koncim
	point begin_p = (is_switch_points)?(_a.trunc()):(_b.trunc());
	point end_p = (is_switch_points)?(_b.trunc()):(_a.trunc());
	
				//zveda se ten druhy rozmer nebo klesa?
	bool increasing = (is_width_bigger ? ((end_p.y-begin_p.y)>0) : ((end_p.x-begin_p.x)>0));
	
	moved_arrays res(__minimum(begin_p.y,end_p.y),__maximum(begin_p.y, end_p.y));
	
	glib_int lower_size = static_cast<glib_int>(is_width_bigger?__abs(begin_p.y-end_p.y):__abs(begin_p.x-end_p.x));
	
	glib_int bigger_size = static_cast<glib_int>(is_width_bigger?__abs(begin_p.x-end_p.x):__abs(begin_p.y-end_p.y));
	
	
	glib_int D = 2*lower_size - bigger_size;
	glib_int inc0 = 2*lower_size;
	glib_int inc1 = 2*(lower_size - bigger_size);
	
	
	point moving = begin_p;
	res.set(moving.x, moving.y);
	
	glib_float& moving_bigger = is_width_bigger?moving.x:moving.y;
	glib_float& moving_lower = is_width_bigger?moving.y:moving.x;
	

	
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


glib_int line::get_min_y() const {return __minimum(_a.y,_b.y)-5;}
glib_int line::get_max_y() const {return __maximum(_a.y,_b.y)+2;}

glib_int line::get_min_x() const {return __minimum(_a.x,_b.x)-5;}
glib_int line::get_max_x() const {return __maximum(_a.x,_b.x)+2;}

shape_type 
line::get_thick_line(const glib_float thickness, const curve* const previous, const curve* const next) const{
	point a;
	point b;
	point c;
	point d;
	
	geom_line my_geom_line =(geom_line(_a,_b));
	
	if (const line* previous_line = dynamic_cast<const line*>(previous)) {
		
		geom_line previous_geom_line = (geom_line(previous_line->_a, previous_line->_b));
		
		geom_line res = previous_geom_line.thick_cover(my_geom_line, thickness/2,true);

		b=res.a;
		a=res.b;
	} else {
		a = my_geom_line.right_angle_a(1,thickness/2);
		b = my_geom_line.right_angle_a(0,thickness/2);
	}
	
	if (const line* next_line = dynamic_cast<const line*>(next)) {
	
		geom_line next_geom_line = (geom_line(next_line->_a, next_line->_b));
		
		geom_line res = my_geom_line.thick_cover(next_geom_line, thickness/2,false);
	
		c = res.a;
		d = res.b;
	} else {
		c = my_geom_line.right_angle_b(1,thickness/2);
		d = my_geom_line.right_angle_b(0,thickness/2);
	}
	
	return polygon(a,b,c,d);
}


