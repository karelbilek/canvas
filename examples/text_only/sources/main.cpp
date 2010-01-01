#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"
#include "geom_line.h"
#include <iostream>
#include <string>

using namespace libcan;
int main () {
	
	libcan_float ll = geom_line(point(10,10), point(0,20)).count_rev_angle(geom_line(point(20,20), point(10,10)));
//	libcan_float ll = geom_line(point(10,0), point(0,10)).angle_from_x();
	std::cout<<ll<<"\n";
	canvas novy(800,800, RGBa(255,0,0),true);
	
	shape_style style2(100, RGBa(0,0,0), RGBa(255,0,255,255));
	
	shape_style style(100, RGBa(0,0,0,255), RGBa(255,0,255,255));
	
	//novy.push_front(style2, segment(0,0,300,100));
	//novy.push_back(style, disk(point(38,49), point(75,88)));

	novy.push_back(style, regular(point(308,409), point(75,88), 5));

	//novy.push_back(style, segment(point(170,144), point(59,64)));

/*	novy.push_back(style, disk(point(50,50), 20));
	
	novy.push_back(style2, disk(point(20,100), 20));
	//novy.push_front(style2, polygon(point(303.897,23.19763), point(295.964,17.02906), point(94.261,359.759), point(105.753,359.933)));
	
		novy.push_front(style2, polygon(point(800,700), point(800,800), point(700,800), point(520,520)));
		//525 - nelame
		//524 - lame
*/	
	png_paint("hear.png", novy);
	//215,299), point(449,266
	return 0;
}
