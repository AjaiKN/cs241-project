#include "libpng/png.h"
#include "zlib.h"


void readpng_version_info()
{
		fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
			PNG_LIBPNG_VER_STRING, png_libpng_ver);
		fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
			ZLIB_VERSION, zlib_version);
}

int main() {
	readpng_version_info();
}


