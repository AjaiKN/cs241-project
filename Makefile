test: test.o libpng/pngerror.o libpng/pngget.o libpng/pngmem.o libpng/png.o libpng/pngpread.o libpng/pngread.o libpng/pngrio.o libpng/pngrtran.o libpng/pngrutil.o libpng/pngset.o libpng/pngtrans.o libpng/pngwio.o libpng/pngwrite.o libpng/pngwtran.o libpng/pngwutil.o -lm -lz
	gccx -o $@ $^ -lm -lz

test.o: test.c
	gccx -c -o $@ $^
