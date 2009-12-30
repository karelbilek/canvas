#ifndef RGBA_INC
#define RGBA_INC

#include "types.h"
#include <map>

namespace canlib{
	
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
		 * Ve skutecnosti ulozeno jako POLE (!) ctyr hodnot canlib_component,
		 * canlib_component = 0...255
		 * 
		 * Pole proto, ze to teoreticky urychluje pristup k barvam, kdyz to pak kladu do PNG
		 * (ta rychlost stejne neni nijak extra velka, ale v jednu chvili vyvoje jsem chtel zrychlit jakkoliv cokoliv)
		 */
	private:
		canlib_component _red;
		canlib_component _green;
		canlib_component _blue;
		canlib_component _alpha;
		
	public:
		
		void set_red(const canlib_component red);
		void set_green(const canlib_component green);
		void set_blue(const canlib_component blue);
		void get_colors(canlib_component* p_red, canlib_component* p_green, canlib_component* p_blue, canlib_component* p_alpha) const;
		
		RGBa(const basic_colors color);
		
		RGBa();
		//0/0/0/0, myslim (s 0 alphou je stejne vsechno naprd)
		
		RGBa(const canlib_component r, const canlib_component g, const canlib_component b);
		//alpha = 255, ne 0 :)
		
		RGBa(const canlib_component r, const canlib_component g, const canlib_component b, const canlib_component a);
		
		RGBa operator+(const RGBa& other) const;
		//secte dve barvy, ale na zaklade "vahy" alphy
		
		
		bool operator==(const RGBa& other) const;
		bool operator>=(const RGBa& other) const;
		
		RGBa operator*(const canlib_float quoc) const;
		//vynasobeni nejakym cislem
		
		
	};
}

#endif
