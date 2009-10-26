#ifndef RGBA_INC
#define RGBA_INC

#include "types.h"
#include <map>

namespace glib{
	
	enum basic_colors{
		Red,
		Green,
		Blue,
		Black,
		White,
		Cyan,
		Magenta,
		Yellow,
		Brown
	};
	
	class RGBa {
		/*
		 * Trida na reprezentaci barev.
		 * Ve skutecnosti ulozeno jako POLE (!) ctyr hodnot glib_component,
		 * glib_component = 0...255
		 * 
		 * Pole proto, ze to teoreticky urychluje pristup k barvam, kdyz to pak kladu do PNG
		 * (ta rychlost stejne neni nijak extra velka, ale v jednu chvili vyvoje jsem chtel zrychlit jakkoliv cokoliv)
		 */
	private:
		glib_component _red;
		glib_component _green;
		glib_component _blue;
		glib_component _alpha;
		
	public:
		
		void set_red(const glib_component red);
		void set_green(const glib_component green);
		void set_blue(const glib_component blue);
		void get_colors(glib_component* p_red, glib_component* p_green, glib_component* p_blue, glib_component* p_alpha) const;
		
		RGBa(const basic_colors color);
		
		RGBa();
		//0/0/0/0, myslim (s 0 alphou je stejne vsechno naprd)
		
		RGBa(const glib_component r, const glib_component g, const glib_component b);
		//alpha = 255, ne 0 :)
		
		RGBa(const glib_component r, const glib_component g, const glib_component b, const glib_component a);
		
		RGBa operator+(const RGBa& other) const;
		//secte dve barvy, ale na zaklade "vahy" alphy
		
		
		bool operator==(const RGBa& other) const;
		bool operator<(const RGBa& other) const;
		
		RGBa operator*(const glib_float quoc) const;
		//vynasobeni nejakym cislem
		
		
	};
}

#endif
