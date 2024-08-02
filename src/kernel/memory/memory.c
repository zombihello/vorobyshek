#include "memory/memory.h"

int memcmp( const void* pA, const void* pB, size_t size )
{
	const uint8_t* 		pAByte = ( const uint8_t* )pA;
	const uint8_t* 		pBByte = ( const uint8_t* )pB;
	for ( size_t index = 0; index < size; ++index )
	{
		if ( pAByte[index] < pBByte[index] )
		{
			return -1;
		}
		else if ( pAByte[index] > pBByte[index] )
		{
			return 1;
		}
	}

	return 0;
}

void* memcpy( void* pDest, const void* pSrc, size_t size )
{
	uint8_t* 		pDestByte = ( uint8_t* )pDest;
	const uint8_t* 	pSrcByte = ( const uint8_t* )pSrc;
	for ( size_t index = 0; index < size; ++index )
	{
		pDestByte[index] = pSrcByte[index];
	}

	return pDest;
}

void* memset( void* pBuffer, uint8_t value, size_t size )
{
	uint8_t* 	pBufferByte = ( uint8_t* )pBuffer;
	for ( size_t index = 0; index < size; ++index )
	{
		pBufferByte[index] = value;
	}

	return pBuffer;
}
