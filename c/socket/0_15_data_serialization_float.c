/**
 * Data Serialization - pack the data into known binary format
 * 
 * But why?? Ensures that what sender wanted to receiver receives that only, iirespective of machine architecture
 * 
 * Below code a method to send float, encode float into known binary format
*/

#include <stdio.h>
#include <stdint.h> // defines uintN_t types
#include <inttypes.h> // defines PRIx macros

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

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

int main(void)
{
    float f = 3.1415926, f2;
    uint32_t netf;
    double d = 3.14159265358979323, d2;
    uint32_t fi;
    uint64_t di;

    netf = htonf(f);                    // convert to "network" form
    f2 = ntohf(netf);                   // convert back to test

    printf("****** METHOD 1*********************\n");
    printf("Original: %f\n", f);        // 3.141593
    printf(" Network: 0x%08X\n", netf); // 0x0003243F
    printf("Unpacked: %f\n", f2);       // 3.141586
    printf("*************************************\n");

    printf("\n\n\n");

    fi = pack754_32(f);
    f2 = unpack754_32(fi);

    di = pack754_64(d);
    d2 = unpack754_64(di);
    printf("********* METHOD 2************************\n");
    printf("float before : %.7f\n", f);
    printf("float encoded: 0x%08" PRIx32 "\n", fi);
    printf("float after : %.7f\n\n", f2);
    printf("double before : %.20lf\n", d);
    printf("double encoded: 0x%016" PRIx64 "\n", di);
    printf("double after : %.20lf\n", d2);
    printf("*************************************\n");
    return 0;
}