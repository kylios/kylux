#ifndef BITMAP_H
#define BITMAP_H

#include "type.h"

#define BITMAP_ERROR    -1
struct bitmap
{
    uint32* bits;
    uint32 length;
};


/**
 * Initializes a new bitmap data structure that supports NUM indices.
 * Returns a NULL pointer if the kernel failed to allocate memory for 
 * the bitmap. 
 * */
struct bitmap* bitmap_init (struct bitmap* bmap, void* buf, uint32 num);

/**
 * Sets the bit at IDX to true.
 * */
void bitmap_set (struct bitmap*, uint32 idx);

/**
 * Sets NUM contiguous bits in the bitmap and returns the index of the 
 * first bit. 
 * */
uint32 bitmap_scan_and_set (struct bitmap*, uint32 num);

/**
 * Scans for the first NUM contiguous bits that are clear and returns
 * the index of the first one.
 * */
uint32 bitmap_scan (struct bitmap*, size_t num);

/**
 * Tests the bit at index IDX and returns its value.
 * */
bool bitmap_test (struct bitmap*, uint32 idx);

/**
 * Sets the bit at index IDX to zero. 
 * */
void bitmap_clear (struct bitmap*, uint32 idx);

/**
 * Deallocates and destroys the specified bitmap. 
 * */
void bitmap_destroy (struct bitmap*);

/* 
 * Dump the contents of this bitmap.  Prints a string of 1's and 0's
 * to represend the bitmap's values.
 */
void bitmap_dump (struct bitmap* bmap);


#endif // BITMAP_H
