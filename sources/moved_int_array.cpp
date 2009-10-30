#include "moved_int_array.h"

using namespace glib;

moved_int_array::moved_int_array(glib_int start, glib_int end)
  : _start(start),
  _end(end+1),
  _array(new glib_int[end-start+2]), //chci to vcetne endu, plus 1 na obcasne zaokrouhleni
  _is_set(new bool[end-start+2]) {
	for (glib_int i = 0; i < end-start+2; ++i) {
		_is_set[i]=false;
	}
}

moved_int_array::moved_int_array(moved_int_array& other)
  : _start(other._start),
  _end(other._end),
  _array(new glib_int[_end-_start+2]), //chci to vcetne endu, plus 1 na obcasne zaokrouhleni
  _is_set(new bool[_end-_start+2]) {
	for (glib_int i = 0; i < _end-_start+2; ++i) {
		_is_set[i]=other._is_set[i];
		_array[i]=other._array[i];
	}

}

moved_int_array::~moved_int_array() {
	delete [] _is_set;
	delete [] _array;
}

moved_int_array& 
moved_int_array::operator=(const moved_int_array& other) {
	_start = other._start;
	_end = other._end;
	delete [] _is_set;
	delete [] _array;
	_is_set = new bool[_end-_start+2];
	_array = new glib_int[_end-_start+2];
	for (glib_int i = 0; i < _end-_start+2; ++i) {
		_is_set[i]=false;
	}
	return *this;
}

glib_int
moved_int_array::get(const glib_int n) const {
	if ((n < _start) || (n > _end)) {
		throw 1; 
	}
	return _array[n - _start];
}

void
moved_int_array::set(const glib_int n, const glib_int co) {
	if ((n < _start) || (n > _end)) {
		throw 1;
	}
	_array[n - _start] = co;
	_is_set[n - _start] = true;
}

bool
moved_int_array::is(const glib_int n) const {
	if ((n < _start) || (n > _end)) {
		return false;
	}
	return _is_set[n - _start];
}