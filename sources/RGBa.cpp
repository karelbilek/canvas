#include "RGBa.h"

using namespace libcan;

//-----------------------CONSTRUCTORS
RGBa::RGBa(const basic_colors color):_red(0),_green(0),_blue(0),_alpha(0){
    switch (color) {
        case Red:
            _red=255; _green=(_blue=0);break;
        case Green:
            _green=255; _red=(_blue=0);break;
        case Blue:
            _blue=255; _red=(_green=0);break;
        case White:
            _blue=(_red=(_green=255));break;
        case Black:
            _blue=(_red=(_green=0));break;
        case Yellow:
            _blue=0; _green=(_red=255);break;
        case Cyan:
            _red=0; _green=(_blue=255);break;
        case Magenta:
            _green=0; _red=(_blue=255);break;
        case Brown:
            _red=165;_green=42;_blue=42;break;
    }
}

RGBa::RGBa() : _red(0), _green(0),_blue(0),_alpha(0) {}

RGBa::RGBa(const libcan_component r, const libcan_component g, const libcan_component b) : _red(r), _green(g),_blue(b),_alpha(255)   {}

RGBa::RGBa(const libcan_component r, const libcan_component g, const libcan_component b, const libcan_component a) : _red(r), _green(g),_blue(b),_alpha(a)  {}


//--------------------------GETTERS
void RGBa::get_colors(libcan_component *p_red, libcan_component *p_green, libcan_component *p_blue, libcan_component *p_alpha) const {
	
	*p_red = _red;
	*p_green = _green;
	*p_blue = _blue;
	*p_alpha = _alpha;
}

//----------------------------SETTERS
void RGBa::set_red(const libcan_component red) {
	_red = red;
}

void RGBa::set_green(const libcan_component green) {
	_green = green;
}

void RGBa::set_blue(const libcan_component blue) {
	_blue = blue;
}


//-------------------------OTHERS
bool
RGBa::operator==(const RGBa& other) const {
    return (_alpha==other._alpha)&&(_blue==other._blue)&&(_green==other._green)&&(_red==other._red);
}

bool
RGBa::operator>=(const RGBa& other) const {
		return _alpha>=other._alpha;
}

RGBa RGBa::operator+(const RGBa& other) const {
	//jedina trochu zajimava cast RGBa
	
	int res_alpha = (int) (_alpha + other._alpha);
	//alpha vysledku
	
	if (res_alpha > 255) {
		res_alpha = 255;
		//preteceni
	};
	
	
	
	libcan_float this_f = (libcan_float)_alpha / (libcan_float)res_alpha;
	libcan_float other_f = 1 - this_f;
	
	
	libcan_component res_red = static_cast<libcan_component>(this_f * _red + other_f * other._red);
	libcan_component res_green = static_cast<libcan_component>(this_f * _green + other_f * other._green);
	libcan_component res_blue = static_cast<libcan_component>(this_f * _blue + other_f * other._blue);

	return RGBa(res_red, res_green, res_blue, res_alpha); 
}

RGBa RGBa::operator* (const libcan_float quoc) const {
	libcan_component res_alpha = static_cast<libcan_component>(_alpha*quoc);
	RGBa res = *this;
	res._alpha = res_alpha;
	return res;
}
