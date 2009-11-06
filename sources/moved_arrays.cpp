#include "moved_arrays.h"

using namespace glib;

moved_arrays::moved_arrays(glib_int min_y, glib_int max_y)
  : _first(true),
  //_min_x(0),
  //_max_x(0),
  _min_nonempty_y(0),
  _max_nonempty_y(0),
  _min_y(min_y),
  _max_y(max_y+1),
  _starts(new glib_int[max_y-min_y+2]), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(new glib_int[max_y-min_y+2]),
  _is_set(new bool[max_y-min_y+2]),
  _sorting_hint(_min_y) {
	for (glib_int i = 0; i < max_y-min_y+2; ++i) {
		_is_set[i]=false;
	}

}

moved_arrays::moved_arrays(glib_float min_y, glib_float max_y)
  : _first(true),
  //_min_x(0),
  //_max_x(0),
  _min_nonempty_y(0),
  _max_nonempty_y(0),
  _min_y(static_cast<glib_int>(min_y)),
  _max_y(static_cast<glib_int>(max_y+1)),
  _starts(new glib_int[_max_y-_min_y+1]), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(new glib_int[_max_y-_min_y+1]),
  _is_set(new bool[_max_y-_min_y+1]),
  _sorting_hint(_min_y) {
	for (glib_int i = 0; i < _max_y-_min_y+1; ++i) {
		_is_set[i]=false;
	}

}

moved_arrays::moved_arrays(const moved_arrays& other)
  :  _first(other._first),
  // _min_x(other._min_x),
  // _max_x(other._max_x),
   _min_nonempty_y(other._min_nonempty_y),
   _max_nonempty_y(other._max_nonempty_y),
  _min_y(other._min_y),
  _max_y(other._max_y),
  _starts(new glib_int[_max_y-_min_y+2]), 
  _ends(new glib_int[_max_y-_min_y+2]),
  _is_set(new bool[_max_y-_min_y+2]),
  _sorting_hint(other._sorting_hint) {
	for (glib_int i = 0; i < _max_y-_min_y+2; ++i) {
		_starts[i]=other._starts[i];
		_ends[i]=other._ends[i];
		_is_set[i]=other._is_set[i];
	}

}


moved_arrays::~moved_arrays() {
	delete [] _is_set;
	delete [] _starts;
	delete [] _ends;
}

moved_arrays& 
moved_arrays::operator=(const moved_arrays& other) {
	_min_y = other._min_y;
	_max_y = other._max_y;
	_min_nonempty_y = other._min_nonempty_y;
	_max_nonempty_y = other._max_nonempty_y;
	// _min_x = other._min_x;
	// _max_x = other._max_x;
	_first = other._first;
	_sorting_hint = other._sorting_hint;
	delete [] _starts;
	delete [] _ends;
	delete [] _is_set;
	_is_set = new bool[_max_y-_min_y+2];
	_starts = new glib_int[_max_y-_min_y+2];
	_ends = new glib_int[_max_y-_min_y+2];
	for (glib_int i = 0; i < _max_y-_min_y+2; ++i) {
		_starts[i]=other._starts[i];
		_ends[i]=other._ends[i];
		_is_set[i]=other._is_set[i];
	}
	return *this;
}

glib_int
moved_arrays::get_start(const glib_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		throw 1; 
	}
	return _starts[y - _min_y];
}

glib_int
moved_arrays::get_end(const glib_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		throw 1; 
	}
	return _ends[y - _min_y];
}

void
moved_arrays::set(const glib_int x, const glib_int y) {
	const glib_int my_y = y - _min_y;
	
	if ((y < _min_y) || (y > _max_y)) {
		throw 1;
	}
	if (_first) {
		//_min_x = (_max_x = x);
		_min_nonempty_y = (_max_nonempty_y = y);
		_first = false;
	} else {
		// if (x < _min_x) {
		// 	_min_x = x;
		// }
		// if (x > _max_x) {
		// 	_max_x = x;
		// }
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
moved_arrays::set(const glib_float x, const glib_float y) {
	set(static_cast<glib_int>(x), static_cast<glib_int>(y));
}

bool
moved_arrays::is_set(const glib_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		return false;
	}
	return _is_set[y - _min_y];
}

bool
moved_arrays::is_near(const glib_int x, const glib_int y) const {
	if ((y < _min_y) || (y > _max_y)) {
		return false;
	}
	const glib_int my_y = y - _min_y;
	const glib_int x_levo = x - _starts[my_y];
	const glib_int x_pravo = _ends[my_y] - x;

	return ((x_levo >= -1) && (x_pravo >= -1));
}

bool 
moved_arrays::is_horizontal() const {
	return (_min_nonempty_y==_max_nonempty_y);
}

//glib_int moved_arrays::get_min_x() const {return _min_x;}
//glib_int moved_arrays::get_max_x() const {return _max_x;}
//glib_int moved_arrays::get_min_y() const {return _min_y;}
//glib_int moved_arrays::get_max_y() const {return _max_y;}
glib_int moved_arrays::get_min_nonempty_y() const {return _min_nonempty_y;}
glib_int moved_arrays::get_max_nonempty_y() const {return _max_nonempty_y;}
