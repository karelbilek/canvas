#include "shape.h"
#include "all_shapes.h"

#include <iostream>
#include <string>
#include <map>
#include <iostream>



using namespace libcan;





int main () {
	
	
	shape s(shape_style(100, RGBa(0,0,0,255), RGBa(255,0,255,255)), regular(point(308,409), point(75,88), 5));
	
	std::set<std::string> myset = s.get_properties();
	for (std::set<std::string>::iterator it=myset.begin(); it!=myset.end(); it++)
	    std::cout << " " << *it;
	
	
	return 0;
}
