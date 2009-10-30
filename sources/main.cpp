#include "canvas.h"
#include "png_paint.h"
#include "stupid_object.h"
#include "style.h"

int main () {
	glib::canvas prvni(100,100,glib::RGBa(255,0,0,255), false);
	
	glib::stupid_object stupid(50,50,glib::RGBa(0,255,0,255));
	prvni.push_front(&stupid);
	prvni.push_front(&stupid);
	
	
	
	glib::png_paint("here.png", prvni);
	
	return 0;
}