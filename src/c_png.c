#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>
#include <pthread.h>
#include "../lib/render/c_render.h"

static int LENGTH = -1;

void *make_forks(void *ptr);
void fork_father(int *_pipe_father, int *_pipe_son);
void fork_son(int *_pipe_father, int *_pipe_son, int _render_x);

int main(void)
{
  int thr1, thr2;
  pthread_t thread, thread2, thread3;

  printf("For see better, please zoom out.\n");
  printf("...Press Enter...");
  getchar();
  move_cursor(0, 0);

  pthread_create(&thread, NULL, make_forks, NULL);
  pthread_create(&thread2, NULL, make_forks, NULL);
  make_forks(NULL);

  return (0);
}

void *make_forks(void *ptr)
{
  int pipes[2];
  pid_t childpid;
  pipe(pipes);

  if ((childpid = fork()) == -1)
  {
    perror("fork");
    exit(1);
  }

  if (childpid == 0)
    fork_father(&pipes[0], &pipes[1]);
  else
    fork_son(&pipes[0], &pipes[1], ++LENGTH);

  return NULL;
}

void fork_father(int *_pipe_father, int *_pipe_son)
{
  close(*_pipe_father);

  List_Image *temp = image_process_init("./images/");

  render(temp, _pipe_son);

  exit(0);
}

void fork_son(int *_pipe_father, int *_pipe_son, int _render_x)
{
  Render readbuffer;

  close(*_pipe_son);

  while ((read(*_pipe_father, &readbuffer, sizeof(readbuffer)) != 0))
  {
    write_render(&readbuffer, _render_x);
  }
}
