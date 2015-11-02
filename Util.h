#ifndef __UTIL_INCLUDED__
#define __UTIL_INCLUDED__

#include <Arduino.h>
#include <stdarg.h>

class Util {
  public:
    static void print(char *fmt, ... );

};

#endif