MATH=-lm
ZLIB=-lz
LIBPNG=libpng/pngerror.o libpng/pngget.o libpng/pngmem.o libpng/png.o libpng/pngpread.o libpng/pngread.o libpng/pngrio.o libpng/pngrtran.o libpng/pngrutil.o libpng/pngset.o libpng/pngtrans.o libpng/pngwio.o libpng/pngwrite.o libpng/pngwtran.o libpng/pngwutil.o $(MATH) $(ZLIB)

EXECUTABLES=test

all: $(EXECUTABLES)

test: test.o readpng.o write_png_file.o
	gccx -o $@ $^ $(LIBPNG)

test.o: test.c readpng.h write_png_file.h
	gccx -c -o $@ $<
readpng.o: readpng.c
	gccx -c -o $@ $<
write_png_file.o: write_png_file.c
	gccx -c -o $@ $<

clean:
	rm $(EXECUTABLES) *.o
