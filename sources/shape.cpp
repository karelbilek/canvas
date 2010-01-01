#include "shape.h"
#include "moved_arrays.h"
#include "point.h"
#include "all_shapes.h"



using namespace libcan;
using namespace std;

map<libcan_int, plane<bool> > libcan::shape::brushes;

shape::
shape(const shape_style& style, const shape_type& type):
  _style(style),
  _type(type){}

shape_style::
shape_style(libcan_int line_size, const RGBa& line_color, const RGBa& fill_color):
  _line_size(line_size),
  _line_color(line_color),
  _fill_color(fill_color) {}

void 
shape::rotate(libcan_float angle){
	_type.rotate(angle);
}

void 
shape::resize(libcan_float quoc){
	_type.resize(quoc);
}

void 
shape::move(const point& where) {
	_type.move(where);
}


plane<RGBa> 
shape::get_pixels(const libcan_int height, const libcan_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done) {
	
	shape_type* type_copy = &_type; //kvuli antialiasu :/
	if (antialias) {
		type_copy = type_copy->clone_double(); //pozor, tady alokuju novy, MUSI dole byt to delete!
	}
	
	std::list<curve*>::iterator i = type_copy->_curves.begin();
	
	std::list<curve*>::iterator one_before_end = type_copy->_curves.end();
	--one_before_end;
	std::list<curve*>::iterator begin = type_copy->_curves.begin();
	std::list<curve*>::iterator end = type_copy->_curves.end();
	
	//-----------------------------------------------hledani minima z curves
	
	libcan_int min_x, max_x, min_y, max_y;
	
	type_copy->get_extremes(min_x,max_x,min_y,max_y);
	
	//------------------------------------------------pridam tloustku cary
	min_x = __maximum(min_x - 3.2*_style._line_size, 0);
	max_x = __minimum(max_x + 3.2*_style._line_size, height);
	min_y = __maximum(min_y - 3.2*_style._line_size, 0);
	max_y = __minimum(max_y + 3.2*_style._line_size, width);

	
	//------------------------------------------------co kdyz vubec nemusim kreslit?
	if (painted_so_far.includes_square(min_x, min_y, max_x, max_y)) {
		done = false;
		return plane<RGBa>();	
	}
	done=true;
	

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
					std::list<curve*>::iterator j = i;
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
					std::list<curve*>::iterator j = i;
					++j;
					next = *j;
				}
			
						//vezmu teda shape_type okraje a pripadne ho nakreslim
				shape_type okraj = (**i).get_thick_line((antialias?2:1)*_style._line_size, previous, next);
				line = paint(&(okraj), min_y, max_y);
				
			} else {
	//------------------------------------------------jdu na caru, co se neumi sama nakreslit
	
				libcan_int thickness = static_cast<libcan_int>((antialias?2:1)*(_style._line_size));
				
				
				if (!brushes.count(thickness)) {
						//pokud neni vygenerovan krouzek, vygeneruj ho!
						
					shape_type b = disk(point(thickness,thickness), static_cast<libcan_float>(thickness)/2);
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
	
	
	
	if (_style._fill_color.is_not_transparent() && _type._filled) {
		result.add((paint(type_copy, min_y, max_y)).flatten_plane<RGBa>(_style._fill_color, true));
		
	}
	
	if (antialias) {
		delete type_copy;
	}
	
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
