#include "canvas.h"

namespace glib {
	class png_canvas:public canvas {
public:

		png_canvas(const size_t width, const size_t height, const RGBa& background, bool antialias)
		  : canvas(width, height, background, antialias,0,1,2,3) { };
		void toPNG(const std::string& path) const;
		virtual ~png_canvas(){}
	};
}
