#include "curves/line.h"
#include "all_shapes.h"
#include "geom_line.h"

using namespace libcan;
using namespace std;

line* 
line::clone() const {line* n= new line(_a,_b);return n;}

line* 
line::clone_double() const {line* n= new line(_a*2,_b*2);return n;}

bool 
line::have_thick_line() const {return 1;}


line::line(point a, point b) : 
  _a(a),
  _b(b) {
}

void
line::rotate(const point& center, const libcan_float angle){
	_a = geom_line(center,_a).rotate_fixed_a(angle).b;
	_b = geom_line(center,_b).rotate_fixed_a(angle).b;
}

void 
line::resize(const point& center, const libcan_float quoc){
	_a = geom_line(center,_a).resize(quoc).b;
	_b = geom_line(center,_b).resize(quoc).b;
}

void 
line::move(const point& where){
	_a = geom_line(point(0,0), where).move_point(_a);
	_b = geom_line(point(0,0), where).move_point(_b);
}

list<moved_arrays>
line::get_arrays() {
	//algoritmus odsud http://cgg.mff.cuni.cz/~pepca/lectures/npgr003.html
	//autor algoritmu Josef Pelikan
		
		//kreslim vzdycky podel delsiho rozmeru, a od mensiho cisla k vetsimu
	bool is_width_bigger = (__abs(_a.x-_b.x) > __abs(_a.y-_b.y));
	bool is_switch_points = ((is_width_bigger)?(_b.x>_a.x):(_b.y>_a.y));
	
	
			//kde zacinam a koncim
	point begin_p = (is_switch_points)?(_a.trunc()):(_b.trunc());
	point end_p = (is_switch_points)?(_b.trunc()):(_a.trunc());
	
				//zveda se ten druhy rozmer nebo klesa?
	bool increasing = (is_width_bigger ? ((end_p.y-begin_p.y)>0) : ((end_p.x-begin_p.x)>0));
	
				//vysledky
	moved_arrays res(__minimum(begin_p.y,end_p.y),__maximum(begin_p.y, end_p.y));
	
	
				//ta mensi velikost
	libcan_int lower_size = static_cast<libcan_int>(is_width_bigger?__abs(begin_p.y-end_p.y):__abs(begin_p.x-end_p.x));
	
				//ta vetsi velikost
	libcan_int bigger_size = static_cast<libcan_int>(is_width_bigger?__abs(begin_p.x-end_p.x):__abs(begin_p.y-end_p.y));
	
	
	libcan_int D = 2*lower_size - bigger_size;
	libcan_int inc0 = 2*lower_size;
	libcan_int inc1 = 2*(lower_size - bigger_size);
	
			//timhle se sunu
	point moving = begin_p;
	res.set(moving.x, moving.y);
	
	
			//tohle sou reference, tj staci zvysovat nebo snizovat je a meni se to i v moving
	libcan_float& moving_bigger = is_width_bigger?moving.x:moving.y;
	libcan_float& moving_lower = is_width_bigger?moving.y:moving.x;
	

	
	for (libcan_int i = 0; i < bigger_size; ++i) {
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


libcan_int line::get_min_y() const {return __minimum(_a.y,_b.y)-5;}
libcan_int line::get_max_y() const {return __maximum(_a.y,_b.y)+2;}

libcan_int line::get_min_x() const {return __minimum(_a.x,_b.x)-5;}
libcan_int line::get_max_x() const {return __maximum(_a.x,_b.x)+2;}

shape_type
line::get_thick_line(const libcan_float thickness, const curve* const previous, const curve* const next) const{
	point a;
	point b;
	point c;
	point d;
	
	geom_line my_geom_line =(geom_line(_a,_b));
		
		//je predchozi vubec cara?
	if (const line* previous_line = dynamic_cast<const line*>(previous)) {
		
		geom_line previous_geom_line = (geom_line(previous_line->_a, previous_line->_b));
		
		geom_line res = previous_geom_line.thick_cover(my_geom_line, thickness/2, false);

		b=res.a;
		a=res.b;
	} else {
		a = my_geom_line.right_angle_a(1,thickness/2);
		b = my_geom_line.right_angle_a(0,thickness/2);
	}
	
	if (const line* next_line = dynamic_cast<const line*>(next)) {
	
		geom_line next_geom_line = (geom_line(next_line->_a, next_line->_b));
		
		geom_line res = my_geom_line.thick_cover(next_geom_line, thickness/2, true);
	
		c = res.a;
		d = res.b;
	} else {
		c = my_geom_line.right_angle_b(1,thickness/2);
		d = my_geom_line.right_angle_b(0,thickness/2);
	}
	
	return polygon(a,b,c,d);
}


