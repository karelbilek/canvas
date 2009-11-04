#include "canvas.h"


using namespace glib;

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

glib_int canvas::get_width() const {
	return _width;
}

glib_int canvas::get_height() const {
	return _height;
}

matrix<glib_component> canvas::get_matrix(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos) {
	
	
	plane<RGBa> all_plane = get_plane();
	matrix<glib_component> all_matrix(_width,_height,4);
	
	for (glib_int y = 0; y < _height; ++y) {
		
		colors_row row = all_plane.get_row(y);
		
		for (colors_row::iterator i = row.begin(); i != row.end(); ++i) {
			
			glib_component color[4];
			i->_cont.get_colors (color+red_pos, color+green_pos, color+blue_pos, color+alpha_pos);
			all_matrix.set_more(__maximum(i->_start,0), i->_end, y, color);
			
		}
	}
	
	return all_matrix;
}


plane<RGBa> 
canvas::get_plane()  {
	RGBa full(0,0,0,255);
	
	plane<RGBa> all_plane(0, _height);
	//nejdriv si vse pridam do plane (je to rychle)
	//az pak si to prehodim do rasteru
	
	
	
	plane<bool> painted_so_far(0,_height,0);
	bool done;
	
	for (_shapes::const_iterator i = _shapes.begin(); i != _shapes.end(); ++i) {
		
		
		plane<RGBa> pixels = (*i).pixels(_height, _width, _antialias, painted_so_far, done);
		if (done) {
			all_plane.add(pixels);
			painted_so_far.add(pixels.flatten_plane<bool>(1, full));
		}
	}
		//tohle je mozna antiintuitivni, ale kreslim zeshora dolu, tj. pozadi prictu jako posledni
	all_plane.add(plane<RGBa>(0, _height, 0, _width, _background));
	
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
	
	gr_objects::iterator it = _shapes.begin();
	for (size_t i = 0; i < pos_c; ++i) {
		++it;
	}
	_shapes.insert(it, g);
	
}

void canvas::push_back(const shape g, const size_t pos) {
	
	size_t pos_c=__minimum(pos, _shapes.size());
	
	
	gr_objects::iterator it = _shapes.end();
	for (size_t i = 0; i < pos_c; ++i) {
		--it;
	}
	
	_shapes.insert(it, g);
}

void canvas::remove(const size_t pos) {
	if (pos < _shapes.size()) {
		gr_objects::iterator it = _shapes.begin();
		for (size_t i = 0; i < pos; ++i, ++it) {}
		_shapes.remove(*it);
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
