#ifndef __UTIL_INCLUDED__
#define __UTIL_INCLUDED__

#include <Arduino.h>
#include <stdarg.h>

class Util {
  public:
    static void print(char *fmt, ... );
    static void quickSort(int *arr, int left, int right);

  private:
    static void swap(int *arr, int i, int j);
    static int partition(int *arr, int l, int r);
};

#endif


