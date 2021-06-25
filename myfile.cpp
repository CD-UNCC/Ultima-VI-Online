#include "stdafx.h"
#include "myfile.h"
#include "mytxt.h"

/*
typedef struct file
{
HFILE h;
} file;
*/

txt* file_error;
txt* file_error_name;

file* open(LPCSTR name)
{
static file* tf;
static OFSTRUCT fs;
tf=(file*)malloc(sizeof(file));
tf->h=HFILE_ERROR;
tf->h=OpenFile(name,&fs,OF_READWRITE|OF_SHARE_COMPAT);
if (tf->h==HFILE_ERROR){
file_error_name=txtnew(); txtset(file_error_name,name);
file_error=txtnew(); txtset(file_error,"File "); txtadd(file_error,file_error_name); txtadd(file_error," not found");
MessageBox(NULL,file_error->d,"Ultima 6 Online",MB_OK);
}
return tf;
}

file* open2(LPCSTR name, unsigned long flags)
{
static file* tf;
static OFSTRUCT fs;
tf=(file*)malloc(sizeof(file));
tf->h=HFILE_ERROR;
tf->h=OpenFile(name,&fs,flags);
return tf;
}

file* open2(txt *t, unsigned long flags)
{
static file* tf;
static OFSTRUCT fs;
tf=(file*)malloc(sizeof(file));
tf->h=HFILE_ERROR;
tf->h=OpenFile(t->d,&fs,flags);
return tf;
}

/*
typedef struct txt
{
char* d; //pointer to data
long l; //length of text in buffer
long bl; //length of current buffer
} txt;
*/

file* open(txt*t)
{
static file* tf;
static OFSTRUCT fs;
tf=(file*)malloc(sizeof(file));
tf->h=HFILE_ERROR;
tf->h=OpenFile(t->d,&fs,OF_READWRITE|OF_SHARE_COMPAT);
if (tf->h==HFILE_ERROR){
txtset(file_error_name,t);
file_error=txtnew(); txtset(file_error,"File "); txtadd(file_error,file_error_name); txtadd(file_error," not found");
MessageBox(NULL,file_error->d,"Ultima 6 Online",MB_OK);
}
return tf;
}

void get(file* filepointer,void* destoffset,long bytes)
{
if (filepointer->h!=HFILE_ERROR) _hread(filepointer->h,destoffset,bytes);
return;
}

void put(file* filepointer,void* sourceoffset,long bytes)
{
if (filepointer->h!=HFILE_ERROR) _hwrite(filepointer->h,(LPCSTR)sourceoffset,bytes);
return;
}

void seek(file* filepointer,long fileoffset)
{
if (filepointer->h!=HFILE_ERROR) _llseek(filepointer->h,fileoffset,FILE_BEGIN);
return;
}

void close(file* filepointer)
{
if (filepointer->h!=HFILE_ERROR) _lclose(filepointer->h);
free((void*)filepointer);
return;
}

long seek(file* filepointer)
{
if (filepointer->h==HFILE_ERROR) return 0;
return _llseek(filepointer->h,0,FILE_CURRENT);
}

long lof(file* filepointer)
{
if (filepointer->h==HFILE_ERROR) return 0;
static long i,i2;
i=_llseek(filepointer->h,0,FILE_CURRENT);
i2=_llseek(filepointer->h,0,FILE_END);
_llseek(filepointer->h,i,FILE_BEGIN);
return i2;
}

long loadfile_FILESIZE;
void* loadfile(LPCSTR name)
{
static file* f;
static void* v;
f=open(name);
if (f->h!=HFILE_ERROR) return NULL;
loadfile_FILESIZE=lof(f);
v=malloc(loadfile_FILESIZE);
get(f,v,loadfile_FILESIZE);
close(f);
return v;
}

void waitforfile(LPCSTR name){
static OFSTRUCT fs;
static HFILE hfile;
waitforfile_retry:
hfile=OpenFile(name,&fs,OF_SHARE_EXCLUSIVE|OF_READWRITE);
if (hfile==HFILE_ERROR) goto waitforfile_retry;
_lclose(hfile);
}

void deletefile(LPCSTR name){
static OFSTRUCT fs;
OpenFile(name,&fs,OF_DELETE);
return;
}