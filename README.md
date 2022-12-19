# Stereo Reconstruction

We have adhered to the Honor Code in this assignment.

Group 10: Ajai Nelson, Elinor Frost, Jason Kuchtey and Kevin Dong

## Building

First, go into the libpng directory and build it using the following commands:

```bash
cd libpng
./configure
make
cd .. # Go back to the project directory
```

Then, build our project:

```bash
make
```

## Running

To run our program, you need to pass it a left image filename, a right image filename, and an output filename. For example, you can run it with the example images in this repository:
```bash
./depthmap L_00001.png R_00001.png output.png
```
