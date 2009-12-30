#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"
#include "geom_line.h"


using namespace canlib;
int main () {
	
	canlib::canlib_float ll = geom_line(point(10,10), point(0,20)).count_rev_angle(geom_line(point(20,20), point(10,10)));
//	canlib::canlib_float ll = geom_line(point(10,0), point(0,10)).angle_from_x();
	std::cout<<ll<<"\n";
	canlib::canvas novy(800,800, canlib::RGBa(255,255,0),true);
	
	canlib::shape_style style2(100, canlib::RGBa(0,0,0), 1, canlib::RGBa(255,0,255,255));
	
	canlib::shape_style style(0, canlib::RGBa(0,0,0), 1, canlib::RGBa(255,0,255,255));
	
	//novy.push_front(style2, canlib::segment(0,0,300,100));


	novy.push_back(style, canlib::disk(canlib::point(50,50), 20));
	
	novy.push_back(style2, canlib::disk(canlib::point(20,100), 20));
	//novy.push_front(style2, canlib::polygon(canlib::point(303.897,23.19763), canlib::point(295.964,17.02906), canlib::point(94.261,359.759), canlib::point(105.753,359.933)));
	
		novy.push_front(style2, canlib::polygon(canlib::point(800,700), canlib::point(800,800), canlib::point(700,800), canlib::point(520,520)));
		//525 - nelame
		//524 - lame
	
	canlib::png_paint("hear.png", novy);
	
	return 0;
}
