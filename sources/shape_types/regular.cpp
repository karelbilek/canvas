#include "shape_types/regular.h"

using namespace libcan;
using namespace std;


__shape_type_properties(regular, "a_x", "a_y", "b_x", "b_y", "n");

shape_type* 
regular::clone() const {
	return new regular(_a, _b, _n);
}

shape_type* 
regular::new_with_property(const string& property, stringstream& what){
	if (property=="a_x") {
		what >> _a.x;
	} else if (property=="a_y") {
		what >> _a.y;
	} else if (property=="b_x") {
		what >> _b.x;
	} else if (property=="b_y") {
		what >> _b.y;
	} else if (property=="n") {
		what >> _n;
	}
	return new regular(_a, _b, _n);
}

void 
regular::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="a_x") {
		where<<_a.x;
	} else if (property=="a_y") {
		where<<_a.y;
	} else if (property=="b_x") {
		where<<_b.x;
	} else if (property=="b_y") {
		where<<_b.y;
	} else if (property=="n") {
		where<<_n;
	} else if (property=="name") {
		where << "regular "<<_n<<"-gon"; 
	}
}



geom_line
regular::help(const point&a, const point& b, int n) {
	double angle = (180-360/n);
	
	geom_line me(a, b);
	
	double length = cos(__DEG2RAD(angle/2))*2*( me.length());
	return me.line_from_rev_angle(angle/2, length);
}

regular::regular(const point& a, const point& b, const int n, const bool from_center):
 shape_type(1,1), 
 _a(from_center ? help(a,b,n).a : a),
 _b(from_center ? help(a,b,n).b : b),
 _n(n) {
	double angle = (180-360/n);
	double length = geom_line(a,b).length();

	point aa = a;
	point bb = b;

	for (int i=0; i<n-1; i++){
		_curves.push_back(new line(aa,bb));
		point c = geom_line(aa,bb).line_from_rev_angle(angle, length).b;
		aa=bb;
		bb=c;
	}

	_curves.push_back(new line(aa,a));
}

