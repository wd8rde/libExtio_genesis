
#ifndef _SI570_H_
#define _SI570_H_

#include <inttypes.h>

#define  _8_24(double_freq) (uint32_t)(double_freq*(1<<24))
#define _11_21(double_freq) (uint32_t)(double_freq*(1<<21))
#define _43_21(double_freq) (uint64_t)(double_freq*(1<<21))


    /* Maximum frequency (depends on speed grade and output option) */
#define SI570_MAX_FREQUENCY     _11_21(160.0)

void si570_set_frequency( uint32_t frequency );

#endif
