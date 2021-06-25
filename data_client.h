#ifndef DATA_CLIENT_H
#define DATA_CLIENT_H

#include "define_client.h" /* keys etc. */
#include "mytxt.h" /* txt* etc */
#include "data_both.h" /* schedule* etc. */
#include "frame.h" /* FRAME* etc. */
#include "dmusic.h" /* INFOPORT etc. */
/* 
 * #include <winsock2.h> 
 * #include <windows.h> 
 * for HINTERNET u6o_internet; //internet session 
 * which seems unused (older code??
 */

/* structure definitions */
struct client_settings{
  short party_frame_offset_x[8],party_frame_offset_y[8];
  short party_spellbook_frame_offset_x[8],party_spellbook_frame_offset_y[8];
  short musickeyboard_offset_x,musickeyboard_offset_y;
  short inpf_offset_x,inpf_offset_y;
  short con_frm_offset_x,con_frm_offset_y;
  short volcontrol_offset_x,volcontrol_offset_y;
  short qkstf_offset_x,qkstf_offset_y;
  unsigned char u6ovolume;
  unsigned char u6omidivolume;
  //add more settings here
  unsigned char u6ovoicevolume;
  unsigned char spellrecall_partymember[8];
  unsigned char spellrecall_i[8];
  short minimap_offset_x,minimap_offset_y;
  short tmap_offset_x,tmap_offset_y;
};
struct inpmess_index{
  txt *t;
  inpmess_index *next;
};


/* external global variables defined in globals.h*/
extern FRAME *pmf;
extern RECT desktop_rect;
extern HMIDIOUT midiout_handle;
extern unsigned char midiout_setup;
extern unsigned char U6O_DISABLEMUSIC;
extern unsigned char U6O_DISABLEJOYSTICK;
extern unsigned char JDISABLED;
extern unsigned char fonts_added;
extern float intro_timer;
extern unsigned short U6OK_DEFAULT[128][2];
extern unsigned short U6OK[128][2];
extern unsigned short U6OK_TEMP[128][2];
//1 INSTANTCLICK ON,?
extern unsigned char U6OK_DEFAULT_FLAGS[128];
extern unsigned char U6OK_FLAGS[128];
extern unsigned char U6OK_TEMP_FLAGS[128];

extern HFONT fnt1;
extern HFONT fnt2;
extern HFONT fnt3;
extern HFONT fnt4;
extern HFONT fnt5;
extern HFONT fnt6;
extern HFONT fnt7;
extern HFONT fnt1naa;
extern HFONT systemfont;

extern surf *intro_ultimavi;
extern surf *intro_ultimavi2;


//visibility checking arrays
extern unsigned char vis[34+2][26+2]; //will be used for pathfind as well!
extern unsigned char vis_window[34+2][26+2];//if =1 window exists here
extern unsigned char vis_chair[34+2][26+2];//1=up 2=right 3=down 4=left 0=none
extern unsigned char vischeck[32][24];//0=objects on this square are not visible, 1=they are
extern unsigned char visalways[256][1024];//bit array, if =1 force visibility
extern unsigned char vis_bed[34+2][26+2];//1=horizontal bed 2=vertical bed
extern unsigned char vis_slime[34+2][26+2];//1=slime

extern unsigned char x5option;

extern long mixer_volume;
extern unsigned char mixer_mute;
extern DWORD mixerid;
extern MIXERLINE mxl;
extern MIXERCONTROL mxc;
extern MIXERLINECONTROLS mxlc;
extern tMIXERCONTROLDETAILS mixer;
extern tMIXERCONTROLDETAILS_UNSIGNED mixervolume[2];
extern MIXERCONTROLDETAILS_BOOLEAN mcb ;
extern MIXERCONTROLDETAILS mcd;

extern unsigned short customportrait[3584];
extern unsigned char customportrait_upload;

extern unsigned char clientframe;

extern long ctpx;
extern long ctpy; //client: screen offset
extern long ctpx2;
extern long ctpy2; //client: selected partymember offset
extern unsigned short cobjtype; //client: object type (selected partymember)

extern unsigned char pw_encrypt;
extern unsigned char setup_message;
extern unsigned char cur_type;
extern unsigned char userkey;
extern unsigned char userspell;
extern unsigned char userspellbook;
extern unsigned short portlast;
extern unsigned char deadglobalmessage;
extern unsigned char keyframe_backup;
extern unsigned short oceantiles;
extern unsigned rivertiles;
extern unsigned char britlens;
extern unsigned char garglens;
extern unsigned char xray;
extern unsigned char peer;
extern unsigned char tmap;
extern float wizardeyetimeleft;
extern float ktar_display;//seconds to display keyboard targetting numbers for
extern unsigned char talkprev;
extern unsigned char directionalmove_only;
extern unsigned char tremor;
extern unsigned long clientplayerid;//only valid if not 0
extern txt *namelast;
extern unsigned char localvoicemessage_return;
extern float autoscroll;
extern HKEY tempregkey;
extern float sysban;
extern unsigned long namecolour;
extern HFONT lastfont;
extern unsigned char voicechat_listeningplayers;
extern unsigned short voicechat_listeningplayerx[256];
extern unsigned short voicechat_listeningplayery[256];
extern unsigned char voicechat_listeningplayeri;
extern unsigned char voicechat_listeningplayervolume[256];
//VOICE CHAT 1.0+
extern unsigned char voicechat_permission;
extern unsigned char voicechat_permissionrequested;
extern char voicechat_recording;
extern float voicechat_mciwait;
extern char voicechat_devicedeallocrequired;
extern float voicechat_recordtime;
//portrait look
extern float portraitlook_wait;
extern unsigned short portraitlook_portrait;
extern unsigned char portraitlook_equip;
extern unsigned short portraitlook_type[8];
extern unsigned char portraitlook_plusbonus[8];
extern txt* portraitlook_name;
extern unsigned long portraitlook_namecolour;
//cloud info
extern unsigned char noclouds;
extern long cloudidealnum;
extern unsigned char cloudloaded;
extern surf *cloudimg[16][4];
extern unsigned char cloudactive[32];
extern unsigned char cloudtype[32];
extern long cloudx[32],cloudy[32];
extern long cloudheight[32];
extern unsigned char firstclouds;
//not4sale info
extern unsigned short not4sale_flags[8];//one index per partymember

extern DWORD dwMilliSeconds;
extern UINT wDeviceID;
extern DWORD dwReturn;
extern MCI_OPEN_PARMS mciOpenParms;
extern MCI_RECORD_PARMS mciRecordParms;
extern MCI_SAVE_PARMS mciSaveParms;
extern MCI_PLAY_PARMS mciPlayParms;



extern surf *vs;

extern unsigned char timelval; //0=full brightness, 15=total darkness

extern unsigned char endgame; //1=play endgame sequence
extern float endgame_timer;
extern unsigned char endgame_message;

extern long omx;
extern long omy;
extern long omb; //used by frame.h
extern long omx2;
extern long omy2;
extern long omx3;
extern long omy3;

extern unsigned short vf_mb2_x;
extern unsigned short vf_mb2_y;

extern bool U6O_WALKTHRU_REC;
extern bool U6O_WALKTHRU;

//master volume controls
extern unsigned char u6ovolume;
extern unsigned char u6omidivolume;
extern unsigned char u6omidisetup;
extern unsigned char u6ovoicevolume;

//wav
extern unsigned char u6osoundtype_volume[255];
extern sound *u6osound[255];
extern unsigned char u6osound_type[255]; //0=combat, etc.
extern short u6osound_volumechange[255]; //adjust volume of this sound (-255 to 255)
extern unsigned char u6osound_volume[255]; //0 to 255
extern bool wavinfo_loaded;

//midi
extern INFOPORT u6omidi_infoport;
extern CMidiMusic *u6omidi;
extern txt *u6omidi_filename[256];
extern unsigned char u6omidi_volume[255];
extern bool midiinfo_loaded;

//System information and advance function declarations
extern HWND hWnd;
extern HWND hWnd2;
extern HWND hWnd3;
extern HWND hWnd4;

extern bool windowchange;

extern bool gotfocus; //TRUE if program is selected
extern long scrx;
extern long scry; //size of the window required by the program


extern bool smallwindow; //use a 512x384 window
extern bool dxrefresh;
extern bool nodisplay;
extern bool isit;
extern bool host_minimize;

extern bool setupfail;

/* luteijn: old stuff no longer referenced anywhere:
   txt* u6oip; //host ip address
   HINTERNET u6o_internet; //internet session 
   bool u6o_offline=FALSE;
   */
extern bool u6o_sound;
extern tagSIZE tagxy;
extern HDC taghdc;

extern FRAME* musickeyboard;

extern client_settings cltset;
extern client_settings cltset2;
extern unsigned char clientsettingsvalid;

extern unsigned char spellrecall_partymember[8];
extern unsigned char spellrecall_i[8];

extern unsigned char moonlight;

//wind (local)
extern char windx2;
extern char windy2;

//light arrays
extern unsigned long ls_off,ls_off_add,ls2_off,ls2_off_add;
extern unsigned char *ls2_p;
extern unsigned short lval[16][65536];
extern unsigned char ls[1024*768];
extern unsigned char ls_moon1[1024*768];
extern unsigned char ls_moon2[1024*768];
extern unsigned char ls_moon3[1024*768];
extern unsigned char ls_moon4[1024*768];
extern unsigned char ls3[32*3][32*3];
extern unsigned char ls3b[32*3][32*3];
extern unsigned char ls5[32*5][32*5];
extern unsigned char ls5b[32*5][32*5];
extern unsigned char ls7[32*7][32*7];
extern unsigned char ls9[32*9][32*9];
extern unsigned char ls11[32*11][32*11];
extern unsigned char ls13[32*13][32*13];

extern unsigned short intro_starx[1024];
extern unsigned short intro_stary[1024];

extern long textdisplayi; //ideal line to finish on (can be changed by user)
extern unsigned char textdisplayupdate;


extern float client_globalwait;

extern txt* tshiftnum;
extern unsigned char shiftnum_show;


extern unsigned long idlst[1024];
extern txt *idlst_name[1024];
extern unsigned char idlst_volume[1024];
extern unsigned long idlst_namecolour[1024];

extern long idlstn;

extern bool qkstf_update;

extern unsigned char inprec; //input receiving
extern unsigned char inprec_global; //input receiving global


extern unsigned char nonvis[32][24];
extern short osn;
extern short osx[1024]; //y
extern short osy[1024]; //x (centre)
extern unsigned short osi[1024]; //index of U6OID
extern unsigned char oshpmp[1024]; //statusbar
extern short osvol[1024];

extern file *u6o_error_file;

extern unsigned long keyframe;
extern unsigned long keyframe2;
extern unsigned long keyframe3;
extern unsigned long keyframe15;
extern unsigned long keyframe31; //animation/palette index (0-7)
extern unsigned long refreshcount; //incremented every refresh

extern surf* ps;
extern surf *ps2;
extern surf *ps3;
extern surf *ps4;
extern surf *ps5;
extern surf *ps6;
extern surf *ps7;
extern surf *ps8;
extern surf *ps640400;
extern surf *ps320200;

// rrr
extern surf* psnew1;
extern surf* psnew1b;
//extern surf* party_surf[8];
extern FRAME* party_frame_new[];
extern unsigned int partyframenewmax;

// r222
extern unsigned int partyresxo;
extern  unsigned int partyresyo;
extern unsigned int partyresxz;
extern unsigned int partyresyz;
extern int newmodestatus;
extern double partyresscale;
extern unsigned int resxn1m;
extern unsigned int resyn1m;
extern unsigned int respn1m;
extern unsigned int resxn1w;
extern unsigned int resyn1w;


// r999
extern surf* panelsurf[PANEL_MAX];
extern int panelx[PANEL_MAX], panely[PANEL_MAX], panelx2[PANEL_MAX], panely2[PANEL_MAX];
extern long panelmx[PANEL_MAX], panelmy[PANEL_MAX];
extern double panelscalex[PANEL_MAX], panelscaley[PANEL_MAX];
extern FRAME panelnew[PANEL_MAX];
extern int panelcount;
extern int panelsideui, panelactionbar1, panelactionbar2, panelactiontalkbar1, panelactiontalkbar2, panelminimap;

// r444
extern surf *minimaptilesurf, *minimaptilesurf1, *minimaptilesurf2;
extern surf* minimap_surf_new;
extern unsigned int minimaptype, minimaptypemax;
extern int minimapnewx, minimapnewy;
extern int minimapdeltax, minimaptilexstart, minimaptilexend;
extern int minimapdeltay, minimaptileystart, minimaptileyend;
extern int minimapplayerx, minimapplayery;
extern float minimapstepsize;

// r666
//extern surf* actionbarsurf[ACTIONBAR_MAX];
//extern int actionbarx[ACTIONBAR_MAX], actionbary[ACTIONBAR_MAX], actionbuttonsizex, actionbuttonsizey;
//extern int actionbuttonx[ACTIONBAR_MAX][ACTIONBUTTON_MAX], actionbuttony[ACTIONBAR_MAX][ACTIONBUTTON_MAX];
extern int actionpending;
extern int actionlast;
extern int actionreset;

//extern surf* actiontalksurf[ACTIONTALKBAR_MAX];
//extern int actiontalkx[ACTIONTALKBAR_MAX], actiontalky[ACTIONTALKBAR_MAX], actiontalkbuttonsizex, actiontalkbuttonsizey;
//extern int actiontalkbuttonx[ACTIONTALKBAR_MAX][ACTIONTALKBUTTON_MAX], actiontalkbuttony[ACTIONTALKBAR_MAX][ACTIONTALKBUTTON_MAX];
extern int actiontalkfilltext;

//extern surf* actionbuttonsurf[ACTIONBUTTON_MAX][10];

// r999
extern surf* uipanelsurf[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelx[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanely[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelsizex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelsizey[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern float uipanelscalex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern float uipanelscaley[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelscaling[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelhitenable[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelusedefaultstatedata[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];

extern int uipaneli[UI_PANEL_MAX][UI_PANELWIDGET_MAX];
extern int uipanelcount;
extern int uipanelwidgetcount[UI_PANEL_MAX];

extern int uipanelsidebar, uipanelminimap, uipanelworldmap, uipanelworldmapbar, uipanelpartymemberparent, uipanelactionbarparent, uipanelactiontalkbarparent;
extern int uipanelactionbar1, uipanelactionbar2, uipaneloptionbar1, uipanelactiontalkbar1, uipanelactiontalkbar2, uipanelactiontalkbar3;
extern int uipanelpartymember0;
extern int uipaneloptioninfo;

extern int hituipaneli, hituiwidgeti;
extern surf* uipanelimgsurf[UI_PANEL_IMG_MAX];
extern surf* uiwidgetimgsurf[UI_PANELWIDGET_IMG_MAX][UI_WIDGETSTATE_IMG_MAX];
extern float uiscalex, uiscaley;
extern int uiscaling;
extern int uihover;
extern surf *uihoveractionbuttonsurf, *uihoveractiontalkbuttonsurf;



// s222
extern int soundn1;
extern int combatsoundn1;

// s333 get type of object at coordinate
extern player combatinfoplayerprev;
extern int combatinfoplayerprevinit;
extern int resultinfon1;

// s444
extern surf* worldmapsurfn1[5];
extern int worldmapindexn1;
extern int enhancen1;
extern int showworldmapn1;
extern int updateworldmapn1;
extern int showenhancehostn1;
extern HFONT txtfntoldn1;

// s555
extern int updatepartyframen1;



extern surf* bt32;
extern surf* bt16;
extern surf* spr84[16];
extern surf* spr8[8];
extern surf* sfx8;
extern surf* bt8[8];
extern sound* SNDhit;

//sf compatible information
extern sfxtype sfx[256]; //local sf

extern unsigned char update[8]; //set to 1 if party frame needs updating
extern unsigned char action; //active action key

extern unsigned char sprpi[65536];//index in pal emu spr8

//portraits 2.0 info
extern surf* portrait[65536];//regular size portraits
extern surf* portrait_doublesize[65536];//double size
extern surf* portrait_halfsize[65536];//half size
extern unsigned char portrait_loaded[65536];//TRUE=PORTRAIT LOADED
extern unsigned char portrait_requested[65536];//TRUE=portrait has been requested
extern txt *portrait_request_txt;
extern surf* PORTRAIT_UNAVAILABLE;

extern unsigned char wateri[32]; //used for hybrid sea tiles

extern object* mobj; //selected (mouse) object *local
extern object *moon1;

extern player *CLIENTplayer;
extern player *tplay; //client temp. player struct

extern short stolenitemwarningn;
extern unsigned short stolenitemwarningx[256];
extern unsigned short stolenitemwarningy[256];
extern unsigned short stolenitemwarningtype[256];

extern unsigned char client_spellwait[8];

extern txt* inpmess;
extern inpmess_index *inpmess_mostrecent;
extern long inpmess_selected;
extern surf *surf_tremor,*surf_tremor2,*surf_tremorcirclemask;
extern surf *intro_startup;

extern unsigned short walkthru_x;
extern unsigned short walkthru_y;
extern file *walkthru_fh;
extern unsigned long walkthru_pos_skip;
extern unsigned long walkthru_pos;
extern JOYINFOEX joy;


extern txt *u6o_user_name; 
extern txt *u6o_user_password; 
extern txt *u6o_name; 
extern unsigned char u6o_malefemale;//0=male, 1=female
extern unsigned short u6o_portrait;
extern unsigned char u6o_type;
extern unsigned char u6o_vq[28];//0=a,1=b
extern unsigned char u6o_createcharacter;//obselete!
extern txt *u6o_new_user_password;

extern surf *spellicon[256];

extern unsigned long midi_song; //handle to current midi
extern short midi_loaded;
extern short midi_background;
extern short midi_foreground;
extern float midi_foreground_wait;

extern txt *con_txt[8];

extern surf* party_spellbook_surf[8];
extern FRAME* party_spellbook_frame[8];
extern unsigned short spell[8][256]; //number of times spell can be cast (+1)
extern unsigned char spellbook_page[8]; //current page in spellbook
extern unsigned char spellbook_flags[8]; //1=left dog-ear 2=right dog-ear


extern surf* party_surf[8];
extern FRAME* party_frame[8];

extern surf* minimap_surf;
extern FRAME* minimap_frame;
extern surf* minimap_b;
extern surf* treasuremap;
extern surf* tmap_surf;
extern FRAME* tmap_frame;
extern surf* tmap_markers;
extern surf* tmap_marker;

extern object* tobj_i[8][16+1+4+1];
extern object* tobj_e[8][8];

extern unsigned char intro; //part of intro to process (0=ingame!)

extern unsigned char cltset2_restored;

extern file* messagelog;

extern FRAME* fs;
extern surf* status8;
extern surf* darrow;
extern surf* uarrow;
extern surf* horizon;
extern surf* horizon2;

extern surf* cave;
extern surf* sun;
extern surf* sun2;
extern surf* mini_1;
extern surf* tmini_1;
extern surf* mini_2;
extern surf* mini_3;
extern surf* u6ob;
extern surf* dhno;
extern surf* not4sale;
extern surf* not4salemask;
extern surf* converse_arrows;
extern surf* spellbook;
extern surf* spellbookmini;
extern surf* statusmessage_arrowup;
extern surf *spellcircle[8];
extern surf *statusbar_r255;
extern surf *statusbar_r128;
extern surf *statusbar_b255;
extern surf *statusbar_b128;
extern surf *statusbar_g255;
extern surf *statusbar_g128;
extern surf* statusbar;
extern surf *dogearr;
extern surf *dogearl;
extern surf *spellbookline;
extern surf *intro_gypsy;
extern surf *intro_gypsy2;
extern surf *intro_vial;
extern surf *intro_svial;
extern surf *intro_bigvial;
extern surf *intro_hpl0;
extern surf *intro_hpl2;
extern surf *intro_hpl3;
extern surf *intro_hps0;
extern surf *intro_hps2;
extern surf *intro_hps3;
extern surf *intro_hpr0;
extern surf *intro_hpr2;
extern surf *intro_hpr3;
extern surf *intro_arml;
extern surf *intro_armr;
extern surf *intro_s64;
extern surf *intro_s64b;
extern surf *intro_s128;
extern surf *intro_ab;
extern surf *intro_aba;
extern surf *intro_abb;
extern surf *intro_caravan;
extern surf *intro_flask;
extern surf *intro_ccsave1;
extern surf *intro_ccsave2;
extern surf *intro_ccsave3;
extern surf *intro_tacinfo;
extern surf *intro_newchar;
extern surf *intro_newchar2;
extern surf *intro_x;
extern surf *intro_back;
extern surf *intro_next;
extern surf *intro_ifield;
extern surf *blr[4];
extern surf *glr[4];
extern surf *instantclickx;
extern surf *instantclickok;
extern surf *endgame_image[10];
extern surf *spellbookmini2;
extern surf *voicechat_voicebar;
extern surf *voicechat_voice1;
extern surf *voicechat_voiceof;
extern surf *inventoryadd_icon;
extern surf *horsemask;
extern surf *horsemask2;
extern surf *horsemaskdress;
extern surf *horsemask2dress;
extern surf *horsemaskdressb;
extern surf *horsemask2dressb;
extern surf* port_temp;
extern surf* vm_volumem;
extern surf* vm_volmmute;
extern surf* vm_voltab2m;
extern surf* volcontrol_background;
extern surf* volcontrol_surf;
extern surf* volcontrol_tab1;
extern surf* volcontrol_tab2;
extern surf* volcontrol_tab3;
extern surf* viewnpc;
extern surf* viewnpc2;
extern surf* viewnpc_temp;
extern surf* viewnpc2_temp;

extern FRAME* statusmessage_viewnpc;
extern FRAME* statusmessage_viewprev;
extern FRAME* voicechat_frame;
extern FRAME* volcontrol;
extern FRAME* qkstf;
extern FRM_IMAGE *con_frm_img;
extern FRAME* con_frm;
extern FRM_TXT* inpft;
extern FRM_INPUT *inpf2;
extern txt *inpftxt;
extern FRAME* inpf;
extern FRAME* vf;

extern HCURSOR cur1;
extern HCURSOR cur2;
extern HCURSOR cur3;
extern HCURSOR cur4;
extern HCURSOR cur5;
extern HCURSOR cur6;
extern HCURSOR cur7;
extern HCURSOR cur8;
extern HCURSOR cur9;
extern HCURSOR cur_u;
extern HCURSOR cur_ru;
extern HCURSOR cur_r;
extern HCURSOR cur_rd;
extern HCURSOR cur_d;
extern HCURSOR cur_ld;
extern HCURSOR cur_l;
extern HCURSOR cur_lu;

/* function prototypes */
void data_client_init(void);


#endif /* DATA_CLIENT_H */
