#include "lib/kernel/bitmap.h"
#include "lib/string.h"
#include "common.h"
#include "type.h"
#include "debug.h"
#include "lib/kernel/framebuf.h"

struct bitmap* 
bitmap_init (struct bitmap* bmap, void* buff, uint32 num)
{
    if (bmap == NULL || buff == NULL || num == 0)
        return NULL;

    bmap->bits = (uint32*) buff;
    memset (bmap->bits, 0, ROUND_UP (num, 32) / 8);
            
    bmap->length = ROUND_UP (num, 32) / 32;
    return bmap;
};

void
bitmap_set (struct bitmap* bmap, uint32 idx)
{
    if (bmap == NULL || idx >= 32 * bmap->length)
        return;

    bmap->bits[idx / 32] |= (0x1 << (idx % 32));
};

uint32 
bitmap_scan_and_set (struct bitmap* bmap, uint32 num)
{
    if (bmap == NULL || num == 0)
        return BITMAP_ERROR;

    uint32 idx = bitmap_scan (bmap, num);
    if (idx == BITMAP_ERROR)    return idx;

    uint32 i;
    uint32 end = idx + num;
    for (i = idx; i < end; i++)
        bitmap_set (bmap, i);

    return idx;
//
//    uint32 i;
//    for (i = 0; i < bmap->length; i++)
//    {
//        if (bmap->bits[i] != 0xFFFFFFFF)
//        {
//            sint32 j;
//            bool one = false;
//            uint32 count = 0;
//            sint32 idx = 0;
//            for (j = 31; j >= 0; j--)
//            {
//                if (!(bmap->bits[i] & (uint32) (0x1 << j)))
//                {
//                    if (!one)
//                    {
//                        idx = j;
//                        count = 0;
//                    }
//                    count++;
//                    one = true;
//                }
//                else
//                    one = false;
//
//                if (count == num)
//                    break;
//            }
//
//            ASSERT (count <= num);
//            ASSERT (idx < 32);
//            if (count == num)
//            {
//                /* We must flip NUM contiguous bits. */
//                sint32 k;
//                for (k = idx; k > (sint32) (idx - num); k--)
//                {
//                    bmap->bits[i] |= (uint32) (0x1 << k);
//                }
//
//                return i * 32 + (31 - idx);
//            }
//        }
//    }
//
//    return BITMAP_ERROR;
};

uint32
bitmap_scan (struct bitmap* bmap, size_t num)
{
    uint32 i;

    if (bmap == NULL || num == 0)   return BITMAP_ERROR;

    for (i = 0; i < bmap->length; i++)
    {
        if (bmap->bits[i] != 0xFFFFFFFF)
        {
            uint32 j;
            uint32 count = 0;
            for (j = 0; j < 31; j++)
            {
                if (bitmap_test (bmap, i * 32 + j))
                    count = 0;
                else
                    count++;
                if (count == num)
                    break;
            }

            if (count == num)
                /* J is the index */
                return i * 32 + j;
        }

    }

    return BITMAP_ERROR;
};

bool 
bitmap_test (struct bitmap* bmap, uint32 idx)
{
    if (bmap == NULL || ROUND_UP (idx / (8 * sizeof (uint32)), 
                sizeof (uint32)) >= bmap->length)
        return false;

    idx %= (8 * sizeof (uint32));
    return bmap->bits[idx / (8 * sizeof (uint32))] & (0x1 << (idx % 32));
};

void 
bitmap_clear (struct bitmap* bmap, uint32 idx)
{
    if (bmap == NULL || ROUND_UP (idx / (8 * sizeof (uint32)), 
                sizeof (uint32)) >= bmap->length)
        return;

    idx %= (8 * sizeof (uint32));
    bmap->bits[idx / (8 * sizeof (uint32))] &= 
        (0xFFFFFFFF ^ (0x1 << idx));
};

void 
bitmap_destroy (struct bitmap* bmap)
{
    if (bmap == NULL)
        return;

    bmap->bits = NULL;
    bmap->length = 0;
};

void
bitmap_dump (struct bitmap* bmap)
{
    if (bmap == NULL)
    {
        framebuf_write ("BITMAP: bitmap is NULL \n");
        return;
    }

    uint32 i;
    for (i = 0; i < bmap->length; i++)
    {
        sint32 j;
        for (j = 31; j >= 0; j--)
        {
            if (bmap->bits[i] & (uint32) (0x1 << j))
                framebuf_write ("1");
            else
                framebuf_write ("0");
        }
    }
    framebuf_write ("\n");
};
