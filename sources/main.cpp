#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"

int main () {
	glib::canvas novy(1000,1000, glib::RGBa(0,255,0),false);
	
	glib::shape_style style(200, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,0));
	glib::shape_style style2(100, glib::RGBa(23,14,99), 1, glib::RGBa(255,12,12,100));
	
	
	novy.push_back(style, glib::disk(glib::point(500,500), 100));
	//novy.push_back(style2, glib::disk(glib::point(300,300), 70));
	
	
	
	glib::png_paint("hear.png", novy);
	
	return 0;
}
