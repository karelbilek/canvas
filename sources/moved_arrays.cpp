#include "moved_arrays.h"

using namespace libcan;

moved_arrays::moved_arrays(long min_y, long max_y)
  : _first(true),
  _min_nonempty_y(0),
  _max_nonempty_y(0),
  _min_y(min_y),
  _max_y(max_y+1),
  _starts(max_y-min_y+2), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(max_y-min_y+2),
  _is_set(max_y-min_y+2),
  _sorting_hint(0){

}

moved_arrays::moved_arrays(double min_y, double max_y)
  : _first(true),
  _min_nonempty_y(0),
  _max_nonempty_y(0),
  _min_y(static_cast<long>(min_y)),
  _max_y(static_cast<long>(max_y+1)),
  _starts(_max_y-_min_y+1), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(_max_y-_min_y+1),
  _is_set(_max_y-_min_y+1),
  _sorting_hint(0) {
}


long
moved_arrays::get_start(const long y) const {
	if ((y < _min_y) || (y > _max_y)) {
		throw 1; 
	}
	return _starts[y - _min_y];
}

long
moved_arrays::get_end(const long y) const {
	if ((y < _min_y) || (y > _max_y)) {
		throw 1; 
	}
	return _ends[y - _min_y];
}

void
moved_arrays::set(const long x, const long y) {
	
	const long my_y = y - _min_y;
	
	if ((y < _min_y) || (y > _max_y)) {
		return;
		//throw 1;
	}
	if (_first) {
		_min_nonempty_y = (_max_nonempty_y = y);
		_first = false;
	} else {

		if (y < _min_nonempty_y) {
			_min_nonempty_y = y;
		}
		if (y > _max_nonempty_y) {
			_max_nonempty_y = y;
		}
	}
	
	if (_is_set[my_y]) {
		if (x < _starts[my_y]) {
			_starts[my_y] = x;
		}
		if (x > _ends[my_y]) {
			_ends[my_y] = x;
		}
	} else {
		_is_set[my_y] = true;
		_starts[my_y] = x;
		_ends[my_y] = x;
	}
}

void
moved_arrays::set(const double x, const double y) {
	set(static_cast<long>(x), static_cast<long>(y));
	
	//set(static_cast<long>(x+0.5), static_cast<long>(y+0.5));
}

plane<bool> 
moved_arrays::to_plane() const {
	plane<bool> res(_min_nonempty_y,_max_nonempty_y+1);
	for (long y = _min_nonempty_y; y<=_max_nonempty_y;++y) {
		if (_is_set[y - _min_y]) {
			res.add_more(_starts[y-_min_y], _ends[y-_min_y],y,true);
		}
	}
	return res;
	
}

bool
moved_arrays::is_set(const long y) const {
	if ((y < _min_y) || (y > _max_y)) {
		return false;
	}
	return _is_set[y - _min_y];
}

bool
moved_arrays::is_near(const long x, const long y) const {
	if ((y < _min_y) || (y > _max_y)) {
		return false;
	}
	const long my_y = y - _min_y;
	const long x_levo = x - _starts[my_y];
	const long x_pravo = _ends[my_y] - x;

	return ((x_levo >= -1) && (x_pravo >= -1));
}

bool 
moved_arrays::is_horizontal() const {
	return (_min_nonempty_y==_max_nonempty_y);
}

long moved_arrays::get_min_nonempty_y() const {return _min_nonempty_y;}
long moved_arrays::get_max_nonempty_y() const {return _max_nonempty_y;}
