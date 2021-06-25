#include "data_both.h"

void data_both_init(void) {
  currentbitstream=NULL;
  oldtime=0;newtime=0;
  et=0.0f;
  ett=0.0f;
  file *log2=NULL;
  U6O_SIGNATURE=85+54*256+79*65536+50*16777216;
  NEThost=0;

  socketclientlast=0;
  endprogram=FALSE; //TRUE if program is ending

  memcpy(szWindowClass,"ULTIMATE\0",9);
  memcpy(window_name,"Ultima VI Online\0",16);
  memcpy(szTitle,"ULTIMATE\0",9);

  mx=0;my=0;mb=0; //mouse input values (mb returns the button status)
  mbclick=0; //mbclick: recieved mouse_down message for mouse button
  mbheld=0; //set if the physical mouse button is being held down and has not been released
  mb_release=0;

  SCRLOG_FILEONLY=FALSE;

  dv=0;
  dv2=0;

  void* NETplayer=NULL;

  HOST_portrait_next=32768;

  //storm cloak arrays
  stormcloak_last=0;
  stormcloak_last2=0;

  u6orevive=0;
  u6opi=0; //u6o program index (0=unknown location)
  u6opi_old=0;
  u6opl=0; //u6o program line
  file *u6orevive_fh=NULL;

  objfixed_next=1;
  tobjfixed_next=1;

  btime=10.0f;
  btime_last=10.0f;
  btime2=10; //ultra precise universal britannian clock!
  bday=1; //day is a value between 1 and 7

  tpx=0;
  tpy=0;
  tpl=0; //used to store each client position (temp)

  exitrequest=FALSE;
  exitrequest_noconfirm=FALSE;

  mess1=txtnew();
  mess_UPDATEps=NULL;
  mess_SF=NULL;
  u6o_namecolour=0;

  lastsecond=-1;
  framerate=0;
  framecount=0; //framerate frames/sec
}

