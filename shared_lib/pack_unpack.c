/**
 * This is a Packing Unpacking Utility which is picked up from beej socket programming book
 * This library helps to pack data of various types to send them reliably over socket and then unpack at the other end
*/
#include <stdio.h>
#include <stdint.h> // defines uintN_t types
#include <inttypes.h> // defines PRIx macros
#include "pack_unpack.h"


uint32_t htonf(float f)
{
    uint32_t p;
    uint32_t sign;
    if (f < 0)
    {
        sign = 1;
        f = -f;
    }
    else
    {
        sign = 0;
    }
    p = ((((uint32_t)f) & 0x7fff) << 16) | (sign << 31); // whole part and sign
    p |= (uint32_t)(((f - (int)f) * 65536.0f)) & 0xffff; // fraction
    return p;
}

float ntohf(uint32_t p)
{
    float f = ((p >> 16) & 0x7fff); // whole part
    f += (p & 0xffff) / 65536.0f;   // fraction
    if (((p >> 31) & 0x1) == 0x1)
    {
        f = -f;
    } // sign bit set
    return f;
}

uint64_t pack754(long double f, unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit
    if (f == 0.0)
        return 0; // get this special case out of the way
    // check sign and begin normalization
    if (f < 0)
    {
        sign = 1;
        fnorm = -f;
    }
    else
    {
        sign = 0;
        fnorm = f;
    }
    // get the normalized form of f and track the exponent
    shift = 0;
    while (fnorm >= 2.0)
    {
        fnorm /= 2.0;
        shift++;
    }
    while (fnorm < 1.0)
    {
        fnorm *= 2.0;
        shift--;
    }
    fnorm = fnorm - 1.0;
    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL << significandbits) + 0.5f);
    // get the biased exponent
    exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias
    // return the final answer
    return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
}

long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit
    if (i == 0)
        return 0.0;
    // pull the significand
    result = (i & ((1LL << significandbits) - 1)); // mask
    result /= (1LL << significandbits);            // convert back to float
    result += 1.0f;                                // add the one back on
    // deal with the exponent
    bias = (1 << (expbits - 1)) - 1;
    shift = ((i >> significandbits) & ((1LL << expbits) - 1)) - bias;
    while (shift > 0)
    {
        result *= 2.0;
        shift--;
    }
    while (shift < 0)
    {
        result /= 2.0;
        shift++;
    }
    // sign it
    result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;
    return result;
}