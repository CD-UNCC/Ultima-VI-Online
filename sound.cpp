#include "stdafx.h"
#include "resource.h"
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <math.h>

#include "myfile.h"
LPDIRECTSOUND dsnd;
extern HWND hWnd;
extern bool u6o_sound;
extern unsigned char u6ovolume;
bool DirectSoundCreate_fail=FALSE;

struct sound {
DSBUFFERDESC d;
WAVEFORMATEX wf;
LPDIRECTSOUNDBUFFER s;
unsigned char* o;
sound* ss;
};

sound* tempsound[256];

bool soundsetupf=FALSE;
bool soundsetup()
{
if (DS_OK!=DirectSoundCreate(NULL,&dsnd,NULL)){
DirectSoundCreate_fail=TRUE;
MessageBox(NULL,"WAV DirectSoundCreate failed","Ultima 6 Online",MB_OK);
return FALSE;
}
dsnd->SetCooperativeLevel(hWnd,DSSCL_NORMAL);
static long i;
for (i=0;i<256;i++)
{
tempsound[i]=NULL;
}
soundsetupf=TRUE;
return TRUE;
} 

sound* soundnew(long freq,long bit,long stereo,long bytes)
{
if (DirectSoundCreate_fail) return NULL;
if (soundsetupf==FALSE) soundsetup();
if (DirectSoundCreate_fail) return NULL;
sound* ts=(sound*)malloc(sizeof(sound));
ZeroMemory(ts,sizeof(sound));
ts->wf.cbSize=0;
ts->wf.wFormatTag=WAVE_FORMAT_PCM;
ts->wf.nChannels=stereo;
ts->wf.nSamplesPerSec=freq;
ts->wf.wBitsPerSample=bit;
ts->wf.nBlockAlign=ts->wf.nChannels*ts->wf.wBitsPerSample/8;
ts->wf.nAvgBytesPerSec=ts->wf.nSamplesPerSec*ts->wf.nBlockAlign;
ts->d.dwSize=sizeof(DSBUFFERDESC);
//software buffer is used to maintain offset of sound data
ts->d.dwFlags=DSBCAPS_GLOBALFOCUS|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN|DSBCAPS_CTRLFREQUENCY|DSBCAPS_GETCURRENTPOSITION2;
ts->d.dwBufferBytes=bytes;
ts->d.lpwfxFormat=&ts->wf;
dsnd->CreateSoundBuffer(&ts->d,&ts->s,NULL);
ts->d.dwBufferBytes=bytes;
static void* o1;
static unsigned long s1;
ts->s->Lock(0,0,&o1,&s1,NULL,NULL,DSBLOCK_ENTIREBUFFER);
ts->o=(unsigned char*)o1;
ts->s->Unlock(o1,s1,NULL,NULL);
return ts;
}

sound* soundload(LPCSTR fn)
{
if (DirectSoundCreate_fail) return NULL;
if (soundsetupf==FALSE) soundsetup();
if (DirectSoundCreate_fail) return NULL;
static sound* ts;
static file* fh;
static long freq,bits,stereo,x;
fh=open(fn);
if (fh->h==HFILE_ERROR)
{
ts=NULL;
return ts;
}
seek(fh,24);
get(fh,&freq,4);
seek(fh,34);
bits=0;
get(fh,&bits,2);
seek(fh,32);
stereo=0;
get(fh,&stereo,2);
if (stereo==(bits/8))
{stereo=1;}else{stereo=2;}
ts=soundnew(freq,bits,stereo,lof(fh)-58-16-16-32-32);
seek(fh,58);
get(fh,ts->o,lof(fh)-58-16-16-32-32);
close(fh);
return ts;
}

sound* soundplay(sound* s)
{
if (DirectSoundCreate_fail) return NULL;
//if (u6o_sound==FALSE) return NULL; //no sound
if (soundsetupf==FALSE) return NULL;
static unsigned long i,i2;
if (s==NULL) return NULL;
i=-1;
busysound:
i++;
if (i==256) return NULL;
if (tempsound[i]!=NULL)
{
tempsound[i]->s->GetStatus(&i2);
if (i2&DSBSTATUS_PLAYING) goto busysound; 
tempsound[i]->s->Release();
free((void*)tempsound[i]);
}
sound* ts=(sound*)malloc(sizeof(sound));
memcpy(ts,s,sizeof(sound));
dsnd->DuplicateSoundBuffer(s->s,&ts->s);
tempsound[i]=ts;
tempsound[i]->ss=s;
ts->s->Play(NULL,NULL,NULL);
return ts;
} 

sound* soundplay2(sound* s,long v)
{
if (DirectSoundCreate_fail) return NULL;
if (soundsetupf==FALSE) return NULL;
if (u6ovolume==0) return NULL;
if (v>255) v=255;
if (v<=0) return NULL;
static long i;
static unsigned long i2;
static float f;
if (s==NULL) return NULL;
i=-1;
busysound2:
i++;
if (i==256) return NULL;
if (tempsound[i]!=NULL)
{
tempsound[i]->s->GetStatus(&i2);
if (i2&DSBSTATUS_PLAYING) goto busysound2; 
tempsound[i]->s->Release();
free((void*)tempsound[i]);
}
sound* ts=(sound*)malloc(sizeof(sound));
memcpy(ts,s,sizeof(sound));
dsnd->DuplicateSoundBuffer(s->s,&ts->s);
tempsound[i]=ts;
tempsound[i]->ss=s;
f=v;
f=f*(float)u6ovolume/255.0f;
f=255-f;
f=f*0.25f;
f*=f;
ts->s->SetVolume(-f);
ts->s->Play(NULL,NULL,NULL);
return ts;
}

void free(sound* s){
if (DirectSoundCreate_fail) return;
if (soundsetupf==FALSE) return;
if (s!=NULL){
s->s->Release();
static long i;
for (i=0;i<256;i++)
{
if (tempsound[i]!=NULL)
{
if (tempsound[i]->ss==s)
{
tempsound[i]->s->Release();
free((void*)tempsound[i]);
tempsound[i]=NULL;
}
free((void*)s);
}
}
}
return;
}