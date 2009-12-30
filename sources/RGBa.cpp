#include "RGBa.h"

using namespace canlib;

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

RGBa::RGBa(const canlib_component r, const canlib_component g, const canlib_component b) : _red(r), _green(g),_blue(b),_alpha(255)   {}

RGBa::RGBa(const canlib_component r, const canlib_component g, const canlib_component b, const canlib_component a) : _red(r), _green(g),_blue(b),_alpha(a)  {}


//--------------------------GETTERS
void RGBa::get_colors(canlib_component *p_red, canlib_component *p_green, canlib_component *p_blue, canlib_component *p_alpha) const {
	
	*p_red = _red;
	*p_green = _green;
	*p_blue = _blue;
	*p_alpha = _alpha;
}

//----------------------------SETTERS
void RGBa::set_red(const canlib_component red) {
	_red = red;
}

void RGBa::set_green(const canlib_component green) {
	_green = green;
}

void RGBa::set_blue(const canlib_component blue) {
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
	
	
	
	canlib_float this_f = (canlib_float)_alpha / (canlib_float)res_alpha;
	canlib_float other_f = 1 - this_f;
	
	
	canlib_component res_red = static_cast<canlib_component>(this_f * _red + other_f * other._red);
	canlib_component res_green = static_cast<canlib_component>(this_f * _green + other_f * other._green);
	canlib_component res_blue = static_cast<canlib_component>(this_f * _blue + other_f * other._blue);

	return RGBa(res_red, res_green, res_blue, res_alpha); 
}

RGBa RGBa::operator* (const canlib_float quoc) const {
	canlib_component res_alpha = static_cast<canlib_component>(_alpha*quoc);
	RGBa res = *this;
	res._alpha = res_alpha;
	return res;
}
