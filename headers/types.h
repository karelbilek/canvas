#ifndef TYPES_INC
#define TYPES_INC


#include <limits.h>


#include <vector>
#include <map>
#include <cmath>
#include <list>

#define __PI (static_cast<libcan::libcan_float>(4*atan(1)))
#define __DEG2RAD(_co) (static_cast<libcan::libcan_float>((((libcan::libcan_float)_co)/180.0)*__PI))
#define __RAD2DEG(_co) (static_cast<libcan::libcan_float>((((libcan::libcan_float)_co)/__PI)*180.0))

#define __minimum(_a,_b) (((_a)<(_b))?(_a):(_b))
#define __maximum(_a,_b) (((_a)>(_b))?(_a):(_b))
#define __minimum4(_a,_b,_c,_d) (((_a)<(_b))?(((_c)<(_d))?(__minimum((_a),(_c))):(__minimum((_a),(_d)))):(((_c)<(_d))?(__minimum((_b),(_c))):(__minimum((_b),(_d)))))

#define __maximum4(_a,_b,_c,_d) (((_a)>(_b))?(((_c)>(_d))?(__maximum((_a),(_c))):(__maximum((_a),(_d)))):(((_c)>(_d))?(__maximum((_b),(_c))):(__maximum((_b),(_d)))))


#define __abs(a) (((a)>0)?((a)):(-(a)))




namespace libcan {
	
	typedef unsigned char small;
	typedef unsigned long libcan_uint;
	typedef long libcan_int; 
	typedef unsigned char libcan_component;
	typedef double libcan_float;
	


}	
	
#define libcan_INT_MAX LONG_MAX
#define libcan_float_to_int(_fx) (((_fx)<0)?(((_fx) == static_cast<libcan_int>(_fx))?(static_cast<libcan_int>(_fx)):(static_cast<libcan_int>(_fx)-1)):(static_cast<libcan_int>(_fx)))
	//truncnuti, ale u zapornych jeste zmensi o 1 (tvary muzou jit i do "nekreslene" casti plochy)

#define __ceiling(___float) ((((___float) - libcan_int((___float)))>0)?(libcan_int(___float)+1):(libcan_int(___float)))

#endif
