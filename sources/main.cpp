#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"

int main () {
	glib::canvas novy(400,400, glib::RGBa(255,255,0),true);
	
	glib::shape_style style2(18, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,255,255));
	
	glib::shape_style style(0, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,255,255));
	
	//novy.push_front(style2, glib::segment(0,0,300,100));


	novy.push_back(style, glib::disk(glib::point(50,50), 20));
	
	novy.push_back(style2, glib::disk(glib::point(100,100), 20));
	novy.push_front(style2, glib::polygon(glib::point(303.897,23.19763), glib::point(295.964,17.02906), glib::point(105.753,359.933), glib::point(94.261,359.759)));
		novy.push_front(style, glib::polygon(glib::point(303.897,23.19763), glib::point(295.964,17.02906), glib::point(105.753,359.933), glib::point(94.261,359.759)));
	
	//novy.push_front(style2, glib::polygon(glib::point(200,200), glib::point(200,300), glib::point(300,300), glib::point(300,200)));
	
	glib::png_paint("hear.png", novy);
	
	return 0;
}
