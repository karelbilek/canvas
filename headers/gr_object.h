#ifndef GR_OBJ_INC
#define GR_OBJ_INC
#include "plane.h"
#include "RGBa.h"


		//NAPROSTO virtualni objekt
namespace glib {
	class gr_object {

	public:
			
		virtual ~gr_object(){};	
			//jen tak, aby nemel linker na co nadavat
		virtual plane<RGBa> pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done) = 0;
			//...tohle ale neumim
		virtual gr_object* copy_me() const=0;
	};
}
	
	
#endif

