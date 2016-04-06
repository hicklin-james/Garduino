#include "Util.h"

void Util::print(char *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.print(buf);
}

void Util::quickSort(int *arr, int left, int right) {
  if ( left >= right ) {
    return;
  }

  int part = Util::partition(arr, left, right);
  Util::quickSort(arr, left, part-1);
  Util::quickSort(arr, part+1, right);
}

// private methods

void Util::swap(int *arr, int i, int j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

int Util::partition(int *arr, int left, int right) {
  int mid = left + (right - left) / 2;
  int pivot = arr[mid];
  Util::swap(arr, mid, left);
  int i = left + 1;
  int j = right;
  while (i <= j) {
    while(i <= j && arr[i] <= pivot) {
      i++;
    }

    while(i <= j && arr[j] > pivot) {
      j--;
    }

    if (i < j) {
      Util::swap(arr, i, j);
    }
  }
  Util::swap(arr, i-1, left);
  return i - 1;
}
