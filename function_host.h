#ifndef FUNCTION_HOST_H
#define FUNCTION_HOST_H
/* To be able to include this and the link link against the object file
* only prototypes and externs go in here, and MAYBE some inline functions 
* actual global variable definitions should be moved to u6o7.cpp itself. (#include "global_vars.h")
*/
#include <winsock2.h>
#include "define_host.h" /* HOUSEMAX etc */
#include "data_host.h"  /* od[][] etc. */
#include "data_both.h" /*types*/

//sockets_accept sets newsocket to new sockets id and waits till user sets
//newsocket=INVALID_SOCKET before accepting new sockets
//shared global variables
extern unsigned long newsocket;
extern unsigned long newsocket_ip;
//temp local variables (transferred to globals after successful connection)
extern unsigned long tnewsocket;
extern unsigned long tnewsocket_ip;
extern HANDLE hsockets_accept;
extern DWORD idsockets_accept;
extern unsigned short AUTOPICKUPfirst;
extern unsigned short AUTOPICKUPnextfree;
extern object *AUTOPICKUPobject[65536];
extern double AUTOPICKUPett[65536];//time at which object was added to list (for later removal)
extern player *AUTOPICKUPplayer[65536];//player* of npc which used the item
extern object *AUTOPICKUPpartymember[65536];
extern unsigned char AUTOPICKUPflags[65536];//1=not-for-sale,2=?,...
extern unsigned char AUTOPICKUP_OBJECTVALID[1024];//array to quickly check if an item could be an autopickup item
extern DWORD WINAPI sockets_accept(LPVOID null_value);
extern unsigned char OBJcheckflags_flags;
extern unsigned long OBJcheckflags_td;
//extern unsigned char OBJadd_allow;
extern object *OBJtmp,*OBJtmp2,*OBJtmp3; 
extern object *OBJaddtocontainer_containermore;
extern unsigned short housex1[HOUSEMAX],housey1[HOUSEMAX],housex2[HOUSEMAX],housey2[HOUSEMAX]; //x,y limits of house
extern unsigned short housepnext[HOUSEMAX];
extern unsigned short housepx[HOUSEMAX][512]; extern unsigned short housepy[HOUSEMAX][512];
extern unsigned short housecost[HOUSEMAX]; //gold cost per REAL day (deducted if current system day!=logged day)
extern unsigned short houseinitialcost[HOUSEMAX];
// s111 increase house storage max slots
//extern unsigned char housestoragenext[HOUSEMAX];
extern unsigned int housestoragenext[HOUSEMAX];
extern unsigned short housestoragex[HOUSEMAX][HOUSESTORAGESLOTMAX]; extern unsigned short housestoragey[HOUSEMAX][HOUSESTORAGESLOTMAX];
extern unsigned char housestorageadd;
extern unsigned char housestoragerestore; //flags used when saving/restoring house items in file
extern unsigned short houseentrancex[HOUSEMAX],houseentrancey[HOUSEMAX];

// t111
//extern object* moblistnew[50];
//extern int mobcountnew;


//house creation tool 1.0 variables
extern unsigned short patchx;
extern unsigned short patchy; //base offset for adding objects/changing basetiles
extern unsigned short housenumber; //house currently being created (1-?, 0 RESERVED, 65535=non-specific)
/* luteijn: mismatches with patches' use of housenumber=basehousenumber+... Quick fix..
* long basehousenumber=20;
*/
extern unsigned short basehousenumber;

extern object *MOVERNEW_OBJECT;
extern unsigned char OBJmove_allow;
extern object *OBJlist_list[65536]; //object list
extern long OBJlist_last;

extern unsigned long wpf_weight[512][512];//weight of that square (a constant throughout the pathfind process)
extern unsigned char wpf_sourcedest[512][512];//0=not set, 1=from source, 2=from dest
extern unsigned long wpf_bestweight[512][512];//lowest weight of the source/dest node that travelled through this point (used for path tracing)

//node array
extern unsigned short wpf_nx[65536];
extern unsigned short wpf_ny[65536];
extern unsigned long wpf_nweight[65536];
extern unsigned char wpf_nsource[65536];//1 if source else dest

extern long wpf_lastusedn;

extern unsigned short wpf_stackn[65536];
extern long wpf_laststackedn;

extern unsigned long nweight;//nodeweights to process next
extern unsigned long nextnweight;

extern unsigned char join2sourcepath[65536];
extern unsigned char join2destpath[65536];

extern unsigned char wpf_nextto;
extern npc *wpf_npc;

extern unsigned char wpf_pathfound;
extern unsigned char wpf_nodeaddflags;

//entry values
extern unsigned char WPF_NEXTTO;
extern object *WPF_OBJECT;
//return values
extern unsigned char WPF_RETURN;
extern unsigned long WPF_PATHLENGTH;
extern long WPF_OFFSETX,WPF_OFFSETY;//map offset of array


extern unsigned short OBJcheckbolt_x,OBJcheckbolt_y; //inpact (x,y) if TRUE
extern float Ocb_x,Ocb_y,Ocb_gx,Ocb_gy,Ocb_l;
extern short Ocb_ix,Ocb_iy,Ocb_i,Ocb_il;
extern object* Ocbo;

extern object* OBJtl[65536];
extern unsigned long WTf_i; //next to implement
extern unsigned long WTf_n; //next unused index
extern unsigned long WTf_w;
extern unsigned long WTf_w2;
extern unsigned long WTf_itemn;
extern long houseowner_FAILVALUE;
//CON temp
extern long CONreg[256];
extern unsigned long CONerr;
extern unsigned short CONnpc; //NPC tplayer is talking to
extern unsigned long CONrnd; 
extern long CONnumber;
extern unsigned long CONqual;
extern unsigned char CONpartymember;
extern unsigned long CONport;
extern unsigned short CONhousecost;
extern unsigned short CONhouseinitialcost;

extern npc *CONnpc2; //only valid if #converse is derived from an NPC pointer!


extern txt *stealing_txt;
extern unsigned char stealing_MESSAGE;


extern long objsave_last;
extern unsigned short objsave_x[65536*4];
extern unsigned short objsave_y[65536*4];
extern object *objsave_obj[65536*4]; //pointer to first saved object
extern float objsave_wait[65536*4];

extern object *hirl_obj[HIRELINGS_MAX]; //list of hirelings for respawning
extern float hirl_wait[HIRELINGS_MAX];

extern object *objsave_node[65536];
extern long objsave_node_last;

extern long roundfloat_l;
//getwindspell return values
extern long WINDSPELL_boltn; extern long WINDSPELL_boltx[5]; extern long WINDSPELL_bolty[5];
extern long WINDSPELL_n; extern long WINDSPELL_x[128]; extern long WINDSPELL_y[128];
extern HANDLE hrevive_infiniteloopexit;
extern DWORD idrevive_infiniteloopexit;

extern unsigned char partyadd_checkarray[7][7];
/*
XXXXXXX
XX???XX
X?????X
X?? ??X
X?????X
XX???XX
XXXXXXX
*/
extern object *HORSEDISMOUNT_HORSEOBJECT;//NULL if unavailable




void function_host_init(void); // groups all the assignments to globals etc. *yuck*




void house();
void AUTOPICKUPadd(object* partymember, object *obj);//adds an item to the autopickup list
object *AUTOPICKUPcheck(player* tplayer, object *obj); //returns an object pointer to the npc to return the item to or NULL

/*
//volatile links (vlnk)
void *vlnkb_lnk[65536]; //vlnk buffer ~0.25M lnk (dest) object
unsigned long vlnkb_off[65536]; //vlnk buffer ~0.25M lnk offset
void *vlnkb_lnks[65536]; //vlnk buffer ~0.25M lnk source object
long vlnkb_last=-1; //last
unsigned long vlnkb_free[65536]; //free vlnkb index
long vlnkb_free_last=-1; //last free vlnkb index
//functions
//VLNK_new(_lnks,_lnk,_off)
//VLNK_remove(_lnk) NULL any link pointing to this object, then remove vlnk
//VLNK_sremove(_lnk) remove any vlnk belonging to _lnk
//! what if the object that created the vlnk is destroyed?
//  ANS OBJdestroy removes any links belonging to the obj to be destroyed
//      then destroys any links pointing to itself created by others
//      then it is removed
*/

void VLNKnew(void *lnks,void *lnk,unsigned long off);
void VLNKremove(void *lnk); //remove any vlnk to lnk from other object
void VLNKsremove(void *lnks); //remove all vlnk using offset within lnks


//OBJnew
//OBJcheckflags
//OBJadd
//OBJremove
//OBJmove !checks to see if walking creatures can pass
//OBJfindlast !not including floating objects
//OBJfindlastall
//OBJmove2 !2003: moves a creature/NPC and adjusts picture sub-index

object *OBJnew();
unsigned char OBJadd(unsigned long x,unsigned long y,object* obj);
void OBJrelease(object* obj);

/* free structures properly from memory */
void free(player* plr);
void free(creature* crt);
void free(npc* tnpc);

//bt[][] flag description
//[1111111]flags [111111111]tile index
//512
//1024
//2048
//4096
//8192
//16384
//343/.

//1024
//2048
//
void OBJcheckflags(unsigned long x,unsigned long y);



void OBJaddtocontainer(object *container,object* objecttoadd);


/* luteijn: all the patches use x,y as arguments all the time and they are longs. Quick fix..
* void BTset(unsigned short x,unsigned short y,unsigned short i){  
*/
void BTset(long x,long y,unsigned short i);

/* luteijn: away with thee! 
* void encrypt(txt *t4);
*/

void decrypt(txt *t4); // needed for backwards compatibility. Don't make new use of this!

void addu6monsterdropitems(object *crtobj);

void crtrespawn(object *obj);

/*
MOVERNEW: FLAGS
flags can be set globally, in which case they need to be reset when finished
or locally by being passed as flags
local flags and OR-ed with global flags
*/
//unsigned long MOVERNEW_GLOBALFLAGS=0;
//unsigned char MOVERNEW_ERROR;//valid when movernew(...) returns 0
//1=FAILED (object is not a mover)
//2=IGNORED (object is not the primary part of a mover)
unsigned char movernew(unsigned short type,unsigned short x,unsigned short y,unsigned long flags);

object* OBJaddnew(unsigned long x,unsigned long y,unsigned short type,unsigned short info,unsigned long more2);

void OBJremove(object* obj); //OBJremove detects container objects

unsigned char OBJmove(object* obj,unsigned long x,unsigned long y);

object* OBJfindlast(unsigned long x,unsigned long y);

object* OBJfindlastall(unsigned long x,unsigned long y);

//OBJlist creates list of all internal&next objects, returns number of objects (0=none)
//NOTE: OBJlist creates a global list, user sets to NULL
unsigned long OBJlist(object* obj);

//OBJmove2 !2003: moves a creature/NPC and adjusts picture sub-index
//x,y are it's new position
unsigned char OBJmove2(object* obj,unsigned long x,unsigned long y);

//OBJdir changes direction of NPC/creature to face x,y
unsigned char OBJdir(object* obj,unsigned long x,unsigned long y);

void wpf_pathfind(unsigned char *d,long sourcex,long sourcey,long destx,long desty,long maxdistance,unsigned long type,unsigned long flags);
//returns a comparitive value re. danger of a square for a mover to walk on/over
unsigned char wpf_dangercheck(short x,short y,object* mover);

bool OBJcheckbolt(unsigned short x,unsigned short y,unsigned short x2,unsigned short y2);

unsigned long SFnew(unsigned short x,unsigned short y);

unsigned long WTfind(object* obj);

unsigned char houseowner(player *p,long housenumber);

//save housesav buffer to disk (with current date)
void housesav_update();


long CON_gv(long v);
void CON_sv(long v,long v2);

void stealing(player *tplayer,object *obj);

object * npc_to_obj(npc * tnpc,player * tplayer);

unsigned char stormcloakcheck(unsigned short x,unsigned short y);

unsigned char stormcloakcheck2(unsigned short x,unsigned short y,player* p);

unsigned char housecheck(unsigned short x,unsigned short y);

void inbritanniacheck();

unsigned char ENHANCEget_attack(object* obj);

unsigned char ENHANCEget_defense(object* obj);
void OBJsave(unsigned short x,unsigned short y);

//calculate mp_max of hired partymember based on INT
unsigned short get_mp_max(unsigned short i); 

unsigned char objvisible(player *p,object* myobj);

long roundfloat(float f);

void getwindspell(long sx,long sy,long dx,long dy);

DWORD WINAPI revive_infiniteloopexit(LPVOID null_value);

//returns player->party[0] or resu[...] if dead
object *getprimarypartymemberobj(player *sourceplayer);

//see getprimarypartymemberobj
object *getpartymemberobj(player *sourceplayer,unsigned char n);

//used for crts with monster-drop inventories to determine the extra armour they could be using
long getequiparmourvalue(object* obj);

long getequiph2hwepdamage(object* obj);

//the purpose of this function is to:
//1. spread out party after using a boat, balloon, ladder, etc.
//2. fix certain piling up bugs
//3. remove any prerecorded paths
//all alive partymembers are assumed to be in a removed but existant state
void partyadd(player *p,long x,long y);

void horsedismount(object *myobj);//forces npc to dismount horse

unsigned char usinghorse(player *tplayer);

void additem(npc* tnpc,object* obj);
//adds an item to an npc's currently open bag (or root inventory if no bag is open)
//note: item must be removed first!

void additemroot(npc* tnpc,object* obj);
//adds an item to an npc's ROOT inventory regardless of which bag is open
//note: item must be removed first!

unsigned long randomchestlocation(bool tmap);

unsigned long randchestitem();

void ENHANCEnewn(object* obj,unsigned short n, unsigned short n2);

void addhireling(unsigned long x3, schedule_i *sched);


// c111
unsigned int new1_getexpdeduction(npc* npc, int option);

// t111
int getarenaid(player* player);


#endif FUNCTION_HOST_H

