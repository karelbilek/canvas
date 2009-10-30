#ifndef STYLE_INC
#define STYLE_INC

#include "RGBa.h"  //kvuli barvam

namespace glib{

	struct style {
		/*
		 *style popisuje styl nakresleni objektu, jako je tloustka a barva cary.
		 *Na barvy pouziva tridu RGBa.
		 */
		bool _brush_is; //kulaty stetec
		bool _fill_is; //jestli je objekt vybarven (stacilo by RGBa(_, _, _, 0), ale tohle vsechno urychli)
		RGBa _fill_color; //barva vyplne
		RGBa _line_color; //barva cary
		glib_int _line_size; //tloustka cary v celych cislech
		
		style(bool fill_is, const RGBa& fill_color, glib_int line_size, const RGBa& line_color, bool brush_is) : 
		  _brush_is(brush_is),
		  _fill_is(fill_is), 
		  _fill_color(fill_color),
		  _line_color(line_color), 
		  _line_size(line_size){ }
			//nastavim vsechno
		
		style(glib_int line_size, const RGBa& line_color, bool brush_is) : 
		  _brush_is(brush_is),
		  _fill_is(false), 
		  _fill_color(),
		  _line_color(line_color)
		  _line_size(line_size) {}
			//bez vyplne, jenom cara
		
		style() : 
		  _brush_is(false),
		  _fill_is(false), 
		  _fill_color(),
		  _line_color(RGBa(0,0,0)),
		  _line_size(1)
		   { };
			//defaultni styl - tlusty 1, cerny, bez vyplne
	};

}
	
#endif
