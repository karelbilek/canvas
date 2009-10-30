#include "gr_object.h"

namespace glib {
	class stupid_object: public gr_object {
	private:
		glib_int _x;
		glib_int _y;
		RGBa _colour;
	public:
		virtual plane<RGBa> pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done);
		
		virtual gr_object* copy_me() const {stupid_object* other=new stupid_object(*this); return other;}
		
		stupid_object(const glib_int x, const glib_int y, const RGBa& colour):_x(x),_y(y),_colour(colour) {}
	};
	
	plane<RGBa>
	stupid_object::pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done) {
		
		done = false;
		
		
		plane<RGBa> result(_y, _y+1);
		if (painted_so_far.get(_x, _y)) {
			std::cout<<"NIC NEKRESLIM!"<<_x<<" a "<<_y<<" jsou plny! \n";
		} else {
			std::cout<<"KRESLIM NA "<<_x<<" a "<<_y<<"\n";
			result.set(_x, _y, _colour);
			done = true;
		}
		return result;
	}
	
}