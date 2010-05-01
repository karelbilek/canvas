#include "all_shapes.h"
#include "geom_line.h"
#include <sstream>

using namespace libcan;
using namespace std;

#define __sizeofarray(array) ((sizeof(array)/sizeof(string)))
#define __properties(name, ...) vector<string> name::get_specific_properties() {string arr[] = {__VA_ARGS__};return vector<string>(arr, arr+sizeof(arr)/sizeof(string));}


__properties(disk, "center_x", "center_y", "radius");

__properties(segment, "a_x", "a_y", "b_x", "b_y");
__properties(bezier_segment, "a_x", "a_y", "b_x", "b_y", "c_x", "c_y", "d_x", "d_y");
__properties(regular, "a_x", "a_y", "b_x", "b_y", "n");
__properties(rectangle, "a_x", "a_y", "b_x", "b_y", "p_x", "p_y");
__properties(elipse, "s_x", "s_y", "a_x", "a_y", "p_x", "p_y");
__properties(regular_from_center, "c_x", "c_y", "a_x", "a_y", "n");

vector<string> polygon::get_specific_properties() {
	vector<string> res;
	
	for (size_t i = 0; i < res.size(); ++i) {
		string x,y;
		stringstream ss;
		ss<<"a"<<i+1<<"_x";
		ss>>x;
		ss<<"a"<<i+1<<"_y";
		ss>>y;
		res.push_back(x);
		res.push_back(y);
	}
	
	return res;
}


disk::disk(const point& center, const libcan_float radius): 
 shape_type(1,1),
 _center(center),
 _radius(radius) {
	_curves.push_back(new circle(center, radius));
}




disk::disk(const point& center, const point& ref_point):
 shape_type(1,1),
 _center(center),
 _radius(geom_line(center, ref_point).length()) {
	_curves.push_back(new circle(center, _radius));
}
	
segment::segment(const libcan_float ax, const libcan_float ay, const libcan_float bx, const libcan_float by): 
 shape_type(0,0),
 _a(point(ax,ay)),
 _b(point(bx,by)) {
	_curves.push_back(new line(_a, _b));
} 

segment::segment(const point& a, const point& b): 
 shape_type(0,0),
 _a(a),
 _b(b) {
	_curves.push_back(new line(a,b));
}


	
bezier_segment::bezier_segment(const point& a, const point& b, const point& c, const point& d): 
 shape_type(0,0),
 _a(a),
 _b(b),
 _c(c),
 _d(d) {
	_curves.push_back(new bezier(a,b,c,d));
} 

polygon::polygon(const point& a, const point& b, const point&c):
 shape_type(1,1),
 _points() {
	
	_points.push_back(a);
	_points.push_back(b);
	_points.push_back(c);
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,a));
}
		
polygon::polygon(const point& a, const point& b, const point& c, const point& d):
 shape_type(1,1),
 _points() {
	_points.push_back(a);
	_points.push_back(b);
	_points.push_back(c);
	_points.push_back(d);
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,a));
}

polygon::polygon(const point& a, const point& b, const point& c, const point& d, const point& e):
 shape_type(1,1),
 _points() {
	_points.push_back(a);
	_points.push_back(b);
	_points.push_back(c);
	_points.push_back(d);
	_points.push_back(e);
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,e));
	_curves.push_back(new line(e,a));
}

rectangle::rectangle(const point& a, const point& b, const point& v):
 shape_type(1,1),
 _a(a),
 _b(b),
 _v(v){
	
	geom_line me(a,b);
	point c = me.right_angle_b(v);
	
	point d = me.reverted().move_point(c);
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,a));
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


polygon::polygon(const point& a, const point& b, const point& c, const point& d, const point& e, const point& f):
 shape_type(1,1),
 _points() {
	
	_points.push_back(a);
	_points.push_back(b);
	_points.push_back(c);
	_points.push_back(d);
	_points.push_back(e);
	_points.push_back(f);
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,e));
	_curves.push_back(new line(e,f));
	_curves.push_back(new line(f,a));
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


polygon::polygon(const std::vector<point>& points):
 shape_type(1,1),
 _points(points) {
	
	std::vector<point>::const_iterator i = points.begin();
	
	point a = *i;
	point first=a;
	
	++i;
	
	for (; i!=points.end(); ++i) {
		point b = *i;
		_curves.push_back(new line(a,b));
		a=b;
	}
	_curves.push_back(new line(a,first));
	
}

polygon::polygon(const std::list<point>& points):
 shape_type(1,1),
 _points(points.begin(), points.end()) {
	
	std::list<point>::const_iterator i = points.begin();
	
	point a = *i;
	point first=a;
	
	++i;
	
	for (; i!=points.end(); ++i) {
		point b = *i;
		_curves.push_back(new line(a,b));
		a=b;
	}
	_curves.push_back(new line(a,first));
	
}