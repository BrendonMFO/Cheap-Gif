#include <stdio.h>
#include <string.h>
#include "order_string.h"

int compare_string_case(const char *_string_1, const char *_string_2)
{
  int i, j;
  int string_1_length;
  int string_2_length;

  if (_string_1 == NULL || _string_2 == NULL)
  {
    return 1;
  }

  string_1_length = strlen(_string_1);
  string_2_length = strlen(_string_2);

  for (i = 0; i < string_1_length || i < string_2_length; i++)
  {
    if (_string_1[i] < _string_2[i])
    {
      return 1;
    }
    if (_string_1[i] > _string_2[i])
    {
      return 2;
    }
  }

  return string_1_length < string_2_length ? 1 : 2;
}
