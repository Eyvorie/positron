#ifndef ALLOC_H
#define ALLOC_H
#include <stdlib.h>

#define new(type) ((type *)malloc(sizeof(type)))
#define array_of(type, n) ((type *)malloc(sizeof(type) * n))
#endif