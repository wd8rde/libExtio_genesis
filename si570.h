#ifndef _SI570_H_
#define _SI570_H_
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void si570_set_frequency( uint32_t frequency, uint8_t *regs );

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
