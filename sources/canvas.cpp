#include "canvas.h"


using namespace glib;

canvas::canvas(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos, bool del)
:_height(1), _width(1), _background(), _gr_objects(), _antialias(), _red_pos(red_pos), _green_pos(green_pos), _blue_pos(blue_pos), _alpha_pos(alpha_pos), _del(del) { }

canvas::canvas(const canvas& other, const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos)
:_height(other._height), _width(other._width), _background(other._background), _gr_objects(other._gr_objects), _antialias(other._antialias), _alpha_pos(alpha_pos), _green_pos(green_pos), _red_pos(red_pos), _blue_pos(blue_pos), _del(false) {
	
}

canvas::canvas(const size_t height, const size_t width, const RGBa& background, bool antialias, const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos, bool del)
:_height(height), _width(width), _background(background), _gr_objects(), _antialias(antialias), _red_pos(red_pos), _green_pos(green_pos), _blue_pos(blue_pos), _alpha_pos(alpha_pos), _del(del)  { }


//----------------Procedury na zachazeni s CANVAS jako s "vektorem"
void canvas::push_front(gr_object* g) {
	//naopak :)
	_gr_objects.push_back(g);
}

void canvas::push_back(gr_object* g) {
	_gr_objects.push_front(g);
	//naopak :)
}

void canvas::push_front(gr_object* g, const size_t pos) {
	if (pos > _gr_objects.size()) {
		throw 1;
	}
	
	gr_objects::iterator it = _gr_objects.end();
	for (size_t i = 0; i < pos; ++i) {
		--it;
	}
	_gr_objects.insert(it, g);
	
}

void canvas::push_back(gr_object* g, const size_t pos) {
	if (pos > _gr_objects.size()) {
		throw 1;
	}
	
	gr_objects::iterator it = _gr_objects.begin();
	for (size_t i = 0; i < pos; ++i) {
		++it;
	}	//list neumi pricitat, takze to musim delat takhle blbe
	_gr_objects.insert(it, g);
}

void canvas::remove(gr_object* g) {   
	_gr_objects.remove(g);
	delete g;
}

void canvas::remove(const size_t pos) {
	if (pos < _gr_objects.size()) {
		gr_objects::reverse_iterator it = _gr_objects.rbegin();
		for (size_t i = 0; i < pos; ++i, ++it) {}
		_gr_objects.remove(*it);
		delete *it;
	}
}

void canvas::remove_all() {
	while (!_gr_objects.empty()){
		gr_object* g = _gr_objects.back();
		_gr_objects.pop_back();
		delete g;
	}
	
}

void canvas::pop_back(){
    if (!_gr_objects.empty()) {
        _gr_objects.pop_front();
    }
}

void canvas::pop_front(){
    if (!_gr_objects.empty()) {
        _gr_objects.pop_back();
    }
}

//------------------------------------------------------------------------------------------------

canvas::~canvas() {
    if (_del)
        remove_all();
}

plane<RGBa> 
canvas::color_plane() const{
	plane<RGBa> all_plane(0, _height, 0, _width, _background);
	//nejdriv si vse pridam do plane (je to rychle)
	//az pak si to prehodim do rasteru
	//a az potom si to "zmensim" kvuli anti-aliasingu
	
	
	for (gr_objects::const_iterator i = _gr_objects.begin(); i != _gr_objects.end(); ++i) {
		plane<RGBa> pixels = (**i).pixels(_height);
		all_plane.add(pixels);
	}
	return all_plane;
	
	
}

matrix<component> canvas::raster() const{
	
	
	plane<RGBa> all_plane = color_plane();
	
	matrix<component> all_matrix(_width,_height,4);
	
	for (size_t y = 0; y < _height; ++y) {
		colors_row row = all_plane.get_row(y);
		for (colors_row::iterator i = row.begin(); i != row.end(); ++i) {
			all_matrix.set_more(__maximum(i->_start,0), i->_end, y, i->_cont.get_colors(_red_pos, _green_pos, _blue_pos, _alpha_pos));
		}
	}
	
	if (_antialias) {
		all_matrix = all_matrix.half();
	}	//A-Aliasing
	
	return all_matrix;
}

size_t canvas::get_width() const {
	return (_antialias)?(_width/2):(_width);
}

size_t canvas::get_height() const {
	return (_antialias)?(_height/2):(_height);
}

