#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../image_list/read_images.h"

void render(List_Image *_list, int *_pipe)
{
  Image *aux;
  Render var_render;

  while (1)
  {
    for (aux = _list->first; aux != NULL; aux = aux->next)
    {
      var_render = make_render(aux);
      write(*_pipe, &var_render, sizeof(Render));
    }
    for (aux = _list->last; aux != NULL; aux = aux->prev)
    {
      var_render = make_render(aux);
      write(*_pipe, &var_render, sizeof(Render));
    }
  }
}

void write_render(Render *_render, int _start_pos_render)
{
  char pos_x, pos_y, a[50], t[200];
  int init_render = (_start_pos_render * 120) + 10;

  move_cursor(10, init_render);

  for (pos_y = 0; pos_y < _render->height; pos_y++)
  {
    for (pos_x = 0; pos_x < _render->width; pos_x++)
    {
      sprintf(a, "\033[%d;%dH\e[48;2;%d;%d;%dm ",
              pos_y + 10,
              init_render + pos_x,
              _render->colors[pos_x][pos_y].r,
              _render->colors[pos_x][pos_y].g,
              _render->colors[pos_x][pos_y].b);

      write(1, a, strlen(a));
    }
  }

  move_cursor(10, init_render);
}