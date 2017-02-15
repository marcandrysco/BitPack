Bit Packing API
===============

The bit packing API provides a simple way of packing integers into data
buffers. The integers are not aligned to byte or word boundaries. The packing
functions write the data to either memory or a file, and the unpacking
functions extract that data to integers.


## Bit Packing

Bits can be packed into either a memory using `ptr` or a file using `file`.
The `byte` and `off` values are used to track the status of the bit packing.
They must be initialized to zero prior to the first packing call.

    void pack_int(void **ptr, uint8_t *byte, uint8_t *off, int val, unsigned int width);
    void pack_uint(void **ptr, uint8_t *byte, uint8_t *off, unsigned int val, unsigned int width);

The `pack_int` and `pack_uint` pack an integer of size `width` into the memory
pointed to by `ptr`. The pointer `ptr` will be incremented as necessary.

    void pack_flush(void **ptr, uint8_t *byte, uint8_t *off);

The `pack_flush_ call` writes any pending bytes in `byte` to the memory of
`ptr`. After completion, the pointer will be aligned with no pending data in
`byte` and `off`.

    void fpack_int(FILE *file, uint8_t *byte, uint8_t *off, int val, unsigned int width);
    void fpack_uint(FILE *file, uint8_t *byte, uint8_t *off, unsigned int val, unsigned int width);
    void fpack_flush(FILE *file, uint8_t *byte, uint8_t *off);

The `fpack_int`, `fpack_uint`, and `fpack_flush` pack and write data directly
to a file referenced by `file`. The `fpack_flush` call is necessary to call
prior to closing the file to guarantee any partial data from `byte` is
written.


## Bit Unpacking

Arbitrary width numbers are unpacked into meomry using `ptr` or a file using
`file`. The `byte` and `off` values track partially unpacked bits, and they
must be initialized to zero prior to the first unpacking call.

    int unpack_int(void **ptr, uint8_t *byte, uint8_t *off, unsigned int width);
    unsigned int unpack_uint(void **ptr, uint8_t *byte, uint8_t *off, unsigned int width);

The `unpack_int` and `unpack_uint` functions read integers of size `width`
from the the memory pointed to by `ptr`. The pointer `ptr` will be incremented
as necessary.

    void unpack_align(void **ptr, uint8_t *byte, uint8_t *off);

The `unpack_align` function is used to realign the unpacking to a byte
boundry. Any pending data on `byte` will be dropped.

    int funpack_int(FILE *file, uint8_t *byte, uint8_t *off, unsigned int width);
    unsigned int funpack_uint(FILE *file, uint8_t *byte, uint8_t *off, unsigned int width);
    void funpack_align(FILE *file, uint8_t *byte, uint8_t *off);

The `funpack_int`, `funpack_uint`, and `funpack_flush` are file-based
equivalents for unpacking data.


## Sign Extending

    static inline unsigned int pack_uext(unsigned int val, int bits)
    static inline int pack_sext(int val, int bits)

The `pack_uext` function takes an unsigned integer `val` and zeros the upper
bits so that the returned value is `bits` wide.

The `pack_sext` function takes a signed integetr `val` and sign extends the
upper bits so that the returned value is `bits` wide.
