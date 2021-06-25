#include "data_client.h" 

void data_client_init(void) {
  midiout_handle=NULL;
  midiout_setup=FALSE;
  U6O_DISABLEMUSIC=FALSE;
  U6O_DISABLEJOYSTICK=FALSE;
  fonts_added=FALSE;

  intro_timer=0;

  fnt1=NULL;
  fnt2=NULL;
  fnt3=NULL;
  fnt4=NULL;
  fnt5=NULL;
  fnt6=NULL;
  fnt7=NULL;
  fnt1naa=NULL;
  systemfont=NULL;

  intro_ultimavi=NULL;
  intro_ultimavi2=NULL;

  x5option=0;

  mixer_volume=255;
  mixer_mute=0;
  //mcb = {0}; // can't init here
  customportrait_upload=0;

  clientframe=0;

  ctpx=0;ctpy=0; //client: screen offset
  ctpx2=0;ctpy2=0; //client: selected partymember offset
  cobjtype=0; //client: object type (selected partymember)

  pw_encrypt=PW_ENCRYPTION;
  setup_message=FALSE;
  cur_type=1;
  userkey=0;
  userspell=0;
  userspellbook=0;
  portlast=0;
  deadglobalmessage=0;
  keyframe_backup=0;
  oceantiles=0,rivertiles=0;
  britlens=0,garglens=0;
  xray=0;
  wizardeyetimeleft=0.0f;
  ktar_display=0;//seconds to display keyboard targetting numbers for
  talkprev=0;
  directionalmove_only=0;
  tremor=0;
  clientplayerid=0;//only valid if not 0
  namelast=txtnew();
  localvoicemessage_return=0;
  autoscroll=0.0f;
  tempregkey;
  sysban=0.0f;
  namecolour=0xFFFFFF;
  voicechat_listeningplayers=FALSE;
  voicechat_listeningplayeri=0;
  voicechat_listeningplayervolume[256];
  //VOICE CHAT 1.0+
  voicechat_permission=FALSE;
  voicechat_permissionrequested=FALSE;
  voicechat_recording=0;
  voicechat_mciwait=0.0f;
  voicechat_devicedeallocrequired=0;
  voicechat_recordtime=0.0f;
  //portrait look
  portraitlook_wait=0.0f;
  portraitlook_portrait=0;
  portraitlook_equip=0;
  portraitlook_type[8];
  portraitlook_plusbonus[8];
  portraitlook_name=NULL;
  portraitlook_namecolour=0xFFFFFF;
  //cloud info
  noclouds=FALSE;
  cloudidealnum=8;
  cloudloaded=0;
  firstclouds=1;

  wDeviceID=0;
  endgame=0; //1=play endgame sequence
  endgame_timer=0;
  endgame_message=0;

  omx=0;omy=0;omb=0; //used by frame.h
  omx2=0;omy2=0;

  vf_mb2_x=0xFFFF;vf_mb2_y=0xFFFF;

  U6O_WALKTHRU_REC=FALSE;
  U6O_WALKTHRU=TRUE;

  //master volume controls
  u6ovolume=128;
  u6omidivolume=128;
  u6omidisetup=0;
  u6ovoicevolume=128;

  //wav
  wavinfo_loaded=FALSE;

  //midi
  u6omidi=NULL;
  midiinfo_loaded=FALSE;

  windowchange=FALSE;

  gotfocus=TRUE; //TRUE if program is selected
  scrx=512*2; scry=384*2; //size of the window required by the program

  smallwindow=FALSE; //use a 512x384 window
  dxrefresh=FALSE;
  nodisplay=FALSE;
  isit=FALSE;
  host_minimize=!U6O_DEBUG;

  setupfail=FALSE;
  u6o_sound=TRUE;

  //tagxy=0; //can't init this struct
  taghdc=0;

  musickeyboard=NULL;

  clientsettingsvalid=FALSE;

  moonlight=0;

  //wind (local) // luteijn: maybe we should make this global, or at least shared within an area of influence? TODO
  windx2=0;
  windy2=0;

  textdisplayi=-1; //ideal line to finish on (can be changed by user)
  textdisplayupdate=0;
  ls_off=0;
  ls_off_add=0;
  ls2_off=0;
  ls2_off_add=0;

  ls2_p=0;

  tshiftnum=NULL;
  shiftnum_show=0;

  idlstn=-1;

  qkstf_update=TRUE;

  inprec=0; //input receiving
  inprec_global=0; //input receiving global


  osn=-1;
  u6o_error_file=NULL;

  keyframe=0;keyframe2=0;keyframe3=0;keyframe15=0;keyframe31=0; //animation/palette index (0-7)
  action=0; //active action key


  portrait_request_txt=NULL;
  PORTRAIT_UNAVAILABLE=NULL;

  mobj=NULL; //selected (mouse) object *local
  moon1=NULL;

  CLIENTplayer=NULL;
  tplay=NULL; //client temp. player struct

  stolenitemwarningn=-1;

  inpmess=txtnew();
  inpmess_selected=-1;

  walkthru_x=0;
  walkthru_y=0;
  walkthru_pos_skip=0;
  walkthru_pos=0;

  u6o_user_name=txtnew(); 
  u6o_user_password=txtnew(); 
  u6o_name=txtnew(); 

  u6o_malefemale=0;//0=male, 1=female
  u6o_portrait=2;
  u6o_type=0;
  u6o_createcharacter=0;//obselete!
  u6o_new_user_password=txtnew();

  midi_song=0; //handle to current midi
  midi_loaded=-1;
  midi_background=0;
  midi_foreground=-1;
  midi_foreground_wait=0.0f;
  intro=200; //part of intro to process (0=ingame!)
  cltset2_restored=0;
}


/* end */
