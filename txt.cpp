#include "stdafx.h"
#include "resource.h"
#include "commdlg.h"
#include <stdio.h>

extern HWND hWnd;

#include "myfile.h"

txt* txtnew() //*externally available
{
  txt* t;
  t=(txt*)malloc(sizeof(txt));
  t->d=(char*)malloc(1);
  t->d[0]=0;
  t->l=0;
  t->bl=1;
  return t;
}

void txtNEWLEN(txt*t,long l) //*externally available
{
  bool keepmem;
  keepmem=TRUE;
  if (l<=0) {l=-l; keepmem=FALSE;}

  if (t->l==l){
    t->d[l]=0; //for data security reasons (perhaps using txtleft or txtright was changing the string?)
    return;
  }

  if (t->bl>l) { //if buffer is big enough, just extend string
    t->l=l;
    t->d[l]=0;
    return;
  }

  if (l<t->l) { //if new string needs to be shorter perform a quick truncate
    t->l=l;
    t->d[l]=0;
    return;
  }

  char* cp; //create a new buffer for larger string
  long i;
  i=l;
  if (i>131072) i=131072;
  i=l+i+1; //apply the doubles rule
  cp=(char*)malloc(i);//GPF???
  if (keepmem==TRUE) memcpy(cp,t->d,t->l);
  free(t->d);
  t->l=l;
  t->bl=i;
  t->d=cp;
  t->d[l]=0;
  return;
}

void txtset(txt*t,const char*d) { //set txt to a directly typed string
  static long l;
  l=strlen(d);
  txtNEWLEN(t,-l);
  memcpy(t->d,d,l);
  return;
}
void txtset(txt*t,txt*t2) { // txt1=txt2
  txtNEWLEN(t,-t2->l);
  memcpy(t->d,t2->d,t2->l);
  return;
}
void txtucase(txt*t) { //make txt uppercase 
  static long i;
  static char v;
  for (i=0;i<t->l;i++) {
    v=t->d[i];
    if (v>=97&&v<=122) {
      v-=32;
      t->d[i]=v;
    }
  }
  return;
}
void txtlcase(txt*t) { //make txt lower case
  static long i;
  static char v;
  for (i=0;i<t->l;i++) {
    v=t->d[i];
    if (v>=65&&v<=90) {
      v+=32;
      t->d[i]=v;
    }
  }
  return;
}
void txtadd(txt*t,txt*t2) { //add second string to first string
  static long l;
  l=t->l;
  txtNEWLEN(t,l+t2->l);
  memcpy(&t->d[l],t2->d,t2->l);
  return;
}

void txtadd(txt*t,const char*d) { //add fixed string to first string
  static long l,l2;
  l=strlen(d);
  l2=t->l;
  txtNEWLEN(t,l+t->l);
  memcpy(&t->d[l2],d,l);
  return;
}

void txtleft(txt*t,long n) { //keep ? left chars of txt
  if (n<0) n=0;
  if (n>t->l) n=t->l;
  txtNEWLEN(t,n);
}

bool txtsame(txt*t,txt*t2) //comparison of two txt items
{
  static DWORD tempoffset;
  static DWORD tempoffset2;
  static DWORD tempoffset3;

  if (t->l!=t2->l) return FALSE;
  tempoffset=(DWORD)t->d;
  tempoffset2=(DWORD)t2->d;
  tempoffset3=(DWORD)t->l;
  _asm
  {
    mov ebx,OFFSET tempoffset
      mov esi,[ebx]
    mov ebx,OFFSET tempoffset2
      mov edi,[ebx]
    mov ebx,OFFSET tempoffset3
      mov ecx,[ebx]
    mov dword ptr [ebx],0
cmploop:
    mov al,[esi]
    cmp al,byte ptr [edi]
    jne noequal
      inc edi
      inc esi
      dec ecx
      jnz cmploop
      mov dword ptr [ebx],1
noequal:
  }
  if (tempoffset3==0) return FALSE;
  return TRUE;
}

void txtmid(txt*t,long position,long length) {
  if (position<1)
  {
    length+=(position-1);
    position=1;
  }
  if (position+length-1>t->l)
  {
    length=t->l-position+1;
  }
  if (length<1)
  {
    txtNEWLEN(t,0);
    return;
  }
  memmove(&t->d[0],&t->d[position-1],length);
  txtNEWLEN(t,length);
  return;
}

void txtright(txt*t,long length) {
  if (length<=0)
  {
    t->l=0;
    t->d[0]=0;
    return;
  }
  if (length>t->l) return;
  memmove(t->d,&t->d[t->l-length],length);
  t->l=length;
  return;
}


unsigned char currentdirectry[1024];
void txtgetfilename(txt*t)
{
  GetCurrentDirectory(1024,(LPSTR)&currentdirectry);
  ShowCursor(TRUE);
  static char filename[10000];
  memset(&filename[0],0,10000);
  filename[0]=0;
  static OPENFILENAME o;
  static long i;
  ShowWindow(hWnd,SW_HIDE);
  ZeroMemory(&o,sizeof(OPENFILENAME));
  o.lStructSize=sizeof(OPENFILENAME);
  o.lpstrFile=&filename[0];
  o.nMaxFile=10000;
  o.Flags=OFN_ALLOWMULTISELECT|OFN_CREATEPROMPT|OFN_EXPLORER;
  GetOpenFileName(&o);
  ShowWindow(hWnd,SW_SHOW);
  i=9999;
nextbyte:
  if (i==-1) goto nobyte;
  if (filename[i]==0)
  {
    i--;
    goto nextbyte;
  }
nobyte:
  i++;
  txtNEWLEN(t,i);
  memcpy(&t->d[0],&filename[0],i);
  ShowCursor(FALSE);
  SetCurrentDirectory((LPSTR)&currentdirectry);
  return;
}

//number string functions
//can not convert .1 or -.1, needs number before decimal point
// luteijn: replaced this with a standard library call.
/*float txtnum(txt*t) { //assumes a valid string is input
return (float) strtod(t->d,NULL);
}*/

long txtnum_LENGTH;
float txtnum(txt*t) { //assumes a valid string is input
  static float n,dp; static long i,ms,i2;
  dp=0; n=0; ms=0; i=0; i2=0;
  for (i=0;i<t->l;i++) {
    if ((t->d[i]>=48)&&(t->d[i]<=57)){
      if (i2==0&&i!=0){
        if (t->d[i-1]==45) ms=1; //minus sign?
      }
      i2=1; 
      if (dp==0)
      {
        n*=10;
        n=n+t->d[i]-48;
      }
      else
      {
        n=n+(t->d[i]-48)*dp;
        dp*=0.1;
      }
    }
    else
    {
      if (t->d[i]==46)
      {  
        dp=0.1;  
      }
      else
      {
        if (i2==1){
          txtnum_LENGTH=i;
          if (ms==1) n=-n;
          if (i2==0) {txtnum_LENGTH=0; return 0;} 
          return n;
        }
      } 
    }
  } //next i
  txtnum_LENGTH=i;
  if (ms==1) n=-n;
  if (i2==0) {txtnum_LENGTH=0; return 0;} 
  return n;
}


void txtnum(txt*t,float number) {
  static char temptxt[100];
  txtNEWLEN(t,_snprintf(&temptxt[0],100,"%f",number));
  memcpy(t->d,&temptxt[0],t->l);
  return;
}
void txtnumint(txt*t,long number) {
  static char temptxt[100];
  txtNEWLEN(t,_snprintf(&temptxt[0],100,"%ld",number));
  memcpy(t->d,&temptxt[0],t->l);
  return;
}
void txtnumhex(txt*t,DWORD number) {
  static char temptxt[100];
  txtNEWLEN(t,_snprintf(&temptxt[0],100,"%lX",number));
  memcpy(t->d,&temptxt[0],t->l);
  return;
}
void txtnumbin(txt*t,DWORD number) {
  static char temptxt[32];
  static DWORD bittest;
  static long i,i2;
  bittest=0x80000000;
  i=0;
  for (i2=0;i2<32;i2++)
  {
    if (bittest&number)
    {
      temptxt[i]='1';
      i++;
    }
    else
    {
      if (i>0||i2==31)
      {
        temptxt[i]='0';
        i++;
      }
    }
    bittest>>=1;
  }
  txtNEWLEN(t,i);
  memcpy(t->d,&temptxt[0],i);
  return;
}

long txtsearch(txt*t,txt*t2) //search for text2 in text
{
  if (t2->l>t->l) return 0;
  if (t2->l==0) return 0;
  if (t->l==0) return 0;
  static long i,i2;
  for (i=0;i<=t->l-t2->l;i++)
  {
    for(i2=0;i2<t2->l;i2++)
    {
      if (t->d[i+i2]!=t2->d[i2]) goto nothere;
    }
    return i+1;
nothere:;
  }
  return 0;
}

void txtfilein(txt* t,file* f)
{
  static unsigned char b[1024];
  static long i,i2,i3,l;
  i=seek(f);
  l=lof(f);
  if (i>=l)
  {
    txtNEWLEN(t,0);
    return;
  }
  if (i+1024>l)
  {l=l-i;} else {l=1024;}
  get(f,&b[0],l);
  i2=-1;
nextbyte:
  i2++;
  i++;
  if (i2==l) goto endoffile;
  if (b[i2]!=13&&b[i2]!=10) goto nextbyte;
  if (b[i2+1]==13) i++; //skip complementary data
  if (b[i2+1]==10) i++;
endoffile:
  txtNEWLEN(t,i2);
  memcpy(t->d,&b[0],i2);
  seek(f,i);
}

void txtfileout(txt* t,file* f)
{
  static unsigned char b[2];
  put(f,t->d,t->l);
  b[0]=13;
  b[1]=10;
  put(f,b,2);
}

void free(txt* t) //*externally available
{
  free(t->d);
  free((void*)t);
  return;
}


//ADD VARIABLE TO STRING
//avoids calling memcpy and txtNEWLEN when possible
//avoids confusing double/triple handling (eg. txtset(t,"?"); t->ds[0]=1;}
void txtaddchar(txt* t,unsigned char d){
  if (t->bl<=(t->l+1)){//extend buffer
    txtNEWLEN(t,t->l+1);
    t->d2[t->l-1]=d;
    return;
  }else{
    t->d2[t->l++]=d;
    t->d2[t->l]=0;
  }
}
static unsigned short *txtaddshort_p;
void txtaddshort(txt* t,unsigned short d){
  if (t->bl<=(t->l+2)){//extend buffer
    txtNEWLEN(t,t->l+2);
    txtaddshort_p=(unsigned short*)(t->d2+t->l-2);
  }else{
    txtaddshort_p=(unsigned short*)(t->d2+t->l);
    t->l+=2; t->d2[t->l]=0;
  }
  *txtaddshort_p=d;
}
static unsigned long *txtaddlong_p;
void txtaddlong(txt* t,unsigned long d){
  if (t->bl<=(t->l+4)){//extend buffer
    txtNEWLEN(t,t->l+4);
    txtaddlong_p=(unsigned long*)(t->d2+t->l-4);
  }else{
    txtaddlong_p=(unsigned long*)(t->d2+t->l);
    t->l+=4; t->d2[t->l]=0;
  }
  *txtaddlong_p=d;
}
static float *txtaddfloat_p;
void txtaddfloat(txt* t,float d){
  if (t->bl<=(t->l+4)){//extend buffer
    txtNEWLEN(t,t->l+4);
    txtaddfloat_p=(float*)(t->d2+t->l-4);
  }else{
    txtaddfloat_p=(float*)(t->d2+t->l);
    t->l+=4; t->d2[t->l]=0;
  }
  *txtaddfloat_p=d;
}
static double *txtadddouble_p;
void txtadddouble(txt* t,double d){
  if (t->bl<=(t->l+8)){//extend buffer
    txtNEWLEN(t,t->l+8);
    txtadddouble_p=(double*)(t->d2+t->l-8);
  }else{
    txtadddouble_p=(double*)(t->d2+t->l);
    t->l+=8; t->d2[t->l]=0;
  }
  *txtadddouble_p=d;
}
//SET VARIABLE TO STRING
void txtsetchar(txt* t,unsigned char d){
  if (t->bl<=1){//extend buffer
    txtNEWLEN(t,-1);
  }
  t->d2[0]=d;
  t->d2[1]=0;
  t->l=1;
}
void txtsetshort(txt* t,unsigned short d){
  if (t->bl<=2){//extend buffer
    txtNEWLEN(t,-2);
  }
  t->ds[0]=d;
  t->d2[2]=0;
  t->l=2;
}
void txtsetlong(txt* t,unsigned long d){
  if (t->bl<=4){//extend buffer
    txtNEWLEN(t,-4);
  }
  t->dl[0]=d;
  t->d2[4]=0;
  t->l=4;
}
void txtsetfloat(txt* t,float d){
  if (t->bl<=4){//extend buffer
    txtNEWLEN(t,-4);
  }
  t->df[0]=d;
  t->d2[4]=0;
  t->l=4;
}
void txtsetfloat(txt* t,double d){
  if (t->bl<=4){//extend buffer
    txtNEWLEN(t,-4);
  }
  t->dd[0]=d;
  t->d2[4]=0;
  t->l=4;
}

bool txtsamewithoutcase(txt*t,txt*t2){
  static txt *t_ucase=txtnew();
  static txt *t2_ucase=txtnew();
  txtset(t_ucase,t); txtucase(t_ucase);
  txtset(t2_ucase,t2); txtucase(t2_ucase);
  return txtsame(t_ucase,t2_ucase);
}
