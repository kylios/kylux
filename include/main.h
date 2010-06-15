#ifndef MAIN_H
#define MAIN_H

#include "type.h"
#include "common.h"

/* Default kernel options */
#define KOPTION_DEFAULTS	\
{							\
    2048,                   \
	-1						\
}

struct koptions
{
	uint32 kernel_frames;
	uint32 user_frames;

};

#endif // MAIN_H

