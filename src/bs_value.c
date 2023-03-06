#include "bs_value.h"

bs_value * new_value(bs_value value) {
  bs_value * ptr = malloc(sizeof(bs_value));
  if (ptr) *ptr = value;
  return ptr;
}  

void free_value(bs_value * value) {
  free(value);
}