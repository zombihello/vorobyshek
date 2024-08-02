#include "utils/string.h"

size_t strlen( const char* pStr )
{
	if ( !pStr || *pStr == '\0' )
	{
		return 0;
	}

	size_t 		length = 0;
	while ( *pStr )
	{
		++length;
		++pStr;
	}
	return length;
}
