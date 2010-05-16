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
	for (size_t i=0; i<from; ++i,++from_it){}
	
	list<shape>::iterator to_it = _shapes.begin();
	for (size_t i=0; i<to; ++i,++to_it){}
	
	_shapes.splice(to_it, _shapes, from_it);
}


//-----------------------------CONSTRUCTORS

canvas::canvas(const size_t width, const size_t height, const RGBa& background, bool antialias) :
  _antialias(antialias),
  _force_paint(true),
  _is_deleted(false),
  _deleted(0, height),
  _height(height),
  _width(width),
  _background(background),
  _shapes(),
  _saved_plane(){}

canvas::canvas() :
  _antialias(false),
  _force_paint(true),
  _is_deleted(false),
  _deleted(0, 1),
  _height(1),
  _width(1),
  _background(),
  _shapes(),
  _saved_plane() {}

//----------------------------------DESTRUCTORS
canvas::~canvas() {
	remove_all();
}

//-----------------------------------GETTERS


matrix<unsigned char> canvas::get_matrix(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos) {

	
	plane<RGBa> all_plane = get_plane();
	matrix<unsigned char> all_matrix(_width,_height,4);
	
	for (long y = 0; y < _height; ++y) {
		
		colors_row row = all_plane.get_row(y);
		for (colors_row::iterator i = row.begin(); i != row.end(); ++i) {
			
			unsigned char color[4];
			i->_cont.get_colors_pointer (color+red_pos, color+green_pos, color+blue_pos, color+alpha_pos);
			
			all_matrix.set_more(std::max(i->_start, (long)0), i->_end, y, color);
			
		}
	}
	
	return all_matrix;
}

void 
canvas::set_antialias(const bool what) {
	if (what!=_antialias) {
		_antialias = what;
		_force_paint = true;
	}
}

void 
canvas::set_RGBa(const RGBa& what) {
	if (!(what==_background)) {
		_background = what;
		_force_paint = true;
	}
}

void 
canvas::get_colors(unsigned char* p_red, unsigned char* p_green, unsigned char* p_blue, unsigned char* p_alpha) const {
	_background.get_colors_pointer(p_red, p_green, p_blue, p_alpha);
}

plane<bool>
canvas::what_to_paint(const bool change) {
	plane<bool> res(0, _height);
	for (list<shape>::iterator i = _shapes.begin(); i != _shapes.end(); ++i) {
		plane<bool> what = (*i).get_footprint(_height, _width, change);
		res.add(what);
	}
	if (_is_deleted) {
		
		res.add(_deleted);
		if (change) {
			
			_is_deleted = false;
			_deleted = plane<bool>(0, _height);
		}
	}
	return res;
	
}

bool
canvas::is_force_paint() const {
	return _force_paint;
}

bool
canvas::should_paint() const {
	if (_is_deleted) {
		return 1;
	}
	if (_force_paint) {
		return 1;
	}
	
	for (list<shape>::const_iterator i = _shapes.begin(); i != _shapes.end(); ++i) {
		if ((*i).should_paint()) {
			return 1;
		}
	}
	return 0;
}


plane<RGBa> 
canvas::get_plane()  {

	
	bool spaint = should_paint();
	if (!spaint && !(_force_paint)) {
		return _saved_plane;
	}
	
	RGBa full(0,0,0,255);
	
	plane<RGBa> all_plane(0, _height);	
	
	plane<bool> not_to_paint(0, _height);
	plane<bool> should_paint;
	bool force;
	if (!_force_paint) {
		
		//neni to poprve, tj. ma smysl resit, co prekreslovat
		should_paint = what_to_paint(true);
		not_to_paint = should_paint.negative(1, 0, _width);
		force = false;
	} else {
		
		force = true;
	}
	
	
	
	for (list<shape>::iterator i = _shapes.begin(); i != _shapes.end(); ++i) {
		
		plane<RGBa> pixels = (*i).get_pixels(_height, _width, _antialias, not_to_paint, _background, force);
		all_plane.add(pixels);
		not_to_paint.add(pixels.flatten_plane<bool>(1, full));

	}
		//tohle je mozna antiintuitivni, ale kreslim zeshora dolu, tj. pozadi prictu jako posledni
	
	//all_plane.add(plane<RGBa>(0, _height, 0, _width, _background));
	
	if (!_force_paint) {
		

		_saved_plane.selective_replace(all_plane, should_paint);
	} else {
		_saved_plane = all_plane;
	}
	
	_force_paint=false;
	
	return _saved_plane;
}



//----------------------------------SETTERS ("vector-like")
void canvas::push_front(const shape g) {
	
	_shapes.push_front(g);
}

void canvas::push_back(const shape g) {

	_shapes.push_back(g);
}

void canvas::push_front(const shape g, const size_t pos) {
	
	size_t pos_c=std::min(pos, _shapes.size());
	
	list<shape>::iterator it = _shapes.begin();
	for (size_t i = 0; i < pos_c; ++i) {
		++it;
	}
	_shapes.insert(it, g);
	
}

vector<string> 
canvas::get_names() {
	vector<string> res;
	for (list<shape>::iterator it = _shapes.begin(); it != _shapes.end(); ++it) {
		
		string w = it->get_property("name");
		res.push_back(w);
	}
	
	return res;
}

void canvas::push_back(const shape g, const size_t pos) {
	
	size_t pos_c=std::min(pos, _shapes.size());
	
	
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
		
		plane<bool> added = (*it).get_footprint(_height, _width, false, true);
		
		_deleted.add(added);
		_is_deleted = true;
		
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
		
		plane<bool> added = (_shapes.back()).get_footprint(_height, _width, false, true);
		
		_deleted.add(added);
		_is_deleted = true;
		
		
		_shapes.pop_back();
    }
}

void canvas::pop_front(){
    if (!_shapes.empty()) {
		plane<bool> added = (_shapes.front()).get_footprint(_height, _width, false, true);

		_deleted.add(added);
		_is_deleted = true;
		
		
		
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

long canvas::get_width() const{
	return _width;
}

long canvas::get_height() const {
	return _height;
}
