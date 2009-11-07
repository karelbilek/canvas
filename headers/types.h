#ifndef TYPES_INC
#define TYPES_INC


#include <limits.h>

#include <utility>
// 
// 
 #include <vector>
#include <map>
#include <string>
//#include <iostream>
#include <list>

#define __PI (4*atan(1))
#define __DEG2RAD(_co) (static_cast<glib_float>(((_co)/180)*__PI))
#define __RAD2DEG(_co) (static_cast<glib_float>(((_co)/__PI)*180))

#define __minimum(_a,_b) (((_a)<(_b))?(_a):(_b))
#define __maximum(_a,_b) (((_a)>(_b))?(_a):(_b))
#define __abs2(a) (((a)>0)?((a)):(-(a)))

	//tyhle makra se straaaasne hodi


	//jeden pokus o zaokrouhlovani - nakonec mi prislo nejlepsi to jednoduse truncnout
//#define floating_to_integer(__co) (((__co)>0)?((((__co) - floating_to_integer(__co)) <= 0.5)?(floating_to_integer(__co)):(floating_to_integer(__co)+1)):((((__co) - floating_to_integer(__co)) >= -0.5)?(floating_to_integer(__co)):(floating_to_integer(__co)+1)))


namespace glib {
	
	typedef unsigned char small;
	typedef unsigned long glib_uint;
	typedef long glib_int; 
	typedef unsigned char glib_component;
	typedef float glib_float;

}	
	
#define GLIB_INT_MAX LONG_MAX
#define glib_float_to_int(_fx) (((_fx)<0)?(((_fx) == static_cast<glib_int>(_fx))?(static_cast<glib_int>(_fx)):(static_cast<glib_int>(_fx)-1)):(static_cast<glib_int>(_fx)))
	//truncnuti, ale u zapornych jeste zmensi o 1 (tvary muzou jit i do "nekreslene" casti plochy)

#define __ceiling(___float) ((((___float) - glib_int((___float)))>0)?(glib_int(___float)+1):(glib_int(___float)))

#endif
