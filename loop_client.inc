if (wheel_move){
  if (wheel_move>0){
    wheel_move--;
    i=0xD8;
    keyon[i]=TRUE;
    key[i]=TRUE;
    key_gotrelease[i]=FALSE;
  }else{
    wheel_move++;
    i=0xD9;
    keyon[i]=TRUE;
    key[i]=TRUE;
    key_gotrelease[i]=FALSE;
  }
}





if (playinstrument){

  txtset(t,"?"); t->d2[0]=25; //play MIDI keys
  txtset(t2,"?");
  //multiple keys can point to a single sound
  for (i=0;i<=255;i++){
    if (keyhit(i)){
      if (t->l<9){
        if (midikeyboard2[i]!=255){
          if (midikeyboard2_keyon[i]){ //if release not received for key, release now
            txtset(t3,"??"); t3->d2[0]=26; //stop MIDI keys
            t3->d2[1]=midikeyboard2[i];
            NET_send(NETplayer,NULL,t3);
          }
          t2->d2[0]=midikeyboard2[i]; txtadd(t,t2);
          midikeyboard2_keyon[i]=1;
        }//!=255
      }
    }//keyhit
  }//i
  if (t->l>1){
    NET_send(NETplayer,NULL,t);
  }

  txtset(t,"?"); t->d2[0]=26; //stop MIDI keys
  txtset(t2,"?");
  for (i=0;i<=255;i++){
    if (keyon[i]==NULL){
      if (midikeyboard2_keyon[i]){
        t2->d2[0]=midikeyboard2[i]; txtadd(t,t2);
        midikeyboard2_keyon[i]=0;
      }}}
  if (t->l>1){
    NET_send(NETplayer,NULL,t);
  }

}//playinstrument

if (setupfail) {
    DestroyWindow( hWnd );
}

if (exitrequest){
  exitrequest=FALSE;
  if (exitrequest_noconfirm){
    DestroyWindow( hWnd );
  }else{
    static txt *exitrequest_t=txtnew();
    static long i;
    for (i=1;i<=7;i++){
      if (tplay->party[i]){
        txtset (exitrequest_t,"Dupre");
        if (txtsame(((npc*)tplay->party[i]->more)->name,exitrequest_t)) MessageBox(NULL,"Dupre cannot stay in your party, if you leave his items will be lost!","Ultima 6 Online",MB_OK);
        txtset (exitrequest_t,"Shamino");
        if (txtsame(((npc*)tplay->party[i]->more)->name,exitrequest_t)) MessageBox(NULL,"Shamino cannot stay in your party, if you leave his items will be lost!","Ultima 6 Online",MB_OK);
        txtset (exitrequest_t,"Iolo");
        if (txtsame(((npc*)tplay->party[i]->more)->name,exitrequest_t)) MessageBox(NULL,"Iolo cannot stay in your party, if you leave his items will be lost!","Ultima 6 Online",MB_OK);
        txtset (exitrequest_t,"Beh Lem");
        if (txtsame(((npc*)tplay->party[i]->more)->name,exitrequest_t)) MessageBox(NULL,"Beh Lem cannot stay in your party, if you leave his items will be lost!","Ultima 6 Online",MB_OK);
        txtset (exitrequest_t,"Sentri");
        if (txtsame(((npc*)tplay->party[i]->more)->name,exitrequest_t)) MessageBox(NULL,"Sentri cannot stay in your party, if you leave his items will be lost!","Ultima 6 Online",MB_OK);
        txtset (exitrequest_t,"Sherry");
        if (txtsame(((npc*)tplay->party[i]->more)->name,exitrequest_t)) MessageBox(NULL,"Sherry cannot stay in your party, if you leave her items will be lost!","Ultima 6 Online",MB_OK);
        txtset (exitrequest_t,"Seggallion");
        if (txtsame(((npc*)tplay->party[i]->more)->name,exitrequest_t)) MessageBox(NULL,"Seggallion cannot stay in your party, if you leave his items will be lost!","Ultima 6 Online",MB_OK);

      }
    }
    if (IDYES==MessageBox(NULL,"Leave Britannia?","Ultima 6 Online",MB_YESNO)){
      //send "leaving Britannia message"
      //note: Sent directly skipping NET_send(...) routine to be sure it is sent before exit.
      txtsetshort(exitrequest_t,1);
      txtaddchar(exitrequest_t,244); 
      send(socketclient[0],(const char*)&exitrequest_t->d2[0],exitrequest_t->l,0);
      DestroyWindow( hWnd );
    }
  }
}
//end exit request



//check for midi keys that need to be cleared
i3=0;
for (i2=0;i2<=15;i2++){
  for (i=0;i<=255;i++){

    if (midikeystack[i2][i]){
      i3=1;
      if (midikeywait[i2][i]<=(ett-8.0f)){
        midiOutShortMsg(midiout_handle,0x00000080+i*256+i2);

        midikeystack[i2][i]=0;
      }}}
}

if ((i3==0)&&midipause){
  if (playinstrument==0){
    midipause=0;

    if (U6O_DISABLEMUSIC==FALSE){
      if (u6omidivolume) u6omidi->Play();
    }

  }
}


clientframe=!clientframe;
//if (!NEThost) clientframe=1;
clientframe=1;

f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
  if (xray) xray--;
  if (peer) peer--;
  if (tmap) tmap--;
}
ktar_display-=et; if (ktar_display<0.0f) ktar_display=0.0f;
wizardeyetimeleft-=et; if (wizardeyetimeleft<0.0f) wizardeyetimeleft=0.0f;

isit=TRUE;

if (!NEThost) {
  if (lastsecond==-1) lastsecond=ett; x=ett; //detect frame rate
  if (x!=lastsecond){
    lastsecond=x; framerate=framecount; framecount=0;
  }
  framecount++;
}

//fix mouse 512x384
if (smallwindow){
  if ((mx!=omx2)||(my!=omy2)){
    mx*=2; my*=2;
    omx2=mx; omy2=my;
  }
}

omb=FRAME_mb; //find old button value using FRAME_mb (which now contains old value)
FRAME_mb=mb; //backup physical mouse value
if ((mb&1)==0) FRAME_mbl[0]=FALSE; //disabled mouse locks
if ((mb&2)==0) FRAME_mbl[1]=FALSE;
if (FRAME_mbl[0]==TRUE){ //mask physical values if locked
  if (mb&1) mb=mb-1;
}
if (FRAME_mbl[1]==TRUE){
  if (mb&2) mb=mb-2;
}
pn=firstpanel;
checkpanel:
if (pn!=NULL){ //frame available
  if ((pn->graphic!=NULL)&&(pn->size_x==0)&&(pn->size_y==0)){
    x3=pn->offset_x; y3=pn->offset_y;
    x4=x3+pn->graphic->d.dwWidth; y4=y3+pn->graphic->d.dwHeight;
  }else{
    x3=pn->offset_x; y3=pn->offset_y;
    x4=x3+pn->size_x; y4=y3+pn->size_y;
  }
  if (mx>=x3){ if (my>=y3){ if (mx<x4){ if (my<y4){ //mouse in within rectangle
    if (pn->graphic_mask!=NULL){ //graphic_mask check
      if (point(pn->graphic_mask,mx-pn->offset_x,my-pn->offset_y)==0) goto maskcheckfailed;
    }
    if ((FRAME_mbl[0]==FALSE)&&((mb&1)==1)){
      FRAME_mbl[0]=TRUE; //lock mouse button
      FRAME_mblf[0]=pn;
    }
    if ((FRAME_mbl[1]==FALSE)&&((mb&2)==2)){
      FRAME_mbl[1]=TRUE; //lock mouse button2
      FRAME_mblf[1]=pn;
    }
    if ((FRAME_mbl[0]==TRUE)&&(FRAME_mblf[0]==pn)) mb=mb|1;
    if ((FRAME_mbl[1]==TRUE)&&(FRAME_mblf[1]==pn)) mb=mb|2;

    x2=0; //=1 to select

    FRM_type=(FRM_TYPE*)pn->firstobject;
checkobj:
    if (FRM_type!=NULL){
      x=FRM_type->type;
      if (x==1){ //process: FRM_ONOFF type
        tonoff=(FRM_ONOFF*)FRM_type;
        if (tonoff->value==0){ //rectangle of current button icon
          x3=pn->offset_x+tonoff->offset_x; y3=pn->offset_y+tonoff->offset_y;
          x4=x3+tonoff->graphic_off->d.dwWidth; y4=y3+tonoff->graphic_off->d.dwHeight;
        }
        else{
          x3=pn->offset_x+tonoff->offset_x+tonoff->offset2_x; y3=pn->offset_y+tonoff->offset_y+tonoff->offset2_y;
          x4=x3+tonoff->graphic_on->d.dwWidth; y4=y3+tonoff->graphic_on->d.dwHeight;
        }
        if (mx>=x3){ if (my>=y3){ if (mx<x4){ if (my<y4){ //mouse in within rectangle
          if (tonoff->hold==FALSE){ //button does not need to be held
            if ((omb&1)==0&&(mb&1)==1){ //button has just been pressed
              if (tonoff->value==0) soundplay(tonoff->sound_on);
              if (tonoff->value==1) soundplay(tonoff->sound_off);
              tonoff->value++; if(tonoff->value==2) tonoff->value=0;
              x2=1;
            }
          }
          else{ //button must be held
            if ((omb&1)==0&&(mb&1)==1){ //button has just been pressed
              soundplay(tonoff->sound_on);
              tonoff->value=1;
              x2=1; //update
              ONOFF_hold=tonoff; //set external disable pointer
            }
          }
          goto skipbaseframe; //skip further processing
        }}}} //mouse within button
      } //process finish: FRM_ONOFF type
      if (x==4){ //process: FRM_INPUT type
        tinp=(FRM_INPUT*)FRM_type;
        x3=pn->offset_x+tinp->offset_x; y3=pn->offset_y+tinp->offset_y;
        x4=x3+tinp->graphic->d.dwWidth; y4=y3+tinp->graphic->d.dwHeight;
        if (mx>=x3){ if (my>=y3){ if (mx<x4){ if (my<y4){ //mouse in within rectangle
          if ((omb&1)==0&&(mb&1)==1){ //button has just been pressed
            tinp->enterpressed=FALSE;
            GETINPUT_setup(tinp->input,&tinp->enterpressed,tinp->length_limit);
            x2=1; //update
          }
          goto skipbaseframe; //skip further processing
        }}}} //mouse within graphic/area
      } //process finish: FRM_INPUT type
      FRM_type=(FRM_TYPE*)FRM_type->next; goto checkobj;
    }

    pn->mouse_over=TRUE;
    pn->mouse_x=mx-pn->offset_x;
    pn->mouse_y=my-pn->offset_y;


    if (mbclick&1) pn->mouse_click|=1;
    if (mb&1){



      pn->mouse_pressed=TRUE;
    }

    if (pn==vf){
      if (mb&2){
        vf_mb2_x=mx; vf_mb2_y=my;
      }
    }

    if (pn->move==TRUE){
      if ((omb&1)==0&&(mb&1)==1){ //button has been pressed
        if (firstpanel!=pn) soundplay(pn->sound_select);
        x2=1; //update
      }
      if ((omb&2)==0&&(mb&2)==2){ //button2 has been pressed
        drg=pn; //set external drag pointer
        x2=1; //update
        FRAME_drg_begin=FALSE;
      }
    }
skipbaseframe:

    if (pn->move==TRUE){

      if (pn->no_focus==FALSE){
        if (x2==1&&firstpanel!=pn){ //update: bring frame to top
          pn2=(FRAME*)pn->previous;
          pn3=(FRAME*)pn->next;
          pn2->next=pn3;
          if (pn3!=NULL) pn3->previous=pn2;
          pn->previous=NULL;
          pn->next=firstpanel;
          firstpanel->previous=pn;
          firstpanel=pn;
        }
      } //no focus
    }

    mb=0; //clear all button information
    goto checkdone; //skip further frame processing
maskcheckfailed:;
  }}}}
  pn=(FRAME*)pn->next; goto checkpanel; //next frame pointer
}
checkdone:
//external functions, results of button presses
if ((ONOFF_hold!=NULL)&&((FRAME_mb&1)==0)){
  soundplay(ONOFF_hold->sound_off);
  ONOFF_hold->value=0;
  ONOFF_hold=NULL;
}
if ((FRAME_mb&2)==0) drg=NULL;

if (drg!=NULL){ //drag panel


  if (drg->move==TRUE){
    x=mx-omx; y=my-omy;
    if ((x!=0)||(y!=0)){
      if (FRAME_drg_begin==FALSE) soundplay(drg->sound_move);
      FRAME_drg_begin=TRUE;
      drg->offset_x+=x;
      drg->offset_y+=y;
    }
  }
}
if ((FRAME_mbl[0]==FALSE)&&((mb&1)==1)){
  FRAME_mbl[0]=TRUE; //lock mouse button for user
  FRAME_mblf[0]=NULL;
}
if ((FRAME_mbl[1]==FALSE)&&((mb&2)==2)){
  FRAME_mbl[1]=TRUE; //lock mouse button2  for user
  FRAME_mblf[1]=NULL;
}
if ((FRAME_mbl[0]==TRUE)&&(FRAME_mblf[0]==NULL)) mb=mb|1;
if ((FRAME_mbl[1]==TRUE)&&(FRAME_mblf[1]==NULL)) mb=mb|2;

//CLIENT2HOST KEEPALIVE MESSAGE
f=4.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
  txtset(t,"?"); t->d2[0]=251; NET_send(NETplayer,NULL,t);
}


if (intro){
  f=intro_timer;


  //refresh backdrop
  if (intro!=100){

    cls(ps,(2<<11)+(27<<5)+18); 
    x2=(4<<11)+(54<<5)+31;
    x3=(3<<11)+(46<<5)+27;
    x4=(2<<11)+(34<<5)+21;
    for (x=0;x<=255;x++){
      ps->o2[intro_stary[x]*1024+intro_starx[x]]=x2;
      ps->o2[intro_stary[x]*1024+intro_starx[x]+1]=x3;
      ps->o2[intro_stary[x]*1024+intro_starx[x]-1]=x3;
      ps->o2[intro_stary[x]*1024+intro_starx[x]+1024]=x3;
      ps->o2[intro_stary[x]*1024+intro_starx[x]-1024]=x3;
      ps->o2[intro_stary[x]*1024+intro_starx[x]-1025]=x4;
      ps->o2[intro_stary[x]*1024+intro_starx[x]-1023]=x4;
      ps->o2[intro_stary[x]*1024+intro_starx[x]+1023]=x4;
      ps->o2[intro_stary[x]*1024+intro_starx[x]+1025]=x4;
    }
    img0(ps,1024-656,768-369,intro_startup);
  }else{
    cls(ps,0);
  }


  static unsigned char intro_setup=0;

  //exit request?
  if (fs->mouse_click){
    if ((mx>(1024-21-4))&&(my<(21+4))){
      exitrequest=TRUE;
      fs->mouse_click=NULL;
    }
  }


  //GYPSY
  static unsigned char vquan[8];
  static unsigned short vcol[8];
  static unsigned short vx[8];//original x offset
  static char vaction=-1;
  static unsigned char vaction_done=0;
  static unsigned char vques[28];
  static char vtext;


  if (intro==100){
    if (intro_setup==0){ intro_setup=1;
    f=0;
    for (x=0;x<=7;x++) vquan[x]=7;
    ZeroMemory(&vques,28);
    vcol[0]=(0>>3<<11)+(0>>16<<5)+(252>>3);
    vcol[1]=(252>>3<<11)+(252>>2<<5)+(84>>3);
    vcol[2]=(252>>3<<11)+(0>>2<<5)+(0>>3);
    vcol[3]=(0>>3<<11)+(252>>2<<5)+(0>>3);
    vcol[4]=(252>>3<<11)+(96>>2<<5)+(0>>3);
    vcol[5]=(168>>3<<11)+(0>>2<<5)+(168>>3);
    vcol[6]=(252>>3<<11)+(252>>2<<5)+(252>>3);
    vcol[7]=(72>>3<<11)+(72>>2<<5)+(72>>3);
    vx[0]=51;
    vx[1]=72;
    vx[2]=93;
    vx[3]=114;
    vx[4]=187;
    vx[5]=208;
    vx[6]=229;
    vx[7]=250;

    img0_0key(intro_vial,21);
    img0_0key(intro_hpl0,21);
    img0_0key(intro_hpl2,21);
    img0_0key(intro_hpl3,21);
    img0_0key(intro_hpr0,21);
    img0_0key(intro_hpr2,21);
    img0_0key(intro_hpr3,21);


    vtext=-2;
    }//intro_setup==0


    f+=et/1.0f; //action timer

    cls(ps320200,0);
    img(ps320200,0,0,intro_gypsy); //backdrop!

    y5=1;
    if (vaction!=-1){


      f2=f;
      if (f2>1.0f){
        if (vaction_done==0) {vaction_done=1; vquan[vaction]--;}
        f2=2.0f-f;
      }


      y2=66-f2*34.0f;


      if (vaction<=3){
        x2=(132-vx[vaction])*f2+vx[vaction];
        img0(ps320200,x2-12,y2+12,intro_arml);
        y5=0; img(ps320200,0,97,intro_gypsy2); //backdrop!
        cls(intro_hps0,vcol[vaction]); img0(intro_hps0,0,0,intro_hpl0); img0_0key(intro_hps0,32);
        cls(intro_hps2,vcol[vaction]); img0(intro_hps2,0,0,intro_hpl2); img0_0key(intro_hps2,32);
        cls(intro_hps3,vcol[vaction]); img0(intro_hps3,0,0,intro_hpl3); img0_0key(intro_hps3,32);


        if (f2<=0.5f) img0(ps320200,x2-2,y2,intro_hps0);
        if ((f2>0.5)&&(f2<=0.9)) img0(ps320200,x2-16,y2,intro_hps2);
        if (f2>0.9f) img0(ps320200,x2-16,y2,intro_hps3);
      }else{
        x2=(132+32+8-vx[vaction])*f2+vx[vaction];
        img0(ps320200,x2-8,y2+12,intro_armr);
        y5=0; img(ps320200,0,97,intro_gypsy2); //backdrop!
        cls(intro_hps0,vcol[vaction]); img0(intro_hps0,0,0,intro_hpr0); img0_0key(intro_hps0,32);
        cls(intro_hps2,vcol[vaction]); img0(intro_hps2,0,0,intro_hpr2); img0_0key(intro_hps2,32);
        cls(intro_hps3,vcol[vaction]); img0(intro_hps3,0,0,intro_hpr3); img0_0key(intro_hps3,32);
        if (f2<=0.5f) img0(ps320200,x2-2,y2,intro_hps0);
        if ((f2>0.5)&&(f2<=0.9)) img0(ps320200,x2-8,y2,intro_hps2);
        if (f2>0.9f) img0(ps320200,x2-10,y2,intro_hps3);
      }









      if (f>2) {f=0;  vaction=-1; }
    }


    if (y5) img(ps320200,0,97,intro_gypsy2); //backdrop!



    for (x=0;x<=7;x++){
      if (vaction!=x){
        cls(intro_s64,vcol[x]);
        cls(intro_s128,0);
        img(intro_s128,0,41-(float)vquan[x]*4.3f,intro_s64);
        img0(intro_s128,0,0,intro_vial);
        img(intro_svial,0,0,intro_s128);
        img0_0key(intro_svial,32*4);
        img0_0key(intro_svial,32);
        img0(ps320200,vx[x],66,intro_svial);
      }
    }


    //draw liquid in vial

    x2=0; //height
    for (x=0;x<=7;x++){
      x2+=(7-vquan[x]);
    }
    x2=x2+(x2>>1); //*1.5


    for (y=103;y>(103-x2);y--){
      for (x=148;x<=171;x++){
vialmix:
        x3=rnd*8;
        x4=rnd*7;
        if (vquan[x3]>x4) goto vialmix;
        ps320200->o2[y*320+x]=vcol[x3];
      }}


    img0(ps320200,142,56,intro_bigvial);


    if (vtext>=0){
      img(ps320200,279,174,intro_ab);

      x2=vtext; x=-1; x4=0; x3=0; //if (vques[vtext]==1) x3=1;

      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_COMPASSION} //i3,d3
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_VALOR} //i3,s3
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_JUSTICE} //i3,X
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_SACRIFICE} //i3,X
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_HONOR} //i3,X
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_SPIRITUALITY} //i3,a1
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_HUMILITY} //i3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_VALOR} //d3,s3
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_JUSTICE} //d3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_SACRIFICE} //d3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_HONOR} //d3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_SPIRITUALITY} //d3,a1
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_HUMILITY} //d3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_JUSTICE} //s3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_SACRIFICE} //s3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_HONOR} //s3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_SPIRITUALITY} //s3,a1
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_HUMILITY} //s3,X
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_SACRIFICE} //i,d
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_HONOR} //s,d
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_SPIRITUALITY} //X,a1
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_HUMILITY} //OK,X
      x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_HONOR} //s,i
      x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_SPIRITUALITY} //X,OK
      x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_HUMILITY} //a1,X
      x++; if (x==x2) if (x3) {U6O2_HONOR} else {U6O2_SPIRITUALITY} //X,a1
      x++; if (x==x2) if (x3) {U6O2_HONOR} else {U6O2_HUMILITY} //OK,X
      x++; if (x==x2) if (x3) {U6O2_SPIRITUALITY} else {U6O2_HUMILITY} //a1,X
      x5=x4;
      x2=vtext; x=-1; x4=0; x3=1;

      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_COMPASSION} //i3,d3
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_VALOR} //i3,s3
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_JUSTICE} //i3,X
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_SACRIFICE} //i3,X
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_HONOR} //i3,X
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_SPIRITUALITY} //i3,a1
      x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_HUMILITY} //i3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_VALOR} //d3,s3
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_JUSTICE} //d3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_SACRIFICE} //d3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_HONOR} //d3,X
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_SPIRITUALITY} //d3,a1
      x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_HUMILITY} //d3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_JUSTICE} //s3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_SACRIFICE} //s3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_HONOR} //s3,X
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_SPIRITUALITY} //s3,a1
      x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_HUMILITY} //s3,X
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_SACRIFICE} //i,d
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_HONOR} //s,d
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_SPIRITUALITY} //X,a1
      x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_HUMILITY} //OK,X
      x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_HONOR} //s,i
      x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_SPIRITUALITY} //X,OK
      x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_HUMILITY} //a1,X
      x++; if (x==x2) if (x3) {U6O2_HONOR} else {U6O2_SPIRITUALITY} //X,a1
      x++; if (x==x2) if (x3) {U6O2_HONOR} else {U6O2_HUMILITY} //OK,X
      x++; if (x==x2) if (x3) {U6O2_SPIRITUALITY} else {U6O2_HUMILITY} //a1,X
      if ((x4<=2)||(x4==6)) {x4=2; goto gotx4;}
      if (x4!=7) {x4=1; goto gotx4;}
      x4=0;
gotx4:
      if ((x5<=2)||(x5==6)) {x5=2; goto gotx5;}
      if (x5!=7) {x5=1; goto gotx5;}
      x5=0;
gotx5:
      x=ett*2.0f; x&=1;
      if (x){
        if (x4>=x5) img(ps320200,279,174,intro_aba);
      }else{
        if (x5>=x4) img(ps320200,279,174,intro_abb);
      }
    }

    img(ps640400,ps320200); img(ps,192,184,ps640400);


    txtset(t4,"");
    switch(vtext) 
    {
    case 0:
      txtset(t,"Entrusted to deliver an uncounted purse of gold, thou dost meet a poor");
      txtset(t2,"beggar. Dost thou:");
      txtset(t3,"A) Deliver the gold knowing the Trust in thee was well-placed; or");
      txtset(t4,"B) Show Compassion, giving the Beggar a coin, knowing it won't be missed?");
      break; case 1:
      txtset(t,"Thou hast been prohibited by thy absent Lord from joining thy friends in a");
      txtset(t2,"close pitched battle. Dost thou:");
      txtset(t3,"A) Refrain, so thou may Honestly claim obedience; or");
      txtset(t4,"B) Show Valor, and aid thy comrades, knowing thou may deny it later?");
      break; case 2:
      txtset(t,"A merchant owes thy friend money, now long past due. Thou dost see the same");
      txtset(t2,"merchant drop a purse of gold. Dost thou:");
      txtset(t3,"A) Honestly return the purse intact; or");
      txtset(t4,"B) Justly give thy friend a portion of the gold first?");
      break; case 3:
      txtset(t,"Thee and thy friend are valiant but penniless warriors. Thou both go out to");
      txtset(t2,"slay a mighty dragon. Thy friend thinks he slew it, thee did. When asked, dost thou:");
      txtset(t3,"A) Truthfully claim the gold; or");
      txtset(t4,"B) Allow thy friend the large reward?");
      break; case 4:
      txtset(t,"Thou art sworn to protect thy Lord at any cost, yet thou knowest he hast");
      txtset(t2,"committed a crime. Authorities ask the of the affair, dost thou:");
      txtset(t3,"A) Break thine oath by Honestly speaking; or");
      txtset(t4,"B) Uphold Honor by silently keeping thine oath?");
      break; case 5:
      txtset(t,"Thy friend seeks admittance to thy Spiritual order. Thou art asked to vouch");
      txtset(t2,"for his purity of Spirit, of which thou art unsure. Dost thou:");
      txtset(t3,"A) Honestly express thy doubt; or");
      txtset(t4,"B) Vouch for him, hoping for his Spiritual improvement?");
      break; case 6:
      txtset(t,"Thy Lord mistakenly believes he slew a dragon. Thou hast proof that thy lance");
      txtset(t2,"felled the beast. When asked, dost thou:");
      txtset(t3,"A) Honestly claim the kill and the prize; or");
      txtset(t4,"B) Humbly permit thy Lord his belief?");
      break; case 7:
      txtset(t,"Thou dost manage to disarm thy mortal enemy in a duel. He is at thy mercy.");
      txtset(t2,"Dost thou:");
      txtset(t3,"A) Show Compassion by permitting him to yield; or");
      txtset(t4,"B) Slay him as expected of a Valiant duelist?");
      break; case 8:
      txtset(t,"After 20 years thou hast found the slayer of thy best friends. The villain");
      txtset(t2,"proves to be a man who provides the sole support for a young girl. Dost thou:");
      txtset(t3,"A) Spare him in Compassion for the girl; or");
      txtset(t4,"B) Slay him in the name of Justice?");
      break; case 9:
      txtset(t,"Thee and thy friends have been routed and ordered to retreat. In defiance of");
      txtset(t2,"thy orders, dost thou:");
      txtset(t3,"A) Stop in Compassion to aid a wounded companion; or");
      txtset(t4,"B) Sacrifice thyself to slow the pursuing enemy, so others can escape?");
      break; case 10:
      txtset(t,"Thou art sworn to uphold a Lord who participates in the forbidden torture of");
      txtset(t2,"prisoners. Each night their cries of pain reach thee. Dost thou:");
      txtset(t3,"A) Show Compassion by reporting the deeds; or");
      txtset(t4,"B) Honor thy oath and ignore the deeds?");
      break; case 11:
      txtset(t,"Thou hast been taught to preserve all life as sacred. A man lies fatally"); 
      txtset(t2,"stung by a venomous serpent. He pleads for a merciful death. Dost thou:");
      txtset(t3,"A) Show Compassion and end his pain; or");
      txtset(t4,"B) Heed thy Spiritual beliefs and refuse?");
      break; case 12:
      txtset(t,"As one of the King's Guard, thy Captain has asked that one amongst you visit");
      txtset(t2,"a hospital to cheer the children with tales of thy valiant deeds. Dost thou:");
      txtset(t3,"A) Show thy Compassion and play the braggart; or");
      txtset(t4,"B) Humbly let another go?");
      break; case 13:
      txtset(t,"Thou hast been sent to secure a needed treaty with a distant Lord. Thy host");
      txtset(t2,"is agreeable to the proposal but insults thy country at dinner. Dost thou:");
      txtset(t3,"A) Valiantly bear the slurs; or");
      txtset(t4,"B) Justly rise and demand an apology?");
      break; case 14:
      txtset(t,"A mighty knight accosts thee and demands thy food. Dost thou:");
      txtset(t2,"A) Valiantly refuse and engage the knight; or");
      txtset(t3,"B) Sacrifice thy food unto the hungry knight?");
      break; case 15:
      txtset(t,"During battle thou art ordered to guard thy commmander's empty tent. The");
      txtset(t2,"battle goes poorly and thou dost yearn to aid thy fellows. Dost thou:");
      txtset(t3,"A) Valiantly enter the battle to aid thy companions; or");
      txtset(t4,"B) Honor thy post as guard?");
      break; case 16:
      txtset(t,"A local bully pushes for a fight. Dost thou:");
      txtset(t2,"A) Valiantly trounce the rogue; or");
      txtset(t3,"B) Decline, knowing in thy Spirit that no lasting good will come of it?");
      break; case 17:
      txtset(t,"Although a teacher of music, thou art a skillful wrestler. Thou hast been");
      txtset(t2,"asked to fight in a local championship. Dost thou:");
      txtset(t3,"A) Accept the invitation and Valiantly fight to win; or");
      txtset(t4,"B) Humbly decline knowing thou art sure to win?");
      break; case 18:
      txtset(t,"During a pitched battle, thou dost see a fellow desert his post, endangering");
      txtset(t2,"many. As he flees, he is set upon by several enemies. Dost thou:");
      txtset(t3,"A) Justly let him fight alone; or");
      txtset(t4,"B) Risk Sacrificing thine own life to aid him?");
      break; case 19:
      txtset(t,"Thou hast sworn to do thy Lord's bidding in all. He covets a piece of land");
      txtset(t2,"and orders the owner removed. Dost thou:");
      txtset(t3,"A) Serve Justice, refusing to act, thus being disgraced; or");
      txtset(t4,"B) Honor thine oath and unfairly evict the landowner?");
      break; case 20:
      txtset(t,"Thou dost believe that virtue resides in all people. Thou dost see a rogue");
      txtset(t2,"steal from thy Lord. Dost thou:");
      txtset(t3,"A) Call him to Justice; or");
      txtset(t4,"B) Personally try to sway him back to the Spiritual path of good?");
      break; case 21:
      txtset(t,"Unwitnessed, thou hast slain a great dragon in self defense. A poor warrior");
      txtset(t2,"claims the offered reward. Dost thou:");
      txtset(t3,"A) Justly step forward to claim the reward; or");
      txtset(t4,"B) Humbly go about life, secure in thy self-esteem?");
      break; case 22:
      txtset(t,"Thou art a bounty hunter sworn to return an alleged murderer. After his");
      txtset(t2,"capture, thou believest him to be innocent. Dost thou:");
      txtset(t3,"A) Sacrifice thy sizeable bounty for thy belief; or");
      txtset(t4,"B) Honor thy oath to return him as thou hast promised?");
      break; case 23:
      txtset(t,"Thou hast spent thy life in charitable and righteous work. Thine uncle the");
      txtset(t2,"innkeeper lies ill and asks you to take over his tavern. Dost thou:");
      txtset(t3,"A) Sacrifice thy life of purity to aid thy kin; or");
      txtset(t4,"B) Decline & follow thy Spirit's call?");
      break; case 24:
      txtset(t,"Thou art an elderly, wealthy eccentric. Thy end is near. Dost thou:");
      txtset(t2,"A) Donate all thy wealth to feed hundreds of starving children, and receive public adulation; or");
      txtset(t3,"B) Humbly live out thy life, willing thy fortune to thy heirs?");
      break; case 25:
      txtset(t,"In thy youth thou pledged to marry thy sweetheart. Now thou art on a sacred");
      txtset(t2,"quest in distant lands. Thy sweetheart asks thee to keep thy vow. Dost thou:");
      txtset(t3,"A) Honor thy pledge to wed; or");
      txtset(t4,"B) Follow thy Spiritual crusade?");
      break; case 26:
      txtset(t,"Thou art at a crossroads in thy life. Dost thou:");
      txtset(t2,"A) Choose the Honorable life of a Paladin, striving for Truth and Courage; or");
      txtset(t3,"B) Choose the Humble life of a Shepherd, and a world of simplicity and peace?");
      break; case 27:
      txtset(t,"Thy parents wish thee to become an apprentice. Two positions are available.");
      txtset(t2,"Dost thou:");
      txtset(t3,"A) Become an acolyte in the Spiritual order; or");
      txtset(t4,"B) Become an assistant to a humble village cobbler?");
      break; case -2:
      txtset(t,"\"At last thou hast come to fulfill thy destiny,\" the gypsy says.");
      txtset(t2,"She smiles, as if in great relief.");
      txtset(t3,"\"Sit before me now, and I shall pour the light of Virtue into the");
      txtset(t4,"shadows of thy future.\"");
      break; case -1:
      txtset(t,"On a wooden table eight bottles stand, a rainbow of bubbling liquids.");
      txtset(t2,"\"Behold the virtues of the Avatar\", the woman says.");
      txtset(t3,"\"Let us begin the casting!\"");
      txtset(t4,"(Recommended buttons for answers are blinking.)");
      break; case -3:
      txtset(t,"\"The path of the Avatar lies beneath thy feet, worthy ");
      txtadd(t,u6o_name); txtadd(t,",\"");
      txtset(t2,"the gypsy intones. With a mysterious smile, she passes you the flask");
      txtset(t3,"of shimmering liquids. \"Drink of these waters and go forth among our");
      txtset(t4,"people, who shall receive thee in Joy!\"");
    }













    x=192+32-16;
    y=184+132*2-8-4;
    txtcol=rgb(255,255,255); txtout(ps,x+1,y+1,t);
    y+=20;
    txtcol=rgb(255,255,255); txtout(ps,x+1,y+1,t2);
    y+=20;
    txtcol=rgb(255,255,255); txtout(ps,x+1,y+1,t3);
    y+=20;
    txtcol=rgb(255,255,255); txtout(ps,x+1,y+1,t4);


    if (keyhit(65)){//"A" key
      mx=(279*2)+192; my=(174*2)+184; goto abkey;
    }
    if (keyhit(66)){//"B" key
      mx=((279+17)*2)+192; my=(174*2)+184; goto abkey;
    }

    //for compatibility, simulate a mouse click in the top left corner
    if (keyhit(VK_RETURN)){
      mx=0; my=0; goto abkey;
    }

    if (fs->mouse_click){
      fs->mouse_click=NULL;
abkey:


      if (vtext<0){
        if (vtext==-3) {intro=101; intro_setup=0; goto intro_done;}
        vtext++; vaction_done=1; 
        if (vtext==0) vtext=rnd*28;
      }else{
        if (vaction_done){




          x=(mx-192)/2; y=(my-184)/2;
          static unsigned char stats_warning=1;

          if ((x>=279)&&(x<(279+16))&&(y>=174)&&(y<(174+16))){
            vques[vtext]=1;
            if (x5>x4){
              if (stats_warning){
                stats_warning=0;
                if (IDNO==MessageBox(NULL,"If you select a non-blinking answer the total of your strength, dexterity and intelligence will be lower. Are you sure?","Ultima 6 Online",MB_YESNO)) vques[vtext]=0;
              }
            }
          }

          if ((x>=(279+17))&&(x<(279+17+16))&&(y>=174)&&(y<(174+16))){
            vques[vtext]=2;
            if (x4>x5){
              if (stats_warning){
                stats_warning=0;
                if (IDNO==MessageBox(NULL,"If you select a non-blinking answer the total of your strength, dexterity and intelligence will be lower. Are you sure?","Ultima 6 Online",MB_YESNO)) vques[vtext]=0;
              }
            }
          }

          if (vques[vtext]==0) goto ab_noselection;





          x2=vtext; x=-1; x4=0; x3=0; if (vques[vtext]==1) x3=1;
          u6o_vq[vtext]=vques[vtext]-1;
          x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_COMPASSION} //i3,d3
          x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_VALOR} //i3,s3
          x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_JUSTICE} //i3,X
          x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_SACRIFICE} //i3,X
          x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_HONOR} //i3,X
          x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_SPIRITUALITY} //i3,a1
          x++; if (x==x2) if (x3) {U6O2_HONESTY} else {U6O2_HUMILITY} //i3,X
          x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_VALOR} //d3,s3
          x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_JUSTICE} //d3,X
          x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_SACRIFICE} //d3,X
          x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_HONOR} //d3,X
          x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_SPIRITUALITY} //d3,a1
          x++; if (x==x2) if (x3) {U6O2_COMPASSION} else {U6O2_HUMILITY} //d3,X
          x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_JUSTICE} //s3,X
          x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_SACRIFICE} //s3,X
          x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_HONOR} //s3,X
          x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_SPIRITUALITY} //s3,a1
          x++; if (x==x2) if (x3) {U6O2_VALOR} else {U6O2_HUMILITY} //s3,X
          x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_SACRIFICE} //i,d
          x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_HONOR} //s,d
          x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_SPIRITUALITY} //X,a1
          x++; if (x==x2) if (x3) {U6O2_JUSTICE} else {U6O2_HUMILITY} //OK,X
          x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_HONOR} //s,i
          x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_SPIRITUALITY} //X,OK
          x++; if (x==x2) if (x3) {U6O2_SACRIFICE} else {U6O2_HUMILITY} //a1,X
          x++; if (x==x2) if (x3) {U6O2_HONOR} else {U6O2_SPIRITUALITY} //X,a1
          x++; if (x==x2) if (x3) {U6O2_HONOR} else {U6O2_HUMILITY} //OK,X
          x++; if (x==x2) if (x3) {U6O2_SPIRITUALITY} else {U6O2_HUMILITY} //a1,X

          //show vial action (needs to relate to question!)
          vaction=x4; f=0; vaction_done=0;

          for (x=0; x<=27; x++) if (vques[x]==0) goto vquesnew;
vquesnew:
          if (x==28) {
            vtext=-3;
          }else{
nextvques: x=rnd*28; if (vques[x]) goto nextvques;
            vtext=x;
          }//28
ab_noselection:;
        }//vaction_done
      }







    }
  }//intro==100


  if (intro==101){
    if (intro_setup==0){ intro_setup=1;
    f=0;
    }
    img(ps320200,0,0,intro_flask);
    f+=et;
    for (y=34;y<=128;y++){
      for (x=116;x<=203;x++){
vialmix2:
        x3=rnd*8;
        x4=rnd*7;
        if (vquan[x3]>x4) goto vialmix2;
        if (ps320200->o2[y*320+x]==0) ps320200->o2[y*320+x]=vcol[x3];
      }}

    img(ps640400,ps320200); img(ps,192,184,ps640400);

    x=144;
    y=640;
    txtset(t,"As you drink from the flask, vertigo overwhelms you. A soothing mist obscures the gypsy's face,");
    txtcol=rgb(255,255,255); txtouts(ps,x+1,y+1,t);
    txtset(t,"and you sink without fear into an untroubled sleep.");
    txtcol=rgb(255,255,255); txtouts(ps,x+1,y+1+20,t);

    if (fs->mouse_click||keyhit(VK_RETURN)){
      //final backup of userinfo
      tfh=open2("userinfo.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
      txtfileout(u6o_user_name,tfh);
      txtfileout(u6o_user_password,tfh);
      close(tfh);

      //save walkthru pos
      txtset(t,".\\dr\\walkthru.pos");
      tfh=open2(t,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
      put(tfh,&walkthru_pos,4); //lines to skip of the walkthru
      close(tfh);
      walkthru_pos_skip=0;

      tfh=open(".\\dr\\controls.bin");
      get(tfh,&U6OK,512); get(tfh,&U6OK_FLAGS,128);
      close(tfh);

      u6o_createcharacter=1; //ALLOW CREATION OF NEW PLAYER

      fs->mouse_click=NULL; intro=0; intro_setup=0;
      fs->offset_x=1024; fs->graphic=NULL;
    }
  }//intro==101

  if (intro==102){
    if (intro_setup==0){ intro_setup=1;
    f=0;
    }
    x=f*16.0f; if (x>191) x=191;
    img(ps320200,-x,0,intro_caravan);

    f+=et;
    img(ps640400,ps320200); img(ps,192,184,ps640400);

    x=192+32-16;
    y=616-16;
    txtset(t,"\"Welcome, O Seeker!\""); txtcol=rgb(255,255,255); txtouts(ps,x,y,t);
    y+=32;
    txtset(t,"A lonely stroll along an unfamiliar forest path brings you upon a curious gypsy"); txtcol=rgb(255,255,255); txtouts(ps,x,y,t);
    y+=32;
    txtset(t,"wagon, its exotic colors dappled in the summer shade."); txtcol=rgb(255,255,255); txtouts(ps,x,y,t);
    y+=32;
    txtset(t,"A woman's voice rings out with friendship, beckoning you into across the"); txtcol=rgb(255,255,255); txtouts(ps,x,y,t);
    y+=32;
    txtset(t,"wagon's threshold and, as it happens, into another life...."); txtcol=rgb(255,255,255); txtouts(ps,x,y,t);


    //txtset(t,"and you sink without fear into an untroubled sleep.");
    //txtcol=rgb(255,255,255); txtout(ps,x+1,y+1+20,t);


    if (fs->mouse_click||keyhit(VK_RETURN)){
      fs->mouse_click=NULL; intro=100; intro_setup=0;
    }
  }//intro==102

  if (intro==200){
    if (intro_setup==0){ intro_setup=1;

    tfh=open2("userinfo.txt",OF_READWRITE|OF_SHARE_COMPAT);
    if (tfh->h!=HFILE_ERROR){
      txtfilein(u6o_user_name,tfh);
      txtfilein(u6o_user_password,tfh);
      close(tfh);
    }else{
      txtset(u6o_user_name,"UNKNOWN");
      txtset(u6o_user_password,"UNKNOWN");
    }

    f=0;
    }


    if (NEThost){
      goto login; //very stupid way of doing this, just a quick fix
    }


    img0(ps,18,25,intro_ultimavi);

    txtset(t,"Create a Character"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*2+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,160,0); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    txtset(t,"Journey Onward"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*3+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,160,0); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    txtset(t,"Transfer a Character"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*4+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,160,0); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    txtset(t,"Custom Controls"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*5+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,160,0); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    txtset(t,"Custom Portrait"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*6+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,160,0); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    txtset(t,"Name Display Colour"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*7+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,160,0); 
    if (u6o_namecolour) txtcol=u6o_namecolour;
    txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;







    f+=et;


    x=mx; y=my;



    if ((x>=0)&&(x<320)&&(y>=128)&&(y<192)){
      txtset(t,"Create a Character"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
      x5=160-tagxy.cx/2; y5=64*2+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
      txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;
    }

    if ((x>=0)&&(x<320)&&(y>=192)&&(y<256)){
      txtset(t,"Journey Onward"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
      x5=160-tagxy.cx/2; y5=64*3+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
      txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;
      txtset(t,"USER NAME: ");
      txtadd(t,u6o_user_name);
      txtadd(t,"        USER PASSWORD: ");
      txtadd(t,u6o_user_password);
      txtcol=rgb(255,255,255); txtouts(ps,256+64,736+4,t);
    }

    if ((x>=0)&&(x<320)&&(y>=256)&&(y<320)){
      txtset(t,"Transfer a Character"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
      x5=160-tagxy.cx/2; y5=64*4+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
      txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;
      txtset(t,"CHANGE USER NAME AND USER PASSWORD");
      txtcol=rgb(255,255,255); txtouts(ps,256+64,736+4,t);
    }

    if ((x>=0)&&(x<320)&&(y>=(256+64))&&(y<(320+64))){
      txtset(t,"Custom Controls"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
      x5=160-tagxy.cx/2; y5=64*5+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
      txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;
      txtset(t,"CHANGE DEFAULT KEYBOARD KEYS");
      txtcol=rgb(255,255,255); txtouts(ps,256+64+64,736+4,t);
    }

    if ((x>=0)&&(x<320)&&(y>=(256+64+64))&&(y<(320+64+64))){
      txtset(t,"Custom Portrait"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
      x5=160-tagxy.cx/2; y5=64*6+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
      txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;
      txtset(t,"UPLOAD A CUSTOM PORTRAIT FOR AN EXISTING CHARACTER");
      txtcol=rgb(255,255,255); txtouts(ps,256+64,736+4,t);
    }


    if ((x>=0)&&(x<320)&&(y>=(256+64+64+64))&&(y<(320+64+64+64))){
      txtset(t,"Name Display Colour"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
      x5=160-tagxy.cx/2; y5=64*7+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
      txtcol=rgb(255,255,96);
      if (u6o_namecolour){
        x6=u6o_namecolour&255;//blue
        x7=(u6o_namecolour>>8)&255;//green
        x8=(u6o_namecolour>>16)&255;//red
        x6+=64; if (x6>255) x6=255;
        x7+=64; if (x7>255) x7=255;
        x8+=64; if (x8>255) x8=255;
        txtcol=rgb(x8,x7,x6);
      }
      txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;
      txtset(t,"SET THE COLOUR TO DISPLAY YOUR CHARACTER'S NAME WITH WHEN SENDING MESSAGES");
      txtcol=rgb(255,255,255); txtouts(ps,256-64-32,736+4,t);
    }




    if (fs->mouse_click){
      fs->mouse_click=NULL;

      if ((x>=0)&&(x<320)&&(y>=128)&&(y<192)){intro=201; intro_setup=0; goto intro_done;}
      if ((x>=0)&&(x<320)&&(y>=256)&&(y<320)){intro=202; intro_setup=0; goto intro_done;}
      if ((x>=0)&&(x<320)&&(y>=(256+64))&&(y<(320+64))){intro=206; intro_setup=0; goto intro_done;}
      if ((x>=0)&&(x<320)&&(y>=(256+64+64))&&(y<(320+64+64))){intro=204; intro_setup=0; goto intro_done;}

      if ((x>=0)&&(x<320)&&(y>=(256+64+64+64))&&(y<(320+64+64+64))){

        x5=getcol(u6o_namecolour);
        //assume a 24bit colour returned

        x6=x5&255;//blue
        x7=(x5>>8)&255;//green
        x8=(x5>>16)&255;//red
        if ((x6+x7+x8)<255){
          MessageBox(NULL,"That colour cannot be used because it's too dark! (The sum of the red, green and blue channels must be at least 255)","Ultima 6 Online",MB_OK);
        }else{
          u6o_namecolour=x5;//initially sent as 24bit
        }

        goto intro_done;
      }

      //journey onward
      if ((x>=0)&&(x<320)&&(y>=192)&&(y<256)){
login:
        txtset(t,"UNKNOWN");
        if (!txtsame(t,u6o_user_name)){
          if (!txtsame(t,u6o_user_password)){

            tfh=open(".\\dr\\controls.bin");
            get(tfh,&U6OK,512); get(tfh,&U6OK_FLAGS,128);
            close(tfh);

            fs->offset_x=1024; fs->graphic=NULL;
            intro=0; intro_setup=0; goto intro_done;
          }}
        MessageBox(NULL,"Select CREATE A CHARACTER if you are a new player.","Ultima 6 Online",MB_OK);

      }

    }
  }//intro==200


  //transfer a character



  if (intro==202){
    static txt *tusername2=txtnew();
    static txt *tuserpassword2=txtnew();
    static txt *tnewuserpassword2=txtnew();
    static unsigned char intro_ep=1;
    static unsigned char intro_ti=0;
    if (intro_setup==0){ intro_setup=1;
    txtset(tusername2,"");
    txtset(tuserpassword2,"");
    txtset(tnewuserpassword2,"");
    f=0;
    //select username text field
    intro_ep=0; intro_ti=2; GETINPUT_setup(tusername2,&intro_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
    }
    f+=et;

    img0(ps,1024-25-22-2,2,intro_back);
    img0(ps,18,25,intro_ultimavi);
    txtset(t,"Transfer a Character"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*4+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;


    img0(ps,0,256+64+8,intro_tacinfo);

    txtcol=rgb(252,200,20);
    txtset(t,tusername2); if (((long)(f*4)&1)&&(intro_ti==2)&&(intro_ep==0)) txtadd(t,"|");
    img(ps,120+4,320+4+4,intro_ifield);
    txtout(ps,120+4+4,320+4-1,t);
    txtset(t,tuserpassword2); if (((long)(f*4)&1)&&(intro_ti==3)&&(intro_ep==0)) txtadd(t,"|");
    img(ps,120+4,320+32+4+4,intro_ifield);
    txtout(ps,120+4+4,320+32+4-1,t);
    txtset(t,tnewuserpassword2); if (((long)(f*4)&1)&&(intro_ti==4)&&(intro_ep==0)) txtadd(t,"|");
    img(ps,120+4,320+32+4+4+96,intro_ifield);
    txtout(ps,120+4+4,320+32+4-1+96,t);

    x=mx; y=my;

    if ((x>=0)&&(x<320)&&(y>=(352+96))&&(y<(384+96))){
      txtset(t,"ONLY SET IF CHANGING YOUR EXISTING PASSWORD");
      txtcol=rgb(255,255,255); txtouts(ps,256+64,736+4,t);
    }


    //switch to next field if tab pressed!
    if (GETINPUT_tab_pressed==GETINPUT_TAB_PRESSED){
      GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_STOP;
      if (intro_ti==4){
        intro_ep=0; intro_ti=2;
        GETINPUT_setup(tusername2,&intro_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
        goto transferachar_newfieldselected;
      }
      if (intro_ti==2){
        intro_ep=0; intro_ti=3;
        GETINPUT_setup(tuserpassword2,&intro_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
        goto transferachar_newfieldselected;
      }
      if (intro_ti==3){
        intro_ep=0; intro_ti=4;
        GETINPUT_setup(tnewuserpassword2,&intro_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
        goto transferachar_newfieldselected;
      }
    }//GETINPUT_TAB_PRESSED
transferachar_newfieldselected:

    if (keyhit(VK_RETURN)) goto transferachar_enterkey;


    if (fs->mouse_click){
      fs->mouse_click=NULL;

      if ((x>(1024-21-4-26))&&(y<(21+4))){
        if (intro_ep==0){
          GETINPUT_stop();
        }
        intro=200; intro_setup=0; goto intro_done;
      }



      if ((x>=0)&&(x<320)&&(y>=320)&&(y<352)){
        intro_ep=0; intro_ti=2;
        GETINPUT_setup(tusername2,&intro_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
      }
      if ((x>=0)&&(x<320)&&(y>=352)&&(y<384)){
        intro_ep=0; intro_ti=3;
        GETINPUT_setup(tuserpassword2,&intro_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
      }
      if ((x>=0)&&(x<320)&&(y>=(352+96))&&(y<(384+96))){
        intro_ep=0; intro_ti=4;
        GETINPUT_setup(tnewuserpassword2,&intro_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
      }

      if ((x>=0)&&(x<320)&&(y>=384)&&(y<416)){
transferachar_enterkey:

        if (tusername2->l==0){
          MessageBox(NULL,"Please select a USER NAME","Ultima 6 Online",MB_OK);
          goto intro_done;
        }
        if (tuserpassword2->l==0){
          MessageBox(NULL,"Please select a USER PASSWORD","Ultima 6 Online",MB_OK);
          goto intro_done;
        }

        if (intro_ep==0){
          GETINPUT_stop();
        }
        txtset(u6o_user_name,tusername2);
        txtset(u6o_user_password,tuserpassword2);
        if (tnewuserpassword2->l){
          txtset(u6o_new_user_password,tnewuserpassword2);
        }

        tfh=open2("userinfo.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
        txtfileout(u6o_user_name,tfh);
        if (tnewuserpassword2->l) txtfileout(u6o_new_user_password,tfh); else txtfileout(u6o_user_password,tfh);
        close(tfh);

        //save walkthru pos
        txtset(t,".\\dr\\walkthru.pos");
        tfh=open2(t,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
        put(tfh,&walkthru_pos,4); //lines to skip of the walkthru
        close(tfh);
        walkthru_pos_skip=0;

        tfh=open(".\\dr\\controls.bin");
        get(tfh,&U6OK,512); get(tfh,&U6OK_FLAGS,128);
        close(tfh);

        fs->offset_x=1024; fs->graphic=NULL;
        intro=0; intro_setup=0; goto intro_done;
      }
    }//->mouse_click
  }//intro==202



  if (intro==206){//custom keys: main menu
    if (intro_setup==0){
      f=0;
      intro_setup=1;
    }
    img0(ps,1024-25-22-2,2,intro_back);
    img0(ps,18,25,intro_ultimavi);
    txtset(t,"Custom Controls"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*5+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;


    txtcol=rgb(255,255,255);

    x=320; y=256;
    x+=32; txtset(t,"RESTORE DEFAULT CONTROLS"); txtouts(ps,x,y,t); x-=32;
    img0(ps,x,y,intro_next);
    y+=64;
    x+=32; txtset(t,"CHANGE CURRENT CUSTOM CONTROLS"); txtouts(ps,x,y,t); x-=32;
    img0(ps,x,y,intro_next);




    if (fs->mouse_click){
      fs->mouse_click=NULL;

      if ((mx>=x)&&(mx<(x+22))&&(my>=(y-64))&&(my<(y+22-64))){
        memcpy(&U6OK_TEMP,&U6OK_DEFAULT,512);
        memcpy(&U6OK_TEMP_FLAGS,&U6OK_DEFAULT_FLAGS,128);
        tfh=open2(".\\dr\\controls.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
        put(tfh,&U6OK_TEMP,512);
        put(tfh,&U6OK_TEMP_FLAGS,128);
        close(tfh);
        intro=200; intro_setup=0; goto intro_done;
      }

      if ((mx>=x)&&(mx<(x+22))&&(my>=y)&&(my<(y+22))){
        intro=203; intro_setup=0; goto intro_done;
      }

      if ((mx>(1024-21-4-26))&&(my<(21+4))){//back
        intro=200; intro_setup=0; goto intro_done;
      }
    }//->mouse_click
  }//intro==206








  if (intro==203){ //custom keys
    static unsigned char nextkey;
    static unsigned short key1,key2;

    if (intro_setup==0){
      tfh=open(".\\dr\\controls.bin");
      get(tfh,&U6OK_TEMP,512);
      get(tfh,&U6OK_TEMP_FLAGS,128);
      close(tfh);
      key1=0;
      key2=0;
      nextkey=0;
      intro_setup=1;
      f=0;
    }

    img0(ps,1024-25-22-2,2,intro_back);
    img0(ps,18,25,intro_ultimavi);
    txtset(t,"Custom Controls"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*5+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    img0(ps,536,420,intro_ccsave1);
    img0(ps,536,484,intro_ccsave3);

    if (nextkey!=0) img0(ps,466,416,intro_back);
    if (nextkey!=72){
      if ((U6OK_TEMP[nextkey][0]!=U6OK_DEFAULT[nextkey][0])||(U6OK_TEMP[nextkey][1]!=U6OK_DEFAULT[nextkey][1])||(U6OK_TEMP_FLAGS[nextkey]!=U6OK_DEFAULT_FLAGS[nextkey])){
        img0(ps,536,420,intro_ccsave2);
      }
      img0(ps,665,416,intro_next);
      if (U6OK_TEMP_FLAGS[nextkey]&2){
        if (U6OK_TEMP_FLAGS[nextkey]&1) img(ps,720,411,instantclickok); else img(ps,720,411,instantclickx);
      }
    }



    x=320; y=128;
    txtcol=rgb(18*2,108*2,255); //(3<<11)+(46<<5)+27;

    txtset(t,"Use the green arrows to select which controls to change");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"Only change the controls you need to, then select SAVE ALL");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"To use a combination (eg. SHIFT+T), hold the 1st key then press the 2nd key");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"You may also use SHIFT, CTRL or ALT independently as non-combination keys");
    txtouts(ps,x,y,t); y+=32;

    txtcol=rgb(255,64,64);
    txtset(t,"CAUTION");
    txtouts(ps,x,y,t); y+=32;
    txtcol=rgb(18*2,108*2,255);
    txtset(t,"All keys can used, but some controls may conflict with each other!");
    txtouts(ps,x,y,t); y+=32;

    txtset(t,"Press SAVE ALL button to save custom controls");
    i=-1;
    i++; if (nextkey==i) txtset(t,"Press WALK RIGHT (WALK EAST) key now [ default key: RIGHT ARROW ]");
    i++; if (nextkey==i) txtset(t,"Press WALK LEFT (WALK WEST) key now [ LEFT ARROW ]");
    i++; if (nextkey==i) txtset(t,"Press WALK UP (WALK NORTH) key now [ UP ARROW ]");
    i++; if (nextkey==i) txtset(t,"Press WALK DOWN (WALK SOUTH) key now [ DOWN ARROW ]");
    i++; if (nextkey==i) txtset(t,"Press TALK key now [ T ]");
    i++; if (nextkey==i) txtset(t,"Press GLOBAL TALK key now [ SHIFT+T ]");
    i++; if (nextkey==i) txtset(t,"Press MESSAGE SEND key now [ ENTER ]");
    i++; if (nextkey==i) txtset(t,"Press ATTACK key now [ A ]");
    i++; if (nextkey==i) txtset(t,"Press LOOK key now [ L ]");
    i++; if (nextkey==i) txtset(t,"Press USE key now [ U ]");
    i++; if (nextkey==i) txtset(t,"Press MAXIMIZE/MINIMIZE key now [ M ]");
    i++; if (nextkey==i) txtset(t,"Press QUIT key now [ Q ]");
    i++; if (nextkey==i) txtset(t,"Press SOUND key now [ S ]");
    i++; if (nextkey==i) txtset(t,"Press CANCEL key now [ ESC ]");
    i++; if (nextkey==i) txtset(t,"Press RETYPE/MESSAGE RECALL key now [ F3 ]");
    i++; if (nextkey==i) txtset(t,"Press RESPAWN key now [ F1 ]");

    i++; if (nextkey==i) txtset(t,"Press SELECT 1ST PARTY MEMBER key now [ 1 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 2ND PARTY MEMBER key now [ 2 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 3RD PARTY MEMBER key now [ 3 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 4TH PARTY MEMBER key now [ 4 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 5TH PARTY MEMBER key now [ 5 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 6TH PARTY MEMBER key now [ 6 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 7TH PARTY MEMBER key now [ 7 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 8TH PARTY MEMBER key now [ 8 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 9TH PARTY MEMBER key now [ 9 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 10TH PARTY MEMBER key now [ 0 ]");

    i++; if (nextkey==i) txtset(t,"Press SELECT 1ST NUMBERED TARGET key now [ 1 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 2ND NUMBERED TARGET key now [ 2 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 3RD NUMBERED TARGET key now [ 3 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 4TH NUMBERED TARGET key now [ 4 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 5TH NUMBERED TARGET key now [ 5 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 6TH NUMBERED TARGET key now [ 6 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 7TH NUMBERED TARGET key now [ 7 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 8TH NUMBERED TARGET key now [ 8 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 9TH NUMBERED TARGET key now [ 9 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 10TH NUMBERED TARGET key now [ 0 ]");

    i++; if (nextkey==i) txtset(t,"Press SELECT 1ST RECALL SPELL key now [ F5 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 2ND RECALL SPELL key now [ F6 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 3RD RECALL SPELL key now [ F7 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 4TH RECALL SPELL key now [ F8 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 5TH RECALL SPELL key now [ F9 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 6TH RECALL SPELL key now [ F10 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 7TH RECALL SPELL key now [ F11 ]");
    i++; if (nextkey==i) txtset(t,"Press SELECT 8TH RECALL SPELL key now [ F12 ]");

    i++; if (nextkey==i) txtset(t,"Press SET 1ST RECALL SPELL key now [ SHIFT+F5 ]");
    i++; if (nextkey==i) txtset(t,"Press SET 2ND RECALL SPELL key now [ SHIFT+F6 ]");
    i++; if (nextkey==i) txtset(t,"Press SET 3RD RECALL SPELL key now [ SHIFT+F7 ]");
    i++; if (nextkey==i) txtset(t,"Press SET 4TH RECALL SPELL key now [ SHIFT+F8 ]");
    i++; if (nextkey==i) txtset(t,"Press SET 5TH RECALL SPELL key now [ SHIFT+F9 ]");
    i++; if (nextkey==i) txtset(t,"Press SET 6TH RECALL SPELL key now [ SHIFT+F10 ]");
    i++; if (nextkey==i) txtset(t,"Press SET 7TH RECALL SPELL key now [ SHIFT+F11 ]");
    i++; if (nextkey==i) txtset(t,"Press SET 8TH RECALL SPELL key now [ SHIFT+F12 ]");

    i++; if (nextkey==i) txtset(t,"Press ENTER PARTIAL AMOUNT/QUANTITY key now [ SHIFT ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 1 FOR PARTIAL AMOUNT/QUANTITY key now [ 1 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 2 FOR PARTIAL AMOUNT/QUANTITY key now [ 2 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 3 FOR PARTIAL AMOUNT/QUANTITY key now [ 3 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 4 FOR PARTIAL AMOUNT/QUANTITY key now [ 4 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 5 FOR PARTIAL AMOUNT/QUANTITY key now [ 5 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 6 FOR PARTIAL AMOUNT/QUANTITY key now [ 6 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 7 FOR PARTIAL AMOUNT/QUANTITY key now [ 7 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 8 FOR PARTIAL AMOUNT/QUANTITY key now [ 8 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 9 FOR PARTIAL AMOUNT/QUANTITY key now [ 9 ]");
    i++; if (nextkey==i) txtset(t,"Press NUMBER 0 FOR PARTIAL AMOUNT/QUANTITY key now [ 0 ]");

    i++; if (nextkey==i) txtset(t,"Press SCROLL UP key now [ MOUSE WHEEL UP ]");
    i++; if (nextkey==i) txtset(t,"Press SCROLL DOWN key now [ MOUSE WHEEL DOWN ]");

    i++; if (nextkey==i) txtset(t,"Press ALTERNATIVE ATTACK key now [ NOT USED ]");
    i++; if (nextkey==i) txtset(t,"Press ALTERNATIVE LOOK key now [ NOT USED ]");
    i++; if (nextkey==i) txtset(t,"Press ALTERNATIVE USE key now [ NOT USED ]");

    i++; if (nextkey==i) txtset(t,"Press CHAT (CONTINUE CONVERSATION) key now [ CONTROL+T ]");

    i++; if (nextkey==i) txtset(t,"Press VOICE CHAT key now [ V ]");

    i++; if (nextkey==i) txtset(t,"Press MARK/UNMARK AS DO-NOT-SELL key now [ X ]");

    i++; if (nextkey==i) txtset(t,"Press RESPAWN FOLLOWERS key now [ SHIFT+F1 ]");


    tagxy.cx=0; tagxy.cy=0;
    ps->s->GetDC(&taghdc);
    SelectObject(taghdc,txtfnt);
    GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
    ps->s->ReleaseDC(taghdc);
    x2=tagxy.cx/2;
    txtcol=rgb(255,255,255); txtouts(ps,512+64-x2,128+64+64+64+22,t);

    if (nextkey!=72){

      txtset(t,"Waiting.");
      x=ett*2.0f; x&=3;
      for (x2=0;x2<=x;x2++){
        txtadd(t,".");
      }
      tagxy.cx=0; tagxy.cy=0;
      ps->s->GetDC(&taghdc);
      SelectObject(taghdc,txtfnt);
      GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
      ps->s->ReleaseDC(taghdc);
      x2=tagxy.cx/2;
      txtcol=rgb(255,255,255); txtouts(ps,512+64-x2,128+64+64+64+22+32,t);

      for (i=0;i<=65535;i++){
        if (keyhit(i)){
          if (!key1) key1=i; else key2=i;
        }
      }
      if (key1){
        if (keyon[key1]==0){
          U6OK_TEMP[nextkey][0]=key1; U6OK_TEMP[nextkey][1]=key2;
          key1=0; key2=0; nextkey++;
        }
      }
    }






    if (fs->mouse_click){
      fs->mouse_click=NULL;

      if (nextkey!=72){
        if (U6OK_TEMP_FLAGS[nextkey]&2){
          if ((mx>=720)&&(mx<(720+84))&&(my>=411)&&(my<(411+32))){
            if (U6OK_TEMP_FLAGS[nextkey]&1) U6OK_TEMP_FLAGS[nextkey]--; else U6OK_TEMP_FLAGS[nextkey]|=1;
          }
        }
      }

      if (nextkey!=72){
        if ((mx>=536)&&(mx<(536+81))&&(my>=420)&&(my<(420+16))){
          if ((U6OK_TEMP[nextkey][0]!=U6OK_DEFAULT[nextkey][0])||(U6OK_TEMP[nextkey][1]!=U6OK_DEFAULT[nextkey][1])||(U6OK_TEMP_FLAGS[nextkey]!=U6OK_DEFAULT_FLAGS[nextkey])){
            U6OK_TEMP[nextkey][0]=U6OK_DEFAULT[nextkey][0];
            U6OK_TEMP[nextkey][1]=U6OK_DEFAULT[nextkey][1];
            U6OK_TEMP_FLAGS[nextkey]=U6OK_DEFAULT_FLAGS[nextkey];
            nextkey++; key1=0; key2=0; goto intro_done;
          }
        }
      }

      if ((mx>=536)&&(mx<(536+60))&&(my>=484)&&(my<(484+16))){ //save all
        tfh=open2(".\\dr\\controls.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
        put(tfh,&U6OK_TEMP,512);
        put(tfh,&U6OK_TEMP_FLAGS,128);
        close(tfh);
        intro=200; intro_setup=0; goto intro_done;
      }

      if ((mx>=466)&&(mx<(466+22))&&(my>=416)&&(my<(416+22))){
        if (nextkey) {nextkey--; key1=0; key2=0; goto intro_done;}
      }

      if ((mx>=665)&&(mx<(665+22))&&(my>=416)&&(my<(416+22))){
        if (nextkey!=72) {nextkey++; key1=0; key2=0; goto intro_done;}
      }

      if ((mx>(1024-21-4-26))&&(my<(21+4))){
        intro=200; intro_setup=0; goto intro_done;
      }

    }//->mouse_click

  }//intro==203





  static surf *customport;


  if (intro==204){//custom portrait
    if (intro_setup==0){
      intro_setup=1;
      f=0;
    }

    img0(ps,1024-25-22-2,2,intro_back);
    img0(ps,18,25,intro_ultimavi);
    txtset(t,"Custom Portrait"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*6+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    x=320; y=128;
    txtcol=rgb(18*2,108*2,255); //(3<<11)+(46<<5)+27;

    txtset(t,"CUSTOM PORTRAIT UPLOAD RULES");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"1. The portrait must portray the Avatar as being human.");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"2. The portrait must consist mostly of the Avatar's face.");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"3. The gender of the portrait must match that of the Avatar.");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"4. The portrait must not be mistakable for any other original or custom portrait.");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"5. The portrait must not contain nudity or adult content.");
    txtouts(ps,x,y,t); y+=32;
    txtset(t,"6. The portrait must be in keeping with the spirit and timeframe of Ultima 6.");
    txtouts(ps,x,y,t); y+=32;
    y+=32;
    txtcol=rgb(255,64,64);
    txtset(t,"WARNING!");
    if ((long)(ett*2)&1) txtouts(ps,x,y,t);
    x+=96;
    txtset(t,"UPLOADING AN INAPPROPRIATE PORTRAIT WILL PERMANENTLY");
    txtouts(ps,x,y,t);
    y+=32;
    txtset(t,"DISABLE THIS FEATURE FOR USE WITH YOUR CHARACTER");

    txtouts(ps,x,y,t); y+=32; x-=96;

    txtcol=rgb(18*2,108*2,255);
    y+=32; 
    txtset(t,"LEVEL RESTRICTION: ONLY CHARACTERS WHO ARE LEVEL 3 OR ABOVE"); txtouts(ps,x,y,t); y+=32;
    txtset(t,"XP COST: 1000 EXPERIENCE POINTS ARE TAKEN PER UPLOAD"); txtouts(ps,x,y,t); y+=32;
    txtset(t,"FORMAT: ANY .BMP TYPE (DIMENTIONS 56x64)"); txtouts(ps,x,y,t); y+=32;
    txtset(t,"TRANSPARENT COLOUR: BLACK (RGB[0,0,0] AFTER CONVERSION TO 16BIT COLOUR)"); txtouts(ps,x,y,t); y+=32;
    y+=32; 
    txtcol=rgb(255,255,255);
    x+=128; x+=64;
    txtset(t,"SELECT PORTRAIT FILE TO UPLOAD"); txtouts(ps,x,y,t);
    x-=32;
    img0(ps,x,y,intro_next);


    if (fs->mouse_click){
      fs->mouse_click=NULL;

      if ((mx>=x)&&(mx<(x+22))&&(my>=y)&&(my<(y+22))){
        txtgetfilename(t);
        if (t->l==0){
          goto intro_done;
        }
        customport=loadimage(t->d,SURF_SYSMEM16);
        if (customport==NULL){
          txtadd(t," is not a .BMP file"); MessageBox(NULL,t->d,"Ultima 6 Online",MB_OK);
          goto intro_done;
        }
        if (customport->d.dwWidth!=56){
          MessageBox(NULL,"BMP must be 56 pixels wide","Ultima 6 Online",MB_OK);
          goto intro_done;
        }
        if (customport->d.dwHeight!=64){
          MessageBox(NULL,"BMP must be 64 pixels high","Ultima 6 Online",MB_OK);
          goto intro_done;
        }
        intro=205; intro_setup=0; goto intro_done;
      }

      if ((mx>(1024-21-4-26))&&(my<(21+4))){
        intro=200; intro_setup=0; goto intro_done;
      }

    }//->mouse_click

  }//intro==204


  if (intro==205){//confirm custom portrait
    if (intro_setup==0){

      intro_setup=1;
      f=0;
    }
    img0(ps,1024-25-22-2,2,intro_back);
    img0(ps,18,25,intro_ultimavi);
    txtset(t,"Custom Portrait"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*6+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    x=ett/3.0f; x&=15;
    if (x>7) {y=1; x-=8;} else y=0;
    img(bt32,-x*32,-y*32,bt8[0]);
    y2=ett*16; y2&=31;
    for (y=0;y<=24;y++){
      img(ps,320,y*32-y2,bt32);
      img(ps,320+32,y*32-y2,bt32);
    }
    img0(ps,320+4,128,customport);

    x=320+64+32; y=128+24;
    txtcol=rgb(18*2,108*2,255); //(3<<11)+(46<<5)+27;

    txtset(t,"Check that the portrait appears correctly on the backgrounds."); txtouts(ps,x,y,t);
    y+=32;
    txtset(t,"Pay attention to areas of the portrait that should or should not be transparent."); txtouts(ps,x,y,t);

    y+=128-32;
    txtcol=rgb(255,255,255);
    x+=32;
    txtset(t,"UPLOAD THIS PORTRAIT WHEN I ENTER BRITANNIA"); txtouts(ps,x,y,t);
    x-=32;
    img0(ps,x,y,intro_next);

    if (fs->mouse_click){
      fs->mouse_click=NULL;

      if ((mx>=x)&&(mx<(x+22))&&(my>=y)&&(my<(y+22))){
        //copy all data to client port buffer
        for(x2=0;x2<3584;x2++){
          customportrait[x2]=customport->o2[x2];
        }
        customportrait_upload=1;
        intro=200; intro_setup=0; goto intro_done;
      }

      if ((mx>(1024-21-4-26))&&(my<(21+4))){
        intro=200; intro_setup=0; goto intro_done;
      }

    }//->mouse_click

  }//intro==205





  //create a char
  if (intro==201){
    static unsigned char sex=0;
    static unsigned char portn=0;
    static unsigned char typen=0;
    static txt *tname2=txtnew();
    static txt *tusername2=txtnew();
    static txt *tuserpassword2=txtnew();
    static unsigned char intro201_ep=1;
    static unsigned char intro201_ti=0;
    static object* typen_obj=OBJnew_local();

    if (intro_setup==0){ intro_setup=1;
    txtset(tname2,"");
    txtset(tusername2,"");
    txtset(tuserpassword2,"");
    sex=0;
    portn=0;
    typen=0;
    intro201_ep=1;
    intro201_ti=0;

    //select username text field
    intro201_ep=0; intro201_ti=2; GETINPUT_setup(tusername2,&intro201_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;

    f=0;
    }
    f+=et;

    //show port
    x3=portn;
    if (sex){
      if (x3==0) x4=194;
      if (x3==1) x4=195;
      if (x3==2) x4=197;
      if (x3==3) x4=198;
      if (x3==4) x4=202;
      if (x3==5) x4=203;
      if (x3==6) x4=207;
    }else{
      if (x3==0) x4=196;
      if (x3==1) x4=199;
      if (x3==2) x4=200;
      if (x3==3) x4=201;
      if (x3==4) x4=204;
      if (x3==5) x4=205;
      if (x3==6) x4=206;
    }

    img0(ps,99+8,168+320,getportrait(x4));

    img0(ps,1024-25-22-2,2,intro_back);
    img0(ps,18,25,intro_ultimavi);
    txtset(t,"Create a Character"); txtfnt=fnt7; tagxy.cx=0; tagxy.cy=0; ps->s->GetDC(&taghdc); SelectObject(taghdc,txtfnt); GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy); ps->s->ReleaseDC(taghdc);
    x5=160-tagxy.cx/2; y5=64*2+22; txtcol=rgb(0,0,0); txtout(ps,x5,y5,t); txtout(ps,x5+1,y5,t); txtout(ps,x5+2,y5,t); txtout(ps,x5+2,y5+1,t); txtout(ps,x5+2,y5+2,t); txtout(ps,x5+1,y5+2,t); txtout(ps,x5,y5+2,t); txtout(ps,x5,y5+1,t);
    txtcol=rgb(255,255,96); txtout(ps,x5+1,y5+1,t); txtfnt=fnt1;

    if (intro201_ep||fs->mouse_click){ //show auto-formatting
      //name
      txtset(t5,tname2);
autoformat_name_cull3:
      for (i3=0;i3<t5->l;i3++){
        x=t5->d2[i3];
        x2=0; //cull?
        if (x<48) x2=1;
        if ((x>57)&&(x<65)) x2=1;
        if ((x>90)&&(x<97)) x2=1;
        if (x>122) x2=1;
        if ((x==45)&&(t5->d2[i3+1]!=45)) x2=0; //-
        if ((x==39)&&(t5->d2[i3+1]!=39)) x2=0; //'
        if ((x==46)&&(t5->d2[i3+1]!=46)) x2=0; //.
        if (x==95){t5->d2[i3]=32; x=32; x2=0; goto autoformat_name_cull3;} //_ to space
        if ((x==32)&&(t5->d2[i3+1]!=32)){
          if (i3&&(i3!=(t5->l-1))) x2=0;
        }
        if (x2){
          txtset(t6,t5); txtright(t6,t5->l-i3-1);
          txtleft(t5,i3); txtadd(t5,t6);
          goto autoformat_name_cull3;
        }
      }
      if (t5->l>16) txtleft(t5,16);
      txtset(tname2,t5);
      //username
      txtset(t5,tusername2);
      txtucase(t5);
autoformat_username_cull3:
      for (i3=0;i3<t5->l;i3++){
        x=t5->d2[i3];
        x2=0; //cull?
        if (x<48) x2=1;
        if (x>90) x2=1;
        if ((x>57)&&(x<65)) x2=1;
        if (x2){
          txtset(t6,t5); txtright(t6,t5->l-i3-1);
          txtleft(t5,i3); txtadd(t5,t6);
          goto autoformat_username_cull3;
        }
      }
      if (t5->l>16) txtleft(t5,16);
      txtset(tusername2,t5);
      //password
      txtset(t5,tuserpassword2);
      txtucase(t5);
autoformat_password_cull3:
      for (i3=0;i3<t5->l;i3++){
        x=t5->d2[i3];
        x2=0; //cull?
        if (x<48) x2=1;
        if (x>90) x2=1;
        if ((x>57)&&(x<65)) x2=1;
        if (x2){
          txtset(t6,t5); txtright(t6,t5->l-i3-1);
          txtleft(t5,i3); txtadd(t5,t6);
          goto autoformat_password_cull3;
        }
      }
      if (t5->l>16) txtleft(t5,16);
      txtset(tuserpassword2,t5);
    }//intro201_ep||fs->mouse_click


    txtcol=rgb(252,200,20);
    txtset(t,tusername2); if (((long)(f*4)&1)&&(intro201_ti==2)&&(intro201_ep==0)) txtadd(t,"|");
    img(ps,99+4,320+4+4,intro_ifield);
    txtout(ps,99+4+4,320+4-1,t);
    txtset(t,tuserpassword2); if (((long)(f*4)&1)&&(intro201_ti==3)&&(intro201_ep==0)) txtadd(t,"|");
    img(ps,99+4,320+32+4+4,intro_ifield);
    txtout(ps,99+4+4,320+32+4-1,t);
    txtset(t,tname2); if (((long)(f*4)&1)&&(intro201_ti==1)&&(intro201_ep==0)) txtadd(t,"|");
    img(ps,99+4,320+32*2+4+4,intro_ifield);
    txtout(ps,99+4+4,320+32*2+4-1,t);

    x3=410; if (typen!=0) x3=375+typen;
    x4=(long)(f*2.0f)%16;
    typen_obj->type=x3+x4*1024;
    getspr(typen_obj);
    img0(ps,99+8+12,136+320-8,bt32);



    img0(ps,0,320,intro_newchar2);





    //switch to next field if tab pressed!
    if (GETINPUT_tab_pressed==GETINPUT_TAB_PRESSED){
      GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_STOP;
      if (intro201_ti==1){
        intro201_ep=0; intro201_ti=2;
        GETINPUT_setup(tusername2,&intro201_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
        goto createachar_newfieldselected;
      }
      if (intro201_ti==2){
        intro201_ep=0; intro201_ti=3;
        GETINPUT_setup(tuserpassword2,&intro201_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
        goto createachar_newfieldselected;
      }
      if (intro201_ti==3){
        intro201_ep=0; intro201_ti=1;
        GETINPUT_setup(tname2,&intro201_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
        goto createachar_newfieldselected;
      }
    }//GETINPUT_TAB_PRESSED
createachar_newfieldselected:

    if (keyhit(VK_RETURN)) goto createachar_enterkey;











    x=mx; y=my;
    if (fs->mouse_click){
      fs->mouse_click=NULL;

      if ((x>(1024-21-4-26))&&(y<(21+4))){
        if (intro201_ep==0){
          GETINPUT_stop();
        }
        intro=200; intro_setup=0; goto intro_done;
      }

      if ((x>=0)&&(x<320)&&(y>=416)&&(y<448)){
        sex++; if (sex>1) sex=0;
      }

      if ((x>=0)&&(x<320)&&(y>=448)&&(y<480)){
        typen++; if (typen>12) typen=0;
      }

      if ((x>=0)&&(x<320)&&(y>=480)&&(y<512)){
        portn++; if (portn>6) portn=0;
      }



      if ((x>=0)&&(x<320)&&(y>=320)&&(y<352)){
        intro201_ep=0; intro201_ti=2;
        GETINPUT_setup(tusername2,&intro201_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
      }
      if ((x>=0)&&(x<320)&&(y>=352)&&(y<384)){
        intro201_ep=0; intro201_ti=3;
        GETINPUT_setup(tuserpassword2,&intro201_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
      }
      if ((x>=0)&&(x<320)&&(y>=384)&&(y<416)){
        intro201_ep=0; intro201_ti=1;
        GETINPUT_setup(tname2,&intro201_ep,16); GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED_INIT;
      }

      if ((x>=0)&&(x<320)&&(y>=576)&&(y<608)){ //continue
createachar_enterkey:

        //error checking MUST OCCUR HERE!
        if (tusername2->l==0){
          MessageBox(NULL,"Please select a USER NAME","Ultima 6 Online",MB_OK);
          goto intro_done;
        }
        if (tuserpassword2->l==0){
          MessageBox(NULL,"Please select a USER PASSWORD","Ultima 6 Online",MB_OK);
          goto intro_done;
        }
        if (tname2->l==0){
          MessageBox(NULL,"Please select a NAME","Ultima 6 Online",MB_OK);
          goto intro_done;
        }

        if (NEThost==NULL){


          //check if username or charname is taken
          txtsetchar(t,246);
          txtaddchar(t,tusername2->l);
          txtadd(t,tusername2);
          txtaddchar(t,tname2->l);
          txtadd(t,tname2);
          NET_send(NETplayer,NULL,t);

          //wait for a reply
          static unsigned long cac_time;
          cac_time=timeGetTime();
cac_waitforhost:
          x3=0;
          i=0;
          if (socketclient_ri[i]->d[socketclient_ri[i]->next]->l){ //buffered data available
            txtset(t,socketclient_ri[i]->d[socketclient_ri[i]->next]);
            txtNEWLEN(socketclient_ri[i]->d[socketclient_ri[i]->next],0);
            socketclient_ri[i]->next++;
            x3=1;
          }//->l
          if (timeGetTime()>(cac_time+8000)) goto intro_done;
          if (!x3) goto cac_waitforhost;
          if (t->d2[0]!=245) goto cac_waitforhost;//incorrect message!

          //0 neither username or charname is taken
          //1 username is taken
          //2 charname is taken
          if (t->d2[1]==1){
            MessageBox(NULL,"USER NAME taken. Please choose a different USER NAME","Ultima 6 Online",MB_OK);
            goto intro_done;
          }
          if (t->d2[1]==2){
            MessageBox(NULL,"Character name taken. Please choose a different NAME","Ultima 6 Online",MB_OK);
            goto intro_done;
          }


        }//NEThost==NULL


        if (intro201_ep==0){
          GETINPUT_stop();
        }

        //allocate info
        txtset(u6o_user_name,tusername2);
        txtset(u6o_user_password,tuserpassword2);
        txtset(u6o_name,tname2);
        u6o_malefemale=sex;
        u6o_type=typen;
        x3=portn;
        if (sex){
          if (x3==0) x4=194;
          if (x3==1) x4=195;
          if (x3==2) x4=197;
          if (x3==3) x4=198;
          if (x3==4) x4=202;
          if (x3==5) x4=203;
          if (x3==6) x4=207;
        }else{
          if (x3==0) x4=196;
          if (x3==1) x4=199;
          if (x3==2) x4=200;
          if (x3==3) x4=201;
          if (x3==4) x4=204;
          if (x3==5) x4=205;
          if (x3==6) x4=206;
        }
        u6o_portrait=x4-194;
        intro=102; intro_setup=0; goto intro_done; //->gypsy intro
      }

    }
  }//intro==201



intro_done:




  img0(ps,1024-21-2,2,intro_x);


  intro_timer=f;
  goto intro_refresh;


}
{



  //btime2+=et*0.00666667f; //non-rolled over btime!
  /*
  //luteijn: attempt 1, slow progress of time by 4 on the client (9600*framerate)
  if (NEThost) {
  btime2+=et/150.0; //non-rolled over btime!
  } else {
  btime2+=et/600.0; //non-rolled over btime!
  }
  */
  //luteijn: attempt 2, speed progress of time by 4 on the host.
  //would give /37.5 vs /150.0 doesn't look nice, so changed to 32 and 128
  
  if (!NEThost) {
    btime2+=et/150.0f; //non-rolled over btime!
    x=btime2/24;
    btime=btime2-(float)x*24.0f;
    x%=7; bday=x+1;
  }
  if ((btime_last<5.0f)&&(btime>=5.0f)) soundplay2(u6osound[28],u6osound_volume[28]);
  btime_last=btime;


  //set key frame
  i=ett*16.0; i=i&7; keyframe=i;
  i=ett*32.0; i=i&7; keyframe2=i;
  i=ett*4.0; i=i&15; keyframe15=i;
  i=ett*8.0; i=i&31; keyframe31=i; //4sec cycle! (8 frames per sec!)

  keyframe3=(unsigned long)(ett*24.0f)%10;

  if (u6okeyhit(U6OK_QUIT)){ //"Q" request exit
    exitrequest=TRUE;
  }

  if ((NEThost)&&(host_minimize)){host_minimize=FALSE; goto host_minimize_goto;}
  if (u6okeyhit(U6OK_MAXMIN)){ //"M" maximize/minimize/mini_window
    if ((smallwindow==FALSE)&&(nodisplay==FALSE)&&NEThost){
host_minimize_goto:
      nodisplay=TRUE;
      ShowWindow(hWnd2,SW_MINIMIZE);
      for (i=0;i<=65535;i++) keyon[i]=FALSE;
      goto maxminmini;
    }
    if ((smallwindow==FALSE)&&(nodisplay==FALSE)){
      smallwindow=TRUE;
      for (i=0;i<=65535;i++) keyon[i]=FALSE;
      goto maxminmini;
    }
    if (nodisplay==TRUE){
      //nodisplay_maximize:
      nodisplay=FALSE;
      ShowWindow(hWnd3,SW_SHOW);
      smallwindow=TRUE;
      for (i=0;i<=65535;i++) keyon[i]=FALSE;
      goto maxminmini;
    }
    smallwindow=FALSE;
    for (i=0;i<=65535;i++) keyon[i]=FALSE;
  }
maxminmini:


  if (u6okeyhit(U6OK_SOUND)){ //"S" sound on/off
    if (volcontrol->offset_x>=1024){
      volcontrol->offset_x-=2048;
    }else{
      volcontrol->offset_x+=2048;
    }
  }

  if (volcontrol->mouse_over){
    if (u6okeyhit(U6OK_SCROLLUP)){
      if (volcontrol->mouse_y<32){
        if ((u6ovolume+8)>255) u6ovolume=255; else u6ovolume+=8;
      }
      if ((volcontrol->mouse_y>=32)&&(volcontrol->mouse_y<=56)){
        if ((u6omidivolume+8)>255) u6omidivolume=255; else u6omidivolume+=8;
        goto u6omidivolume_changed;
      }
      if (volcontrol->mouse_y>56){
        if ((u6ovoicevolume+8)>255) u6ovoicevolume=255; else u6ovoicevolume+=8;
      }
    }
    if (u6okeyhit(U6OK_SCROLLDOWN)){
      if (volcontrol->mouse_y<32){
        if ((u6ovolume-8)>0) u6ovolume-=8; else u6ovolume=0;
      }
      if ((volcontrol->mouse_y>=32)&&(volcontrol->mouse_y<=56)){
        if ((u6omidivolume-8)>0) u6omidivolume-=8; else u6omidivolume=0;
        goto u6omidivolume_changed;
      }
      if (volcontrol->mouse_y>56){
        if ((u6ovoicevolume-8)>0) u6ovoicevolume-=8; else u6ovoicevolume=0;
      }
    }





  }//mouse_over
  if (volcontrol->mouse_pressed){
    if (volcontrol->mouse_x<46) volcontrol->mouse_x=46;
    if (volcontrol->mouse_x>117) volcontrol->mouse_x=117;
    if (volcontrol->mouse_y<32){
      u6ovolume=(volcontrol->mouse_x-46)*72/20;
    }
    if ((volcontrol->mouse_y>=32)&&(volcontrol->mouse_y<=56)){
      u6omidivolume=(volcontrol->mouse_x-46)*72/20;
u6omidivolume_changed:


      if (U6O_DISABLEMUSIC==FALSE){
        f=u6omidi_volume[midi_loaded];
        f=f*(float)u6omidivolume/255.0f;
        f=255-f; f=f*0.25f; f*=f;
        //DMUS_VOLUME_MAX     2000        /* +20 dB */
        //DMUS_VOLUME_MIN   -20000        /* -200 dB */
        u6omidi->SetMasterVolume(-f);
        if (u6omidivolume==0) u6omidi->Stop();

        if (midiout_setup){
          x=u6omidivolume/2;//change 0-255 to 0-127
          midiOutShortMsg(midiout_handle,0x000007B0+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B1+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B2+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B3+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B4+x*65536); //set volume
        }

      }

    }
    if (volcontrol->mouse_y>56){
      u6ovoicevolume=(volcontrol->mouse_x-46)*72/20;
    }
    volcontrol->mouse_pressed=0;
  }


  if (mess_UPDATEps==NULL) mess_UPDATEps=txtnew();
  if (mess_SF==NULL) mess_SF=txtnew();
  if (mess1==NULL) mess1=txtnew();
  if (CLIENTplayer==NULL) CLIENTplayer=tplay;

  //CLIENT2HOST CONNECTION MESSAGE
  if (setup_message==FALSE){
    setup_message=TRUE;
    txtset(t,"?"); t->d2[0]=255; //setup
    txtset(t2,"??"); t2->ds[0]=5+(2<<8); txtadd(t,t2); //version 5.2
    txtset(t2,"?"); t2->d2[0]=u6o_name->l; txtadd(t,t2); txtadd(t,u6o_name); //name
    txtset(t2,"?"); t2->d2[0]=u6o_malefemale; txtadd(t,t2); //male/female
    txtset(t2,"??"); t2->ds[0]=u6o_portrait; txtadd(t,t2); //portrait
    txtset(t2,"?"); t2->d2[0]=u6o_type; txtadd(t,t2); //type (avatar, fighter,)
    txtNEWLEN(t2,28); for (i=0;i<=27;i++){t2->d2[i]=u6o_vq[i]; }
    txtadd(t,t2); //virtue answers
    txtset(t2,"?"); t2->d2[0]=u6o_user_name->l; txtadd(t,t2); txtadd(t,u6o_user_name); //user_name
    txtset(t2,"?"); t2->d2[0]=u6o_user_password->l; txtadd(t,t2); txtadd(t,u6o_user_password); //user_password
    txtset(t2,"?"); t2->d2[0]=u6o_createcharacter; txtadd(t,t2);
    txtset(t2,"?"); t2->d2[0]=u6o_new_user_password->l; txtadd(t,t2); txtadd(t,u6o_new_user_password); //user_password
    //...
    if (customportrait_upload){
      txtset(t2,"?"); t2->d2[0]=1; txtadd(t,t2);//extra info: upload character portrait
      txtNEWLEN(t2,3584*2); memcpy(&t2->d2[0],&customportrait,3584*2); txtadd(t,t2);
    }//customportrait_upload

    if (u6o_namecolour){
      txtaddchar(t,2);
      txtaddshort(t,u6o_namecolour&65535); txtaddchar(t,(u6o_namecolour>>16)&255);
    }//u6o_namecolour

#ifdef SECURE
#include "secret_a.inc"
#endif

    NET_send(NETplayer,NULL,t);
  }

  if (sysban){
    sysban+=et;
    if (sysban>=8.0f){
      MessageBox(NULL,"Leaving Britannia (Due to serious offenses administration no longer allows you to play u6o)","Ultima 6 Online",MB_OK);
      DestroyWindow(hWnd);
      sysban=0.0f;
    }
  }





  //assign mouse cursor
  if ((vf_mb2_x==0xFFFF)&&(vf_mb2_y==0xFFFF)){
    if (cur_type==1) SetCursor (cur1);
  }
  if (cur_type==2) SetCursor (cur2);
  if (cur_type==3){
    if (CLIENTplayer->mobj) SetCursor (cur7); else SetCursor (cur3);
  }
  if (cur_type==4) SetCursor (cur4);
  if (cur_type==5) SetCursor (cur5);
  if (cur_type==6) SetCursor (cur6);
  if (cur_type==9) SetCursor (cur9);

  //set mouse cursor type (based on previous loops state)
  cur_type=1;
  if (inprec) cur_type=2;
  if (userkey==1) cur_type=3;
  if (userkey==2) cur_type=4;
  if (userkey==3) cur_type=5;
  if (userkey==4) cur_type=6;
  if (userkey==5) cur_type=9;

  if (u6okeyhit(U6OK_CANCEL)){ //"ESC" cancel
userkey_cancel:
    userkey=0;
    txtNEWLEN(inpf2->input,0);
    inpf2->enterpressed=NULL;
    GETINPUT_txt=NULL;
    if (inpf->offset_x<1024) inpf->offset_x+=2048;
    if (inprec){
      txtset(t,"?"); t->d2[0]=20; NET_send(NETplayer,NULL,t); //typing... cancel
    }
    inprec=0;
  }



  if (autoscroll){
    autoscroll-=et;
    if (autoscroll<0.0f){
      autoscroll=0.0f;
      textdisplayi=0xFFFFFFF;
      textdisplayupdate=1;
      goto updatetextdisplay; //->
    }
  }

  if (con_frm->mouse_over==TRUE){
    if (textdisplayi!=-1){
      if (u6okeyhit(U6OK_SCROLLUP)){
        if (textdisplayi>0){
          textdisplayi--;
          textdisplayupdate=1;
          autoscroll=16.0f;
          goto updatetextdisplay; //->
        }
      }
      if (u6okeyhit(U6OK_SCROLLDOWN)){
        textdisplayi++;
        textdisplayupdate=1;
        autoscroll=16.0f;
        goto updatetextdisplay; //->
      }
    }//i!=-1
  }//con_frm->mouse_over==TRUE

  //text display up/down buttons
  if (con_frm->mouse_click){
    if (textdisplayi!=-1){
      if (con_frm->mouse_x>=17){
        textdisplayi++;
      }else{
        if (textdisplayi>0) textdisplayi--;
      }
      textdisplayupdate=1;
      autoscroll=16.0f;
      goto updatetextdisplay; //->
updatetextdisplay2: //<-
      textdisplayupdate=0;
    }
    con_frm->mouse_click=FALSE;
  }

  //CLIENT2HOST MESSAGES (ALLOWED EVEN IF DEAD)
  //global messages
  //send text message to NPC/player

  if (u6okeyhit(U6OK_RESPAWNFOLLOWERS)){
    txtsetchar(t,248); txtaddchar(t,254); NET_send(NETplayer,NULL,t);
  }

  if (u6okeyhit(U6OK_RESPAWN)){
    txtsetchar(t,248); NET_send(NETplayer,NULL,t);
  }

  if (inprec){
    if (inpf->mouse_over||vf->mouse_over){
      if (u6okeyhit(U6OK_SCROLLDOWN)){
        inpmess_selected++;
        static inpmess_index *inpmess_tmpi;
        inpmess_tmpi=inpmess_mostrecent;
        if (inpmess_tmpi->next==NULL) {inpmess_selected=-1; goto inpf_scroll_failed;}
        for (i=0;i<inpmess_selected;i++){
          if (inpmess_tmpi->next){
            if (inpmess_tmpi->next->next) inpmess_tmpi=inpmess_tmpi->next; else inpmess_selected=i-1;
          }
        }//i
        txtset(inpf2->input,inpmess_tmpi->t);
      }
      if (u6okeyhit(U6OK_SCROLLUP)){
        if (inpmess_selected>=1) inpmess_selected--; else inpmess_selected=0;
        static inpmess_index *inpmess_tmpi;
        inpmess_tmpi=inpmess_mostrecent;
        if (inpmess_tmpi->next==NULL) {inpmess_selected=-1; goto inpf_scroll_failed;}
        for (i=0;i<inpmess_selected;i++){
          if (inpmess_tmpi->next) inpmess_tmpi=inpmess_tmpi->next;
        }//i
        txtset(inpf2->input,inpmess_tmpi->t);
      }
    }//mouse_over
inpf_scroll_failed:
    if (u6okeyhit(U6OK_RETYPE)){
      txtadd(inpf2->input,inpmess_mostrecent->t);
    }
    if (CLIENTplayer->mf==255){
      if (CLIENTplayer->key&KEYmbclick){
        CLIENTplayer->key-=KEYmbclick;
        txtset(t,inpf2->input);
        txtmakeu6ocompatible(t);
        txtset(inpmess,t);
        if (t->l){
          if (!txtsame(inpmess_mostrecent->t,t)){
            static inpmess_index* inpmess_index_new;
            inpmess_index_new=(inpmess_index*)malloc(sizeof(inpmess_index)); inpmess_index_new->next=inpmess_mostrecent;
            txtset(inpmess_index_new->t=txtnew(),inpmess); inpmess_mostrecent=inpmess_index_new;
          }
        }
        inpmess_selected=-1;
        txtNEWLEN(inpf2->input,0);
        inpf2->enterpressed=NULL;
        if (inpf->offset_x<1024) inpf->offset_x+=2048;
        inprec=0;
        GETINPUT_txt=NULL;
        txtset(t2,"?"); t2->d2[0]=7; //message type 7
        txtset(t3,"????");
        t3->ds[0]=CLIENTplayer->mx;
        t3->ds[1]=CLIENTplayer->my;
        txtadd(t2,t3);
        txtadd(t2,inpmess);
        NET_send(NETplayer,NULL,t2);
        txtNEWLEN(inpmess,0);
      } //KEYmbclick
    } //mf==255
  }else{ //inprec
    u6okeyhit(U6OK_RETYPE); //dump the keyhit
  }



  //VOICE CHAT 1.0: DETECT AUDIO OUTPUT LEVEL
  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){



    //MIXER: variables

    static unsigned long MIXER_mastervolume;
    static unsigned long MIXER_wavevolume;
    static unsigned char hMixer_opened=FALSE;
    HMIXER hMixer=NULL;//handle to the mixer
    MIXERLINE ml;
    MIXERLINECONTROLS mlc;
    MIXERCONTROL mc;
    MIXERCONTROLDETAILS mcd;
    MIXERCONTROLDETAILS_UNSIGNED mcdu;

    //MIXER: open
    if (hMixer_opened==FALSE){ hMixer_opened=TRUE;
    mixerOpen(&hMixer, MIXER_OBJECTF_MIXER, 0, 0, 0);
    }

    //MIXER: get master volume line
    ZeroMemory(&ml,sizeof(ml));
    ml.cbStruct = sizeof(MIXERLINE);
    ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
    mixerGetLineInfo((HMIXEROBJ)hMixer,&ml,MIXER_GETLINEINFOF_COMPONENTTYPE);
    //MIXER: get MIXERCONTROL_CONTROLTYPE_VOLUME of master volume line
    ZeroMemory(&mlc,sizeof(mlc));
    ZeroMemory(&mc,sizeof(mc));
    mlc.cbStruct = sizeof(MIXERLINECONTROLS);
    mlc.dwLineID = ml.dwLineID;
    mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
    mlc.cControls = 1;
    mlc.pamxctrl = &mc;
    mlc.cbmxctrl = sizeof(MIXERCONTROL);
    mixerGetLineControls((HMIXEROBJ)hMixer,&mlc,MIXER_GETLINECONTROLSF_ONEBYTYPE);
    //MIXER: get volume level of master volume
    ZeroMemory(&mcd,sizeof(mcd));
    ZeroMemory(&mcdu,sizeof(mcdu));
    mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mcd.hwndOwner = 0;
    mcd.dwControlID = mc.dwControlID;
    mcd.paDetails = &mcdu;
    mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    mcd.cChannels = 1;
    mixerGetControlDetails((HMIXEROBJ)hMixer,&mcd,MIXER_SETCONTROLDETAILSF_VALUE);
    MIXER_mastervolume=mcdu.dwValue/256;

    //MIXER: get wave volume line
    ZeroMemory(&ml,sizeof(ml));
    ml.cbStruct = sizeof(MIXERLINE);
    ml.Target.dwType= MIXERLINE_TARGETTYPE_WAVEOUT;
    mixerGetLineInfo((HMIXEROBJ) hMixer,&ml,MIXER_GETLINEINFOF_TARGETTYPE);
    //MIXER: get MIXERCONTROL_CONTROLTYPE_VOLUME of wave volume line
    ZeroMemory(&mlc,sizeof(mlc));
    ZeroMemory(&mc,sizeof(mc));
    mlc.cbStruct = sizeof(MIXERLINECONTROLS);
    mlc.dwLineID = ml.dwLineID;
    mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
    mlc.cControls = 1;
    mlc.pamxctrl = &mc;
    mlc.cbmxctrl = sizeof(MIXERCONTROL);
    mixerGetLineControls((HMIXEROBJ)hMixer,&mlc,MIXER_GETLINECONTROLSF_ONEBYTYPE);
    //MIXER: get volume level of wave volume
    ZeroMemory(&mcd,sizeof(mcd));
    ZeroMemory(&mcdu,sizeof(mcdu));
    mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mcd.hwndOwner = 0;
    mcd.dwControlID = mc.dwControlID;
    mcd.paDetails = &mcdu;
    mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    mcd.cChannels = 1;
    mixerGetControlDetails((HMIXEROBJ)hMixer,&mcd,MIXER_SETCONTROLDETAILSF_VALUE);
    MIXER_wavevolume=mcdu.dwValue/256;







    x2=MIXER_wavevolume*MIXER_mastervolume*u6ovoicevolume;
    x2/=65536;
    if (x2!=mixer_volume){
      mixer_volume=x2;
      txtset(t2,"??"); t2->d2[0]=39;//update mixer volume
      t2->d2[1]=mixer_volume;


      NET_send(NETplayer,NULL,t2);


    }

  }//every ? seconds

  voicechat_mciwait-=et; if (voicechat_mciwait<0.0f) voicechat_mciwait=0.0f;
  voicechat_recordtime+=et;

  if (voicechat_recording){
    if (voicechat_recordtime<3.0f){
      f=voicechat_recordtime*10;
      x2=f;
      for (x3=1;x3<=x2;x3++){
        img(voicechat_frame->graphic,3+x3*13-13,3,voicechat_voice1);
      }
    }else{
      img(voicechat_frame->graphic,0,0,voicechat_voiceof);
    }
  }

  //VOICE CHAT 1.0+
  if (u6okeyon(U6OK_VOICECHAT)){
    if (voicechat_recording==0){
      if (playinstrument) goto voicechat_permissionrequestfinished;

      if (voicechat_permissionrequested==FALSE){
        voicechat_permissionrequested=TRUE;
        tfh=open2(".\\voice\\allow.bin",OF_READWRITE);
        if (tfh->h==HFILE_ERROR){
          if (IDYES==MessageBox(NULL,"Voice chat sends input from your microphone to other players when you hold this key. Do you allow U6O to use this feature?","Ultima 6 Online",MB_YESNO)){
            voicechat_permission=TRUE;
            if (IDYES==MessageBox(NULL,"Would you like to always start with voice chat allowed?","Ultima 6 Online",MB_YESNO)){
              tfh=open2(".\\voice\\allow.bin",OF_READWRITE|OF_CREATE); close(tfh);
            }
          }else{
            voicechat_permissionrequested=FALSE;
          }
          u6okeysetoff(U6OK_VOICECHAT);
          goto voicechat_permissionrequestfinished;
        }else{
          close(tfh);
          voicechat_permission=TRUE;
        }
      }//voicechat_permissionrequested==FALSE










      //begin recording
      voicechat_recording=1;
      voicechat_recordtime=0.0f;
      if (voicechat_frame->offset_x>=1024) voicechat_frame->offset_x-=2048;
      img(voicechat_frame->graphic,0,0,voicechat_voicebar);

      voicechat_listeningplayers=FALSE;//must be validated by returned message
      voicechat_listeningplayeri=0;
      txtset(t2,"?"); t2->d2[0]=40;//request voice volume levels of others(40)
      NET_send(NETplayer,NULL,t2);
      //returned by ID!



      dwMilliSeconds=3000;



      mciOpenParms.lpstrDeviceType = "waveaudio";
      mciOpenParms.lpstrElementName = "";




      if(mciSendCommand(0,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_OPEN_TYPE|MCI_WAIT,(DWORD)(LPVOID)&mciOpenParms)) exit(7);
      wDeviceID = mciOpenParms.wDeviceID;


      /*
      MCI_WAVE_SET_PARMS mwspWaveFormParameters;
      mwspWaveFormParameters.ulSamplesPerSec = 11025;
      mwspWaveFormParameters.usBitsPerSample = 8;
      mciSendCommand (SoundDevice.usSoundDeviceID,
      MCI_SET,                      
      MCI_WAIT,                      
      MCI_WAVE_SET_SAMPLESPERSEC,
      MCI_WAVE_SET_BITSPERSAMPLE,
      (PVOID)&mwspWaveFormParameters, 
      0);                             
      */

      /*
      MCI_WAVE_SET_PARMS mwspWaveFormParameters;
      ZeroMemory(&mwspWaveFormParameters,sizeof(mwspWaveFormParameters));
      mwspWaveFormParameters.nSamplesPerSec = 44100;
      mwspWaveFormParameters.wBitsPerSample = 16;
      if(x3=mciSendCommand (wDeviceID,MCI_SET,MCI_WAVE_SET_BITSPERSAMPLE|MCI_WAVE_SET_SAMPLESPERSEC,(DWORD)(LPVOID)&mwspWaveFormParameters)) return x3;
      */

      mciRecordParms.dwTo = dwMilliSeconds;
      if(mciSendCommand(wDeviceID,MCI_RECORD,MCI_TO,(DWORD)(LPVOID)&mciRecordParms)) exit(8);




    }
  }



  //finish recording?
  if (voicechat_recording){
    if (u6okeyon(U6OK_VOICECHAT)==FALSE){
      voicechat_recording=0;
      if (voicechat_frame->offset_x<1024) voicechat_frame->offset_x+=2048;
      if (voicechat_recordtime>3.0f){//recording has exceeded maximum allowable time!
        if(mciSendCommand(wDeviceID,MCI_CLOSE,MCI_WAIT,NULL)) exit(6);

        STATUSMESSadd("Voice chat is limited to 3 seconds!");
        goto getvoice_timeexceeded;
      }
      mciSendCommand(wDeviceID,MCI_PAUSE,MCI_WAIT,NULL);
      deletefile(".\\voice\\voice.wav");

      mciSaveParms.lpfilename = ".\\voice\\voice.wav";
      if(mciSendCommand(wDeviceID, MCI_SAVE,MCI_SAVE_FILE|MCI_WAIT,(DWORD)(LPVOID)&mciSaveParms)) exit(10);
      if(mciSendCommand(wDeviceID,MCI_CLOSE,MCI_WAIT,NULL)) exit(6);

      waitforfile(".\\voice\\voice.wav");

      deletefile(".\\voice\\voice.spx");
      PROCESS_INFORMATION pi;
      STARTUPINFO si;
      ZeroMemory(&pi,sizeof(pi)); ZeroMemory(&si,sizeof(si));
      si.cb=sizeof(STARTUPINFO);
      si.dwFlags=STARTF_USESIZE|STARTF_USESHOWWINDOW;
      si.wShowWindow=SW_HIDE;
      CreateProcess(NULL,".\\voice\\speexenc.exe -n --bitrate 8192 .\\voice\\voice.wav .\\voice\\voice.spx",NULL,NULL,FALSE, 0, NULL, NULL, &si, &pi);
      waitforfile(".\\voice\\voice.spx");

      //get buffered sound
      tfh=open(".\\voice\\voice.spx");
      txtNEWLEN(t2,lof(tfh));
      get(tfh,t2->d,lof(tfh));
      close(tfh);

      if (voicechat_listeningplayers){
        txtsetchar(t3,37);//send voice chat message (local)
        if (voicechat_listeningplayers&2) txtadd(t3,t2);
        NET_send(NETplayer,NULL,t3);
      }


      txtset(t4,""); tplay->GNPCflags[245]=namecolour;
      //simulate message as a local
      if (tplay->party[0]){
        tnpc=(npc*)tplay->party[0]->more;
        txtsetshort(t,tnpc->port);
        txtset(t5,tnpc->name); txtadd(t5,":");
        txtaddcolouredname(t4,t5,tplay);
        txtaddchar(t,t4->l);
        txtadd(t,t4);
      }else{
        txtsetshort(t,portlast);
        txtset(t5,namelast); txtadd(t5,":");
        txtaddcolouredname(t4,t5,tplay);
        txtaddchar(t,t4->l);
        txtadd(t,t4);
      }
      txtaddshort(t,tplay->x); txtaddshort(t,tplay->y);
      txtadd(t,t2);
      localvoicemessage_return=1;
      goto localvoicemessage_begin;
    }
  }
localvoicemessage_returnpoint:
getvoice_timeexceeded:
voicechat_permissionrequestfinished:

  if (tplay->party[0]==NULL){
    if (u6okeyhit(U6OK_TALK)){ //"T" talk
      talkprev=0;
      if (inprec==0){
        deadglobalmessage=1; goto dglobal;
      }
    }
    if (inpf2->enterpressed){
      deadglobalmessage=1; goto dglobal2; //process receiving input
    }
  }//tplay->party[0]==NULL
deadglobalmessage_return:

  if (inpmess->l){ //send input message to the host
    txtset(t2,"?"); t2->d2[0]=6; 
    if (inprec_global){t2->d2[0]=12; client_globalwait=0;}//global message
    if (talkprev) t2->d2[0]=34;
    txtadd(t2,inpmess);
    NET_send(NETplayer,NULL,t2);
    txtNEWLEN(inpmess,0);
  }

  f=client_globalwait;
  client_globalwait+=et;
  if ((f<32)&&(client_globalwait>=32)){
    STATUSMESSadd("Ready to send next global message.");
  }

  //CLIENT2HOST MESSAGES (SKIPPED WHEN DEAD)
  if (tplay->party[0]){


    x=0;
    if (u6okeyhit(U6OK_TALK_GLOBAL)){x=2; talkprev=0;}
    if (u6okeyhit(U6OK_TALKPREV)){x=1; talkprev=1;}
    if (u6okeyhit(U6OK_TALK)){x=1; talkprev=0;}
    if (x){ //"T" talk
      if (inprec==0){
        if (x==2){
dglobal:
          if (client_globalwait<32){
            STATUSMESSadd("\x0FFWait 30 seconds between sending global messages!");
            goto skipglobalmessage;
          }
          inprec_global=1; shiftnum_show=0;
        }else inprec_global=0;
        txtset(t,"?"); t->d2[0]=19; NET_send(NETplayer,NULL,t); //typing... message
        userkey=0;
        inpf2->enterpressed=FALSE;
        GETINPUT_setup(inpf2->input,&inpf2->enterpressed,inpf2->length_limit);
        if (inpf->offset_x>=1024) inpf->offset_x-=2048;

        if (tplay->party[0]){
          tnpc=(npc*)tplay->party[0]->more;
          inpf->graphic=getportrait(tnpc->port);
        }else inpf->graphic=getportrait(portlast);

        if (inprec_global){
          inpft->col=rgb(96,128,255);
          inpf2->col=rgb(96,128,255);
        }else{
          inpft->col=rgb(255,255,255);
          inpf2->col=rgb(255,255,255);
        }
        inprec=1;
      }
    }
skipglobalmessage:
    if (inpf2->enterpressed){
dglobal2:
      txtset(t,inpf2->input);
      txtmakeu6ocompatible(t);

      txtset(t2,"/NOCLOUDS");
      if (txtsamewithoutcase(t,t2)){
        noclouds=TRUE;
        txtset(STATUSMESSt,"Clouds disabled."); STATUSMESSadd(STATUSMESSt);
        goto inpmess_skip;
      }

      txtset(t2,"/CLOUDS");
      if (txtsamewithoutcase(t,t2)){
        noclouds=FALSE;
        txtset(STATUSMESSt,"Clouds enabled."); STATUSMESSadd(STATUSMESSt);
        goto inpmess_skip;
      }


      i4=0;
      txtset(t3,t); txtset(t9,t3); txtucase(t3);
      txtset(t2,"/IGNORE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-8); txtright(t3,t3->l-8); goto ignoreremove;
      }
      txtset(t2,"/UNIGNORE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-10); txtright(t3,t3->l-10); goto ignoreremove;
      }
      txtset(t2,"/MUTE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-6); txtright(t3,t3->l-6); goto ignoreremove;
      }
      txtset(t2,"/UNMUTE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-8); txtright(t3,t3->l-8); goto ignoreremove;
      }
      goto skipignoreremove;
ignoreremove:
      tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
ignoreremoveplayer:
      i=seek(tfh);
      txtfilein(t6,tfh); txtucase(t6);

      txtset(t2,"[MUTE]"); if (txtsearch(t6,t2)==1){
        txtadd(t2,t3); if (txtsame(t6,t2)){
          i2=seek(tfh);
          for (i3=0;i3<t2->l;i3++) t2->d2[i3]=42;
          seek(tfh,i); put(tfh,t2->d,t2->l);
          seek(tfh,i2);

          txtset(STATUSMESSt,t9); txtadd(STATUSMESSt," unmuted!"); STATUSMESSadd(STATUSMESSt);
        }
      }
      txtset(t2,"[IGNORE]"); if (txtsearch(t6,t2)==1){
        txtadd(t2,t3); if (txtsame(t6,t2)){
          i2=seek(tfh);
          for (i3=0;i3<t2->l;i3++) t2->d2[i3]=42;
          seek(tfh,i); put(tfh,t2->d,t2->l);
          seek(tfh,i2);

          txtset(STATUSMESSt,t9); txtadd(STATUSMESSt," unignored!"); STATUSMESSadd(STATUSMESSt);
        }
      }
      if (t6->l) goto ignoreremoveplayer;
      close(tfh);
      i4=1;
skipignoreremove:
      //client commands
      txtset(t2,"/IGNORE "); txtset(t3,t); txtucase(t3); if (txtsearch(t3,t2)==1){
        txtright(t3,t3->l-8);
        txtset(t2,"[IGNORE]"); txtadd(t2,t3);
        tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
        seek(tfh,lof(tfh));
        txtfileout(t2,tfh);
        close(tfh);
        txtright(t,t->l-8); txtadd(t," ignored!");

        STATUSMESSadd(t);
        goto inpmess_skip;
      }
      txtset(t2,"/MUTE "); txtset(t3,t); txtucase(t3); if (txtsearch(t3,t2)==1){
        txtright(t3,t3->l-6);
        txtset(t2,"[MUTE]"); txtadd(t2,t3);
        tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
        seek(tfh,lof(tfh));
        txtfileout(t2,tfh);
        close(tfh);
        txtright(t,t->l-6); txtadd(t," muted!");

        STATUSMESSadd(t);
        goto inpmess_skip;
      }
      if (i4) goto inpmess_skip;





      txtset(t2,"load wavinfo.txt"); if (txtsame(t,t2)){

        STATUSMESSadd("loaded wavinfo.txt");
        wavinfo_loaded=FALSE; goto inpmess_skip;
      }
      txtset(t2,"load midiinfo.txt"); if (txtsame(t,t2)){

        STATUSMESSadd("loaded midiinfo.txt");
        midiinfo_loaded=FALSE; goto inpmess_skip;
      }
      txtset(t2,"set musickeyboard"); if (txtsame(t,t2)){
        midikeyboard_set=24;
        mididown(clientinstrument,midikeyboard_set);
        goto inpmess_skip;
      }
      txtset(inpmess,t);
      if (inpmess->l){
        if (!txtsame(inpmess_mostrecent->t,t)){
          static inpmess_index* inpmess_index_new;
          inpmess_index_new=(inpmess_index*)malloc(sizeof(inpmess_index)); inpmess_index_new->next=inpmess_mostrecent;
          txtset(inpmess_index_new->t=txtnew(),inpmess); inpmess_mostrecent=inpmess_index_new;
        }
        inpmess_selected=-1;
        if (U6O_WALKTHRU_REC) txtfileout(inpmess,walkthru_fh);
      }else{
inpmess_skip: txtset(t,"?"); t->d2[0]=20; NET_send(NETplayer,NULL,t); //typing... cancel
      }
      txtNEWLEN(inpf2->input,0);
      inpf2->enterpressed=NULL;
      if (inpf->offset_x<1024) inpf->offset_x+=2048;
      inprec=0;
    }
    if (deadglobalmessage) {deadglobalmessage=0; goto deadglobalmessage_return;}

    CLIENTplayer->key2=CLIENTplayer->key; //backup old key info
    CLIENTplayer->key=0; //NULL key info

    CLIENTplayer->mf=255; //assume default: primary surface
    CLIENTplayer->mx=vf->mouse_x;
    CLIENTplayer->my=vf->mouse_y;
    if (vf->mouse_click==TRUE) CLIENTplayer->key|=KEYmbclick;
    for (i=0;i<=7;i++){
      pmf=party_frame[i];
      if (pmf->mouse_over==TRUE){
        CLIENTplayer->mf=i;
        CLIENTplayer->mx=pmf->mouse_x;
        CLIENTplayer->my=pmf->mouse_y;
        if (pmf->mouse_click==TRUE) CLIENTplayer->key|=KEYmbclick;
        if (u6okeyhit(U6OK_SCROLLUP)){
          CLIENTplayer->mx=118;
          CLIENTplayer->my=202;
          CLIENTplayer->key|=KEYmbclick;
          if (CLIENTplayer->key2&KEYmbclick) CLIENTplayer->key2-=KEYmbclick;
        }
        if (u6okeyhit(U6OK_SCROLLDOWN)){
          CLIENTplayer->mx=118;
          CLIENTplayer->my=240;
          CLIENTplayer->key|=KEYmbclick;
          if (CLIENTplayer->key2&KEYmbclick) CLIENTplayer->key2-=KEYmbclick;
        }
      }//pmf->mouse_over==TRUE
    }//i
    //turn spellbook pages
    for (i=0;i<=7;i++){
      pmf=party_spellbook_frame[i];
      if (pmf->mouse_over==TRUE){
        if (u6okeyhit(U6OK_SCROLLUP)){
          if (spellbook_flags[i]&1) goto turnspellpagel;
        }
        if (u6okeyhit(U6OK_SCROLLDOWN)){
          if (spellbook_flags[i]&2) goto turnspellpager;
        }
      }
      if (spellbook_flags[i]&1){
        if (pmf->mouse_click){
          if (pmf->mouse_x<(33+32)){
            if (pmf->mouse_y<32){
turnspellpagel:
              spellbook_page[i]--;
              i4=0;
              for (i5=0;i5<=15;i5++){
                if (spell[i][((spellbook_page[i]-1)<<4)+i5]) i4=1;
              }
              if (i4==0) goto turnspellpagel;
              tnpc=(npc*)tplay->party[i]->more; tnpc->upflags|=128;
            }}
        }
      }
      if (spellbook_flags[i]&2){
        if (pmf->mouse_click){
          if (pmf->mouse_x>=(212-32)){
            if (pmf->mouse_y<32){
turnspellpager:
              spellbook_page[i]++;
              i4=0;
              for (i5=0;i5<=15;i5++){
                if (spell[i][((spellbook_page[i]-1)<<4)+i5]) i4=1;
              }
              if (i4==0) goto turnspellpager;
              tnpc=(npc*)tplay->party[i]->more; tnpc->upflags|=128;
            }}
        }
      }
    }//i













    tnpc=(npc*)tplay->party[0]->more;


    if (tshiftnum==NULL) tshiftnum=txtnew();
    if (u6okeyon(U6OK_QUANTITY)){
      x=tshiftnum->l;
      if (u6okeyhit(U6OK_QUANTITY0)&&(tshiftnum->l!=0)) txtadd(tshiftnum,"0");
      if (u6okeyhit(U6OK_QUANTITY1)) txtadd(tshiftnum,"1");
      if (u6okeyhit(U6OK_QUANTITY2)) txtadd(tshiftnum,"2");
      if (u6okeyhit(U6OK_QUANTITY3)) txtadd(tshiftnum,"3");
      if (u6okeyhit(U6OK_QUANTITY4)) txtadd(tshiftnum,"4");
      if (u6okeyhit(U6OK_QUANTITY5)) txtadd(tshiftnum,"5");
      if (u6okeyhit(U6OK_QUANTITY6)) txtadd(tshiftnum,"6");
      if (u6okeyhit(U6OK_QUANTITY7)) txtadd(tshiftnum,"7");
      if (u6okeyhit(U6OK_QUANTITY8)) txtadd(tshiftnum,"8");
      if (u6okeyhit(U6OK_QUANTITY9)) txtadd(tshiftnum,"9");
      if (tshiftnum->l>5) txtleft(tshiftnum,5);
      if (x!=tshiftnum->l){
        shiftnum_show=1;
        x2=txtnum(tshiftnum);
        if (x2>65535) x2=65535;
        txtset(t,"?"); t->d2[0]=13; //update QUANtity value
        txtset(t2,"??"); t2->ds[0]=x2;
        txtadd(t,t2);
        NET_send(NETplayer,NULL,t);
      }//change made
    }else{
      shiftnum_show=0; txtset(tshiftnum,"");
    }


    if (u6okeyhit(U6OK_NOT4SALE)){ //"X" not4sale
      userkey=5;
      if (U6OK_FLAGS[U6OK_NOT4SALE]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click
    }

    if (u6okeyhit(U6OK_USE)){ //"U" use
      userkey=1;
      if (U6OK_FLAGS[U6OK_USE]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click
      goto skipaltuse;
    }//use
    if (u6okeyhit(U6OK_USE2)){ //"U" alt use
      userkey=1;
      if (U6OK_FLAGS[U6OK_USE2]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click
    }//use
skipaltuse:


    if (u6okeyhit(U6OK_LOOK)){ //"L" look
      userkey=2;
      if (U6OK_FLAGS[U6OK_LOOK]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click
      goto skipaltlook;
    }
    if (u6okeyhit(U6OK_LOOK2)){ //"L" alt look
      userkey=2;
      if (U6OK_FLAGS[U6OK_LOOK2]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click
    }
skipaltlook:




    if (u6okeyhit(U6OK_ATTACK)){ //"A" attack
      if (U6OK_FLAGS[U6OK_ATTACK]&1){//instant click: simulate mouse click on current surface
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        userkey=3;
        CLIENTplayer->key|=KEYmbclick;
        goto u6okattackdone;
      }
      if (userkey!=3){
        txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
        ktar_display=2.0f;
        userkey=3;
      }else{
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        goto userkey_cancel;
      }
      goto u6okattackdone;
    }
    if (u6okeyhit(U6OK_ATTACK2)){ //"A" alt attack
      if (U6OK_FLAGS[U6OK_ATTACK2]&1){//instant click: simulate mouse click on current surface
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        userkey=3;
        CLIENTplayer->key|=KEYmbclick;
        goto u6okattackdone;
      }
      if (userkey!=3){
        txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
        ktar_display=2.0f;
        userkey=3;
      }else{
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        goto userkey_cancel;
      }
      goto u6okattackdone;
    }
u6okattackdone:

    //mouse movement
    {
      static unsigned char directionalmove_mbheld=0;
      static unsigned char pathmove_mbheld=0;
      static float pathmove_wait=0;
      if ((mbheld&2)==0){
        directionalmove_mbheld=0; pathmove_mbheld=0;
      }
      if ((mbheld&2)||(mbclick&2)){
        if ((vf_mb2_x!=0xFFFF)&&(vf_mb2_y!=0xFFFF)){ //right mouse pressed on ps
          //x2,y2 pixel at middle of selected player sprite on screen
          x2=(ctpx2-ctpx)*32; y2=(ctpy2-ctpy)*32;
          x2=vf_mb2_x-(x2+16); y2=vf_mb2_y-(y2+16);
          x3=abs(x2); y3=abs(y2);
          if (directionalmove_only||wizardeyetimeleft) goto directionalmove_force;
          if (directionalmove_mbheld) goto directionalmove_force;
          if (pathmove_mbheld) goto pathmove_force;
          if ((x3>48)||(y3>48)){ //path finding range
pathmove_force:
            pathmove_mbheld=1;
            if (cur_type==1) SetCursor(cur8);
            if (omb&2){ 
              pathmove_wait+=et;
              if (pathmove_wait<=0.25f) goto mousemove_finish;
            }
            pathmove_wait=0;
            //send pathfinding message
            txtset(t,"?"); t->d2[0]=21; //client update message
            x=ctpx+vf_mb2_x/32; y=ctpy+vf_mb2_y/32;
            txtset(t2,"????"); t2->ds[0]=x; t2->ds[1]=y;
            txtadd(t,t2);
            NET_send(NETplayer,NULL,t);
            goto mousemove_finish;
          }
directionalmove_force:
          directionalmove_mbheld=1;
          x=0; y=0;
          if (x3>=y3) x=1;
          if (y3>=x3) y=1;
          if (y3>=(x3/2)){
            if (x3>=(y3/2)){
              x=1;
              y=1;
            }
          }
          if (x2<0) x=-x;
          if (y2<0) y=-y;
          if (x==1) CLIENTplayer->key|=KEYright;
          if (x==-1) CLIENTplayer->key|=KEYleft;
          if (y==1) CLIENTplayer->key|=KEYdown;
          if (y==-1) CLIENTplayer->key|=KEYup;
          if ((x==1)&&(y==0)) if (cur_type==1) SetCursor (cur_r);
          if ((x==1)&&(y==1)) if (cur_type==1) SetCursor (cur_rd);
          if ((x==0)&&(y==1)) if (cur_type==1) SetCursor (cur_d);
          if ((x==-1)&&(y==1)) if (cur_type==1) SetCursor (cur_ld);
          if ((x==-1)&&(y==0)) if (cur_type==1) SetCursor (cur_l);
          if ((x==-1)&&(y==-1)) if (cur_type==1) SetCursor (cur_lu);
          if ((x==0)&&(y==-1)) if (cur_type==1) SetCursor (cur_u);
          if ((x==1)&&(y==-1)) if (cur_type==1) SetCursor (cur_ru);
          if ((x==0)&&(y==0)) if (cur_type==1) SetCursor(cur1);
        }
      }
    }
mousemove_finish:
    vf_mb2_x=0xFFFF; vf_mb2_y=0xFFFF;


    if (u6okeyhit(U6OK_RIGHT)){CLIENTplayer->key|=KEYright2;
    }else{
      if (u6okeyon(U6OK_RIGHT)){
        CLIENTplayer->key|=KEYright;
      }

    }

    if (u6okeyhit(U6OK_LEFT)){CLIENTplayer->key|=KEYleft2;
    }else{
      if (u6okeyon(U6OK_LEFT)) CLIENTplayer->key|=KEYleft;
    }
    if (u6okeyhit(U6OK_UP)){CLIENTplayer->key|=KEYup2;
    }else{
      if (u6okeyon(U6OK_UP)) CLIENTplayer->key|=KEYup;
    }
    if (u6okeyhit(U6OK_DOWN)){CLIENTplayer->key|=KEYdown2;
    }else{
      if (u6okeyon(U6OK_DOWN)) CLIENTplayer->key|=KEYdown;
    }



    //turn spell
    for (i=0;i<=7;i++){

      if (party_spellbook_frame[i]->mouse_click){
        if ((party_spellbook_frame[i]->mouse_y>=32)&&(party_spellbook_frame[i]->mouse_y<152)){ //5, 24pixel areas, after 32 pixels down
          if ((party_spellbook_frame[i]->mouse_x>=16)&&(party_spellbook_frame[i]->mouse_x<229)){ //16 pixels on each side
            if ((party_spellbook_frame[i]->mouse_x>=126)||(party_spellbook_frame[i]->mouse_x<118)){ //4|4 middle pixels
              //ok so now we must obtain a number, x, of which spell the player selected!
              x=(party_spellbook_frame[i]->mouse_y-32)/24;
              x2=party_spellbook_frame[i]->mouse_y-32-x*24;
              if (x2<16){
                if (party_spellbook_frame[i]->mouse_x>=126) x+=5;

                //is that spell in the spellbook at all???
                i3=spellbook_page[i]-1;

                i4=0;
                for (i2=0;i2<=15;i2++){
                  if (spell[i][(i3<<4)+i2]){ //valid









                    if (i4==x){
                      if (spell[i][(i3<<4)+i2]!=1){ //can cast?
                        userkey=4;
                        userspell=(i3<<4)+i2;
                        userspellbook=i;
                        if (spelltarget[(i3<<4)+i2]==1){
                          txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
                          ktar_display=2.0f;
                        }
                        goto gotspell;

                      }
                    }

                    i4++;
                  }
                }
                //user didn't click on a spell!

gotspell:;

              }//x2<20
            }//4|4
          }//16
        }//32+5*24
      }

      party_spellbook_frame[i]->mouse_click=0;
    }//i




    if (userkey==4){
      x=-1;
      if (u6okeyhit(U6OK_SETCASTRECALL1)) x=0;
      if (u6okeyhit(U6OK_SETCASTRECALL2)) x=1;
      if (u6okeyhit(U6OK_SETCASTRECALL3)) x=2;
      if (u6okeyhit(U6OK_SETCASTRECALL4)) x=3;
      if (u6okeyhit(U6OK_SETCASTRECALL5)) x=4;
      if (u6okeyhit(U6OK_SETCASTRECALL6)) x=5;
      if (u6okeyhit(U6OK_SETCASTRECALL7)) x=6;
      if (u6okeyhit(U6OK_SETCASTRECALL8)) x=7;
      if (x!=-1){
        //remove other F-keys to this spell
        for (x2=0;x2<=7;x2++) if ((spellrecall_partymember[x2]==(userspellbook+1))&&(spellrecall_i[x2]==userspell)) spellrecall_partymember[x2]=0;
        spellrecall_partymember[x]=userspellbook+1;
        spellrecall_i[x]=userspell;
        //update ALL spellbooks
        for (x2=0;x2<=7;x2++){
          if (tplay->party[x2]){
            tnpc=(npc*)tplay->party[x2]->more; tnpc->upflags|=128;
          }//party
        }//x2
        userkey=0;
      }//!=-1


      x=-1;
      if (u6okeyhit(U6OK_KTAR1)) x=0;
      if (u6okeyhit(U6OK_KTAR2)) x=1;
      if (u6okeyhit(U6OK_KTAR3)) x=2;
      if (u6okeyhit(U6OK_KTAR4)) x=3;
      if (u6okeyhit(U6OK_KTAR5)) x=4;
      if (u6okeyhit(U6OK_KTAR6)) x=5;
      if (u6okeyhit(U6OK_KTAR7)) x=6;
      if (u6okeyhit(U6OK_KTAR8)) x=7;
      if (u6okeyhit(U6OK_KTAR9)) x=8;
      if (u6okeyhit(U6OK_KTAR0)) x=9;
      if (x!=-1){
        CLIENTplayer->mx=32768+x;
        CLIENTplayer->key|=KEYmbclick;
        goto ktarcast;
      }

      if (vf->mouse_click){
        CLIENTplayer->mx=vf->mouse_x/32;
ktarcast:
        userkey=0;
        CLIENTplayer->mf=8+userspellbook;
        CLIENTplayer->my=vf->mouse_y/32;
        CLIENTplayer->my|=(userspell<<8);
      }
    }

    //recall spell using f5-f12
    if (userkey==0){
      x=-1;
      if (u6okeyhit(U6OK_CASTRECALL1)) x=0;
      if (u6okeyhit(U6OK_CASTRECALL2)) x=1;
      if (u6okeyhit(U6OK_CASTRECALL3)) x=2;
      if (u6okeyhit(U6OK_CASTRECALL4)) x=3;
      if (u6okeyhit(U6OK_CASTRECALL5)) x=4;
      if (u6okeyhit(U6OK_CASTRECALL6)) x=5;
      if (u6okeyhit(U6OK_CASTRECALL7)) x=6;
      if (u6okeyhit(U6OK_CASTRECALL8)) x=7;
      if (x!=-1){
        if (spellrecall_partymember[x]){
          if (tplay->party[spellrecall_partymember[x]-1]){
            if (spell[spellrecall_partymember[x]-1][spellrecall_i[x]]>1){ //valid
              userkey=4;
              userspell=spellrecall_i[x];
              userspellbook=spellrecall_partymember[x]-1;
              if (spelltarget[userspell]==1){
                txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
                ktar_display=2.0f;
              }//ktar
            }//valid
          }//party
        }//partymember
      }//!=-1
    }//0


    //keyb target
    if (userkey==3){
      x=-1;
      if (u6okeyhit(U6OK_KTAR1)) x=0;
      if (u6okeyhit(U6OK_KTAR2)) x=1;
      if (u6okeyhit(U6OK_KTAR3)) x=2;
      if (u6okeyhit(U6OK_KTAR4)) x=3;
      if (u6okeyhit(U6OK_KTAR5)) x=4;
      if (u6okeyhit(U6OK_KTAR6)) x=5;
      if (u6okeyhit(U6OK_KTAR7)) x=6;
      if (u6okeyhit(U6OK_KTAR8)) x=7;
      if (u6okeyhit(U6OK_KTAR9)) x=8;
      if (u6okeyhit(U6OK_KTAR0)) x=9;
      if (x!=-1){
        txtset(t,"??"); t->d2[0]=16; t->d2[1]=x; NET_send(NETplayer,NULL,t); //keyboard targeting request
        userkey=0;
      }
    }//userkey 3

    //change party member?
    x=0;
    if (u6okeyhit(U6OK_PARTY1)) x=1;
    if (u6okeyhit(U6OK_PARTY2)) x=2;
    if (u6okeyhit(U6OK_PARTY3)) x=3;
    if (u6okeyhit(U6OK_PARTY4)) x=4;
    if (u6okeyhit(U6OK_PARTY5)) x=5;
    if (u6okeyhit(U6OK_PARTY6)) x=6;
    if (u6okeyhit(U6OK_PARTY7)) x=7;
    if (u6okeyhit(U6OK_PARTY8)) x=8;
    if (x){
      CLIENTplayer->mf=15+x;
      CLIENTplayer->key|=KEYmbclick;
    }

    CLIENTplayer->action=0;


    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==5)){
      userkey=0; CLIENTplayer->key-=KEYmbclick;
      //send message to host
      if (CLIENTplayer->mf<=7){
        txtsetchar(t2,45); txtaddchar(t2,CLIENTplayer->mf);
        txtaddshort(t2,CLIENTplayer->mx); txtaddshort(t2,CLIENTplayer->my);
        NET_send(NETplayer,NULL,t2);
      }//CLIENTplayer->mf<=7
    }//not4sale



    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==1)){
      userkey=0;
      CLIENTplayer->key|=KEYu;
      CLIENTplayer->action=1;
    }

    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==2)){
      userkey=0;
      CLIENTplayer->key|=KEYl;
      CLIENTplayer->action=2;

    }

    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==3)){
      userkey=0;
      CLIENTplayer->key|=KEYa;
      CLIENTplayer->action=3;
    }



    if (u6okeyon(U6OK_QUANTITY)) CLIENTplayer->key|=KEYquan;

    //check if message is different from previous message (ignore mx,my)
    x=0; //send FALSE
    if (CLIENTplayer->key!=CLIENTplayer->key2) x=1;
    //?action
    if (x==1){ //send INPUT update message to host



      txtNEWLEN(t,(DWORD)&CLIENTplayer->mobj-(DWORD)&CLIENTplayer->mf);
      memcpy(t->d,&CLIENTplayer->mf,(DWORD)&CLIENTplayer->mobj-(DWORD)&CLIENTplayer->mf);
      txtset(t2,"?"); t2->d2[0]=5; txtadd(t2,t);
      NET_send(NETplayer,NULL,t2);
    } //x==1

  }//if (tplay->party[0]){ //end of active player output message processing


  //null SF message to avoid errors
  txtset(mess_SF,"??");
  mess_SF->d2[0]=0; mess_SF->d2[1]=0;


  //if (NEThost==NULL){ //client

CLIENT_readnext:
  x3=0;
  i=0;
  if (socketclient_ri[i]->d[socketclient_ri[i]->next]->l){ //buffered data available
    txtset(t,socketclient_ri[i]->d[socketclient_ri[i]->next]);
    txtNEWLEN(socketclient_ri[i]->d[socketclient_ri[i]->next],0);
    socketclient_ri[i]->next++;
    x3=1;
  }//->l

  if (x3){
    //U6Ohostlink1:

    if (t->l!=0){

      if (t->d2[0]==1){


        scrlog("Time update via message 1. DANGER WILL ROBINSON DANGER");
        txtset(mess_UPDATEps,t);
        txtright(mess_UPDATEps,mess_UPDATEps->l-1);
        if (!NEThost) btime2=mess_UPDATEps->df[0];
        txtright(mess_UPDATEps,mess_UPDATEps->l-4);

        goto CLIENT_donemess;
      }

      if (t->d2[0]==2){
        if (nodisplay) goto CLIENT_donemess;

        txtset(mess_SF,t); txtright(mess_SF,mess_SF->l-1);
        //SF (mess_SF) -> local SF
        i5=mess_SF->d2[0]+(mess_SF->d2[1]<<8);
        i4=2; //message offset
        for (i=0;i<i5;i++){
          for (i3=0;i3<=255;i3++){ if (sfx[i3].type==0){
            sfx[i3].type=mess_SF->d2[i4]+(mess_SF->d2[i4+1]<<8); i4+=2;
            sfx[i3].x=mess_SF->d2[i4]+(mess_SF->d2[i4+1]<<8); i4+=2;
            sfx[i3].y=mess_SF->d2[i4]+(mess_SF->d2[i4+1]<<8); i4+=2;
            sfx[i3].x2=mess_SF->d2[i4]+(mess_SF->d2[i4+1]<<8); i4+=2;
            sfx[i3].y2=mess_SF->d2[i4]+(mess_SF->d2[i4+1]<<8); i4+=2;
            sfx[i3].more=mess_SF->d2[i4]+(mess_SF->d2[i4+1]<<8); i4+=2;
            sfx[i3].wait2[0]=mess_SF->d2[i4]; i4++;
            sfx[i3].wait2[1]=mess_SF->d2[i4]; i4++;
            sfx[i3].wait2[2]=mess_SF->d2[i4]; i4++;
            sfx[i3].wait2[3]=mess_SF->d2[i4]; i4++;
            if (sfx[i3].type==3){ //get message
              static txt *dbgt5;
              dbgt5=txtnew(); //NEW TXT ALLOCATED

              txtNEWLEN(dbgt5,sfx[i3].x2);
              memcpy(&dbgt5->d2[0],&mess_SF->d2[i4],sfx[i3].x2);
              i4+=sfx[i3].x2;
              sfx[i3].p=dbgt5;
              txtset(t,(txt*)sfx[i3].p);
              txtset(t2,"?"); t2->d2[0]=92;
              z=txtsearch(t,t2);
              if (z==0) z=t->l; else z--;
              f=(float)z/10.0f;
              if (f<1)f=1;
              if(f>4)f=4;
              sfx[i3].wait=f;
              //new text message
              //check for and elimate position conflicts
              if ((sfx[i3].x==2047)&&(sfx[i3].y==1023)) goto shiftdown_skip;
              if ((sfx[i3].x==2048)&&(sfx[i3].y==1023)) goto shiftdown_skip;
shiftdown:
              for (i2=0;i2<=255;i2++){ if ((sfx[i2].type==3)&&(i2!=i3)){
                if ((sfx[i3].y==sfx[i2].y)||(sfx[i3].y==(sfx[i2].y+1))||(sfx[i3].y==(sfx[i2].y-1))) {sfx[i3].y=sfx[i2].y+2; goto shiftdown;}
              }}
shiftdown_skip:;

              //***** PLAYER MESSAGE MUTING AND IGNORING *****
              if (sfx[i3].more>=194){//player port range
                txtset(t3,": \"");
                if(x2=txtsearch((txt*)sfx[i3].p,t3)){
                  txtset(t4,(txt*)sfx[i3].p);
                  txtleft(t4,x2-1);
                  txtucase(t4);
                  if (t4->d2[0]==6){//check if a coloured name header exists, and if so remove it
                    txtright(t4,t4->l-5);
                  }
                  //exact search needed
                  tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT); //log
ignore_nextplayer:
                  txtfilein(t6,tfh);
                  txtucase(t6);
                  txtset(t7,"[MUTE]"); txtadd(t7,t4); if (txtsame(t6,t7)){
                    txtleft((txt*)sfx[i3].p,x2-1); txtadd((txt*)sfx[i3].p,": \"...\"");
                    goto muted;
                  }
                  txtset(t7,"[IGNORE]"); txtadd(t7,t4); if (txtsame(t6,t7)){
                    sfx[i3].type=0;
                    goto ignored;
                  }
                  if (t6->l) goto ignore_nextplayer;
                  close(tfh);
                }else{

                  txtset(t4,(txt*)sfx[i3].p); txtucase(t4);

                  tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT); //log
ignore_nextplayerspace:
                  txtfilein(t6,tfh); txtucase(t6);//read file line

                  txtset(t5,"[MUTE]"); txtadd(t5,t4); txtleft(t5,t6->l);
                  if (txtsame(t5,t6)){
                    txtleft((txt*)sfx[i3].p,t5->l-6); txtadd((txt*)sfx[i3].p," ...");
                    goto muted;
                  }
                  txtset(t5,"[IGNORE]"); txtadd(t5,t4); txtleft(t5,t6->l);
                  if (txtsame(t5,t6)){
                    sfx[i3].type=0;
                    goto ignored;
                  }
                  if (t6->l) goto ignore_nextplayerspace;
                  close(tfh);

                }//no speech marks

              }//player port range
muted:;
ignored:;






            }//text special effect
            goto mess_SFok;
          }} //i3, type==0
mess_SFok:;
        } //i

        goto CLIENT_donemess;
      }

      if (t->d2[0]==3){


        i2=t->d2[1]; //party member index
        x4=t->ds[1]; //update flags
        txtright(t,t->l-4);
        //1		name, portriat, str, dex, int, lev, hp_max, mp_max (all)
        //2		hp
        //4     mp
        //8     xp
        //16    weight (automatically set if inventory/equipped items change)
        //32    inventory, bag icon
        //64    equipped items


        tplay->party[i2]=tplayobj[i2];


        tnpc=(npc*)tplay->party[i2]->more;
        tnpc->upflags|=x4;




        if (x4&32){
          x3=t->d2[0]; //no-objs+up/down arrows
          txtright(t,t->l-1);
          x5=x3;
          x3&=31; //clear arrow flags, num of items
          x2=0;
          myobj=tnpc->items;
          myobj->more=NULL; //clear old data (in case these is no new data)
          tnpc->baseitem=tnpc->items; tnpc->baseitem_offset=0;
          if (x5&64){ //up arrow
            tnpc->baseitem_offset=4;
            for (x=0;x<=3;x++){
              myobj2=tobj_i[i2][x2];
              ZeroMemory(tobj_i[i2][x2],sizeof(object));
              if (x2==0){
                myobj->more=myobj2;
              }else{
                myobj->next=myobj2;
              }
              x2++;
              myobj=myobj2;
            }
          }

dbg1:
          if ((x2-tnpc->baseitem_offset)<x3){
            x=t->ds[0];
            i5=2;
            myobj2=tobj_i[i2][x2];
            ZeroMemory(tobj_i[i2][x2],sizeof(object));
            myobj2->type=x;
            if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].flags&4096){ //temp quantity
              txtright(t,t->l-2);
              myobj2->more2=t->ds[0];
            }
            if ((myobj2->type&1023)==64){ //key
              txtright(t,t->l-2);
              myobj2->more2=t->ds[0];
            }

            if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].v1 ||  //v1 (damage) + info
              obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].v2 || (myobj2->type&1023)==448) { //v2 (armour) + info and horse papers + info
                txtright(t,t->l-2);
                myobj2->more2=t->d2[0];
                i5=1;
            }

            if (x2==0){
              myobj->more=myobj2;
            }else{
              myobj->next=myobj2;
            }
            myobj=myobj2;
            x2++;
            txtright(t,t->l-i5);
            goto dbg1;
          }
          if (x5&128){ //down arrow
            myobj2=tobj_i[i2][x2];
            ZeroMemory(tobj_i[i2][x2],sizeof(object));
            myobj->next=myobj2;
            x2++;
          }

          if (x5&32){ //bag
            x=t->ds[0];
            txtright(t,t->l-2);
            myobj2=tobj_i[i2][x2];
            ZeroMemory(tobj_i[i2][x2],sizeof(object));
            myobj2->type=x;
            myobj2->more=tnpc->items->more;
            tnpc->items->more=myobj2;
            tnpc->baseitem=myobj2;
          }
        }//x4&32



        if (x4&64){
          x=t->ds[0]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][0];myobj->type=x;} tnpc->helm=myobj;
          x=t->ds[1]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][1];myobj->type=x;} tnpc->wep_right=myobj;
          x=t->ds[2]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][2];myobj->type=x;} tnpc->wep_left=myobj;
          x=t->ds[3]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][3];myobj->type=x;} tnpc->armour=myobj;
          x=t->ds[4]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][4];myobj->type=x;} tnpc->boots=myobj;
          x=t->ds[5]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][5];myobj->type=x;} tnpc->neck=myobj;
          x=t->ds[6]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][6];myobj->type=x;} tnpc->ring_right=myobj;
          x=t->ds[7]; myobj=NULL; if (x!=0){myobj=tobj_e[i2][7];myobj->type=x;} tnpc->ring_left=myobj;
          tnpc->wt2=t->ds[8];
          txtright(t,t->l-18);
        }//x4&64

        if (x4&1){//1		name, portriat, str, dex, int, lev, hp_max, mp_max (does not change often)
          x=t->d2[0];
          txtNEWLEN(tnpc->name,x);
          memcpy(&tnpc->name->d2[0],&t->d2[1],x);

          if (i2==0) txtset(namelast,tnpc->name);
          txtright(t,t->l-(x+1));
          tnpc->port=t->ds[0];
          if (i2==0) portlast=tnpc->port;

          tnpc->s=t->ds[1];
          tnpc->d=t->ds[2];
          tnpc->i=t->ds[3];
          tnpc->lev=t->ds[4];
          if ((i2==0)&&(tnpc->lev>1)) U6O_WALKTHRU=FALSE;
          tnpc->hp_max=t->ds[5];
          tnpc->mp_max=t->ds[6];
          tnpc->wt_max=t->ds[7];
          tnpc->wt2_max=t->ds[8];
          txtright(t,t->l-18);
        }//x4&1



        if (x4&16){//wt
          tnpc->wt=t->ds[0]; txtright(t,t->l-2);
        }//x4&16
        if (x4&2){//hp
          tnpc->hp=t->ds[0]; txtright(t,t->l-2);
        }//x4&2
        if (x4&4){//mp
          x=tnpc->mp; client_spellwait[i2]=0;
          tnpc->mp=t->ds[0]; txtright(t,t->l-2);
          if ((x4&1)==0){ //not setting up a new npc
            if (tnpc->mp<x) client_spellwait[i2]=1;
          }//(x4&1)==0
        }//x4&4
        if (x4&8){//xp
          tnpc->exp=t->dl[0]; txtright(t,t->l-4);
        }//x4&8


        if (x4&128){//spellbook
          ZeroMemory(&spell[i2][0],512);
          x=t->d2[0]; txtright(t,t->l-1); //number of spells



          for (x2=0;x2<x;x2++){
            x3=t->d2[0]; txtright(t,t->l-1); //spell type


            spell[i2][x3]=t->ds[0]; txtright(t,t->l-2); //cast!


          }
          if (x){

            if (party_spellbook_frame[i2]->offset_x==4096){
              party_spellbook_frame[i2]->offset_x=i2*32; party_spellbook_frame[i2]->offset_y=128-16+i2*32+256;
              if (cltset2_restored){ if (cltset2.party_spellbook_frame_offset_x[i2]!=32767){
                party_spellbook_frame[i2]->offset_x=cltset2.party_spellbook_frame_offset_x[i2]; party_spellbook_frame[i2]->offset_y=cltset2.party_spellbook_frame_offset_y[i2];
                cltset2.party_spellbook_frame_offset_x[i2]=32767;
              }}
            }





            if (party_spellbook_frame[i2]->offset_x>1024) party_spellbook_frame[i2]->offset_x-=2048;
          }else{
            if (party_spellbook_frame[i2]->offset_x<1024) party_spellbook_frame[i2]->offset_x+=2048;
          }

          //check spellbookpage! (if invalid select first spellbook page only!)

          for (x2=0;x2<=15;x2++){
            if (spell[i2][((spellbook_page[i2]-1)<<4)+x2]) goto validpage;
          }
          for (x2=0;x2<=255;x2++){
            if (spell[i2][x2]) {spellbook_page[i2]=(x2>>4)+1; goto validpage;}
          }
validpage:;
        }//spellbook


        goto CLIENT_donemess;
      }

      if (t->d2[0]==4){ //set mouse object
        txtright(t,t->l-1);
        static object *mobj_local=NULL;
        if (mobj_local==NULL) mobj_local=OBJnew_local();
        mobj_local->type=t->ds[0];
        CLIENTplayer->mobj=mobj_local;
        if (t->ds[0]==0xFFFF) CLIENTplayer->mobj=NULL;
        goto CLIENT_donemess;
      }

      //8 recieve status message (white text at bottom of the screen, not conversation)
      if (t->d2[0]==8){
        txtright(t,t->l-1);
        STATUSMESSadd(t);
        goto CLIENT_donemess;
      }

      if (t->d2[0]==10){ //type 10: get U6OID info


        txtright(t,t->l-1);
        tu6oid=t->dl[0]; txtright(t,t->l-4);

        //scan list of U6OIDs, add name
        if (idlstn!=-1){
          for (z2=0;z2<=idlstn;z2++){
            if (tu6oid==idlst[z2]){
              x=t->d2[0]; txtright(t,t->l-1);
              txtset(idlst_name[z2],t); txtleft(idlst_name[z2],x); txtright(t,t->l-x);
              idlst_namecolour[z2]=t->dl[0]&0xFFFFFF; txtright(t,t->l-3);
getid_more:
              if (t->l){
                if (t->d2[0]==1){ txtright(t,t->l-1);

                goto getid_more;
                }//1
                //...
              }//t->l

              //store MY ID
              if (tplay->party[0]){ tnpc3=(npc*)tplay->party[0]->more; if (txtsame(idlst_name[z2],tnpc3->name)){
                clientplayerid=idlst[z2];
                namecolour=idlst_namecolour[z2];
                tnpc3->upflags|=1;
              }}


            }//==
          }//z2 
        }//!=-1
        goto CLIENT_donemess;
      } //10

      if (t->d2[0]==11){ //type 11: set background midi
        if (U6O_DISABLEMUSIC) goto CLIENT_donemess;
        if (midi_background==0){//stop playing intro immediately
          u6omidi->Stop();//stop playing background midi
isplayingwait3: if (u6omidi->IsPlaying()==S_OK) goto isplayingwait3;
        }

        midi_background=t->d2[1];
        goto CLIENT_donemess;
      }//11

      if (t->d2[0]==30){ //type 30: set foreground midi & midi_foreground_wait
        midi_foreground=t->d2[1];
        midi_foreground_wait=t->ds[1];
        goto CLIENT_donemess;
      }//30

      if (t->d2[0]==14){ //type 14: set wind
        windx2=(t->d2[1]&3)-1;
        windy2=(t->d2[1]>>2)-1;
        goto CLIENT_donemess;
      }//14

      if (t->d2[0]==18){ //type 18: client_spellwait returned to 0
        if (tplay->party[t->d2[1]]){
          client_spellwait[t->d2[1]]=0;
          tnpc=(npc*)tplay->party[t->d2[1]]->more;
          tnpc->upflags|=4;
        }
        goto CLIENT_donemess;
      }//18

      if (t->d2[0]==22){ //type 22: play endgame sequence!
        endgame=1;
        goto CLIENT_donemess;
      }//22

      if (t->d2[0]==23){ //type 23: set staff cast cursor
        txtright(t,t->l-1);
        userkey=4;
        userspell=t->ds[0];
        userspellbook=16; //+8==24 staff override value!
        if (spelltarget[userspell]==1){
          txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
          ktar_display=2.0f;
        }
        goto CLIENT_donemess;
      }//23

      if (t->d2[0]==24){ //type 24: play instrument!
        clientinstrument=t->d2[1];

        playinstrument=1;

        if (clientinstrument==0) STATUSMESSadd("You begin playing the lute. (Press ESC when finished)");
        if (clientinstrument==1) STATUSMESSadd("You begin playing the harpsichord. (Press ESC when finished)");
        if (clientinstrument==2) STATUSMESSadd("You begin playing the harp. (Press ESC when finished)");
        if (clientinstrument==3) STATUSMESSadd("You begin playing the panpipes. (Press ESC when finished)");
        if (clientinstrument==4) STATUSMESSadd("You begin playing the xylophone. (Press ESC when finished)");

        if (musickeyboard->offset_x>=1024) musickeyboard->offset_x-=2048;
        goto CLIENT_donemess;
      }//24

      if (t->d2[0]==27){ //type 27: play MIDI note(s): client
        for (x=2;x<t->l;x++){
          mididown(t->d2[1],t->d2[x]);
        }

        goto CLIENT_donemess;
      }//27

      if (t->d2[0]==28){ //type 28: stop MIDI note(s): client
        for (x=2;x<t->l;x++){
          midiup(t->d2[1],t->d2[x]);
        }
        goto CLIENT_donemess;
      }//28

      if (t->d2[0]==29){ //type 29: stop playing instrument
        if (playinstrument){
          if (clientinstrument==0) STATUSMESSadd("You finish playing the lute.");
          if (clientinstrument==1) STATUSMESSadd("You finish playing the harpsichord.");
          if (clientinstrument==2) STATUSMESSadd("You finish playing the harp.");
          if (clientinstrument==3) STATUSMESSadd("You finish playing the panpipes.");
          if (clientinstrument==4) STATUSMESSadd("You finish playing the xylophone.");

          playinstrument=0;
          if (musickeyboard->offset_x<1024) musickeyboard->offset_x+=2048;
        }
        goto CLIENT_donemess;
      }//29

      if (t->d2[0]==35){ //flush buffers & scene update
        static player *tplayer_backup;
        tplayer_backup=tplayer; tplayer=tplay;
        //flush object & mover buffers
        tplayer->x=0;
        tplayer->y=0;
        tplayer->sobj_bufoffx=0;
        tplayer->sobj_bufoffy=0;
        tplayer->mv_i=0;
        for (y=0;y<=71;y++){ for (x=0;x<=95;x++){
          tplayer->sobj_bufsize[x][y]=0;
          tplayer->sobj_tempfixed[x][y]=0;
        }}
        tplayer=tplayer_backup;



        goto scene_update_message;
      }//35

      if (t->d2[0]==31){ //scene update
scene_update_message:

        static unsigned long bitsi,bitsi2;
        static unsigned short *tp2;

        static player *tplayer_backup;
        tplayer_backup=tplayer; tplayer=tplay;
        bitsi=8;

        if (BITSget(t,&bitsi,1)){//x,y change
          tplayer->x=BITSget(t,&bitsi,11);
          tplayer->y=BITSget(t,&bitsi,10);
        }

        x=tplayer->x; y=tplayer->y;

        getscreenoffset(x,y,&tpx,&tpy);


        ctpx2=tplayer->x; ctpy2=tplayer->y;
        ctpx=tpx; ctpy=tpy;

        //screen+1 shift
        x3=tpx-1; y3=tpy-1; x4=tpx+32; y4=tpy+24;
        x5=tplayer->sobj_bufoffx; y5=tplayer->sobj_bufoffy; x6=x5+96-1; y6=y5+72-1; //current buffer extents
        //i. if the screen+1 buffer fits within buffer don't relocate
        if (x3>=x5){ if (x4<=x6){ if (y3>=y5){ if (y4<=y6){
          goto screen1shiftokc;
        }}}}
        //ii. clear buffer if screen+1 buffer is entirely outside
        i=0;
        if (x4<x5) i=1;
        if (x3>x6) i=1;
        if (y4<y5) i=1;
        if (y3>y6) i=1;
        if (i){
          ZeroMemory(&tplayer->sobj_bufsize,96*72*2); ZeroMemory(&tplayer->sobj_tempfixed,96*72*4);
          tplayer->sobj_bufoffx=tpx-32; tplayer->sobj_bufoffy=tpy-24;
          goto screen1shiftokc;
        }
        //iii. relocate screen+1 buffer
        //calculate new buffer location in x2,y2
        x2=tplayer->sobj_bufoffx;
        if (x2>x3) x2=x3;
        if ((x2+95)<x4) x2=x4-95;
        y2=tplayer->sobj_bufoffy;
        if (y2>y3) y2=y3;
        if ((y2+71)<y4) y2=y4-71;
        //modified for loop
        if (x2>=tplayer->sobj_bufoffx){
          if (y2>=tplayer->sobj_bufoffy) i=0; else i=2;
        }else{
          if (y2>=tplayer->sobj_bufoffy) i=1; else i=3;
        }
        if (i==0){y9=0; x9=0;}
        if (i==1){y9=0; x9=95;}
        if (i==2){y9=71; x9=0;}
        if (i==3){y9=71; x9=95;}
sobj_copyloop1c:
        x8=x2-tplayer->sobj_bufoffx+x9; y8=y2-tplayer->sobj_bufoffy+y9;
        if (x8>=0){ if (x8<=95){ if (y8>=0){ if (y8<=71){
          //copy position
          tplayer->sobj_tempfixed[x9][y9]=tplayer->sobj_tempfixed[x8][y8];
          i2=tplayer->sobj_buf2bytes[x9][y9]; tplayer->sobj_buf2bytes[x9][y9]=tplayer->sobj_buf2bytes[x8][y8]; tplayer->sobj_buf2bytes[x8][y8]=i2;
          tp2=tplayer->sobj[x9][y9]; tplayer->sobj[x9][y9]=tplayer->sobj[x8][y8]; tplayer->sobj[x8][y8]=tp2;
          tplayer->sobj_bufsize[x9][y9]=tplayer->sobj_bufsize[x8][y8]; tplayer->sobj_bufsize[x8][y8]=0;
          goto sobj_copiedpos1c;
        }}}}
        //clear position
        tplayer->sobj_tempfixed[x9][y9]=0;
        tplayer->sobj_bufsize[x9][y9]=0;
sobj_copiedpos1c:;
        if (i==0){
          x9++; if (x9>=96){y9++; x9=0;}
          if (y9<72) goto sobj_copyloop1c;
        }
        if (i==1){
          x9--; if (x9<0){y9++; x9=95;}
          if (y9<72) goto sobj_copyloop1c;
        }
        if (i==2){
          x9++; if (x9>=96){y9--; x9=0;}
          if (y9>=0) goto sobj_copyloop1c;
        }
        if (i==3){
          x9--; if (x9<0){y9--; x9=95;}
          if (y9>=0) goto sobj_copyloop1c;
        }
        tplayer->sobj_bufoffx=x2; tplayer->sobj_bufoffy=y2;
screen1shiftokc:;

        if (BITSget(t,&bitsi,1)){//obj buffer has changed
          //screen+8 shift
          x3=tpx-8; y3=tpy-8; x4=tpx+32+8-1; y4=tpy+24+8-1;
          x5=tplayer->sobj_bufoffx; y5=tplayer->sobj_bufoffy; x6=x5+96-1; y6=y5+72-1; //current buffer extents
          //i. if the screen+8 buffer fits within buffer don't relocate
          if (x3>=x5){ if (x4<=x6){ if (y3>=y5){ if (y4<=y6){
            goto screen8shiftokc;
          }}}}
          //ii. clear buffer if screen+8 buffer is entirely outside
          i=0;
          if (x4<x5) i=1;
          if (x3>x6) i=1;
          if (y4<y5) i=1;
          if (y3>y6) i=1;
          if (i){
            ZeroMemory(&tplayer->sobj_bufsize,96*72*2); ZeroMemory(&tplayer->sobj_tempfixed,96*72*4);
            tplayer->sobj_bufoffx=tpx-32; tplayer->sobj_bufoffy=tpy-24;
            goto screen8shiftokc;
          }
          //iii. relocate buffer
          //calculate new buffer location in x2,y2
          x2=tplayer->sobj_bufoffx;
          if (x2>x3) x2=x3;
          if ((x2+95)<x4) x2=x4-95;
          y2=tplayer->sobj_bufoffy;
          if (y2>y3) y2=y3;
          if ((y2+71)<y4) y2=y4-71;
          //modified for loop
          if (x2>=tplayer->sobj_bufoffx){
            if (y2>=tplayer->sobj_bufoffy) i=0; else i=2;
          }else{
            if (y2>=tplayer->sobj_bufoffy) i=1; else i=3;
          }
          if (i==0){y9=0; x9=0;}
          if (i==1){y9=0; x9=95;}
          if (i==2){y9=71; x9=0;}
          if (i==3){y9=71; x9=95;}
sobj_copyloopc:
          x8=x2-tplayer->sobj_bufoffx+x9; y8=y2-tplayer->sobj_bufoffy+y9;
          if (x8>=0){ if (x8<=95){ if (y8>=0){ if (y8<=71){
            //copy position
            tplayer->sobj_tempfixed[x9][y9]=tplayer->sobj_tempfixed[x8][y8];
            i2=tplayer->sobj_buf2bytes[x9][y9]; tplayer->sobj_buf2bytes[x9][y9]=tplayer->sobj_buf2bytes[x8][y8]; tplayer->sobj_buf2bytes[x8][y8]=i2;
            tp2=tplayer->sobj[x9][y9]; tplayer->sobj[x9][y9]=tplayer->sobj[x8][y8]; tplayer->sobj[x8][y8]=tp2;
            tplayer->sobj_bufsize[x9][y9]=tplayer->sobj_bufsize[x8][y8]; tplayer->sobj_bufsize[x8][y8]=0;
            goto sobj_copiedpos0c;
          }}}}
          //clear position
          tplayer->sobj_tempfixed[x9][y9]=0;
          tplayer->sobj_bufsize[x9][y9]=0;
sobj_copiedpos0c:;

          if (i==0){
            x9++; if (x9>=96){y9++; x9=0;}
            if (y9<72) goto sobj_copyloopc;
          }
          if (i==1){
            x9--; if (x9<0){y9++; x9=95;}
            if (y9<72) goto sobj_copyloopc;
          }
          if (i==2){
            x9++; if (x9>=96){y9--; x9=0;}
            if (y9>=0) goto sobj_copyloopc;
          }
          if (i==3){
            x9--; if (x9<0){y9--; x9=95;}
            if (y9>=0) goto sobj_copyloopc;
          }
          tplayer->sobj_bufoffx=x2; tplayer->sobj_bufoffy=y2;
screen8shiftokc:;

changestate: if (BITSget(t,&bitsi,1)){
          y=BITSget(t,&bitsi,11); x=y%48; y/=48;
          i3=tobjfixed_index[tpy-8+y][tpx-8+x];
          i4=tobjfixed_type[i3];
          z=BITSget(t,&bitsi,getnbits(i4));
          i5=1<<z;

          x2=tpx+x-8; y2=tpy+y-8; x3=x2-tplayer->sobj_bufoffx; y3=y2-tplayer->sobj_bufoffy;
          if (tplayer->sobj_tempfixed[x3][y3]&i5) tplayer->sobj_tempfixed[x3][y3]-=i5; else tplayer->sobj_tempfixed[x3][y3]|=i5;
          goto changestate;
             }

             static unsigned short vbuf[1024];
oum_getnextsquare: if (BITSget(t,&bitsi,1)){//if =1 set object of a/another square on the map
             y=BITSget(t,&bitsi,11); x=y%48; y/=48;
             x2=tpx+x-8; y2=tpy+y-8; x3=x2-tplayer->sobj_bufoffx; y3=y2-tplayer->sobj_bufoffy;
             i=0;//vbuf index
oum_getnextobj: if (BITSget(t,&bitsi,1)){//if =1 a/another object exists on this square
             vbuf[i]=BITSget(t,&bitsi,16);
             vbuf[i]|=(BITSget(t,&bitsi,1)*32768);
             i++;
             goto oum_getnextobj;
                }//if =1 a/another object exists on this square
                //update allocated buffer size
                if (tplayer->sobj_buf2bytes[x3][y3]<i){
                  if (tplayer->sobj[x3][y3]) free(tplayer->sobj[x3][y3]);
                  tplayer->sobj[x3][y3]=(unsigned short*)malloc(i*2);
                  tplayer->sobj_buf2bytes[x3][y3]=i;
                }
                //update buffer info
                tp2=tplayer->sobj[x3][y3];
                for (i2=0;i2<i;i2++){
                  tp2[i2]=vbuf[i2];
                }
                tplayer->sobj_bufsize[x3][y3]=i;
                goto oum_getnextsquare;
                   }//if =1 set object of a/another square on the map




                   //update mover buffer




                   /*
                   ***OBJECT UPDATE MESSAGE*** [OFFICIAL]
                   b:
                   1	if =1 set object of a/another square on the map
                   {
                   11	x,y location offset of square within buffer+8
                   a:
                   1	if =1 a/another object exists on this square
                   {
                   10 object graphic index (optomized)
                   1 object is flashing
                   goto a
                   }
                   goto b
                   }
                   */




        }//object buffer changed


        //remove all offscreen objects in client's array
mover_removeoffscreen_restartc:
        for (i=0;i<tplayer->mv_i;i++){
          x=tplayer->mv_x[i]-tpx; y=tplayer->mv_y[i]-tpy;
          if ((x<-1)||(x>32)||(y<-1)||(y>24)){

            //reshuffle array
            for (i3=i+1;i3<tplayer->mv_i;i3++){
              tplayer->mv_x[i3-1]=tplayer->mv_x[i3];
              tplayer->mv_y[i3-1]=tplayer->mv_y[i3];
              tplayer->mv_type[i3-1]=tplayer->mv_type[i3];
              tplayer->mv_dir[i3-1]=tplayer->mv_dir[i3];
              tplayer->mv_frame[i3-1]=tplayer->mv_frame[i3];
              tplayer->mv_object[i3-1]=tplayer->mv_object[i3];
              tplayer->mv_flags[i3-1]=tplayer->mv_flags[i3];
              tplayer->mv_hpmp[i3-1]=tplayer->mv_hpmp[i3];
              tplayer->mv_playerid[i3-1]=tplayer->mv_playerid[i3];
              tplayer->mv_ktar[i3-1]=tplayer->mv_ktar[i3];
              tplayer->mv_more[i3-1]=tplayer->mv_more[i3];
            }//i3
            tplayer->mv_i--;




            goto mover_removeoffscreen_restartc;
          }//x,y boundary check
        }//i


        //1. REMOVE
mover_remove_next: if (BITSget(t,&bitsi,1)){
        i=BITSget(t,&bitsi,getnbits(tplayer->mv_i));

        //reshuffle array
        for (i3=i+1;i3<tplayer->mv_i;i3++){
          tplayer->mv_x[i3-1]=tplayer->mv_x[i3];
          tplayer->mv_y[i3-1]=tplayer->mv_y[i3];
          tplayer->mv_type[i3-1]=tplayer->mv_type[i3];
          tplayer->mv_dir[i3-1]=tplayer->mv_dir[i3];
          tplayer->mv_frame[i3-1]=tplayer->mv_frame[i3];
          tplayer->mv_object[i3-1]=tplayer->mv_object[i3];
          tplayer->mv_flags[i3-1]=tplayer->mv_flags[i3];
          tplayer->mv_hpmp[i3-1]=tplayer->mv_hpmp[i3];
          tplayer->mv_playerid[i3-1]=tplayer->mv_playerid[i3];
          tplayer->mv_ktar[i3-1]=tplayer->mv_ktar[i3];
          tplayer->mv_more[i3-1]=tplayer->mv_more[i3];
        }//i3
        tplayer->mv_i--;



        goto mover_remove_next;
                   }

                   /*
                   ***2.3 MOVE***
                   1 if =1 move a/another mover
                   {
                   [?] index of mover to move in current buffer
                   5 number representing a special x,y offset from current location to move to
                   }
                   */
                   /*
                   for (i=0;i<tplayer->mv_i;i++){
                   i2=mv_last[i];
                   x=mv_x[i2]-tplayer->mv_x[i]; y=mv_y[i2]-tplayer->mv_y[i];
                   if (x||y){
                   BITSadd(t,&bitsi,1,1);//move a/another mover
                   z4=tplayer->mv_i; z2=1; z3=0; getnbits3: if (z2<z4){z3++; z2<<=1; goto getnbits3;}//z3=nbits required to store which tempfixed obj has changed
                   if (z3) BITSadd(t,&bitsi,i,z3);//index of mover to move in current buffer
                   BITSadd(t,&bitsi,mover_offseti[x+3][y+3],5);
                   }//x||y
                   }//i
                   BITSadd(t,&bitsi,0,1);//no more movers to move
                   */
mover_move_next: if (BITSget(t,&bitsi,1)){
                   i=BITSget(t,&bitsi,getnbits(tplayer->mv_i));
                   z=BITSget(t,&bitsi,5);
                   x=mover_offsetx[z]; y=mover_offsety[z];
                   tplayer->mv_x[i]+=x; tplayer->mv_y[i]+=y;




                   if (x>0) tplayer->mv_dir[i]=1;
                   if (x<0) tplayer->mv_dir[i]=3;
                   if (y<0) tplayer->mv_dir[i]=0;
                   if (y>0) tplayer->mv_dir[i]=2;

                   tplayer->mv_frame[i]=objgetnextframe(tplayer->mv_type[i],tplayer->mv_frame[i]);

                   goto mover_move_next;
                 }



                 //2.2 MOVER DIRECTION/FRAME CHANGE
                 /*
                 1 change mover frame/dir
                 ? index of mover
                 [2] if mover CAN have a direction it is set here, otherwise this can be omitted and the movers frame only will be incremented
                 */
                 /*
                 for (i=0;i<tplayer->mv_i;i++){
                 i2=mv_last[i];
                 if ((tplayer->mv_dir[i]!=mv_dir[i2])||(tplayer->mv_frame[i]!=mv_frame[i2])){
                 BITSadd(t,&bitsi,1,1);
                 BITSadd(t,&bitsi,i,getnbits(tplayer->mv_i));//index of mover to move in current buffer
                 BITSadd(t,&bitsi,mv_dir[i2],2);
                 tplayer->mv_dir[i]=mv_dir[i2];
                 tplayer->mv_frame[i]=mv_frame[i2];
                 sceneupdaterequired=1;
                 }
                 }//i
                 BITSadd(t,&bitsi,0,1);//no more movers to direction/frame change
                 */
mover_dirfrmchange_next: if (BITSget(t,&bitsi,1)){
                 i=BITSget(t,&bitsi,getnbits(tplayer->mv_i));
                 tplayer->mv_dir[i]=BITSget(t,&bitsi,2);
                 tplayer->mv_frame[i]=objgetnextframe(tplayer->mv_type[i],tplayer->mv_frame[i]);
                 goto mover_dirfrmchange_next;
                         }





mover_statechange_next: if (BITSget(t,&bitsi,1)){
                         i=BITSget(t,&bitsi,getnbits(tplayer->mv_i));
mover_statechange_next2:
                         z=BITSget(t,&bitsi,4);
                         if (z==0){
                           tplayer->mv_flags[i]^=MV_LIGHTBRIGHT;
                           if (tplayer->mv_flags[i]&MV_LIGHTBRIGHT){
                             if (tplayer->mv_flags[i]&MV_LIGHTGLOW) tplayer->mv_flags[i]-=MV_LIGHTGLOW;
                           }
                         }//0
                         if (z==1){
                           tplayer->mv_flags[i]^=MV_LIGHTGLOW;
                           if (tplayer->mv_flags[i]&MV_LIGHTGLOW){
                             if (tplayer->mv_flags[i]&MV_LIGHTBRIGHT) tplayer->mv_flags[i]-=MV_LIGHTBRIGHT;
                           }
                         }//1
                         if (z==2) tplayer->mv_flags[i]^=MV_INVISIBLE;
                         if (z==3) tplayer->mv_flags[i]^=MV_PARALYZE;
                         if (z==4) tplayer->mv_flags[i]^=MV_SLEEP;
                         if (z==5) tplayer->mv_flags[i]^=MV_PROTECT;
                         if (z==6) tplayer->mv_flags[i]^=MV_STORMCLOAK;
                         if (z==7) tplayer->mv_flags[i]^=MV_TYPING;
                         if (z==8){
                           tplayer->mv_hpmp[i]=BITSget(t,&bitsi,8);
                         }
                         if (z==9){
                           tplayer->mv_ktar[i]=BITSget(t,&bitsi,4);
                         }
                         if (z==10) tplayer->mv_flags[i]^=MV_SPEAKING;
                         if (BITSget(t,&bitsi,1)) goto mover_statechange_next2;
                         goto mover_statechange_next;
                        }

                        //3. ADD
mover_add_next: if (BITSget(t,&bitsi,1)){
                        i=tplayer->mv_i;
                        y=BITSget(t,&bitsi,10); x=y%34; y/=34;
                        x=x+tpx-1; y=y+tpy-1;
                        tplayer->mv_x[i]=x; tplayer->mv_y[i]=y;
                        z=BITSget(t,&bitsi,10);
                        //special cases exist
                        tplayer->mv_type[i]=z;
                        tplayer->mv_frame[i]=0;
                        tplayer->mv_dir[i]=0;

                        if (z==413){//silver serp
                          tplayer->mv_frame[i]=BITSget(t,&bitsi,4);
                          goto mover_add_specialc;
                        }
                        if (z==366){//tanglevine tendril (not core)
                          tplayer->mv_frame[i]=BITSget(t,&bitsi,3);
                          goto mover_add_specialc;
                        }
                        //check if direction is required (z3 is number of bits to use to send direction)
                        z3=2;
                        if (obji[sprlnk[z]].v4==3) z3=0;//"bird"
                        if (z==365) z3=0;//tanglevine core
                        if (z==374) z3=0;//hydra core
                        if (z==375) z3=0;//slime
                        if (z==391) z3=0;//farmer hoeing
                        if (z==415) z3=0;//raft
                        tplayer->mv_dir[i]=BITSget(t,&bitsi,z3);
mover_add_specialc:

                        tplayer->mv_flags[i]=0;
                        tplayer->mv_hpmp[i]=0;
                        tplayer->mv_playerid[i]=0;
                        tplayer->mv_ktar[i]=0;
                        tplayer->mv_more[i]=0;

mover_add_getstate_next: if (BITSget(t,&bitsi,1)){
                        z2=BITSget(t,&bitsi,4);

                        if (z2==0) tplayer->mv_flags[i]|=MV_LIGHTBRIGHT;
                        if (z2==1) tplayer->mv_flags[i]|=MV_LIGHTGLOW;
                        if (z2==2) tplayer->mv_flags[i]|=MV_INVISIBLE;
                        if (z2==3) tplayer->mv_flags[i]|=MV_PARALYZE;
                        if (z2==4) tplayer->mv_flags[i]|=MV_SLEEP;
                        if (z2==5) tplayer->mv_flags[i]|=MV_PROTECT;
                        if (z2==6) tplayer->mv_flags[i]|=MV_STORMCLOAK;
                        if (z2==7) tplayer->mv_flags[i]|=MV_TYPING;
                        if (z2==8){
                          tplayer->mv_hpmp[i]=BITSget(t,&bitsi,8);
                          tplayer->mv_playerid[i]=BITSget(t,&bitsi,32);
                        }
                        if (z2==9){
                          tplayer->mv_ktar[i]=BITSget(t,&bitsi,4);
                        }
                        if (z2==10) tplayer->mv_flags[i]|=MV_SPEAKING;

                        if (z2==11){
                          tplayer->mv_more[i]=BITSget(t,&bitsi,16);
                        }


                        goto mover_add_getstate_next;
                         }

                         tplayer->mv_i++;
                         goto mover_add_next;
                }

                tplayer=tplayer_backup;
                goto CLIENT_donemess;
      }//31

      if (t->d2[0]==32){//send time
        if (U6O_DEBUG) {
          scrlog("Time update via message 32.");
          txtset(t2,"Client time was: "); 
          txtset(t3,"                                ");
          _snprintf((char *)t3->d2,32,"%e",btime2);
          txtadd(t2,t3);
          scrlog((const char *)t2->d2);
        }
        txtright(t,8);
        if (!NEThost) btime2=t->dd[0];
        if (U6O_DEBUG) {
          txtset(t2,"Server time was: "); 
          txtset(t3,"                                ");
          _snprintf((char *)t3->d2,32,"%e",btime2);
          txtadd(t2,t3);
          scrlog((const char *)t2->d2);
        }
        goto CLIENT_donemess;
      }//send time

#ifdef SECURE
#include "secret_b.inc"
#endif

      if (t->d2[0]==33){//xray update
        xray=t->d2[1];
        goto CLIENT_donemess;
      }//xray update

      if (t->d2[0]==36){//wizardeyetimeleft update
        wizardeyetimeleft=t->d2[1];
        goto CLIENT_donemess;
      }//wizardeyetimeleft update




      if (t->d2[0]==254){ //incorrect version
        MessageBox(NULL,"U6O incorrect version, please download new version","Ultima 6 Online",MB_OK); setupfail=TRUE;
        goto CLIENT_donemess;
      }

      if (t->d2[0]==252){ //remove NPC from temp party
        x=t->d2[1];
        tplay->party[x]=NULL;
        if (party_spellbook_frame[x]->offset_x<1024) party_spellbook_frame[x]->offset_x+=2048;
        if (party_frame[x]->offset_x<1024) party_frame[x]->offset_x+=2048;
        qkstf_update=TRUE;
        goto CLIENT_donemess;
      }

      if (t->d2[0]==250){
        if (t->d2[1]==0) MessageBox(NULL,"Leaving Britannia (Return after 30 minutes to fully heal)","Ultima 6 Online",MB_OK);
        if (t->d2[1]==1) MessageBox(NULL,"Leaving Britannia (Host has quit)","Ultima 6 Online",MB_OK);
        if (t->d2[1]==2) MessageBox(NULL,"Invalid name, username or password","Ultima 6 Online",MB_OK);
        if (t->d2[1]==3) MessageBox(NULL,"Username already taken (Select a different username)","Ultima 6 Online",MB_OK);
        if (t->d2[1]==4) MessageBox(NULL,"Character name already taken (Select a different character name)","Ultima 6 Online",MB_OK);
        if (t->d2[1]==5) MessageBox(NULL,"Username Incorrect","Ultima 6 Online",MB_OK);
        if (t->d2[1]==6) MessageBox(NULL,"Password Incorrect","Ultima 6 Online",MB_OK);
        if (t->d2[1]==7) MessageBox(NULL,"Leaving Britannia (Administration forced you to leave)","Ultima 6 Online",MB_OK);
        if (t->d2[1]==8) MessageBox(NULL,"Leaving Britannia (Administration has currently banned your character)","Ultima 6 Online",MB_OK);
        if (t->d2[1]==9) MessageBox(NULL,"Leaving Britannia (Idle)","Ultima 6 Online",MB_OK);
        if (t->d2[1]==10) {MessageBox(NULL,"Voice Chat Disabled (Blocked by administration, your message was not sent to others)","Ultima 6 Online",MB_OK); goto CLIENT_donemess;}
        DestroyWindow( hWnd );
        goto CLIENT_donemess;
      }

      if (t->d2[0]==38){//receive voice message
        txtright(t,t->l-1);
localvoicemessage_begin:
        x2=t->ds[0]; txtright(t,t->l-2);//port
        txtNEWLEN(t5,-t->d2[0]); txtright(t,t->l-1); memcpy(t5->d,t->d,t5->l); txtright(t,t->l-t5->l);//name
        x3=t->ds[0]; txtright(t,t->l-2); y3=t->ds[0]; txtright(t,t->l-2);

        //***** PLAYER VOICE MESSAGE MUTING AND IGNORING *****
        txtset(t4,t5); txtucase(t4);
        tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
ignore_nextplayer2:
        txtfilein(t6,tfh);
        txtucase(t6);
        txtset(t7,"[MUTE]"); txtadd(t7,t4); if (txtsame(t6,t7)){
          txtNEWLEN(t,0);
          goto muted2;
        }
        txtset(t7,"[IGNORE]"); txtadd(t7,t4); if (txtsame(t6,t7)){
          if (localvoicemessage_return) {localvoicemessage_return=0; goto localvoicemessage_returnpoint;}
          goto CLIENT_donemess;
        }
        if (t6->l) goto ignore_nextplayer2;
        close(tfh);
muted2:;







        if (t->l){
          //1. build file
          tfh=open2(".\\voice\\voicein.spx",OF_READWRITE|OF_CREATE);
          put(tfh,t->d2,t->l);
          close(tfh);
          //2. decode sound
          deletefile(".\\voice\\voicein.wav");
          PROCESS_INFORMATION pi;
          STARTUPINFO si;
          ZeroMemory(&pi,sizeof(pi)); ZeroMemory(&si,sizeof(si));
          si.cb=sizeof(STARTUPINFO);
          si.dwFlags=STARTF_USESIZE|STARTF_USESHOWWINDOW;
          si.wShowWindow=SW_HIDE;
          CreateProcess(NULL,".\\voice\\speexdec.exe .\\voice\\voicein.spx .\\voice\\voicein.wav",NULL,NULL,FALSE, 0, NULL, NULL, &si, &pi);
          waitforfile(".\\voice\\voicein.wav");
          //3. load sound
          static sound *voicein;
          voicein=soundload(".\\voice\\voicein.wav");
          //4. play sound
          x4=u6ovolume;
          u6ovolume=u6ovoicevolume;
          soundplay2(voicein,255);//should reflect voice volume controls!!
          u6ovolume=x4;
          //5. delete (primary) sound
          voicein->s->Release();
          free((void*)voicein);
        }

        txtadd(t5," (SPEAKING)?"); t5->d2[t5->l-1]=255;

        //add sfx message
        for (i3=0;i3<=255;i3++){ if (sfx[i3].type==0){
          sfx[i3].type=3;
          sfx[i3].x=x3;
          sfx[i3].y=y3-2;
          sfx[i3].x2=t5->l;
          static txt *dbgt5;
          dbgt5=txtnew();
          txtset(dbgt5,t5);
          sfx[i3].p=dbgt5;
          z=t5->l;
          f=(float)z/10.0f;
          if (f<1)f=1;
          if(f>4)f=4;
          sfx[i3].wait=f;
          sfx[i3].more=x2;
shiftdown4:
          for (i2=0;i2<=255;i2++){ if ((sfx[i2].type==3)&&(i2!=i3)){
            if ((sfx[i3].y==sfx[i2].y)||(sfx[i3].y==(sfx[i2].y+1))||(sfx[i3].y==(sfx[i2].y-1))) {sfx[i3].y=sfx[i2].y+2; goto shiftdown4;}
          }}

          if (localvoicemessage_return) {localvoicemessage_return=0; goto localvoicemessage_returnpoint;}
          goto CLIENT_donemess;
        }}
        if (localvoicemessage_return) {localvoicemessage_return=0; goto localvoicemessage_returnpoint;}
        goto CLIENT_donemess;
      }//38

      if (t->d2[0]==41){//receive volume levels of nearby players
        txtright(t,t->l-1);
receivenextvolume:
        x2=t->dl[0]; txtright(t,t->l-4);//id
        if (x2==0xFFFFFFFF){//ID not being sent, use static position
          voicechat_listeningplayeri++;
          voicechat_listeningplayerx[voicechat_listeningplayeri]=t->ds[0]; txtright(t,t->l-2);
          voicechat_listeningplayery[voicechat_listeningplayeri]=t->ds[0]; txtright(t,t->l-2);




          x3=t->d2[0]; txtright(t,t->l-1);//volume
          voicechat_listeningplayervolume[voicechat_listeningplayeri]=x3;
        }else{
          x3=t->d2[0]; txtright(t,t->l-1);//volume
          if (idlstn!=-1){
            for (z2=0;z2<=idlstn;z2++){
              if (x2==idlst[z2]){
                idlst_volume[z2]=x3;
              }//x2==idlst[z2]
            }//z2
          }//!=-1
        }
        voicechat_listeningplayers|=1;
        if (x3) voicechat_listeningplayers|=2;//player can hear voice
        if (t->l) goto receivenextvolume;
        goto CLIENT_donemess;
      }//41

      if (t->d2[0]==43){//receive portrait data from host
        txtright(t,t->l-1);
        x2=t->ds[0];
        txtright(t,t->l-2);

        static surf *receiveport=NULL;
        receiveport=newsurf(56,64,SURF_SYSMEM16);


        /*
        LOSS-LESS COMPRESS PORTRAIT DATA
        loop:
        2bits    (V0-V3)

        V0 repeat last known (last known starts off as black)
        {
        5bits    1-32 repeats
        loop
        }

        V1 can't be created
        {
        5:5:5bits    fixed colour
        loop
        }

        V2 accend
        {
        3bits    blue
        3bits    green
        3bits    red
        loop
        }

        V3 decend
        {
        3bits    blue
        3bits    green
        3bits    red
        loop
        }

        */
        static unsigned long bitsi;
        static long r,g,b,r2,g2,b2,r3,g3,b3,repeat;

        bitsi=0;
        b=0; g=0; r=0; repeat=0;

        x3=0;
lluc_nextpixel:
        y2=BITSget(t,&bitsi,2);

        if (y2==0){
          y3=BITSget(t,&bitsi,5)+1;
          for (y4=1;y4<=y3;y4++){
            receiveport->o2[x3]=b+(g<<6)+(r<<11); x3++;
          }//y4
        }//y2==0

        if (y2==1){
          b=BITSget(t,&bitsi,5);
          g=BITSget(t,&bitsi,5);
          r=BITSget(t,&bitsi,5);
          receiveport->o2[x3]=b+(g<<6)+(r<<11); x3++;
        }//y2==1

        if (y2==2){
          b+=BITSget(t,&bitsi,3); if (b>31) b=b-32;
          g+=BITSget(t,&bitsi,3); if (g>31) g=g-32;
          r+=BITSget(t,&bitsi,3); if (r>31) r=r-32;
          receiveport->o2[x3]=b+(g<<6)+(r<<11); x3++;
        }//y2==2

        if (y2==3){
          b-=BITSget(t,&bitsi,3); if (b<0) b=b+32;
          g-=BITSget(t,&bitsi,3); if (g<0) g=g+32;
          r-=BITSget(t,&bitsi,3); if (r<0) r=r+32;
          receiveport->o2[x3]=b+(g<<6)+(r<<11); x3++;
        }//y2==3

        if (x3<3584) goto lluc_nextpixel;

        loadportrait(x2,receiveport);
        if (myobj=tplay->party[0]){
          tnpc=(npc*)myobj->more;
          if (tnpc->port==x2) tnpc->upflags|=1;
        }//tplay->party[0]
        goto CLIENT_donemess;
      }//43

      if (t->d2[0]==44){//receive portrait look message
        txtright(t,t->l-1);
        portraitlook_wait=8.0f;
        if (statusmessage_viewnpc->offset_x>=1024) statusmessage_viewnpc->offset_x-=2048;
        portraitlook_portrait=t->ds[0]; txtright(t,t->l-2);
        if (portraitlook_name==NULL) portraitlook_name=txtnew();
        txtNEWLEN(portraitlook_name,-t->d2[0]); txtright(t,t->l-1);
        memcpy(portraitlook_name->d,t->d,portraitlook_name->l); txtright(t,t->l-portraitlook_name->l);
        portraitlook_namecolour=t->dl[0]&0xFFFFFF; txtright(t,t->l-3);
        portraitlook_equip=t->d2[0]; txtright(t,t->l-1);
        x3=1;
        for (x2=0;x2<8;x2++){
          if (portraitlook_equip&x3){
            portraitlook_type[x2]=t->ds[0]; txtright(t,t->l-2);
            portraitlook_plusbonus[x2]=t->d2[0]; txtright(t,t->l-1);
          }else{
            portraitlook_type[x2]=0;
          }
          x3<<=1;
        }

        if (portraitlook_equip){
          statusmessage_viewnpc->graphic=viewnpc_temp;
          img(viewnpc_temp,0,0,viewnpc);
        }else{
          statusmessage_viewnpc->graphic=viewnpc2_temp;
          img(viewnpc2_temp,0,0,viewnpc2);
          if (statusmessage_viewnpc->offset_x<=-112) statusmessage_viewnpc->offset_x=-112+8;
        }
        img0(statusmessage_viewnpc->graphic,0,0,getportrait_doublesize(portraitlook_portrait));







        goto CLIENT_donemess;
      }//44 receive portrait look message


      if (t->d2[0]==46){//receive not4sale info
        not4sale_flags[t->d2[1]]=t->ds[1];
        goto CLIENT_donemess;
      }//46 receive not4sale info

      if (t->d2[0]==48){//peer update
        peer=t->d2[1];
        goto CLIENT_donemess;
      }//peer update

      if (t->d2[0]==49){//treasure map look message
        tmap=32;
        txtright(t,t->l-1);
        //treasuremap
        x3=(t->dl[0])&1023;
        y3=(t->dl[0]>>10)&1023;
        x4=(t->dl[0]>>20)&7;
        y4=(t->dl[0]>>23)&7;
        x3+=(x4-3)*12;
        y3+=(y4-3)*12;
        if(x3<0) {x3=0;}
        if(x3>=1024) {x3=1023;}
        if(y3<0) {y3=0;}
        if(y3>=1024) {y3=1023;}
        img(ps7,-(x3),-(y3),treasuremap);
        img(tmap_marker,0,0,tmap_markers);
        img0(ps7,60-(x4-3)*12,60-(y4-3)*12,tmap_marker);
        img(ps8,ps7);
        img0(tmap_frame->graphic,8,8,ps8);
        goto CLIENT_donemess;
      }//treasure map look message

    } //l!=0
CLIENT_donemess:



    //if (NEThost) goto U6Ohostlink1return;

    goto CLIENT_readnext;
  } //read local message
  //} //client (+host link)


















































































































































































































  u6opi=3;





  if (clientframe&&tplay->x){
    directionalmove_only=0;




    static object* fakeobj=OBJnew_local();

    tplayer=tplay;


    x=ett;
    x=x%24; //x now between 0 and 23
    if (x>12) x=24-(12-(x-12));
    f=float(x)*1.25;
    x5=f;
    x5=0;
    if (btime>=22) x5=15;
    if (btime<=3) x5=15;
    if ((btime>=7)&&(btime<=18)) x5=0;
    if ((btime>=3)&&(btime<=7)){ //sunrise
      f=15.0f-((btime-3.0f)/4.0f*15.0f); //0->15
      x5=f;
    }
    if ((btime>=18)&&(btime<=22)){ //sunset
      f=(btime-18.0f)/4.0f*15.0f; //0->15
      x5=f;
    }
    timelval=x5;

    if (getambientlight(tplay->x,tplay->y)){
      timelval=AMBIENTLIGHT_LIGHTVALUE;
    }




    if (wizardeyetimeleft) timelval=0;

    if (nodisplay) goto skiprefresh2;


    if (moonlight==0) ZeroMemory(&ls,sizeof(ls)); //clear array
    if (moonlight==1) memcpy(&ls,&ls_moon1,1024*768);
    if (moonlight==2) memcpy(&ls,&ls_moon2,1024*768);
    if (moonlight==3) memcpy(&ls,&ls_moon3,1024*768);
    if (moonlight==4) memcpy(&ls,&ls_moon4,1024*768);

    //calculate tpx,tpy from current x,y
    getscreenoffset(tplayer->x,tplayer->y,&tpx,&tpy);




    keyframe_backup=keyframe;
    oceantiles=0; rivertiles=0; britlens=0; garglens=0;

    osn=-1;
    stormcloak_last2=-1;




    //init vis arrays
    for (y=0;y<=27;y++){ for (x=0;x<=35;x++){
      vis[x][y]=1;
      vis_window[x][y]=0;
      vis_chair[x][y]=0;
      vis_bed[x][y]=0;
      vis_slime[x][y]=0;
    }}
    //get vis
    static long mapx,mapy,bufx,bufy;
    static unsigned short *tp2;
    myobj=fakeobj;
    for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
      x2=tpx+x-1; y2=tpy+y-1; if ((x2>=0)&&(y2>=0)&&(x2<=2047)&&(y2<=1023)){
        mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
        //get basetile-vis
        i=bt[y2][x2]&1023;
        i2=0;//visible
        if ((i>=192)&&(i<=207)) vis_window[x+1][y+1]=1;//window basetile
        if ((i>=140)&&(i<188)) i2=1;
        if ((i>=240)&&(i<252)) i2=1;
        if ((i>=192)&&(i<208)) i2=1;
        vis[x+1][y+1]=i2;
        //get fixedobj-vis
        if (i=objfixed_index[tpy+y-1][tpx+x-1]){
          i2=objfixed_type[i];
          for (i3=i+1;i3<=(i+i2);i3++){
            x3=objfixed_type[i3];
            myobj->type=x3;
            if ((x3>=29)&&(x3<=32)){//overlay floor: edit basetile 
              if (x3==29) x4=108;
              if (x3==30) x4=208;
              if (x3==31) x4=210;
              if (x3==32) x4=213;
              bt[y2][x2]&=(0xFFFF-1023);
              bt[y2][x2]|=x4;
            }
            if ((myobj->type&1023)==301){
              if ((myobj->type&1024)==0) vis[x+1][y+1]=1;//top/left of doorframe always blocks
            }
            if ((myobj->type&1023)==213) vis[x+1][y+1]=1;//mousehole
            if (myobj->type==(327+2048)) vis_chair[x+1][y+1]=3;//throne
            if (myobj->type==163) vis_bed[x+1][y+1]=1;//bed (horizontal)
            if (myobj->type==163+(6*1024)) vis_bed[x+1][y+1]=2;//bed (vertical)
          }//i3
        }//i
        //get tfixedobj-vis
        if (i=tobjfixed_index[tpy+y-1][tpx+x-1]){
          i2=tobjfixed_type[i];
          i5=1;
          for (i3=i+1;i3<=(i+i2);i3++){
            if ((tplayer->sobj_tempfixed[bufx][bufy]&i5)==0){
              x3=tobjfixed_type[i3];
              myobj->type=x3;
              x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
              if ((x5==334)&&((x4&1024)==0)) vis[x+1][y+1]=1;//secret door(closed)
              if ((myobj->type&1023)==298) vis_window[x+1][y+1]=1;//windowed door
              if ((myobj->type&1023)==252) vis_chair[x+1][y+1]=(myobj->type>>10)+1;//chair
              if ((x3&1023)!=x3){ //possibly buildable
                if (tclass_build[x3-1024]&2){//horizontal
                  x2--;
                  x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                  x2++;
                }
                if (tclass_build[x3-1024]&4){//vertical
                  y2--;
                  x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                  y2++;
                }
              }//build
            }//&i5
            i5<<=1;
          }//i3
        }//i
        //get objectbuffer-vis
        if (i=tplayer->sobj_bufsize[bufx][bufy]){
          tp2=tplayer->sobj[bufx][bufy];
          for (i2=0;i2<i;i2++){
            x3=tp2[i2];
            myobj->type=x3;
            x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
            if ((x5==334)&&((x4&1024)==0)) vis[x+1][y+1]=1;//secret door(closed)
            if ((myobj->type&1023)==252) vis_chair[x+1][y+1]=(myobj->type>>10)+1;//chair
            if ((x3&1023)!=x3){ //possibly buildable!
              if (tclass_build[x3-1024]&2){//horizontal
                x2--;
                x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                x2++;
              }
              if (tclass_build[x3-1024]&4){//vertical
                y2--;
                x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                y2++;
              }
            }//build
          }//i2
        }//i

        //get visalways
        x3=x2>>3; x4=x2&7;
        if (visalways[x3][y2]&(1<<x4)){
          vis[x+1][y+1]=0;
        }
        //get x,y vis complete
      }else vis[x+1][y+1]=1;
    }}

    if (xray){
      for (y=1;y<=26;y++){ for (x=1;x<=34;x++){
        vis[x][y]=4;
      }}
      goto viewfind_skip;
    }

    //pathfind
    static unsigned char vis_index_x[4096];
    static unsigned char vis_index_y[4096];
    i=-1; //last index set
    i2=0; //last index checked
    mapx=tpx-2; mapy=tpy-2; x=tplayer->x-mapx; y=tplayer->y-mapy;
    //add windows
    if (vis_window[x][y-1]) vis[x][y-1]=0;
    if (vis_window[x+1][y]) vis[x+1][y]=0;
    if (vis_window[x][y+1]) vis[x][y+1]=0;
    if (vis_window[x-1][y]) vis[x-1][y]=0;
    i++; vis_index_x[i]=x; vis_index_y[i]=y;
vis_scan2:
    x=vis_index_x[i2]; y=vis_index_y[i2]; 
    if (vis[x-1][y]==0){i++; vis_index_x[i]=x-1; vis_index_y[i]=y; vis[x-1][y]=4;}
    if (vis[x+1][y]==0){i++; vis_index_x[i]=x+1; vis_index_y[i]=y; vis[x+1][y]=4;}
    if (vis[x][y-1]==0){i++; vis_index_x[i]=x; vis_index_y[i]=y-1; vis[x][y-1]=4;}
    if (vis[x][y+1]==0){i++; vis_index_x[i]=x; vis_index_y[i]=y+1; vis[x][y+1]=4;}
    if (i2<i){i2++; goto vis_scan2;}
    //0=visible, but blocked by somthing else
    //1=blocked
    //2=window
    //4=visible
    //8=edge visible
    for (y=1;y<=26;y++){ for (x=1;x<=34;x++){ //find visible edges
      i=vis[x][y];
      if (i==4){
        if (vis[x+1][y]!=4) vis[x+1][y]=8;
        if (vis[x-1][y]!=4) vis[x-1][y]=8;
        if (vis[x][y+1]!=4) vis[x][y+1]=8;
        if (vis[x][y-1]!=4) vis[x][y-1]=8;
        if (vis[x-1][y-1]!=4) vis[x-1][y-1]=8;
        if (vis[x+1][y-1]!=4) vis[x+1][y-1]=8;
        if (vis[x+1][y+1]!=4) vis[x+1][y+1]=8;
        if (vis[x-1][y+1]!=4) vis[x-1][y+1]=8;
      }
    }}
viewfind_skip:

    //get vischeck
    ZeroMemory(&vischeck,sizeof(vischeck));
    for (y=0;y<=23;y++){ for (x=0;x<=31;x++){
      if (vis[x+2][y+2]&4) vischeck[x][y]=1;
      if (vis[x+2][y+2]&8){
        if (vis[x+3][y+2]&4) vischeck[x][y]=1;
        if (vis[x+3][y+3]&4) vischeck[x][y]=1;
        if (vis[x+2][y+3]&4) vischeck[x][y]=1;
      }//&8
    }}//x,y

    //base tiles
    for (y=0;y<=23;y++){ for (x=0;x<=31;x++){
      i=bt[y+tpy][x+tpx]&1023;
      z=0;
      if ((i>=221)&&(i<224)) z=3; //lava
      if ((i>=2)&&(i<6)) z=3; //swamp
      if (z==3) LIGHTnew(x,y,(unsigned long)&ls3b,3);
      if ((i>=8)&&(i<48)){ //ocean and coast
        if (i<=15) oceantiles++; else rivertiles++;
        x2=i&7;
        y2=i/8;
        i2=i-8;
        x4=0; if (i2>=8) {i2=wateri[i2-8]; x4=1; }
        i2=i2*8+keyframe;
        x3=i2&31;
        y3=i2/32;
        y3+=4;
        sf32(ps,x*32,y*32,sfx8,i2+128);
        if (x4==1) g32z(ps,x*32,y*32,bt8[0],i);
      }else{//not ocean
        i2=0;
        if (i==252){i2=keyframe; i=14;}
        if (i==253){i2=keyframe; i=15;}
        if (i==254){i2=keyframe; i=0;}
        if ((i>=221)&&(i<=223)){i2=keyframe; i=i-210;}
        if ((i>=217)&&(i<=219)){i2=keyframe; i=i-209;}
        if (i<=7){i2=keyframe;} //changed
        g32(ps,x*32,y*32,bt8[i2],i);
      }
    }}

    if (oceantiles||rivertiles){
      if (oceantiles>=576){
        f=9.0f; if (int((ett/f))!=int((ett/f)-(et/f))) soundplay2(u6osound[29],u6osound_volume[29]);
      }else{
        f=4.0f; if (int((ett/f))!=int((ett/f)-(et/f))) soundplay2(u6osound[9],u6osound_volume[9]);
      }
    }


    //minimap tiles its done in a VERY stupid way, but looks like the asm code doesn't like other size surfaces, so this will have to do for now.
    if(peer) {
      for (x5=0;x5<4;x5++) { for (y5=0;y5<4;y5++) {
        for (y=y5*24;y<24+y5*24;y++){ for (x=x5*24;x<24+x5*24;x++){
          if (((y+tpy-35)>=1024)||((y+tpy-35)<0)||((x+tpx-31)>=2048)||((x+tpx-31)<0)) {
            i=0;
          }
          else {
            i=bt[y+tpy-35][x+tpx-31]&1023;
          }
          if ((i>=8)&&(i<48)){ //ocean and coast
            if (i<=15) oceantiles++; else rivertiles++;
            i2=i-8;
            x4=0; if (i2>=8) {i2=wateri[i2-8]; x4=1; }
            sf32(ps5,(x-x5*24)*32,(y-y5*24)*32,sfx8,i2+128);
            if (x4==1) g32z(ps5,(x-x5*24)*32,(y-y5*24)*32,bt8[0],i);
          }else{//not ocean
            i2=0;
            if (i==252){i=14;}
            if (i==253){i=15;}
            if (i==254){i=0;}
            if ((i>=221)&&(i<=223)){i=i-210;}
            if ((i>=217)&&(i<=219)){i=i-209;}
            g32(ps5,(x-x5*24)*32,(y-y5*24)*32,bt8[0],i);
          }
        }}
        img(ps6,ps5);
        img0(minimap_frame->graphic,8+60*x5,8+60*y5,ps6);
      }}
    }




    //objfixed (non floating)
    myobj=fakeobj;
    for (y=25;y>=0;y--){ for (x=33;x>=0;x--){
      x2=x-1; y2=y-1;
      mapx=tpx+x-1; mapy=tpy+y-1; if ((mapx!=2048)&&(mapy!=1024)){

        if (i=objfixed_index[tpy+y-1][tpx+x-1]){
          i2=objfixed_type[i];
          for (i3=i+1;i3<=(i+i2);i3++){
            x3=objfixed_type[i3];
            myobj->type=x3;






            if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
              if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                GSs=1; GSx=x2*32; GSy=y2*32;
                getspr(myobj);
                getsound(myobj->type,x2,y2);
                getlight(myobj->type,x2,y2);
              }
            }}}}






            if ((x3&1023)!=x3){ //possibly buildable!
              if (tclass_build[x3-1024]&1){//square
                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-1024;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-2048;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-3072;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2++;
              }
              if (tclass_build[x3-1024]&2){//horizontal
                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-1024;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++;
              }
              if (tclass_build[x3-1024]&4){//vertical
                y2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-1024;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                y2++;
              }
            }//build
          }//i3
        }//i

      }//mapx,mapy
    }}//x,y



    //tobjfixed
    myobj=fakeobj;
    for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
      x2=x-1; y2=y-1;
      mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
      if ((mapx!=2048)&&(mapy!=1024)){

        if (i=tobjfixed_index[tpy+y-1][tpx+x-1]){
          i2=tobjfixed_type[i];
          i5=1;
          for (i3=i+1;i3<=(i+i2);i3++){
            if ((tplayer->sobj_tempfixed[bufx][bufy]&i5)==0){

              x3=tobjfixed_type[i3];
              myobj->type=x3;

              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){




                if (refreshcount&1){ for (x4=0;x4<=stolenitemwarningn;x4++){ if (stolenitemwarningx[x4]==mapx){ if (stolenitemwarningy[x4]==mapy){ if (stolenitemwarningtype[x4]==myobj->type){
                  static unsigned long *ps_realoffset;
                  static unsigned short ps_fakebuffer[1024*32];
                  static long siw_x,siw_y,siw_r,siw_g,siw_b;
                  for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                    ps_fakebuffer[siw_y*1024+siw_x]=0;
                  }}
                  ps_realoffset=ps->o;
                  ps->o=(unsigned long*)&ps_fakebuffer;
                  GSs=1; GSx=0; GSy=0;
                  getspr(myobj);
                  ps->o=ps_realoffset;
                  for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                    if (x5=ps_fakebuffer[siw_y*1024+siw_x]){
                      x6=x5>>11;
                      x7=(x5&0x7E0)>>6; if (x7>x6) x6=x7;
                      x7=x5&0x1F; if (x7>x6) x6=x7;
                      x6=x6<<11;
                      ps->o2[y2*1024*32+siw_y*1024+x2*32+siw_x]=x6;
                    }//x5
                  }}
                  goto stolenitemwarningflash;
                }}}}}

                //endgame lenses
                if (myobj->type==394){
                  if (endgame){
                    if (endgame_message>=7) goto lens_hide;
                  }
                  britlens=1;
                }
                if (myobj->type==396){
                  if (endgame){
                    if (endgame_message>=12) goto lens_hide;
                  }
                  garglens=1;
                }


                GSs=1; GSx=x2*32; GSy=y2*32;
                getspr(myobj);
stolenitemwarningflash:
lens_hide:






                getsound(myobj->type,x2,y2);
                getlight(myobj->type,x2,y2);

              }}}}





              if ((x3&1023)!=x3){ //possibly buildable!


                if (tclass_build[x3-1024]&1){//square

                  x2--;
                  if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                    myobj->type=x3-1024;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}
                  x2++; y2--;
                  if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                    myobj->type=x3-2048;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}
                  x2--;
                  if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                    myobj->type=x3-3072;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}
                  x2++; y2++;
                }
                if (tclass_build[x3-1024]&2){//horizontal
                  x2--;
                  if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                    myobj->type=x3-1024;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}

                  x2++;
                }
                if (tclass_build[x3-1024]&4){//vertical
                  y2--;
                  if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                    myobj->type=x3-1024;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}

                  y2++;
                }
              }//build
            }//&i5
            i5<<=1;
          }//i3
        }//i

      }//mapx,mapy
    }}//x,y



    x5option++; if (x5option>7) x5option=0;

    //objbuffer
    myobj=fakeobj;
    for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
      mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
      x2=x-1; y2=y-1;
      if (i=tplayer->sobj_bufsize[bufx][bufy]){
        tp2=tplayer->sobj[bufx][bufy];
        for (i2=0;i2<i;i2++){
          x3=tp2[i2];
          myobj->type=x3;

          x5=0;//flash?
          if (myobj->type&32768){
            if ((sprlnk[(myobj->type+1)&1023]-sprlnk[myobj->type&1023])>32) goto flash_disable2;
            myobj->type-=32768; x3=myobj->type;

            x5=1; //goto flash_skip2;
          }
flash_disable2:

          if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
            if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating


              getsound(myobj->type,x2,y2);
              getlight(myobj->type,x2,y2);



              if (refreshcount&1){ for (x4=0;x4<=stolenitemwarningn;x4++){ if (stolenitemwarningx[x4]==mapx){ if (stolenitemwarningy[x4]==mapy){ if (stolenitemwarningtype[x4]==myobj->type){
                static unsigned long *ps_realoffset;
                static unsigned short ps_fakebuffer[1024*32];
                static long siw_x,siw_y,siw_r,siw_g,siw_b;
                for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                  ps_fakebuffer[siw_y*1024+siw_x]=0;
                }}
                ps_realoffset=ps->o;
                ps->o=(unsigned long*)&ps_fakebuffer;
                GSs=1; GSx=0; GSy=0;
                getspr(myobj);
                ps->o=ps_realoffset;
                for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                  if (x5=ps_fakebuffer[siw_y*1024+siw_x]){
                    x6=x5>>11;
                    x7=(x5&0x7E0)>>6; if (x7>x6) x6=x7;
                    x7=x5&0x1F; if (x7>x6) x6=x7;
                    x6=x6<<11;
                    ps->o2[y2*1024*32+siw_y*1024+x2*32+siw_x]=x6;
                  }//x5
                }}
                goto stolenitemwarningflash2;
              }}}}}


              if (x5){
                if (myobj->type==(84+1024)){ getspr(myobj); imgt0(ps,x2*32,y2*32,bt32); goto generatinggate1;}
              }

              if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }



generatinggate1:;

stolenitemwarningflash2:;
            }
          }}}}
          if ((x3&1023)!=x3){ //possibly buildable
            if (tclass_build[x3-1024]&1){//square
              x2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-1024;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }


                }
              }}}}
              x2++; y2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-2048;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

                }
              }}}}
              x2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-3072;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

                }
              }}}}
              x2++; y2++;
            }
            if (tclass_build[x3-1024]&2){//horizontal
              x2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-1024;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating

                  if (x5){
                    if (myobj->type==84){ getspr(myobj); imgt0(ps,x2*32,y2*32,bt32); goto generatinggate0;}
                  }


                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

generatinggate0:;

                }
              }}}}
              x2++;
            }
            if (tclass_build[x3-1024]&4){//vertical
              y2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-1024;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

                }
              }}}}
              y2++;
            }
          }//build
          //flash_skip2:;
        }//i2
      }//i
    }}//objbuffer end












    //PASS 0: tanglevine tendrils ONLY
    //PASS 1: non-float
    //PASS 2: float
    static unsigned char flash_skip;
    for (z=0;z<=2;z++){//pass
      for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
        mapx=tpx+x-1; mapy=tpy+y-1;
        for (i=0;i<tplayer->mv_i;i++){
          if (tplayer->mv_x[i]==mapx){ if (tplayer->mv_y[i]==mapy){
            x2=x-1; y2=y-1;
            x3=objgettype(tplayer->mv_type[i],tplayer->mv_dir[i],tplayer->mv_frame[i]);
            myobj->type=x3;

            //non-pass specific changes
            if (tplayer->mv_flags[i]&MV_PARALYZE) keyframe=0;
            flash_skip=0; if (tplayer->mv_flags[i]&MV_INVISIBLE){

              flash_skip=1;
            }
            //check if sleeping
            if (tplayer->mv_flags[i]&MV_SLEEP){
              z3=myobj->type&1023; z2=0;
              if (z3==376) z2=339+6*1024;
              if (z3==377) z2=339+5*1024;
              if (z3==378) z2=339+3*1024;
              if (z3==379) z2=339+2*1024;
              if (z3==380) z2=339+2*1024;
              if (z3==381) z2=339+8*1024;
              if (z3==382) z2=339+4*1024;
              if (z3==383) z2=339+8*1024;
              if (z3==384) z2=339+5*1024;
              if (z3==385) z2=339+8*1024;
              if (z3==386) z2=339+7*1024;
              if (z3==387) z2=339+3*1024;
              if (z3==410) z2=339+7*1024;
              if (z3==409) z2=myobj->type;//LB FIX
              if (z2){
                if (vis_bed[x2+2][y2+2]==1){
                  myobj->type=146; x3=myobj->type; goto moverinbed;
                }//1
                if (vis_bed[x2+2][y2+2]==2){
                  myobj->type=146+1024; x3=myobj->type; goto moverinbed;
                }//2
              }//z2
              //monsters
              if (z3==370) z2=339+1*1024;//headless
              if (z3==371) z2=339+0*1024;//troll
              if (z3==424) z2=340+3*1024;//cyclops
              if (z3==362) z2=341+3*1024;//winged gargoyle
              if (z2){myobj->type=z2; x3=z2;}
            }//MV_SLEEP
moverinbed:
            //end: non-pass specific changes




            //first pass only changes
            if (z==0){
              if ((x3&1023)==375) vis_slime[x+1][y+1]=1;//slime vis check
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                if (vischeck[x2][y2]){
                  if (tplayer->mv_flags[i]&MV_LIGHTBRIGHT) LIGHTnew(x2,y2,(unsigned long)&ls13,13);
                  if (tplayer->mv_flags[i]&MV_LIGHTGLOW) LIGHTnew(x2,y2,(unsigned long)&ls5b,5);
                  if (tplayer->mv_flags[i]&MV_STORMCLOAK){stormcloak_last2++; stormcloak_x2[stormcloak_last2]=x2; stormcloak_y2[stormcloak_last2]=y2;}
                  if (tplayer->mv_playerid[i]){
                    if ((mapx==tplayer->x)&&(mapy==tplayer->y)){
                      //423 balloon
                      //414 skiff
                      //412 ship
                      //415 raft
                      z3=myobj->type&1023;
                      if ((z3==423)||(z3==414)||(z3==412)||(z3==415)){
                        directionalmove_only=1;
                      }//z3
                    }//(mapx==tplayer->x)&&(mapy==tplayer->y)
                    z3=0;//display name
                    if (idlstn!=-1){
                      for (z2=0;z2<=idlstn;z2++){
                        if (tplayer->mv_playerid[i]==idlst[z2]){
                          osn++; osx[osn]=x2*32+16; osy[osn]=y2*32; osi[osn]=z2;
                          if ((x3&1023)==412) osy[osn]-=32;
                          if ((x3&1023)==423) osy[osn]-=32;
                          //horse edit
                          if (tplayer->mv_type[i]==431){//horse&rider
                            if (tplayer->mv_dir[i]==1){
                              osx[osn]-=16;
                              osy[osn]-=12;
                            }
                            if (tplayer->mv_dir[i]==3){
                              osx[osn]+=16;
                              osy[osn]-=12;
                            }
                            if (tplayer->mv_dir[i]==2){
                              osy[osn]-=32;
                            }
                          }


                          oshpmp[osn]=tplayer->mv_hpmp[i];
                          z3=1;
                          osvol[osn]=-1; if (idlst[z2]!=clientplayerid) osvol[osn]=idlst_volume[z2];
                        }
                      }//z2 
                    }//!=-1
                    if (z3==0){ //add new id
                      idlstn++;
                      idlst[idlstn]=tplayer->mv_playerid[i];
                      idlst_name[idlstn]=txtnew();
                      txtset(idlst_name[idlstn],"reading data");
                      idlst_namecolour[idlstn]=0xFFFFFF;
                      txtset(t2,"?"); t2->d2[0]=9; txtset(t3,"????"); t3->dl[0]=tplayer->mv_playerid[i]; txtadd(t2,t3); NET_send(NETplayer,NULL,t2);
                    }//z3==0
                    if (tplayer->mv_flags[i]&MV_TYPING){
                      osn++;
                      osx[osn]=x2*32+16;
                      osy[osn]=y2*32;
                      osi[osn]=16+1024;
                    }//tplayer->MV_TYPING

                    if (tplayer->mv_flags[i]&MV_SPEAKING){
                      osn++;
                      osx[osn]=x2*32+16;
                      osy[osn]=y2*32;
                      osi[osn]=17+1024;
                    }//tplayer->MV_SPEAKING


                  }//playedid
                  if (tplayer->mv_ktar[i]){
                    if (ktar_display){
                      osn++;
                      osx[osn]=x2*32+16;
                      osy[osn]=y2*32;
                      osi[osn]=tplayer->mv_ktar[i]-1+1024;
                    }
                  }//ktar

                }//vischeck
              }}}}

            }//z==0
            //end: first pass only changes


            //PASS 0: selected movers only
            if (z==0){
              if ((x3&1023)==366) goto passok;//tanglevine tendril
              if ((x3&1023)==414) goto passok;//skiff
              goto passskip;
            }
            if (z==1){
              if ((x3&1023)==366) goto passskip;//tanglevine tendril
              if ((x3&1023)==414) goto passskip;//skiff
            }







passok:









            if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
              if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))||(z==0)){//floating check

                if (vis_chair[x2+2][y2+2]){
                  if (obji[sprlnk[myobj->type&1023]].v4==1){//"avatar"
                    if ((myobj->type&1023)!=363){//not a gargoyle!
                      myobj->type=objgettype(tplayer->mv_type[i],vis_chair[x2+2][y2+2]-1,3);
                    }
                  }
                }


                if ((x3&1023)==375){//slime
                  x4=0;
                  if (vis_slime[x+1][y+1-1]) x4|=1;
                  if (vis_slime[x+1+1][y+1]) x4|=2;
                  if (vis_slime[x+1][y+1+1]) x4|=4;
                  if (vis_slime[x+1-1][y+1]) x4|=8;
                  myobj->type=375+x4*1024; 
                }//slime

                if (flash_skip==0){
                  GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);
                }else{
                  getspr(myobj); imgt0(ps,x2*32,y2*32,bt32);
                }

                getsound_MOVERSOUND=1; getsound(myobj->type,x2,y2); getsound_MOVERSOUND=0;
                getlight(myobj->type,x2,y2);

              }
            }}}}





            if ((x3&1023)!=x3){ //possibly buildable!


              if (x3==(340+3*1024)) goto mover_square;
              if (x3==(341+3*1024)) goto mover_square;
              if (tclass_build[x3-1024]&1){//square
mover_square:


                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-1024;
                  if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-2048;
                  if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-3072;
                  if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2++;
              }
            }//build
            if (tclass_mover[x3]&2){//2 part mover (cow,ant,etc.)
              x4=(x3>>11)&3;
              if (x4==0) y2++;
              if (x4==1) x2--;
              if (x4==2) y2--;
              if (x4==3) x2++;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3+1024*8;

                x8=0;//double headed animal?
                x7=mapx; y7=mapy;
                if ((x7>=747)&&(x7<=829)&&(y7>=905)&&(y7<=983)) x8=1; //on island
                if ((x7>=792)&&(x7<=796)&&(y7>=937)&&(y7<=938)) x8=0; //in cage
                if ((x7==1475)&&(y7==244)) x8=1; //pushmepullyou
                if (x8){
                  x8=myobj->type>>11;
                  x8-=4;
                  if (x8==0) x8=2; else if (x8==2) x8=0; else if (x8==1) x8=3; else if (x8==3) x8=1;
                  myobj->type=(myobj->type&2047)+x8*2048;
                }//x8

                if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check

                  if (flash_skip==0){
                    GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);
                  }else{
                    getspr(myobj); imgt0(ps,x2*32,y2*32,bt32);
                  }


                }
              }}}}
              if (x4==0) y2--;
              if (x4==1) x2++;
              if (x4==2) y2++;
              if (x4==3) x2--;
            }//2 part mover

            if ((x3&1023)==412){//ship
              if (z==2){//float
                x4=(x3>>11)&3; x5=x2; y5=y2;
                if (x4==0){
                  myobj->type=x3-1*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5-1; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==2){
                  myobj->type=x3-1*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5-1; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==1){
                  myobj->type=x3-1*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5+1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==3){
                  myobj->type=x3-1*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5+1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                x2=x5; y2=y5;
              }//float
            }//boat

            if ((x3&1023)==411){//dragon
              if (z==2){//float
                //use random
                x4=(x3>>11)&3; x5=x2; y5=y2;
                x6=0; //head (should change from 0 to 1 randomly)
                y6=0; //tail
                y6=ett+(float)x2+(float)y2; y6&=1;
                x6=ett/1.4f+(float)x2+(float)y2; x6&=1;

                if (x4==0){
                  myobj->type=x3+8*1024+x6*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==1){
                  myobj->type=x3+8*1024+x6*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==2){
                  myobj->type=x3+8*1024+x6*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==3){
                  myobj->type=x3+8*1024+x6*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                x2=x5; y2=y5;
              }//float
            }//dragon

            if ((x3&1023)==374){//hydra
              if (z==2){//float
                x5=x2; y5=y2;
                myobj->type=425+0*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+4*1024; x2=x5+1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+8*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+12*1024; x2=x5+1; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+16*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+20*1024; x2=x5-1; y2=y5+1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+24*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+28*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                x2=x5; y2=y5;
              }//float
            }//hydra

            //mover_flash_skip:;

passskip:;

            //after pass 1 effects
            if (z==1){
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                if (vischeck[x2][y2]){
                  if (tplayer->mv_flags[i]&MV_PROTECT) sf32z(ps,x2*32,y2*32,sfx8,3*32+19+(keyframe>>1));
                }
              }}}}
            }//z==1
            //end: after pass 1 effects

            //after pass 2 extras
            if (z==2){


              if (tplayer->mv_type[i]==431){
                if (tplayer->mv_more[i]){

                  myobj->type=tplayer->mv_more[i]+(tplayer->mv_dir[i]*4+1)*1024;

                  if (tplayer->mv_more[i]==354){//mouse
                    myobj->type=tplayer->mv_more[i]+tplayer->mv_dir[i]*1024;
                    getspr(myobj);
                    x5=x2*32; y5=y2*32;
                    if (tplayer->mv_dir[i]==0){
                      y5+=21;
                      x5-=1;
                    }
                    if (tplayer->mv_dir[i]==1){
                      x5-=23;
                      y5-=3;
                    }
                    if (tplayer->mv_dir[i]==2){
                      y5-=25;
                      x5-=7;
                    }
                    if (tplayer->mv_dir[i]==3){
                      x5+=23;
                      y5-=11;
                    }
                    goto onhorse_specialoffsetused;
                  }//mouse

                  if (tplayer->mv_more[i]==363){//garg
                    myobj->type=tplayer->mv_more[i]+(tplayer->mv_dir[i]*3+1)*1024;
                  }



                  getspr(myobj);




                  if ((tplayer->mv_more[i]==410)||(tplayer->mv_more[i]==387)||(tplayer->mv_more[i]==386)||(tplayer->mv_more[i]==378)){

                    if ((tplayer->mv_more[i]==410)||(tplayer->mv_more[i]==386)){
                      if ((tplayer->mv_dir[i]==1)||(tplayer->mv_dir[i]==2)) goto usenormalmask;
                    }

                    //special masks
                    if ((tplayer->mv_dir[i]==0)||(tplayer->mv_dir[i]==2)){
                      for (x5=0;x5<1024;x5++){
                        if (horsemaskdress->o2[x5]) bt32->o2[x5]=0;
                      }
                      img0(bt32,0,0,horsemaskdressb);
                    }else{
                      for (x5=0;x5<1024;x5++){
                        if (horsemask2dress->o2[x5]) bt32->o2[x5]=0;
                      }
                      img0(bt32,0,0,horsemask2dressb);
                    }

                    goto specialmaskused;
                  }
usenormalmask:
                  //apply masks
                  if ((tplayer->mv_dir[i]==0)||(tplayer->mv_dir[i]==2)){
                    for (x5=0;x5<1024;x5++){
                      if (horsemask->o2[x5]) bt32->o2[x5]=0;
                    }


                  }else{
                    for (x5=0;x5<1024;x5++){
                      if (horsemask2->o2[x5]) bt32->o2[x5]=0;
                    }


                  }

specialmaskused:



                  x5=x2*32; y5=y2*32;
                  if (tplayer->mv_dir[i]==0){
                    y5+=19;
                    x5-=7;
                  }
                  if (tplayer->mv_dir[i]==1){
                    x5-=21;
                    y5-=7;
                  }
                  if (tplayer->mv_dir[i]==2){
                    y5-=21;
                    x5-=7;
                  }
                  if (tplayer->mv_dir[i]==3){
                    x5+=17;
                    y5-=7;
                  }


onhorse_specialoffsetused:

                  if (flash_skip==0){
                    img0(ps,x5,y5,bt32);
                  }else{
                    imgt0(ps,x5,y5,bt32);
                  }






                }}

            }//z==2

            keyframe=keyframe_backup;
          }}//mapx,mapy
        }//i
      }}//x,y
    }//z(pass)

    //objbuffer (floating)
    myobj=fakeobj;
    for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
      mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
      x2=x-1; y2=y-1;
      if (i=tplayer->sobj_bufsize[bufx][bufy]){
        tp2=tplayer->sobj[bufx][bufy];
        for (i2=0;i2<i;i2++){
          x3=tp2[i2];
          myobj->type=x3;
          //flashing?
          if (myobj->type&32768){
            if ((sprlnk[(myobj->type+1)&1023]-sprlnk[myobj->type&1023])>32) goto flash_disable;
            myobj->type-=32768;
            if (refreshcount&1) goto flash_skip;
          }
flash_disable:
          if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
            if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
              GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);
              getsound(myobj->type,x2,y2);
              getlight(myobj->type,x2,y2);
            }
          }}}}
          if ((x3&1023)!=x3){//might be buildable
            if (tclass_build[x3-1024]&1){//square
              x2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-1024;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2++; y2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-2048;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-3072;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2++; y2++;
            }
            if (tclass_build[x3-1024]&2){//horizontal
              x2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-1024;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2++;
            }
            if (tclass_build[x3-1024]&4){//vertical
              y2--;
              if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                myobj->type=x3-1024;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              y2++;
            }
          }//build
flash_skip:;
        }//i2
      }//i
    }}//objbuffer (floating) end





    //objfixed (floating)
    myobj=fakeobj;
    for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
      x2=x-1; y2=y-1;
      mapx=tpx+x-1; mapy=tpy+y-1; if ((mapx!=2048)&&(mapy!=1024)){

        if (i=objfixed_index[tpy+y-1][tpx+x-1]){
          i2=objfixed_type[i];
          for (i3=i+1;i3<=(i+i2);i3++){
            x3=objfixed_type[i3];
            myobj->type=x3;
            if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
              if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                GSs=1; GSx=x2*32; GSy=y2*32;
                getspr(myobj);
                getsound(myobj->type,x2,y2);
                getlight(myobj->type,x2,y2);
              }
            }}}}


            if ((x3&1023)!=x3){ //possibly buildable!



              if (tclass_build[x3-1024]&1){//square
                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-1024;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-2048;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-3072;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2++;
              }
              if (tclass_build[x3-1024]&2){//horizontal
                x2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-1024;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++;
              }
              if (tclass_build[x3-1024]&4){//vertical
                y2--;
                if (x2>=0){ if (x2<=31){ if (y2>=0){ if (y2<=23){
                  myobj->type=x3-1024;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                y2++;
              }
            }//build
          }//i3
        }//i

      }//mapx,mapy

    }}//x,y






    for (y=2;y<=25;y++){ for (x=2;x<=33;x++){ //overwrite objects on reverse side of wall
      if (vis[x][y]==8){
        mapx=tpx+x-2; mapy=tpy+y-2;
        i=bt[mapy][mapx]&1023;
        if (((i>=140)&&(i<=207))||((i>=240)&&(i<=251))){
          if (((vis[x][y+1]&12)==0)||((vis[x+1][y]&12)==0)||((vis[x+1][y+1]&12)==0)){
            g32(ps,(x-2)*32,(y-2)*32,bt8[0],i);
          }
        }//i
      }//8
    }}
    for (y=2;y<=25;y++){ for (x=2;x<=33;x++){//edit edge basetiles
      if (vis[x][y]==8){
        mapx=tpx+x-2; mapy=tpy+y-2;
        i=bt[mapy][mapx]&1023;
        i2=i;
        if (i==146){//corner going down to straight accross
          if ((vis[x][y+1]&12)==0) i=144;
          if ((vis[x+1][y]&12)==0) i=145;
        }
        if (i==162){
          if ((vis[x][y+1]&12)==0) i=160;
          if ((vis[x+1][y]&12)==0) i=161;
        }
        if (i==178){
          if ((vis[x][y+1]&12)==0) i=176;
          if ((vis[x+1][y]&12)==0) i=177;
        }
        if ((i>=140)&&(i<188)){//check corners
          i3=0;
          if (i>=160) i3=1;
          if (i>=176) i3=2;
          if ((vis[x][y-1]&12)==0){ if ((vis[x+1][y]&12)==0){
            if (i3==0) i=152;
            if (i3==1) i=152+16;
            if (i3==2) i=152+32;
          }}
          if ((vis[x][y+1]&12)==0){ if ((vis[x-1][y]&12)==0){
            if (i3==0) i=152+1;
            if (i3==1) i=152+16+1;
            if (i3==2) i=152+32+1;
          }}
          if ((vis[x][y+1]&12)==0){ if ((vis[x+1][y]&12)==0){
            if (i3==0) i=152-3;
            if (i3==1) i=152+16-3;
            if (i3==2) i=152+32-3;
          }}
        }//i
        if (i!=i2){
          g32(ps,(x-2)*32,(y-2)*32,bt8[0],i);
        }
      }//8
    }}
    for (y=2;y<=25;y++){ for (x=2;x<=33;x++){ //black corners
      if (vis[x][y]==8){
        mapx=tpx+x-2; mapy=tpy+y-2;
        i=bt[mapy][mapx]&1023;
        if ((i>=140)&&(i<188)){
          if ((vis[x+1][y]&12)==0){ if ((vis[x][y-1]&12)==0){
            sf32z(ps,(x-2)*32,(y-2)*32,sfx8,10);
          }}
          if ((vis[x-1][y]&12)==0){ if ((vis[x][y+1]&12)==0){
            sf32z(ps,(x-2)*32,(y-2)*32,sfx8,11);
          }}
        }//i
      }//8
    }}
    //set black squares
    myobj=fakeobj;
    myobj->type=331+22*1024;
    for (y=0;y<=23;y++){ for (x=0;x<=31;x++){
      if (vis[x+2][y+2]<4){
        GSs=1; GSx=x*32; GSy=y*32;
        getspr(myobj);
      }
    }}







    if (endgame){
      if (endgame_timer<=140.0f){
        myobj=fakeobj;

        i=0;
        if ((endgame_message>=2)&&(endgame_message<=6)) i=1;
        if (i){
          myobj->type=84+1024; GSx=(921-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=84; GSx=(920-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

        i=0;
        if (endgame_message>=3) i=1;
        if (i){
          myobj->type=409+4*1024; GSx=(921-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=378+9*1024; GSx=(920-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

        i=0;
        if ((endgame_message>=9)&&(endgame_message<=11)) i=1;
        if (i){
          myobj->type=84+1024; GSx=(926-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=84; GSx=(925-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

        i=0;
        if (endgame_message>=10) i=1;
        if (i){
          myobj->type=363+9*1024; GSx=(925-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=363+7*1024; GSx=(925+2-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+36*1024; GSx=(925-tpx)*32; GSy=(849-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+37*1024; GSx=(925+1-tpx)*32; GSy=(849-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+38*1024; GSx=(925-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+39*1024; GSx=(925+1-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

      }
    }


    if (wizardeyetimeleft){
      myobj=fakeobj;
      myobj->type=52;
      GSs=1; GSx=(tplayer->x-tpx)*32; GSy=(tplayer->y-tpy)*32; getspr(myobj);
    }

    if ((minimap_frame->offset_x>=1024)&&peer){
      minimap_frame->offset_x-=2048;
    }else if((minimap_frame->offset_x<=1024)&&!peer){
      minimap_frame->offset_x+=2048;
    }

    if ((tmap_frame->offset_x>=1024)&&tmap){
      tmap_frame->offset_x-=2048;
    }else if((tmap_frame->offset_x<=1024)&&!tmap){
      tmap_frame->offset_x+=2048;
    }

    for (i=0;i<=stormcloak_last2;i++){
      x=stormcloak_x2[i]; y=stormcloak_y2[i];
      z=ett*12; z%=14; if (z>7) z=14-z;
      ls2_p=(unsigned char*)&stormcloak[z][0]; z2=7;
      x7=(x-z2)*32; y7=(y-z2)*32; x8=(x+z2+1)*32; y8=(y+z2+1)*32; //screen rect
      x9=-x7; y9=-y7; //offset inside lsX array
      if (x7<0) x7=0; if (y7<0) y7=0; if (x8>1024) x8=1024; if (y8>768) y8=768; //crop rect
      ls_off=(y7<<10)+x7; ls_off_add=1024-(x8-x7); //starting offset/add
      ls2_off=(y7+y9)*((z2*2+1)*32)+x7+x9; ls2_off_add=((z2*2+1)*32)-(x8-x7); 
      for (y6=y7;y6<y8;y6++){ for (x6=x7;x6<x8;x6++){
        if (z=ls2_p[ls2_off]){
          z+=ls_off;
          if ((z>>10)!=(ls_off>>10)) z=(ls_off>>10<<10)+1023;
          ps->o2[ls_off]=ps->o2[z];
        }//z
        ls_off++; ls2_off++;
      } ls_off+=ls_off_add; ls2_off+=ls2_off_add; } //x6, y6
    }//i



    //tremor
    if (tremor){

      i2=tremor*16;
      for (i=0;i<i2;i++){

        //equal weighting
        x=rnd*1280-255; y=rnd*1024-255;
        cls(surf_tremor,0);
        img(surf_tremor,-x,-y,ps);

        for (z=0;z<65536;z++){
          if (surf_tremorcirclemask->o2[z]){
            surf_tremor->o2[z]=0;
          }else{
            if (surf_tremor->o2[z]==0) surf_tremor->o2[z]=1;
          }
        }

        if (tremor==1){
          x2=rnd*3-1; y2=rnd*3-1;
        }
        if (tremor==2){
          x2=rnd*5-2; y2=rnd*5-2;
        }
        if (tremor==3){
          x2=rnd*7-3; y2=rnd*7-3;
        }

        x+=x2; y+=y2;
        img0(ps,x-1,y-1,surf_tremor);

      }//i
    }



    tremor=0;



    if (cloudloaded==0){ cloudloaded=1;
    for (i=0;i<=3;i++){
      if (i==0) cloudimg[i][1]=loadimage(".\\dr\\cld4.bmp",SURF_SYSMEM16);
      if (i==1) cloudimg[i][1]=loadimage(".\\dr\\cld5.bmp",SURF_SYSMEM16);
      if (i==2) cloudimg[i][1]=loadimage(".\\dr\\cld6.bmp",SURF_SYSMEM16);
      if (i==3) cloudimg[i][1]=loadimage(".\\dr\\cld7.bmp",SURF_SYSMEM16);
      x=cloudimg[i][1]->d.dwWidth; y=cloudimg[i][1]->d.dwHeight;
      cloudimg[i][0]=newsurf(x,y,SURF_SYSMEM16);
      cloudimg[i][2]=newsurf(x,y,SURF_SYSMEM16);
      cloudimg[i][3]=newsurf(x,y,SURF_SYSMEM16);
      for (y2=0;y2<y;y2++){
        for (x2=0;x2<x;x2++){
          i2=cloudimg[i][1]->o2[y2*x+x2];
          if ((i2&31)<=8) i2=0;
          cloudimg[i][1]->o2[y2*x+x2]=i2;
          cloudimg[i][0]->o2[y2*x+x2]=0; if (i2) cloudimg[i][0]->o2[y2*x+x2]=1<<5;//shadow
          cloudimg[i][2]->o2[y2*x+x2]=i2; if ((i2&31)<15) cloudimg[i][2]->o2[y2*x+x2]=0;
          cloudimg[i][3]->o2[y2*x+x2]=i2; if ((i2&31)<24) cloudimg[i][3]->o2[y2*x+x2]=0;
        }}
    }//i
    }

    //remove out of range clouds
    i2=1024;//max distance before removal
    x=tpx*32+512; y=tpy*32+384;//screen centre
    for (i=0;i<=31;i++){ if (cloudactive[i]){
      x2=cloudx[i]+cloudimg[cloudtype[i]][0]->d.dwWidth/2; y2=cloudy[i]+cloudimg[cloudtype[i]][0]->d.dwHeight/2-cloudheight[i]/2;//approx. centre of cloud
      if ((abs(x2-x)>=i2)||(abs(y2-y)>=i2)){
        cloudactive[i]=FALSE;//remove cloud
      }//range>i2
    }}

    //count number of clouds
    i2=0;
    for (i=0;i<=31;i++){
      if (cloudactive[i]) i2++;
    }


    x2=(long)btime2%28;
    x2-=20;
    if (x2>=0){
      if (x2==0) cloudidealnum=2;
      if (x2==1) cloudidealnum=4;
      if (x2==2) cloudidealnum=6;
      if (x2==3) cloudidealnum=8;
      if (x2==4) cloudidealnum=6;
      if (x2==5) cloudidealnum=4;
      if (x2==6) cloudidealnum=2;
    }else{
      cloudidealnum=1;
    }

cloudadded:
    if (i2<cloudidealnum){

      for (i=0;i<=31;i++){
        if (cloudactive[i]==FALSE){ cloudactive[i]=TRUE;



        cloudtype[i]=rnd*4;

        cloudheight[i]=256+(rnd*256);

        i3=rnd*4;
        if (i3==0){
          cloudx[i]=long(x+rnd*(1024+cloudimg[cloudtype[i]][0]->d.dwWidth)-cloudimg[cloudtype[i]][0]->d.dwWidth-512);
          cloudy[i]=y-384-cloudimg[cloudtype[i]][0]->d.dwHeight;
        }
        if (i3==1){
          cloudx[i]=long(x+rnd*(1024+cloudimg[cloudtype[i]][0]->d.dwWidth)-cloudimg[cloudtype[i]][0]->d.dwWidth-512);
          cloudy[i]=y+384+cloudheight[i];
        }
        if (i3==2){
          cloudx[i]=x-512-cloudimg[cloudtype[i]][0]->d.dwWidth;
          cloudy[i]=long(y+rnd*(768+cloudimg[cloudtype[i]][0]->d.dwHeight+cloudheight[i])-384-cloudimg[cloudtype[i]][0]->d.dwHeight);
        }
        if (i3==3){
          cloudx[i]=x+512;
          cloudy[i]=long(y+rnd*(768+cloudimg[cloudtype[i]][0]->d.dwHeight+cloudheight[i])-384-cloudimg[cloudtype[i]][0]->d.dwHeight);
        }

        if (firstclouds){
          cloudx[i]=long(x+rnd*(1024+cloudimg[cloudtype[i]][0]->d.dwWidth)-cloudimg[cloudtype[i]][0]->d.dwWidth-512);
          cloudy[i]=long(y+rnd*(768+cloudimg[cloudtype[i]][0]->d.dwHeight+cloudheight[i])-384-cloudimg[cloudtype[i]][0]->d.dwHeight);
        }



















        i2++;
        goto cloudadded;
        }
      }
    }
    firstclouds=0;

    if (noclouds==FALSE){
      x=tplay->x; y=tplay->y;
      if ((x>0)&&(y>0)&&(x<1024)&&(y<1024)){

        if ((bt[y][x]&1023)<192){

          for (i=0;i<=31;i++){
            if (cloudactive[i]){
              if (timelval!=15){
                imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32,cloudimg[cloudtype[i]][0]);
              }
            }
          }


          for (i2=256;i2<=(256+256);i2++){
            for (i=0;i<=31;i++){
              if (cloudheight[i]==i2){
                if (cloudactive[i]){
                  imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32-cloudheight[i],cloudimg[cloudtype[i]][1]);
                  imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32-cloudheight[i],cloudimg[cloudtype[i]][2]);
                  if (timelval!=15){
                    imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32-cloudheight[i],cloudimg[cloudtype[i]][3]);
                  }
                }
              }
            }
          }

        }else{//outside
          firstclouds=1;
        }//inside

      }//x,y on surface check
    }//noclouds

    for (i=0;i<=31;i++){
      if (cloudactive[i]){


        x=(cloudheight[i]-256)/64;
        cloudx[i]+=((float)windx2*(1+x));
        cloudy[i]+=((float)windy2*(1+x));
      }
    }

    if (x5=timelval){
      i=(unsigned long)&ls;
      i2=(unsigned long)ps->o;
      i3=(unsigned long)lval;
      _asm{
        ;preserve registers
          push ebx
          push esi
          push edi
          ;init registers
          mov esi,i ;z
          mov edi,i2
          mov edx,i3
          mov ecx,x5
          push ebp
          mov ebp,786432
          ;main loop
asm_lightshow0:
        mov al,[esi]
        mov ebx,ecx
          cmp bl,al
          ja asm_lightshow1
          xor ebx,ebx
          jmp asm_lightshow2
asm_lightshow1:
        sub bl,al
          shl ebx,16
asm_lightshow2:
        mov bx,[edi] ;screen value
          shl ebx,1
          add ebx,edx
          mov ax,[ebx]
        mov [edi],ax
          inc esi
          add edi,2
          dec ebp
          jnz asm_lightshow0
          pop ebp
          pop edi
          pop esi
          pop ebx
      }
    }

    if (britlens){


      img0(ps,(920-tpx)*32,(851-tpy)*32,blr[keyframe>>1]);


    }

    if (garglens){
      img0(ps,(924-tpx)*32-2,(851-tpy)*32,glr[keyframe>>1]);
    }










    //display onscreen text
    if (osn!=-1){
      for (z=0;z<=osn;z++){

        if (osi[z]>=1024){
          z2=osi[z]-1024;

          if (z2==16){
            txtset(t,"typing");
            x=ett*2.0f; x&=3;
            if (x==1) txtadd(t,".");
            if (x==2) txtadd(t,"..");
            if (x==3) txtadd(t,"...");
            x=osx[z];
            y=osy[z]+16;
            txtfnt=fnt3;
            goto osdisplay_ktar;
          }

          if (z2==17){
            txtset(t,"speaking");
            x=ett*2.0f; x&=3;
            if (x==1) txtadd(t,".");
            if (x==2) txtadd(t,"..");
            if (x==3) txtadd(t,"...");
            x=osx[z];
            y=osy[z]+16;
            txtfnt=fnt3;
            goto osdisplay_ktar;
          }


          if ((userkey!=3)&&(userkey!=4)) goto osdisplay_ktar_skip;
          z2++;
          if (z2==10) z2=0;
          x=osx[z];
          y=osy[z]-8;
          if ((timelval-ls[x+(y+8+16)*1024])==15) goto osdisplay_ktar_skip;
          txtnumint(t,z2);
          txtfnt=fnt2;
          goto osdisplay_ktar;
        }

        z2=oshpmp[z];

        if (z2&128) img(statusbar,0,0,statusbar_g128); else img(statusbar,0,0,statusbar_r128);
        img(statusbar,0,5,statusbar_b128);
        if (z2&15){
          if (z2&128) img(statusbar,-32+2+(z2&15)*2,0,statusbar_g255); else img(statusbar,-32+2+(z2&15)*2,0,statusbar_r255);
        }
        if ((z2>>4)&7) img(statusbar,-32+4+((z2>>4)&7)*4,5,statusbar_b255);
        img(ps,osx[z]-16,osy[z]-9,statusbar);



        if (voicechat_recording){
          if (osvol[z]!=-1){
            if (x2=osvol[z]){
              x=osx[z]-45; y=osy[z]+32;
              img0(ps,x,y,vm_volumem);

              f=64.0f/255.0f*(float)x2;
              img(ps,x+17+f,y+1,vm_voltab2m);



            }else{
              x=osx[z]-8; y=osy[z]+32;
              img0(ps,x,y,vm_volmmute);
            }



          }
        }


        x=osx[z];
        y=osy[z]-8;
        txtset(t,idlst_name[osi[z]]);
        txtfnt=fnt3;
        txtcol=idlst_namecolour[osi[z]];
osdisplay_ktar:
        tagxy.cx=0; tagxy.cy=0;
        ps->s->GetDC(&taghdc);
        SelectObject(taghdc,txtfnt);
        GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
        ps->s->ReleaseDC(taghdc);
        x-=tagxy.cx/2;
        x2=txtcol;
        txtcol=rgb(0,0,0);
        txtout(ps,x+1,y+1-1,t);
        txtout(ps,x+1-1,y+1,t);
        txtout(ps,x+1+1,y+1,t);
        txtout(ps,x+1,y+1+1,t);
        txtcol=x2;
        txtout(ps,x+1,y+1,t);
        txtfnt=fnt1;
        txtcol=rgb(255,255,255);
osdisplay_ktar_skip:;
      }
    }

    //voice chat: volume meters of dead players (if any)
    if (voicechat_recording){
      for (z=1;z<=voicechat_listeningplayeri;z++){
        x=(voicechat_listeningplayerx[z]-tpx)*32+16; y=(voicechat_listeningplayery[z]-tpy)*32;
        if (x2=voicechat_listeningplayervolume[z]){
          x=x-45; y=y+32;
          img0(ps,x,y,vm_volumem);
          f=64.0f/255.0f*(float)x2;
          img(ps,x+17+f,y+1,vm_voltab2m);
        }else{
          x=x-8; y=y+32;
          img0(ps,x,y,vm_volmmute);
        }//x2
      }//z
    }//voicechat_recording

    //update local SF
    stolenitemwarningn=-1;//reset number of stolen item warnings onscreen
    for(i3=0;i3<=255;i3++){
      if (sfx[i3].type!=0){
        i4=sfx[i3].type;

        if (i4>=256){//time delayed effect!
          f=0.0625f; if (int((ett/f))!=int((ett/f)-(et/f))){//dec every 1/16th of a sec
            i5=(i4>>8)-1;
            sfx[i3].type=(i4&255)+(i5<<8);
          }//dec
          goto donesf2;
        }//time delayed effect!

        if (i4==1){ //physical attack
          img(bt32,-32,0,sfx8);

          if ((sfx[i3].wait<=0.04f)||(sfx[i3].wait>=0.85f)){
            imgt0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }else{
            img0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }



          //number above?
          if (sfx[i3].x2){
            txtfnt=fnt1;
            txtnumint(t,sfx[i3].x2);
            x=(sfx[i3].x-tpx)*32;
            if (t->l==1) x+=10;
            if (t->l==2) x+=5;
            y=(sfx[i3].y-tpy)*32-16+sfx[i3].wait*16.0f;
            txtcol=rgb(0,0,0); txtout(ps,x,y,t); txtout(ps,x+2,y+2,t); txtout(ps,x+2,y,t); txtout(ps,x,y+2,t);
            txtcol=rgb(255,0,0); txtout(ps,x+1,y+1,t);
          }


          if (sfx[i3].more==1){
            sfx[i3].more=0;
            soundplay2(u6osound[0],u6osound_volume[0]);
          }
          goto donesf;
        }//1

        if (i4==16){ //physical attack miss!
          img(bt32,-32*14,-32*3,sfx8);

          if ((sfx[i3].wait<=0.04f)||(sfx[i3].wait>=0.85f)){
            imgt0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }else{
            img0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }




          if (sfx[i3].more==1){
            sfx[i3].more=0;
            soundplay2(u6osound[0],u6osound_volume[0]);
          }
          goto donesf;
        }//1


        if (i4==5) goto pw_jmp;
        if (i4==6) goto pw_jmp;
        if (i4==7) goto pw_jmp;
        if (i4==8) goto pw_jmp;
        if (i4==9) goto pw_jmp;
        if (i4==10) goto pw_jmp;
        if (i4==11) goto pw_jmp;
        if (i4==12) goto pw_jmp;
        if (i4==13) goto pw_jmp;
        if (i4==14) goto pw_jmp; //blue ball
        if (i4==15) goto pw_jmp; //kill
        if (i4==17) goto pw_jmp; //cannon ball
        if (i4==18) goto pw_jmp; //green ball
        if (i4==19) goto pw_jmp; //blue bolt
        if (i4==20) goto pw_jmp; //pink ball
        if (i4==28) goto pw_jmp; //red bolt
        if (i4==29) goto pw_jmp; //red ball

        if (i4==2){ //arrow
pw_jmp:


          f3=sqrt((float)(abs(sfx[i3].x2-sfx[i3].x)*abs(sfx[i3].x2-sfx[i3].x)+abs(sfx[i3].y2-sfx[i3].y)*abs(sfx[i3].y2-sfx[i3].y)))/24.0f;
          if (sfx[i3].more==0xFFFF){ //setup



            sfx[i3].more=0; //index type *up arrow
            if (i4==9){
              if ((unsigned long)sfx[i3].wait&32768) {soundplay2(u6osound[18],u6osound_volume[18]); sfx[i3].wait-=32768;}
              sfx[i3].more=sfx[i3].wait; sfx[i3].wait=1;
            }



            f=abs(sfx[i3].x2-sfx[i3].x);
            f2=abs(sfx[i3].y2-sfx[i3].y);

            if (f||f2){
              if (f2>f){
                f4=2/f2;
                f=(sfx[i3].x2-sfx[i3].x); f2=(sfx[i3].y2-sfx[i3].y);
                if (f2<0) y=-2; if (f2>0) y=2;
                f=f*f4;
                x=0; if (f<-0.5) x=-1; if (f<-1.5) x=-2; if (f>0.5) x=1; if (f>1.5) x=2;
                if (i4!=9) sfx[i3].more=qkdir[x+2][y+2];
              }else{
                f4=2/f;
                f=(sfx[i3].x2-sfx[i3].x); f2=(sfx[i3].y2-sfx[i3].y);
                if (f<0) x=-2; if (f>0) x=2;
                f2=f2*f4;
                y=0; if (f2<-0.5) y=-1; if (f2<-1.5) y=-2; if (f2>0.5) y=1; if (f2>1.5) y=2;
                if (i4!=9) sfx[i3].more=qkdir[x+2][y+2];
              }
            }else{ //both f and f2 ==0
              if (i4!=9) sfx[i3].more=qkdir[2][0]; //up!
            }

            sfx[i3].wait=f3-(0.75f/24.0f);
            if (i4==7) {sfx[i3].wait=(f3*2)-(0.75f/24.0f); soundplay2(u6osound[10],u6osound_volume[10]);}//*boomerang
            if (sfx[i3].wait<0) sfx[i3].wait=0;
            if (i4==2) soundplay2(u6osound[7],u6osound_volume[7]);

            if (i4==6) soundplay2(u6osound[8],u6osound_volume[8]);
            if (i4==17) soundplay2(u6osound[14],u6osound_volume[14]);

            if (i4==8) soundplay2(u6osound[20],u6osound_volume[20]);

          }//first instance only!

          if (f3){
            f=(sfx[i3].x2-sfx[i3].x)*32;
            f=f/f3*(f3-sfx[i3].wait); //move per second
            f2=(sfx[i3].y2-sfx[i3].y)*32;
            f2=f2/f3*(f3-sfx[i3].wait); //move per second
            x=f;
            y=f2;
          }else{
            x=(sfx[i3].x2-sfx[i3].x)*32; y=(sfx[i3].y2-sfx[i3].y)*32;
          }


          if (i4==7){ //*boomerang
            if (sfx[i3].wait>f3){
              f=(sfx[i3].x2-sfx[i3].x)*32;
              f=f/f3*(f3-(sfx[i3].wait-f3)); //move per second
              f2=(sfx[i3].y2-sfx[i3].y)*32;
              f2=f2/f3*(f3-(sfx[i3].wait-f3)); //move per second
              x=f;
              y=f2;
            }else{
              f=(sfx[i3].x2-sfx[i3].x)*32;
              f=f/f3*(f3-(f3-sfx[i3].wait)); //move per second
              f2=(sfx[i3].y2-sfx[i3].y)*32;
              f2=f2/f3*(f3-(f3-sfx[i3].wait)); //move per second
              x=f;
              y=f2;
              if (sfx[i3].wait<=(0.75f/24.0f)) {sfx[i3].wait=0; goto donesf;} //finish and don't draw
            }
            img(bt32,keyframe2*-32,-96,u6ob);
          }//boomerang


          if (i4==2) img(bt32,sfx[i3].more*-32,-32,u6ob);
          if (i4==5) img(bt32,sfx[i3].more*-32,0,u6ob);
          if (i4==6) img(bt32,sfx[i3].more*-32,-64,u6ob); //bolt
          if (i4==8) img(bt32,sfx[i3].more*-32,-128,u6ob); //fire wand
          if (i4==9){
            static object *GENERICthrow_obj=NULL;
            if (GENERICthrow_obj==NULL) GENERICthrow_obj=OBJnew_local();
            GENERICthrow_obj->type=sfx[i3].more;
            getspr(GENERICthrow_obj);
          }
          if (i4==10) img(bt32,keyframe2*-32-256,-96,u6ob); //dagger
          if (i4==11) img(bt32,keyframe2*-32,-160,u6ob); //axe
          if (i4==12) img(bt32,sfx[i3].more*-32,-192,u6ob); //spear
          if (i4==13) img(bt32,-8*32,0,sfx8); //rock! sling

          if (i4==14) img(bt32,(keyframe2&3)*-32-256,-160,u6ob); //blue ball!
          if (i4==15) img(bt32,sfx[i3].more*-32,-224,u6ob); //kill
          if (i4==17) img(bt32,-9*32,0,sfx8); //cannon
          if (i4==18) img(bt32,(keyframe2&3)*-32-256-128,-160,u6ob); //green ball!
          if (i4==19) img(bt32,sfx[i3].more*-32,-256,u6ob); //blue bolt
          if (i4==20) img(bt32,(keyframe2&3)*-32,-288,u6ob); //pink ball!
          if (i4==28) img(bt32,sfx[i3].more*-32,-320,u6ob); //red bolt
          if (i4==29) img(bt32,(keyframe2&3)*-32-128,-288,u6ob); //red ball!

          if (i4==8){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==14){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==18){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==19){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==20){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==5){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==28){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==29){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}


          img0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;
        }//2,...



        if (i4==3){ //global portrait message
          txtset(t,(txt*)sfx[i3].p);
          txtset(t2,"?"); t2->d2[0]=92;
          z=txtsearch(t,t2);

          if (sfx[i3].wait==0){ //send message to message log and remove






            txtset(t3,t);

            txtset(t,"P");//standard portrait
            txtnumint(t2,sfx[i3].more); txtadd(t,t2);//portrait number
            txtset(t2,"/"); txtadd(t,t2);




            txtset(t4,t3);
            if (z){
              txtleft(t4,z-1);
            }

            if ((sfx[i3].x==2047)&&(sfx[i3].y==1023)){
              txtset(t5,"?"); t5->ds[0]=4; txtadd(t,t5);
            }

            if ((sfx[i3].x==2048)&&(sfx[i3].y==1023)){
              txtset(t5,"?"); t5->ds[0]=5; txtadd(t,t5);
            }






            //fetch colourname if available
            txtset(t6,"");
            if (t4->d2[0]==6){
              txtset(t6,"6CCCN...");
              t6->d2[0]=6;
              t6->d2[1]=t4->d2[1];
              t6->d2[2]=t4->d2[2];
              t6->d2[3]=t4->d2[3];
              t6->d2[4]=3;
            }
            static unsigned char wraptext_firstline;
            wraptext_firstline=1;
            //check if data exceeds 1 line inc port! 16 across
wraptext_recheck:
            ps->s->GetDC(&taghdc);
            SelectObject(taghdc,fnt1);//not valid for garg text or runes!
            tagxy.cx=0; tagxy.cy=0; GetTextExtentPoint32(taghdc,t4->d,t4->l,&tagxy);//get width of t4
            ps->s->ReleaseDC(taghdc);
            if (t6->l&&wraptext_firstline) tagxy.cx-=64;
            if ((64+tagxy.cx)>=1024){//original string is more than 1024 chars
              txtset(t7,"");//clear t7




              //move t4 string into t7 until it fits exactly
movet4intot7:
              txtaddchar(t7,t4->d2[0]); txtright(t4,t4->l-1);

              ps->s->GetDC(&taghdc);
              SelectObject(taghdc,fnt1);//not valid for garg text or runes!
              tagxy.cx=0; tagxy.cy=0; GetTextExtentPoint32(taghdc,t7->d,t7->l,&tagxy);
              ps->s->ReleaseDC(taghdc);
              if (t6->l&&wraptext_firstline) tagxy.cx-=64;
              if ((64+tagxy.cx)<1024) goto movet4intot7;


              if (wraptext_firstline){ wraptext_firstline=0;
              txtset(t8,t);  txtadd(t8,t7); txtfileout(t8,messagelog);

              }else{
                txtset(t8,t); txtadd(t8,t6); txtadd(t8,t7); txtfileout(t8,messagelog);
              }

              goto wraptext_recheck;


            }
            txtfnt=fnt1;
            if (wraptext_firstline==FALSE) txtadd(t,t6);

            txtadd(t,t4);


            txtfileout(t,messagelog);




            if (autoscroll==0.0f) textdisplayi=0xFFFFFFF; //position list at last index!


            if (z){
              txtright(t3,t3->l-z);
              txtset((txt*)sfx[i3].p,t3);

              txtset(t4,"?"); t4->d2[0]=92;
              z=txtsearch(t3,t4);
              if (z==0) z=t->l; else z--;
              f=(float)z/10.0f;
              if (f<1)f=1;
              if(f>4)f=4;
              sfx[i3].wait=f;
            }else{
              //last item
              free((txt*)sfx[i3].p); //free memory
            }





updatetextdisplay:

            //reconfigure mess_frm to display most recent items
            //1. seek pos 0 in message log
            //2. scan till the end moving each txt* index up 1 till finished
            x5=-1;

            for (x4=0;x4<=7;x4++){
              txtset(con_txt[x4],"");
            }

            seek(messagelog,0);
            x2=lof(messagelog);
seekmore:
            txtfilein(t,messagelog);
            x5++;
            x3=seek(messagelog);

            for (x4=6;x4>=0;x4--){
              txtset(con_txt[x4+1],con_txt[x4]);
            }
            txtset(con_txt[0],t);


            if ((x3<x2)&&(x5<textdisplayi)) goto seekmore;
            if (x3>=x2) {textdisplayi=x5; autoscroll=0.0f;}


            seek(messagelog,x2);

            cls(con_frm_img->graphic,0);
            for (x4=0;x4<=7;x4++){

              txtset(t,con_txt[x4]);
              if (t->l){

                txtset(t2,"/");
                if (t->d2[0]==80){//"P"
                  txtright(t,t->l-1); x3=txtsearch(t,t2);
                  z4=txtnum(t); txtright(t,t->l-x3);
                  img0(con_frm_img->graphic,0,256-32-32*x4,getportrait_halfsize(z4));
                }








                //rune font?
                txtfnt=fnt1;
                if (t->d2[0]==3){
                  txtright(t,t->l-1);
                  txtfnt=fnt5;
                  //edit special characters!
runeedit2:
                  for (x5=0;x5<t->l;x5++){
                    if ((t->d2[x5]==84)&&(t->d2[x5+1]==72)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C4"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//TH
                    if ((t->d2[x5]==69)&&(t->d2[x5+1]==69)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C1"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//EE
                    if ((t->d2[x5]==78)&&(t->d2[x5+1]==71)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C2"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//NG
                    if ((t->d2[x5]==69)&&(t->d2[x5+1]==65)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C0"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//EA
                    if ((t->d2[x5]==83)&&(t->d2[x5+1]==84)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C3"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//ST
                  }//x5
                }//3

                if (t->d2[0]==1){
                  txtright(t,t->l-1);
                  txtfnt=fnt6;
                  //edit special characters!
gargedit2:
                  for (x5=0;x5<t->l;x5++){
                    if ((t->d2[x5]==99)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x043"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//ch->C
                    if ((t->d2[x5]==103)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x047"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//gl->G
                    if ((t->d2[x5]==104)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x048"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//hl->H
                    if ((t->d2[x5]==107)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04B"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//kl->K
                    if ((t->d2[x5]==110)&&(t->d2[x5+1]==103)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04E"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//ng->N
                    if ((t->d2[x5]==110)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04C"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//nl->L
                    if ((t->d2[x5]==110)&&(t->d2[x5+1]==121)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x059"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//ny->Y
                    if ((t->d2[x5]==115)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x053"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//sh->S
                    if ((t->d2[x5]==122)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x05A"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//zh->Z
                  }//x5
                }//1


                static unsigned char txtlog_gm;
                static unsigned char txtlog_pm;
                static unsigned char txtlog_vm;
                txtlog_pm=0; txtlog_gm=0; txtlog_vm=0;

                if (t->d2[0]==4){ //global message
                  txtlog_gm=1; txtright(t,t->l-1);
                }
                if (t->d2[0]==5){ //private message
                  txtlog_pm=1; txtright(t,t->l-1);
                }
                if (t->d2[t->l-1]==255){//voice message
                  txtlog_vm=1; txtleft(t,t->l-1);
                }


                //text
                x=56/2;
                y=256-32-x4*32+2;
                //display (red)text and port
text_continue_con:


                txtset(t2,"@"); z=txtsearch(t,t2);
                txtsetchar(t2,6); z2=txtsearch(t,t2);
                if (z2){
                  if (z){
                    if (z2<z) z=z2;
                  }else{
                    z=z2;
                  }
                }
                if (z){ txtset(t3,t); txtleft(t,z-1);}


                for (z3=0;z3<t->l;z3++){
                  if (t->d2[z3]==7) t->d2[z3]=64;
                  if (t->d2[z3]==8) t->d2[z3]=92;
                }

                z3=rgb(255,255,255);
                if (txtlog_gm) z3=rgb(96,128,255);//global message
                if (txtlog_pm) z3=rgb(255,64,128);//private message
                if (txtlog_vm) z3=rgb(64,224,64);//voice message
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                SelectObject(taghdc,txtfnt);
                GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                ps->s->ReleaseDC(taghdc);


                lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1-1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1+1,t); txtout(con_frm_img->graphic,x+1-1,y+1+1,t);
                txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1,y+1-1,t); txtout(con_frm_img->graphic,x+1-1,y+1,t); txtout(con_frm_img->graphic,x+1+1,y+1,t); txtout(con_frm_img->graphic,x+1,y+1+1,t);
                txtfnt=lastfont;

                txtcol=z3;
                txtout(con_frm_img->graphic,x+1,y+1,t);
                x+=tagxy.cx;

                if (z){ //red

                  if (t3->d2[z-1]==64){//@ (red text)
                    txtright(t3,t3->l-z);
                    z=65536;
                    txtset(t2," "); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,","); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,"."); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,"?"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,")"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,"!"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    if (z==65536) z=0;
                    txtset(t,t3);
                    if (z) txtleft(t,z-1);


                    lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1-1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1+1,t); txtout(con_frm_img->graphic,x+1-1,y+1+1,t);
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1,y+1-1,t); txtout(con_frm_img->graphic,x+1-1,y+1,t); txtout(con_frm_img->graphic,x+1+1,y+1,t); txtout(con_frm_img->graphic,x+1,y+1+1,t);
                    txtfnt=lastfont;

                    txtcol=rgb(255,64,32); txtout(con_frm_img->graphic,x+1,y+1,t);

                    tagxy.cx=0; tagxy.cy=0;
                    ps->s->GetDC(&taghdc);
                    SelectObject(taghdc,txtfnt);
                    GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                    ps->s->ReleaseDC(taghdc);
                    x+=tagxy.cx;
                    if (z){ txtright(t3,t3->l-z+1); txtset(t,t3); goto text_continue_con; }
                    goto txtconlog_done;
                  }

                  if (t3->d2[z-1]==6){//coloured name
                    z3=t3->d2[z];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    z3<<=3; if (z3==248) z3=255;
                    z2=z3;

                    z3=t3->d2[z+1];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    z3<<=3; if (z3==248) z3=255;
                    z2+=(z3<<8);

                    z3=t3->d2[z+2];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    z3<<=3; if (z3==248) z3=255;
                    z2+=(z3<<16);


                    z3=t3->d2[z+3];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    txtright(t3,t3->l-z-4);//remove header and other info
                    z=z3;

                    txtset(t,t3);
                    txtleft(t,z);


                    lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1-1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1+1,t); txtout(con_frm_img->graphic,x+1-1,y+1+1,t);
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1,y+1-1,t); txtout(con_frm_img->graphic,x+1-1,y+1,t); txtout(con_frm_img->graphic,x+1+1,y+1,t); txtout(con_frm_img->graphic,x+1,y+1+1,t);
                    txtfnt=lastfont;


                    txtcol=z2; txtout(con_frm_img->graphic,x+1,y+1,t);

                    tagxy.cx=0; tagxy.cy=0;
                    ps->s->GetDC(&taghdc);
                    SelectObject(taghdc,txtfnt);
                    GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                    ps->s->ReleaseDC(taghdc);
                    x+=tagxy.cx;


                    if (t3->l>z){ txtright(t3,t3->l-z); txtset(t,t3); goto text_continue_con; }


                    goto txtconlog_done;
                  }






                }//z
txtconlog_done:;





              }
            }




            if (textdisplayupdate) goto updatetextdisplay2;


          }
          else{




            if (z){
              txtleft(t,z-1);
            }

            //rune font?
            txtfnt=fnt1;
            if (t->d2[0]==3){
              txtright(t,t->l-1);
              txtfnt=fnt5;
              //edit special characters!
runeedit:
              for (x5=0;x5<t->l;x5++){
                if ((t->d2[x5]==84)&&(t->d2[x5+1]==72)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C4"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//TH
                if ((t->d2[x5]==69)&&(t->d2[x5+1]==69)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C1"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//EE
                if ((t->d2[x5]==78)&&(t->d2[x5+1]==71)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C2"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//NG
                if ((t->d2[x5]==69)&&(t->d2[x5+1]==65)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C0"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//EA
                if ((t->d2[x5]==83)&&(t->d2[x5+1]==84)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C3"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//ST
              }//x5
            }//3

            if (t->d2[0]==1){
              txtright(t,t->l-1);
              txtfnt=fnt6;
              //edit special characters!
gargedit:
              for (x5=0;x5<t->l;x5++){
                if ((t->d2[x5]==99)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x043"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//ch->C
                if ((t->d2[x5]==103)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x047"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//gl->G
                if ((t->d2[x5]==104)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x048"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//hl->H
                if ((t->d2[x5]==107)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04B"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//kl->K
                if ((t->d2[x5]==110)&&(t->d2[x5+1]==103)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04E"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//ng->N
                if ((t->d2[x5]==110)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04C"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//nl->L
                if ((t->d2[x5]==110)&&(t->d2[x5+1]==121)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x059"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//ny->Y
                if ((t->d2[x5]==115)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x053"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//sh->S
                if ((t->d2[x5]==122)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x05A"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//zh->Z
              }//x5
            }//1


            //display (red)text and port
            //make t2, remove "@"
            z3=0;
            for (z2=0;z2<t->l;z2++){
              if (t->d2[z2]!=64) z3++;
            }
            txtNEWLEN(t2,z3);
            z3=0;
            for (z2=0;z2<t->l;z2++){
              if (t->d2[z2]!=64) t2->d2[z3++]=t->d2[z2];
            }
            //get text dimentions (using t2)
            tagxy.cx=0; tagxy.cy=0;
            ps->s->GetDC(&taghdc);
            SelectObject(taghdc,txtfnt);
            GetTextExtentPoint32(taghdc,t2->d,t2->l,&tagxy);
            ps->s->ReleaseDC(taghdc);
            x=(sfx[i3].x-tpx)*32-16;
            y=(sfx[i3].y-tpy)*32-8;
            if ((sfx[i3].x==2047)&&(sfx[i3].y==1023)){
              x=480-16;
              y=352-32*6-8;
            }

            if ((sfx[i3].x==2048)&&(sfx[i3].y==1023)){
              x=480-16;
              y=352-32*4-8;
            }



            if ((x+56+tagxy.cx)>1024) x=1024-56-tagxy.cx; if (x<0) x=0;
            if (y<0) y=0; if (y>704) y=704;

            img0(ps,x,y,getportrait(sfx[i3].more));


            static unsigned char txt_gm;
            static unsigned char txt_pm;
            static unsigned char txt_vm;
            txt_pm=0; txt_gm=0; txt_vm=0;

            if ((sfx[i3].x==2047)&&(sfx[i3].y==1023)) txt_gm=1;//global message
            if ((sfx[i3].x==2048)&&(sfx[i3].y==1023)) txt_pm=1;//private message
            if (t->d2[t->l-1]==255){//voice message
              txt_vm=1; txtleft(t,t->l-1);
            }



            x=x+56; y=y+21;
text_continue:


            txtset(t2,"@"); z=txtsearch(t,t2);
            txtsetchar(t2,6); z2=txtsearch(t,t2);
            if (z2){
              if (z){
                if (z2<z) z=z2;
              }else{
                z=z2;
              }
            }
            if (z){ txtset(t3,t); txtleft(t,z-1);}

            for (z3=0;z3<t->l;z3++){
              if (t->d2[z3]==7) t->d2[z3]=64;
              if (t->d2[z3]==8) t->d2[z3]=92;
            }



            z3=rgb(255,255,255);
            if (txt_gm) z3=rgb(96,128,255);//global message
            if (txt_pm) z3=rgb(255,64,128);//private message
            if (txt_vm) z3=rgb(64,224,64);//voice message
            tagxy.cx=0; tagxy.cy=0;
            ps->s->GetDC(&taghdc);
            SelectObject(taghdc,txtfnt);
            GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
            ps->s->ReleaseDC(taghdc);




            lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
            txtcol=rgb(0,0,0); txtout(ps,x+1-1,y+1-1,t); txtout(ps,x+1+1,y+1-1,t); txtout(ps,x+1+1,y+1+1,t); txtout(ps,x+1-1,y+1+1,t);
            txtcol=rgb(0,0,0); txtout(ps,x+1,y+1-1,t); txtout(ps,x+1-1,y+1,t); txtout(ps,x+1+1,y+1,t); txtout(ps,x+1,y+1+1,t);
            txtfnt=lastfont;

            txtcol=z3;
            txtout(ps,x+1,y+1,t);
            x+=tagxy.cx;


            if (z){//special text

              if (t3->d2[z-1]==64){//@ (red text)
                txtright(t3,t3->l-z);
                z=65536;
                txtset(t2," "); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,","); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,"."); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,"?"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,")"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,"!"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                if (z==65536) z=0;
                txtset(t,t3);
                if (z) txtleft(t,z-1);

                lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                txtcol=rgb(0,0,0); txtout(ps,x+1-1,y+1-1,t); txtout(ps,x+1+1,y+1-1,t); txtout(ps,x+1+1,y+1+1,t); txtout(ps,x+1-1,y+1+1,t);
                txtcol=rgb(0,0,0); txtout(ps,x+1,y+1-1,t); txtout(ps,x+1-1,y+1,t); txtout(ps,x+1+1,y+1,t); txtout(ps,x+1,y+1+1,t);
                txtfnt=lastfont;


                txtcol=rgb(255,64,32); txtout(ps,x+1,y+1,t);
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                SelectObject(taghdc,txtfnt);
                GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                ps->s->ReleaseDC(taghdc);
                x+=tagxy.cx;
                if (z){ txtright(t3,t3->l-z+1); txtset(t,t3); goto text_continue; }
                goto txtsf_done;
              }

              if (t3->d2[z-1]==6){//coloured name

                z3=t3->d2[z];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                z3<<=3; if (z3==248) z3=255;
                z2=z3;

                z3=t3->d2[z+1];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                z3<<=3; if (z3==248) z3=255;
                z2+=(z3<<8);

                z3=t3->d2[z+2];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                z3<<=3; if (z3==248) z3=255;
                z2+=(z3<<16);


                z3=t3->d2[z+3];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                txtright(t3,t3->l-z-4);//remove header and other info
                z=z3;

                txtset(t,t3);
                txtleft(t,z);



                lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                txtcol=rgb(0,0,0); txtout(ps,x+1-1,y+1-1,t); txtout(ps,x+1+1,y+1-1,t); txtout(ps,x+1+1,y+1+1,t); txtout(ps,x+1-1,y+1+1,t);
                txtcol=rgb(0,0,0); txtout(ps,x+1,y+1-1,t); txtout(ps,x+1-1,y+1,t); txtout(ps,x+1+1,y+1,t); txtout(ps,x+1,y+1+1,t);
                txtfnt=lastfont;


                txtcol=z2; txtout(ps,x+1,y+1,t);
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                SelectObject(taghdc,txtfnt);
                GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                ps->s->ReleaseDC(taghdc);
                x+=tagxy.cx;


                if (t3->l>z){ txtright(t3,t3->l-z); txtset(t,t3); goto text_continue; }


                goto txtsf_done;
              }


            }//z
          }//?

txtsf_done:
          txtfnt=fnt1;

          goto donesf;
        }



        if (i4==4){ //sound effect

          //unsigned short more; //distance bolt travels, ...


          if (sfx[i3].more==1) soundplay2(u6osound[1],u6osound_volume[1]);
          if (sfx[i3].more==2) soundplay2(u6osound[2],u6osound_volume[2]);
          if (sfx[i3].more==3) soundplay2(u6osound[4],u6osound_volume[4]);
          if (sfx[i3].more==4) soundplay2(u6osound[6],u6osound_volume[6]);
          if (sfx[i3].more==5) soundplay2(u6osound[19],u6osound_volume[19]);
          if (sfx[i3].more==6) soundplay2(u6osound[31],u6osound_volume[31]);
          if (sfx[i3].more==7) soundplay2(u6osound[32],u6osound_volume[32]);

          sfx[i3].type=0;




          goto donesf;
        }//4

        if (i4==21){//stolen item warning
          stolenitemwarningn++;
          stolenitemwarningx[stolenitemwarningn]=sfx[i3].x;
          stolenitemwarningy[stolenitemwarningn]=sfx[i3].y;
          stolenitemwarningtype[stolenitemwarningn]=sfx[i3].more;
          goto donesf;
        }//21

        if (i4==22){//multiple fireballs!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=8+x3;//fireball
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multiplefireballs_added;
            }}//type==0,i5
multiplefireballs_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//22

        if (i4==23){//multiple green balls!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=18+x3;//green ball
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multiplegreenballs_added;
            }}//type==0,i5
multiplegreenballs_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//23

        if (i4==24){//multiple lightning!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=5+x3;//lightning
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multiplelightning_added;
            }}//type==0,i5
multiplelightning_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//24

        if (i4==25){//multiple death bolts!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=15+x3;//lightning
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multipledeathbolts_added;
            }}//type==0,i5
multipledeathbolts_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//25

        if (i4==26){//tremor
          if (sfx[i3].wait==1.0f) soundplay2(u6osound[35],u6osound_volume[35]);
          x2=1;
          if (sfx[i3].wait>=0.4f) x2=2;
          if (sfx[i3].wait>=0.8f) x2=3;
          if (tremor<x2) tremor=x2;
          goto donesf;
        }//26

        if (i4==27){//inventory add
          x2=sfx[i3].wait*32;
          img0(ps,(sfx[i3].x-tpx)*32+8,(sfx[i3].y-tpy)*32+8+x2,inventoryadd_icon);
          static object inventoryadd_tempobj;
          ZeroMemory(&inventoryadd_tempobj,sizeof(object));
          inventoryadd_tempobj.type=sfx[i3].x2;
          getspr(&inventoryadd_tempobj);
          img(bt16,bt32);
          img0(ps,(sfx[i3].x-tpx)*32+8,(sfx[i3].y-tpy)*32+8+x2,bt16);
          goto donesf;
        }//27

donesf:
        if (sfx[i3].wait<=0) sfx[i3].type=0; //remove sfx
        sfx[i3].wait-=et;
        if (sfx[i3].wait<=0) sfx[i3].wait=0;
donesf2:;

        //check for more sf
      }//=!0
    }//i3






    if (STATUSMESSwait){
      txtset(t,STATUSMESSdisplaying);
      x=0;
      y=768-32;
      txtfnt=fnt1naa;
      txtcol=rgb(0,0,0);
      txtout(ps,x,y,t);
      txtout(ps,x+2,y+2,t);
      txtout(ps,x+2,y,t);
      txtout(ps,x,y+2,t);
      txtout(ps,x+1,y,t);
      txtout(ps,x+2,y+1,t);
      txtout(ps,x,y+1,t);
      txtout(ps,x+1,y+2,t);
      txtcol=rgb(255,255,255);
      txtfnt=fnt1;
      txtout(ps,x+1,y+1,t);
    }



    if (U6O_WALKTHRU){
walkthru_pos_skip0:

      i=0;
      if (!walkthru_x) i=1;
      x=abs(tpx-walkthru_x); y=abs(tpy-walkthru_y);
      if ((x<=3)&&(y<=3)) i=1;
      if (walkthru_pos_skip) i=1;

      if (i){ //i==1 to load a new location


        i2=seek(walkthru_fh);
        if (seek(walkthru_fh)<lof(walkthru_fh)){
          txtfilein(t,walkthru_fh); if (walkthru_pos_skip) walkthru_pos_skip--; 
          walkthru_pos++;
          if ((t->d2[0]>=48)&&(t->d2[0]<=57)){
            walkthru_x=txtnum(t);
            txtfilein(t,walkthru_fh); if (walkthru_pos_skip) walkthru_pos_skip--;
            walkthru_pos++;
            walkthru_y=txtnum(t);
            goto walkthru_newxy;

          }else{






            if (!walkthru_pos_skip){
              if (x||y){
                seek(walkthru_fh,i2); walkthru_pos--;
              }else{
                //display text now
                for (i3=0;i3<=255;i3++){ if (sfx[i3].type==0){
                  sfx[i3].type=3;
                  sfx[i3].x=walkthru_x+15;
                  sfx[i3].y=walkthru_y+11+2;
                  sfx[i3].x2=t->l;
                  static txt *dbgt5;
                  dbgt5=txtnew();
                  txtset(dbgt5,t);
                  sfx[i3].p=dbgt5;
                  z=t->l;
                  f=(float)z/10.0f;
                  if (f<1)f=1;
                  if(f>4)f=4;
                  sfx[i3].wait=f;
                  sfx[i3].more=187;
shiftdown2:
                  for (i2=0;i2<=255;i2++){ if ((sfx[i2].type==3)&&(i2!=i3)){
                    if ((sfx[i3].y==sfx[i2].y)||(sfx[i3].y==(sfx[i2].y+1))||(sfx[i3].y==(sfx[i2].y-1))) {sfx[i3].y=sfx[i2].y+2; goto shiftdown2;}
                  }}

                  //save walkthru pos
                  txtset(t,".\\dr\\walkthru.pos");
                  tfh=open2(t,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
                  put(tfh,&walkthru_pos,4); //lines to skip of the walkthru
                  close(tfh);

                  goto walkthru_txtdone;
                }} //i3, type==0
              }
            }//!walkthru_pos_skip

          }
        }else{//seek(walkthru_fh)<lof(walkthru_fh)
          U6O_WALKTHRU=FALSE;
        }
      }//i
walkthru_txtdone:
      if ((unsigned long)(ett*4.0f)&1){
walkthru_newxy:
        if (!walkthru_pos_skip){
          img(bt32,-32*15,-32*3,sfx8);
          img0(ps,(walkthru_x-tpx+15)*32,(walkthru_y-tpy+11)*32,bt32);
        }//!walkthru_pos_skip
      }
    }//U6O_WALKTHRU
    if (walkthru_pos_skip) goto walkthru_pos_skip0;






  } //(mess_UPDATEps->l&&clientframe)
skiprefresh2:



  if (nodisplay) goto skiprefresh;
  if (!clientframe) goto skiprefresh;


  if (minimap_frame->offset_x==4096){
    minimap_frame->offset_x=0; minimap_frame->offset_y=0; //default
    if (cltset2_restored){ if (cltset2.minimap_offset_x!=32767){
      minimap_frame->offset_x=cltset2.minimap_offset_x; minimap_frame->offset_y=cltset2.minimap_offset_y;
      cltset2.minimap_offset_x=32767;
    }}
  }

  if (tmap_frame->offset_x==4096){
    tmap_frame->offset_x=0; tmap_frame->offset_y=0; //default
    if (cltset2_restored){ if (cltset2.tmap_offset_x!=32767){
      tmap_frame->offset_x=cltset2.tmap_offset_x; tmap_frame->offset_y=cltset2.tmap_offset_y;
      cltset2.tmap_offset_x=32767;
    }}
  }

  for (i=0;i<=7;i++){


    if (CLIENTplayer->party[i]!=NULL){

      pmf=party_frame[i]; //shortcut

      if (pmf->offset_x==4096){
        pmf->offset_x=i*32; pmf->offset_y=128-16+i*32; //default
        if (cltset2_restored){ if (cltset2.party_frame_offset_x[i]!=32767){
          pmf->offset_x=cltset2.party_frame_offset_x[i]; pmf->offset_y=cltset2.party_frame_offset_y[i];
          cltset2.party_frame_offset_x[i]=32767;
        }}
      }
      if (pmf->offset_x>=1024){
        pmf->offset_x-=2048;
      }


      tnpc=(npc*)CLIENTplayer->party[i]->more; //shortcut
      if (tnpc->upflags){ //upflags





        img(party_frame[i]->graphic,status8); //clear frame




        img0(party_frame[i]->graphic,0,0,getportrait_doublesize(tnpc->port));


        txtset(t,tnpc->name);
        x=56-4*t->l; y=128-16-8;
        txtcol=rgb(0,0,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,255,255);
        if (i==0) txtcol=namecolour;
        txtout(party_frame[i]->graphic,x+1,y+1,t);
        txtcol=rgb(255,255,255);

        txtset(t,"Health");
        x=128-16; y=12-2;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        x2=tnpc->hp&32767;
        txtnumint(t,x2);
        x=128-16+64-8+4-2; y=12-2;
        if (x2<100) x=x+10;
        if (x2<10) x=x+10;


        txtcol=rgb(168,30,24);

        if (tnpc->hp&32768) txtcol=rgb(24,168,24);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,64,32);
        if (tnpc->hp&32768) txtcol=rgb(64,255,64);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"/");
        txtnumint(t2,tnpc->hp_max);
        txtadd(t,t2);
        x=128-16+64-8+4+32-3; y=12-2;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"Magic");
        x=128-16; y=12-2+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);



        txtnumint(t,tnpc->mp);
        x=128-16+64-8+4-2; y=12-2+18;
        if (tnpc->mp<100) x=x+10;
        if (tnpc->mp<10) x=x+10;

        txtcol=rgb(72,72,168);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(96,128,255);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"/");
        txtnumint(t2,tnpc->mp_max);
        txtadd(t,t2);
        x=128-16+64-8+4+32-3; y=12-2+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"STR  ");
        txtnumint(t2,tnpc->s);
        txtadd(t,t2);
        x=128-16; y=12+16+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"DEX  ");
        txtnumint(t2,tnpc->d);
        txtadd(t,t2);
        x=128-16+64+8-4; y=12+16+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"INT  ");
        txtnumint(t2,tnpc->i);
        txtadd(t,t2);
        x=128-16; y=12+16+18+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"LEV  ");
        txtnumint(t2,tnpc->lev);
        txtadd(t,t2);
        x=128-16+64+8-4; y=12+16+18+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);


        txtset(t,"EXP ");
        txtnumint(t2,tnpc->exp);
        txtadd(t,t2);


        x=800;
        if (tnpc->lev>=2) x=1600;
        if (tnpc->lev>=3) x=3200;
        if (tnpc->lev>=4) x=6400;
        if (tnpc->lev>=5) x=12800;
        if (tnpc->lev>=6) x=25600;
        if (tnpc->lev>=7) x=51200;
        if (tnpc->lev>=8) goto skiplevnext;
        txtset(t2,"/");
        txtadd(t,t2);
        txtnumint(t2,x);
        txtadd(t,t2);
skiplevnext:


        x=128-16; y=12+16+18+18+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);



        txtset(t,"E:");
        txtnumint(t2,tnpc->wt2/100); txtadd(t,t2);
        txtset(t2,"/"); txtadd(t,t2);
        txtnumint(t2,tnpc->wt2_max/100); txtadd(t,t2);

        x=128-16; y=12+16+18+18+18+18;
        txtcol=rgb(192,126,0);

        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(252,244,192);
        txtout(party_frame[i]->graphic,x+1,y+1,t);



        txtset(t,"I:");
        txtnumint(t2,tnpc->wt/100); txtadd(t,t2);
        txtset(t2,"/"); txtadd(t,t2);
        txtnumint(t2,tnpc->wt_max/100); txtadd(t,t2);

        x=128-16+64+8-4; y=12+16+18+18+18+18;
        txtcol=rgb(192,126,0);

        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(252,244,192);
        txtout(party_frame[i]->graphic,x+1,y+1,t);



        spellbook_flags[i]=0;
        if (spellbook_page[i]){
          img(party_spellbook_frame[i]->graphic,0,0,spellbook);

          img0(party_spellbook_frame[i]->graphic,212-28,0,getportrait_halfsize(tnpc->port));


          i3=spellbook_page[i]-1;
          //port

          //name
          txtset(t,tnpc->name);
          x=132; y=4;
          txtcol=rgb(8,8,8);
          txtout(party_spellbook_frame[i]->graphic,x,y,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
          txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
          txtcol=rgb(255,255,255);
          if (i==0) txtcol=namecolour;
          txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);
          txtcol=rgb(255,255,255);

          //spellcircle
          x=(32-spellcircle[i3]->d.dwHeight)>>1;
          img0(party_spellbook_frame[i]->graphic,115-spellcircle[i3]->d.dwWidth,x,spellcircle[i3]);

          txtset(t,"LEV ?");
          t->d2[4]=49+i3;
          x=33+4; y=4;
          txtcol=rgb(8,8,8);
          txtout(party_spellbook_frame[i]->graphic,x,y,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
          txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
          txtcol=rgb(255,255,255);
          txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);


          //do we need dogears??

          if (i3){
            i4=0;
            for (i5=0;i5<(i3<<4);i5++){
              if (spell[i][i5]) i4=1;
            }
            if (i4){
              img(party_spellbook_frame[i]->graphic,20,2,dogearl);
              spellbook_flags[i]|=1;
            }
          }

          if (i3!=7){
            i4=0;
            for (i5=((i3+1)<<4);i5<=255;i5++){
              if (spell[i][i5]) i4=1;
            }
            if (i4){
              img(party_spellbook_frame[i]->graphic,212,2,dogearr);
              spellbook_flags[i]|=2;
            }
          }





          i4=0; i5=0; //i5=side! //i4=vertical line
          for (i2=0;i2<=15;i2++){
            if (spell[i][(i3<<4)+i2]){ //valid
              if (i5) img(party_spellbook_frame[i]->graphic,130,28+i4*24+2+1,spellbookline); else img(party_spellbook_frame[i]->graphic,20-2,28+i4*24+2+1,spellbookline);
              i4++; if (i4==5) {i4=0; i5++;}
            }
          }

          i4=0; i5=0; //i5=side! //i4=vertical line
          for (i2=0;i2<=15;i2++){
            if (spell[i][(i3<<4)+i2]){ //valid

              if (spellicon[(i3<<4)+i2]){
                if (i5==0) img0(party_spellbook_frame[i]->graphic,112-20-spellicon[(i3<<4)+i2]->d.dwWidth,28+i4*24+24-spellicon[(i3<<4)+i2]->d.dwHeight+5,spellicon[(i3<<4)+i2]);
                if (i5) img0(party_spellbook_frame[i]->graphic,225-20-spellicon[(i3<<4)+i2]->d.dwWidth,28+i4*24+24-spellicon[(i3<<4)+i2]->d.dwHeight+5,spellicon[(i3<<4)+i2]);
              }//icon

              txtfnt=fnt4;
              txtset(t,spellname[(i3<<4)+i2]);
              x=18; y=28+i4*24-2;
              if (i5) x=130;
              txtcol=rgb(8,8,8);
              txtout(party_spellbook_frame[i]->graphic,x,y,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
              txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
              txtcol=rgb(255,255,255);
              txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);

              txtfnt=fnt1;
              txtnumint(t,spell[i][(i3<<4)+i2]-1);
              x=112-8*t->l; y=28+i4*24-2;
              if (i5) x=225-8*t->l;

              txtcol=rgb(72,72,168);
              txtout(party_spellbook_frame[i]->graphic,x,y,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
              txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
              txtcol=rgb(96,128,255);
              txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);

              txtfnt=fnt3;
              for (x2=0;x2<=7;x2++){
                if (spellrecall_partymember[x2]==(i+1)){
                  if (spellrecall_i[x2]==((i3<<4)+i2)){

                    txtset(t,"F"); txtnumint(t2,x2+5); txtadd(t,t2);

                    x=18; y=28+i4*24-2;



                    x=18+48; y=28+i4*24-2+8;


                    if (i5) x=130;

                    txtcol=rgb(8,8,8);
                    txtout(party_spellbook_frame[i]->graphic,x,y-1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y-1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y-1,t);
                    txtout(party_spellbook_frame[i]->graphic,x,y+3,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y+3,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y+3,t);
                    txtout(party_spellbook_frame[i]->graphic,x-1,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x-1,y+1,t);
                    txtout(party_spellbook_frame[i]->graphic,x-1,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x+3,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+3,y+1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+3,y+2,t);
                    txtcol=rgb(0,96+32,96+32);
                    txtout(party_spellbook_frame[i]->graphic,x,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y+1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x,y+1,t);
                    txtcol=rgb(0,192,192);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);


                  }
                }
              }
              txtfnt=fnt1;


              i4++; if (i4==5) {i4=0; i5++;}
            }
          }//i2

          txtfnt=fnt1;





        }//display spellbook!


        //clear spellbook buttons

        qkstf_update=TRUE;
        tnpc->upflags=0; //clear
      }

      //player inventory
      myobj=(object*)tnpc->baseitem;
      myobj=(object*)myobj->more;
      x=120; y=120; x3=tnpc->baseitem_offset;
      y3=0;
dni:
      if (myobj!=NULL){
        if (x3>0) {x3--;goto skipbio1;}
        getspr(myobj);

        if (not4sale_flags[i]&(1<<y3)) img0(party_frame[i]->graphic,x+14,y+8,not4salemask);

        img0(party_frame[i]->graphic,x,y,bt32);


        if (not4sale_flags[i]&(1<<y3)) imgt0(party_frame[i]->graphic,x+14,y+8,not4sale);
        y3++;

        //display quantity
        if ((myobj->type&1023)==64){ //key (1=A, etc.)
          x2=myobj->more2;
          if (x2<=26){txtset(t,"?"); t->d2[0]=x2+64; goto keyjmp;}
          txtset(t,"??"); t->d2[1]=(x2%26)+64; t->d2[0]=(x2/26)+64; goto keyjmp;
        }
        if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){
          txtnumint(t,myobj->more2);
keyjmp:
          txtfnt=fnt2;
          txtcol=rgb(8,8,8);


          x2=x+2;
          if (t->l==1) x2=x+12;
          if (t->l==2) x2=x+8;
          if (t->l==3) x2=x+4;

          txtout(party_frame[i]->graphic,x2,y+32-12,t);
          txtfnt=fnt1;
        }
        //display item +
        if ((obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].v1 || 
          obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].v2 || (myobj->type&1023)==448) && myobj->more2) {
            x2=x+16;
            y2=y+2;
            txtset(t,"+");
            txtnumint(t2,myobj->more2);
            txtadd(t,t2);

            txtfnt=fnt3; txtcol=0;
            txtout(party_frame[i]->graphic,x2,y2-1,t);
            txtout(party_frame[i]->graphic,x2-1,y2,t);
            txtout(party_frame[i]->graphic,x2+1,y2,t);
            txtout(party_frame[i]->graphic,x2,y2+1,t);
            txtout(party_frame[i]->graphic,x2-1,y2-1,t);
            txtout(party_frame[i]->graphic,x2+1,y2-1,t);
            txtout(party_frame[i]->graphic,x2+1,y2+1,t);
            txtout(party_frame[i]->graphic,x2-1,y2+1,t);
            txtcol=rgb(32,255,32);
            txtout(party_frame[i]->graphic,x2,y2,t);
            txtfnt=fnt1;
        }
        x=x+32; if (x==248) {x=120;y+=32;}
        if (y==248){
          if (myobj->next!=NULL) img0(party_frame[i]->graphic,112-1,128-8-2+32+16+60+2,darrow);
          goto diskip;
        }
        if (myobj->next!=NULL){
skipbio1:
          myobj=(object*)myobj->next;
          goto dni;
        }
      }
diskip:

      //display up arrow as required in inventory
      if (tnpc->baseitem_offset>0) img0(party_frame[i]->graphic,112-1,128-8-2+32+16+60+2-32+2-1,uarrow);


      //player readied items
      if (tnpc->helm!=NULL){
        myobj=tnpc->helm;
        getspr(myobj);
        img0(party_frame[i]->graphic,helmx,helmy,bt32);
      }
      if (tnpc->wep_right!=NULL){
        myobj=tnpc->wep_right;
        getspr(myobj);
        img0(party_frame[i]->graphic,wep_rightx,wep_righty,bt32);
        if (obji[sprlnk[tnpc->wep_right->type&1023]+(tnpc->wep_right->type>>10)].flags&16){
          img0(party_frame[i]->graphic,wep_leftx+8,wep_lefty+8,dhno);
        }
      }
      if (tnpc->wep_left!=NULL){
        myobj=tnpc->wep_left;
        getspr(myobj);
        img0(party_frame[i]->graphic,wep_leftx,wep_lefty,bt32);
        if (obji[sprlnk[tnpc->wep_left->type&1023]+(tnpc->wep_left->type>>10)].flags&16){
          img0(party_frame[i]->graphic,wep_rightx+8,wep_righty+8,dhno);
        }
      }
      if (tnpc->armour!=NULL){
        myobj=tnpc->armour;
        getspr(myobj);
        img0(party_frame[i]->graphic,armourx,armoury,bt32);
      }
      if (tnpc->boots!=NULL){
        myobj=tnpc->boots;
        getspr(myobj);
        img0(party_frame[i]->graphic,bootsx,bootsy,bt32);
      }
      if (tnpc->neck!=NULL){
        myobj=tnpc->neck;
        getspr(myobj);
        img0(party_frame[i]->graphic,neckx,necky,bt32);
      }
      if (tnpc->ring_right!=NULL){
        myobj=tnpc->ring_right;
        getspr(myobj);
        img0(party_frame[i]->graphic,ring_rightx,ring_righty,bt32);
      }
      if (tnpc->ring_left!=NULL){
        myobj=tnpc->ring_left;
        getspr(myobj);
        img0(party_frame[i]->graphic,ring_leftx,ring_lefty,bt32);
      }


      if (tnpc->baseitem!=tnpc->items){
        getspr(tnpc->baseitem);
        img(bt16,bt32);
        img0(party_frame[i]->graphic,90,123+8,bt32);
      }





    } //party member active?
  } //0-7 for/next





  if (qkstf_update){
    qkstf_update=FALSE;
    //refresh qkstf
    x=0;
    for (i=0;i<=7;i++){
      if (CLIENTplayer->party[i]!=NULL){
        x++; //count party members
      }}
    if (qkstf->graphic->d.dwHeight!=(x*64+32)){
      free(qkstf->graphic);
      qkstf->graphic=newsurf(128,x*64+32,SURF_SYSMEM16);  //adjust frame size
    }

    for (i=0;i<=x;i++){
      img(qkstf->graphic,0,i*64+32,mini_2);
    }
    img(qkstf->graphic,0,x*64,mini_3);

    i=-1;
    for (i2=0;i2<=7;i2++){
      if (CLIENTplayer->party[i2]!=NULL){
        i++;
        tnpc=(npc*)CLIENTplayer->party[i2]->more; //shortcut

        img0(qkstf->graphic,4,i*64-2+32,getportrait(tnpc->port));



        txtset(t,tnpc->name);
        x=8; y=i*64+32+16-8+32;
        txtcol=rgb(0,0,0);
        txtout(qkstf->graphic,x,y,t);
        txtout(qkstf->graphic,x+2,y+2,t);
        txtout(qkstf->graphic,x+2,y,t);
        txtout(qkstf->graphic,x,y+2,t);
        txtcol=rgb(255,255,255);
        if (i2==0) txtcol=namecolour;
        txtout(qkstf->graphic,x+1,y+1,t);
        txtcol=rgb(255,255,255);

        x2=tnpc->hp&32767;
        txtnumint(t,x2);
        x=64+32-16+4; y=i*64+4+32;
        if (x2<100) x=x+10;
        if (x2<10) x=x+10;


        txtcol=rgb(168,30,24);
        if (tnpc->hp&32768) txtcol=rgb(24,168,24);
        txtout(qkstf->graphic,x,y,t);
        txtout(qkstf->graphic,x+2,y+2,t);
        txtout(qkstf->graphic,x+2,y,t);
        txtout(qkstf->graphic,x,y+2,t);
        txtcol=rgb(255,64,32);
        if (tnpc->hp&32768) txtcol=rgb(64,255,64);
        txtout(qkstf->graphic,x+1,y+1,t);

        txtnumint(t,tnpc->mp);
        x=64+32-16+4; y=i*64+4+16+4+32;
        if (tnpc->mp<100) x=x+10;
        if (tnpc->mp<10) x=x+10;


        txtcol=rgb(72,72,168);
        txtout(qkstf->graphic,x,y,t);
        txtout(qkstf->graphic,x+2,y+2,t);
        txtout(qkstf->graphic,x+2,y,t);
        txtout(qkstf->graphic,x,y+2,t);
        txtcol=rgb(96,128,255);
        txtout(qkstf->graphic,x+1,y+1,t);


        x2=0;
        for (x=0;x<=255;x++){
          if (spell[i2][x]) x2=1;
        }
        if (x2){
          if (client_spellwait[i2]){
            img0(qkstf->graphic,4+56-2-2,i*64-16+32+32+1,spellbookmini2);
          }else{
            img0(qkstf->graphic,4+56-2-2,i*64-16+32+32+1,spellbookmini);
          }
        }


      }
    }

  } //upflags_update==TRUE;

  if (qkstf->mouse_click){
    qkstf->mouse_click=FALSE;
    i2=(qkstf->mouse_y-32)/64;
    x=qkstf->mouse_x; y=qkstf->mouse_y-32-64*i2;
    for (i=0;i<=7;i++){ if (CLIENTplayer->party[i]!=NULL){
      if (i2==0){

        if ((x>=(56+4))&&(x<(56+4+32))){ //hide/unhide spellbook!
          if ((y>=(16))&&(y<(16+32))){

            if (party_spellbook_frame[i]->offset_y<768){
              party_spellbook_frame[i]->offset_y+=1536;
            }else{
              if (party_spellbook_frame[i]->offset_y>=768) party_spellbook_frame[i]->offset_y-=1536;
            }


          }}


        if (x<(56+4)){ //hide/unhide!
          if (party_frame[i]->offset_y<768){
            party_frame[i]->offset_y+=1536;
          }else{
            if (party_frame[i]->offset_y>=768) party_frame[i]->offset_y-=1536;
          }
        }//hide/unhide

        goto qkstf_mcdone;
      }
      i2--;
    }}
qkstf_mcdone:;
  }







  img(tmini_1,0,0,mini_1); //copy original background

  //underground?
  moonlight=0;
  if (AMBIENTLIGHT_SHOWSUN==FALSE){
    img0(tmini_1,7,4,cave); goto underground;
  }








  //use btime
  if ((btime>5)&&(btime<20)){
    x=(float)128.0f-((btime-5.0f)*6.5f)-32+8+4-8;
    y=16+4-(sin((btime-5.0f)/4.8f)*14.0f)-8;
    if ((btime>7)&&(btime<18)){
      img0(tmini_1,x,y,sun);
    }else{
      img0(tmini_1,x,y,sun2);
    }
  }


  if (moon1==NULL) {moon1=OBJnew_local(); moon1->type=73;}

  //display moons!
  f=btime2*1.125f+3.0f; //first moon fixed offset sun+4 hours
  x=f/24; f-=(float)x*24.0f;
  if ((f>5)&&(f<20)){

    if (f>=12.0f) f3=-(f-12.0f); else f3=12.0f-f;
    f3/=1.125f;
    f2=btime+f3;

    f4=f; f=12.0f;
    if (f2>=24.0f) f2-=24.0f;
    if (f2<0.0f) f2+=24.0f;

    if (f2>=f) f2=f2-f; else f2=f2+(24.0f-f);
    f2+=1.5f; if (f2>=24.0f) f2-=24.0f;
    //f2=distance LEFT of the sun 0-24.0f
    x2=f2/3.0f;

    moonlight=4-abs(4-x2);

    x2*=1024;



    x2+=73;
    moon1->type=x2;




    f=f4;
    x=(float)128.0f-((f-5.0f)*6.5f)-32+8+4-8;
    y=16+4-(sin((f-5.0f)/4.8f)*14.0f)-8;
    getspr(moon1);
    img0(tmini_1,x,y,bt32);
  }

  //display moons! (lol this will be interesting!)
  f=btime2*1.1875f+5.0f; //first moon fixed offset sun+4 hours
  x=f/24; f-=(float)x*24.0f;
  if ((f>5)&&(f<20)){

    //calculate moon phase!
    if (f>=12.0f) f3=-(f-12.0f); else f3=12.0f-f;
    f3/=1.1875f;

    f2=btime+f3; 
    f4=f; f=12.0f;
    if (f2>=24.0f) f2-=24.0f;
    if (f2<0.0f) f2+=24.0f;

    if (f2>=f) f2=f2-f; else f2=f2+(24.0f-f);
    f2+=1.5f; if (f2>=24.0f) f2-=24.0f;
    //f2=distance LEFT of the sun 0-24.0f
    x2=f2/3.0f;

    if (moonlight<(4-abs(4-x2))) moonlight=4-abs(4-x2);

    x2*=1024;


    x2+=73;
    moon1->type=x2;

    f=f4;
    x=(float)128.0f-((f-5.0f)*6.5f)-32+8+4-8;
    y=16+4-(sin((f-5.0f)/4.8f)*14.0f)-8;
    getspr(moon1);
    img0(tmini_1,x,y,bt32);
  }




  if ((btime>7)&&(btime<18)){


    img0(tmini_1,0,16,horizon);//GPFGPF
    //img0(tmini_1,0,16,horizontesting);//GPFGPF testing
  }else{
    img0(tmini_1,0,16,horizon2);
  }



  txtset(t,"");
  if (windy2==1) txtset(t,"N");
  if (windy2==-1) txtset(t,"S");
  if (windx2==1) txtadd(t,"W");
  if (windx2==-1) txtadd(t,"E");
  if (t->l){
    txtfnt=fnt2;
    x=128-8-t->l*8; y=2;
    txtcol=rgb(168,60,0);
    txtout(tmini_1,x+1,y+1,t);
    txtfnt=fnt1;
  }

underground:
  img(qkstf->graphic,0,0,tmini_1); //moon and sun on qk stat

  img(volcontrol_surf,0,0,volcontrol_background);
  img(volcontrol_surf,46+u6ovolume*20/72-4,8+3,volcontrol_tab1);
  img(volcontrol_surf,46+u6omidivolume*20/72-4,32+8-5,volcontrol_tab3);
  img(volcontrol_surf,46+u6ovoicevolume*20/72-4,32+8-5+24,volcontrol_tab2);
  if (u6ovoicevolume==0){
    img0(volcontrol_surf,12,60,vm_volmmute);
  }




  if (musickeyboard->mouse_click){
    //calculate key pressed
    x=musickeyboard->mouse_x;
    //black key?
    if (musickeyboard->mouse_y<=25){
      //9,7b,9,7b,17,7b,9,7b,9,7b,17...
      y2=0; y3=1; x3=8;
nextblackkey:
      if ((x>=x3)&&(x<(x3+9))){
        x2=y3; goto gotkey;
      }
      x3+=16; x4=x3; y3+=2;
      if (y2==1) x3+=8;
      if (y2==4) x3+=8;
      if (y2==6) x3+=8;
      if (y2==9) x3+=8;
      if (y2==11) x3+=8;
      if (y2==14) x3+=8;
      if (y2==16) x3+=8;
      if (y2==19) x3+=8;
      if (y2==21) x3+=8;
      if (y2==24) x3+=8;
      if (y2==26) x3+=8;
      if (y2==29) x3+=8;
      if (y2==31) x3+=8;
      if (x4!=x3) y3++;
      if (y2==34) goto blackkeycheck_done;
      y2++; goto nextblackkey;
    }
blackkeycheck_done:
    //white keys
    f=x; f/=13.7f; x2=f; x3=x2;
    for (y2=1;y2<=x3;y2++){
      x2++;
      if (y2==3) x2--;
      if (y2==7) x2--;
      if (y2==10) x2--;
      if (y2==14) x2--;
      if (y2==17) x2--;
      if (y2==21) x2--;
      if (y2==24) x2--;
      if (y2==28) x2--;
      if (y2==31) x2--;
      if (y2==35) x2--;
      if (y2==38) x2--;
      if (y2==42) x2--;
      if (y2==45) x2--;
    }
gotkey: //x2 is value of key

    mididown(0,24+x2); musickeyboard_set=24+x2;

    txtset(t,"Press computer key to assign to music keyboard note("); txtnumint(t2,x2); txtadd(t,t2); txtadd(t,")");

    STATUSMESSadd(t);
    musickeyboard->mouse_click=0;
  }




  if (portraitlook_wait){





    static object* portraitlook_obj=OBJnew_local();
    if (portraitlook_equip){
      for (i=0;i<8;i++){
        if (portraitlook_obj->type=portraitlook_type[i]){
          getspr(portraitlook_obj);


          if (i==0) img0(statusmessage_viewnpc->graphic,helmx+112,helmy-128,bt32);
          if (i==1) img0(statusmessage_viewnpc->graphic,wep_rightx+112,wep_righty-128,bt32);
          if (i==2) img0(statusmessage_viewnpc->graphic,wep_leftx+112,wep_lefty-128,bt32);
          if (i==3) img0(statusmessage_viewnpc->graphic,armourx+112,armoury-128,bt32);
          if (i==4) img0(statusmessage_viewnpc->graphic,bootsx+112,bootsy-128,bt32);
          if (i==5) img0(statusmessage_viewnpc->graphic,ring_rightx+112,ring_righty-128,bt32);
          if (i==6) img0(statusmessage_viewnpc->graphic,ring_leftx+112,ring_lefty-128,bt32);
          if (i==7) img0(statusmessage_viewnpc->graphic,neckx+112,necky-128,bt32);

          if (portraitlook_plusbonus[i]){

            if (i==0){x=helmx;y=helmy;}
            if (i==1){x=wep_rightx;y=wep_righty;}
            if (i==2){x=wep_leftx;y=wep_lefty;}
            if (i==3){x=armourx;y=armoury;}
            if (i==4){x=bootsx;y=bootsy;}
            if (i==5){x=ring_rightx;y=ring_righty;}
            if (i==6){x=ring_leftx;y=ring_lefty;}
            if (i==7){x=neckx;y=necky;}
            x+=112; y-=128;
            x+=24;
            txtset(t,"+");
            txtnumint(t2,portraitlook_plusbonus[i]);
            txtadd(t,t2);
            txtfnt=fnt3; txtcol=0;
            txtout(statusmessage_viewnpc->graphic,x,y-1,t);
            txtout(statusmessage_viewnpc->graphic,x-1,y,t);
            txtout(statusmessage_viewnpc->graphic,x+1,y,t);
            txtout(statusmessage_viewnpc->graphic,x,y+1,t);
            txtout(statusmessage_viewnpc->graphic,x-1,y-1,t);
            txtout(statusmessage_viewnpc->graphic,x+1,y-1,t);
            txtout(statusmessage_viewnpc->graphic,x+1,y+1,t);
            txtout(statusmessage_viewnpc->graphic,x-1,y+1,t);
            txtcol=rgb(32,255,32);
            txtout(statusmessage_viewnpc->graphic,x,y,t);
            txtfnt=fnt1; txtcol=rgb(255,255,255);
          }








        }//portraitlook_type[i]
      }//i
    }//portraitlook_equip

    txtset(t,portraitlook_name);
    x=56-4*t->l; y=128-16-8;
    txtcol=rgb(0,0,0);
    txtout(statusmessage_viewnpc->graphic,x,y,t);
    txtout(statusmessage_viewnpc->graphic,x+2,y+2,t);
    txtout(statusmessage_viewnpc->graphic,x+2,y,t);
    txtout(statusmessage_viewnpc->graphic,x,y+2,t);
    txtcol=portraitlook_namecolour;
    txtout(statusmessage_viewnpc->graphic,x+1,y+1,t);
    txtcol=rgb(255,255,255);

  }




  for(i=0;i<=22;i++){
    if ((i>=0)&&(i<=7)) pmf=party_frame[i];
    if ((i>=8)&&(i<=15)) pmf=party_spellbook_frame[i-8];
    if (i==16) pmf=musickeyboard;
    if (i==17) pmf=inpf;
    if (i==18) pmf=con_frm;
    if (i==19) pmf=volcontrol;
    if (i==20) pmf=qkstf;
    if (i==21) pmf=minimap_frame;
    if (i==22) pmf=tmap_frame;
    x=pmf->offset_x; y=pmf->offset_y;
    //get dimentions of frame as x2,y2
    if (pmf->graphic&&(pmf->size_x==0)&&(pmf->size_y==0)){
      x2=pmf->graphic->d.dwWidth; y2=pmf->graphic->d.dwHeight;
    }else{
      x2=pmf->size_x; y2=pmf->size_y;
    }
    if ((x<=(2048-x2))&&(y<=(1536-y2))){ //onscreen (or supposed to be onscreen!)
      x3=8-x2; if (x<x3) pmf->offset_x=x3;
      y3=8-y2; if (y<y3) pmf->offset_y=y3;
      x3=1024-8; if (x>x3) pmf->offset_x=x3;
      y3=768-8; if (y>y3) pmf->offset_y=y3;
      if (i==16){cltset.musickeyboard_offset_x=pmf->offset_x; cltset.musickeyboard_offset_y=pmf->offset_y;}
      if (i==17){cltset.inpf_offset_x=pmf->offset_x; cltset.inpf_offset_y=pmf->offset_y;}
    }//onscreen
    //update default frame settings
    if ((i>=0)&&(i<=7)){
      cltset.party_frame_offset_x[i]=pmf->offset_x; cltset.party_frame_offset_y[i]=pmf->offset_y;
      if (cltset.party_frame_offset_x[i]>1024) cltset.party_frame_offset_x[i]-=2048;
    }
    if ((i>=8)&&(i<=15)){
      cltset.party_spellbook_frame_offset_x[i-8]=pmf->offset_x; cltset.party_spellbook_frame_offset_y[i-8]=pmf->offset_y;
      if (cltset.party_spellbook_frame_offset_x[i-8]>1024) cltset.party_spellbook_frame_offset_x[i-8]-=2048;
    }
    if (i==18){cltset.con_frm_offset_x=pmf->offset_x; cltset.con_frm_offset_y=pmf->offset_y;}
    if (i==20){cltset.qkstf_offset_x=pmf->offset_x; cltset.qkstf_offset_y=pmf->offset_y;}
    if (i==19){cltset.volcontrol_offset_x=pmf->offset_x; cltset.volcontrol_offset_y=pmf->offset_y;}
    if (i==21){cltset.minimap_offset_x=pmf->offset_x; cltset.minimap_offset_y=pmf->offset_y;}
    if (i==22){cltset.tmap_offset_x=pmf->offset_x; cltset.tmap_offset_y=pmf->offset_y;}
  }//i (frame)
  clientsettingsvalid=TRUE;




  //frame: display
  pn=firstpanel;
checkpanel2:
  if (pn!=NULL){
    lpn=pn; pn=(FRAME*)pn->next; goto checkpanel2;
  }
  pn=lpn;
displaypanel:
  if (pn!=NULL){
    if (pn->graphic!=NULL){
      if (pn->graphic_use0pixel==TRUE){


        if (pn->mouse_over||(pn->mouse_over_transparent==FALSE)){
          img(ps,pn->offset_x,pn->offset_y,pn->graphic);
        }else{

          if (pn->mouse_over_transparent==1){
            imgt(ps,pn->offset_x,pn->offset_y,pn->graphic);
          }else{
            img75t(ps,pn->offset_x,pn->offset_y,pn->graphic);

          }

        }


      }else{

        if (pn->mouse_over||(pn->mouse_over_transparent==FALSE)){
          img0(ps,pn->offset_x,pn->offset_y,pn->graphic);
        }else{


          if (pn->mouse_over_transparent==1){
            imgt0(ps,pn->offset_x,pn->offset_y,pn->graphic);
          }else{

            img75t0(ps,pn->offset_x,pn->offset_y,pn->graphic);
          }



        }





      }
    }
    FRM_type=(FRM_TYPE*)pn->firstobject;
displayobj:
    if (FRM_type!=NULL)
    {
      x=FRM_type->type;
      if (x==1){ //FRM_ONOFF
        tonoff=(FRM_ONOFF*)FRM_type;
        if (tonoff->value==0) img0(ps,pn->offset_x+tonoff->offset_x,pn->offset_y+tonoff->offset_y,tonoff->graphic_off);
        if (tonoff->value==1) img0(ps,pn->offset_x+tonoff->offset_x+tonoff->offset2_x,pn->offset_y+tonoff->offset_y+tonoff->offset2_y,tonoff->graphic_on);
      }
      if (x==2){ //FRM_IMAGE
        timage=(FRM_IMAGE*)FRM_type;
        if (timage->graphic_use0pixel==TRUE){
          img(ps,pn->offset_x+timage->offset_x,pn->offset_y+timage->offset_y,timage->graphic);
        }else{
          img0(ps,pn->offset_x+timage->offset_x,pn->offset_y+timage->offset_y,timage->graphic);
        }
      }
      if (x==3){ //FRM_TXT
        ttxt=(FRM_TXT*)FRM_type;
        if (ttxt->timer!=0.0f){
          if (ttxt->timer<=0.015625f){
            ttxt->timer=0.015625f;
          }else{
            ttxt->timer-=et;
          }}
        if (ttxt->timer!=0.015625f){
          thfont=txtfnt;
          if (ttxt->font!=NULL) txtfnt=ttxt->font;
          if (ttxt->special&1){
            txtcol=0;
            txtout(ps,pn->offset_x+ttxt->offset_x-1,pn->offset_y+ttxt->offset_y-1,ttxt->text);
            txtout(ps,pn->offset_x+ttxt->offset_x+1,pn->offset_y+ttxt->offset_y+1,ttxt->text);
            txtout(ps,pn->offset_x+ttxt->offset_x+1,pn->offset_y+ttxt->offset_y-1,ttxt->text);
            txtout(ps,pn->offset_x+ttxt->offset_x-1,pn->offset_y+ttxt->offset_y+1,ttxt->text);
          }
          txtcol=ttxt->col;
          txtout(ps,pn->offset_x+ttxt->offset_x,pn->offset_y+ttxt->offset_y,ttxt->text);
          txtcol=0xFFFFFF; txtfnt=thfont;
        }
      }
      if (x==4){ //FRM_INPUT
        tinp=(FRM_INPUT*)FRM_type;
        thfont=txtfnt;
        if (tinp->col!=0xFFFFFF) txtcol=tinp->col;
        if (tinp->font!=NULL) txtfnt=tinp->font;
        if (tinp->prefix!=NULL){txtset(t,tinp->prefix);}else{txtset(t,"");}
        txtadd(t,tinp->input);
        if (GETINPUT_current()==tinp->input){
          i=ett*tinp->cursor_blinkrate;
          if (!(i&1)){
            txtNEWLEN(t,t->l+1);
            t->d[t->l-1]=tinp->cursor;
          }
          else
          {
            txtadd(t," ");
          }
        }
        if (tinp->suffix!=NULL) txtadd(t,tinp->suffix);
        //optional graphic
        img0(ps,pn->offset_x+tinp->offset_x,pn->offset_y+tinp->offset_y,tinp->graphic);


        //inpf2->input
        //check if input 't' will go beyond screen boundry
        if (tinp==inpf2){
          if (t->l>2){
            tagxy.cx=0; tagxy.cy=0;
            ps->s->GetDC(&taghdc);
            SelectObject(taghdc,txtfnt);
            GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
            ps->s->ReleaseDC(taghdc);
            if (pn->offset_x+tinp->offset_x+tagxy.cx>=1024){
inpf2crop:
              txtset(t2,t); txtset(t,"..."); txtadd(t,t2);
              if (t->l>=6){
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                SelectObject(taghdc,txtfnt);
                GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                ps->s->ReleaseDC(taghdc);
                if (pn->offset_x+tinp->offset_x+tagxy.cx>=1024){
                  txtright(t,t->l-4);
                  goto inpf2crop;
                }//...+tagxy.cx>=1024
              }//>=6
            }//...+tagxy.cx>=1024
          }//>2
        }//inpf2

        if (tinp->special&1){
          txtcol=0;
          txtout(ps,pn->offset_x+tinp->offset_x-1,pn->offset_y+tinp->offset_y-1,t);
          txtout(ps,pn->offset_x+tinp->offset_x+1,pn->offset_y+tinp->offset_y+1,t);
          txtout(ps,pn->offset_x+tinp->offset_x+1,pn->offset_y+tinp->offset_y-1,t);
          txtout(ps,pn->offset_x+tinp->offset_x-1,pn->offset_y+tinp->offset_y+1,t);
        }
        txtcol=tinp->col;
        txtout(ps,pn->offset_x+tinp->offset_x,pn->offset_y+tinp->offset_y,t);
        txtcol=0xFFFFFF; txtfnt=thfont;
      }
      FRM_type=(FRM_TYPE*)FRM_type->next; goto displayobj;
    }
    pn=(FRAME*)pn->previous; goto displaypanel;
  }


  if (shiftnum_show){
    x=14*32;
    y=12*32;
    txtset(t,"How many? ");
    txtadd(t,tshiftnum);
    txtcol=0;
    txtout(ps,x,y,t);
    txtout(ps,x+2,y+2,t);
    txtout(ps,x+2,y,t);
    txtout(ps,x,y+2,t);
    txtcol=rgb(255,255,255);
    txtout(ps,x+1,y+1,t);
  }


  if (endgame){
    if (endgame_timer<=140.0f){

      x=0; txtset(t5,""); x5=187;


      if (endgame_message==0){
        STATUSMESSadd("The Codex has vanished!");
        endgame_message++;
      }

      if (endgame_message==1){
        if (endgame_timer>=4.0f){
          txtset(t5,"A glowing portal springs from the floor!");
          endgame_message++;
        }
      }

      if (endgame_message==2){
        x=1;
        if (endgame_timer>=8.0f){
          STATUSMESSadd("You have gained strength, dexterity and intelligence!");
          txtset(t5,"From its crimson depths Lord British emerges, trailed by the mage Nystul.");
          endgame_message++;
        }
      }

      if (endgame_message==3){
        x=2;
        if (endgame_timer>=12.0f){
          x5=5; txtset(t5,"Anguish and disbelief prevail on the royal seer's face.");
          endgame_message++;
        }
      }

      if (endgame_message==4){
        x=2;
        if (endgame_timer>=16.0f){
          x5=4;
          txtset(t5,"But Lord British directs his stony gaze at you and speaks as if to a wayward child.\\\"Thou didst have just cause to burgle our Codex, I trust\", his Majesty says.\\\"But for Virtue's sake...\"\\\"WHAT HAST THOU DONE WITH IT?\"");
          endgame_message++;
        }
      }

      if (endgame_message==5){
        x=2;
        if (endgame_timer>=32.0f){
          if (tplay->party[0]){
            tnpc=(npc*)tplay->party[0]->more;
            x5=tnpc->port;
          }else x5=portlast;
          txtset(t5,"\"Was the book ever truly ours, your majesty?\"\\\"Was it written for Britannia alone?\"\\\"Thou dost no longer hold the Codex, but is its wisdom indeed lost?\"\\\"Look into the Vortex, and let the Codex answer for itself!\"\\You pick up the concave lens and pass it to the King.");
          endgame_message++;
        }
      }

      if (endgame_message==6){
        x=2;
        if (endgame_timer>=52.0f){
          x5=4; txtset(t5,"Lord British holds the glass before the wall.");
          endgame_message++;
        }
      }

      if (endgame_message==7){
        x=3;
        if (endgame_timer>=56.0f){
          txtset(t5,"The Codex of Ultimate Wisdom wavers into view against a myriad of swimming stars!\\Yet the book remains closed.");
          endgame_message++;
        }
      }

      if (endgame_message==8){
        x=3;
        if (endgame_timer>=64.0f){
          txtset(t5,"And waves of heat shimmer in the air, heralding the birth of another red gate!");
          endgame_message++;
        }
      }

      if (endgame_message==9){
        x=4;
        if (endgame_timer>=68.0f){
          x5=166; txtset(t5,"King Draxinusom of the Gargoyles strides forward.\\Flanked by a small army of wingless attendants.\\Like Lord British, he seems to suppress his rage only through a heroic effort of will.\\His scaly hand grasps your shoulder, and your Amulet of Submission grows very warm.\\\"Thy time hath come, Thief\", he says.");
          endgame_message++;
        }
      }

      if (endgame_message==10){
        x=5;
        if (endgame_timer>=88.0f){
          if (tplay->party[0]){
            tnpc=(npc*)tplay->party[0]->more;
            x5=tnpc->port;
          }else x5=portlast;
          txtset(t5,"\"Join my Lord in his search for peace, I beg thee.\"\\Quickly you reach down to seize the convex lens...\\...and you press it into the hand of the towering Gargoyle king, meeting his sunken eyes.");
          endgame_message++;
        }
      }

      if (endgame_message==11){
        x=6;
        if (endgame_timer>=100.0f){
          x5=166; txtset(t5,"At your urging, King Draxinusom reluctantly raises his lens to catch the light.");
          endgame_message++;
        }
      }

      if (endgame_message==12){
        x=8;
        if (endgame_timer>=104.0f){
          txtset(t5,"As Lord British holds up his own lens, every eye in the room, human and Gargoyle alike,\\fixes upon the image of the Codex which shines upon the wall.");
          endgame_message++;
        }
      }

      if (endgame_message==13){
        x=8;
        if (endgame_timer>=112.0f){
          STATUSMESSadd("Your Amulet of Submission has shattered!");
          soundplay2(u6osound[18],u6osound_volume[18]);
          txtset(t5,"The ancient book opens.\\Both kings gaze upon its pages in spellbound silence, as the eloquence of Ultimate Wisdom is revealed\\in the tongues of each lord's domain.\\You, too, can read the answers the Codex gives...\\...and when its wisdom is gleaned, when Lord British  and King Draxinusom turn to each other as friends,\\hating no longer, fearing no more, you know that your mission in Britannia has ended at last.");
          endgame_message++;
        }
      }

      if (endgame_message==14){
        x=9;
      }

      if (t5->l){
        for (i3=0;i3<=255;i3++){ if (sfx[i3].type==0){
          sfx[i3].type=3;
          sfx[i3].x=920;
          sfx[i3].y=851-3;
          sfx[i3].x2=t5->l;
          static txt *dbgt5;
          dbgt5=txtnew();
          txtset(dbgt5,t5);
          sfx[i3].p=dbgt5;
          txtset(t,(txt*)sfx[i3].p);
          txtset(t2,"?"); t2->d2[0]=92;
          z=txtsearch(t,t2);
          if (z==0) z=t->l; else z--;
          f=(float)z/10.0f;
          if (f<1)f=1;
          if(f>4)f=4;
          sfx[i3].wait=f;
          sfx[i3].more=x5;
shiftdown3:
          for (i2=0;i2<=255;i2++){ if ((sfx[i2].type==3)&&(i2!=i3)){
            if ((sfx[i3].y==sfx[i2].y)||(sfx[i3].y==(sfx[i2].y+1))||(sfx[i3].y==(sfx[i2].y-1))) {sfx[i3].y=sfx[i2].y+2; goto shiftdown3;}
          }}
          goto endgame_donemessage;
        }}
      }//t5->l
endgame_donemessage:

      img(ps,608,476,endgame_image[x]);
      endgame_timer+=et;

    }//timer<=140.0f
  }//endgame



  if (statusmessage_viewprev->mouse_over){
    statusmessage_viewprev->mouse_over=FALSE;
    if (drg!=statusmessage_viewprev){
      x=statusmessage_viewprev->offset_x;
      y=statusmessage_viewprev->offset_y-24;
      for (i=0;i<=7;i++){
        if (STATUSMESSprev[i]->l){
          txtset(t,STATUSMESSprev[i]);
          txtfnt=fnt1naa;
          txtcol=rgb(0,0,0);
          txtout(ps,x,y,t);
          txtout(ps,x+2,y+2,t);
          txtout(ps,x+2,y,t);
          txtout(ps,x,y+2,t);
          txtout(ps,x+1,y,t);
          txtout(ps,x+2,y+1,t);
          txtout(ps,x,y+1,t);
          txtout(ps,x+1,y+2,t);
          txtcol=rgb(255,255,255);
          txtfnt=fnt1;
          txtout(ps,x+1,y+1,t);
        }
        y-=24;
      }//i
    }
  }


  if (CLIENTplayer->mobj!=NULL){ //show mobject: active player
    getspr(CLIENTplayer->mobj);
    img0(ps,mx-16,my-16,bt32);
    if (keyon[VK_SPACE]&&U6O_DEBUG) {
      txtnumint(t,CLIENTplayer->mobj->type&1023); txtadd(t,"<MouseObject type"); txtout(ps,0,16,t);
      txtnumint(t,CLIENTplayer->mobj->type>>10); txtadd(t,"<MouseObject sub-index"); txtout(ps,0,48,t);
      txtnumint(t,CLIENTplayer->mobj->info); txtadd(t,"<MouseObject info"); txtout(ps,0,80,t);
      txtnumint(t,CLIENTplayer->mobj->more2); txtadd(t,"<MouseObject more"); txtout(ps,0,112,t);
    }


  }




  //DISPLAY DEBUG INFO WHEN SPACE IS HELD (ONLY IF U6O_DEBUG IS TRUE)
  if (keyon[VK_SPACE]&&U6O_DEBUG){
    txtnumint(t,btimeh); txtout(ps,512,32,t);
    txtnumint(t,framerate); txtout(ps,512,16,t);

    txtnum(t,dv); txtadd(t,"<debug value 1"); txtout(ps,512+64,16,t);
    txtnum(t,dv2); txtadd(t,"<debug value 2"); txtout(ps,512+64,16+64+32,t);

    txtnumint(t,bt[tpy+my/32][tpx+mx/32]&1023); txtadd(t,"<bt"); txtout(ps,512+64,16+32,t);

    x=tpx+mx/32; txtnumint(t,x); txtadd(t,"<x"); txtout(ps,1024-64,0,t);
    y=tpy+my/32; txtnumint(t,y); txtadd(t,"<y"); txtout(ps,1024-64,32,t);

    if (NEThost){
      txtset(t,"Network Status: HOST");
    }else{
      txtset(t,"Network Status: CLIENT");
    }
    txtout(ps,1024-128-64,768-32,t);

    //#ifdef HOST
    //txtnumint(t,vlnkb_last); txtadd(t,"<vlnkb_last (number of allocated VLNK slots MAX:65536)"); txtout(ps,0,128,t);
    //txtnumint(t,objb_last); txtadd(t,"<objb_last (number of allocated OBJ slots MAX:524228)"); txtout(ps,0,128+32*1,t);
    //#endif
  }//END DEBUG INFO

}
intro_refresh:




refreshcount++;



if ((gotfocus==FALSE)&&(dxrefresh)) goto skiprefresh;
refresh();
skiprefresh:

mb=FRAME_mb; //restore physical mouse values
omx=mx; omy=my; //set old mouse values
isit=FALSE;

//BACKGROUND/FOREGROND MIDI
x=0;//set volume
if (midiinfo_loaded){
  if (u6omidivolume){//midi cannot be processed if volume==NULL

    if (midi_foreground!=-1){ //set midi_foreground as loaded midi
      if (midi_foreground!=midi_loaded){
        midi_loaded=midi_foreground;
        u6omidi->LoadMidiFromFile(u6omidi_filename[midi_loaded]->d,TRUE);
        if (midipause==0){
          u6omidi->Play();
isplayingwait0: if (u6omidi->IsPlaying()==S_FALSE) goto isplayingwait0;
        }//midipause==0
        x=1;
      }//midi_foreground!=midi_loaded
      midi_foreground=-1;
    }//midi_foreground!=-1

    if (midi_foreground_wait){
      midi_foreground_wait-=et; if (midi_foreground_wait<0.0f) midi_foreground_wait=0.0f;
      if (u6omidi->IsPlaying()==S_FALSE) midi_foreground_wait=0.0f;//midi finished playing
      if (midi_foreground_wait==0.0f){
        u6omidi->Stop();//stop playing foreground midi
isplayingwait2: if (u6omidi->IsPlaying()==S_OK) goto isplayingwait2;
      }
    }

    if (midi_foreground_wait==0.0f){//no foreground midi is playing
      if (midi_loaded!=midi_background){//load correct midi
        if (u6omidi->IsPlaying()==S_FALSE){//midi is not playing
          midi_loaded=midi_background;
          u6omidi->LoadMidiFromFile(u6omidi_filename[midi_loaded]->d,TRUE);
        }
      }//midi_loaded!=midi_background

      if (u6omidi->IsPlaying()==S_FALSE){//midi is not playing
        if (midipause==0){
          u6omidi->Play();
isplayingwait1: if (u6omidi->IsPlaying()==S_FALSE) goto isplayingwait1;
        }//midipause==0
        x=1;
      }//==S_FALSE
    }//midi_foreground_wait==0.0f

    if (x){
      f=u6omidi_volume[midi_loaded];
      f=f*(float)u6omidivolume/255.0f;
      f=255-f; f=f*0.25f; f*=f;
      //DMUS_VOLUME_MAX     2000         +20 dB
      //DMUS_VOLUME_MIN   -20000        -200 dB
      u6omidi->SetMasterVolume(-f);
    }

  }//u6omidivolume
}//midiinfo_loaded


for (i=0;i<=7;i++){ //mouse flag cancel
  pmf=party_frame[i]; pmf->mouse_pressed=FALSE; pmf->mouse_over=FALSE; pmf->mouse_click=FALSE;
  pmf=party_spellbook_frame[i]; pmf->mouse_pressed=FALSE; pmf->mouse_over=FALSE; pmf->mouse_click=FALSE;
}
vf->mouse_click=FALSE; vf->mouse_over=FALSE; vf->mouse_pressed=FALSE;
con_frm->mouse_over=FALSE;
inpf->mouse_over=FALSE;
volcontrol->mouse_over=FALSE;
qkstf->mouse_over=FALSE;
con_frm->mouse_over=FALSE;
musickeyboard->mouse_over=FALSE;
inpf->mouse_over=FALSE;





ZeroMemory(&key,65536);

if (mb_release){
  if ((mb_release&1)&&(mb&1)) mb-=1;
  if ((mb_release&2)&&(mb&2)) mb-=2;
  mb_release=0;
}
mbclick=0;

if (U6O_DISABLEMUSIC==FALSE){
  if (midiinfo_loaded==FALSE){
    //load midiinfo.txt
    midiinfo_loaded=TRUE;
    txtset(t3," ");
    tfh=open(".\\midi\\midiinfo.txt");
midiinfo_next:
    txtfilein(t,tfh);
    if (t->l){
      txtucase(t);
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      i=txtnum(t2);
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      txtset(t4,".\\midi\\"); txtadd(t4,t2); txtadd(t4,".mid");
      if (u6omidi_filename[i]==NULL) u6omidi_filename[i]=txtnew();
      txtset(u6omidi_filename[i],t4);

      x2=100; //volume
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      if (t2->l){
        x2=txtnum(t2);
        if (x2<0) x=0;
        if (x2>100) x=100;
      }
      x2=(float)x2*2.55f;
      u6omidi_volume[i]=x2;


      goto midiinfo_next;
    }
    close(tfh);
    u6omidi->LoadMidiFromFile(u6omidi_filename[0]->d,TRUE);
  }//midiinfo_loaded
}


if (wavinfo_loaded==FALSE){
  //load wavinfo.txt
  wavinfo_loaded=TRUE;
  u6osoundtype_volume[0]=255; //UNKNOWN type 100%
  txtset(t3," ");
  tfh=open(".\\wav\\wavinfo.txt");
wavinfo_next:
  txtfilein(t,tfh);
  if (t->l){
    txtucase(t);
    x4=0;
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    //check for catagory definitions
    txtset(t4,"COMBAT"); if (txtsame(t2,t4)) x4=1;
    txtset(t4,"FOREGROUND"); if (txtsame(t2,t4)) x4=2;
    txtset(t4,"BACKGROUND"); if (txtsame(t2,t4)) x4=3;
    txtset(t4,"BACKGROUND_CONTINUOUS"); if (txtsame(t2,t4)) x4=4;
    if (x4){
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      f=txtnum(t2)*2.55f;
      x=f;
      if (x<0) x=0;
      if (x>255) x=255;
      u6osoundtype_volume[x4]=x;
      goto wavinfo_next;
    }
    i=txtnum(t2);
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    txtset(t4,".\\wav\\"); txtadd(t4,t2); txtadd(t4,".wav");
    if (u6osound[i]==0) u6osound[i]=soundload(t4->d);
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    u6osound_type[i]=0; //UNKNOWN
    txtset(t4,"COMBAT"); if (txtsame(t2,t4)) u6osound_type[i]=1;
    txtset(t4,"FOREGROUND"); if (txtsame(t2,t4)) u6osound_type[i]=2;
    txtset(t4,"BACKGROUND"); if (txtsame(t2,t4)) u6osound_type[i]=3;
    txtset(t4,"BACKGROUND_CONTINUOUS"); if (txtsame(t2,t4)) u6osound_type[i]=4;
    u6osound_volumechange[i]=0;
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    if (t2->l){
      if (t2->d2[0]==43) txtright(t2,t2->l-1); //remove "+"
      x=txtnum(t2);
      if (x<-100) x=-100;
      if (x>100) x=100;
      u6osound_volumechange[i]=x;
    }//t->l
    f=u6osoundtype_volume[u6osound_type[i]];
    f2=u6osound_volumechange[i];
    f2=f2/100.0f+1.0f;
    x=f*f2;
    if (x<0) x=0;
    if (x>255) x=255;
    u6osound_volume[i]=x;
    goto wavinfo_next;
  }
  close(tfh);
}//wavinfo_loaded





if (STATUSMESSwait){
  STATUSMESSwait-=(et*(1.0f+(float)STATUSMESSpending->l*0.005f));
  if (STATUSMESSwait<=0.0f){
    //add to the buffer
    for (i=7;i>=1;i--){
      txtset(STATUSMESSprev[i],STATUSMESSprev[i-1]);
    }//i
    txtset(STATUSMESSprev[0],STATUSMESSdisplaying);
    STATUSMESSwait=0.0f;
    txtset(STATUSMESSdisplaying,"");
  }//STATUSMESSwait<=0.0f
}//STATUSMESSwait

if (STATUSMESSpending->l){
  if (STATUSMESSdisplaying->l==0){
    txtsetchar(t,13);
    if (i=txtsearch(STATUSMESSpending,t)){
      txtset(STATUSMESSdisplaying,STATUSMESSpending);
      txtleft(STATUSMESSdisplaying,i-1);
      txtright(STATUSMESSpending,STATUSMESSpending->l-i);
    }else{
      txtset(STATUSMESSdisplaying,STATUSMESSpending);
      txtset(STATUSMESSpending,"");
    }
    STATUSMESSwait=1.0f+(float)STATUSMESSdisplaying->l/16.0f;
    STATUSMESSskipok=0;
    if (STATUSMESSdisplaying->d2[0]==255){
      txtright(STATUSMESSdisplaying,STATUSMESSdisplaying->l-1);
      STATUSMESSskipok=1;
    }
  }else{
    if (STATUSMESSskipok){
      STATUSMESSskipok=0;
      STATUSMESSwait=0.01f;
    }
  }
}

if (portraitlook_wait){
  portraitlook_wait-=et;
  if (portraitlook_wait<0.0f){
    portraitlook_wait=0.0f;
    if (statusmessage_viewnpc->offset_x<1024) statusmessage_viewnpc->offset_x+=2048;
  }
}


keyon[0xD8]=FALSE; keyon[0xD9]=FALSE; //release mousewheel "buttons"
