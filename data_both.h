#ifndef DATA_BOTH_H
#define DATA_BOTH_H

#include "define_both.h"
#include "mytxt.h"
#include <windows.h>
/* data structure definitions */

#pragma region
#pragma warning(disable: 4800)
typedef char int1;
typedef unsigned char uint1;
typedef short int2;
typedef unsigned short uint2;
typedef long int4;
typedef unsigned long uint4;
#pragma endregion Bulk sell patch, Xenkan 2010-12-05

//schedule
struct schedule_i{
	unsigned char hour;
	unsigned char day;
	unsigned short x;
	unsigned short y;
	unsigned char z;
	unsigned char type;
};

//special effects message structure
struct sfxtype{
	unsigned short type; //type of special effect (0=inactive)
	unsigned short x;
	unsigned short y;
	union{
		struct {
			unsigned short x2;
			unsigned short y2;
		};
		void *p;
	};
	unsigned short more; //distance bolt travels, ...
	union{
		float wait; //wait time or display time remaining
		unsigned char wait2[4];
	};
	unsigned long next; //next sf
};

struct object{
	unsigned short type; //bits values above 1024 are the sprite sub-index
	unsigned short info; //quantity/position of lever/creature type...
	//1 walking/movement state (for NPC and monsters)
	//2 NPC struct attached (->more)
	//4 crt attached
	//8 ->more is a pointer to a root object, refer to root
	//16temporary object delay value.byte1
	//32temporary object delay value.byte2
	//64temporary object delay value.byte3
	//128 1=stealing, 0=ok to take
	//256 1=quest item, 0=non-quest item
	//512	item info USED FOR BURNING DOWN TORCHES and now powder keg fun!, enchant uses multiples of 512 starting from 512*5 and going down with each enchant
	//1024	item info 
	//2048	item info
	//4096	item info
	//8192	?
	//16384	random treasure chest item id, moved it here so It wouldn't mess up with new item info changes easily.
	//32768 update object
	unsigned short x; unsigned short y; //position
	union{
		void* more; //pointer to more info (chest's objects)
		unsigned long more2;
	};
	void* prev; //pointer to previous object (NULL=first)
	void* next; //pointer to next object (NULL=last)
	
#pragma region
	bool IsStealing() { return info & 128; }
	bool IsQuestItem() { return info & 256; }
	bool IsNotForSale() { return info & 32768; }
	bool IsStackable();
	bool IsContainer();
	uint4 GetQuantity() { return more2; }
	void SetQuantity(uint4 q) { more2 = q; }
#pragma endregion Bulk sell patch, Xenkan 2010-12-05
};

struct player{
	unsigned long id;
	txt *name; //limit32
	txt *password; //limit32
	txt *newpassword; //limit32
	unsigned char male_female; //0=male, 1=female
	object* party[8]; //pointers to party members
	unsigned char selected_partymember; //party member under direct control

	//input information (updated based on INPUT message)
	unsigned char mf; //frame mouse is over (0=vs, 1=partymember 1, ...)
	unsigned short mx,my; //offset of mouse over frame
	unsigned long key; //keys/mouse buttons being held down
	unsigned char action; //input: action

	unsigned long key2; //backup of previous keys/mouse buttons
	object* mobj; //mouse cursor object
	object* mobj_last; //prev cursor object

	unsigned long net;

	unsigned long MESSAGEupdate_chksum; //checksum used avoid sending identical messages
	unsigned long NPCflags[256]; //256 converse index support (default value 0)
	unsigned long GNPCflags[256]; //256 DWORD (default value 0)
	unsigned char readmessage; //TRUE/FALSE (host set after general input message)
	object *craft; //skiff, raft, boat, balloon (temp activeplayer object*)
	object *craft_con; //temp object* to craft constructor
	float idle; //time since last "player active" message has been received
	float idle_connect; //time since last client-update-message has been received
	float save_update; //time since player data has been updated (every 8min)
	unsigned short px,py; //used when other values dont exist

	float karma;
	object *sitem[1024]; //stolen items list
	unsigned short sitem_next; //next free index

	unsigned char midi; //background midi

	float globalmessage_wait;
	unsigned short quan;
	unsigned short port;
	txt *npcname;

	unsigned char xray;
    unsigned char peer;

	//wind direction (NULL means wind is not being overridden!)
	char windx,windy;

	unsigned char updatemessage; //a basetile+object update message was sent last "clientframe"

	//keyboardtargetting for enemies (crts)
	object *ktar[10]; unsigned char ktar_wait[10];
	float ktar_display; //if non-zero keyboard targeting numbers are sent to the client

	unsigned char typing;
	unsigned char kallor;
	float speaking;

	unsigned char rest;//time till automatic removal of player (measured in host frames)

	unsigned char pathfind; //set to 1 if currently selected player should follow pathfind array
	unsigned char guardianstatuewait;

	object *staffcast_npc; //object* of npc using the staff
	object *staffcast_staff; //object* of staff

	unsigned short trapx[8],trapy[8]; //last location partymember was injured by a trap

	float online; //time player has spent online since logging in

	unsigned char instrument; //current/last instrument played
	unsigned char playinstrument; //1=currently playing instrument
	unsigned char respawn;

	//buffered selected player position
	unsigned short x,y;
	//object scene buffer info
	short sobj_bufoffx,sobj_bufoffy; //x,y offset of scene's top left corner
	unsigned short sobj_buf2bytes[96][72]; //number of 2 byte elements allocated
	unsigned short sobj_bufsize[96][72]; //number of 2 byte elements actually used
	unsigned short *sobj[96][72]; //scene object raw data
	unsigned long  sobj_tempfixed[96][72]; //temp fixed objects

	short mv_i;//number of indexes
	short mv_x[MVLISTLAST+1];
	short mv_y[MVLISTLAST+1];
	unsigned short mv_type[MVLISTLAST+1];//not including top 6 bits
	unsigned char mv_dir[MVLISTLAST+1];//direction (0=up, 1=right, 2=down, 3=left)
	unsigned char mv_frame[MVLISTLAST+1];//movement frame (only used by host for comparison with previous frame)
	object *mv_object[MVLISTLAST+1];//pointer to mover's object (if NULL movement cannot be performed)
	unsigned short mv_flags[MVLISTLAST+1];//flags
	unsigned char mv_hpmp[MVLISTLAST+1];//this way it's not updated unless a visible change has occurred
	unsigned long mv_playerid[MVLISTLAST+1];
	unsigned char mv_ktar[MVLISTLAST+1];
	unsigned short mv_more[MVLISTLAST+1];//type specific (rider for horses,?)

	object *talk_target;

	unsigned char resync;//set to 1 if resync has been requested

	float wizardeyetimeleft;//timeleft is in seconds
	short wizardeyex; short wizardeyey;
	float wizardeyemovewait;

	unsigned char mixer_volume;
	
#pragma region
	uint2 bulktypes[32];
	uint2 bulkqty[32];
	uint2 bulkprice;
	
	void GetPartyInventory(object*** Items, uint4* ItemCount);
#pragma endregion Bulk sell patch, Xenkan 2010-12-04
};

struct bitstream{
	txt *bytebuffer;
	unsigned long bitlength; //0=no data, 1=1 bit stored, etc.
};

struct objectinfo{ //this structure will form an array
	unsigned long flags;      
	//1 helm
	//2 neck
	//4 armour
	//8 weapon
	//16 double handed weapon
	//32 ring
	//64 boots
	//128 item can be picked up
	//256 item can be moved/pushed
	//512 things can be put on top of the item
	//1024 container (general)
	//2048 container (specific) v1,v2,v3,v4 *advanced implementation
	//4096 multiple
	unsigned short v1; //value 1: weapon damage
	unsigned short v2; //value 2: defense value
	unsigned short v3; //value 3: unknown
	unsigned short v4; //value 4: crt/npc type
	unsigned short v5; //value 5: unknown
	unsigned short v6; //value 6: unknown
	unsigned short v7; //value 7: unknown
	unsigned short v8; //value 8: unknown
	unsigned long weight; //x100 stones
};

struct creature{ //user for enemy characters that do not use weapons
	//standard info
	unsigned char crt_struct; //TRUE=crt struct, FALSE=npc struct
	void *more; //type specific info
	unsigned char al; //allengence
	short hp;
	float wait;
	object *items;
	unsigned char flags; //1=poison, 2=invisible, 4=asleep, 8=paralyzed, 16=fear, 32=charmed,128=custom flag
	unsigned char path[8064];
	unsigned short pathn; //number of instructions left unprocessed
	unsigned short path_max; //total number of instrucutions
	object* target;
	//creature specific info
	unsigned char charm; //time units till charm effect wears off! (always reverts to type 1)



	unsigned char mp;
	unsigned char cast; //1=cast, 0=wait


	//respawn info
	short respawn_hp;
	unsigned short respawn_x; unsigned short respawn_y;
};


struct npc{ //used for all people, including players (characters can use weapons)
	//standard info
	unsigned char crt_struct; //TRUE=crt struct, FALSE=npc struct
	void *more; //type specific info
	unsigned char al; //allengence
	short hp;
	float wait_walk;
	object *items;
	unsigned char flags; //1=poison, 2=invisible, 4=asleep, 8=paralyzed, 16=fear,32=charmed,128=custom flag
	unsigned char path[8064];
	unsigned short pathn; //number of instructions left unprocessed
	unsigned short path_max; //total number of instrucutions
	object* target;
	//npc specific info

	float wait_cast;
	float wait_attack;
	float wait_disable;



	unsigned short s; //str
	unsigned short d; //dex
	unsigned short i; //int

	unsigned short hp_max;
	short mp; //magic points
	unsigned short mp_max;
	unsigned long exp; //experience points
	unsigned short lev; //level (exp needed for next level is a secret)
	object* helm; //current weapons and armour
	object* wep_left; //*left and right and reversed on screen display
	object* wep_right;
	object* armour;
	object* boots;
	object* ring_left;
	object* ring_right;
	object* neck;
	unsigned short port; //character's portrait index
	unsigned short converse; //coni index (conversation number)

	schedule_i *schedule; //pointer to an array of schedules

	unsigned short schedule_last; //last worktype (probably current too!)
	short schedule_i; //current index being enacted (only set after NPC arrives at destination)


	unsigned char light; //value of light type

	unsigned char invisibility; //number of "ticks" player will automatically stay invisible for
	unsigned char protection;
	unsigned char negatemagic;

	object *baseitem; //base inventory item (bag, spellbook, chest)
	unsigned short baseitem_offset; //0=first item (always valid)
	unsigned long wt,wt_max; //carried weight, max weight
	unsigned long wt2,wt2_max; //wielded weight, max weight

	object* follow; //party member to follow

	unsigned char order;
	//0=none
	//1=follow party leader
	//2=schedule
	//3=attack target

	txt* name;

	unsigned long upflags; //update NPC flags
	//1		name, portriat, str, dex, int, lev, hp_max, mp_max
	//2		hp
	//4     mp
	//8     xp
	//16    weight (automatically set if inventory/equipped items change)
	//32    inventory, bag icon
	//64    equipped items
	//128   spellbook

	player* player; //pointer to a player structure (NULL if none)
	unsigned short arm; //armour value

	unsigned char food; //value of food the player has 1food=1hp
	//you can have a maximum of 255 food points
	//food is used even if your player is on full health

	unsigned short command;

	unsigned short originalobjtype;
	object *horse;

};

struct sockets_info{
	unsigned short i;//u6o socket connection array index
	txt *d[256];//array of txt*s
	unsigned char next;
	unsigned char nextfree;
	//other variables for temp use?
	long x4,x2,y2;
	txt *t;
	unsigned char exit_thread;
	unsigned long thread_id;
};


/* external variables defined in globals.h */
extern WSAData wsaData;
extern struct sockaddr_in server;
extern bitstream *currentbitstream;
extern objectinfo obji[4096];
extern unsigned long oldtime;
extern unsigned long newtime;
extern float et;
extern double ett;
extern file *log2file;
extern unsigned long U6O_SIGNATURE;
extern unsigned char incorrectversionmessage[9];
extern unsigned char NEThost;

extern unsigned long u6osocket;//host?
extern unsigned long u6osocket2;//client?
//info for recv and send threads
extern long socketclientlast;
extern unsigned long socketclient[SOCKETLAST+1];
extern unsigned long socketclient_ip[SOCKETLAST+1];
extern unsigned char socketclient_verified[SOCKETLAST+1];
extern sockets_info *socketclient_si[SOCKETLAST+1];
extern sockets_info *socketclient_ri[SOCKETLAST+1];
extern unsigned short socketclient_packetsizedownloaded[SOCKETLAST+1];
extern unsigned short socketclient_packetsize[SOCKETLAST+1];
extern unsigned char socket_timeout[SOCKETLAST+1];
extern unsigned char socket_disconnect[SOCKETLAST+1];
//temp wait value used by sockets_disconnect to force thread closure if necessary
extern unsigned char socket_disconnect_wait[SOCKETLAST+1];
extern bool endprogram; //TRUE if program is ending

extern HINSTANCE hInst;
extern TCHAR szWindowClass[100];
extern TCHAR window_name[100];
extern TCHAR szTitle[100];


// b111
extern int buildtablewithstorage;
extern int hlocx[BUILD_HOUSEID_MAX][BUILD_SECTIONID_MAX];
extern int hlocy[BUILD_HOUSEID_MAX][BUILD_SECTIONID_MAX];
extern object* hobj[BUILD_HOUSEID_MAX][BUILD_OBJPOINTER_MAX];
extern int arenalocx[BUILD_ARENAID_MAX];
extern int arenalocy[BUILD_ARENAID_MAX];
extern int arenasizex[BUILD_ARENAID_MAX];
extern int arenasizey[BUILD_ARENAID_MAX];
extern int arenaaddmobnum[BUILD_ARENAID_MAX];
extern int arenacount;


extern bool keyon[65536]; //TRUE if key is being held down
extern bool key[65536]; //TRUE if key has been pressed (manually set to FALSE)
extern bool key_gotrelease[65536]; //UNUSED?
extern bool keyasc[65536]; //TRUE if the ASCII indexed key has been pressed 
extern long mx;
extern long my;
extern long mb; //mouse input values (mb returns the button status)
extern unsigned long mbclick; //mbclick: recieved mouse_down message for mouse button
extern unsigned long mbheld; //set if the physical mouse button is being held down and has not been released
extern long wheel_move;
extern unsigned char mb_release;

extern unsigned char SCRLOG_FILEONLY;

extern double dv;
extern double dv2;

extern unsigned short bt[1024][2048]; //4M, top 6 bits reserved (0-1023, valid)

extern unsigned long sprlnk[1024]; //actual physical offset
extern unsigned char objpassflags[4096];
extern unsigned char objfloatflags[4096];

extern unsigned char tclass_object[65536]; //1=object
extern unsigned char tclass_mover[65536]; //1=mover 2=2-square mover (used in conjunction with first bit)
extern unsigned char tclass_fixed[65536]; //1=fixed
extern unsigned char tclass_build[65536]; //1=square, 2=horizontal, 4=vertically, 8=unique

extern player *tplayer;
extern player *tplayer2;
extern player *tplayer3;
extern npc *tnpc;
extern npc *tnpc2;
extern npc *tnpc3;
extern void* NETplayer;

//storm cloak arrays
extern unsigned char stormcloak[8][480*480];
extern unsigned short stormcloak_x[65536];
extern unsigned short stormcloak_y[65536];
extern player *stormcloak_player[65536];
extern short stormcloak_last;
extern short stormcloak_x2[128]; //local offsets of storm cloak fields to display
extern short stormcloak_y2[128];
extern char stormcloak_last2;
extern unsigned char stormcloak_mask[8][8];

extern unsigned char u6orevive;
extern unsigned long u6opi; //u6o program index (0=unknown location)
extern unsigned long u6opi_old;
extern unsigned long u6opl; //u6o program line
extern file *u6orevive_fh;

extern unsigned long *objname;
extern unsigned char *objname2;
extern unsigned long *tsign;
extern unsigned char *tsign2;

extern txt *spellname[256];
extern unsigned char spellreagent[256];
extern unsigned char spelltarget[256]; 

extern unsigned short objfixed_next;
extern unsigned short objfixed_type[65536]; //number, object types
extern unsigned short objfixed_index[1024][2048];
extern unsigned short tobjfixed_next;
extern unsigned short tobjfixed_type[65536]; //[number of objects],[object type(s)],...
extern unsigned short tobjfixed_index[1024][2048];

extern float btime;
extern float btime_last;
extern double btime2; //ultra precise universal britannian clock!
extern unsigned char btimeh; //Britannian hour
extern unsigned char bday; //day is a value between 1 and 7

extern long tpx;
extern long tpy;
extern long tpl; //used to store each client position (temp)

extern bool exitrequest;
extern bool exitrequest_noconfirm;

//local comparison buffer
extern short mv_i;//number of indexes
extern short mv_x[MVLISTLAST+1];
extern short mv_y[MVLISTLAST+1];
extern unsigned short mv_type[MVLISTLAST+1];//not including top 6 bits
extern unsigned char mv_dir[MVLISTLAST+1];//direction (0=up, 1=right, 2=down, 3=left)
extern unsigned char mv_frame[MVLISTLAST+1];//movement frame (only used by host for comparison with previous frame)
extern object *mv_object[MVLISTLAST+1];//pointer to mover's object (if NULL movement cannot be performed)
extern unsigned short mv_flags[MVLISTLAST+1];//flags
extern unsigned char mv_hpmp[MVLISTLAST+1];//this way it's not updated unless a visible change has occurred
extern unsigned long mv_playerid[MVLISTLAST+1];
extern unsigned char mv_ktar[MVLISTLAST+1];
extern unsigned short mv_more[MVLISTLAST+1];//type specific (rider for horses)
//flags/pointers used while comparing buffers
extern unsigned long mv_last[MVLISTLAST+1];
extern unsigned long mv_new[MVLISTLAST+1];

extern unsigned char mover_offseti[7][7];
extern char mover_offsetx[32];
extern char mover_offsety[32];

extern txt *mess1;
extern txt *mess_UPDATEps;
extern txt *mess_SF;
extern unsigned long u6o_namecolour;

extern unsigned char HOST_portrait_loaded[65536];
extern unsigned long HOST_portrait_next;
extern unsigned short *HOST_portrait_data[65536];

extern unsigned long tu6oid; //temp U6OID

extern long lastsecond;
extern long framerate;
extern long framecount; //framerate frames/sec

extern unsigned int windowsizecyclenum;

// s555 turn on/off new changes
extern int easymodehostn1;
extern int enhancehostn1;
extern int enhanceclientn1;


/* init_function */
void data_both_init(void);
#endif /* DATA_BOTH_H */
