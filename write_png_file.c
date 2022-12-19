#include <stdlib.h>
#include <stdio.h>
#include "libpng/png.h"

/*
 * This function is from https://gist.github.com/niw/5963798, modified for our purposes
 *
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * To build, use the next instruction on OS X.
 * $ brew install libpng
 * $ clang -lz -lpng16 libpng_test.c
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */
void write_png_file(char *filename, int width, int height, png_bytep *row_pointers) {
  //int y; //unused variable, not sure why this was here -Ajai

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  if (!row_pointers) abort();

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

	/*
  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);
	*/

  fclose(fp);

  png_destroy_write_struct(&png, &info);
}

void array_to_png(char *filename, int width, int height, unsigned char *array) {
	printf("Printing!\n");
	// NOTE: png_byte = unsigned char
	//       png_bytep = png_byte* = unsigned char*
	png_bytep *row_pointers = malloc(height * sizeof(png_bytep));
	for (int y = 0; y < height; y++) {
		row_pointers[y] = array + (y * width * 4);
	}
	write_png_file(filename, width, height, row_pointers);
	free(row_pointers);
}
