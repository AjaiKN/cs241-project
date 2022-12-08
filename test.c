#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "readpng.h"
#include "write_png_file.h"

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
	assert((*channels) == 4);
	assert((*width)*(*channels) == rowbytes);

	readpng_cleanup(false);

	fclose(fptr);

	printf("size of image_data should be height*width*channels = %lu*%lu*%d = %lu\n", *height, *width, *channels, (*height)*(*width)*(*channels));
}

double distance(unsigned char *left, unsigned char *right) {
	int d0 = left[0] - right[0];
	int d1 = left[1] - right[1];
	int d2 = left[2] - right[2];
	return sqrt(d0*d0 + d1*d1 + d2*d2);
}

int main() {
	unsigned long left_height, left_width;
	int left_channels;
	unsigned char *left_image_data; // height*width*channels
	png_to_array("L_00001.png", &left_image_data, &left_height, &left_width, &left_channels);
	//unsigned char left_image_data_arr[left_height][left_width][left_channels];
	//memcpy(left_image_data_arr, left_image_data, left_height*left_width*left_channels*sizeof(unsigned char));

	array_to_png("input_dup.png", left_width, left_height, left_image_data);

	unsigned long right_height, right_width;
	int right_channels;
	unsigned char *right_image_data;
	png_to_array("R_00001.png", &right_image_data, &right_height, &right_width, &right_channels);
	//unsigned char right_image_data_arr[right_height][right_width][right_channels];
	//memcpy(right_image_data_arr, right_image_data, right_height*right_width*right_channels*sizeof(unsigned char));

	assert(left_height == right_height);
	assert(left_width == right_width);
	assert(left_height < INT_MAX);
	assert(left_width < INT_MAX);
	int width = (int) left_width;
	int height = (int) left_height;

	int *disparities = malloc(height * width * sizeof(int));
	for (int y = 0; y < height; y++) {
		for (int x_left = 0; x_left < width; x_left++) {
			bool started = false;
			double min_difference = 0;
			int best_x_right = -1;
			for (int x_right = x_left - 150; x_right < x_left + 150; x_right++) {
				if (!(x_right >= 0 && x_right < width)) continue;
				double diff = distance(&left_image_data[(width*y + x_left)*4], &right_image_data[(width*y + x_right)*4]);
				if (!started || diff < min_difference) {
					min_difference = diff;
					best_x_right = x_right;
					started = true;
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
			double disparity_scaled = (double) disparity / 150.0;
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
	/*
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	for (int i = 0; i < width; i++) printf("%u ", output_image[i * 4]);
	*/
	array_to_png("output.png", width, height, output_image);

	free(output_image);
	free(disparities);
	free(left_image_data);
	free(right_image_data);
}
