#ifndef PANIC_H
#define PANIC_H

// Raise critical error and hang the system
void 		panic( const char* pFormat, ... );

#endif // !PANIC_H
