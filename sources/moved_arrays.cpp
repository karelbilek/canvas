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
  _starts(max_y-min_y+2), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(max_y-min_y+2),
  _is_set(max_y-min_y+2),
  _sorting_hint(0){

}

moved_arrays::moved_arrays(glib_float min_y, glib_float max_y)
  : _first(true),
  _min_nonempty_y(0),
  _max_nonempty_y(0),
  _min_y(static_cast<glib_int>(min_y)),
  _max_y(static_cast<glib_int>(max_y+1)),
  _starts(_max_y-_min_y+1), //chci to vcetne max_yu, plus 1 na obcasne zaokrouhleni
  _ends(_max_y-_min_y+1),
  _is_set(_max_y-_min_y+1),
  _sorting_hint(0) {
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
		std::cout <<"chyba lol chci na "<<y<<"a _min_y je "<<_min_y<<"a _max_y je "<<_max_y<<" lol\n";
		
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
	
	//set(static_cast<glib_int>(x+0.5), static_cast<glib_int>(y+0.5));
}

void
moved_arrays::set(const double x, const double y) {
	set(static_cast<glib_int>(x), static_cast<glib_int>(y));
	
	//set(static_cast<glib_int>(x+0.5), static_cast<glib_int>(y+0.5));
}

plane<bool> 
moved_arrays::to_plane() const {
	plane<bool> res(_min_nonempty_y,_max_nonempty_y+1);
	for (glib_int y = _min_nonempty_y; y<=_max_nonempty_y;++y) {
		if (_is_set[y - _min_y]) {
			res.add_more(_starts[y-_min_y], _ends[y-_min_y],y,true);
		}
	}
	return res;
	
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
