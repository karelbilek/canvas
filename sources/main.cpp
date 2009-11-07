#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"

int main () {
	glib::canvas novy(100,100, glib::RGBa(0,255,0),true);
	
	glib::shape_style style(5, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,0));
	glib::shape_style style2(3, glib::RGBa(0,0,0), 1, glib::RGBa(255,255,255,130));
	
	
	novy.push_back(style, glib::disk(glib::point(50,50), 20));
	novy.push_front(style2, glib::disk(glib::point(70,70), 17));
	
	
	
	glib::png_paint("hear.png", novy);
	
	return 0;
}
