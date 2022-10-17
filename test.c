#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "readpng.h"

void to_grayscale(unsigned long height, unsigned long width, unsigned char *colored, unsigned char *gray) {
	unsigned char *coloredPtr = colored;
	unsigned char *grayPtr = gray;
	for (unsigned long y = 0; y < height; y++) {
		for (unsigned long x = 0; x < width; x++) {
			double red   = *(coloredPtr + 0);
			double green = *(coloredPtr + 1);
			double blue  = *(coloredPtr + 2);
			//unsigned char alpha = *(coloredPtr + 3);
			// https://e2eml.school/convert_rgb_to_grayscale.html
			*grayPtr = round(0.299*red + 0.587*green + 0.114*blue);
			coloredPtr += 4;
			grayPtr += 1;
		}
	}
}

// also see https://gist.github.com/niw/5963798
// That might be simpler
void png_to_array(char* filename, unsigned char **array_ptr, unsigned long *height, unsigned long *width, int *channels) {
	readpng_version_info();

	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL) {
		printf("Error! opening file\n");
		exit(1);
	}

	int init_ret = readpng_init(fptr, width, height);
	printf("readpng_init returned %d \t width=%lu height=%lu\n", init_ret, *width, *height);

	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
	int bg_ret = readpng_get_bgcolor(&red, &green, &blue);
	printf("readpng_get_bgcolor returned %d \t red=%u green=%u blue=%u\n", bg_ret, red, green, blue);

	unsigned long rowbytes = 0;
	*array_ptr = readpng_get_image(channels, &rowbytes);
	printf("channels=%d rowbytes=%lu\n", *channels, rowbytes);

	readpng_cleanup(false);

	fclose(fptr);

	printf("rowbytes should be width*channels = %lu*%d = %lu\n", *width, *channels, (*width)*(*channels));
	printf("size of image_data should be height*width*channels = %lu*%lu*%d = %lu\n", *height, *width, *channels, (*height)*(*width)*(*channels));
}

int main() {
	unsigned long height, width;
	int channels;
	unsigned char *image_data;
	png_to_array("L_00001.png", &image_data, &height, &width, &channels);

	unsigned char *ptr = image_data;
	for (unsigned long y = 0; y < height; y++) {
		for (unsigned long x = 0; x < width; x++) {
			if (y == 0) printf("%u %u %u %u\n", ptr[0], ptr[1], ptr[2], ptr[3]);
			ptr += 4;
		}
	}

	unsigned char *grayscaled = malloc(height * width * sizeof(unsigned char));
	to_grayscale(height, width, image_data, grayscaled);

	for (int i = 0; i < 100; i++) printf("%u\n", grayscaled[i]);

	free(image_data);
}
