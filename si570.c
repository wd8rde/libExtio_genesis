/* Copyright (c) 2010, DF9DQ
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions
 * and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 * and the following disclaimer in the documentation and/or other materials provided with the
 * distribution.
 * Neither the name DF9DQ nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



#include <inttypes.h>
#include <stdbool.h>
#include "si570.h"


#define SI570_I2C_ADDR              0xAA
#define SI570_DCO_HIGH              5670.0
#define SI570_DCO_LOW               4850.0
#define SI570_NOMINAL_XTALL_FREQ    114285000
#define SI570_XTALL_DEVIATION_PPM   2000
#define SI570_DEFAULT_STARTUP_FREQ  56.32
#define SI570_SMOOTH_TUNE_PPM 3000

    /* I2C bus address. Automatically detected by firmware. */
static uint8_t si570_address;
    /* Current *virtual* frequency (11.21). This is what the host sees! */
static uint32_t si570_current_frequency;
    /* Si570 anchor frequency. Real frequency of Si570. May differ from
     * the frequency of the host by the automatically selected prescaler.
     */
static uint32_t si570_anchor_frequency;
    /* Current prescaler setting (1, 4, 16, 64) */
static uint32_t si570_current_prescaler;
    /* Current content of real Si570 registers 7...12 */
static uint8_t si570_regs[6];
    /* Current content of virtual Si570 registers 7...12 */
static uint8_t si570_vregs[6];



/** Return the optimum values for HS_DIV and N1.
 *
 *  For power reasons it is desirable to have the lowest possible DCO
 *  frequency within its operating range of 4850...5670 MHz. For the same
 *  reason, HS_DIV should be as large as possible.
 *
 *  \param[in] freq Frequency (format 11.21)
 *  \param[out] hsdiv Pointer to HS_DIV value (4, 5, 6, 7, 9, 11)
 *  \param[out] n1 Pointer to N1 value (1...128)
 *  \param[in] check_limits Check limitations of real Si570 device
 *
 *  \return true if ok, false if freq not allowed
 */
static bool si570_find_hsdiv_n1 (uint32_t freq, uint32_t *hsdiv, uint32_t *n1, bool check_limits)
{
    const uint8_t hsdiv_val[6] = {4, 5, 6, 7, 9, 11};
    int i, n;


#define DCO_MIN _43_21(4850.0)
#define DCO_MAX _43_21(5670.0)

    /* Check limits of real device. Ignore for virtual device. */
    if (check_limits)
    {
        /* Handle the special frequency bands first.
        * 970 MHz <= freq <= 1134 MHz       --> HS_DIV = 5, N1 = 1
        * 1213 MHz <= freq <= 1417.5 MHz    --> HS_DIV = 4, N1 = 1
        */
        if( (_11_21(970.0) <= freq) && (freq <= _11_21(1134.0)) )
        {
            *hsdiv = 5;
            *n1 = 1;
            return true;
        }

        if( (_11_21(1213.0) <= freq) && (freq <= _11_21(1417.5)) )
        {
            *hsdiv = 5;
            *n1 = 1;
            return true;
        }

        /* Check valid frequency range */
        if ((freq < _11_21(10.0)) || (freq > SI570_MAX_FREQUENCY))
            return false;
    }

    /* Check with all possible N1 dividers: 1 or even number 2...128 */
    for( n = 1; n <= 128 ; n++ )
    {
        if( (n % 2) && (n > 1) )
            continue;

        /* Check with all six possible HS_DIV values. Start with the large values,
        * and stop at the first match.
        */
        for( i = 0; i <= 5; i++ )
        {
            if( ((n * hsdiv_val[i]) * (uint64_t)freq >= DCO_MIN) &&
                ((n * hsdiv_val[i]) * (uint64_t)freq <= DCO_MAX) )
            {
                *hsdiv = hsdiv_val[i];
                *n1 = n;
                return true;
            }
        }
    }

    /* We should never come here... */
    return false;
}



/** Set a new frequency in Si570.
 *
 *  Calculates the required register settings for the Si570.
 *  Automatically selects a suitable prescaler in the CPLD.
 *
 *  \param[in] freq Frequency (format 11.21)
 */
void si570_set_frequency (uint32_t frequency)
{
    uint32_t hs_div;
    uint32_t hs_div_reg;
    uint32_t n1;
    uint64_t rfreq;
    bool smooth;
    int32_t delta_f;
    uint8_t txbuf[7];
    uint32_t freq;
    uint32_t prescaler;


//frequency -= g_params.freqsubtract;


    freq = frequency * 4;

    /* Find optimum post dividers */
    if (!si570_find_hsdiv_n1( freq, &hs_div, &n1, true ))
        return;

    si570_current_frequency = frequency;

    /* RFREQ */
    rfreq = freq;
    rfreq <<= 23;
    rfreq *= hs_div * n1;
    rfreq /= (SI570_NOMINAL_XTALL_FREQ >> 8);  //TODO Auflösung!

    /* Prepare message for I2C bus */
    hs_div_reg = hs_div - 4;
    txbuf[0] = 7;
    txbuf[1] = ((hs_div_reg & 0x07) << 5) | (((n1-1) >> 2) & 0x1F); // Reg 7
    txbuf[2] = (((n1-1) & 0x03) << 6) | ((rfreq >> 32) & 0x3F);     // Reg 8
    txbuf[3] = ((rfreq >> 24) & 0xFF);                              // Reg 9
    txbuf[4] = ((rfreq >> 16) & 0xFF);                              // Reg 10
    txbuf[5] = ((rfreq >> 8) & 0xFF);                               // Reg 11
    txbuf[6] = (rfreq & 0xFF);                                      // Reg 12

    /* Check if we can use "smooth tuning" */
    delta_f = (int32_t)freq - (int32_t)si570_anchor_frequency;
    if( delta_f < 0 )
        delta_f = -delta_f;
    /* Note: 'ppm' requires to divide by 1000000. It's easier to divide
     *       by 1048576 (shift 20 bits) though. This reduces the range for
     *       smooth tuning by 5%.
     */
    smooth =
        delta_f <= ((SI570_SMOOTH_TUNE_PPM * (int64_t)si570_anchor_frequency) >> 20);

    /* Smooth tuning requires to update the RFREQ registers only.
     * A larger frequency step requires to freeze the oscillator first. In this
     * case the frequency becomes the new anchor.
     */
    if( smooth )
    {
    }
    else
    {
        si570_anchor_frequency = freq;
    }

    si570_regs[0] = ((hs_div_reg & 0x07) << 5) | (((n1-1) >> 2) & 0x1F);
    si570_regs[1] = (((n1-1) & 0x03) << 6) | ((rfreq >> 32) & 0x3F);
    si570_regs[2] = ((rfreq >> 24) & 0xFF);
    si570_regs[3] = ((rfreq >> 16) & 0xFF);
    si570_regs[4] = ((rfreq >> 8) & 0xFF);
    si570_regs[5] = (rfreq & 0xFF);
}


