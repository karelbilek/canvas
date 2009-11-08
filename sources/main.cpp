#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"

int main () {
	glib::canvas novy(400,400, glib::RGBa(255,255,0),false);
	
	//glib::shape_style style(5, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,0));
	glib::shape_style style2(10, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,255,130));
	//vsechno pod 11 vypada blbe
	
	//novy.push_back(style, glib::disk(glib::point(50,50), 20));
	//novy.push_front(style2, glib::segment(0,0,50,30));
	novy.push_front(style2, glib::polygon(glib::point(400,0),glib::point(200,340), glib::point(30,30), glib::point(320,350),glib::point(0,390)));
	
	
	
	glib::png_paint("hear.png", novy);
	
	return 0;
}
