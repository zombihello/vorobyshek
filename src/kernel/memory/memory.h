#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

// Compare memory
int 		memcmp( const void* pA, const void* pB, size_t size );

// Copy memory
void* 		memcpy( void* pDest, const void* pSrc, size_t size );

// Set memory
void* 		memset( void* pBuffer, uint8_t value, size_t size );

#endif // !MEMORY_H
