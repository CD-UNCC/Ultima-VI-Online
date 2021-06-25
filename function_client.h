#ifndef FUNCTION_CLIENT_H
#define FUNCTION_CLIENT_H
/* To be able to include this and then link against the object file
 * only prototypes and externs go in here, and MAYBE some inline functions 
 * actual global variable definitions should be moved to u6o7.cpp itself. (#include "global_vars.h")
 */
#include "data_client.h"
#include "function_both.h"

/* external global variables */
	//GetInput variables 
	//tab_pressed allows program to trap the tab key
	//once trapped it also counts as an enterpressed, so serves a dual purpose
	//otherwise tab key inserts an undefined amount of spaces
	//it MUST be set after a call to getinput_setup
extern unsigned char GETINPUT_tab_pressed;
extern txt *GETINPUT_txt;
extern unsigned char *GETINPUT_enterpressed;
extern txt *GETINPUT_old; //used to detect new pointers
extern unsigned long GETINPUT_maxlength; //maximum length of GETINPUT_txt (0=infinite)
extern long gs_i; //getspr static data
extern long gs_i2; //getspr static data
extern long gs_x; //getspr static data
extern long gs_y; //getspr static data
extern long gs_t; //getspr static data
extern unsigned long GSs;
extern unsigned long GSx;
extern unsigned long GSy;
extern unsigned char midikeyboard2[256]; //reverse of midikeybaord array!
extern unsigned char midikeyboard2_keyon[256]; //whether key is being held or not
extern short midikeyboard_set;
extern unsigned char musickeyboard_set;
extern unsigned char midikeystack[16][256];
extern float midikeywait[16][256];
extern unsigned char clientinstrument;
extern unsigned char playinstrument;
extern unsigned char midipause;
extern unsigned char getsound_MOVERSOUND;
extern unsigned char AMBIENTLIGHT_LIGHTVALUE;
extern unsigned char AMBIENTLIGHT_SHOWSUN;
extern txt *STATUSMESSprev[8];//the previous 8 status messages are stored here
extern txt *STATUSMESSdisplaying;//the message currently being displayed
extern float STATUSMESSwait;
extern unsigned char STATUSMESSskipok;//the message will be skipped if any messages are pending
extern txt *STATUSMESSt;//temp txt for building messages (included to aid conversion from older system)
extern txt *STATUSMESSpending;
extern txt *GETSETTING_RAW;//the actualt text between the square brackets [...]
extern txt *li2_t;


//void updatepartyframe1(FRAME* f, int partyindex, double scale);

// r999 new
void inituidatan1();
void applyscaleuipanelwidget(int uipi, int uiwi, int uisi, float scalex, float scaley);
int gethituipaneli(int x, int y);
int gethituipanelwidgeti(int x, int y, int uipaneli);
int gethituipanelwidgeti(int x, int y);
int testhituipanel(int x, int y, int uipaneli);



// s333 get type of object at coordinate
void backupplayermvinfon1(player* tplayer);
//int isobjonscreenn1(int mapxn, int mapyn, int tpxn, int tpyn);
int getobjtypen1(player* tplayer, int mapx, int mapy);
//int getobjtypen1b(player p, int mapx, int mapy);
int getobjtypen1b(player p, int mapx, int mapy, int exactcoord);
int checkdeadobjtypen1(int typeparam);
int convertdeadobjtypen1(int typeparam);
int checkobjtypecreaturen1(int typeparam);

// s333 get description of object type (for creature)
void getobjdescn1(txt* txtdesc, int typeparam);

// s555
void updateoptioninfo();




/* function prototypes */
void function_client_init(void); 


//for compatibility the client supports this function
object *OBJnew_local();

void GETINPUT_setup(txt* input_pointer,void* enterpressed_pointer,unsigned long maxlength);
txt* GETINPUT_current();
void GETINPUT_stop();

/* luteijn: FIXME Oh JOY inline asm code that looks as portable as a uhm as a very heavy object, like a neutron star */

/* probably best to make these 'inline' ? */
//g32
//req: d=1024x?xSYSMEM16 s=32x?xSYSMEM16
//used for basetiles
inline void g32(surf *d,unsigned long x,unsigned long y,surf *s,unsigned long i){
  static unsigned long pebx,pecx;
  pebx=(unsigned long)s->o+((i/8)*2048*8)+((i&7)*64);
  pecx=(unsigned long)d->o+x*2+y*2048;
  //ebx=source pointer ecx=destination pointer
  _asm{
    mov ebx,pebx
    mov ecx,pecx
#ifdef OPTION_HIRES
#include "inline_asm/fastHI.asm"
#else /* so ! OPTION_HIRES */
#include "inline_asm/fast.asm"
#endif /* OPTION_HIRES */
  } 
}


//used for overlaying dirt on basetiles
inline void g32z(surf *d,unsigned long x,unsigned long y,surf *s,unsigned long i){
  static unsigned long pebx,pecx;
  pebx=(unsigned long)s->o+((i/8)*2048*8)+((i&7)*64);
  pecx=(unsigned long)d->o+x*2+y*2048;
  //ebx=source pointer ecx=destination pointer
  _asm{
    mov ebx,pebx
      mov ecx,pecx
#include "inline_asm/fast2hi.asm"
  }
}

inline void sf32(surf *d,unsigned long x,unsigned long y,surf *s,unsigned long i){
  static unsigned long pebx,pecx;
  pebx=(unsigned long)s->o+((i/32)*2048*32)+((i&31)*64);
  pecx=(unsigned long)d->o+x*2+y*2048;
  //ebx=source pointer ecx=destination pointer
  _asm{
    mov ebx,pebx
      mov ecx,pecx
#include "inline_asm/fast3.asm"
  }
}

inline void sf32z(surf *d,unsigned long x,unsigned long y,surf *s,unsigned long i){
  static unsigned long pebx,pecx;
  pebx=(unsigned long)s->o+((i/32)*2048*32)+((i&31)*64);
  pecx=(unsigned long)d->o+x*2+y*2048;
  //ebx=source pointer ecx=destination pointer
  _asm{
    mov ebx,pebx
      mov ecx,pecx
#include "inline_asm/fast5.asm"
  }
}

inline void im32z(surf *d,unsigned long x,unsigned long y,surf *s,unsigned long i){
  static unsigned long pebx,pecx;
  pebx=(unsigned long)s->o+i*64*32;
  pecx=(unsigned long)d->o+x*2+y*2048;
  //ebx=source pointer ecx=destination pointer
    _asm{
      mov ebx,pebx
	mov ecx,pecx
#ifdef OPTION_HIRES
#include "inline_asm/fast4HI.asm"
#else /* so ! OPTION_HIRES */
#include "inline_asm/fast4.asm"
#endif /* OPTION_HIRES */
    }
}

void getspr(object* obj); //loads bt32 with a spr (uses keyframe) FIXME: candidate for optimising


bool keyhit(unsigned short k);

bool u6okeyhit(unsigned char i); //returns TRUE if key has been pressed
bool u6okeyon(unsigned char i); //returns TRUE if key has been pressed

void u6okeyseton(unsigned char i);
void u6okeysetoff(unsigned char i);

void GETINPUT_update();

void mididown(unsigned char instrument,unsigned char key);
void midiup(unsigned char instrument,unsigned char key);

void LIGHTnew(unsigned short x,unsigned short y,unsigned long light_data_offset, unsigned short x_axis_size);
//LIGHTnew has inline assembly FIXME

unsigned short objgettype(unsigned short type,unsigned char dir,unsigned char frm);

unsigned char objgetnextframe(unsigned short type,unsigned char frm);

void getsound(unsigned short type,long x,long y);
void getlight(unsigned short type,long x,long y);

//portraits 2.0 functions
void loadportrait(unsigned short i,surf *s);
surf *getportrait(unsigned short i);
surf *getportrait_doublesize(unsigned short i);
surf *getportrait_halfsize(unsigned short i);

unsigned char getambientlight(unsigned short x,unsigned short y);

void txtmakeu6ocompatible(txt* t); /* filters out nasty characters that would mess up things */
void STATUSMESSadd(txt *t);
void STATUSMESSadd(const char *t);

// s555
void STATUSMESSadd(txt *t, int skippable);
void STATUSMESSadd(const char *t, int skippable);
void STATUSMESSadd(const char *t, int skippable, int num);


long getsetting(const char*d);

void refresh();  // FIXME Inline assembly alert!

void scrlog(const char*d); //screen log

surf *loadimage2(LPCSTR name);
surf *loadimage2(txt* name);
surf *loadimage2(LPCSTR name,long flags);
surf *loadimage2(txt* name,long flags);

#endif /* FUNCTION_CLIENT_H */
