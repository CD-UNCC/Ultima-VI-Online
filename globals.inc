#ifndef GLOBALS_H
#define GLOBALS_H
/*
 * actual definition of the global variables, to be included in the main program
 * these are referred to as extern everywhere else
 */

long global_TCP_listen_port=22;

/* variables originally from frame.h */
FRAME* firstpanel;
FRM_ONOFF *ONOFF_hold;
//frame: information
FRAME *FRAME_mblf[2];
bool FRAME_mbl[2];
long* objtype;
FRAME *drg;
bool FRAME_drg_begin;
FRM_TYPE* FRM_type;
FRAME *pn,*lpn,*pn2,*pn3;
FRM_ONOFF *tonoff;
FRM_IMAGE *timage;
FRM_INPUT *tinp;
long FRAME_mb;
HFONT thfont;
FRM_TXT *ttxt;

// rrr
static unsigned int resxa = 1024;
static unsigned int resya = 768;

static unsigned int resxb = 512;
static unsigned int resyb = 384;
static unsigned int respb = resxb*resyb;

static unsigned int resxc = 1200;
static unsigned int resyc = resxc*3/4;
static unsigned int respc = resxc*resyc;

unsigned int resxo = resxa;
unsigned int resyo = resya;

unsigned int resxs = resxb;
unsigned int resys = resyb;

unsigned int resxn1w = resxc;
unsigned int resyn1w = resyc;

unsigned int resxn1m = resxn1w - 260;
unsigned int resyn1m = resxn1m*3/4;
unsigned int respn1m = resxn1m * resyn1m;

unsigned int resxz;
unsigned int resyz;

unsigned int windowsizecyclenum = 0;
unsigned int windowsizecyclemax = 1;

long omx3;
long omy3;

double scalexm = 1.0f;
double scaleym = 1.0f;

// r999 new
surf* uipanelsurf[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
int uipanelx[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
int uipanely[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
int uipanelsizex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
int uipanelsizey[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
float uipanelscalex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
float uipanelscaley[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
int uipanelscaling[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
int uipanelhitenable[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
int uipanelusedefaultstatedata[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];

int uipaneli[UI_PANEL_MAX][UI_PANELWIDGET_MAX];
int uipanelcount;
int uipanelwidgetcount[UI_PANEL_MAX];
int uipanelsidebar, uipanelminimap, uipanelworldmap, uipanelworldmapbar, uipanelpartymemberparent, uipanelactionbarparent, uipanelactiontalkbarparent;
int uipanelactionbar1, uipanelactionbar2, uipaneloptionbar1, uipanelactiontalkbar1, uipanelactiontalkbar2, uipanelactiontalkbar3;
int uipanelpartymember0;
int uipaneloptioninfo;

// s555 turn on/off new changes
int easymodehostn1 = 0;
int enhancehostn1 = 0;
int enhanceclientn1 = 1;


/* function_both */
unsigned char OBJGETDIR_FRAME;
unsigned long BITSleftmask[33];//mask of index-many bits to keep

#ifdef HOST
/* variables originally from function_host.h */
unsigned long newsocket;
unsigned long newsocket_ip;
unsigned long tnewsocket;
unsigned long tnewsocket_ip;
HANDLE hsockets_accept;
DWORD idsockets_accept;
unsigned short AUTOPICKUPfirst;
unsigned short AUTOPICKUPnextfree;
object *AUTOPICKUPobject[65536];
double AUTOPICKUPett[65536];//time at which object was added to list (for later removal)
player *AUTOPICKUPplayer[65536];//player* of npc which used the item
object *AUTOPICKUPpartymember[65536];
unsigned char AUTOPICKUPflags[65536];//1=not-for-sale,2=?,...
unsigned char AUTOPICKUP_OBJECTVALID[1024];//array to quickly check if an item could be an autopickup item
DWORD WINAPI sockets_accept(LPVOID null_value);
unsigned char OBJcheckflags_flags;
unsigned long OBJcheckflags_td;
//unsigned char OBJadd_allow;
object *OBJtmp,*OBJtmp2,*OBJtmp3; /* luteijn: static global variables to make them init to 0? */
object *OBJaddtocontainer_containermore;
unsigned short housex1[HOUSEMAX],housey1[HOUSEMAX],housex2[HOUSEMAX],housey2[HOUSEMAX]; //x,y limits of house
unsigned short housepnext[HOUSEMAX];
unsigned short housepx[HOUSEMAX][512]; unsigned short housepy[HOUSEMAX][512];
unsigned short housecost[HOUSEMAX]; //gold cost per REAL day (deducted if current system day!=logged day)
unsigned short houseinitialcost[HOUSEMAX];
// s111 increase house storage max slots
//unsigned char housestoragenext[HOUSEMAX];
unsigned int housestoragenext[HOUSEMAX];
unsigned short housestoragex[HOUSEMAX][HOUSESTORAGESLOTMAX]; unsigned short housestoragey[HOUSEMAX][HOUSESTORAGESLOTMAX];
unsigned char housestorageadd;
unsigned char housestoragerestore; //flags used when saving/restoring house items in file
unsigned short houseentrancex[HOUSEMAX],houseentrancey[HOUSEMAX];

// t111
//object* moblistnew[50];
//int mobcountnew = 0;
int num;
int addmobnum = 1;

// b111
int buildtablewithstorage = BUILD_TABLEWITHSTORAGE_NO;
int hlocx[BUILD_HOUSEID_MAX][BUILD_SECTIONID_MAX];
int hlocy[BUILD_HOUSEID_MAX][BUILD_SECTIONID_MAX];
object* hobj[BUILD_HOUSEID_MAX][BUILD_OBJPOINTER_MAX];
int arenalocx[BUILD_ARENAID_MAX];
int arenalocy[BUILD_ARENAID_MAX];
int arenasizex[BUILD_ARENAID_MAX];
int arenasizey[BUILD_ARENAID_MAX];
int arenaaddmobnum[BUILD_ARENAID_MAX];
int arenacount = 0;


// c222 allowed to cast spells that are in the last "used/registered" spellbook, even if it's unequiped.
object* playerspellbook = NULL;

//house creation tool 1.0 variables
unsigned short patchx;
unsigned short patchy; //base offset for adding objects/changing basetiles
unsigned short housenumber; //house currently being created (1-?, 0 RESERVED, 65535=non-specific)
/* luteijn: mismatches with patches' use of housenumber=basehousenumber+... Quick fix..
 * long basehousenumber=20;
 */
unsigned short basehousenumber;

object *MOVERNEW_OBJECT;
unsigned char OBJmove_allow;
object *OBJlist_list[65536]; //object list
long OBJlist_last;

unsigned long wpf_weight[512][512];//weight of that square (a constant throughout the pathfind process)
unsigned char wpf_sourcedest[512][512];//0=not set, 1=from source, 2=from dest
unsigned long wpf_bestweight[512][512];//lowest weight of the source/dest node that travelled through this point (used for path tracing)

//node array
unsigned short wpf_nx[65536];
unsigned short wpf_ny[65536];
unsigned long wpf_nweight[65536];
unsigned char wpf_nsource[65536];//1 if source else dest

long wpf_lastusedn;

unsigned short wpf_stackn[65536];
long wpf_laststackedn;

unsigned long nweight;//nodeweights to process next
unsigned long nextnweight;

unsigned char join2sourcepath[65536];
unsigned char join2destpath[65536];

unsigned char wpf_nextto;
npc *wpf_npc;

unsigned char wpf_pathfound;
unsigned char wpf_nodeaddflags;

//entry values
unsigned char WPF_NEXTTO;
object *WPF_OBJECT;
//return values
unsigned char WPF_RETURN;
unsigned long WPF_PATHLENGTH;
long WPF_OFFSETX,WPF_OFFSETY;//map offset of array


unsigned short OBJcheckbolt_x,OBJcheckbolt_y; //inpact (x,y) if TRUE
float Ocb_x,Ocb_y,Ocb_gx,Ocb_gy,Ocb_l;
short Ocb_ix,Ocb_iy,Ocb_i,Ocb_il;
object* Ocbo;

object* OBJtl[65536];
unsigned long WTf_i; //next to implement
unsigned long WTf_n; //next unused index
unsigned long WTf_w;
unsigned long WTf_w2;
unsigned long WTf_itemn;
long houseowner_FAILVALUE;
//CON temp
long CONreg[256];
unsigned long CONerr;
unsigned short CONnpc; //NPC tplayer is talking to
unsigned long CONrnd; 
long CONnumber;
unsigned long CONqual;
unsigned char CONpartymember;
unsigned long CONport;
unsigned short CONhousecost;
unsigned short CONhouseinitialcost;

npc *CONnpc2; //only valid if #converse is derived from an NPC pointer!

txt *stealing_txt;
unsigned char stealing_MESSAGE;

long objsave_last;
unsigned short objsave_x[65536*4];
unsigned short objsave_y[65536*4];
object *objsave_obj[65536*4]; //pointer to first saved object
float objsave_wait[65536*4];

object *hirl_obj[HIRELINGS_MAX]; //list of hirelings for respawning
float hirl_wait[HIRELINGS_MAX];

object *objsave_node[65536];
long objsave_node_last;

long roundfloat_l;
//getwindspell return values
long WINDSPELL_boltn; long WINDSPELL_boltx[5]; long WINDSPELL_bolty[5];
long WINDSPELL_n; long WINDSPELL_x[128]; long WINDSPELL_y[128];
HANDLE hrevive_infiniteloopexit;
DWORD idrevive_infiniteloopexit;

unsigned char partyadd_checkarray[7][7];
/*
   XXXXXXX
   XX???XX
   X?????X
   X?? ??X
   X?????X
   XX???XX
   XXXXXXX
   */
object *HORSEDISMOUNT_HORSEOBJECT;//NULL if unavailable

/* variables originally from function_host.h */
/* moved to function_both section */
#endif /* HOST */
#ifdef CLIENT
/* variables originally from function_client.h */
//GetInput variables 
//tab_pressed allows program to trap the tab key
//once trapped it also counts as an enterpressed, so serves a dual purpose
//otherwise tab key inserts an undefined amount of spaces
//it MUST be set after a call to getinput_setup
unsigned char leak=0;
unsigned char GETINPUT_tab_pressed;
txt *GETINPUT_txt;
unsigned char *GETINPUT_enterpressed;
txt *GETINPUT_old; //used to detect new pointers
unsigned long GETINPUT_maxlength; //maximum length of GETINPUT_txt (0=infinite)
long gs_i; //getspr static data
long gs_i2; //getspr static data
long gs_x; //getspr static data
long gs_y; //getspr static data
long gs_t; //getspr static data
unsigned long GSs;
unsigned long GSx;
unsigned long GSy;
unsigned char midikeyboard2[256]; //reverse of midikeybaord array!
unsigned char midikeyboard2_keyon[256]; //whether key is being held or not
short midikeyboard_set;
unsigned char musickeyboard_set;
unsigned char midikeystack[16][256];
float midikeywait[16][256];
unsigned char clientinstrument;
unsigned char playinstrument;
unsigned char midipause;
unsigned char getsound_MOVERSOUND;
unsigned char AMBIENTLIGHT_LIGHTVALUE;
unsigned char AMBIENTLIGHT_SHOWSUN;
txt *STATUSMESSprev[8];//the previous 8 status messages are stored here
txt *STATUSMESSdisplaying;//the message currently being displayed
float STATUSMESSwait;
unsigned char STATUSMESSskipok;//the message will be skipped if any messages are pending
txt *STATUSMESSt;//temp txt for building messages (included to aid conversion from older system)
txt *STATUSMESSpending;
txt *GETSETTING_RAW;//the actualt text between the square brackets [...]
txt *li2_t;
#endif /* CLIENT */


/* variables originally defined in data_both.h */
WSAData wsaData;
struct sockaddr_in server;
bitstream *currentbitstream;
objectinfo obji[4096];
unsigned long oldtime;
unsigned long newtime;
float et;
double ett;
file *log2file;
unsigned long U6O_SIGNATURE;
unsigned char incorrectversionmessage[9];
unsigned char NEThost=0;

unsigned long u6osocket;//host?
unsigned long u6osocket2;//client?
//info for recv and send threads
long socketclientlast;
unsigned long socketclient[SOCKETLAST+1];
unsigned long socketclient_ip[SOCKETLAST+1];
unsigned char socketclient_verified[SOCKETLAST+1];
sockets_info *socketclient_si[SOCKETLAST+1];
sockets_info *socketclient_ri[SOCKETLAST+1];
unsigned short socketclient_packetsizedownloaded[SOCKETLAST+1];
unsigned short socketclient_packetsize[SOCKETLAST+1];
unsigned char socket_timeout[SOCKETLAST+1];
unsigned char socket_disconnect[SOCKETLAST+1];
//temp wait value used by sockets_disconnect to force thread closure if necessary
unsigned char socket_disconnect_wait[SOCKETLAST+1];
bool endprogram; //TRUE if program is ending

HINSTANCE hInst;
TCHAR szWindowClass[100];
TCHAR window_name[100];
TCHAR szTitle[100];

bool keyon[65536]; //TRUE if key is being held down
bool key[65536]; //TRUE if key has been pressed (manually set to FALSE)
bool key_gotrelease[65536]; //UNUSED?
bool keyasc[65536]; //TRUE if the ASCII indexed key has been pressed 
long mx;
long my;
long mb; //mouse input values (mb returns the button status)
unsigned long mbclick; //mbclick: recieved mouse_down message for mouse button
unsigned long mbheld; //set if the physical mouse button is being held down and has not been released
long wheel_move;
unsigned char mb_release;

unsigned char SCRLOG_FILEONLY;

double dv;
double dv2;

unsigned short bt[1024][2048]; //4M, top 6 bits reserved (0-1023, valid)

unsigned long sprlnk[1024]; //actual physical offset
unsigned char objpassflags[4096];
unsigned char objfloatflags[4096];

unsigned char tclass_object[65536]; //1=object
unsigned char tclass_mover[65536]; //1=mover 2=2-square mover (used in conjunction with first bit)
unsigned char tclass_fixed[65536]; //1=fixed
unsigned char tclass_build[65536]; //1=square, 2=horizontal, 4=vertically, 8=unique

player *tplayer,*tplayer2,*tplayer3;
npc *tnpc,*tnpc2,*tnpc3;

void* NETplayer;

//storm cloak arrays
unsigned char stormcloak[8][480*480];
unsigned short stormcloak_x[65536];
unsigned short stormcloak_y[65536];
player *stormcloak_player[65536];
short stormcloak_last;
short stormcloak_x2[128]; //local offsets of storm cloak fields to display
short stormcloak_y2[128];
char stormcloak_last2;
unsigned char stormcloak_mask[8][8];

unsigned char u6orevive;
unsigned long u6opi; //u6o program index (0=unknown location)
unsigned long u6opi_old;
unsigned long u6opl; //u6o program line
file *u6orevive_fh;

unsigned long *objname;
unsigned char *objname2;
unsigned long *tsign;
unsigned char *tsign2;

txt *spellname[256];
unsigned char spellreagent[256];
unsigned char spelltarget[256]; 

unsigned short objfixed_next;
unsigned short objfixed_type[65536]; //number, object types
unsigned short objfixed_index[1024][2048];
unsigned short tobjfixed_next;
unsigned short tobjfixed_type[65536]; //[number of objects],[object type(s)],...
unsigned short tobjfixed_index[1024][2048];

float btime;
float btime_last;
double btime2; //ultra precise universal britannian clock!
unsigned char btimeh; //Britannian hour
unsigned char bday; //day is a value between 1 and 7

long tpx;
long tpy;
long tpl; //used to store each client position (temp)

bool exitrequest;
bool exitrequest_noconfirm;

//local comparison buffer
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
unsigned short mv_more[MVLISTLAST+1];//type specific (rider for horses)
//flags/pointers used while comparing buffers
unsigned long mv_last[MVLISTLAST+1];
unsigned long mv_new[MVLISTLAST+1];

unsigned char mover_offseti[7][7];
char mover_offsetx[32];
char mover_offsety[32];

txt *mess1;
txt *mess_UPDATEps;
txt *mess_SF;
unsigned long u6o_namecolour;

unsigned char HOST_portrait_loaded[65536];
unsigned long HOST_portrait_next;
unsigned short *HOST_portrait_data[65536];

unsigned long tu6oid; //temp U6OID

long lastsecond;
long framerate;
long framecount; //framerate frames/sec

#ifdef CLIENT
/* variables originally from data_client.h */
FRAME *pmf;
RECT desktop_rect;
HMIDIOUT midiout_handle;
unsigned char midiout_setup;
unsigned char U6O_DISABLEMUSIC;
unsigned char U6O_DISABLEJOYSTICK;
unsigned char JDISABLED;
unsigned char fonts_added;

float intro_timer;

unsigned short U6OK_DEFAULT[128][2];
unsigned short U6OK[128][2];
unsigned short U6OK_TEMP[128][2];
//1 INSTANTCLICK ON,?
unsigned char U6OK_DEFAULT_FLAGS[128];
unsigned char U6OK_FLAGS[128];
unsigned char U6OK_TEMP_FLAGS[128];

HFONT fnt1;
HFONT fnt2;
HFONT fnt3;
HFONT fnt4;
HFONT fnt5;
HFONT fnt6;
HFONT fnt7;
HFONT fnt1naa;
HFONT systemfont;

surf *intro_ultimavi;
surf *intro_ultimavi2;


//visibility checking arrays
unsigned char vis[34+2][26+2]; //will be used for pathfind as well!
unsigned char vis_window[34+2][26+2];//if =1 window exists here
unsigned char vis_chair[34+2][26+2];//1=up 2=right 3=down 4=left 0=none
unsigned char vischeck[32][24];//0=objects on this square are not visible, 1=they are
unsigned char visalways[256][1024];//bit array, if =1 force visibility
unsigned char vis_bed[34+2][26+2];//1=horizontal bed 2=vertical bed
unsigned char vis_slime[34+2][26+2];//1=slime

unsigned char x5option;

long mixer_volume;
unsigned char mixer_mute;
DWORD mixerid;
MIXERLINE mxl;
MIXERCONTROL mxc;
MIXERLINECONTROLS mxlc;
tMIXERCONTROLDETAILS mixer;
tMIXERCONTROLDETAILS_UNSIGNED mixervolume[2];
MIXERCONTROLDETAILS_BOOLEAN mcb={0} ;
MIXERCONTROLDETAILS mcd;

unsigned short customportrait[3584];
unsigned char customportrait_upload;

unsigned char clientframe;

long ctpx;
long ctpy; //client: screen offset
long ctpx2;
long ctpy2; //client: selected partymember offset
unsigned short cobjtype; //client: object type (selected partymember)

unsigned char pw_encrypt;
unsigned char setup_message;
unsigned char cur_type;
unsigned char userkey;
unsigned char userspell;
unsigned char userspellbook;
unsigned short portlast;
unsigned char deadglobalmessage;
unsigned char keyframe_backup;
unsigned short oceantiles;
unsigned rivertiles;
unsigned char britlens;
unsigned char garglens;
unsigned char xray;
unsigned char peer;
unsigned char tmap;
float wizardeyetimeleft;
float ktar_display;//seconds to display keyboard targetting numbers for
unsigned char talkprev;
unsigned char directionalmove_only;
unsigned char tremor;
unsigned long clientplayerid;//only valid if not 0
txt *namelast;
unsigned char localvoicemessage_return;
float autoscroll;
HKEY tempregkey;
float sysban;
unsigned long namecolour;
HFONT lastfont;
unsigned char voicechat_listeningplayers;
unsigned short voicechat_listeningplayerx[256];
unsigned short voicechat_listeningplayery[256];
unsigned char voicechat_listeningplayeri;
unsigned char voicechat_listeningplayervolume[256];
//VOICE CHAT 1.0+
unsigned char voicechat_permission;
unsigned char voicechat_permissionrequested;
char voicechat_recording;
float voicechat_mciwait;
char voicechat_devicedeallocrequired;
float voicechat_recordtime;
//portrait look
float portraitlook_wait;
unsigned short portraitlook_portrait;
unsigned char portraitlook_equip;
unsigned short portraitlook_type[8];
unsigned char portraitlook_plusbonus[8];
txt* portraitlook_name;
unsigned long portraitlook_namecolour;
//cloud info
unsigned char noclouds;
long cloudidealnum;
unsigned char cloudloaded;
surf *cloudimg[16][4];
unsigned char cloudactive[32];
unsigned char cloudtype[32];
long cloudx[32],cloudy[32];
long cloudheight[32];
unsigned char firstclouds;
//not4sale info
unsigned short not4sale_flags[8];//one index per partymember

DWORD dwMilliSeconds;
UINT wDeviceID;
DWORD dwReturn;
MCI_OPEN_PARMS mciOpenParms;
MCI_RECORD_PARMS mciRecordParms;
MCI_SAVE_PARMS mciSaveParms;
MCI_PLAY_PARMS mciPlayParms;



surf *vs;

unsigned char timelval; //0=full brightness, 15=total darkness

unsigned char endgame; //1=play endgame sequence
float endgame_timer;
unsigned char endgame_message;

long omx;
long omy;
long omb; //used by frame.h
long omx2;
long omy2;

unsigned short vf_mb2_x;
unsigned short vf_mb2_y;

bool U6O_WALKTHRU_REC;
bool U6O_WALKTHRU;

//master volume controls
unsigned char u6ovolume;
unsigned char u6omidivolume;
unsigned char u6omidisetup;
unsigned char u6ovoicevolume;

//wav
unsigned char u6osoundtype_volume[255];
sound *u6osound[255];
unsigned char u6osound_type[255]; //0=combat, etc.
short u6osound_volumechange[255]; //adjust volume of this sound (-255 to 255)
unsigned char u6osound_volume[255]; //0 to 255
bool wavinfo_loaded;

//midi
INFOPORT u6omidi_infoport;
CMidiMusic *u6omidi;
txt *u6omidi_filename[256];
unsigned char u6omidi_volume[255];
bool midiinfo_loaded;

//System information and advance function declarations
HWND hWnd;
HWND hWnd2;
HWND hWnd3;

// rrr
HWND hWnd4;

bool windowchange;

bool gotfocus; //TRUE if program is selected
long scrx; long scry; //size of the window required by the program


bool smallwindow; //use a 512x384 window
bool dxrefresh;
bool nodisplay;
bool isit;
bool host_minimize;

bool setupfail;

/* luteijn: old stuff no longer referenced anywhere:
   txt* u6oip; //host ip address
   HINTERNET u6o_internet; //internet session 
   bool u6o_offline=FALSE;
   */
bool u6o_sound;
tagSIZE tagxy;
HDC taghdc;

FRAME* musickeyboard;

client_settings cltset;
client_settings cltset2;
unsigned char clientsettingsvalid;

unsigned char spellrecall_partymember[8];
unsigned char spellrecall_i[8];

/* luteijn:
 * option_hires never changes during execution, so made it a #define and #ifdef 
 */

unsigned char moonlight;

//wind (local)
char windx2;
char windy2;

//light arrays
unsigned long ls_off,ls_off_add,ls2_off,ls2_off_add;
unsigned char *ls2_p;
unsigned short lval[16][65536];
unsigned char ls[1024*768];
unsigned char ls_moon1[1024*768];
unsigned char ls_moon2[1024*768];
unsigned char ls_moon3[1024*768];
unsigned char ls_moon4[1024*768];
unsigned char ls3[32*3][32*3];
unsigned char ls3b[32*3][32*3];
unsigned char ls5[32*5][32*5];
unsigned char ls5b[32*5][32*5];
unsigned char ls7[32*7][32*7];
unsigned char ls9[32*9][32*9];
unsigned char ls11[32*11][32*11];
unsigned char ls13[32*13][32*13];

unsigned short intro_starx[1024];
unsigned short intro_stary[1024];

long textdisplayi; //ideal line to finish on (can be changed by user)
unsigned char textdisplayupdate;


float client_globalwait = 10;

txt* tshiftnum;
unsigned char shiftnum_show;


unsigned long idlst[1024];
txt *idlst_name[1024];
//surf *idlst_port[1024];
unsigned char idlst_volume[1024];
unsigned long idlst_namecolour[1024];

long idlstn;

bool qkstf_update;

unsigned char inprec; //input receiving
unsigned char inprec_global; //input receiving global


unsigned char nonvis[32][24];
short osn;
short osx[1024]; //y
short osy[1024]; //x (centre)
unsigned short osi[1024]; //index of U6OID
unsigned char oshpmp[1024]; //statusbar
short osvol[1024];

file *u6o_error_file;

unsigned long keyframe;
unsigned long keyframe2;
unsigned long keyframe3;
unsigned long keyframe15;
unsigned long keyframe31; //animation/palette index (0-7)
unsigned long refreshcount; //incremented every refresh

surf* ps;
surf *ps2;
surf *ps3;
surf *ps4;
surf *ps5;
surf *ps6;
surf *ps7;
surf *ps8;
surf *ps640400;
surf *ps320200;

// rrr
surf* psnew1;
surf* psnew1b;

// r999
surf* panelsurf[PANEL_MAX];
int panelx[PANEL_MAX], panely[PANEL_MAX], panelx2[PANEL_MAX], panely2[PANEL_MAX];
long panelmx[PANEL_MAX], panelmy[PANEL_MAX];
double panelscalex[PANEL_MAX], panelscaley[PANEL_MAX];

FRAME panelnew[PANEL_MAX];
int panelcount = 0;
int panelsideui, panelactionbar1, panelactionbar2, panelactiontalkbar1, panelactiontalkbar2, panelminimap;


// r999 new
int hituipaneli, hituiwidgeti;
surf* uipanelimgsurf[UI_PANEL_IMG_MAX];
surf* uiwidgetimgsurf[UI_PANELWIDGET_IMG_MAX][UI_WIDGETSTATE_IMG_MAX];
float uiscalex, uiscaley;
int uiscaling = 0;
int uihover = 1;
surf *uihoveractionbuttonsurf, *uihoveractiontalkbuttonsurf;


// r444
surf *minimaptilesurf, *minimaptilesurf1, *minimaptilesurf2;
surf* minimap_surf_new;
unsigned int minimaptype;
unsigned int minimaptypemax = 0;
int minimapnewx, minimapnewy;
int minimapdeltax, minimaptilexstart, minimaptilexend;
int minimapdeltay, minimaptileystart, minimaptileyend;
int minimapplayerx, minimapplayery;
float minimapstepsize;

// r666
//surf* actionbarsurf[ACTIONBAR_MAX];
//int actionbarmax=1;
//int actionbarx[ACTIONBAR_MAX], actionbary[ACTIONBAR_MAX], actionbuttonsizex, actionbuttonsizey;
//int actionbuttonx[ACTIONBAR_MAX][ACTIONBUTTON_MAX], actionbuttony[ACTIONBAR_MAX][ACTIONBUTTON_MAX];
int actionpending = 0;
int actionlast = 0;
int actionreset = 0;

//surf* actiontalksurf[ACTIONTALKBAR_MAX];
//int actiontalkmax=1;
//int actiontalkx[ACTIONTALKBAR_MAX], actiontalky[ACTIONTALKBAR_MAX], actiontalkbuttonsizex, actiontalkbuttonsizey;
//int actiontalkbuttonx[ACTIONTALKBAR_MAX][ACTIONTALKBUTTON_MAX], actiontalkbuttony[ACTIONTALKBAR_MAX][ACTIONTALKBUTTON_MAX];
int actiontalkfilltext = 0;

//surf* actionbuttonsurf[ACTIONBUTTON_MAX][10];
/*
surf* actiondropsetbuttonsurf;
surf* actiondropupbuttonsurf;
surf* actiondropdownbuttonsurf;
surf* actiondropleftbuttonsurf;
surf* actiondroprightbuttonsurf;
surf* actionfoodfullbuttonsurf;
*/

// r777
int itemtoinv = 0;
int checkstatusmessage = 0;
int foodstatus = 0;
int setdroplocation = 0;
int droplocation = 1;
int droploc = 0;
int playeronscreenxn1, playeronscreenyn1;

// r222
//surf* pspartyorg;
//surf* pspartytemp;
//surf* pspartynew;
//surf* party_surf[8];

unsigned int partyframenewmax = 1;
FRAME* party_frame_new[1];


static unsigned int partyresxn1 = 128;
static unsigned int partyresyn1 = 128;

unsigned int partyresxo = 256;
unsigned int partyresyo = 256;

unsigned int partyresxz = partyresxo;
unsigned int partyresyz = partyresyo;
int newmodestatus = 1;
double partyresscale = 1.0;

// s333
int combatinfo = 0;
int hittarget = 0;
int checkpendingstatusmessage = 0;
int statusmessagependingprevlen = 0;
int statusmessagechanged = 0;
int txtcolprev = 0;
int objtypen1;
player combatinfoplayerprev;
int combatinfoplayerprevinit = 0;
int objremovedn1;
double combatinfoplayerprevett = 0.0f;
int resultinfon1 = 0;
int sfxonscreenn1 = 0;

// s222
int soundn1 = 2;
int combatsoundn1 = 2;
int playstatusmessagesound;

// s444
int enhancen1 = 2;
int showworldmapn1 = 0;
surf* worldmapsurfn1[5];
int worldmapindexn1 = 1;
int updateworldmapn1 = 1;
int showenhancehostn1 = 1;
HFONT txtfntoldn1;

// s555
int updatepartyframen1 = 0;


surf* bt32;
surf* bt16;
surf* spr84[16];
surf* spr8[8];
surf* sfx8;
surf* bt8[8];
sound* SNDhit;




//sf compatible information


sfxtype sfx[256]; //local sf

unsigned char update[8]; //set to 1 if party frame needs updating
unsigned char action; //active action key

unsigned char sprpi[65536];//index in pal emu spr8

//portraits 2.0 info
surf* portrait[65536];//regular size portraits
surf* portrait_doublesize[65536];//double size
surf* portrait_halfsize[65536];//half size
unsigned char portrait_loaded[65536];//TRUE=PORTRAIT LOADED
unsigned char portrait_requested[65536];//TRUE=portrait has been requested
txt *portrait_request_txt;
surf* PORTRAIT_UNAVAILABLE;

unsigned char wateri[32]; //used for hybrid sea tiles

object* mobj; //selected (mouse) object *local
object *moon1;

player *CLIENTplayer;
player *tplay; //client temp. player struct

short stolenitemwarningn;
unsigned short stolenitemwarningx[256];
unsigned short stolenitemwarningy[256];
unsigned short stolenitemwarningtype[256];

unsigned char client_spellwait[8];

txt* inpmess;
inpmess_index *inpmess_mostrecent;
long inpmess_selected;
surf *surf_tremor,*surf_tremor2,*surf_tremorcirclemask;
surf *intro_startup;

unsigned short walkthru_x;
unsigned short walkthru_y;
file *walkthru_fh;
unsigned long walkthru_pos_skip;
unsigned long walkthru_pos;
JOYINFOEX joy;


txt *u6o_user_name; 
txt *u6o_user_password; 
txt *u6o_name; 
unsigned char u6o_malefemale;//0=male, 1=female
unsigned short u6o_portrait;
unsigned char u6o_type;
unsigned char u6o_vq[28];//0=a,1=b
unsigned char u6o_createcharacter;//obselete!
txt *u6o_new_user_password;

surf *spellicon[256];

unsigned long midi_song; //handle to current midi
short midi_loaded;
short midi_background;
short midi_foreground;
float midi_foreground_wait;

txt *con_txt[8];

surf* party_spellbook_surf[8];
FRAME* party_spellbook_frame[8];
unsigned short spell[8][256]; //number of times spell can be cast (+1)
unsigned char spellbook_page[8]; //current page in spellbook
unsigned char spellbook_flags[8]; //1=left dog-ear 2=right dog-ear


surf* party_surf[8];
FRAME* party_frame[8];

surf* minimap_surf;
FRAME* minimap_frame;
surf* minimap_b;
surf* treasuremap;
surf* tmap_surf;
FRAME* tmap_frame;
surf* tmap_markers;
surf* tmap_marker;

object* tobj_i[8][16+1+4+1];
object* tobj_e[8][8];

unsigned char intro; //part of intro to process (0=ingame!)

unsigned char cltset2_restored;

file* messagelog;

FRAME* fs;
surf* status8;
surf* darrow;
surf* uarrow;
surf* horizon;
surf* horizon2;

surf* cave;
surf* sun;
surf* sun2;
surf* mini_1;
surf* tmini_1;
surf* mini_2;
surf* mini_3;
surf* u6ob;
surf* dhno;
surf* not4sale;
surf* not4salemask;
surf* converse_arrows;
surf* spellbook;
surf* spellbookmini;
surf* statusmessage_arrowup;
surf *spellcircle[8];
surf *statusbar_r255;
surf *statusbar_r128;
surf *statusbar_b255;
surf *statusbar_b128;
surf *statusbar_g255;
surf *statusbar_g128;
surf* statusbar;
surf *dogearr;
surf *dogearl;
surf *spellbookline;
surf *intro_gypsy;
surf *intro_gypsy2;
surf *intro_vial;
surf *intro_svial;
surf *intro_bigvial;
surf *intro_hpl0;
surf *intro_hpl2;
surf *intro_hpl3;
surf *intro_hps0;
surf *intro_hps2;
surf *intro_hps3;
surf *intro_hpr0;
surf *intro_hpr2;
surf *intro_hpr3;
surf *intro_arml;
surf *intro_armr;
surf *intro_s64;
surf *intro_s64b;
surf *intro_s128;
surf *intro_ab;
surf *intro_aba;
surf *intro_abb;
surf *intro_caravan;
surf *intro_flask;
surf *intro_ccsave1;
surf *intro_ccsave2;
surf *intro_ccsave3;
surf *intro_tacinfo;
surf *intro_newchar;
surf *intro_newchar2;
surf *intro_x;
surf *intro_back;
surf *intro_next;
surf *intro_ifield;
surf *blr[4];
surf *glr[4];
surf *instantclickx;
surf *instantclickok;
surf *endgame_image[10];
surf *spellbookmini2;
surf *voicechat_voicebar;
surf *voicechat_voice1;
surf *voicechat_voiceof;
surf *inventoryadd_icon;
surf *horsemask;
surf *horsemask2;
surf *horsemaskdress;
surf *horsemask2dress;
surf *horsemaskdressb;
surf *horsemask2dressb;
surf* port_temp;
surf* vm_volumem;
surf* vm_volmmute;
surf* vm_voltab2m;
surf* volcontrol_background;
surf* volcontrol_surf;
surf* volcontrol_tab1;
surf* volcontrol_tab2;
surf* volcontrol_tab3;
surf* viewnpc;
surf* viewnpc2;
surf* viewnpc_temp;
surf* viewnpc2_temp;

FRAME* statusmessage_viewnpc;
FRAME* statusmessage_viewprev;
FRAME* voicechat_frame;
FRAME* volcontrol;
FRAME* qkstf;
FRM_IMAGE *con_frm_img;
FRAME* con_frm;
FRM_TXT* inpft;
FRM_INPUT *inpf2;
txt *inpftxt;
FRAME* inpf;
FRAME* vf;

HCURSOR cur1;
HCURSOR cur2;
HCURSOR cur3;
HCURSOR cur4;
HCURSOR cur5;
HCURSOR cur6;
HCURSOR cur7;
HCURSOR cur8;
HCURSOR cur9;
HCURSOR cur_u;
HCURSOR cur_ru;
HCURSOR cur_r;
HCURSOR cur_rd;
HCURSOR cur_d;
HCURSOR cur_ld;
HCURSOR cur_l;
HCURSOR cur_lu;
#endif /* CLIENT */

#ifdef HOST
/* variables originally from data_host.h */
unsigned char save_buffer[SAVESLOTLAST+1];
txt *save_username[SAVESLOTLAST+1];
txt *save_password[SAVESLOTLAST+1];
txt *save_name[SAVESLOTLAST+1];
unsigned long save_exp[SAVESLOTLAST+1];
unsigned long save_bytes[SAVESLOTLAST+1];
unsigned char save_dump;
unsigned char login_dead_callback;

//1=mover drops blood, 0=mover doesn't drop blood
unsigned char mover_blood[1024];
unsigned short mover_body[1024];

unsigned char sfbuffersend;
float sfbufferwait;

unsigned char cast_spell;
unsigned char staff_cast_spell;

long CASTSPELL_SPELLTYPE;

txt *admins[ADMINSMAX];

txt *motd;
txt *inbritannia;
long inbritannia_totalplayers;

unsigned long U6ONEWID; //never 0

housesav_info housesav[256];

//orb of the moons destinations
unsigned short orbx[5][5];
unsigned short orby[5][5];

//wind direction (global)
char windx;
char windy;
unsigned char windnew;

unsigned char party_ok[8];
unsigned short save_version;
unsigned char encryptcode[65536];

unsigned char itemused;
bool spellattcrt;

unsigned short autosetup;
bool autosetup_next;
unsigned short autosetup_counter;

bool autospell;

unsigned long treagent[8];
unsigned long tspell[256]; //set to 1 if spell exists, then added to if reagents exist

//jump/call-back flag for formatting username and charactername
unsigned char format_usernames;

unsigned long objr_x;
unsigned long objr_y;

schedule_i schedule[256][32];
schedule_i schedule2[1024][32];

unsigned long npci[256];
unsigned char *npcinf;
npcbin_i *npcbin;

unsigned long sfi[256][256]; //pointer to first sf
sfxtype sf[65536]; //clear every cycle
unsigned long sf_playerid[65536];//only valid for text messages where top bit of port is set
long sfn; //last sf
unsigned long sfx_playerid[256];

unsigned char btu6[1024][1024];
unsigned char btu60[256][256];

unsigned short npcobj[256];


objentry ol[32768]; //modify to allow for more complex base maps
long oln;

unsigned char btflags[256];

object objb[524228]; //host object buffer ~10M
long objb_last; //last objb
unsigned long objb_free[524228]; //free objb index
long objb_free_last; //last free objb index





//doorclose: automatically locks unlocked doors after 2 hours if unused and noone has passed through
long doorclose_last;
float doorclose_wait[1024];
object *doorclose_obj[1024],*doorclose_obj2[1024];
unsigned short doorclose_oldtype[1024],doorclose_oldtype2[1024];

//leverchange
long leverchange_last;
float leverchange_wait[1024];
object *leverchange_obj[1024];
unsigned short leverchange_oldtype[1024];


unsigned char showmoongates;
unsigned short moongatex[8],moongatey[8];
object *moongate[8][2];
unsigned char moonphase;

object *nuggetsfix;

//volatile links (vlnk)
void *vlnkb_lnk[65536]; //vlnk buffer ~0.25M lnk (dest) object
unsigned long vlnkb_off[65536]; //vlnk buffer ~0.25M lnk offset
void *vlnkb_lnks[65536]; //vlnk buffer ~0.25M lnk source object
long vlnkb_last; //last
unsigned long vlnkb_free[65536]; //free vlnkb index
long vlnkb_free_last; //last free vlnkb index
//functions
//VLNK_new(_lnks,_lnk,_off)
//VLNK_remove(_lnk) NULL any link pointing to this object, then remove vlnk
//VLNK_sremove(_lnk) remove any vlnk belonging to _lnk


object* portcullis[256][16];
object* lever[256][16];
object* efield[256][16]; //electric field
object* eswitch[256][16]; //electric switch


crtenum_struct crtenum[1073];
object* crtenum_pathok_castok[1024]; short crtenum_pathok_castok_i;
object* crtenum_pathok[1024]; short crtenum_pathok_i;
object* crtenum_castok[1024]; short crtenum_castok_i;



//resurrect info
object *resu[65536]; //object
object *resu_body[65536]; //dead body object
unsigned short resu_body_type[65536];//object type of dead body (used to remake lost body and check if correct)
float resu_wait[65536]; //time until object automatically resurrected
player *resu_player[65536]; //player object belongs to
unsigned char resu_partymember[65536]; //party member index
unsigned short resu_x[65536],resu_y[65536];
long nresu;


//wizard eye(s) list: locations of wizard eyes currently displayed in britannia

unsigned char wizardeyesi;
unsigned short wizardeyesx[256];
unsigned short wizardeyesy[256];
unsigned char wizardeyesi2;
unsigned char wizardeyesadded;
object *wizardeyesobj;

unsigned char economy_setup[1024][4];
long economy_limit[1024][4];
long economy_value[1024][4];
long economy_change[1024][4];


//when a monster is "killed" it is added to the respawn list
//a delay is also set
//
//respawn info
object *respawn[1200];
unsigned short respawn_delay[1200]; //number of seconds till creature will respawn
//*note: creature will not respawn if player is too near (eg. 8 squares or less)
long respawn_last;




player* playerlist[1024]; //supports up to 1024 online players
long playerlist_last;


unsigned char chunks[1024][8][8]; //"c:\ultima6\chunk."

unsigned char u6wci2[8][8][16][8][3];
unsigned short u6wci[128][128];
unsigned char u60ci2[32][16][3];
unsigned short u60ci[32][32];

object *od[1024][2048]; //8M, pointers to all data, NULL pointers are common





object *oul[65536]; //list of active objects to update (size: 1M)
long ouln;
object *f_oul[65536]; //list of fixed objects to update
long f_ouln;

txt *tname;
txt *tusername;
txt *tuserpassword;
txt *tnewuserpassword;
unsigned char tcustomportrait_upload;
unsigned short tcustomportrait[3584];
unsigned long tnamecolour;
unsigned char tmale_female;
unsigned short tport;
unsigned short ttype;
unsigned char tcreatecharacter;

//keyb target
char ktar_x[768];
char ktar_y[768];
float ktar_xydis[768];

/*
MOVERNEW: FLAGS
flags can be set globally, in which case they need to be reset when finished
or locally by being passed as flags
local flags and OR-ed with global flags
*/
unsigned long MOVERNEW_GLOBALFLAGS;
unsigned char MOVERNEW_ERROR;//valid when movernew(...) returns 0
//1=FAILED (object is not a mover)
//2=IGNORED (object is not the primary part of a mover)

#ifndef CLIENT
//STUBS TO SUPPORT EXTERNAL LINKING (DATA NOT ACTUALLY USED!)
unsigned char u6omidisetup;
HWND hWnd;
HWND hWnd2;
HWND hWnd3;
HWND hWnd4;
RECT desktop_rect;
bool smallwindow;
unsigned char u6ovolume;
#endif /* !CLIENT */

unsigned long revive_infiniteloopexit_i;
unsigned long revive_infiniteloopexit_i2;
unsigned short revive_infiniteloopexit_i3;

unsigned long mycount;
long newschedule2;
object* newll;
unsigned long ol_tag;
unsigned long ol_tag_prev;
#endif /* HOST */

/* */

#endif /* GLOBALS_H */
