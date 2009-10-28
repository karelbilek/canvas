#include "canvas.h"


using namespace glib;

//-----------------------------CONSTRUCTORS

canvas::canvas(const size_t width, const size_t height, const RGBa& background, bool antialias) :
  _antialias(antialias),
  _height(height),
  _width(width),
  _background(background),
  _gr_objects() {}

canvas::canvas() :
  _antialias(false),
  _height(1),
  _width(1),
  _background(),
  _gr_objects() {}

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
	plane<RGBa> all_plane(0, _height, 0, _width, _background);
	//nejdriv si vse pridam do plane (je to rychle)
	//az pak si to prehodim do rasteru
	
	plane<bool> painted_so_far(0,_height,0);
	
	for (gr_objects::const_iterator i = _gr_objects.begin(); i != _gr_objects.end(); ++i) {
		plane<RGBa> pixels = (**i).pixels(_height, _width, _antialias, painted_so_far);
		all_plane.add(pixels);
		painted_so_far.add(pixels.flatten_plane<bool>(1));
	}
	return all_plane;
}



//----------------------------------SETTERS ("vector-like")
void canvas::push_front(const gr_object* const g) {
	gr_object* copy= g->copy_me();
	
	_gr_objects.push_back(copy);//sic!!!
}

void canvas::push_back(const gr_object* const g) {
	gr_object* copy= g->copy_me();
	
	_gr_objects.push_front(copy);//sic!!!
}

void canvas::push_front(const gr_object* const g, const size_t pos) {
	gr_object* copy= g->copy_me();
	
	size_t pos_c=__minimum(pos, _gr_objects.size());
	
	gr_objects::iterator it = _gr_objects.end();
	for (size_t i = 0; i < pos_c; ++i) {
		--it;
	}
	_gr_objects.insert(it, copy);
	
}

void canvas::push_back(const gr_object* const g, const size_t pos) {
	gr_object* copy= g->copy_me();
	
	size_t pos_c=__minimum(pos, _gr_objects.size());
	
	
	gr_objects::iterator it = _gr_objects.begin();
	for (size_t i = 0; i < pos_c; ++i) {
		++it;
	}
	
	_gr_objects.insert(it, copy);
}

void canvas::remove(const size_t pos) {
	if (pos < _gr_objects.size()) {
		gr_objects::reverse_iterator it = _gr_objects.rbegin();
		for (size_t i = 0; i < pos; ++i, ++it) {}
		_gr_objects.remove(*it);
		delete *it;//delam si kopie, tedy tohle je OK
	}
}

void canvas::remove_all() {
	while (!_gr_objects.empty()){
		pop_back();
	}
	
}

void canvas::pop_back(){
    if (!_gr_objects.empty()) {
        gr_object* g = _gr_objects.front();
		_gr_objects.pop_front();
		delete g;
    }
}

void canvas::pop_front(){
    if (!_gr_objects.empty()) {
        gr_object* g = _gr_objects.back();
		_gr_objects.pop_back();
		delete g;
    }
}
