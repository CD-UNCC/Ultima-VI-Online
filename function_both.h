#ifndef FUNCTION_BOTH_H
#define FUNCTION_BOTH_H
/* To be able to include this and the link link against the object file
 * only prototypes and externs go in here, and MAYBE some inline functions 
 * actual global variable definitions should be moved to u6o7.cpp itself. (#include "global_vars.h")
 */
#include <winsock2.h>
#include "data_both.h"
#include "math.h" 

/* external global variables section */
extern unsigned long BITSleftmask[33];//mask of index-many bits to keep 
extern unsigned char OBJGETDIR_FRAME;

/* function headers */
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

void function_both_init(void);
void BITSadd(txt *t,unsigned long *start,unsigned long value,unsigned long nbits);
unsigned long BITSget(txt *t,unsigned long *start,unsigned long nbits);
void LOGadd(txt* t);

DWORD WINAPI sockets_send(LPVOID i);
DWORD WINAPI sockets_receive(LPVOID i);
DWORD WINAPI sockets_disconnect(LPVOID i);

bool NET_send(void *s,unsigned long d,txt *t);
void getscreenoffset(long x,long y,long *mapx,long *mapy);

//getnbits returns the number of bits required to store n combinations
//if combinations is 1, getnbits returns 0 (only 1 combination requires 0 bits represent)
//if combinations is 2, getnbits returns 1
//unsigned char getnbits(unsigned long combinations);

// luteijn: basically this does (x?log2(x):0) BUT: if answer is not integer, we
// have to round up, not truncate or down.  can't just truncate and add 1,
// since that messes up exact answers and we lose combatibility with the real
// client...  since we already link the maths lib anyway, we might as well just
// put in a macro here, since this is used extensively when decoding the update
// messages.  
#define getnbits(x) ((unsigned char) ceil(x?((logf((float)x)/logf(2.0))):0))
// the way this is used, this makes sense. FIXME still ugly.  

unsigned char objgetdir(unsigned short type);
void txtaddcolouredname(txt *t,txt *name,player *p);


void SHA2(txt* t,txt* t2);


// c111
unsigned long new1_getexprequired(unsigned short level);
unsigned long new1_getexprequired(npc* npc);


#endif /* FUNCTION_BOTH_H */
