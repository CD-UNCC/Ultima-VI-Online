#ifndef _INC_MYTXT
#define _INC_MYTXT

struct txt
{
union{ 
char *d; //pointer to data
unsigned char *d2; //pointer to data (unsigned)
unsigned long *dl; //pointer to data (unsigned long)
unsigned short *ds; //pointer to data (unsigned short)
float *df; //pointer to data (float)
double *dd; //pointer to double prec. float data
};
long l; //length of text in buffer
long bl; //length of current buffer
};

#include "myfile.h"

//basic operations
txt* txtnew();
void txtNEWLEN(txt*t,long l);
void txtset(txt*t,const char*d);
void txtset(txt*t,txt*t2);
void free(txt* t);
void txtadd(txt*t,txt*t2);
void txtadd(txt*t,const char*d);

//editing
void txtlcase(txt*t);
void txtucase(txt*t);
void txtleft(txt*t,long n);
void txtright(txt*t,long length);
void txtmid(txt*t,long position,long length);

//searching
long txtsearch(txt*t,txt*t2);
bool txtsame(txt*t,txt*t2);
bool txtsamewithoutcase(txt*t,txt*t2);

//string< >number
void txtnum(txt*t,float number);
float txtnum(txt*t);
extern long txtnum_LENGTH;
void txtnumhex(txt*t,DWORD number);
void txtnumbin(txt*t,DWORD number);
void txtnumint(txt*t,long number);

//file associated
void txtgetfilename(txt*t);
void txtfilein(txt* t,file* f);
void txtfileout(txt* t,file* f);

//add variable to string
void txtaddchar(txt* t,unsigned char d);
void txtaddshort(txt* t,unsigned short d);
void txtaddlong(txt* t,unsigned long d);
void txtaddfloat(txt* t,float d);
void txtadddouble(txt* t,double d);
//set variable as string
void txtsetchar(txt* t,unsigned char d);
void txtsetshort(txt* t,unsigned short d);
void txtsetlong(txt* t,unsigned long d);
void txtsetfloat(txt* t,float d);
void txtsetdouble(txt* t,double d);

#endif
