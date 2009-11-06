#include "shape_type.h"

using namespace glib;
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

shape_type& 
shape_type::operator=(const shape_type& other) {
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
