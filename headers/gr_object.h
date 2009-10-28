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
		virtual plane<RGBa> pixels(glib_int height, glib_int width, bool antialias, plane<bool> painted_so_far) = 0;
			//...tohle ale neumim
		virtual gr_object* copy_me() const=0;
	};
}
	
	
#endif

