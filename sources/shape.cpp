#include "shape.h"
#include "moved_arrays.h"

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
	for (other.curves::const_iterator i = other.curves.begin(); i!= other.curves.end(); ++i) {
		_curves.push_back(**i.clone());
	}
}

shape_type& 
shape_type::operator=(const shape_type& other) {
	_filled = other._filled;
	_joined_ends = other._joined_ends;
	for (other._curves::const_iterator i = other.curves.begin(); i!= other.curves.end(); ++i) {
		_curves.push_back(**i.clone());
	}
}

shape_type::
~shape_type() {
	for (_curves::iterator i = curves.begin(); i!= curves.end(); ++i) {
		curve* point = *i;
		delete point;
	}
}

shape::
shape(const shape_style& style, const shape& type):
  _style(style),
  _type(type){}

shape_style::
shape_style(glib_int line_size, const RGBa& line_color, bool brush_is, bool fill_is, const RGBa& fill_color):
  _line_size(line_size),
  _line_color(line_color),
  _brush_is(brush_is),
  _fill_is(fill_is),
  _fill_color(fill_color) {}

//--------------------------------FIRST IMPORTANT THING IN DECADE


shape::
plane<RGBa> get_pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done) {
	//TODO - 2x stejny sofar PRUNIK minmax -> neprekresluj
	
	_type._curves::iterator i = _type._curves.begin();
	
	_type._curves::iterator one_before_end = _type._curves.end();
	--one_before_end;
	_type._curves::iterator begin = _type._curves.begin();
	_type._curves::iterator end = _type._curves.end();
	
	
	glib_int min_x = i->get_min_x();
	glib_int max_x = i->get_max_x();
	glib_int min_y = i->get_min_y();
	glib_int max_y = i->get_max_y();

	for (++i;i!=end; ++i) {
		min_x = __minimum(i->get_min_x(), min_x);
		max_x = __maximum(i->get_max_x(), max_x);
		min_y = __minimum(i->get_min_y(), min_y);
		max_y = __maximum(i->get_max_y(), max_y);
	}
	
	min_x = min_x - _style._line_size;
	max_x = max_x - _style._line_size;
	min_y = min_y - _style._line_size;
	max_y = max_y - _style._line_size;
	
	max_y = __minimum(max_y, width);
	max_x = __minimum(max_x, height);
	
	if (painted_so_far.most_left()<min_x 
		&& 
		painted_so_far.most_right()>max_x 
		&& 
		painted_so_far.first_non_zero()<min_y
		&&
		painted_so_far.last_non_zero()>max_y) {
		return plane<RGBa>();	
	}
	
	//----- ma to cenu kreslit.
	
	plane<RGBa> result(min_y, max_y);
	//------kraje
	
	if (_style._line_size != 0) {
		for (i=begin;i!=end; ++i) {
			curve* previous;
			curve* next;
		
			if (i==begin) {
				if (_type._joined_ends) {
					previous = *one_before_end;
				} else {
					previous = NULL;
				}
			} else {
				_type._curves::iterator j = i;
				--j;
				previous = j;
			}
		
			if (j==_one_before_end) {
				if (_type._joined_ends) {
					next = *begin;
				} else {
					next = NULL;
				}
			} else {
				_type._curves::iterator j = i;
				++j;
				next = j;
			}
		
			plane<bool> curve_painted = paint(**i.get_thick_line(_style._line_size, previous, next));
			result.add(curve_painted.flatten_plane<RGBa>(_style._line_color));
		
		}
	}
	
	if (_style._fill_is) {
		plane<bool> fill_painted = paint(_type, min_y, max_y);
		result.add(fill_painted.flatten_plane<RGBa>(_style._fill_color));
		
	}
	
	return result;


}
// bool 
// shape::compare_by_top(const moved_arrays& a, const moved_arrays& b) {
// 	if (a.is_horizontal()) return false;
// 	if (b.is_horizontal()) return true;
//     //vsechny vodorovne pujdou na konec, nelibi se mi
// 
// 	glib_int ay = a.get_min_nonempty_y();
// 	glib_int by = b.get_min_nonempty_y();
// 	
// 	if (ay < by) return true;
// 	if (ay > by) return false;
// 	
// 	
// 	glib_int ax = a.get_start(ay);
// 	glib_int bx = b.get_start(by);
// 	
// 	if (ax < bx) return true;
// 	if (ax > bx) return false;
// 	
// 	
// 	return false;
// }


bool 
shape::compare_by_row(const moved_arrays& a, const moved_arrays& b) {
	
	if (!a.is_set(a._sorting_hint)) {
		return false;
	} 
	if (!b.is_set(b._sorting_hint)){
		return true;
	} 	//little "hack" that puts all not set somewhere back
	

	return (a.get_end(a._sorting_hint) < b.get_start(b._sorting_hint));
		//reflexivita? WHO CARES!! WEEEEE
}

plane<bool> 
shape::paint(const shape_type& type, const glib_int min_y, const glib_int max_y){
	//neni pres referenci ale kopii, protoze ho budu menit
	list<moved_arrays> borders;
	for(type._curves::const_iterator i = type._curves.begin(); i!=type._curves.end(); ++i) {
		list<moved_arrays> to_add = **i.get_arrays();
		borders.splice(to_add); 
	}
	
	//borders.sort(compare_by_top);
	
	for(borders::iterator i = borders.begin(); i!=borders.end(); ++i) {
		if *i.is_horizontal() {
			borders.remove(i); 
		}
	}
	
	if (min_y==0 && max_y==0) {
		borders::iterator i = borders.begin();
		min_y = i->get_min_nonempty_y();
		max_y = i->get_max_nonempty_y();
		for (++i; i!=borders.end();++i){
			min_y = __minimum(min_y, i->get_min_nonempty_y());
			max_y = __maximum(max_y, i->get_max_nonempty_y());
		}
	}
	
	plane<bool> res(min_y,max_y);
	for (glib_int y = min_y; y < max_y; ++y) {
		
		for(borders::iterator i = borders.begin(); i!=borders.end(); ++i) {
			*i._sorting_hint = y; //I got no better idea than this crap
		}
		borders.sort(compare_by_row);
		
		glib_int paint_start = 0;
		glib_int paint_end = 0;
		bool paint_part = false;
		
		for (borders::iterator i = borders.begin();i!=borders.end() && i.is_set(y);++i) {
			if (!paint_part) {
				paint_start = i->get_end(y);
			} else {
				paint_end = i->get_start(y);
				if (paint_end > paint_start) {
					res.add_more(paint_start, paint_end, y, true);
				} else {
					res.add_more(paint_end, paint_start, y, true);
				}
			}
			
			paint_part = !paint_part;
		}
		
		
	}
	
	
}