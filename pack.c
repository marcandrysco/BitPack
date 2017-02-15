#include "pack.h"


/**
 * Pack a signed integer.
 *   @ptr: Ref. The write pointer.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @val: The value.
 *   @width: The width.
 */
void pack_int(void **ptr, uint8_t *byte, uint8_t *off, int val, unsigned int width)
{
	pack_uint(ptr, byte, off, val, width);
}

/**
 * Pack an unsigned integer.
 *   @ptr: Ref. The write pointer.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @val: The value.
 *   @width: The width.
 */
void pack_uint(void **ptr, uint8_t *byte, uint8_t *off, unsigned int val, unsigned int width)
{
	assert((8 * sizeof(int)) >= width);

	if((*off + width) < 8) {
		*byte |= (val << *off);
		*off += width;
	}
	else {
		if(*off > 0) {
			*(uint8_t *)(*ptr)++ = *byte | (val << *off);
			val >>= 8 - *off;
			width -= 8 - *off;
			*byte = *off = 0;
		}

		while(width >= 8) {
			*(uint8_t *)(*ptr)++ = val;
			val >>= 8;
			width -= 8;
		}

		*byte = val;
		*off = width;
	}
}

/**
 * Flush any pending data.
 *   @ptr: Ref. The write pointer.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 */
void pack_flush(void **ptr, uint8_t *byte, uint8_t *off)
{
	if(*off > 0) {
		*(uint8_t *)(*ptr) = *byte;
		*byte = *off = 0;
	}
}


/**
 * Pack a signed integer into a file.
 *   @file: The output file.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @val: The value.
 *   @width: The width.
 */
void fpack_int(FILE *file, uint8_t *byte, uint8_t *off, int val, unsigned int width)
{
	fpack_uint(file, byte, off, val, width);
}

/**
 * Pack an unsigned integer into a file.
 *   @file: The output file.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @val: The value.
 *   @width: The width.
 */
void fpack_uint(FILE *file, uint8_t *byte, uint8_t *off, unsigned int val, unsigned int width)
{
	assert((8 * sizeof(int)) >= width);

	if((*off + width) < 8) {
		*byte |= (val << *off);
		*off += width;
	}
	else {
		if(*off > 0) {
			fputc(*byte | (val << *off), file);
			val >>= 8 - *off;
			width -= 8 - *off;
			*byte = *off = 0;
		}

		while(width >= 8) {
			fputc(val, file);
			val >>= 8;
			width -= 8;
		}

		*byte = val;
		*off = width;
	}
}

/**
 * Flush any pending data to a file.
 *   @file: The output file.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 */
void fpack_flush(FILE *file, uint8_t *byte, uint8_t *off)
{
	if(*off > 0) {
		fputc(*byte, file);
		*byte = *off = 0;
	}
}


/**
 * Unpack a signed integer.
 *   @ptr: Ref. The read pointer.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @width: The width.
 *   &returns: The value.
 */
int unpack_int(void **ptr, uint8_t *byte, uint8_t *off, unsigned int width)
{
	return pack_sext(unpack_uint(ptr, byte, off, width), width);
}

/**
 * Unpack an unsigned integer.
 *   @ptr: Ref. The read pointer.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @width: The width.
 *   &returns: The value.
 */
unsigned int unpack_uint(void **ptr, uint8_t *byte, uint8_t *off, unsigned int width)
{
	unsigned int val, bit = 0;

	assert((8 * sizeof(int)) >= width);

	if(width > *off) {
		if(*off > 0) {
			val = *byte;
			width -= *off;
			bit = *off;
			*byte = *off = 0;
		}
		else
			val = 0;

		while(width >= 8) {
			val |= *(uint8_t *)(*ptr)++ << bit;
			bit += 8;
			width -= 8;
		}

		if(width > 0) {
			*byte = *(uint8_t *)(*ptr)++;
			val |= pack_uext(*byte, width) << bit;
			*byte >>= width;
			*off = 8 - width;
		}
	}
	else {
		val = pack_uext(*byte, width);
		*byte >>= width;
		*off -= width;
	}

	return val;
}

/**
 * Align the unpacking to the nearest byte.
 *   @ptr: Ref. The read pointer.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 */
void unpack_align(void **ptr, uint8_t *byte, uint8_t *off)
{
	*byte = 0;
	*off = 0;
}


/**
 * Unpack a signed integer from a file.
 *   @file: The input file.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @width: The width.
 *   &returns: The value.
 */
int funpack_int(FILE *file, uint8_t *byte, uint8_t *off, unsigned int width)
{
	return pack_sext(funpack_uint(file, byte, off, width), width);
}

/**
 * Unpack an unsigned integer from a file.
 *   @file: The input file.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 *   @width: The width.
 *   &returns: The value.
 */
unsigned int funpack_uint(FILE *file, uint8_t *byte, uint8_t *off, unsigned int width)
{
	unsigned int val, bit = 0;

	assert((8 * sizeof(int)) >= width);

	if(width > *off) {
		if(*off > 0) {
			val = *byte;
			width -= *off;
			bit = *off;
			*byte = *off = 0;
		}
		else
			val = 0;

		while(width >= 8) {
			val |= fgetc(file) << bit;
			bit += 8;
			width -= 8;
		}

		if(width > 0) {
			*byte = fgetc(file);
			val |= pack_uext(*byte, width) << bit;
			*byte >>= width;
			*off = 8 - width;
		}
	}
	else {
		val = pack_uext(*byte, width);
		*byte >>= width;
		*off -= width;
	}

	return val;
}

/**
 * Align the unpacking to the nearest byte.
 *   @file: The file.
 *   @byte: Ref. The byte.
 *   @off: Ref. The offset.
 */
void funpack_align(FILE *file, uint8_t *byte, uint8_t *off)
{
	*byte = 0;
	*off = 0;
}
