# for debugging
CC = gccx
# for speed
#CC = clang -O3 -funroll-loops

MATH=-lm
ZLIB=-lz
LIBPNG=libpng/pngerror.o libpng/pngget.o libpng/pngmem.o libpng/png.o libpng/pngpread.o libpng/pngread.o libpng/pngrio.o libpng/pngrtran.o libpng/pngrutil.o libpng/pngset.o libpng/pngtrans.o libpng/pngwio.o libpng/pngwrite.o libpng/pngwtran.o libpng/pngwutil.o $(MATH) $(ZLIB)

EXECUTABLES=depthmap

all: $(EXECUTABLES)

depthmap: depthmap.o readpng.o write_png_file.o
	$(CC) -o $@ $^ $(LIBPNG)

depthmap.o: depthmap.c readpng.h write_png_file.h
	$(CC) -c -o $@ $<
readpng.o: readpng.c
	$(CC) -c -o $@ $<
write_png_file.o: write_png_file.c
	$(CC) -c -o $@ $<

clean:
	rm $(EXECUTABLES) *.o
