#include "interval.h"
#include <iostream>
#include <string>

using namespace libcan;


void print_all(const interval<bool>& what) {
	std::cout<<what.get_this()<<" od "<<what.get_start()<<" k "<<what.get_end()<<"\n";
	std::cout<<"vlevo:\n";
	if (what.has_left()) {
		print_all(what.get_left());
	}
	if (what.has_right()) {
		print_all(what.get_right());
	}
}

int main () {
	interval<bool> it(10,20,1);
	
	interval<bool>* it2 = it.negative(1, 0,30); 
	print_all(*it2);
	
	
	return 0;
}
