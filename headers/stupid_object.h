#include "gr_object.h"

namespace glib {
	class stupid_object: public gr_object {
	private:
		glib_int _x;
		glib_int _y;
		RGBa _colour;
	public:
		virtual plane<bool> pixels(glib_int height, glib_int width, bool antialias, plane<bool> painted_so_far);
		
		virtual gr_object* copy_me() const {new stupid_object other*=stupid_object(*this); return other;}
		
		stupid_object(const glib_int x, const glib_int y, const RGBa& colour):_x(x),_y(y),_colour(colour) {}
	};
	
	plane<bool>::pixels(glib_int height, glib_int width, bool antialias, plane<bool> painted_so_far) {
		plane<bool> result = plane(0, height);
		if (painted_so_far.get(_x, _y)) {
			std::cout<<"NIC NEKRESLIM!"<<_x<<" a "<<_y<<" jsou plny! \n";
		} else {
			std::cout<<"KRESLIM NA "<<_x<<" a "<<_y<<"\n";
			result.set(_x, _y);
		}
		return result;
	}
	
}