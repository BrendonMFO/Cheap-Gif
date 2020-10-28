compile:
	gcc -c -g lib/image_list/image.c
	gcc -c -g lib/image_list/image.c
	gcc -c -g lib/image_list/read_images.c
	gcc -c -g lib/list_string/list_string.c
	gcc -c -g lib/order_string/order_string.c
	gcc -c -g lib/render/c_render.c
	gcc -g src/c_png.c -o cheap_gif *.o -lpng -pthread
	rm *.o