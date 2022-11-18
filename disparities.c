int *disparities(int height, int width, unsigned char *left_grayscaled, unsigned char *right_grayscaled){
  int *disparities = malloc(height * width * sizeof(int))
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
}
