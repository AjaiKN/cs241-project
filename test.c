#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "readpng.h"

// also see https://gist.github.com/niw/5963798
// That might be simpler

int main() {
	readpng_version_info();

	FILE *fptr;
	if ((fptr = fopen("L_00001.png", "r")) == NULL) {
		printf("Error! opening file\n");
		exit(1);
	}

	unsigned long width = 0;
	unsigned long height = 0;
	int init_ret = readpng_init(fptr, &width, &height);
	printf("readpng_init returned %d \t width=%lu height=%lu\n", init_ret, width, height);

	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
	int bg_ret = readpng_get_bgcolor(&red, &green, &blue);
	printf("readpng_get_bgcolor returned %d \t red=%u green=%u blue=%u\n", bg_ret, red, green, blue);

	double display_exponent = 1;
	int channels = 0;
	unsigned long rowbytes = 0;
	unsigned char *image_data;
	image_data = readpng_get_image(display_exponent, &channels, &rowbytes);
	printf("channels=%d rowbytes=%lu\n", channels, rowbytes);

	readpng_cleanup(false);

	fclose(fptr);

	printf("rowbytes should be width*channels = %lu*%d = %lu\n", width, channels, width*channels);
	printf("size of image_data should be height*width*channels = %lu*%lu*%d = %lu\n", height, width, channels, height*width*channels);

	free(image_data);
}

