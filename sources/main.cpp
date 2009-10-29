#include "canvas.h"
#include "png_paint.h"

int main () {
	glib::canvas prvni(100,100,glib::RGBa(255,0,0,255), false);
	
	png_paint("here.png", prvni);
	
	return 0;
}