#include "shape_types/rectangle.h"

using namespace libcan;
using namespace std;


__shape_type_properties(rectangle, "a_x", "a_y", "b_x", "b_y", "p_x", "p_y");

shape_type* 
rectangle::clone() const {
	return new rectangle(_a, _b, _p);
}

shape_type* 
rectangle::new_with_property(const string& property, stringstream& what){
	if (property=="a_x") {
		what >> _a.x;
	} else if (property=="a_y") {
		what >> _a.y;
	} else if (property=="b_x") {
		what >> _b.x;
	} else if (property=="b_y") {
		what >> _b.y;
	} else if (property=="p_x") {
		what >> _p.x;
	} else if (property=="p_y") {
		what >> _p.y;
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
	
	point d = me.reverted().move_point(c);
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,a));
}

