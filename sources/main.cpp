#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"
#include "geom_line.h"


using namespace glib;
int main () {
	
	glib::glib_float ll = geom_line(point(10,10), point(0,20)).count_rev_angle(geom_line(point(20,20), point(10,10)));
//	glib::glib_float ll = geom_line(point(10,0), point(0,10)).angle_from_x();
	std::cout<<ll<<"\n";
	glib::canvas novy(800,800, glib::RGBa(255,255,0),true);
	
	glib::shape_style style2(100, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,255,255));
	
	glib::shape_style style(0, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,255,255));
	
	//novy.push_front(style2, glib::segment(0,0,300,100));


	novy.push_back(style, glib::disk(glib::point(50,50), 20));
	
	novy.push_back(style2, glib::disk(glib::point(20,100), 20));
	//novy.push_front(style2, glib::polygon(glib::point(303.897,23.19763), glib::point(295.964,17.02906), glib::point(94.261,359.759), glib::point(105.753,359.933)));
	
		novy.push_front(style2, glib::polygon(glib::point(800,700), glib::point(800,800), glib::point(700,800), glib::point(525,525)));
		//525 - nelame
		//524 - lame
	
	glib::png_paint("hear.png", novy);
	
	return 0;
}
