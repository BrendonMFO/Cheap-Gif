#ifndef _LIB_IMAGE_LIST_
#define _LIB_IMAGE_LIST_

typedef struct s_Colors
{
  int r;
  int g;
  int b;
  int a;
} Colors;

typedef struct s_Image
{
  struct s_Image *prev;
  int width;
  int height;
  Colors *colors;
  struct s_Image *next;
} Image;

typedef struct s_List_Images
{
  Image *first;
  Image *last;
} List_Image;

typedef struct s_Render
{
  Colors colors[200][200];
  int width;
  int height;
  int render_pos_x;
  int render_pos_y;
} Render;

void image_list_init(List_Image *_list_image);

void image_list_free(List_Image *_list);

Colors *image_list_push(List_Image *_list_image, int width, int height);

void move_cursor(int _x, int _y);

Render make_render(Image *_image);

#endif
