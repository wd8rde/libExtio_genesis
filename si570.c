#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define FDCO_MIN		4850000000LL
#define FDCO_MAX		5670000000LL
#define HS_DIV_SHIFT		5
#define HS_DIV_MASK		0xe0
#define HS_DIV_OFFSET		4
#define N1_6_2_MASK		0x1f
#define N1_1_0_MASK		0xc0
#define RFREQ_37_32_MASK	0x3f
#define SI570_XTAL_FREQ 114285000

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static const uint8_t si570_hs_div_values[] = { 11, 9, 7, 6, 5, 4 };

/*
 * Set si570 frequency.
 * This function must be called with update mutex lock held.
 */
void si570_set_frequency( uint32_t frequency, uint8_t *regs )
{
	int i, n1, hs_div;
	uint64_t fdco, best_fdco = ULLONG_MAX;
    uint32_t rslt_n1 = 0;
    uint32_t rslt_hs_div = 0;
    uint64_t rslt_rfreq = 0;
    uint64_t rslt_frequency = 0;

	for (i = 0; i < ARRAY_SIZE(si570_hs_div_values); i++) {
		hs_div = si570_hs_div_values[i];
		/* Calculate lowest possible value for n1 */
		n1 = FDCO_MIN / (uint64_t)hs_div / (uint64_t)frequency;
		if (!n1 || (n1 & 1))
			n1++;
		while (n1 <= 128) {
			fdco = (uint64_t)frequency * (uint64_t)hs_div * (uint64_t)n1;
			if (fdco > FDCO_MAX)
				break;
			if (fdco >= FDCO_MIN && fdco < best_fdco) {
				rslt_n1 = n1;
				rslt_hs_div = hs_div;
				rslt_frequency = frequency;
				rslt_rfreq = (fdco << 28) / SI570_XTAL_FREQ;
				best_fdco = fdco;
			}
			n1 += (n1 == 1 ? 1 : 2);
		}
	}
	if (best_fdco == ULLONG_MAX)
		return;

	regs[0] = (((rslt_hs_div - HS_DIV_OFFSET) << HS_DIV_SHIFT) | (((rslt_n1 - 1) >> 2) & N1_6_2_MASK));
	regs[1] = (((rslt_n1 - 1) << 6) | ((rslt_rfreq >> 32) & RFREQ_37_32_MASK));
	regs[2] = ((rslt_rfreq >> 24) & 0xff);
	regs[3] = ((rslt_rfreq >> 16) & 0xff);
	regs[4] = ((rslt_rfreq >> 8) & 0xff);
	regs[5] = (rslt_rfreq & 0xff);
    return;
}

