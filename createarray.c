#inlcude <stdio.h>


int createArray(char *name, int *height, int *width, int *channels){
  unsigned long height, width;
  int channels;
  unsigned char *image_data;
  char filename[50];
  printf("Input png filename: ");
  scanf("%s", &filename);
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL){
    printf("File not found!");
    exit(1);
  }

  

  
  png_to_array("L_00001.png", &left_image_data, &left_height, &left_width, &left_channels);
  unsigned char *left_grayscaled = malloc(left_height * left_width * sizeof(unsigned char));
  to_grayscale(left_height, left_width, left_image_data, left_grayscaled);

}
