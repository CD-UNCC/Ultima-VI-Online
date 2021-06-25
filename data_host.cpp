#include "data_host.h" /*SAVESLOTLAST etc */

void data_host_init(void) {
  save_dump=0;
  save_version=0;
  login_dead_callback=0;
  sfbuffersend=0;
  sfbufferwait=0.0f;
  cast_spell=0;
  staff_cast_spell=0;
  motd=NULL;
  inbritannia=NULL;
  inbritannia_totalplayers=0;
  U6ONEWID=1; //never 0
  //wind direction (global)
  windx=0;
  windy=0;
  spellattcrt=FALSE;
  autosetup=0;
  autosetup_next=FALSE;
  autosetup_counter=0;
  autospell=FALSE;
  format_usernames=0;
  objr_x=0; 
  objr_y=0;
  sfn=-1; //last sf
  oln=0;
  objb_last=-1; //last objb
  objb_free_last=-1; //last free objb index
  doorclose_last=-1;
  leverchange_last=-1;
  showmoongates=0;
  moonphase=255;
  object *nuggetsfix=NULL;
  vlnkb_last=-1; //last
  vlnkb_free_last=-1; //last free vlnkb index
  nresu=-1;
  wizardeyesi2=0;
  wizardeyesadded=0;
  wizardeyesobj=NULL;
  respawn_last=-1;
  playerlist_last=-1;
  ouln=-1;
  f_ouln=-1;
  tname=NULL;
  tusername=NULL;
  tuserpassword=NULL;
  tnewuserpassword=NULL;
  MOVERNEW_GLOBALFLAGS=0;

#ifndef CLIENT
  //STUBS TO SUPPORT EXTERNAL LINKING (DATA NOT ACTUALLY USED!)
  u6omidisetup=NULL;
  hWnd=NULL;
  hWnd2=NULL;
  hWnd3=NULL;
  hWnd4=NULL;
  smallwindow=NULL;
  u6ovolume=NULL;
#endif

  revive_infiniteloopexit_i=0;
  revive_infiniteloopexit_i2=0xFFFFFFFF;
  revive_infiniteloopexit_i3=0;
  mycount=0;
  newschedule2=-1;
  newll=NULL;
  ol_tag=0;
  ol_tag_prev=0;

}

/* */
