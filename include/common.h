#ifndef COMMON_ROUTINES_H
#define COMMON_ROUTINES_H

#define ROUND_UP(NUM, ROUND)   \
    (((NUM) + (ROUND) - 1) / (ROUND) * (ROUND))

#define ROUND_DOWN(NUM, ROUND)  \
    (((NUM) / (ROUND)) * (ROUND))

#define BITMASK(SHIFT, CNT) \
    (((1ul << (CNT)) - 1) << (SHIFT))

static inline int 
round_up_power_2 (int num)
{
    num--;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num++;

    return num;
};

#define PAGE_SIZE 0x1000

#define PHYS_TO_VIRT(ADDR)  \
    ((void*) (((void*)(ADDR)) + 0xC0000000))
#define VIRT_TO_PHYS(ADDR)  \
    ((void*) (((void*)(ADDR)) - 0xC0000000))

#endif // COMMON_ROUTINES_H

