#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define arrayOf(T) struct { T* buffer; size_t count, capacity; }
#define arrInit(arr)                                                          \
  do                                                                          \
    {                                                                         \
      (arr)->count = 0;                                                       \
      (arr)->capacity = 0;                                                    \
      (arr)->buffer = NULL;                                                   \
    }                                                                         \
  while (0)

#define arrAppend(arr, item)                                                  \
  do                                                                          \
    {                                                                         \
      if ((arr)->count >= (arr)->capacity)                                    \
        {                                                                     \
          (arr)->capacity = (arr)->capacity == 0 ? 4 : (arr)->capacity * 2;   \
          (arr)->buffer = realloc ((arr)->buffer, sizeof (*(arr)->buffer)     \
                                                      * (arr)->capacity);     \
        }                                                                     \
      (arr)->buffer[(arr)->count++] = item;                                   \
    }                                                                         \
  while (0)

#define arrCount(arr) (arr)->count
#define arrGet(arr, n) (arr)->buffer[n]
#define arrLast(arr) (arr)->buffer[(arr)->count - 1]
#define arrForEach(arr, item)                                                 \
  for (typeof (*(arr)->buffer) *item = (arr)->buffer;                         \
       item != (arr)->buffer + (arr)->count; item++)

// TODO: arrFree

