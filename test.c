#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include "readpng.h"
#include "write_png_file.h"

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
	unsigned long left_height, left_width;
	int left_channels;
	unsigned char *left_image_data; // height*width*channels
	png_to_array("L_00001.png", &left_image_data, &left_height, &left_width, &left_channels);
	unsigned char *left_grayscaled = malloc(left_height * left_width * sizeof(unsigned char)); // height*width
	to_grayscale(left_height, left_width, left_image_data, left_grayscaled);

	array_to_png("input_dup.png", left_width, left_height, left_image_data);

	unsigned char *full_grayscaled = malloc(left_height*left_width*left_channels*sizeof(unsigned char));
	for (unsigned long y = 0; y < left_height; y++) {
		for (unsigned long x = 0; x < left_width; x++) {
			unsigned long index = y*left_width + x;
			unsigned long index_full = index * 4;
			unsigned char val = left_grayscaled[index];
			full_grayscaled[index_full + 0] = val;
			full_grayscaled[index_full + 1] = val;
			full_grayscaled[index_full + 2] = val;
			full_grayscaled[index_full + 3] = 255;
		}
	}
	array_to_png("left_gray.png", left_width, left_height, full_grayscaled);

	unsigned long right_height, right_width;
	int right_channels;
	unsigned char *right_image_data;
	png_to_array("R_00001.png", &right_image_data, &right_height, &right_width, &right_channels);
	unsigned char *right_grayscaled = malloc(right_height * right_width * sizeof(unsigned char));
	to_grayscale(right_height, right_width, right_image_data, right_grayscaled);

	assert(left_height == right_height);
	assert(left_width == right_width);
	assert(left_height < INT_MAX);
	assert(left_width < INT_MAX);
	int width = (int) left_width;
	int height = (int) left_height;

	int *disparities = malloc(height * width * sizeof(int));
	for (int y = 0; y < height; y++) {
		for (int x_left = 0; x_left < width; x_left++) {
			int min_difference = INT_MAX;
			int best_x_right = -1;
			for (int x_right = 0; x_right < width; x_right++) {
				int diff = abs(left_grayscaled[width*y + x_left] - right_grayscaled[width*y + x_right]);
				if (diff < min_difference) {
					min_difference = diff;
					best_x_right = x_right;
				}
			}
			disparities[width*y + x_left] = abs(best_x_right - x_left);
		}
	}
	for (int i = 0; i < width / 4; i++) printf("%d ", disparities[i]);
	printf("\n");
	printf("\n");
	for (int i = 0; i < width / 4; i++) printf("%d ", disparities[width/4 + i]);
	printf("\n");

	unsigned char *output_image = malloc(height * width * 4 * sizeof(int));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int disparities_index = width*y + x;
			int disparity = disparities[disparities_index];
			int output_image_index = disparities_index * 4;
			double disparity_scaled = (double) disparity / (double) width;
			double distance = 1.0 / disparity_scaled;
			double distance_scaled = 255.0 / distance;
			assert(distance_scaled >= 0);
			assert(distance_scaled <= 255);
			unsigned char distance_output = (unsigned char) distance_scaled;
			output_image[output_image_index + 0] = distance_output; //red
			output_image[output_image_index + 1] = distance_output; //green
			output_image[output_image_index + 2] = distance_output; //blue
			output_image[output_image_index + 3] = 255;             //alpha
		}
	}
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	for (int i = 0; i < width; i++) printf("%u ", output_image[i * 4]);
	array_to_png("output.png", width, height, output_image);

	free(output_image);
	free(disparities);
	free(left_image_data);
	free(right_image_data);
}
