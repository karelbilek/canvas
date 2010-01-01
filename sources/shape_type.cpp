#include "shape_type.h"
//vypada mozna inteligentne, ale jelikoz veskera funkcionalita je stejne prenasena na 
//krivky, necham bez komentare

using namespace libcan;
using namespace std;

shape_type::
shape_type( const bool filled, const bool joined_ends, const list<curve*>& curves):
  _curves(curves),	//POZOR - cekam, ze kdo vytvarel, dal tam NEW - tj. ja to pak odalokuju
  _filled(filled),
  _joined_ends(joined_ends) {
}

shape_type::
shape_type(const shape_type& other) :
  _curves(),
  _filled(other._filled),
  _joined_ends(other._joined_ends) {
	for (list<curve*>::const_iterator i = other._curves.begin(); i!= other._curves.end(); ++i) {
		_curves.push_back((**i).clone());
	}
}

shape_type::
shape_type() :
  _curves(),
  _filled(),
  _joined_ends() {
}

void
shape_type::rotate(libcan_float angle) {
	point c = center();
	
	for (list<curve*>::iterator i = _curves.begin();i!=_curves.end(); ++i) {
		(**i).rotate(c, angle);
	}
}

void
shape_type::resize(libcan_float quoc) {
	point c = center();
	
	for (list<curve*>::iterator i = _curves.begin();i!=_curves.end(); ++i) {
		(**i).resize(c, quoc);
	}
}

void 
shape_type::move(const point& where){
	point c = center();
	
	for (list<curve*>::iterator i = _curves.begin();i!=_curves.end(); ++i) {
		(**i).move(where);
	}
}


point 
shape_type::center() const {
	libcan_int min_x, max_x, min_y, max_y;
	
	get_extremes(min_x,max_x,min_y,max_y);
	
	return point((min_x+max_x)/2, (min_y+max_y)/2);
}

void
shape_type::get_extremes(libcan_int& min_x,libcan_int& max_x,libcan_int& min_y,libcan_int& max_y) const{
	std::list<curve*>::const_iterator i = _curves.begin();	
	std::list<curve*>::const_iterator end = _curves.end();
	
	min_x = (**i).get_min_x();
	max_x = (**i).get_max_x();
	min_y = (**i).get_min_y();
	max_y = (**i).get_max_y();

	for (++i;i!=end; ++i) {
		min_x = __minimum((**i).get_min_x(), min_x);
		max_x = __maximum((**i).get_max_x(), max_x);
		min_y = __minimum((**i).get_min_y(), min_y);
		max_y = __maximum((**i).get_max_y(), max_y);
	}
}

shape_type& 
shape_type::operator=(const shape_type& other) {
	for (list<curve*>::iterator i = _curves.begin(); i!= _curves.end(); ++i) {
		curve* point = *i;
		delete point;
	}
	
	_curves = list<curve*>();
	
	_filled = other._filled;
	_joined_ends = other._joined_ends;
	for (list<curve*>::const_iterator i = other._curves.begin(); i!= other._curves.end(); ++i) {
		_curves.push_back((**i).clone());
	}
	return *this;
}

shape_type::
~shape_type() {
	for (list<curve*>::iterator i = _curves.begin(); i!= _curves.end(); ++i) {
		curve* point = *i;
		delete point;
	}
}

shape_type* 
shape_type::clone_double() const {
	list<curve*> new_curves;
	for (list<curve*>::const_iterator i = _curves.begin(); i!= _curves.end(); ++i) {
		new_curves.push_back((**i).clone_double());
	}
	shape_type* copy=new shape_type(_filled, _joined_ends, new_curves);
	return copy;
}

list<moved_arrays> 
shape_type::all_curve_arrays() const {
	list<moved_arrays> arrays;
	for(list<curve*>::const_iterator i = _curves.begin(); i!=_curves.end(); ++i) {
		list<moved_arrays> to_add = (**i).get_arrays();

		arrays.splice(arrays.end(), to_add); 
	}
	
	return arrays;
}
