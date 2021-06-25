#ifndef DATA_HOST_H
#define DATA_HOST_H

#include "define_host.h" /*SAVESLOTLAST etc */
#include "mytxt.h" /* txt* etc */
#include "data_both.h" /* schedule* etc. */


/* structure definitions */
struct housesav_info{
  unsigned short flags;
  //1 occupied (vacant if NULL)
  //*EXCEPTION*: array index 0: flags set to day of month)
  unsigned char username[32];
  unsigned char charactername[32];
  unsigned short gold;
};
//converse information
struct npcbin_i{ //npcbin information/instruction
  long l1;
  long l2;
  long l3;
  long l4;
};
struct objentry
{
  unsigned char status;
  unsigned char unk_h;
  unsigned char unk_d1;
  unsigned char unk_d2;
  unsigned short type;
  unsigned char qty;
  unsigned char tag;
};
struct mlobj{ //multi-object index array
  object *obj[1]; //use 1 to avoid compile warning
};
struct crtenum_struct{
  char x;
  char y;
};



/* external variables globals.h */
extern unsigned char save_buffer[SAVESLOTLAST+1];
extern txt *save_username[SAVESLOTLAST+1];
extern txt *save_password[SAVESLOTLAST+1];
extern txt *save_name[SAVESLOTLAST+1];
extern unsigned long save_exp[SAVESLOTLAST+1];
extern unsigned long save_bytes[SAVESLOTLAST+1];
extern unsigned char save_dump;
extern unsigned char login_dead_callback;

//1=mover drops blood, 0=mover doesn't drop blood
extern unsigned char mover_blood[1024];
extern unsigned short mover_body[1024];

extern unsigned char sfbuffersend;
extern float sfbufferwait;

extern unsigned char cast_spell;
extern unsigned char staff_cast_spell;

extern long CASTSPELL_SPELLTYPE;

extern txt *admins[ADMINSMAX];

extern txt *motd;
extern txt *inbritannia;
extern long inbritannia_totalplayers;

extern unsigned long U6ONEWID; //never 0

extern housesav_info housesav[256];

//orb of the moons destinations
extern unsigned short orbx[5][5];
extern unsigned short orby[5][5];

//wind direction (global)
extern char windx;
extern char windy;
extern unsigned char windnew;

extern unsigned char party_ok[8];
extern unsigned short save_version;
extern unsigned char encryptcode[65536];

extern unsigned char itemused;
extern bool spellattcrt;

extern unsigned short autosetup;
extern bool autosetup_next;
extern unsigned short autosetup_counter;

extern bool autospell;

extern unsigned long treagent[8];
extern unsigned long tspell[256]; //set to 1 if spell exists, then added to if reagents exist

//jump/call-back flag for formatting username and charactername
extern unsigned char format_usernames;

extern unsigned long objr_x;
extern unsigned long objr_y;

extern schedule_i schedule[256][32];
extern schedule_i schedule2[1024][32];

extern unsigned long npci[256];
extern unsigned char *npcinf;
extern npcbin_i *npcbin;

extern unsigned long sfi[256][256]; //pointer to first sf
extern sfxtype sf[65536]; //clear every cycle
extern unsigned long sf_playerid[65536];//only valid for text messages where top bit of port is set
extern long sfn; //last sf
extern unsigned long sfx_playerid[256];

extern unsigned char btu6[1024][1024];
extern unsigned char btu60[256][256];

extern unsigned short npcobj[256];


extern objentry ol[32768]; //modify to allow for more complex base maps
extern long oln;

extern unsigned char btflags[256];

extern object objb[524228]; //host object buffer ~10M
extern long objb_last; //last objb
extern unsigned long objb_free[524228]; //free objb index
extern long objb_free_last; //last free objb index

//doorclose: automatically locks unlocked doors after 2 hours if unused and noone has passed through
extern long doorclose_last;
extern float doorclose_wait[1024];
extern object *doorclose_obj[1024],*doorclose_obj2[1024];
extern unsigned short doorclose_oldtype[1024],doorclose_oldtype2[1024];

//leverchange
extern long leverchange_last;
extern float leverchange_wait[1024];
extern object *leverchange_obj[1024];
extern unsigned short leverchange_oldtype[1024];



extern unsigned char showmoongates;
extern unsigned short moongatex[8],moongatey[8];
extern object *moongate[8][2];
extern unsigned char moonphase;

extern object *nuggetsfix;

//volatile links (vlnk)
extern void *vlnkb_lnk[65536]; //vlnk buffer ~0.25M lnk (dest) object
extern unsigned long vlnkb_off[65536]; //vlnk buffer ~0.25M lnk offset
extern void *vlnkb_lnks[65536]; //vlnk buffer ~0.25M lnk source object
extern long vlnkb_last; //last
extern unsigned long vlnkb_free[65536]; //free vlnkb index
extern long vlnkb_free_last; //last free vlnkb index

extern object* portcullis[256][16];
extern object* lever[256][16];
extern object* efield[256][16]; //electric field
extern object* eswitch[256][16]; //electric switch

extern crtenum_struct crtenum[1073];
extern object* crtenum_pathok_castok[1024];
extern short crtenum_pathok_castok_i;
extern object* crtenum_pathok[1024]; 
extern short crtenum_pathok_i;
extern object* crtenum_castok[1024]; 
extern short crtenum_castok_i;

//resurrect info
extern object *resu[65536]; //object
extern object *resu_body[65536]; //dead body object
extern unsigned short resu_body_type[65536];//object type of dead body (used to remake lost body and check if correct)
extern float resu_wait[65536]; //time until object automatically resurrected
extern player *resu_player[65536]; //player object belongs to
extern unsigned char resu_partymember[65536]; //party member index
extern unsigned short resu_x[65536],resu_y[65536];
extern long nresu;


//wizard eye(s) list: locations of wizard eyes currently displayed in britannia

extern unsigned char wizardeyesi;
extern unsigned short wizardeyesx[256];
extern unsigned short wizardeyesy[256];
extern unsigned char wizardeyesi2;
extern unsigned char wizardeyesadded;
extern object *wizardeyesobj;

extern unsigned char economy_setup[1024][4];
extern long economy_limit[1024][4];
extern long economy_value[1024][4];
extern long economy_change[1024][4];


//when a monster is "killed" it is added to the respawn list
//a delay is also set
//
//respawn info
extern object *respawn[1200];
extern unsigned short respawn_delay[1200]; //number of seconds till creature will respawn
//*note: creature will not respawn if player is too near (eg. 8 squares or less)
extern long respawn_last;




extern player* playerlist[1024]; //supports up to 1024 online players
extern long playerlist_last;


extern unsigned char chunks[1024][8][8]; //"c:\ultima6\chunk."

extern unsigned char u6wci2[8][8][16][8][3];
extern unsigned short u6wci[128][128];
extern unsigned char u60ci2[32][16][3];
extern unsigned short u60ci[32][32];

extern object *od[1024][2048]; //8M, pointers to all data, NULL pointers are common





extern object *oul[65536]; //list of active objects to update (size: 1M)
extern long ouln;
extern object *f_oul[65536]; //list of fixed objects to update
extern long f_ouln;

extern txt *tname;
extern txt *tusername;
extern txt *tuserpassword;
extern txt *tnewuserpassword;
extern unsigned char tcustomportrait_upload;
extern unsigned short tcustomportrait[3584];
extern unsigned long tnamecolour;
extern unsigned char tmale_female;
extern unsigned short tport;
extern unsigned short ttype;
extern unsigned char tcreatecharacter;

//keyb target
extern char ktar_x[768];
extern char ktar_y[768];
extern float ktar_xydis[768];

/*
MOVERNEW: FLAGS
flags can be set globally, in which case they need to be reset when finished
or locally by being passed as flags
local flags and OR-ed with global flags
*/
extern unsigned long MOVERNEW_GLOBALFLAGS;
extern unsigned char MOVERNEW_ERROR;//valid when movernew(...) returns 0
//1=FAILED (object is not a mover)
//2=IGNORED (object is not the primary part of a mover)


#ifndef CLIENT
//STUBS TO SUPPORT EXTERNAL LINKING (DATA NOT ACTUALLY USED!)
extern unsigned char u6omidisetup;
extern HWND hWnd;
extern HWND hWnd2;
extern HWND hWnd3;
extern HWND hWnd4;
extern RECT desktop_rect;
extern bool smallwindow;
extern unsigned char u6ovolume;
#endif

extern unsigned long revive_infiniteloopexit_i;
extern unsigned long revive_infiniteloopexit_i2;
extern unsigned short revive_infiniteloopexit_i3;

extern unsigned long mycount;
extern long newschedule2;
extern object* newll;
extern unsigned long ol_tag;
extern unsigned long ol_tag_prev;

/* function prototypes */
void data_host_init(void);

#endif /* DATA_HOST_H */
