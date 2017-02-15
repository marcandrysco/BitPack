#include "pack.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>


/**
 * Main entry point for testing the packing code.
 *   @argc: The argument count.
 *   @argv: The argument array.
 *   &returns: Zero on success.
 */
int main(int argc, char **argv)
{
	void *ptr;
	uint8_t byte, off;
	unsigned int i, arr[128], val[128], width[128];

	srand(time(NULL));
	memset(arr, 0x00, sizeof(arr));

	for(i = 0; i < 128; i++) {
		width[i] = (unsigned int)rand() % 31 + 1;
		val[i] = (unsigned int)rand();
		
		if(width[i] != 32)
			val[i] %= (1 << width[i]);
	}

	ptr = arr;
	byte = off = 0;

	for(i = 0; i < 128; i++)
		pack_uint(&ptr, &byte, &off, val[i], width[i]);

	pack_flush(&ptr, &byte, &off);

	ptr = arr;
	byte = off = 0;

	for(i = 0; i < 128; i++)
		assert(unpack_uint(&ptr, &byte, &off, width[i]) == val[i]);

	fprintf(stderr, "Test OK\n");

	return 0;
}
