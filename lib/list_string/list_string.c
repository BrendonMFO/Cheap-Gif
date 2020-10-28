#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_string.h"
#include "../order_string/order_string.h"

int list_string_ordened_push(list_string *list, list_string_data *data);

void list_string_init(list_string *list)
{
  list->size = 0;
  list->first = NULL;
  list->last = NULL;
}

int list_string_ordened_push(list_string *list, list_string_data *data)
{
  if (list == NULL || data == NULL)
  {
    return 0;
  }

  list_string_data *aux = list->first;

  if (compare_string_case(data->text, aux->text) == 1)
  {
    data->prev = aux->prev;
    data->next = aux;
    aux->prev = data;
    list->first = data;
    return 1;
  }

  while ((aux = aux->next) != NULL)
  {
    if (compare_string_case(data->text, aux->text) == 1)
    {
      aux->prev->next = data;
      data->prev = aux->prev;
      aux->prev = data;
      data->next = aux;
      return 1;
    }
  }

  data->prev = list->last;
  data->next = NULL;
  list->last->next = data;
  list->last = data;

  return 1;
}

int list_string_push(list_string *list, const char *_folder, const char *text)
{
  list_string_data *aux = (list_string_data *)malloc(sizeof(list_string_data));

  if (list == NULL || text == NULL || aux == NULL)
  {
    return 0;
  }

  aux->text = (char *)malloc(sizeof(char) * (strlen(_folder) + strlen(text)) + 1);

  if (aux->text == NULL)
  {
    return 0;
  }

  strcpy(aux->text, _folder);
  strcat(aux->text, text);

  if (list->first == NULL && list->last == NULL)
  {
    aux->index = 0;
    aux->next = NULL;
    aux->prev = NULL;
    list->first = aux;
    list->last = aux;
  }
  else
  {
    list_string_ordened_push(list, aux);
  }

  list->size++;

  return 1;
}

void list_string_free(list_string *list)
{
  list_string_data *aux = list->first, *temp;

  while ((temp = aux) != NULL)
  {
    aux = aux->next;
    free(temp);
  }
}
