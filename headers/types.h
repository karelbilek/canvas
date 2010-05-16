#ifndef TYPES_INC
#define TYPES_INC


#include <limits.h>


#include <vector>
#include <map>
#include <cmath>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <set>
#include <string>
#include <sstream>


//tohle je nutne - v klasickych c++ knihovnach PI proste neni.
//nekde lze pouzit M_PI v math.h, ale neni to ve standardu c++ 
#define __PI (static_cast<double>(4*atan(1)))
#define __DEG2RAD(_co) (static_cast<double>((((double)_co)/180.0)*__PI))
#define __RAD2DEG(_co) (static_cast<double>((((double)_co)/__PI)*180.0))


#endif
