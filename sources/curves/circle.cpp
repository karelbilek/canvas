#include "curves/circle.h"
#include "shape.h"

using namespace libcan;
using namespace std;

circle::circle(point center, double radius) :
  _center(center),
  _radius(radius) {
}

circle* 
circle::clone() const {circle* n= new circle(_center,_radius);return n;}

circle* 
circle::clone_double() const {circle* n= new circle(_center*2,_radius*2);return n;}


list<moved_arrays> 
circle::get_arrays() {
	//algoritmus odsud http://cgg.mff.cuni.cz/~pepca/lectures/npgr003.html
	//autor algoritmu Josef Pelikan

	std::list<moved_arrays> res;
	
		//specialni pripady beru extra
	if (_radius <= 0.5) {
		
		moved_arrays one(_center.y, _center.y);
		one.set(_center.x, _center.y);
		
		res.push_back(one);
	}
	else if (_radius <= 1) {
		moved_arrays one(_center.y, _center.y+1);
		
		one.set(_center.x, _center.y);
		one.set(_center.x+1, _center.y);
		one.set(_center.x, _center.y+1);
		one.set(_center.x+1, _center.y+1);

		
		res.push_back(one);
	}
	else if (_radius <= 1.5) {
		moved_arrays small(_center.y-1, _center.y+1);
		
		small.set(_center.x, _center.y);
		small.set(_center.x+1, _center.y);
		small.set(_center.x-1, _center.y);
		small.set(_center.x, _center.y+1);
		small.set(_center.x, _center.y-1);
		
		res.push_back(small);
	} 
	else if (_radius<=2.5) {
		moved_arrays left(_center.y-1, _center.y+1);
		moved_arrays right(_center.y-1, _center.y+1);
		
		left.set(_center.x-1,_center.y-1);
		left.set(_center.x-2,_center.y);
		left.set(_center.x-2,_center.y+1);
		left.set(_center.x-1,_center.y+2);
		
		right.set(_center.x,_center.y-1);
		right.set(_center.x+1,_center.y);
		right.set(_center.x+1,_center.y+1);
		right.set(_center.x,_center.y+2);
		res.push_back(left);
		res.push_back(right);
		
	} 
	else {
		//nespecialni pripad
		double lx = 0;
		double ly = _radius;
		double d = 1 - ly;
	
		moved_arrays left(_center.y-_radius, _center.y+_radius+1);
		moved_arrays right(_center.y-_radius, _center.y+_radius+1);
	
	
		paint_more(0,ly,left,right);
		while (ly>=lx) {
			if (d<0) {
				d = d + 2*lx + 3;
			} else {
				d = d + 2*(lx-ly) + 5;
				--ly;
			}
			
			++lx;
			
			paint_more(lx,ly,left,right);
		} //^^ algoritmus ze slidu :)
		
		res.push_back(left);
		res.push_back(right);
		
	}

	
	return res;
}

bool 
circle::have_thick_line() const {return 1;}

void 
circle::rotate(const point& center, const double angle){
	if (center!=_center) {
		_center = geom_line(center, _center).rotate_fixed_a(angle).b;
	}
};
		//rotating circle.... does exactly nothing.

void 
circle::resize(const point& center, const double quoc){
	if (center!=_center) {
		_center = geom_line(center, _center).resize(quoc).b;
	}
	
	_radius = _radius*quoc;
}

void 
circle::move(const point& where){
	_center = geom_line(point(0,0), where).move_point(_center);
}


void 
circle::paint_more(double fx, double fy, moved_arrays& left, moved_arrays& right){
	long x = static_cast<long>(fx+0.5);
	long y = static_cast<long>(fy+0.5);
	left.set(_center.x-x, _center.y+y+1);
	left.set(_center.x-y, _center.y+x+1);
	left.set(_center.x-x, _center.y-y);
	left.set(_center.x-y, _center.y-x);
	
	right.set(_center.x+x, _center.y+y+1);
	right.set(_center.x+y, _center.y+x+1);
	right.set(_center.x+x, _center.y-y);
	right.set(_center.x+y, _center.y-x);
}

long 
circle::get_min_y() const {
	return static_cast<long>(_center.y - _radius);
}

long 
circle::get_max_y() const {
	return static_cast<long>(_center.y + _radius+2);
}


long 
circle::get_min_x() const {
	return static_cast<long>(_center.x - _radius);
}

long 
circle::get_max_x() const {
	return static_cast<long>(_center.x + _radius+2);
}

shape_type
circle::get_thick_line(const double thickness, const curve* const previous, const curve* const next) const{
	//nakresli se jako dva soustredne kruhy
	circle* circle_in= new circle(_center, _radius-(thickness/2));
	circle* circle_out= new circle(_center, _radius+(thickness/2));
	list<curve*> res;
	res.push_back(circle_in);
	res.push_back(circle_out);
	return shape_type(1,0,res);
}
