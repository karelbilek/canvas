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

bool
RGBa::is_not_transparent() const{
	return (_alpha!=0);
}

RGBa::RGBa() : _red(0), _green(0),_blue(0),_alpha(0) {}

RGBa::RGBa(const unsigned char r, const unsigned char g, const unsigned char b) : _red(r), _green(g),_blue(b),_alpha(255)   {}

RGBa::RGBa(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) : _red(r), _green(g),_blue(b),_alpha(a)  {}


//--------------------------GETTERS
void RGBa::get_colors_pointer(unsigned char *p_red, unsigned char *p_green, unsigned char *p_blue, unsigned char *p_alpha) const {
	
	*p_red = _red;
	*p_green = _green;
	*p_blue = _blue;
	*p_alpha = _alpha;
}

void RGBa::get_colors(unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &alpha) const {
	
	red = _red;
	green = _green;
	blue = _blue;
	alpha = _alpha;
}

unsigned char 
RGBa::get_green() const {
	return _green;
}
unsigned char 
RGBa::get_red() const {
	return _red;
};

unsigned char 
RGBa::get_blue() const {
	return _blue;
}

unsigned char 
RGBa::get_alpha() const {
	return _alpha;
};

//----------------------------SETTERS
void RGBa::set_red(const unsigned char red) {
	_red = red;
}

void RGBa::set_green(const unsigned char green) {
	_green = green;
}

void RGBa::set_blue(const unsigned char blue) {
	_blue = blue;
}

void RGBa::set_alpha(const unsigned char alpha) {
	_alpha = alpha;
}

RGBa 
RGBa::operator*(const RGBa& what) const{
	RGBa hnus = RGBa(0,0,255);
	
	if (what == RGBa() && !((*this)==RGBa())) {
		return ((*this) * 0.55);
	}
	if ((*this)==RGBa() && !(what==RGBa())) {
		
		return (what * 0.55);
	}
	return RGBa((_red+what._red)/2, (_green+what._green)/2, (_blue+what._blue)/2, (_alpha+what._alpha)/2);
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
	
	
	
	double this_f = (double)_alpha / (double)res_alpha;
	double other_f = 1 - this_f;
	
	
	unsigned char res_red = static_cast<unsigned char>(this_f * _red + other_f * other._red);
	unsigned char res_green = static_cast<unsigned char>(this_f * _green + other_f * other._green);
	unsigned char res_blue = static_cast<unsigned char>(this_f * _blue + other_f * other._blue);

	return RGBa(res_red, res_green, res_blue, res_alpha); 
}

/*RGBa RGBa::avg_with(const RGBa& other) const {
	return 
}*/

RGBa RGBa::operator* (const double quoc) const {
	
	unsigned char res_alpha = static_cast<unsigned char>(_alpha*quoc);
	RGBa res = *this;
	res._alpha = res_alpha;
	return res;
}

// std::ostream& operator<<(std::ostream& out, const libcan::RGBa& c) {
// 	return out << (int) (c.get_red()) << " " << (int)(c.get_green()) << " " << (int)(c.get_blue()) << "(" << (int)(c.get_alpha()) << ")";
// }
