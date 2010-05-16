#ifndef RGBA_INC
#define RGBA_INC

#include "types.h"
//#include <iostream>



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
		 * Ve skutecnosti ulozeno jako POLE (!) ctyr hodnot unsigned char,
		 * unsigned char = 0...255
		 * 
		 * Pole proto, ze to teoreticky urychluje pristup k barvam, kdyz to pak kladu do PNG
		 * (ta rychlost stejne neni nijak extra velka, ale v jednu chvili vyvoje jsem chtel zrychlit jakkoliv cokoliv)
		 */
	private:
		unsigned char _red;
		unsigned char _green;
		unsigned char _blue;
		unsigned char _alpha;
		
	public:
		
		bool is_not_transparent() const;
		
		void set_alpha(const unsigned char alpha);
		void set_red(const unsigned char red);
		void set_green(const unsigned char green);
		void set_blue(const unsigned char blue);
		
		unsigned char get_green() const;
		unsigned char get_red() const;
		unsigned char get_blue() const;
		unsigned char get_alpha() const;
		
		
		void get_colors_pointer(unsigned char* p_red, unsigned char* p_green, unsigned char* p_blue, unsigned char* p_alpha) const;
		
		void get_colors(unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &alpha) const;
		
		RGBa(const basic_colors color);
		
		RGBa();
		//0/0/0/0, myslim (s 0 alphou je stejne vsechno naprd)
		
		RGBa(const unsigned char r, const unsigned char g, const unsigned char b);
		//alpha = 255, ne 0 :)
		
		RGBa(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
		
		RGBa operator+(const RGBa& other) const;
		//secte dve barvy, ale na zaklade "vahy" alphy
		
		RGBa operator-() const {
			return RGBa(0,0,255);
		}
		
		bool operator==(const RGBa& other) const;
		bool operator>=(const RGBa& other) const;
		
		RGBa operator*(const double quoc) const;
		//vynasobeni nejakym cislem
		
		RGBa operator*(const RGBa& what) const;
		//prumer
		
	};
}


//std::ostream& operator<<(std::ostream& out, const libcan::RGBa& c);

#endif
