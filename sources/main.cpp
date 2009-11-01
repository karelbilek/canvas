#include "curve.h"
#include "moved_arrays.h"

int main () {
	glib::moved_arrays marray(10,20);
	
	std::cout<<marray.is_set(15)<<"\n";
	marray.set(20,15);
	
	std::cout<<marray.is_set(15)<<" "<<marray.get_start(15)<<" "<<marray.get_end(15)<<"\n";
	marray.set(21,15);
	std::cout<<marray.is_set(15)<<" "<<marray.get_start(15)<<" "<<marray.get_end(15)<<"\n";
	
	glib::moved_arrays sarray(marray);
	std::cout<<sarray.is_set(15)<<" "<<sarray.get_start(15)<<" "<<sarray.get_end(15)<<"\n";
	glib::moved_arrays sarrai(1,2);
	sarrai = sarray;
	std::cout<<sarrai.is_set(15)<<" "<<sarrai.get_start(15)<<" "<<sarrai.get_end(15)<<"\n";
	
	return 0;
}