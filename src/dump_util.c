#include "dump_util.h"
#include <stdio.h>


void dump_buf(void *buf, size_t len)
{
    size_t i;
    size_t h = 0;

    for (i = 0; i < len; i++) {
        printf("%02x ", ((unsigned char *)buf)[i]);
        if (h == 15) {
            printf("\n");
        }
        else if (h == 7) {
            printf("- ");
        }
        h = (h+1) % 16;
    }
}
