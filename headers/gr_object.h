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
		virtual plane<RGBa> pixels(integer height) = 0;
			//...tohle ale neumim
	};
}
	
	
#endif

