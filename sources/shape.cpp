#include "shape.h"
#include "moved_arrays.h"
#include "point.h"
#include "all_shapes.h"
#include "RGBa.h"


using namespace libcan;
using namespace std;



map<long, plane<bool> > libcan::shape::brushes;

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
			long int_what;
			
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
				_style._line_size = abs(int_what);
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
shape_style(long line_size, const RGBa& line_color, const RGBa& fill_color):
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
shape::rotate(double angle){
	destroying_change();
	_type->rotate(angle);
}

void 
shape::resize(double quoc){
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
shape::get_footprint( const long height, const long width, const bool do_change, const bool force)  {
	
	plane<bool> res(0, height);
	
	if (!_footprint_given || _changed || force) {
		
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
			long min_x, max_x, min_y, max_y;
			get_extremes(min_x,max_x,min_y,max_y, false, height, width);
			plane<bool> ad(min_x, max_x, min_y, max_y, 1);
			res.add(ad);
		}
	}
	
	

	
	return res;
}

void
shape::get_extremes(long& min_x, long& max_x, long& min_y, long& max_y, const bool& double_it, const long height, const long width) const {
	_type->get_extremes(min_x, max_x, min_y, max_y);
	if (double_it) {
		min_x *= 2;
		max_x *= 2;
		min_y *= 2;
		max_y *= 2;
	}
	
	min_x = std::max(min_x - 3.2*_style._line_size, (double) 0);
	max_x = std::min(max_x + 3.2*_style._line_size, (double) height);
	min_y = std::max(min_y - 3.2*_style._line_size, (double) 0);
	max_y = std::min(max_y + 3.2*_style._line_size, (double) width);
}


plane<RGBa> 
shape::get_pixels(const long small_height, const long small_width, const bool& antialias, const plane<bool>& where_not_paint, const RGBa& background, const bool& force) {
	
	if (_painted && !force) {
		return _pixels;
	} 
	
	long width = small_width * (antialias ? 2 : 1);
	long height = small_height * (antialias ? 2 : 1);
	
	_painted = true;
	
	const shape_type* type_copy = _type; //kvuli antialiasu :/
	if (antialias) {
		type_copy = type_copy->clone_double(); //pozor, tady alokuju novy, MUSI dole byt to delete!
	}
	

	
	//-----------------------------------------------hledani minima z curves
	
	long min_x, max_x, min_y, max_y;
	
	get_extremes(min_x,max_x,min_y,max_y, antialias, height, width);
	
	
	//------------------------------------------------co kdyz vubec nemusim kreslit?
	if (where_not_paint.includes_square(min_x, min_y, max_x, max_y)) {
		
		return plane<RGBa>();	
	}
	

	//------------------------------------------------jdu kreslit!!!!!
	
	plane<RGBa> result(min_y, max_y);
	
	//------------------------------------------------jdu na okraje, ale jenom, kdyz jsou
	if (_style._line_size != 0) {
		
		std::list<curve*>::const_iterator i = type_copy->_curves.begin();

		std::list<curve*>::const_iterator one_before_end = type_copy->_curves.end();
		--one_before_end;
		std::list<curve*>::const_iterator begin = type_copy->_curves.begin();
		std::list<curve*>::const_iterator end = type_copy->_curves.end();
		
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
	
				long thickness = static_cast<long>((antialias?2:1)*(_style._line_size));
				
				
				if (!brushes.count(thickness)) {
						//pokud neni vygenerovan krouzek, vygeneruj ho!
						
					disk b = disk(point(thickness,thickness), static_cast<double>(thickness)/2);
					plane<bool> p = paint(&b, 0, 2*thickness);
					
					p._pivot_width = thickness;
					p._pivot_height = thickness;
							
					brushes.insert(pair<long,plane<bool> >(thickness, p));
				}
				
				plane<bool> p = brushes[thickness];
				list<moved_arrays> borders = type_copy->all_curve_arrays();
				
				for (list<moved_arrays>::iterator i=borders.begin(); i!=borders.end(); ++i) {
					for (long y = i->get_min_nonempty_y(); y<=i->get_max_nonempty_y(); ++y) {
						for (long x = i->get_start(y); x<=i->get_end(y); ++x) {
							
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
shape::paint(const shape_type* const type, long min_y, long max_y){
	
	list<moved_arrays> borders = type->all_curve_arrays();
		//vrati vsechny konkretni segmenty vsech okraju

	
	plane<bool> res(min_y,max_y);

			//proste pridam cary do vysledku
	for(list<moved_arrays>::iterator i = borders.begin(); i!=borders.end(); ++i) {
		res.add(i->to_plane());
	}
	
	for (long y = min_y; y < max_y; ++y) {
		//uplne puvodni algoritmus odsud http://cgg.mff.cuni.cz/~pepca/lectures/npgr003.html
		//autor Josef Pelikan
		//mnou notne upraveno
		
		
		//seradi tak, ze na radku y jsou pekne za sebou
		for(list<moved_arrays>::iterator i = borders.begin(); i!=borders.end(); ++i) {
			i->_sorting_hint = y; //I got no better idea than this
		} 
								  //.....because of this
		borders.sort(shape::compare_by_row);
		
		//do paint_start si budu ukladat, odkud mam kreslit, do paint_end kam mam kreslit,
		//do paint_part jestli zrovna kreslim nebo
		long paint_start;
		long paint_end;
		bool paint_part = false;
		
		
		bool last_max;
		bool last_min;
		
		for (list<moved_arrays>::iterator i = borders.begin();i!=borders.end() && i->is_set(y);++i) {
			
			bool switchit;
			if (y==i->get_max_nonempty_y() || y==i->get_min_nonempty_y()) {
				if (!(last_min || last_max)) {
					last_min = (y == i->get_min_nonempty_y());
					last_max = (y == i->get_max_nonempty_y());
					switchit = false;
				} else if (last_min) {
					switchit = !(y == i->get_min_nonempty_y());
				} else if (last_max) {
					switchit = !(y == i->get_max_nonempty_y());
				}
			} else {
				switchit = true;
			}
			if (switchit) {
				if (!paint_part) {
					paint_start = i->get_end(y);
				} else {
					paint_end = i->get_start(y);
					if (paint_end >= paint_start) {
						res.add_more(paint_start, paint_end, y, true);
					} else {
						res.add_more(paint_end, paint_start, y, true);
					}
				}
				paint_part = !paint_part;
			}
		}
		
		
	}
	return res;
}
