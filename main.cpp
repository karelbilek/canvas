//#include "png_canvas.h"
//#include "all_objects.h"

#include "interval.h"

int main () {
	glib::interval<int> kk(10,20,30);
	kk.add_more(1,15,20);
	
	glib::interval<int> lll=kk;
	glib::interval<int> llk(0,0,0);
	llk = lll;
	
	glib::interval<int> ll;
	ll=llk;
	

	
	std::cout<<ll.most_left()<<" "<<ll.get(0)<<" "<<ll.get(10)<<" "<<ll.get(20)<<" "<<ll.most_right()<<"\n";
	
	glib::interval<int> posledni;
	posledni.move(1);
	posledni.get_all();
	std::cout<<posledni.most_left()<<" "<<posledni.get(0)<<" "<<posledni.get(10)<<" "<<posledni.get(20)<<" "<<posledni.most_right()<<"\n";
	ll=posledni;
	std::cout<<ll.most_left()<<" "<<ll.get(0)<<" "<<ll.get(10)<<" "<<ll.get(20)<<" "<<ll.most_right()<<"\n";
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