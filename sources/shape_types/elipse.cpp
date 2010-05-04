#include "shape_types/elipse.h"

using namespace libcan;
using namespace std;


__shape_type_properties(elipse, "s_x", "s_y", "a_x", "a_y", "v_x", "v_y");

shape_type* 
elipse::clone() const {
	return new elipse(_s, _a, _v);
}

shape_type* 
elipse::new_with_property(const string& property, const string& what, const libcan_int what_int, const libcan_float what_float){
	if (property=="a_x") {
		_a.x = what_int;
	} else if (property=="a_y") {
		_a.y = what_int;
	} else if (property=="s_x") {
		_s.x = what_int;
	} else if (property=="s_y") {
		_s.y = what_int;
	} else if (property=="v_x") {
		_v.x = what_int;
	} else if (property=="v_y") {
		_v.y = what_int;
	} 
	return new elipse(_s, _a, _v);
}

void 
elipse::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="a_x") {
		where << _a.x;
	} else if (property=="a_y") {
		where << _a.y;
	} else if (property=="s_x") {
		where << _s.x;
	} else if (property=="s_y") {
		where << _s.y;
	} else if (property=="v_x") {
		where << _v.x;
	} else if (property=="v_y") {
		where << _v.y;
	} else if (property=="name") {
		where << "elipse with center ["<<_s.x<<","<<_s.y<<"]"; 
	}
}


elipse::elipse(const point& s, const point& a, const point& v):
 shape_type(1,1),
 _s(s),
 _a(a),
 _v(v){
	
	geom_line me=geom_line(a,s);
	
	point b = geom_line(a,s).right_angle_b(v);
	geom_line another(s,b);
	
	point c = me.resize(2).b;
	point d = another.resize(-1).b;

	geom_line another_half=another.resize(0.5);
	geom_line me_half=me.resize(0.5);

	point e = another_half.move_point(a);
	point f = another_half.move_point(c);
	point g = another_half.reverted().move_point(a);
	point h = another_half.reverted().move_point(c);

	point i = me_half.move_point(b);
	point j = me_half.move_point(d);
	point k = me_half.reverted().move_point(b);
	point l = me_half.reverted().move_point(d);
	
	_curves.push_back(new bezier(a,e,k, b));
	_curves.push_back(new bezier(b,i,f, c));
	_curves.push_back(new bezier(c,h,j, d));
	_curves.push_back(new bezier(d,l,g, a));
}
