#include "png_paint.h"
#include <png.h>
#include <string>

using namespace libcan;

void libcan::png_paint(const std::string& path, canvas& my_canvas) {
											//neni const - kresli se az s prikazem "vrat mi rastr"
	
	
	//Tohle vypadá hrozně složitě a opravdu to JE složité.
	//Použité tyhle kusy kódu:
	//http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-10
	//http://zarb.org/~gc/html/libpng.html
	//http://www.root.cz/clanky/jak-vytvorit-rastrovy-obrazek-v-jazyce-c/
	
	matrix<unsigned char> rast = my_canvas.get_matrix(0,1,2,3);
	
	png_structp my_png_ptr = NULL;
	png_infop my_info_ptr = NULL; 
	
	png_text my_pngtext[1];
	FILE * my_fp;
	my_fp = fopen (path.c_str(), "wb");
	if (my_fp == NULL) {
		throw 1;
	}
	
	
	my_png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, (png_voidp) NULL, NULL, NULL);
	if (my_png_ptr == NULL) {  
		throw 1;
	}
	my_info_ptr = png_create_info_struct (my_png_ptr);    
	
	if (my_info_ptr == NULL) {
		fclose (my_fp);
		png_destroy_write_struct (&my_png_ptr, png_infopp_NULL);
		throw 1;
	}
	
	
	if (setjmp (png_jmpbuf (my_png_ptr))) {
		fclose (my_fp);
		png_destroy_write_struct (&my_png_ptr, &my_info_ptr);
		throw 1;
	}
	
	png_init_io (my_png_ptr, my_fp); 
	
	
	png_set_IHDR (my_png_ptr, my_info_ptr, rast.get_width(), rast.get_height(), 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, 
				  PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); 
	my_info_ptr->sig_bit.red = 8;
	my_info_ptr->sig_bit.green = 8;
	my_info_ptr->sig_bit.blue = 8;
	
	
	my_pngtext[0].key = (char*)"Graphic";
	my_pngtext[0].text = (char*)"Graphic file"; 
	my_pngtext[0].compression = PNG_TEXT_COMPRESSION_NONE; 
	png_set_text (my_png_ptr, my_info_ptr, my_pngtext, 1); 
		
	png_write_info (my_png_ptr, my_info_ptr);
	
	
	png_byte** my_rows = rast.get_rows();	
	png_write_image(my_png_ptr, my_rows);	
	delete [] my_rows;
						//nemaze ten rastr, ale ty pointry :)
	
	png_write_end (my_png_ptr, my_info_ptr);      
	
	
	png_destroy_write_struct (&my_png_ptr, &my_info_ptr);
	fclose (my_fp);
	
}
