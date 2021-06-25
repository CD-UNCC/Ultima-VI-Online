#ifndef _INC_SOUND
#define _INC_SOUND
#include <mmsystem.h>
#include <dsound.h>
extern LPDIRECTSOUND dsnd;
struct sound {
DSBUFFERDESC d;
WAVEFORMATEX wf;
LPDIRECTSOUNDBUFFER s;
unsigned char* o;
sound* ss;
};
sound* soundnew(long freq,long bit,long stereo,long bytes);
sound* soundload(LPCSTR fn);
sound* soundplay(sound* s);
sound* soundplay2(sound* s,long v);
bool soundsetup();
void free(sound* s);
#endif