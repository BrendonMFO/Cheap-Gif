#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <unistd.h>
#include "read_images.h"
#include "../list_string/list_string.h"

typedef struct s_Read_Image
{
  int width;
  int height;
  png_byte color_type;
  png_byte bit_depth;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep *row_pointers;
} Read_Image;

void process_file(List_Image *_list, Read_Image *_image_info);
int read_png_file(Read_Image *_image_info, const char *file_name);
void organize_images(list_string *_list, const char *_image_folder);
void read_all_files(list_string *_list, List_Image *_list_images, Read_Image *_image_info);

List_Image *image_process_init(const char *_image_folder)
{
  list_string *list = (list_string *)malloc(sizeof(list_string) * 1);
  List_Image *list_images = (List_Image *)malloc(sizeof(List_Image) * 1);
  Read_Image *image_info = (Read_Image *)malloc(sizeof(Read_Image) * 1);

  list_string_init(list);
  image_list_init(list_images);

  if (list == NULL || list_images == NULL || image_info == NULL)
  {
    exit(-1);
  }

  organize_images(list, _image_folder);
  read_all_files(list, list_images, image_info);

  list_string_free(list);
  free(list);
  free(image_info->row_pointers);
  free(image_info);

  return list_images;
}

void organize_images(list_string *_list, const char *_image_folder)
{
  DIR *d = opendir(_image_folder);
  list_string_data *aux;
  struct dirent *dir;
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      list_string_push(_list, _image_folder, dir->d_name);
    }
    closedir(d);
  }
}

void read_all_files(list_string *_list, List_Image *_list_images, Read_Image *_image_info)
{
  list_string_data *aux;
  for (aux = _list->first; aux != NULL; aux = aux->next)
  {
    if (read_png_file(_image_info, aux->text) == 1)
    {
      process_file(_list_images, _image_info);
    }
  }
}

int read_png_file(Read_Image *_image_info, const char *file_name)
{
  char header[8];
  int x, y;

  FILE *fp = fopen(file_name, "rb");

  if (!fp || _image_info == NULL)
    return -1;

  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8))
    return -1;

  _image_info->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!_image_info->png_ptr)
    return -1;

  _image_info->info_ptr = png_create_info_struct(_image_info->png_ptr);
  if (!_image_info->info_ptr)
    return -1;

  if (setjmp(png_jmpbuf(_image_info->png_ptr)))
    return -1;

  png_set_gray_to_rgb(_image_info->png_ptr);
  png_set_palette_to_rgb(_image_info->png_ptr);

  png_init_io(_image_info->png_ptr, fp);
  png_set_sig_bytes(_image_info->png_ptr, 8);

  png_read_info(_image_info->png_ptr, _image_info->info_ptr);

  _image_info->width = (png_get_image_width(_image_info->png_ptr, _image_info->info_ptr));
  _image_info->height = png_get_image_height(_image_info->png_ptr, _image_info->info_ptr);
  _image_info->color_type = png_get_color_type(_image_info->png_ptr, _image_info->info_ptr);
  _image_info->bit_depth = png_get_bit_depth(_image_info->png_ptr, _image_info->info_ptr);

  png_set_interlace_handling(_image_info->png_ptr);
  png_read_update_info(_image_info->png_ptr, _image_info->info_ptr);

  if (setjmp(png_jmpbuf(_image_info->png_ptr)))
    return -1;

  _image_info->row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * _image_info->height);

  for (y = 0; y < _image_info->height; y++)
  {
    _image_info->row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(_image_info->png_ptr, _image_info->info_ptr));
  }

  png_read_image(_image_info->png_ptr, _image_info->row_pointers);

  fclose(fp);

  return 1;
}

void process_file(List_Image *_list, Read_Image *_image_info)
{
  int offset;
  int pY, pX;

  png_get_color_type(_image_info->png_ptr, _image_info->info_ptr);
  png_get_color_type(_image_info->png_ptr, _image_info->info_ptr);

  Colors *colors = image_list_push(_list, _image_info->width, _image_info->height);

  for (pY = 0; pY < _image_info->height; pY++)
  {
    png_byte *row = _image_info->row_pointers[pY];
    for (pX = 0; pX < _image_info->width; pX++)
    {
      png_byte *ptr = &(row[pX * 4]);
      offset = pY * _image_info->width + pX;
      colors[offset].r = ptr[0];
      colors[offset].g = ptr[1];
      colors[offset].b = ptr[2];
      colors[offset].a = ptr[3];
    }
  }
}