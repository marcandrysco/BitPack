#ifndef PACK_H
#define PACK_H

/*
 * dependent headers
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

/*
 * packing declarations
 */
void pack_int(void **ptr, uint8_t *byte, uint8_t *off, int val, unsigned int width);
void pack_uint(void **ptr, uint8_t *byte, uint8_t *off, unsigned int val, unsigned int width);
void pack_flush(void **ptr, uint8_t *byte, uint8_t *off);

void fpack_int(FILE *file, uint8_t *byte, uint8_t *off, int val, unsigned int width);
void fpack_uint(FILE *file, uint8_t *byte, uint8_t *off, unsigned int val, unsigned int width);
void fpack_flush(FILE *file, uint8_t *byte, uint8_t *off);

int unpack_int(void **ptr, uint8_t *byte, uint8_t *off, unsigned int width);
unsigned int unpack_uint(void **ptr, uint8_t *byte, uint8_t *off, unsigned int width);
void unpack_align(void **ptr, uint8_t *byte, uint8_t *off);

int funpack_int(FILE *file, uint8_t *byte, uint8_t *off, unsigned int width);
unsigned int funpack_uint(FILE *file, uint8_t *byte, uint8_t *off, unsigned int width);
void funpack_align(FILE *file, uint8_t *byte, uint8_t *off);


/**
 * Unsigned extend a value.
 *   @val: The value to be extended.
 *   @bits: The number of bits to extend.
 */
static inline unsigned int pack_uext(unsigned int val, int bits)
{
	assert((bits > 0) && (bits < (8 * sizeof(unsigned int))));

	return (val << ((8 * sizeof(unsigned int)) - bits)) >> ((8 * sizeof(unsigned int)) - bits);
}

/**
 * Sign extend a value.
 *   @val: The value to be extended.
 *   @bits: The number of bits to extend.
 */
static inline int pack_sext(int val, int bits)
{
	assert((bits > 0) && (bits < (8 * sizeof(int))));

	return (val << ((8 * sizeof(int)) - bits)) >> ((8 * sizeof(int)) - bits);
}

#endif
