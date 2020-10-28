#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void image_list_init(List_Image *_list_image)
{
  _list_image->first = NULL;
  _list_image->last = NULL;
}

Colors *image_list_push(List_Image *_list_image, int width, int height)
{
  Image *aux = (Image *)malloc(sizeof(Image) * 1);
  if (aux == NULL)
  {
    exit(-1);
  }
  aux->colors = (Colors *)malloc(height * width * sizeof(Colors));
  aux->width = width;
  aux->height = height;
  aux->next = NULL;
  if (_list_image->first == NULL && _list_image->last == NULL)
  {
    aux->prev = NULL;
    _list_image->first = aux;
    _list_image->last = aux;
  }
  else
  {
    aux->prev = _list_image->last;
    _list_image->last->next = aux;
    _list_image->last = aux;
  }
  return aux->colors;
}

void image_list_free(List_Image *_list)
{
  Image *aux = _list->first, *temp;

  while ((temp = aux) != NULL)
  {
    aux = aux->next;
    free(temp->colors);
    free(temp);
  }
}

void move_cursor(int _x, int _y)
{
  char b[30];
  sprintf(b, "\033[%d;%dH", _x, _y);
  write(1, b, strlen(b));
}

Render make_render(Image *_image)
{
  Render var_render;
  int px, py, offset;

  var_render.width = _image->width;
  var_render.height = _image->height;

  for (py = 0; py < _image->height; py++)
  {
    for (px = 0; px < _image->width; px++)
    {
      offset = py * _image->width + px;
      var_render.colors[px][py] = _image->colors[offset];
    }
  }
  return var_render;
}