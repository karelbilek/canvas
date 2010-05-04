#include "shape.h"
#include "moved_arrays.h"
#include "point.h"
#include "all_shapes.h"
#include "RGBa.h"
#include <sstream>


using namespace libcan;
using namespace std;



map<libcan_int, plane<bool> > libcan::shape::brushes;

set<string> 
shape::get_properties() {
	
	//vlastnosti, co ma kazdy shape, bez ohledu na cokoliv
	string always[] = {"line_red", 
		"line_green", 
		"line_blue", 
		"line_alpha", 
		"fill_red", 
		"fill_green", 
		"fill_blue", 
		"fill_alpha",
		"line_size",
		"name"};
	
	set<string> res = set<string>(always, always+(sizeof (always) / sizeof (string)));
	
	
	vector<string> add = _type->get_specific_properties();
	res.insert(add.begin(), add.end());
	
	return res;
}

string
shape::get_property(const string& property) {
	stringstream res;
	if (property=="line_red") {
		res << (int) _style._line_color.get_red();
	} else if (property=="line_green") {
		res << (int) _style._line_color.get_green();
	} else if (property=="line_blue"){
		res << (int) _style._line_color.get_blue();
	} else if (property=="line_alpha") {
		res << (int) _style._line_color.get_alpha();
	} else if (property=="fill_red") {
		res << (int) _style._fill_color.get_red();
	} else if (property=="fill_green") {
		res << (int) _style._fill_color.get_green();
	} else if (property=="fill_blue"){
		res << (int) _style._fill_color.get_blue();
	} else if (property=="fill_alpha") {
		res << (int) _style._fill_color.get_alpha();
	} else if (property=="line_size") {
		res << _style._line_size;
	} else if (property=="name") {
		if (_name=="") {
			
			_type->get_property(property, res);
		} else {
			
			return _name;
		}
	} else {
		_type->get_property(property, res);
	}
	
	return res.str();
}

void 
shape::set_property(const string& property, const string& what){
	if (!get_properties().count(property)) {
		throw 1;
	}
	
	string old = get_property(property);
	if (old!=what) {
		destroying_change();
	
		stringstream wstream;
		wstream<<what;
		
	
		if (property.substr(0,4)=="line" || property.substr(0,4)=="fill"){
			libcan_int int_what;
			
			wstream>>int_what;
			if (property=="line_red") {
				_style._line_color.set_red(int_what);
			} else if (property=="line_green") {
				_style._line_color.set_green(int_what);
			} else if (property=="line_blue"){
				_style._line_color.set_blue(int_what);
			} else if (property=="line_alpha") {
				_style._line_color.set_alpha(int_what);
			} else if (property=="fill_red") {
				_style._fill_color.set_red(int_what);
			} else if (property=="fill_green") {
				_style._fill_color.set_green(int_what);
			} else if (property=="fill_blue"){
				_style._fill_color.set_blue(int_what);
			} else if (property=="fill_alpha") {
				_style._fill_color.set_alpha(int_what);
			} else if (property=="line_size") {
				_style._line_size = int_what;
			}
		} else if (property=="name") {
			_name = what;
		} else {
			shape_type* new_type = _type->new_with_property(property, wstream);
			delete _type;
			_type = new_type;
		}
	}
}

shape& 
shape::operator=(const shape& another) {
	_name = another._name;
	_style = another._style;
	shape_type* new_type = another._type -> clone();
	delete _type;
	_type = new_type;
	_pixels = another._pixels;
	_changed = another._changed;
	_pixels = another._pixels;
	_footprint_given = another._footprint_given;
	_old_footprint = another._old_footprint;
	_new_footprint = another._new_footprint;
	return *this;
}

shape::
shape(const shape& another) :
  _name(another._name),
  _style(another._style),
  _type(another._type->clone()),
  _pixels(another._pixels),
  _changed(another._changed),
  _painted(another._painted),
  _footprint_given(another._footprint_given),
  _old_footprint(another._old_footprint),
  _new_footprint(another._new_footprint){	
}



shape::
shape(const shape_style& style, const shape_type& type):
  _name(),
  _style(style),
  _type(type.clone()),
  _pixels(),
  _changed(0),
  _painted(0),
  _footprint_given(0),
  _old_footprint(),
  _new_footprint(){
}

shape_style::
shape_style(libcan_int line_size, const RGBa& line_color, const RGBa& fill_color):
  _line_size(line_size),
  _line_color(line_color),
  _fill_color(fill_color) {}

shape_style& 
shape::get_style(const bool& will_change) {
	if (will_change) {
		destroying_change();
	}
	return _style;
}

void
shape::destroying_change() {
	_old_footprint = _new_footprint;
	_painted = false;
	_changed = true;
}

void 
shape::rotate(libcan_float angle){
	destroying_change();
	_type->rotate(angle);
}

void 
shape::resize(libcan_float quoc){
	destroying_change();
	_type->resize(quoc);
}

void 
shape::move(const point& where) {
	_old_footprint = _new_footprint;
	_changed = true;
	_painted = true;	
	
	_new_footprint = _old_footprint.move_relative(where.x, where.y);	
	_pixels = _pixels.move_relative(where.x, where.y);
	_type->move(where);
}

bool
shape::should_paint() const {
	return (_changed || !_footprint_given);
}

plane<bool> 
shape::get_footprint(const bool& antialias, const libcan_int height, const libcan_int width, const bool do_change)  {
	
	plane<bool> res(0, height);
	
	if (!_footprint_given || _changed) {
		
		if (do_change) {
			_footprint_given = true;
		}
		
		if (_changed) {
			if (do_change) {
				_changed = false;
			}
			res.add(_old_footprint);
		}
		
		if (_painted) {
			res.add(_new_footprint);
		} else {
			libcan_int min_x, max_x, min_y, max_y;
			get_extremes(min_x,max_x,min_y,max_y, antialias, height, width);
			plane<bool> ad(min_x, max_x, min_y, max_y, 1);
			res.add(ad);
		}
	}
	
	

	
	return res;
}

void
shape::get_extremes(libcan_int& min_x, libcan_int& max_x, libcan_int& min_y, libcan_int& max_y, const bool& antialias, const libcan_int height, const libcan_int width) const {
	_type->get_extremes(min_x, max_x, min_y, max_y);
	if (antialias) {
		min_x *= 2;
		max_x *= 2;
		min_y *= 2;
		max_y *= 2;
	}
	
	min_x = __maximum(min_x - 3.2*_style._line_size, 0);
	max_x = __minimum(max_x + 3.2*_style._line_size, height);
	min_y = __maximum(min_y - 3.2*_style._line_size, 0);
	max_y = __minimum(max_y + 3.2*_style._line_size, width);
}


plane<RGBa> 
shape::get_pixels(const libcan_int small_height, const libcan_int small_width, const bool& antialias, const plane<bool>& where_not_paint, const bool& force) {
	
	if (_painted && !force) {
		return _pixels;
	} 
	
	libcan_int width = small_width * (antialias ? 2 : 1);
	libcan_int height = small_height * (antialias ? 2 : 1);
	
	_painted = true;
	
	const shape_type* type_copy = _type; //kvuli antialiasu :/
	if (antialias) {
		type_copy = type_copy->clone_double(); //pozor, tady alokuju novy, MUSI dole byt to delete!
	}
	
	std::list<curve*>::const_iterator i = type_copy->_curves.begin();
	
	std::list<curve*>::const_iterator one_before_end = type_copy->_curves.end();
	--one_before_end;
	std::list<curve*>::const_iterator begin = type_copy->_curves.begin();
	std::list<curve*>::const_iterator end = type_copy->_curves.end();
	
	//-----------------------------------------------hledani minima z curves
	
	libcan_int min_x, max_x, min_y, max_y;
	
	get_extremes(min_x,max_x,min_y,max_y, antialias, height, width);
	
	
	//------------------------------------------------co kdyz vubec nemusim kreslit?
	if (where_not_paint.includes_square(min_x, min_y, max_x, max_y)) {
		
		return plane<RGBa>();	
	}
	

	//------------------------------------------------jdu kreslit!!!!!
	
	plane<RGBa> result(min_y, max_y);
	
	//------------------------------------------------jdu na okraje, ale jenom, kdyz jsou
	if (_style._line_size != 0) {
		
		for (i=begin;i!=end; ++i) {
	//------------------------------------------------jdu na konretni caru
		
			plane<bool> line(min_y, max_y);
			
			
			if ((**i).have_thick_line() && _style._line_size > 3) {
	//------------------------------------------------jdu na caru, co se umi sama nakreslit
				curve* previous;
				curve* next;
							//care musim rict predchazejici....
				if (i==begin) {
					if (type_copy->_joined_ends) {
						previous = *one_before_end;
					} else {
						previous = NULL;
					}
				} else {
					std::list<curve*>::const_iterator j = i;
					--j;
					previous = *j;
				}
							//i nasledujici, aby se sama nakreslila
				if (i==one_before_end) {
					if (type_copy->_joined_ends) {
						next = *begin;
					} else {
						next = NULL;
					}
				} else {
					std::list<curve*>::const_iterator j = i;
					++j;
					next = *j;
				}
			
						//vezmu teda shape_type okraje a pripadne ho nakreslim
				shape_type okraj = (**i).get_thick_line((antialias?2:1)*_style._line_size, previous, next);
				line = paint((&okraj), min_y, max_y);
			} else {
	//------------------------------------------------jdu na caru, co se neumi sama nakreslit
	
				libcan_int thickness = static_cast<libcan_int>((antialias?2:1)*(_style._line_size));
				
				
				if (!brushes.count(thickness)) {
						//pokud neni vygenerovan krouzek, vygeneruj ho!
						
					disk b = disk(point(thickness,thickness), static_cast<libcan_float>(thickness)/2);
					plane<bool> p = paint(&b, 0, 2*thickness);
					
					p._pivot_width = thickness;
					p._pivot_height = thickness;
							
					brushes.insert(pair<libcan_int,plane<bool> >(thickness, p));
				}
				
				plane<bool> p = brushes[thickness];
				list<moved_arrays> borders = type_copy->all_curve_arrays();
				
				for (list<moved_arrays>::iterator i=borders.begin(); i!=borders.end(); ++i) {
					for (libcan_int y = i->get_min_nonempty_y(); y<=i->get_max_nonempty_y(); ++y) {
						for (libcan_int x = i->get_start(y); x<=i->get_end(y); ++x) {
							
								//cely to zamaluj!
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
	
	
	
	if (_style._fill_color.is_not_transparent() && _type->_filled) {
		result.add((paint(type_copy, min_y, max_y)).flatten_plane<RGBa>(_style._fill_color, true));
		
	}
	
	if (antialias) {
		delete type_copy;
		result = result.half(RGBa(), width);
	}
	

	_pixels = result;
	RGBa empty(0,0,0,0);
	_new_footprint = _pixels.flatten_plane<bool>(1,empty);
	return result;


}

bool 
shape::compare_by_row(const moved_arrays& a, const moved_arrays& b) {
	
	if (!a.is_set(a._sorting_hint)) {
		return false;
	} 
	if (!b.is_set(b._sorting_hint)){
		return true;
	} 	//little "hack" that puts all not set somewhere back
	

	return (a.get_end(a._sorting_hint) < b.get_start(b._sorting_hint));
		//reflexivita? WHO CARES :)
}

plane<bool>
shape::paint(const shape_type* const type, libcan_int min_y, libcan_int max_y){
	
	//to min_y neni pres referenci ale kopii, protoze ho budu menit
	list<moved_arrays> borders = type->all_curve_arrays();
	

	
	for(list<moved_arrays>::iterator i = borders.begin(); i!=borders.end(); ) {
		
			//horizontalni me moc nezajimaji
		if (i->is_horizontal()) {
			list<moved_arrays>::iterator j = i;
			++i;
			borders.erase(j); 
		} else {
			++i;
		}
	}
	
		//extremni pripad
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
	
	for(list<moved_arrays>::iterator i = borders.begin(); i!=borders.end(); ++i) {
		res.add(i->to_plane());
	}
	
	for (libcan_int y = min_y; y < max_y; ++y) {
		//uplne puvodni algoritmus odsud http://cgg.mff.cuni.cz/~pepca/lectures/npgr003.html
		//autor Josef Pelikan
		//mnou notne upraveno
		
		for(list<moved_arrays>::iterator i = borders.begin(); i!=borders.end(); ++i) {
			i->_sorting_hint = y; //I got no better idea than this
		} 
								  //.....because of this
		borders.sort(shape::compare_by_row);
		
		libcan_int paint_start = 0;
		libcan_int paint_end = 0;
		bool paint_part = false;
		
		bool previous_was_ending=false;
		bool previous_was_starting=false;
		
		for (list<moved_arrays>::iterator i = borders.begin();i!=borders.end() && i->is_set(y);++i) {
			if (!paint_part) {
				paint_start = i->get_end(y);
	
				previous_was_ending = (y==i->get_max_nonempty_y());
				
				previous_was_starting = (y==i->get_min_nonempty_y());
				paint_part = true;
			} else {
				if (i->is_set(y)) {
					paint_end = i->get_start(y);
					
					bool should_paint = true;
					if (y==i->get_max_nonempty_y()) { //konci
						
						if (previous_was_starting&&paint_end<=paint_start) {
							should_paint = false;
						}
					} else if (y==i->get_min_nonempty_y()) {
					
						
						if (previous_was_ending&&paint_end<=paint_start) {
							should_paint = false;
						}
					}
					previous_was_ending = (y==i->get_max_nonempty_y());
					previous_was_starting = (y==i->get_min_nonempty_y());
					
					if (should_paint) {
						if (paint_end >= paint_start) {
							res.add_more(paint_start, paint_end, y, true);
						} else {
							res.add_more(paint_end, paint_start, y, true);
						}
						paint_part = false;
					}
				}
			}
			
		}
		
		
	}
	return res;
}
