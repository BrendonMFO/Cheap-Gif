#ifndef _LIB_LIST_STRING_
#define _LIB_LIST_STRING_

typedef struct s_list_string_data
{
  struct s_list_string_data *prev;
  int index;
  char *text;
  struct s_list_string_data *next;
} list_string_data;

typedef struct s_list_string
{
  int size;
  list_string_data *first;
  list_string_data *last;
} list_string;

void list_string_init(list_string *list);

void list_string_free(list_string *list);

int list_string_push(list_string *list, const char *_folder, const char *text);

#endif
