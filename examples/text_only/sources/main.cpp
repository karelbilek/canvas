#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"
#include "geom_line.h"
#include <iostream>
#include <string>

using namespace libcan;
int main () {
	
	canvas novy(800,800, RGBa(255,0,0),true);
		
	shape_style style(10, RGBa(0,0,0,255), RGBa(255,0,255,255));
	
	novy.push_back(style, disk(point(400,400), 100.5));
	
	novy.set_antialias(true);

	png_paint("hearee.png", novy);

	return 0;
}
