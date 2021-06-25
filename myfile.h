#ifndef _INC_MYFILE
#define _INC_MYFILE

#include <winsock2.h>
struct file
{
HFILE h;
};
#include "mytxt.h"
#include <io.h>

file* open(LPCSTR name);
file* open2(LPCSTR name, unsigned long flags);
void get(file* filepointer,void* destoffset,long bytes);
void seek(file* filepointer,long fileoffset);
void close(file* filepointer);
void put(file* filepointer,void* sourceoffset,long bytes);
long seek(file* filepointer);
long lof(file* filepointer);
file* open(txt*t);
file* open2(txt*t, unsigned long flags);
extern long loadfile_FILESIZE;
void* loadfile(LPCSTR name);
void waitforfile(LPCSTR name);
void deletefile(LPCSTR name);
#endif
