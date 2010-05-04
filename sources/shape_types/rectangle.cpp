#include "shape_types/rectangle.h"

using namespace libcan;
using namespace std;


__shape_type_properties(rectangle, "a_x", "a_y", "b_x", "b_y", "p_x", "p_y");

shape_type* 
rectangle::clone() const {
	return new rectangle(_a, _b, _p);
}

shape_type* 
rectangle::new_with_property(const string& property, const string& what, const libcan_int what_int, const libcan_float what_float){
	if (property=="a_x") {
		_a.x = what_int;
	} else if (property=="a_y") {
		_a.y = what_int;
	} else if (property=="b_x") {
		_b.x = what_int;
	} else if (property=="b_y") {
		_b.y = what_int;
	} else if (property=="p_x") {
		_p.x = what_int;
	} else if (property=="p_y") {
		_p.y = what_int;
	}
	return new rectangle(_a, _b, _p);
}

void 
rectangle::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="a_x") {
		where<<_a.x;
	} else if (property=="a_y") {
		where<<_a.y;
	} else if (property=="b_x") {
		where<<_b.x;
	} else if (property=="b_y") {
		where<<_b.y;
	} else if (property=="p_x") {
		where<<_p.x;
	} else if (property=="p_y") {
		where<<_p.y;
	} else if (property=="name") {
		where << "rectangle with side "<<_a.x<<","<<_a.y<<"] - ["<<_b.x<<","<<_b.y<<"]"; 
	}
}

rectangle::rectangle(const point& a, const point& b, const point& p):
 shape_type(1,1),
 _a(a),
 _b(b),
 _p(p){
	
	geom_line me(a,b);
	point c = me.right_angle_b(p);
	
	point d = me.reverted().move_point(p);
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,a));
}

regular::regular(const point& a, const point& b, int n):
 shape_type(1,1),
 _a(a),
 _b(b),
 _n(n) {
	libcan_float angle = (180-360/n);
	libcan_float length = geom_line(a,b).length();
	
	point aa = a;
	point bb = b;
	
	for (int i=0; i<n-1; i++){
		_curves.push_back(new line(aa,bb));
		point c = geom_line(aa,bb).line_from_rev_angle(angle, length).b;
		aa=bb;
		bb=c;
	}
	
	_curves.push_back(new line(aa,a));
			//nepresnosti se kumuluji, tak o pixel to vyjde jinak, ale to uz dela potize!
	
}

regular_from_center::regular_from_center(const point& c, const point& a, int n):
 shape_type(1,1),
 _c(c),
 _a(a),
 _n(n) {
	libcan_float angle = (180-360/n);
	
	geom_line me(c, a);
	
	libcan_float length = cos(__DEG2RAD(angle/2))*2*( me.length());
	geom_line l = me.line_from_rev_angle(angle/2, length);
	
	point aa = l.a;
	point bb = l.b;
	
	for (int i=0; i<n-1; i++){
		_curves.push_back(new line(aa,bb));
		point c = geom_line(aa,bb).line_from_rev_angle(angle, length).b;
		aa=bb;
		bb=c;
	}
	
	_curves.push_back(new line(aa,l.a));	
	
	
}