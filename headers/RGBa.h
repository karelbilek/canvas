#ifndef RGBA_INC
#define RGBA_INC

#include "types.h"
#include <map>

namespace libcan{
	
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
		 * Ve skutecnosti ulozeno jako POLE (!) ctyr hodnot libcan_component,
		 * libcan_component = 0...255
		 * 
		 * Pole proto, ze to teoreticky urychluje pristup k barvam, kdyz to pak kladu do PNG
		 * (ta rychlost stejne neni nijak extra velka, ale v jednu chvili vyvoje jsem chtel zrychlit jakkoliv cokoliv)
		 */
	private:
		libcan_component _red;
		libcan_component _green;
		libcan_component _blue;
		libcan_component _alpha;
		
	public:
		
		bool is_not_transparent() const;
		
		void set_red(const libcan_component red);
		void set_green(const libcan_component green);
		void set_blue(const libcan_component blue);
		void get_colors_pointer(libcan_component* p_red, libcan_component* p_green, libcan_component* p_blue, libcan_component* p_alpha) const;
		
		void get_colors(libcan_component &red, libcan_component &green, libcan_component &blue, libcan_component &alpha) const;
		
		RGBa(const basic_colors color);
		
		RGBa();
		//0/0/0/0, myslim (s 0 alphou je stejne vsechno naprd)
		
		RGBa(const libcan_component r, const libcan_component g, const libcan_component b);
		//alpha = 255, ne 0 :)
		
		RGBa(const libcan_component r, const libcan_component g, const libcan_component b, const libcan_component a);
		
		RGBa operator+(const RGBa& other) const;
		//secte dve barvy, ale na zaklade "vahy" alphy
		
		RGBa operator-() const {
			return RGBa(0,0,255);
		}
		
		bool operator==(const RGBa& other) const;
		bool operator>=(const RGBa& other) const;
		
		RGBa operator*(const libcan_float quoc) const;
		//vynasobeni nejakym cislem
		
		RGBa operator*(const RGBa& what) const;
		//prumer
		
	};
}

#endif
