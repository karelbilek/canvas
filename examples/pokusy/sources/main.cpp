#include "interval.h"
#include <iostream>
#include <string>

using namespace libcan;



int main () {
	interval<int> it(3,4,30);
	it.add_more(1,2,10);
	it.add_more(5,5,20);
	
	bool aaa;
	(it.half(aaa, false))->print_all();
	
	
	
	
	return 0;
}
