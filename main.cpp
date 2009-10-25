//#include "png_canvas.h"
//#include "all_objects.h"

#include "types.h"
#include "matrix.h"

int main () {
	glib::matrix<bool> shit(10,10,2);
	
	shit.set(9,9,1,1);
	
	glib::matrix<bool> opossum;
	opossum = shit;
	
	std::cout << opossum.get(9,9,1)<<"\n";

	
	return 0;
}


	// using namespace glib;
	// 
	// 
	// //pix hovno(100,100);
	// //hovno = hovno.move(10,-10);
	// //std::cout<<hovno.x<<" "<<hovno.y<<"\n";
	// regular hovno(6,pix(100,100), pix(3,150),style(), true);
	// png_canvas c(500,500, RGBa(255,255,0),false);
	// /*62,58
	// 	55,117*/
	// 	
	// style mujstyl(true, RGBa(255,0,50), 9,RGBa(0,0,0),false);
	// style mujstyl2(true, RGBa(255,0,50), 0,RGBa(0,0,0),false);
	// 
	// 	 
	// ellipse* e = new ellipse(pix(250,250), pix(400,250), pix(0,300), mujstyl);
	// c.push_back(e);
	// 
	// c.toPNG("obr.png");