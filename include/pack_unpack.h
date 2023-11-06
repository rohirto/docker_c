/**
 * This is a Packing Unpacking Utility which is picked up from beej socket programming book
 * This library helps to pack data of various types to send them reliably over socket and then unpack at the other end
*/
#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

void unpack(unsigned char *buf, char *format, ...);
unsigned int pack(unsigned char *buf, char *format, ...);

#endif