#ifndef PNGPAINT_INC 
#define PNGPAINT_INC

#include "canvas.h"

namespace glib {
		/*konkretni pouziti canvasu (nebude v knihovni verzi)*/
	void png_paint(const std::string& path, canvas& my_canvas);
	
}

#endif
