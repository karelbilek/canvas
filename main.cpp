#include "plane.h"
 
int main () {
	glib::plane<bool> kk(10,20,30,50,false);
	glib::plane<int> kl(kk,888);
	
	//kl=kk;
	
	return 0;
}