#include "shape_types/polygon.h"

using namespace libcan;
using namespace std;


__shape_type_properties(disk, "center_x", "center_y", "radius");

shape_type* 
disk::clone() const {
	return new disk(_center, _radius);	
}

shape_type* 
disk::new_with_property(const string& property, const string& what, const libcan_int what_int, const libcan_float what_float){
	if (property=="center_x") {
		_center.x = what_int;
	} else if (property=="center_y") {
		_center.y = what_int;
	} else if (property=="radius") {
		_radius = what_float; 
	}
	return new disk(_center, _radius);
}

void 
disk::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="center_x") {
		where << _center.x;
	} else if (property=="center_y") {
		where << _center.y;
	} else if (property=="radius") {
		where << _radius; 
	} else if (property=="name") {
		where << "disk with center ["<<_center.x<<","<<_center.y<<"]"; 
	}
}

shape_type* 
polygon::clone() const {
	return new polygon(_points);
}

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

void 
polygon::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="name") {
		where << _points.size()<<"-gon"; 
	} else {
		size_t pos;
		string xy;
		istringstream(property.substr(1))>>pos>>xy;
		if (xy=="_x") {
			where << _points[pos].x;
		} else if (xy=="_y") {
			where << _points[pos].y;
		} else {
			throw 1;
		}
	}
}

shape_type* 
polygon::new_with_property(const string& property, const string& what, const libcan_int what_int, const libcan_float what_float) {
	size_t pos;
	string xy;
	istringstream(property.substr(1))>>pos>>xy;
	if (xy=="_x") {
		_points[pos].x = what_int;
	} else if (xy=="_y") {
		_points[pos].y = what_int;
	} else {
		throw 1;
	}
	
	return new polygon(_points);
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