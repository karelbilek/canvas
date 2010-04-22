#include "canvas.h"

using namespace std;
using namespace libcan;


shape& 
canvas::get_object(size_t pos) {
	if (_shapes.size() <= pos) {
		throw 1;
	}
	
	list<shape>::iterator it = _shapes.begin();
	for (size_t i = 0; i < pos; ++i, ++it) {}
	
	return *it;
}

shape& 
canvas::get_front(){
	list<shape>::iterator it = _shapes.begin();	
	return *it;
}

shape& 
canvas::get_back(){
	list<shape>::iterator it = _shapes.end();
	--it;
	return *it;
}

size_t 
canvas::count(){
	return _shapes.size();
}

void 
canvas::change_order(size_t from, size_t to){
	list<shape>::iterator from_it = _shapes.begin();
	for (int i=0; i<from; ++i,++from_it){}
	
	list<shape>::iterator to_it = _shapes.begin();
	for (int i=0; i<to; ++i,++to_it){}
	
	_shapes.splice(to_it, _shapes, from_it);
}


//-----------------------------CONSTRUCTORS

canvas::canvas(const size_t width, const size_t height, const RGBa& background, bool antialias) :
  _antialias(antialias),
  _height(height),
  _width(width),
  _background(background),
  _shapes() {}

canvas::canvas() :
  _antialias(false),
  _height(1),
  _width(1),
  _background(),
  _shapes() {}

//----------------------------------DESTRUCTORS
canvas::~canvas() {
	remove_all();
}

//-----------------------------------GETTERS


matrix<libcan_component> canvas::get_matrix(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos) const {
	small quoc = (_antialias?2:1);
	
	plane<RGBa> all_plane = get_plane();
	matrix<libcan_component> all_matrix(quoc*_width,quoc*_height,4);
	
	for (libcan_int y = 0; y < quoc*_height; ++y) {
		
		colors_row row = all_plane.get_row(y);
		for (colors_row::iterator i = row.begin(); i != row.end(); ++i) {
			
			libcan_component color[4];
			i->_cont.get_colors (color+red_pos, color+green_pos, color+blue_pos, color+alpha_pos);
			
			all_matrix.set_more(__maximum(i->_start,0), i->_end, y, color);
			
		}
	}
	
	if (!_antialias) {
		return all_matrix;
	} else {
		return all_matrix.half();
	}
}

void 
canvas::set_antialias(const bool what) {
	_antialias = what;
}

void 
canvas::set_RGBa(const RGBa& what) {
	_background = what;
}

void 
canvas::get_colors(libcan_component* p_red, libcan_component* p_green, libcan_component* p_blue, libcan_component* p_alpha) const {
	_background.get_colors(p_red, p_green, p_blue, p_alpha);
}

 plane<RGBa> 
canvas::get_plane() const  {
	RGBa full(0,0,0,255);
	
	small quoc = (_antialias?2:1);
	plane<RGBa> all_plane(0, quoc*_height);	
	
	
	plane<bool> painted_so_far(0,quoc*_height,0);
	bool done;
	
	for (list<shape>::const_iterator i = _shapes.begin(); i != _shapes.end(); ++i) {
		
		
		plane<RGBa> pixels = (*i).get_pixels(quoc*_height, quoc*_width, _antialias, painted_so_far, done);
		if (done) {
			all_plane.add(pixels);
			painted_so_far.add(pixels.flatten_plane<bool>(1, full));
		}
	}
		//tohle je mozna antiintuitivni, ale kreslim zeshora dolu, tj. pozadi prictu jako posledni
	all_plane.add(plane<RGBa>(0, quoc*_height, 0, quoc*_width, _background));
	
	return all_plane;
}



//----------------------------------SETTERS ("vector-like")
void canvas::push_front(const shape g) {
	
	_shapes.push_front(g);
}

void canvas::push_back(const shape g) {

	_shapes.push_back(g);
}

void canvas::push_front(const shape g, const size_t pos) {
	
	size_t pos_c=__minimum(pos, _shapes.size());
	
	list<shape>::iterator it = _shapes.begin();
	for (size_t i = 0; i < pos_c; ++i) {
		++it;
	}
	_shapes.insert(it, g);
	
}

void canvas::push_back(const shape g, const size_t pos) {
	
	size_t pos_c=__minimum(pos, _shapes.size());
	
	
	list<shape>::iterator it = _shapes.end();
	for (size_t i = 0; i < pos_c; ++i) {
		--it;
	}
	
	_shapes.insert(it, g);
}

void canvas::remove(const size_t pos) {
	if (pos < _shapes.size()) {
		list<shape>::iterator it = _shapes.begin();
		for (size_t i = 0; i < pos; ++i, ++it) {}
		_shapes.erase(it);
	}
}

void canvas::remove_all() {
	while (!_shapes.empty()){
		pop_back();
	}
	
}

void canvas::pop_back(){
    if (!_shapes.empty()) {
		_shapes.pop_back();
    }
}

void canvas::pop_front(){
    if (!_shapes.empty()) {
		_shapes.pop_front();
    }
}

void 
canvas::push_front(const shape_style& style, const shape_type& type) {
	push_front(shape(style, type));
}

void canvas::push_back(const shape_style& style, const shape_type& type) {
	push_back(shape(style, type));
}
