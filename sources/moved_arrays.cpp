#include "moved_arrays.h"

using namespace libcan;

moved_arrays::moved_arrays(libcan_int min_y, libcan_int max_y)
  : _first(true),
  //_min_x(0),
  //_max_x(0),
  _min_nonempty_y(0),
  _max_nonempty_y(0),
  _min_y(min_y),
  _max_y(max_y+1),
  _starts(max_y-min_y+2), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(max_y-min_y+2),
  _is_set(max_y-min_y+2),
  _sorting_hint(0){

}

moved_arrays::moved_arrays(libcan_float min_y, libcan_float max_y)
  : _first(true),
  _min_nonempty_y(0),
  _max_nonempty_y(0),
  _min_y(static_cast<libcan_int>(min_y)),
  _max_y(static_cast<libcan_int>(max_y+1)),
  _starts(_max_y-_min_y+1), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(_max_y-_min_y+1),
  _is_set(_max_y-_min_y+1),
  _sorting_hint(0) {
}


libcan_int
moved_arrays::get_start(const libcan_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		throw 1; 
	}
	return _starts[y - _min_y];
}

libcan_int
moved_arrays::get_end(const libcan_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		throw 1; 
	}
	return _ends[y - _min_y];
}

void
moved_arrays::set(const libcan_int x, const libcan_int y) {
	
	const libcan_int my_y = y - _min_y;
	
	if ((y < _min_y) || (y > _max_y)) {
		throw 1;
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
	set(static_cast<libcan_int>(x), static_cast<libcan_int>(y));
	
	//set(static_cast<libcan_int>(x+0.5), static_cast<libcan_int>(y+0.5));
}

plane<bool> 
moved_arrays::to_plane() const {
	plane<bool> res(_min_nonempty_y,_max_nonempty_y+1);
	for (libcan_int y = _min_nonempty_y; y<=_max_nonempty_y;++y) {
		if (_is_set[y - _min_y]) {
			res.add_more(_starts[y-_min_y], _ends[y-_min_y],y,true);
		}
	}
	return res;
	
}

bool
moved_arrays::is_set(const libcan_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		return false;
	}
	return _is_set[y - _min_y];
}

bool
moved_arrays::is_near(const libcan_int x, const libcan_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		return false;
	}
	const libcan_int my_y = y - _min_y;
	const libcan_int x_levo = x - _starts[my_y];
	const libcan_int x_pravo = _ends[my_y] - x;

	return ((x_levo >= -1) && (x_pravo >= -1));
}

bool 
moved_arrays::is_horizontal() const {
	return (_min_nonempty_y==_max_nonempty_y);
}

libcan_int moved_arrays::get_min_nonempty_y() const {return _min_nonempty_y;}
libcan_int moved_arrays::get_max_nonempty_y() const {return _max_nonempty_y;}
