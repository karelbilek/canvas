#include "canvas.h"
#include "all_shapes.h"
#include "png_paint.h"

int main () {
	glib::canvas novy(400,400, glib::RGBa(255,255,0),true);
	
	//glib::shape_style style(5, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,0));
	glib::shape_style style2(5, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,255,130));
	
	//glib::shape_style style2(0, glib::RGBa(0,0,0), 1, glib::RGBa(255,0,255,130));
	glib::shape_style style(0, glib::RGBa(0,0,0), 1, glib::RGBa(0,0,0,130));
	
	novy.push_back(style, glib::disk(glib::point(50,50), 20));
	//novy.push_front(style2, glib::segment(0,0,300,100));
	
	
	novy.push_front(style2, glib::polygon(glib::point(400,0),glib::point(200,340), glib::point(30,30), glib::point(320,350),glib::point(0,390)));
	// 
	 novy.push_front(style2, glib::polygon(glib::point(403.897,3.19763), glib::point(395.964,-3.02906), glib::point(205.753,339.933), glib::point(194.261,339.759)));
	 novy.push_front(style, glib::polygon(glib::point(194.261,339.759), glib::point(205.753,339.933), glib::point(25.98,33.0258), glib::point(34.1728,27.1833)));
	 novy.push_front(style2, glib::polygon(glib::point(34.1728,27.1833), glib::point(25.98,33.0258), glib::point(321.82,344.656), glib::point(317.902,355.216)));
	 novy.push_front(style2, glib::polygon(glib::point(317.902,355.216), glib::point(321.82,344.656), glib::point(2.39922,394.683), glib::point(-2.17693,385.184)));
	 novy.push_front(style2, glib::polygon(glib::point(-2.17693,385.184), glib::point(2.39922,394.683), glib::point(395.964,-3.02905), glib::point(403.897,3.19763)));
	// 
	
	
	glib::png_paint("hear.png", novy);
	
	return 0;
}
// 
// #include "geom_line.h"
// #include <math.h>
// int main() {
// 	
// 	glib::geom_line first(glib::point(10,10), glib::point(20,20));
// 	glib::geom_line second = first.parallel(0, 2);
// 	std::cout<<second.a.x<<":"<<second.a.y<<"||"<<second.b.x<<":"<<second.b.y<<"\n";
// 
// 	//glib::point p = first.right_angle_b(1,10);
// 	
// }