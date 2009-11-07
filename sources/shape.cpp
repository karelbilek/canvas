#include "shape.h"
#include "moved_arrays.h"
#include "point.h"
#include "all_shapes.h"

using namespace glib;
using namespace std;

map<glib_int, plane<bool> > glib::shape::brushes;

shape::
shape(const shape_style& style, const shape_type& type):
  _style(style),
  _type(type){}

shape_style::
shape_style(glib_int line_size, const RGBa& line_color, bool fill_is, const RGBa& fill_color):
  _line_size(line_size),
  _line_color(line_color),
  _fill_is(fill_is),
  _fill_color(fill_color) {}

//--------------------------------FIRST IMPORTANT THING IN DECADE


plane<RGBa> 
shape::get_pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done) {
	
	shape_type* type_copy = &_type; //kvuli antialiasu :/
	if (antialias) {
		type_copy = type_copy->clone_double(); //pozor, tady alokuju novy
	}
	
	std::list<curve*>::iterator i = type_copy->_curves.begin();
	
	std::list<curve*>::iterator one_before_end = type_copy->_curves.end();
	--one_before_end;
	std::list<curve*>::iterator begin = type_copy->_curves.begin();
	std::list<curve*>::iterator end = type_copy->_curves.end();
	
	
	glib_int min_x = (**i).get_min_x();
	glib_int max_x = (**i).get_max_x();
	glib_int min_y = (**i).get_min_y();
	glib_int max_y = (**i).get_max_y();

	for (++i;i!=end; ++i) {
		min_x = __minimum((**i).get_min_x(), min_x);
		max_x = __maximum((**i).get_max_x(), max_x);
		min_y = __minimum((**i).get_min_y(), min_y);
		max_y = __maximum((**i).get_max_y(), max_y);
	}
	
	min_x = min_x - _style._line_size-1;
	max_x = max_x + _style._line_size+1;
	min_y = min_y - _style._line_size-1;
	max_y = max_y + _style._line_size+1;
	
	max_y = __minimum(max_y, width);
	max_x = __minimum(max_x, height);
	
	if (painted_so_far.most_left()<min_x 
		&& 
		painted_so_far.most_right()>max_x 
		&& 
		painted_so_far.first_non_zero()<min_y
		&&
		painted_so_far.last_non_zero()>max_y) {
		done = false;
		return plane<RGBa>();	
	}
	done=true;
	
	//----- ma to cenu kreslit.
	
	plane<RGBa> result(min_y, max_y);
	//------kraje
	
	
	if (_style._line_size != 0) {
		for (i=begin;i!=end; ++i) {
			
			plane<bool> line(min_y, max_y);
			
			if ((**i).have_thick_line()) {
				curve* previous;
				curve* next;
		
				if (i==begin) {
					if (type_copy->_joined_ends) {
						previous = *one_before_end;
					} else {
						previous = NULL;
					}
				} else {
					std::list<curve*>::iterator j = i;
					--j;
					previous = *j;
				}
		
				if (i==one_before_end) {
					if (type_copy->_joined_ends) {
						next = *begin;
					} else {
						next = NULL;
					}
				} else {
					std::list<curve*>::iterator j = i;
					++j;
					next = *j;
				}
			
				shape_type okraj = (**i).get_thick_line((antialias?2:1)*_style._line_size, previous, next);
				line = paint(&(okraj), min_y, max_y);
				//result.add(().flatten_plane<RGBa>(_style._line_color, true));
			} else {
				glib_int thickness = static_cast<glib_int>((antialias?2:1)*(_style._line_size)+1);
				
				if (!brushes.count(thickness)) {
					shape_type b = disk(point(thickness,thickness), thickness/2);
					plane<bool> p = paint(&b, 0, 2*thickness);
					p._pivot_width = thickness;
					p._pivot_height = thickness;
					brushes.insert(pair<glib_int,plane<bool> >(thickness, p));
				}
				
				plane<bool> p = brushes[thickness];
				list<moved_arrays> borders = type_copy->all_curve_arrays();
				
				for (list<moved_arrays>::iterator i=borders.begin(); i!=borders.end(); ++i) {
					for (glib_int y = i->get_min_nonempty_y(); y<=i->get_max_nonempty_y(); ++y) {
						for (glib_int x = i->get_start(y); x<=i->get_end(y); ++x) {
							plane<bool> m = p.move(x,y);
							m.add(x,y,1);
							line.add(m);
						}
					}
				}
			}
			result.add(line.flatten_plane<RGBa>(_style._line_color, true)); 
		}
	}
	
	
	
	if (_style._fill_is) {
		result.add((paint(type_copy, min_y, max_y)).flatten_plane<RGBa>(_style._fill_color, true));
		
	}
	if (antialias) {
		delete type_copy;
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
shape::paint(const shape_type* const type, glib_int min_y, glib_int max_y){
	
	//to min_y neni pres referenci ale kopii, protoze ho budu menit
	list<moved_arrays> borders = type->all_curve_arrays();
	
	//borders.sort(compare_by_top);
	
	for(list<moved_arrays>::iterator i = borders.begin(); i!=borders.end(); ++i) {
		if (i->is_horizontal()) {
			borders.erase(i); 
		}
	}
	
	if (min_y==0 && max_y==0) {
		list<moved_arrays>::iterator i = borders.begin();
		min_y = i->get_min_nonempty_y();
		max_y = i->get_max_nonempty_y();
		for (++i; i!=borders.end();++i){
			min_y = __minimum(min_y, i->get_min_nonempty_y());
			max_y = __maximum(max_y, i->get_max_nonempty_y());
		}
	}
	
	plane<bool> res(min_y,max_y);
	for (glib_int y = min_y; y < max_y; ++y) {
		
		for(list<moved_arrays>::iterator i = borders.begin(); i!=borders.end(); ++i) {
			i->_sorting_hint = y; //I got no better idea than this crap
		} 
		
		borders.sort(shape::compare_by_row);
		
		glib_int paint_start = 0;
		glib_int paint_end = 0;
		bool paint_part = false;
		
		for (list<moved_arrays>::iterator i = borders.begin();i!=borders.end() && i->is_set(y);++i) {
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
	return res;
}
