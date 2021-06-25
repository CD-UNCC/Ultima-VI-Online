#include "function_host.h"
#include "function_both.h" /* LOGadd etc */
#include <math.h> /* fabs etc. */

#pragma warning(disable: 4018 4244)

void function_host_init(void) { /* this groups all the initialisation stuff that was floating around the .h file */

  //sockets_accept sets newsocket to new sockets id and waits till user sets
  //newsocket=INVALID_SOCKET before accepting new sockets
  //shared global variables
  newsocket=INVALID_SOCKET;
  newsocket_ip=0;
  //temp local variables (transferred to globals after successful connection)
  tnewsocket=INVALID_SOCKET;
  tnewsocket_ip=0;
  hsockets_accept=NULL;
  idsockets_accept=NULL;

  AUTOPICKUPfirst=0;
  AUTOPICKUPnextfree=0;
  housestorageadd=0;housestoragerestore=0; //flags used when saving/restoring house items in file
  //house creation tool 1.0 variables
  patchx=0;
  patchy=0; //base offset for adding objects/changing basetiles
  housenumber=65535; //house currently being created (1-?, 0 RESERVED, 65535=non-specific)
  basehousenumber=20;

  //OBJadd_allow=FALSE;
  OBJmove_allow=FALSE;
  OBJlist_last=0;

  //entry values
  WPF_NEXTTO=0;
  WPF_OBJECT=NULL;
  objsave_last=-1;
  objsave_node_last=-1;

  hrevive_infiniteloopexit=NULL;
  idrevive_infiniteloopexit=NULL;
  stealing_txt=NULL;
  stealing_MESSAGE=TRUE;
  OBJtmp=NULL;
  OBJtmp2=NULL;
  OBJtmp3=NULL; 
  OBJaddtocontainer_containermore=NULL;
}


void AUTOPICKUPadd(object* partymember, object *obj){//adds an item to the autopickup list
  static npc *tnpc;
  tnpc=(npc*)partymember->more;
  AUTOPICKUPobject[AUTOPICKUPnextfree]=obj;
  AUTOPICKUPett[AUTOPICKUPnextfree]=ett;
  AUTOPICKUPplayer[AUTOPICKUPnextfree]=tnpc->player;
  AUTOPICKUPpartymember[AUTOPICKUPnextfree]=partymember;
  AUTOPICKUPflags[AUTOPICKUPnextfree]=0;
  if (obj->info&32768){//remove mark if present and flag correctly
    obj->info^=32768;
    AUTOPICKUPflags[AUTOPICKUPnextfree]|=1;
  }
  AUTOPICKUPnextfree++;
  if (AUTOPICKUPnextfree==AUTOPICKUPfirst){
    AUTOPICKUPfirst++;
  }
AUTOPICKUPadd_selfmanagement:
  if (AUTOPICKUPfirst!=AUTOPICKUPnextfree){
    if (AUTOPICKUPett[AUTOPICKUPfirst]<(ett-1024.0f)){
      AUTOPICKUPfirst++; goto AUTOPICKUPadd_selfmanagement;
    }
  }
}//AUTOPICKUPadd

object *AUTOPICKUPcheck(player* tplayer, object *obj){
  //returns an object pointer to the npc to return the item to or NULL
  static short i,i2;
AUTOPICKUPcheck_selfmanagement:
  if (AUTOPICKUPfirst!=AUTOPICKUPnextfree){
    if (AUTOPICKUPett[AUTOPICKUPfirst]<(ett-1024.0f)){
      AUTOPICKUPfirst++; goto AUTOPICKUPcheck_selfmanagement;
    }
  }
  i=AUTOPICKUPfirst;
AUTOPICKUPcheck_next:
  if (i!=AUTOPICKUPnextfree){
    if (AUTOPICKUPobject[i]==obj){
      if (AUTOPICKUPplayer[i]==tplayer){
        for (i2=0;i2<=7;i2++){
          if (AUTOPICKUPpartymember[i]==tplayer->party[i2]){
            AUTOPICKUPobject[i]=NULL;//set object as invalid
            AUTOPICKUPett[i]=ett-1024.0f-1.0f;//set ett so object can easily be cleared
            if (AUTOPICKUPflags[i]&1){
              obj->info|=32768;
            }
            return AUTOPICKUPpartymember[i];
          }
        }//i2
        return NULL;
      }
    }
    i++; goto AUTOPICKUPcheck_next;
  }
  return NULL;
}//AUTOPICKUPcheck


DWORD WINAPI sockets_accept(LPVOID null_value){
  static long x;
sockets_accept_next:
  static sockaddr_in accept_addr;
  ZeroMemory(&accept_addr,sizeof(accept_addr));
  x=sizeof(accept_addr);
  tnewsocket=accept(u6osocket,(sockaddr*)&accept_addr,(int*)&x);
  tnewsocket_ip=NULL;
  if (accept_addr.sin_family==AF_INET){
    tnewsocket_ip=accept_addr.sin_addr.S_un.S_addr;
  }

  //ban by IP?
  static file *tfh;
  static txt *t=txtnew(),*t2=txtnew();
  tfh=open2("banip.txt",OF_READWRITE|OF_SHARE_COMPAT);
  if (tfh->h!=HFILE_ERROR){
banip_nextip:
    if (seek(tfh)<lof(tfh)){
      txtfilein(t,tfh);
      if (t->l){
        txtset(t2,t);
        x=(long)txtnum(t);
banip_nextbyte1: txtright(t,t->l-1); if (t->d2[0]!=46) goto banip_nextbyte1; txtright(t,t->l-1);
        x+=((long)txtnum(t)<<8);
banip_nextbyte2: txtright(t,t->l-1); if (t->d2[0]!=46) goto banip_nextbyte2; txtright(t,t->l-1);
        x+=((long)txtnum(t)<<16);
banip_nextbyte3: txtright(t,t->l-1); if (t->d2[0]!=46) goto banip_nextbyte3; txtright(t,t->l-1);
        x+=((long)txtnum(t)<<24);
        if (tnewsocket_ip==x){
          x=tnewsocket;
          txtset(t,"BannedIP_Blocked:"); txtadd(t,t2); LOGadd(t);//log
          shutdown(x,SD_RECEIVE|SD_SEND);
          SleepEx(4096,NULL);
          closesocket(x);
          goto sockets_accept_next;
        }//tnewsocket_ip==x
      }//t->l
      goto banip_nextip;
    }
    close(tfh);
  }//!=HFILE_ERROR




  x=1; setsockopt(tnewsocket,IPPROTO_TCP,TCP_NODELAY,(char*)&x,4);
  x=65536; setsockopt(tnewsocket,SOL_SOCKET,SO_RCVBUF,(char*)&x,4);
  x=65536; setsockopt(tnewsocket,SOL_SOCKET,SO_SNDBUF,(char*)&x,4);
  x=1; ioctlsocket(tnewsocket,FIONBIO,(unsigned long*)&x);


  //WSAAsyncSelect(newsocket, hWnd, WM_USER + 100, FD_READ | FD_WRITE | FD_CONNECT);

  /*
  timeval tv_r;
  tv_r.tv_sec = 0;
  tv_r.tv_usec = 500;
  fd_set socketset;
  FD_ZERO(&socketset);
  FD_SET(newsocket, &socketset);
  select(FD_SETSIZE, &socketset, NULL, NULL, &tv_r);
  */

  //x=1; ioctlsocket(newsocket, FIONBIO, (u_long FAR*) &x);

  //x=0; setsockopt(newsocket,SOL_SOCKET,SO_DONTLINGER,(char*)&x,4);

  send(tnewsocket,(char*)&U6O_SIGNATURE,4,0);

  newsocket_ip=tnewsocket_ip;
  newsocket=tnewsocket;
newsocket_wait:
  if (newsocket!=INVALID_SOCKET){
    SleepEx(15,FALSE);
    goto newsocket_wait;
  }
  goto sockets_accept_next;
  ExitThread(0);
  return 0;
}



/*
//volatile links (vlnk)
*/

void VLNKnew(void *lnks,void *lnk,unsigned long off){
  if (vlnkb_free_last==-1){
    vlnkb_off[++vlnkb_last]=off;
    vlnkb_lnk[vlnkb_last]=lnk;
    vlnkb_lnks[vlnkb_last]=lnks;
  }else{
    vlnkb_off[vlnkb_free[vlnkb_free_last]]=off;
    vlnkb_lnk[vlnkb_free[vlnkb_free_last]]=lnk;
    vlnkb_lnks[vlnkb_free[vlnkb_free_last--]]=lnks;
  }
}

void VLNKremove(void *lnk){ //remove any vlnk to lnk from other object
  //if (lnk==NULL) MessageBox(NULL,"NULL/removed vlnk","Ultima 6 Online",MB_OK);
  static long i;
  static unsigned long *lp;
  for (i=0;i<=vlnkb_last;i++){
    if (vlnkb_lnk[i]==lnk){
      lp=(unsigned long*)vlnkb_off[i];
      lp[0]=NULL; //NULL vlnk
      vlnkb_lnk[i]=NULL; vlnkb_lnks[i]=NULL; vlnkb_off[i]=NULL; //remove vlnk
      vlnkb_free[++vlnkb_free_last]=i;

    }//=
  }//i
}

void VLNKsremove(void *lnks){ //remove all vlnk using offset within lnks
  static long i;
  for (i=0;i<=vlnkb_last;i++){
    if (vlnkb_lnks[i]==lnks){
      vlnkb_lnk[i]=NULL; vlnkb_lnks[i]=NULL; vlnkb_off[i]=NULL; //remove vlnk
      vlnkb_free[++vlnkb_free_last]=i;
    }//=
  }//i
}

object *OBJnew(){
  if (objb_free_last==-1){

    //if (objb_last>28039) MessageBox(NULL,"OBJnew","Ultima 6 Online",MB_OK);

    //objb_last+=10000;
    ZeroMemory(&objb[++objb_last],sizeof(object));
    return &objb[objb_last];
  }else{
    ZeroMemory(&objb[objb_free[objb_free_last]],sizeof(object));
    return &objb[objb_free[objb_free_last--]];
  }
  return NULL; //OBJnew failed
}


void OBJrelease(object* obj){
  int i=0;
  //all objects attached to primary object must also be released
  objb_free[++objb_free_last]=((unsigned long)obj-(unsigned long)&objb)/sizeof(object);
  //for debugging "double release object bug"
  /*for(i=0;i<objb_free_last-1;i++) {
    if(objb_free[objb_free_last]==objb_free[i]) {
      i=objb_free_last;
    }
  }*/
  //clear object data
  ZeroMemory(obj,sizeof(object));
}

/* free player struct */
void free(player* plr) {
  static unsigned int i;
  for (i=0;i<=9;i++) {
    VLNKremove(plr->ktar[i]); VLNKsremove(plr->ktar[i]);
    OBJremove(plr->ktar[i]); OBJrelease(plr->ktar[i]);
  }
  VLNKremove(plr->talk_target); VLNKsremove(plr->talk_target);
  OBJremove(plr->talk_target); OBJrelease(plr->talk_target);
  free(plr->password);
  free(plr->name);
  free(plr->newpassword);
  if (plr->npcname) { /* this should be always set when the player logs in, but I'll add this here just in case */
    free(plr->npcname);
  }
  free((void*)plr);
}
/* free creature items and creature struct */
void free(creature* crt) {
  static unsigned int x5=0,y6;
  static object *myobj;
  //free inventory items
  myobj=(object*)crt->items; //includes the bag!
  if (myobj) x5=OBJlist(myobj);
  OBJlist_last=NULL;
  for(y6=0;y6<x5;y6++){
    OBJremove(OBJlist_list[y6]); OBJrelease(OBJlist_list[y6]);
  }
  free((void*)crt);
}

/* this should properly clean everything from npc struct and after that free the struct,except the horse, because when removing npc it should be always dismounted*/
void free(npc* tnpc) {
  static unsigned int x5,x2,y6;
  static object *myobj;
  //free inventory items
  x5=0;
  for (x2=0;x2<=8;x2++){
    if (x2==0) myobj=tnpc->helm;
    if (x2==1) myobj=tnpc->wep_right;
    if (x2==2) myobj=tnpc->wep_left;
    if (x2==3) myobj=tnpc->armour;
    if (x2==4) myobj=tnpc->boots;
    if (x2==5) myobj=tnpc->neck;
    if (x2==6) myobj=tnpc->ring_right;
    if (x2==7) myobj=tnpc->ring_left;
    if (x2==8) myobj=(object*)tnpc->items; //includes the bag!
    if (myobj) x5=OBJlist(myobj);
  }
  OBJlist_last=NULL;
  for(y6=0;y6<x5;y6++){
    if ((OBJlist_list[y6]->type&1023)==OBJ_HORSE_PAPERS) { /* if item being relesed is horse paper, remove the horse also */
                myobj=(object*)((creature*)((object*)OBJlist_list[y6]->more)->more)->more;
                OBJremove(myobj); OBJrelease(myobj);
                free((creature*)((object*)OBJlist_list[y6]->more)->more);
                OBJremove((object*)OBJlist_list[y6]->more); OBJrelease((object*)OBJlist_list[y6]->more);
    }
    else if ((OBJlist_list[y6]->type&1023)==OBJ_SHIP_DEED) { /* remove the ship */
      myobj=(object*)OBJlist_list[y6]->more;
      if ((myobj->type&1023)==OBJ_SHIP){ //ship
        static mlobj *mmyobj;
        if (myobj->info&2){ /* If on board the ship */
          tnpc=(npc*)myobj->more;
          mmyobj=(mlobj*)tnpc->player->craft_con;
        }else{
          mmyobj=(mlobj*)myobj->more;
        }
        for (x2=0;x2<=4;x2++){
          OBJremove(mmyobj->obj[x2]); OBJrelease(mmyobj->obj[x2]);
        }
      }
      if (!(myobj->info&2) && myobj->type!=0 && myobj->x!=0&& myobj->y!=0) { //remove the ship main object / skiff only if no one is abroad and the skiff is not already removed
        OBJremove(myobj); OBJrelease(myobj);
      }
    }
    OBJremove(OBJlist_list[y6]); OBJrelease(OBJlist_list[y6]);
  }
  free(tnpc->name);
  free((void*)tnpc);
}

void OBJcheckflags(unsigned long x,unsigned long y){

  //bt[][] unsigned short
  //512 current basetiles exist ->expandable to 1024 using 10 bits
  //6 bit flags reamain
  //1=land passabe
  //2=air passable
  //4=sea passable
  //8=bolt passable
  //16=NPC passable (unlocked doors/containing items that can be picked up)
  //32=view OK (can be seen past)
  OBJcheckflags_flags=btflags[bt[y][x]&1023]; //reset using basetile flags
  OBJcheckflags_flags+=32; //look ok
  if (((bt[y][x]&1023)>=140)&&((bt[y][x]&1023)<188)) OBJcheckflags_flags&=(255-32); //look NOT ok wall
  if (((bt[y][x]&1023)>=240)&&((bt[y][x]&1023)<252)) OBJcheckflags_flags&=(255-32); //look NOT ok cave
  if (((bt[y][x]&1023)>=192)&&((bt[y][x]&1023)<208)) OBJcheckflags_flags&=(255-32); //look NOT ok win


  OBJtmp=od[y][x];
OBJcf1: if (OBJtmp!=NULL){ //object exists
  OBJcheckflags_td=objpassflags[(OBJtmp->type>>10)+sprlnk[OBJtmp->type&1023]];


  if ((OBJcheckflags_td&1)==0) {OBJcheckflags_flags=OBJcheckflags_flags&(255-1-2);} //remove bottom bit from byte value
  //*if item can be picked up include the NPCpassable flag else remove it (todo)
  //skiff and ship can go on top of chests and probably some other items too needs fixing.
  if (OBJcheckflags_td&2) {OBJcheckflags_flags=OBJcheckflags_flags|1|2;
  if (OBJcheckflags_flags&4) OBJcheckflags_flags-=4;
  } //override



  if (OBJcheckflags_td&4) {
    OBJcheckflags_flags=OBJcheckflags_flags&(255-8);
    //if a bolt can't pass through it may affect our vision too
    //is it a door?
    if (((OBJtmp->type&1023)>=297)&&((OBJtmp->type&1023)<=300)) OBJcheckflags_flags&=(255-32);

    if ((OBJtmp->type&1023)==334){
      if ((OBJtmp->type&1024)==0) OBJcheckflags_flags&=(255-32); //secret door
    }
    if ((OBJtmp->type&1023)==213) OBJcheckflags_flags&=(255-32); //mousehole


  } //remove bolt passable flag

  //make back of horse passable
  //if ((OBJtmp->type&1023)==431){
  //if ((OBJtmp->type>>10)>=8){
  //OBJcheckflags_flags|=16;
  //}
  //}

  if (OBJcheckflags_td&8){ //NPC passable
    OBJcheckflags_flags|=16;
  }

  if (OBJtmp->info&(4+2)){ //crt or npc
    if (OBJcheckflags_flags&4){ //sea passable
      OBJcheckflags_flags-=4;
    }
    if (OBJcheckflags_flags&2){ //air passable
      OBJcheckflags_flags-=2;
    }
  }




  OBJtmp=(object*)OBJtmp->next; goto OBJcf1;
        }

        bt[y][x]=(bt[y][x]&1023)+OBJcheckflags_flags*1024;
}

unsigned char OBJadd(unsigned long x,unsigned long y,object* obj){
  if (od[y][x]==NULL){
    od[y][x]=obj;
    obj->prev=NULL;
    obj->next=NULL;
    obj->x=x;
    obj->y=y;
  }else{

    OBJtmp=od[y][x];
OBJadd_j1:

    //if ((objfloatflags[(OBJtmp->type>>10)+sprlnk[OBJtmp->type&1023]])&&((objfloatflags[(obj->type>>10)+sprlnk[obj->type&1023]] )==0)){
    if (objfloatflags[(OBJtmp->type>>10)+sprlnk[OBJtmp->type&1023]]){

      OBJtmp2=(object*)OBJtmp->prev;
      if (OBJtmp2!=NULL){
        OBJtmp2->next=obj;
        obj->prev=OBJtmp2;
      }else{
        od[y][x]=obj;
        obj->prev=NULL;
      }
      obj->next=OBJtmp;
      OBJtmp->prev=obj;
      obj->x=x;
      obj->y=y;
      goto OBJaddflt1;
    }
    if (OBJtmp->next!=NULL){
      OBJtmp=(object*)OBJtmp->next;
      goto OBJadd_j1;
    }

    OBJtmp->next=obj;
    obj->prev=OBJtmp;
    obj->next=NULL;
    obj->x=x;
    obj->y=y;
  }

OBJaddflt1:
  OBJcheckflags(x,y);
  return 0; //success
}



void OBJaddtocontainer(object *container,object* objecttoadd){
  if (obji[sprlnk[objecttoadd->type&1023]].weight==NULL){
    if (U6O_DEBUG){
      static txt *errortext=txtnew();
      MessageBox(NULL,"ERROR CORRECTION: NULL WEIGHT ITEM TO PUT INTO CONTAINER PLACED ABOVE CONTAINER!","Ultima 6 Online",MB_OK);
      txtnumint(errortext,container->x); txtadd(errortext,"<-X CONTAINER LOCATION"); MessageBox(NULL,errortext->d,"Ultima 6 Online",MB_OK);
      txtnumint(errortext,container->y); txtadd(errortext,"<-Y CONTAINER LOCATION"); MessageBox(NULL,errortext->d,"Ultima 6 Online",MB_OK);
    }
    OBJadd(container->x,container->y,objecttoadd);
    return;
  }//weight==0
  OBJaddtocontainer_containermore=(object*)container->more;
  container->more=objecttoadd;
  objecttoadd->next=OBJaddtocontainer_containermore;
  objecttoadd->prev=container;
  if (OBJaddtocontainer_containermore) OBJaddtocontainer_containermore->prev=objecttoadd;
}

void BTset(long x,long y,unsigned short i){ 
  static unsigned short p;
  x+=patchx; y+=patchy;
  bt[y][x]=i;
  if (NEThost){
    OBJcheckflags(x,y);
    if (housenumber!=65535){
      if ((housex1[housenumber]==0)||(housex1[housenumber]>x)) housex1[housenumber]=x;
      if ((housex2[housenumber]==0)||(housex2[housenumber]<x)) housex2[housenumber]=x;
      if ((housey1[housenumber]==0)||(housey1[housenumber]>y)) housey1[housenumber]=y;
      if ((housey2[housenumber]==0)||(housey2[housenumber]<y)) housey2[housenumber]=y;
      for (p=0;p<housepnext[housenumber];p++){
        if ((x==housepx[housenumber][p])&&(y==housepy[housenumber][p])) return;
      }//p
      p=housepnext[housenumber]; housepx[housenumber][p]=x; housepy[housenumber][p]=y;
      housepnext[housenumber]++;
    }//housenumber!=65535
  }//NEThost
}
/* luteijn: away with thee! 
void encrypt(txt *t4){
static txt *t2=txtnew();
static long x,x2,x3,x4,i3;
//ENCRYPT3.0
x4=rnd*65536; txtset(t2,"????"); t2->df[0]=(float)x4; //code offset
x3=0; //prev UNENCRYPTED value
for(i3=2;i3<t4->l;i3++){ //skip version (first 2 bytes)
x2=t4->d2[i3]; //=original value
x2=x2^0xFF; //bitwise NOT
x2+=x3; //add prev UNENCRYPTED value
if (x2>=256) x2-=256; //"loop" byte value
x2+=encryptcode[x4]; x4=(x4+1)&65535;
if (x2>=256) x2-=256; //"loop" byte value
x3=t4->d2[i3]; //set prev UNENCRYPTED value
t4->d2[i3]=x2; //set ENCRYPTED value
}
txtadd(t4,t2); //add encrypt code starting offset (float)
}
*/

void decrypt(txt *t4){
  static txt *t5=txtnew();
  static long x,x2,x3,x4,i3;
  static float f;
  //DECRYPT3.0
  txtset(t5,"????");
  t5->d2[0]=t4->d2[t4->l-4]; t5->d2[1]=t4->d2[t4->l-3]; t5->d2[2]=t4->d2[t4->l-2]; t5->d2[3]=t4->d2[t4->l-1];
  f=t5->df[0]; x4=f;
  x3=0; //prev UNENCRYPTED value
  for(i3=2;i3<(t4->l-4);i3++){ //skip version (first 2 bytes)
    x2=t4->d2[i3]; //=encrypted value
    x2-=encryptcode[x4]; x4=(x4+1)&65535;
    if (x2<0) x2+=256; //"loop" byte value
    x2-=x3; //- prev UNENCRYPTED value
    if (x2<0) x2+=256; //"loop" byte value
    x2=x2^0xFF; //bitwise NOT
    x3=x2; //set prev UNENCRYPTED value
    t4->d2[i3]=x2; //set UNENCRYPTED value
  }//i3
}

void addu6monsterdropitems(object *crtobj){
  static creature *crt;
  static unsigned char wep[32];
  static unsigned char arm[32];
  static unsigned short inv[32]; //changed to short, to allow objects after 255 being added to drops
  static unsigned char chest[32];
  static long x1,x2,x3,x5,x8,y8,x7,y7;

  static unsigned short type;
  static unsigned char wepn,armn,invn,chestn;
  type=crtobj->type&1023;
  crt=(creature*)crtobj->more;
  wepn=0; armn=0; invn=0;

  if (type==384){//BEGGAR/PEASANT
    wep[wepn++]=38;
    wep[wepn++]=34;
    arm[armn++]=185;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==428){//COW
    inv[invn++]=129;
  }

  if (type==424){//CYCLOPS
    wep[wepn++]=44;
    wep[wepn++]=33;
    wep[wepn++]=34;
    arm[armn++]=20;
    arm[armn++]=3;
    arm[armn++]=13;
    inv[invn++]=98;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==350){//DEER
    inv[invn++]=129;
  }

  if (type==411){//DRAGON
    inv[invn++]=98;
    inv[invn++]=88;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==369){//DRAKE
    inv[invn++]=88;
    inv[invn++]=88;
  }

  if (type==387){//WOMAN/DRESS-WEARER
    wep[wepn++]=38;
    arm[armn++]=185;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==385){//FARMER
    wep[wepn++]=105;
    wep[wepn++]=101;
    arm[armn++]=17;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==376){//FIGHTER
    wep[wepn++]=43;
    wep[wepn++]=42;
    arm[armn++]=21;
    arm[armn++]=10;
    arm[armn++]=3;
    arm[armn++]=22;
    inv[invn++]=88;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==362){//WINGED gargoyle
    wep[wepn++]=46;
    wep[wepn++]=41;
    wep[wepn++]=47;
    wep[wepn++]=49;
    arm[armn++]=18;
    arm[armn++]=1;
    inv[invn++]=58;
    inv[invn++]=129;
	inv[invn++]=57;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==363){//gargoyle (WINGLESS)
    wep[wepn++]=34;
    wep[wepn++]=49;
    wep[wepn++]=37;
    arm[armn++]=1;
    arm[armn++]=9;
    arm[armn++]=18;
    arm[armn++]=20;
  }

  if (type==382){//GUARD
    wep[wepn++]=42;
    wep[wepn++]=47;
    arm[armn++]=22;
    arm[armn++]=10;
    arm[armn++]=3;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==370){//HEADLESS
    wep[wepn++]=34;
    wep[wepn++]=36;
    arm[armn++]=17;
    arm[armn++]=9;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==383){//JESTER
    wep[wepn++]=38;
    wep[wepn++]=33;
    arm[armn++]=17;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }


  if (type==386){//LUTE-PLAYER/MUSCIAN
    wep[wepn++]=41;
    wep[wepn++]=33;
    wep[wepn++]=43;
    arm[armn++]=19;
    arm[armn++]=1;
    arm[armn++]=9;
    inv[invn++]=88;
    inv[invn++]=158;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==378){//MAGE
    wep[wepn++]=38;
    arm[armn++]=17;
    arm[armn++]=1;
    inv[invn++]=58;
    inv[invn++]=98;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==380){//MERCHANT
    wep[wepn++]=38;
    wep[wepn++]=43;
    arm[armn++]=17;
    arm[armn++]=1;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==372){//MONGBAT
    wep[wepn++]=38;
    wep[wepn++]=43;
    wep[wepn++]=36;
    wep[wepn++]=37;
    arm[armn++]=9;
    arm[armn++]=17;
    arm[armn++]=18;
    inv[invn++]=98;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==347){//REAPER
    inv[invn++]=58;
    inv[invn++]=88;
  }

  if (type==348){//SHEEP
    inv[invn++]=129;
  }

  if (type==368){//SKELETON
    wep[wepn++]=43;
    wep[wepn++]=36;
    wep[wepn++]=37;
    wep[wepn++]=41;
    wep[wepn++]=38;
    arm[armn++]=9;
    arm[armn++]=1;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==361){//SPIDER
    inv[invn++]=129;
  }

  if (type==377){//SWASHBUCKLER
    wep[wepn++]=43;
    wep[wepn++]=41;
    wep[wepn++]=33;
    arm[armn++]=19;
    arm[armn++]=1;
    arm[armn++]=9;
    inv[invn++]=88;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==371){//TROLL
    wep[wepn++]=37;
    wep[wepn++]=36;
    wep[wepn++]=43;
    wep[wepn++]=41;
    wep[wepn++]=34;
    arm[armn++]=9;
    arm[armn++]=18;
    arm[armn++]=1;
    arm[armn++]=19;
    inv[invn++]=98;
    inv[invn++]=88;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  if (type==379){//VILLAGER
    wep[wepn++]=43;
    wep[wepn++]=33;
    arm[armn++]=17;
    inv[invn++]=88;
    inv[invn++]=OBJ_TMAP;
  }

  /*
  if (type==){//
  wep[wepn++]=0;
  arm[armn++]=0;
  inv[invn++]=0;
  }
  [GREMLIN] (CHECK THIS DATA BEFORE IMPLEMENTING)
  198264  0 nothing
  198265  0 nothing
  198266  128 bread
  198267  99 backpack
  198268  0 nothing
  198269  0 nothing
  198270  0 nothing
  198271  0 nothing
  198272  0 nothing
  [MIMIC BOX] (NOT IMPLEMENTED)
  198349  0 nothing
  198350  0 nothing
  198351  98 chest
  198352  98 chest
  198353  0 nothing
  198354  0 nothing
  */
  static object *invbag,*invobj;
  static long i,i2,n,item,chance,chanceresult,addmany,chest_restorei2;


  invbag=crt->items;

  for (i=0;i<=2;i++){
    i2=0;
    if (i==0) n=wepn; if (i==1) n=armn; if (i==2) n=invn;
whilei2:
    while (i2<n){
      if (i==0) item=wep[i2]; if (i==1) item=arm[i2]; if (i==2) item=inv[i2]; if (i==3) item=chest[i2];

      //add given item to crt's inventory
      chance=4;
      if (i2>=1) chance=8;
      if (i2>=3) chance=16;

      if (i==3){
        chance=2;
        if (i2>=1) chance=2;
        if (i2>=3) chance=16;
      }

      if (item==58) chance=16;//spell (actually a random potion) RARE

      if (item==OBJ_TMAP) chance=128;//for debugging set to 2, will set to 128 or something like that later.


      chanceresult=rnd*chance;
      if (chanceresult==0){

        addmany=0;
        if (item==36){chanceresult=rnd*5; chanceresult++; addmany=chanceresult;}//spear
        if (item==37){chanceresult=rnd*5; chanceresult++; addmany=chanceresult;}//axe
        if (item==38){chanceresult=rnd*5; chanceresult++; addmany=chanceresult;}//dagger
addmany_loop:

        invobj=OBJnew(); invobj->type=item;

        if (item==88){
          //gold modification
          //2/3 times it's in the first half of the range
          //1/3 times it's in the top half of the range
          chanceresult=rnd*3;
          //gold(quantity 10-60)
          invobj->more2=unsigned long(rnd*26)+10;
          if (chanceresult==2) invobj->more2=unsigned long(rnd*26)+35;
          //CHEST ITEM: gold (quantity 10-100)
          if (i==3){
            invobj->more2=unsigned long(rnd*46)+10;
            if (chanceresult==2) invobj->more2=unsigned long(rnd*46)+55;
          }
          //old code
          //invobj->more2=unsigned long(rnd*51)+10;//gold (quantity 10-60)
          //if (i==3) invobj->more2=unsigned long(rnd*91)+10;//CHEST ITEM: gold (quantity 10-100)
        }//item==88



        if (item==58){//spell (actually a random potion)
          chanceresult=rnd*8;
          invobj->type=275+chanceresult*1024;
        }

        if (item==90){//torch(es)
          //CHEST ITEM
          invobj->more2=unsigned long(rnd*3)+1;
        }

        if (item==77){//gem
          //CHEST ITEM
          invobj->more2=1;
        }

        if (item==OBJ_TMAP) {
          x7=randomchestlocation(true);
          y7=x7>>10;
          x7=x7&1023;
          x1=rnd*7;
          x2=rnd*7;
          while (((bt[y7+(x2-3)*2][x7+(x1-3)*2]&1023)>=8)&&((bt[y7+(x2-3)*2][x7+(x1-3)*2]&1023)<16)) { //check that the map marker is not in the sea.
            x1=rnd*7;
            x2=rnd*7;
          }
          x3=rnd*7;
          x5=rnd*7;
          invobj->more2=x7+1024*y7+1024*1024*x1+1024*1024*8*x2+1024*1024*8*8*x3+1024*1024*8*8*8*x5;

        }
        if (item==129){
          if ((i==3)||(type==362)){//CHEST ITEM/WINGED GARG SPECIAL ITEM: random food drop

            chanceresult=rnd*3;
            if (chanceresult!=2){
              if (chanceresult==1) invobj->type=128;//bread
              invobj->more2=unsigned long(rnd*5)+1;
            }else{
              invobj->type=133;//ham
              invobj->more2=0;
            }

          }else{
            invobj->more2=unsigned long(rnd*3)+1;
          }
        }

        //WHAT ABOUT THE CURRENT OBJEST IN THE BAGS PREV POINTER!!!!
        invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;


		// t222
        ENHANCEnewn(invobj,8,4);

        if (item==42){//xbow
          chanceresult=rnd*25; chanceresult++;
          invobj=OBJnew(); invobj->type=56; invobj->more2=chanceresult;
          invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;
        }
        if (item==41){//bow
          chanceresult=rnd*25; chanceresult++;
          invobj=OBJnew(); invobj->type=55; invobj->more2=chanceresult;
          invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;
        }


        //chest?
        if (item==98){
          invobj->type=98+1024;
          //IDEA: add lock or magical lock ;) depending on INT of crt
          chestn=0;
          chest[chestn++]=88;//gold
          chest[chestn++]=90;//torch
          chest[chestn++]=129;//meat
          chest[chestn++]=77;//gem
          chest_restorei2=i2+1;
          n=chestn;
          i=3;
          i2=0;
          invbag=invobj;
          goto whilei2;
        }


        if (addmany){addmany--; goto addmany_loop;}




      }//chanceresult==0
      i2++;
    }

    if (i==3){
      invbag=crt->items;
      n=invn;
      i=2;
      i2=chest_restorei2;
      goto whilei2;
    }


  }//i



  return;
}


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
unsigned char movernew(unsigned short type,unsigned short x,unsigned short y,unsigned long flags){
  static long x2,y2,x3,y3,z,z2,z3;
  static object *myobj3,*myobj;
  static creature *crt,*crt2;
  static unsigned char allegiance;
  static object *invbag;
  static object *invobj;
  flags|=MOVERNEW_GLOBALFLAGS;


  MOVERNEW_ERROR=0;
  if (!tclass_mover[type]){
    MOVERNEW_ERROR=1;
    if (obji[sprlnk[type&1023]].v4){
      MOVERNEW_ERROR=2;
    }
  }
  if ((type&1023)==423) MOVERNEW_ERROR=2;
  if ((type&1023)==412) MOVERNEW_ERROR=2;
  if ((type&1023)==414) MOVERNEW_ERROR=2;
  if ((type&1023)==415) MOVERNEW_ERROR=2;
  if ((type&1023)==423) MOVERNEW_ERROR=2;
  if ((type&1023)==366) MOVERNEW_ERROR=2;
  if (MOVERNEW_ERROR) return 0;




  MOVERNEW_OBJECT=OBJnew();
  MOVERNEW_OBJECT->type=type;
  OBJadd(x,y,MOVERNEW_OBJECT);



  MOVERNEW_OBJECT->more=malloc(sizeof(creature));
  ZeroMemory(MOVERNEW_OBJECT->more,sizeof(creature));

  crt=(creature*)MOVERNEW_OBJECT->more;
  crt->crt_struct=TRUE;
  MOVERNEW_OBJECT->info|=4; //<-crt

  crt->hp=(obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v8>>8)*4;
  crt->mp=rnd*9;

  allegiance=1;
  //friendly
  if ((MOVERNEW_OBJECT->type&1023)==382){ //guard
    if (x<1024) allegiance=4; //not underground
  }
  //passive
  if ((MOVERNEW_OBJECT->type&1023)==373) allegiance=0; //wisp
  if ((MOVERNEW_OBJECT->type&1023)==430) allegiance=0; //horse
  if ((MOVERNEW_OBJECT->type&1023)==428) allegiance=0; //cow
  if ((MOVERNEW_OBJECT->type&1023)==356) allegiance=0; //bird
  if ((MOVERNEW_OBJECT->type&1023)==348) allegiance=0; //sheep
  if ((MOVERNEW_OBJECT->type&1023)==350) allegiance=0; //deer
  if ((MOVERNEW_OBJECT->type&1023)==388){ //cat
    if ((x>=400)&&(y>=576)&&(x<=415)&&(y<=591)) allegiance=0; //Dr Cat's cats
  }
  if ((MOVERNEW_OBJECT->type&1023)==352){ //ghost
    if ((x==139)&&(y==196)) allegiance=0; //abby ghosts
  }

  if (flags&MOVERNEW_FLAG_PASSIVE) allegiance=0;
  if (flags&MOVERNEW_FLAG_FRIENDLY) allegiance=4;
  if (flags&MOVERNEW_FLAG_AGRESSIVE) allegiance=1;
  crt->al=allegiance;

  invbag=OBJnew(); invbag->type=188; crt->items=invbag;//inventory bag(not shown)


  if (crt->al==1){//aggressive


    //x2=obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v5>>2;
    //x2=rnd*(x2+1);
    //if (x2){
    //invobj=OBJnew(); invobj->type=88; invobj->more2=x2;//add x2 gold coins
    //XXXXXXXXBAD CODEXXXXXXXXXXXX *next_pointer=(unsigned long)invobj; next_pointer=(unsigned long*)&invbag->next; invobj->prev=invobj_prev; invobj_prev=invobj;
    //}//x2
    addu6monsterdropitems(MOVERNEW_OBJECT);

    //integrate XP based gold
    x2=rnd*8; if (x2==0){
      x2=obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v5; x2=rnd*(x2+1); if (x2){
        if (myobj=(object*)invbag->more){
movernew_goldscan:
          if (myobj->type==88){
            myobj->more2+=x2;
            goto movernew_goldadded;
          }
          myobj=(object*)myobj->next; if (myobj) goto movernew_goldscan;
        }
        invobj=OBJnew(); invobj->type=88; invobj->more2=x2;
        invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;
      }//x2=obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v5; x2=rnd*(x2+1); if (x2==0){
    }//x2=rnd*8; if (x2==0){
movernew_goldadded:

    goto movernew_u6inventoryadded;
  }//al==1

  if (((MOVERNEW_OBJECT->type&1023)==428)||((MOVERNEW_OBJECT->type&1023)==348)||((MOVERNEW_OBJECT->type&1023)==350)){//cow/sheep/deer
    x2=rnd*2; if (x2){
      invobj=OBJnew(); invobj->type=129; invobj->more2=1;//add meat
      invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;
    }//x2
  }//cow/sheep/deer

movernew_u6inventoryadded:

  if (obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v4==8){ //2-part creature
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT;
    myobj3->type=MOVERNEW_OBJECT->type+8*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y+1,myobj3);
    crt->more=myobj3;
    //Sutek's creatures' hp increase
    z=0;
    if ((x>=747)&&(x<=829)&&(y>=905)&&(y<=983)) z=1; //on island
    if ((x>=792)&&(x<=796)&&(y>=937)&&(y<=938)) z=0; //in cage
    if (z) crt->hp*=4;
  }

  if (obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v4==5){ //cyclops/winged gargoyle
    static mlobj *mmyobj; //array size varies
    mmyobj=(mlobj*)malloc(sizeof(object*)*3);
    crt->more=mmyobj;
    MOVERNEW_OBJECT->type=(MOVERNEW_OBJECT->type&1023)+3*1024;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[0]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type-3*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[1]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type-2*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[2]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type-1*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y,myobj3);
  }

  if (obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v4==11){ //hydra
    static mlobj *mmyobj; //array size varies
    mmyobj=(mlobj*)malloc(sizeof(object*)*8);
    crt->more=mmyobj;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[0]=myobj3;
    myobj3->type=425+28*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[1]=myobj3;
    myobj3->type=425+0*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[2]=myobj3;
    myobj3->type=425+4*1024; myobj3->info|=8; //<-base object
    OBJadd(x+1,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[3]=myobj3;
    myobj3->type=425+8*1024; myobj3->info|=8; //<-base object
    OBJadd(x+1,y,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[4]=myobj3;
    myobj3->type=425+12*1024; myobj3->info|=8; //<-base object
    OBJadd(x+1,y+1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[5]=myobj3;
    myobj3->type=425+16*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y+1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[6]=myobj3;
    myobj3->type=425+20*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y+1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[7]=myobj3;
    myobj3->type=425+24*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y,myobj3);

  }


  if (obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v4==10){ //silver serpent
    static mlobj *mmyobj; //array size varies
    mmyobj=(mlobj*)malloc(sizeof(object*)*7);
    crt->more=mmyobj;
    MOVERNEW_OBJECT->type=(MOVERNEW_OBJECT->type&1023)+4*1024;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[0]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+8*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[1]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+7*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[2]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+6*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[3]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+9*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[4]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+8*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[5]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+7*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[6]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type-3*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y,myobj3);
  }

  if (obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v4==9){ //dragon
    static mlobj *mmyobj; //array size varies
    mmyobj=(mlobj*)malloc(sizeof(object*)*4);
    crt->more=mmyobj;
    MOVERNEW_OBJECT->type=411; //(MOVERNEW_OBJECT->type&1023)+3*1024;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[0]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+8*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[1]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+16*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y+1,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[2]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+24*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y,myobj3);
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[3]=myobj3;
    myobj3->type=MOVERNEW_OBJECT->type+32*1024; myobj3->info|=8; //<-base object
    OBJadd(x+1,y,myobj3);
  }

  if (obji[sprlnk[MOVERNEW_OBJECT->type&1023]].v4==13){ //tangle vine
    static mlobj *mmyobj; //array size varies
    mmyobj=(mlobj*)malloc(sizeof(object*)*4);
    crt->more=mmyobj;
    MOVERNEW_OBJECT->type=365; //(MOVERNEW_OBJECT->type&1023)+3*1024;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[0]=myobj3;
    myobj3->type=366+1*1024; //myobj3->info|=8; //<-base object
    OBJadd(MOVERNEW_OBJECT->x,MOVERNEW_OBJECT->y-1,myobj3);
    myobj3->more=malloc(sizeof(creature));
    ZeroMemory(myobj3->more,sizeof(creature));
    crt2=(creature*)myobj3->more;
    crt2->crt_struct=TRUE;
    crt2->al=crt->al;
    crt2->hp=32767; //maximum allowed
    crt2->mp=0;
    crt2->more=MOVERNEW_OBJECT;
    invbag=OBJnew(); invbag->type=188; crt2->items=invbag;//inventory bag(not shown)

    myobj3->info|=4; //<-crt
    crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[1]=myobj3;
    myobj3->type=366+1*1024; //myobj3->info|=8; //<-base object
    OBJadd(MOVERNEW_OBJECT->x,MOVERNEW_OBJECT->y+1,myobj3);
    myobj3->more=malloc(sizeof(creature));
    ZeroMemory(myobj3->more,sizeof(creature));
    crt2=(creature*)myobj3->more;
    crt2->crt_struct=TRUE;
    crt2->al=crt->al;
    crt2->hp=32767; //maximum allowed
    crt2->mp=0;
    crt2->more=MOVERNEW_OBJECT;
    invbag=OBJnew(); invbag->type=188; crt2->items=invbag;//inventory bag(not shown)

    myobj3->info|=4; //<-crt
    crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[2]=myobj3;
    myobj3->type=366; //myobj3->info|=8; //<-base object
    OBJadd(MOVERNEW_OBJECT->x-1,MOVERNEW_OBJECT->y,myobj3);
    myobj3->more=malloc(sizeof(creature));
    ZeroMemory(myobj3->more,sizeof(creature));
    crt2=(creature*)myobj3->more;
    crt2->crt_struct=TRUE;
    crt2->al=crt->al;
    crt2->hp=32767; //maximum allowed
    crt2->mp=0;
    crt2->more=MOVERNEW_OBJECT;
    invbag=OBJnew(); invbag->type=188; crt2->items=invbag;//inventory bag(not shown)

    myobj3->info|=4; //<-crt
    crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;
    myobj3=OBJnew(); myobj3->more=MOVERNEW_OBJECT; mmyobj->obj[3]=myobj3;
    myobj3->type=366; //myobj3->info|=8; //<-base object
    OBJadd(MOVERNEW_OBJECT->x+1,MOVERNEW_OBJECT->y,myobj3);
    myobj3->more=malloc(sizeof(creature));
    ZeroMemory(myobj3->more,sizeof(creature));
    crt2=(creature*)myobj3->more;
    crt2->crt_struct=TRUE;
    crt2->al=crt->al;
    crt2->hp=32767; //maximum allowed
    crt2->mp=0;
    crt2->more=MOVERNEW_OBJECT;
    invbag=OBJnew(); invbag->type=188; crt2->items=invbag;//inventory bag(not shown)

    myobj3->info|=4; //<-crt
    crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;
  }//tangle vine

  //slime
  if ((MOVERNEW_OBJECT->type&1023)==375) MOVERNEW_OBJECT->info|=(3<<9);//slime divide value

  //respawn info
  crt->respawn_hp=crt->hp; crt->respawn_x=MOVERNEW_OBJECT->x; crt->respawn_y=MOVERNEW_OBJECT->y;
  return 1;
}

object* OBJaddnew(unsigned long x,unsigned long y,unsigned short type,unsigned short info,unsigned long more2){
  //call movernew()
  if (movernew(type,x,y,NULL)) return MOVERNEW_OBJECT;//mover was created successfully!
  if (MOVERNEW_ERROR==2) return NULL;//IGNORED (object is not the primary part of a mover)

  //check for multiple quantity items with NULL quantity and set to 1
  if (obji[sprlnk[type&1023]+(type>>10)].flags&4096){//multiple quantity
    if (more2==NULL){
      //if (U6O_DEBUG){
      if (0){
        static txt *errortext=txtnew();
        MessageBox(NULL,"ERROR CORRECTION: MULTIPLE QUANTITY ITEM WITH NULL QUANTITY SET TO 1","Ultima 6 Online",MB_OK);
        txtnumint(errortext,x); txtadd(errortext,"<-X LOCATION"); MessageBox(NULL,errortext->d,"Ultima 6 Online",MB_OK);
        txtnumint(errortext,y); txtadd(errortext,"<-Y LOCATION"); MessageBox(NULL,errortext->d,"Ultima 6 Online",MB_OK);
      }
      more2=1;
    }
  }

  static unsigned short p;
  static object *obj; obj=OBJnew(); obj->type=type; obj->info=info; obj->more2=more2;
  x+=patchx; y+=patchy;
  if (od[y][x]==NULL){
    od[y][x]=obj;
    obj->prev=NULL;
    obj->next=NULL;
    obj->x=x;
    obj->y=y;
  }else{
    OBJtmp=od[y][x];
OBJadd_j1new:

    //if ((objfloatflags[(OBJtmp->type>>10)+sprlnk[OBJtmp->type&1023]])&&((objfloatflags[(obj->type>>10)+sprlnk[obj->type&1023]] )==0)){
    if (objfloatflags[(OBJtmp->type>>10)+sprlnk[OBJtmp->type&1023]]){

      OBJtmp2=(object*)OBJtmp->prev;
      if (OBJtmp2!=NULL){

        OBJtmp2->next=obj;
        obj->prev=OBJtmp2;

      }else{
        od[y][x]=obj;
        obj->prev=NULL;
      }
      obj->next=OBJtmp;
      OBJtmp->prev=obj;
      obj->x=x;
      obj->y=y;
      goto OBJaddflt1new;
    }
    if (OBJtmp->next!=NULL){
      OBJtmp=(object*)OBJtmp->next;
      goto OBJadd_j1new;
    }

    OBJtmp->next=obj;
    obj->prev=OBJtmp;
    obj->x=x;
    obj->y=y;
  }
OBJaddflt1new:
  OBJcheckflags(x,y);

  if (housenumber!=65535){
    if ((housex1[housenumber]==0)||(housex1[housenumber]>x)) housex1[housenumber]=x;
    if ((housex2[housenumber]==0)||(housex2[housenumber]<x)) housex2[housenumber]=x;
    if ((housey1[housenumber]==0)||(housey1[housenumber]>y)) housey1[housenumber]=y;
    if ((housey2[housenumber]==0)||(housey2[housenumber]<y)) housey2[housenumber]=y;
    for (p=0;p<housepnext[housenumber];p++){
      if ((x==housepx[housenumber][p])&&(y==housepy[housenumber][p])) return obj;
    }//p
    p=housepnext[housenumber]; housepx[housenumber][p]=x; housepy[housenumber][p]=y;
    housepnext[housenumber]++;
  }//housenumber!=65535

  return obj;
}


void OBJremove(object* obj){ //OBJremove detects container objects
  OBJtmp=(object*)obj->prev;
  OBJtmp2=(object*)obj->next;
  if (OBJtmp==NULL){ //if no object preceeds obj
    if (OBJtmp2==NULL){ //if no object is after obj
      od[obj->y][obj->x]=NULL;
      //type 13743 (horse rear)
      //info 49
      //x 6400
      //y 47361
      //more 1
      //next 0
      //prev 0
      //called from objmove2 in winmain
      //is this a result of not removing rear end of horse previously??!!??!!??


    }else{ //an object is after obj
      od[obj->y][obj->x]=OBJtmp2;
      OBJtmp2->prev=NULL;
    }
  }else{ //an object is before obj
    if (OBJtmp2==NULL){ //if no object is after obj
      //if the previous object is NOT a container carrying obj
      if (OBJtmp->next==obj){OBJtmp->next=NULL;}else{OBJtmp->more=NULL;}
    }else{ //an object is after obj
      //if the previous object is NOT a container carrying obj
      if (OBJtmp->next==obj){OBJtmp->next=OBJtmp2;}else{OBJtmp->more=OBJtmp2;}
      OBJtmp2->prev=OBJtmp; //fix the prev-link of the next object
    }
  }
  OBJcheckflags(obj->x,obj->y); //check flags after obj was removed
  //clear any link information and position information
  obj->next=NULL; obj->prev=NULL;
  obj->x=0; obj->y=0;
}

unsigned char OBJmove(object* obj,unsigned long x,unsigned long y){
  if ((x<0)||(x>2047)||(y<0)||(y>1023)) return 1;

  if (OBJmove_allow) goto moveallow;


  if ((bt[y][x]&1024)||(keyon[VK_SPACE]&&U6O_DEBUG)){




    //if ((bt[y][x]&1024)||(1)){
moveallow:
    OBJremove(obj);
    OBJadd(x,y,obj);
    return 0;
  } //walking living passable
  return 1;
}

object* OBJfindlast(unsigned long x,unsigned long y){
  //finds the last object (*not including floating objects)
  //used for droping items
  if ((x<0)||(y<0)) return NULL;
  if ((x>=2048)||(y>=1024)) return NULL;


  OBJtmp=od[y][x];
  if (OBJtmp!=NULL){
OBJfindlast_next:
    if (OBJtmp->next!=NULL){
      OBJtmp2=(object*)OBJtmp->next;

      if ((objfloatflags[(OBJtmp2->type>>10)+sprlnk[OBJtmp2->type&1023]]&1)==0){
        OBJtmp=(object*)OBJtmp->next;
        goto OBJfindlast_next;
      }
    }
    if ((objfloatflags[(OBJtmp->type>>10)+sprlnk[OBJtmp->type&1023]]&1)!=0) OBJtmp=NULL;
  }
  return OBJtmp;
}

object* OBJfindlastall(unsigned long x,unsigned long y){
  //finds the last object (including floating objects)
  //used for dropping items
  if ((x<0)||(y<0)) return NULL;
  if ((x>=2048)||(y>=1024)) return NULL;
  OBJtmp=od[y][x];
  if (OBJtmp!=NULL){
OBJfindlast_next:
    if (OBJtmp->next!=NULL){
      OBJtmp=(object*)OBJtmp->next;
      goto OBJfindlast_next;
    }
  }
  return OBJtmp;
}

//OBJlist creates list of all internal&next objects, returns number of objects (0=none)
//NOTE: OBJlist creates a global list, user sets to NULL
unsigned long OBJlist(object* obj){

  static object *l[65536]; //link
  static long l_last,l_done;
  static object *myobj;
  if (obj==NULL) return OBJlist_last;
  l_last=-1; l_done=-1; myobj=obj;
OBJlist_nextobject:
  OBJlist_list[OBJlist_last]=myobj; //add object to list
  OBJlist_last++;
  if (myobj->more){ //container
    if (obji[sprlnk[myobj->type&1023]].flags&1024){
      l_last++; l[l_last]=(object*)myobj->more; //add unchecked container
    }}
  if (myobj->next){
    if ( (myobj->next!=myobj->more) || ((obji[sprlnk[myobj->type&1023]].flags&1024)==0) ){ //avoid reverse linked inventory containers
      myobj=(object*)myobj->next; goto OBJlist_nextobject; //get next object
    }}
  if (l_done<l_last){
    l_done++;
    myobj=l[l_done]; goto OBJlist_nextobject; //check next container
  }
  return OBJlist_last;
}


//OBJmove2 !2003: moves a creature/NPC and adjusts picture sub-index
//x,y are it's new position

unsigned char OBJmove2(object* obj,unsigned long x,unsigned long y){

  if ((x<0)||(x>2047)||(y<0)||(y>1023)) return 1;

  static unsigned long xx,xx2,xx3,xx4,xx5,ox,oy,type,pass;
  static long tx,ty,dx,dy,i;
  static object *m2obj,*obj2,*obj3,*obj4;
  static creature *tcrt;
  static npc *tnpc,*tnpc3;
  static player *tplayer;

  if (OBJmove_allow) goto move2allow;

  pass=0;
  type=obj->type&1023;
  if (type==345) pass=2; //squid
  if (type==346) pass=2; //sea serpent
  if (type==343) pass=3; //insects
  if (type==344) pass=3; //bat
  if (type==356) pass=3; //bird
  if (type==362) pass=3; //winged gargoyle
  if (type==369) pass=3; //drake
  if (type==372) pass=3; //mongbat
  if (type==373) pass=3; //wisp
  if (type==411) pass=3; //dragon

  if (type==366){ //tangle vine tendril!
    if (od[y][x]) return 1;
  }

  if (type==352){ //ghost (unlimited!)
    if (obj2=OBJfindlast(x,y)){ //can't move on top of other creature/player!
      if (obj2->info&(2+4)) return 1;
    }
    goto move2allow;
  }

  if (type==354){ //mouse
    if ((bt[y][x]&1024)==0){ //blocked!
      if (obj2=OBJfindlastall(x,y)){
        if ((obj2->type&1023)==213) goto move2allow; //mousehole
      }
      if (obj2=OBJfindlastall(x,y)){
        if (obj2=(object*)obj2->prev){
          if (((obj2->type&1023)==310)&&(obj2->type&1024)) goto move2allow; //portcullis
        }
      }
    }//blocked!
  }//mouse

  if (type==413){ //silver serpant MUST NOT GO BACKWARDS OVER OWN BODY!
    if ( ((obj->type>>10)==0) && (y>obj->y) ) return 1;
    if ( ((obj->type>>10)==2) && (x<obj->x) ) return 1;
    if ( ((obj->type>>10)==4) && (y<obj->y) ) return 1;
    if ( ((obj->type>>10)==6) && (x>obj->x) ) return 1;

    if ( ((obj->type>>10)==14) && (y>obj->y) ) return 1;
    if ( ((obj->type>>10)==15) && (x<obj->x) ) return 1;
  }

  if (pass==2){ //sea
    if (obj2=OBJfindlast(x,y)){ //can't move on top of other creature/player!
      if (obj2->info&(2+4)) return 1;
    }
    if (bt[y][x]&(4*1024)) goto move2allow;
    return 1;
  }

  if (pass==3){ //air
    if (obj2=OBJfindlast(x,y)){ //can't move on top of other creature/player!
      if (obj2->info&(2+4)) return 1;
    }
    if (bt[y][x]&(2*1024)) goto move2allow;
    return 1;
  }

  if ((obj->type&1023)==423){ //balloon
    if (obj2=OBJfindlast(x,y)){ //can't move on top of other creature/player!
      if (obj2->info&(2+4)) return 1;
    }
    if (bt[y][x]&(4*1024)) goto move2allow; //over sea

    if (bt[y][x]&1024){
      //if walking passable and NOT a doorway or a door allow entry
      if (obj2=OBJfindlastall(x,y)){ //can't move on top of other creature/player!
        if (((obj2->type&1023)>=297)&&((obj2->type&1023)<=302)) return 1;
      }
      goto move2allow; //land passable
    }
    if (od[y][x]){
      if ((od[y][x]->type&1023)==423){
        if ((bt[y][x]&1023)<=124) goto move2allow;
      }
    }
    if (od[y][x]==NULL){ //mountain & forest exception
      if ((bt[y][x]&1023)<=124) goto move2allow;
    }
    return 1;
  }

  //ships
  if ((obj->type&1023)==412){
    if (obj2=OBJfindlast(x,y)){ //can't move on top of other creature
      if (obj2->info&4) return 1;
    }
    xx=bt[y][x]&1023;
    if (((xx>=8)&&(xx<16))&&(bt[y][x]&(4*1024))) goto move2allow;
    return 1;
  }
  if ((obj->type&1023)==414){
    if (obj2=OBJfindlast(x,y)){ //can't move on top of other creature
      if (obj2->info&4) return 1;
    }
    if (bt[y][x]&(4*1024)) goto move2allow;
    return 1;
  }
  if ((obj->type&1023)==415){
    if (obj2=OBJfindlast(x,y)){ //can't move on top of other creature
      if (obj2->info&4) return 1;
    }
    if (bt[y][x]&(4*1024)) goto move2allow;
    return 1;
  }

  if (obji[sprlnk[obj->type&1023]].v4==10){ //serpent 2003: allow serpent to cross over it's tail!
    m2obj=OBJfindlast(x,y);
    if (m2obj!=NULL){
      if (obji[sprlnk[m2obj->type&1023]].v4==10){
        if (m2obj->info&8){
          if (m2obj->more==obj) goto move2allow;
        }
      }
    }
  }

  //check for animals and fences
  if (((obj->type&1023)==428)||((obj->type&1023)==430)||((obj->type&1023)==348)){
    if (od[y][x]!=NULL){
      if ((od[y][x]->type&1023)==281) return 1;
    }
  }

  if (obji[sprlnk[obj->type&1023]].v4==8){ //2-part
    m2obj=OBJfindlast(x,y);
    if (m2obj!=NULL){
      if (obji[sprlnk[m2obj->type&1023]].v4==8){
        if (m2obj->info&8){
          if (m2obj->more==obj) goto move2allow;
        }//has redirect
      }//2-part type
    }//!=NULL
  }//2-p





  if ((bt[y][x]&1024)||(keyon[VK_SPACE]&&U6O_DEBUG)){

move2allow:

    ox=obj->x;
    oy=obj->y;

    if ((obj->x==0)&&(obj->y==0)){
      dy=1; dx=0; //face forward
      goto skipremove;
    }

    //object can be added
    dx=0; dy=0;
    if (x>obj->x) dx=1;
    if (x<obj->x) dx=-1;
    if (y>obj->y) dy=1;
    if (y<obj->y) dy=-1;


    OBJremove(obj); //remove

    if ((obj->type&1023)==423){ //balloon (remove)
      static mlobj *mmyobj;
      if (obj->info&2){
        tnpc=(npc*)obj->more;
        tplayer=tnpc->player;
        mmyobj=(mlobj*)tplayer->craft_con;
      }else{
        mmyobj=(mlobj*)obj->more;
      }
      for (i=0;i<=2;i++){
        OBJremove(mmyobj->obj[i]);
      }
    }

    if ((obj->type&1023)==375){ //slime (remove)
      xx=0;
      if (obj2=OBJfindlast(ox,oy-1)){
        if ((obj2->type&1023)==375){
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }
      if (obj2=OBJfindlast(ox+1,oy)){
        if ((obj2->type&1023)==375){
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }
      if (obj2=OBJfindlast(ox,oy+1)){
        if ((obj2->type&1023)==375){
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }
      if (obj2=OBJfindlast(ox-1,oy)){
        if ((obj2->type&1023)==375){
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }
    }//slime (remove)


    //remove other constructs
    static object *myobj;
    xx=obj->type&1023;

    if (xx==412){ //boat
      static mlobj *mmyobj;
      if (obj->info&2){
        tnpc=(npc*)obj->more;
        tplayer=tnpc->player;
        mmyobj=(mlobj*)tplayer->craft_con;
      }else{
        mmyobj=(mlobj*)obj->more;
      }
      for (i=0;i<=4;i++){
        OBJremove(mmyobj->obj[i]);
      }
    }

    if (obji[sprlnk[xx]].v4==5){ //cyclops/winged gargoyle
      tnpc3=(npc*)obj->more;
      static mlobj *mmyobj;
      mmyobj=(mlobj*)tnpc3->more;
      for (i=0;i<=2;i++){
        OBJremove(mmyobj->obj[i]);
      }
    }

    if (obji[sprlnk[xx]].v4==9){ //dragon
      tnpc3=(npc*)obj->more;
      static mlobj *mmyobj;
      mmyobj=(mlobj*)tnpc3->more;
      for (i=0;i<=3;i++){
        OBJremove(mmyobj->obj[i]);
      }
    }

    /*
    if (obji[sprlnk[xx]].v4==10){ //serpent
    tnpc3=(npc*)obj->more;
    static mlobj *mmyobj;
    mmyobj=(mlobj*)tnpc3->more;
    for (i=0;i<=6;i++){
    //OBJremove(mmyobj->obj[i]);
    }
    }
    */

    if (obji[sprlnk[xx]].v4==11){ //hydra
      tnpc3=(npc*)obj->more;
      static mlobj *mmyobj;
      mmyobj=(mlobj*)tnpc3->more;
      for (i=0;i<=7;i++){
        OBJremove(mmyobj->obj[i]);
      }
    }




skipremove:

    //adjust flags accordingly
    xx=obj->type&1023;
    xx2=0;
    if (obji[sprlnk[obj->type&1023]].v4==1) xx2=1; //person
    if (xx==412) xx2=254; //boat
    if (obji[sprlnk[obj->type&1023]].v4==2) xx2=2; //demon,mongbat,gargoyle
    if (obji[sprlnk[obj->type&1023]].v4==6) xx2=2; //drake
    if (obji[sprlnk[obj->type&1023]].v4==8) xx2=3; //2-part
    if (obji[sprlnk[obj->type&1023]].v4==5) xx2=4; //4-part
    if (obji[sprlnk[obj->type&1023]].v4==9) xx2=9; //dragon
    if (obji[sprlnk[obj->type&1023]].v4==10) xx2=10; //dragon
    if (obji[sprlnk[obj->type&1023]].v4==11) xx2=11; //hydra
    if (obji[sprlnk[obj->type&1023]].v4==4) xx2=5; //mouse/rabbit
    if (xx==414) xx2=255; //skiff

    if ((obj->type&1023)==423){ //balloon
      static mlobj *mmyobj;
      if (obj->info&2){
        tnpc=(npc*)obj->more;
        tplayer=tnpc->player;
        mmyobj=(mlobj*)tplayer->craft_con;
      }else{
        mmyobj=(mlobj*)obj->more;
      }
      OBJadd(x-1,y-1,mmyobj->obj[0]);
      OBJadd(x,y-1,mmyobj->obj[1]);
      OBJadd(x-1,y,mmyobj->obj[2]);
    }

    //tangle vine?
    if ((obj->type&1023)==365){ //remove tendrils NOW! and add at new destination!
      static mlobj *mmyobj;
      tcrt=(creature*)obj->more;
      mmyobj=(mlobj*)tcrt->more;

      obj2=mmyobj->obj[0]; //first tendril!
      obj2->type=366+1*1024;

      OBJremove(obj2); OBJadd(x,y-1,obj2); //new location!
      //remove prev instances of obj2
      tcrt=(creature*)obj2->more;
      obj3=(object*)tcrt->more;
rt0: if (obj3!=obj){ //hack away my friends!
      OBJremove(obj3);
      obj3=(object*)obj3->more;
      goto rt0;
     }
     tcrt->more=(object*)obj;

     obj2=mmyobj->obj[1]; //first tendril!
     obj2->type=366+1*1024;
     OBJremove(obj2); OBJadd(x,y+1,obj2); //new location!
     //remove prev instances of obj2
     tcrt=(creature*)obj2->more;
     obj3=(object*)tcrt->more;
rt1: if (obj3!=obj){ //hack away my friends!
     OBJremove(obj3);
     obj3=(object*)obj3->more;
     goto rt1;
     }
     tcrt->more=(object*)obj;

     obj2=mmyobj->obj[2]; //first tendril!
     obj2->type=366;
     OBJremove(obj2); OBJadd(x-1,y,obj2); //new location!
     //remove prev instances of obj2
     tcrt=(creature*)obj2->more;
     obj3=(object*)tcrt->more;
rt2: if (obj3!=obj){ //hack away my friends!
     OBJremove(obj3);
     obj3=(object*)obj3->more;
     goto rt2;
     }
     tcrt->more=(object*)obj;

     obj2=mmyobj->obj[3]; //first tendril!
     obj2->type=366;
     OBJremove(obj2); OBJadd(x+1,y,obj2); //new location!
     //remove prev instances of obj2
     tcrt=(creature*)obj2->more;
     obj3=(object*)tcrt->more;
rt3: if (obj3!=obj){ //hack away my friends!
     OBJremove(obj3);
     obj3=(object*)obj3->more;
     goto rt3;
     }
     tcrt->more=(object*)obj;


    }//hack away my friends

    if ((obj->type&1023)==366){ //tanglevine tendril!

      xx=obj->type>>10;
      obj2=OBJnew();
      obj2->type=366+(xx<<10);
      tcrt=(creature*)obj->more;
      obj3=(object*)tcrt->more;
      obj2->more=(object*)tcrt->more;
      OBJadd(x-dx,y-dy,obj2);

      tcrt->more=(object*)obj2;

      tx=obj3->x; ty=obj3->y;


      if (tx<obj2->x){
        obj2->type=366+(0<<10);
        if (dy==1) obj2->type=366+(4<<10);
        if (dy==-1) obj2->type=366+(5<<10);
      }

      if (tx>obj2->x){
        obj2->type=366+(0<<10);
        if (dy==1) obj2->type=366+(3<<10);
        if (dy==-1) obj2->type=366+(2<<10);
      }

      if (ty<obj2->y){
        obj2->type=366+(1<<10);
        if (dx==1) obj2->type=366+(2<<10);
        if (dx==-1) obj2->type=366+(5<<10);
      }

      if (ty>obj2->y){
        obj2->type=366+(1<<10);
        if (dx==1) obj2->type=366+(3<<10);
        if (dx==-1) obj2->type=366+(4<<10);
      }

      if (dx) obj->type=366; else obj->type=366+1024;

    }


    if (xx2==11){ //hydra
      tnpc3=(npc*)obj->more;
      static mlobj *mmyobj;
      mmyobj=(mlobj*)tnpc3->more;
      OBJadd(x-1,y-1,mmyobj->obj[0]);
      OBJadd(x,y-1,mmyobj->obj[1]);
      OBJadd(x+1,y-1,mmyobj->obj[2]);
      OBJadd(x+1,y,mmyobj->obj[3]);
      OBJadd(x+1,y+1,mmyobj->obj[4]);
      OBJadd(x,y+1,mmyobj->obj[5]);
      OBJadd(x-1,y+1,mmyobj->obj[6]);
      OBJadd(x-1,y,mmyobj->obj[7]);
    }

    if (xx2==10){ //serpent
      //move all objects up one notch
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;
      obj->type=xx+(xx4<<11);
      //mess with dx&dy

      tnpc3=(npc*)obj->more;
      static mlobj *mmyobj;
      mmyobj=(mlobj*)tnpc3->more;


      for (i=6;i>=1;i--){
        OBJremove(mmyobj->obj[i]);
        OBJadd(mmyobj->obj[i-1]->x,mmyobj->obj[i-1]->y,mmyobj->obj[i]);
      }
      //xx3=mmyobj->obj[0]->x; xx4=mmyobj->obj[0]->y;
      OBJremove(mmyobj->obj[0]);
      OBJadd(ox,oy,mmyobj->obj[0]);

      //tail
      if (mmyobj->obj[5]->x<mmyobj->obj[6]->x) mmyobj->obj[6]->type=xx+7*1024;  
      if (mmyobj->obj[5]->x>mmyobj->obj[6]->x) mmyobj->obj[6]->type=xx+3*1024;  
      if (mmyobj->obj[5]->y<mmyobj->obj[6]->y) mmyobj->obj[6]->type=xx+1*1024;  
      if (mmyobj->obj[5]->y>mmyobj->obj[6]->y) mmyobj->obj[6]->type=xx+5*1024;  

      for (i=5;i>=0;i--){
        //2 values for 0 to 3 lets go!

        //dx=(mmyobj->obj[6]->type>>11)&3; //tail direction
        if (i!=0){
          if (mmyobj->obj[i-1]->x>mmyobj->obj[i]->x) xx4=1; //go to direction dx
          if (mmyobj->obj[i-1]->x<mmyobj->obj[i]->x) xx4=3;
          if (mmyobj->obj[i-1]->y<mmyobj->obj[i]->y) xx4=0;
          if (mmyobj->obj[i-1]->y>mmyobj->obj[i]->y) xx4=2;
        }else{//head is next
          if (x>mmyobj->obj[i]->x) xx4=1; //go to direction dx
          if (x<mmyobj->obj[i]->x) xx4=3;
          if (y<mmyobj->obj[i]->y) xx4=0;
          if (y>mmyobj->obj[i]->y) xx4=2;
        }

        if (mmyobj->obj[i+1]->x>mmyobj->obj[i]->x) xx3=1; //inbound from direction dy
        if (mmyobj->obj[i+1]->x<mmyobj->obj[i]->x) xx3=3;
        if (mmyobj->obj[i+1]->y<mmyobj->obj[i]->y) xx3=0;
        if (mmyobj->obj[i+1]->y>mmyobj->obj[i]->y) xx3=2;
        if (xx4==0){
          if (xx3==1) mmyobj->obj[i]->type=xx+10*1024;
          if (xx3==2) mmyobj->obj[i]->type=xx+8*1024;
          if (xx3==3) mmyobj->obj[i]->type=xx+13*1024;
        }
        if (xx4==1){
          if (xx3==0) mmyobj->obj[i]->type=xx+10*1024;
          if (xx3==2) mmyobj->obj[i]->type=xx+11*1024;
          if (xx3==3) mmyobj->obj[i]->type=xx+9*1024;
        }
        if (xx4==2){
          if (xx3==0) mmyobj->obj[i]->type=xx+8*1024;
          if (xx3==1) mmyobj->obj[i]->type=xx+11*1024;
          if (xx3==3) mmyobj->obj[i]->type=xx+12*1024;

          //mmyobj->obj[i]->type=xx+8*1024;

        }
        if (xx4==3){
          if (xx3==0) mmyobj->obj[i]->type=xx+13*1024;
          if (xx3==1) mmyobj->obj[i]->type=xx+9*1024;
          if (xx3==2) mmyobj->obj[i]->type=xx+12*1024;
        }


      }


      //if (dx!=0) {


      //mmyobj->obj[i]->type=xx+9*1024; 
      //OBJadd(mmyobj->obj[i]->x,y-1,mmyobj->obj[0]);


      //}




      //}



      //OBJremove(myobj->obj[6]);
    }

    if (xx2==9){ //dragon
      //xx3=(obj->type>>12)%3;
      //xx3++; if (xx3==3) xx3=0;

      //xx3=0;
      //xx3++; if (xx3==2) xx3=0;
      //fix direction
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;


      obj->type=xx+(xx4<<11);

      tnpc3=(npc*)obj->more;
      //if (tnpc3!=NULL){
      //if (obj->info&8) exit (0);
      //if (tnpc==NULL) exit2(obj->type&1023);

      static mlobj *mmyobj;
      mmyobj=(mlobj*)tnpc3->more;


      if (xx4==0){
        xx5=rnd*2; mmyobj->obj[0]->type=obj->type+8*1024+xx5*1024; OBJadd(x,y-1,mmyobj->obj[0]);
        xx5=rnd*2; mmyobj->obj[1]->type=obj->type+16*1024+xx5*1024; OBJadd(x,y+1,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type+24*1024; OBJadd(x-1,y,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+32*1024; OBJadd(x+1,y,mmyobj->obj[3]);
      }

      if (xx4==1){
        xx5=rnd*2; mmyobj->obj[0]->type=obj->type+8*1024+xx5*1024; OBJadd(x+1,y,mmyobj->obj[0]);
        xx5=rnd*2; mmyobj->obj[1]->type=obj->type+16*1024+xx5*1024; OBJadd(x-1,y,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type+24*1024; OBJadd(x,y-1,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+32*1024; OBJadd(x,y+1,mmyobj->obj[3]);
      }
      if (xx4==2){
        xx5=rnd*2; mmyobj->obj[0]->type=obj->type+8*1024+xx5*1024; OBJadd(x,y+1,mmyobj->obj[0]);
        xx5=rnd*2; mmyobj->obj[1]->type=obj->type+16*1024+xx5*1024; OBJadd(x,y-1,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type+24*1024; OBJadd(x+1,y,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+32*1024; OBJadd(x-1,y,mmyobj->obj[3]);
      }
      if (xx4==3){
        xx5=rnd*2; mmyobj->obj[0]->type=obj->type+8*1024+xx5*1024; OBJadd(x-1,y,mmyobj->obj[0]);
        xx5=rnd*2; mmyobj->obj[1]->type=obj->type+16*1024+xx5*1024; OBJadd(x+1,y,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type+24*1024; OBJadd(x,y+1,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+32*1024; OBJadd(x,y-1,mmyobj->obj[3]);
      }
      //xx5=rnd*2; obj->type+=xx5*1024;

      //myobj=mmyobj->obj[0];
      //exit2(myobj->info);

    }


    /*
    if (obji[sprlnk[myobj2->type&1023]].v4==9){ //dragon

    static mlobj *mmyobj; //array size varies
    mmyobj=(mlobj*)malloc(sizeof(object*)*4);
    crt->more=mmyobj;
    myobj2->type=411; //(myobj2->type&1023)+3*1024;

    myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[0]=myobj3;
    myobj3->type=myobj2->type+8*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y-1,myobj3);
    myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[1]=myobj3;
    myobj3->type=myobj2->type+16*1024; myobj3->info|=8; //<-base object
    OBJadd(x,y+1,myobj3);
    myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[2]=myobj3;
    myobj3->type=myobj2->type+24*1024; myobj3->info|=8; //<-base object
    OBJadd(x-1,y,myobj3);
    myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[3]=myobj3;
    myobj3->type=myobj2->type+32*1024; myobj3->info|=8; //<-base object
    OBJadd(x+1,y,myobj3);

    //crt->more=myobj3;


    }
    */







    if (xx2==4){ //4-part cyclops/winged gargoyle
      xx3=(obj->type>>12)%3;
      xx3++; if (xx3==3) xx3=0;
      if (xx==424){ //cyclops
        xx3=(obj->type>>12)&1;
        xx3++; if (xx3==2) xx3=0;
      }

      //xx3=0;
      //xx3++; if (xx3==2) xx3=0;
      //fix direction
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;
      obj->type=xx+(xx3<<12)+(xx4*12288)+1024*3;
      if (xx==424){ //cyclops
        obj->type=xx+(xx3<<12)+(xx4<<13)+1024*3;
      }

      tnpc3=(npc*)obj->more;
      if (tnpc3!=NULL){
        //if (obj->info&8) exit (0);
        //if (tnpc==NULL) exit2(obj->type&1023);

        static mlobj *mmyobj;
        mmyobj=(mlobj*)tnpc3->more;

        mmyobj->obj[0]->type=obj->type-3*1024; OBJadd(x-1,y-1,mmyobj->obj[0]);
        mmyobj->obj[1]->type=obj->type-2*1024; OBJadd(x,y-1,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type-1*1024; OBJadd(x-1,y,mmyobj->obj[2]);

        //myobj=mmyobj->obj[0];
        //exit2(myobj->info);

      }

      /*
      static mlobj *mmyobj; //array size varies
      mmyobj=(mlobj*)malloc(sizeof(object*)*3);
      tnpc->more=mmyobj;
      myobj2->type=(myobj2->type&1023)+3*1024;
      myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[0]=myobj2;
      myobj3->type=myobj2->type-3*1024; myobj3->info|=8; //<-base object
      OBJadd(x-1,y-1,myobj3);
      myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[1]=myobj2;
      myobj3->type=myobj2->type-2*1024; myobj3->info|=8; //<-base object
      OBJadd(x,y-1,myobj3);
      myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[2]=myobj2;
      myobj3->type=myobj2->type-1*1024; myobj3->info|=8; //<-base object
      OBJadd(x-1,y,myobj3);
      */

    }


    if (xx2==3){ //2-part creature

      xx3=(obj->type>>10)&1;
      xx3++; if (xx3==2) xx3=0;
      //fix direction
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;
      obj->type=xx+(xx3<<10)+(xx4<<11);

      //attach backside to correct position
      //get backside
      static creature *crt;
      static object *myobj;
      crt=(creature*)obj->more;
      myobj=(object*)crt->more;

      myobj->type=xx+(xx3<<10)+(xx4<<11)+8*1024;
      OBJremove(myobj);
      OBJadd(x-dx,y-dy,myobj);




      /*
      if (obji[sprlnk[myobj2->type&1023]].v4==8){ //2-part creature
      myobj3=OBJnew(); myobj3->more=myobj2;
      myobj3->type=myobj2->type+8*1024; myobj3->info|=8; //<-base object
      OBJadd(x,y+1,myobj3);
      crt->more=myobj3;
      }
      */

    }


    if (xx2==1){
      xx3=(obj->type>>10)&3;

      if (xx==363) xx3=(obj->type>>10)%3; //*gargoyle

      if (obj->info&32768){ //update walking position offset
        //fix pose
        //->info flag 1 is to do with walking!
        if ((xx3==1)&&(obj->info&1)){
          xx3=0;
          obj->info-=1;
        }else{
          xx3++;
          if (xx3==4) {obj->info|=1; xx3=1;}
          if (xx3==3) {obj->info|=1; xx3=1;}
        }
        obj->info-=32768;
      }

      //fix direction
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;

      /*
      //sitting? temp fix only!
      m2obj=OBJfindlast(x,y);
      if (m2obj){
      if ((m2obj->type&1023)==252){
      xx4=(m2obj->type&3072)>>10;
      xx3=3;
      }

      if ((m2obj->type&1023)==327){
      if (((m2obj->type&3072)>>10)==2){
      xx4=(m2obj->type&3072)>>10;
      xx3=3;
      }}

      }
      */

      obj->type=xx+(xx3<<10)+(xx4<<12);
      if (xx==363) obj->type=xx+(xx3<<10)+(xx4<<10)*3; //*gargoyle

    } //xx2=1

    if (xx2==2){
      xx3=(obj->type>>10)&1;
      xx3++; if (xx3==2) xx3=0;
      //fix direction
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;
      obj->type=xx+(xx3<<10)+(xx4<<11);
    } //xx2=2

    if (xx2==255){ //skiff
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;
      obj->type=xx+(xx4<<10);
    } //xx2=255


    if (xx2==5){ //mouse/rabbit
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;
      obj->type=xx+(xx4<<10);
    } //xx2=5


    //if (xx==412) xx2=254; //boat
    if (xx2==254){ //boat
      if (dx==-1) xx4=3;
      if (dx==1) xx4=1;
      if (dy==-1) xx4=0;
      if (dy==1) xx4=2;
      obj->type=xx+((xx4*2+9)<<10);
      //add constructs

      static mlobj *mmyobj;
      if (obj->info&2){
        tnpc=(npc*)obj->more;
        tplayer=tnpc->player;
        mmyobj=(mlobj*)tplayer->craft_con;
      }else{
        mmyobj=(mlobj*)obj->more;
      }

      if (xx4==0){
        mmyobj->obj[0]->type=obj->type-1*1024; OBJadd(x-1,y,mmyobj->obj[0]);
        mmyobj->obj[1]->type=obj->type-8*1024; OBJadd(x,y-1,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type-9*1024; OBJadd(x-1,y-1,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+8*1024; OBJadd(x,y+1,mmyobj->obj[3]);
        mmyobj->obj[4]->type=obj->type+7*1024; OBJadd(x-1,y+1,mmyobj->obj[4]);
      }
      if (xx4==2){
        mmyobj->obj[0]->type=obj->type-1*1024; OBJadd(x-1,y,mmyobj->obj[0]);
        mmyobj->obj[1]->type=obj->type-8*1024; OBJadd(x,y+1,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type-9*1024; OBJadd(x-1,y+1,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+8*1024; OBJadd(x,y-1,mmyobj->obj[3]);
        mmyobj->obj[4]->type=obj->type+7*1024; OBJadd(x-1,y-1,mmyobj->obj[4]);
      }

      if (xx4==1){
        mmyobj->obj[0]->type=obj->type-1*1024; OBJadd(x,y-1,mmyobj->obj[0]);
        mmyobj->obj[1]->type=obj->type-8*1024; OBJadd(x+1,y,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type-9*1024; OBJadd(x+1,y-1,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+8*1024; OBJadd(x-1,y,mmyobj->obj[3]);
        mmyobj->obj[4]->type=obj->type+7*1024; OBJadd(x-1,y-1,mmyobj->obj[4]);
      }

      if (xx4==3){
        mmyobj->obj[0]->type=obj->type-1*1024; OBJadd(x,y-1,mmyobj->obj[0]);
        mmyobj->obj[1]->type=obj->type-8*1024; OBJadd(x-1,y,mmyobj->obj[1]);
        mmyobj->obj[2]->type=obj->type-9*1024; OBJadd(x-1,y-1,mmyobj->obj[2]);
        mmyobj->obj[3]->type=obj->type+8*1024; OBJadd(x+1,y,mmyobj->obj[3]);
        mmyobj->obj[4]->type=obj->type+7*1024; OBJadd(x+1,y-1,mmyobj->obj[4]);
      }


    } //xx2=255

    OBJadd(x,y,obj);

    if ((obj->type&1023)==375){ //slime (combine)
      //need a grid addition array!
      /*
      ka1dosh: ok.. label each connection clockwise
      galleondragon: oh I see the pattern now
      ka1dosh: now, if it has a connection, put a 1 in that bit, if it dont, then a 0
      ka1dosh: then its binary
      galleondragon: gradually all each element and every combination of each element
      ka1dosh: least sig at the top
      u=bit 0
      r=bit 1
      d=bit 2
      l=bit 3
      connections (u,d,l,r,X)
      X
      u 0001
      r 0010
      u,r 0011
      d 0100
      u,d
      d,r
      u,d,r
      l
      u,l
      l,r
      u,l,r
      d,l
      u,d,l
      d,l,r
      u,d,l,r
      */

      xx=0;
      if (obj2=OBJfindlast(x,y-1)){
        if ((obj2->type&1023)==375){
          xx|=1;
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }

      if (obj2=OBJfindlast(x+1,y)){
        if ((obj2->type&1023)==375){
          xx|=2;
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }

      if (obj2=OBJfindlast(x,y+1)){
        if ((obj2->type&1023)==375){
          xx|=4;
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }

      if (obj2=OBJfindlast(x-1,y)){
        if ((obj2->type&1023)==375){
          xx|=8;
          xx3=0;
          if (obj3=OBJfindlast(obj2->x,obj2->y-1)){
            if ((obj3->type&1023)==375) xx3|=1;
          }
          if (obj3=OBJfindlast(obj2->x+1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=2;
          }
          if (obj3=OBJfindlast(obj2->x,obj2->y+1)){
            if ((obj3->type&1023)==375) xx3|=4;
          }
          if (obj3=OBJfindlast(obj2->x-1,obj2->y)){
            if ((obj3->type&1023)==375) xx3|=8;
          }
          obj2->type=375+xx3*1024;
        }
      }
      obj->type=375+xx*1024;
    }//slime

    return 0;

  } //walking living passable
  return 1;
}


//OBJdir changes direction of NPC/creature to face x,y
unsigned char OBJdir(object* obj,unsigned long x,unsigned long y){





  static long dx,dy;
  static unsigned short movertype;
  static creature *crt;
  static object *obj2;

  movertype=obj->type&1023;

  if (obji[sprlnk[movertype]].v4==8){//two part movers (only frame change)
    if (obj->info&(2+4)){//crt or npc
      crt=(creature*)obj->more;
      obj2=(object*)crt->more;
      if (obj->type&1024){
        obj->type-=1024;
        if (obj2->type&1024) obj2->type-=1024;
      }else{
        obj->type|=1024;
        obj2->type|=1024;
      }
    }//crt or npc
    return NULL;
  }

  if (movertype==366) return NULL; //tanglevine tendril
  if (movertype==365) return NULL; //tanglevine core
  if (movertype==413){//serpent
    //open & close mouth
    if (obj->type==(413+0*1024)){obj->type=413+14*1024; return NULL;}
    if (obj->type==(413+2*1024)){obj->type=413+15*1024; return NULL;}
    if (obj->type==(413+14*1024)){obj->type=413+0*1024; return NULL;}
    if (obj->type==(413+15*1024)){obj->type=413+2*1024; return NULL;}
    return NULL; 
  }
  if (movertype==347) return NULL; //reaper
  if (movertype==343) return NULL; //insects
  if (movertype==344) return NULL; //bat
  if (movertype==345) return NULL; //squid
  if (movertype==352) return NULL; //ghost
  if (movertype==353) return NULL; //gremlin
  if (movertype==355) return NULL; //gazer
  if (movertype==356) return NULL; //bird
  if (movertype==357) return NULL; //corpser
  if (movertype==360) return NULL; //rotworms
  if (movertype==364) return NULL; //acid slug
  if (movertype==373) return NULL; //wisp
  if (movertype==374) return NULL; //hydra
  if (movertype==425) return NULL; //hydra (tenticles)
  if (movertype==375) return NULL; //slime
  if (movertype==391) return NULL; //hoeing farmer
  if (movertype==415) return NULL; //raft

  //current method cannot be used if near map boundaries!
  if ((obj->x<=1)||(obj->y<=1)||(obj->x>=2046)||(obj->y>=1022)) return NULL;


  dx=0; dy=0;
  if (x>obj->x) dx=1;
  if (x<obj->x) dx=-1;
  if (y>obj->y) dy=1;
  if (y<obj->y) dy=-1;

  if (abs((long)(x-obj->x))>abs((long)(y-obj->y))) dy=0; else dx=0;



  OBJmove_allow=TRUE;
  OBJmove2(obj,obj->x-dx,obj->y-dy);
  //OBJmove2(obj,obj->x-dx,obj->y-dy);
  //OBJmove2(obj,obj->x+dx*2,obj->y+dy*2);
  OBJmove2(obj,obj->x+dx,obj->y+dy);
  OBJmove_allow=FALSE;

  return NULL;

  /*
  if ((xx>=376)&&(xx<=387)) xx2=1; //fighter...woman
  if ((xx>=409)&&(xx<=410)) xx2=1; //LB+avatar
  if ((xx>=367)&&(xx<=372)) xx2=2; //demon...mongbat

  if (xx2==2){
  xx3=(obj->type>>10)&1;
  //fix direction
  if (dx==-1) xx4=3;
  if (dx==1) xx4=1;
  if (dy==-1) xx4=0;
  if (dy==1) xx4=2;
  obj->type=xx+(xx3<<10)+(xx4<<11);
  } //xx2=2
  */

  return NULL;
}



void wpf_pathfind(unsigned char *d,long sourcex,long sourcey,long destx,long desty,long maxdistance,unsigned long type,unsigned long flags){
  static long x,x2,x3,x4,y,y2,y3,y4,z,z2,z3,z4,zlooplast;//temp use variables
  static object *obj,obj2,obj3,obj4;
  static npc *tnpc,*tnpc2;
  static mlobj *mmyobj;

  wpf_nextto=WPF_NEXTTO; WPF_NEXTTO=0;

  if ((flags&8)==0){
    //return if dest is the same as source
    if ((destx==sourcex)&&(desty==sourcey)){
      WPF_PATHLENGTH=0; WPF_RETURN=WPF_PATHFOUND; return;
    }
    if (wpf_nextto){
      if ((abs(destx-sourcex)<=1)&&(abs(desty-sourcey)<=1)){
        WPF_PATHLENGTH=0; WPF_RETURN=WPF_PATHFOUND; return;
      }
    }//wpf_nextto
    //return if dest is out of range of maxdistance
    x=0; if (wpf_nextto) x=1;
    if ((abs(destx-sourcex)+x)>maxdistance){
      WPF_RETURN=WPF_NOPATHFOUND; return;
    }
    if ((abs(desty-sourcey)+x)>maxdistance){
      WPF_RETURN=WPF_NOPATHFOUND; return;
    }
  }//(flags&8)==0

  WPF_OFFSETX=sourcex-maxdistance-1; WPF_OFFSETY=sourcey-maxdistance-1;
  z=maxdistance*2+2;//array limit

  ZeroMemory(&wpf_sourcedest,sizeof(wpf_sourcedest));

  //set boundries
  for (x=0;x<=z;x++){
    wpf_weight[x][0]=0;
    wpf_weight[x][z]=0;
    wpf_weight[0][x]=0;
    wpf_weight[z][x]=0;
  }


  if (type==0){//type 0 setup
    wpf_npc=(npc*)WPF_OBJECT->more;
  }

  //create weightings
  for (y=1;y<z;y++){ for (x=1;x<z;x++){

    z2=0;//accumulative weight
    x2=WPF_OFFSETX+x; y2=WPF_OFFSETY+y; if (x2>=0){ if (y2>=0){ if (x2<2048){ if (y2<1024){//map boundries check
      x3=bt[y2][x2];


      if (type==0){
        z3=0;

        if ((x3&1024)==0){//not land passable
          if ((x3&16384)==0){//not npc-swap passable
            wpf_weight[x][y]=0; goto wpf_blocked;
          }//not npc-swap passable



          if (flags&2){//cannot swap with any npc, so this is a last resort to all alternatives!
            z3=65536; goto wpf_passcheckdone;
          }

          if (flags&1){
            if (obj=od[y2][x2]){
wpft0next:


              //if ((obj->type&1023)==431){
              //if ((obj->type>>10)>=8){
              //z3=16;
              //}
              //}


              //if ((obj->type&1023)==431){
              //if ((obj->type>>

              //}

              if (obj->info&2){
                tnpc=(npc*)obj->more;
                if (wpf_npc->player==tnpc->player){
                  //wpf_weight[x][y]=0; goto wpf_blocked;
                  z3=16;
                }//wpf_npc->player==tnpc->player
              }//info&2
              if (obj=(object*)obj->next) goto wpft0next;
            }//obj
          }//flags&1

        }//not land passable
wpf_passcheckdone:

        /*
        OFFICIAL
        0	a place that cannot be travelled on
        3	path
        4	grass or swamp+boots
        28	tanglevine (6 extra steps to avoid)
        36	light lava,fire field,sleep field,spikes (8 steps to avoid)
        44	swamp(no boots),poison field(if not poisoned),medium lava,traps(if visible) (10 extra steps to avoid)
        52	heavy lava (12 extra steps to avoid)
        +16	partymember of same party (4 extra steps to avoid)
        */
        x4=x3&1023;
        z2=4;
        if (flags&4) z2=12;//for npc scheduled pathfinding avoid non-path tiles if possible
        if ((x4>=108)&&(x4<=111)) z2=3;
        if ((x4>=208)&&(x4<=220)) z2=3;
        if ((x4>=3)&&(x4<=6)){
          z2=44;//swamp(no boots)
          if (wpf_npc->boots!=NULL){ if (wpf_npc->boots->type==28){ z2=4; }}//swamp boots
        }
        if (x4==221) z2=36;//light lava
        if (x4==222) z2=44;//medium lava
        if (x4==223) z2=52;//heavy lava
        if (obj=od[y2][x2]){
wpft0next2:
          z4=obj->type&1023;
          if (z4==172) z2=36;//spikes
          if (z4==173){//trap
            if (obj->info&(1<<9)) z2=44;
          }//trap
          if (z4==317) z2=36;//fire field
          if (z4==318){//poison field
            if ((wpf_npc->flags&1)==0) z2=44;//only if not poisoned
          }//318
          if (z4==320) z2=36;//sleep field
          if (z4==366) z2=28;//tendril
          if (obj=(object*)obj->next) goto wpft0next2;
        }//obj
        z2+=z3;
      }//type==0

      if (type==1){//land passable
        if (x3&1024) z2=1; 
      }//type==1

      if (type==2){//sea passable
        if (x3&4096) z2=1;
      }//type==2

      if (type==3){//air passable
        if (x3&2048) z2=1;
      }//type==3

      if (type==4){//ghost passable
        if (obj=OBJfindlast(x2,y2)){
          z=1; if (obj->info&6) z=0;
        }
      }

      if (type==5){//tanglevine tendril passable
        if (x3&1024) z=1;
        if (od[y2][x2]) z=0;
      }


    }}}}//map boundries check
    wpf_weight[x][y]=z2;
wpf_blocked:;
  }}//x,y
  //wpfdebug1:

  wpf_pathfound=0;
  wpf_lastusedn=-1;
  wpf_laststackedn=0;
  ZeroMemory(&wpf_nweight,sizeof(wpf_nweight));

  //allow squares belonging to parts of the mover calling the pathfind to be passable
  if (obji[sprlnk[WPF_OBJECT->type&1023]].v4==8){//2-part mover
    obj=WPF_OBJECT; if (obj->info&8) obj=(object*)obj->more;//get "head" of mover
    wpf_npc=(npc*)obj->more; obj=(object*)wpf_npc->more;

    //NEW GPF THIS LINE! X2!
    wpf_weight[obj->x-WPF_OFFSETX][obj->y-WPF_OFFSETY]=1;

  }//8
  if (obji[sprlnk[WPF_OBJECT->type&1023]].v4==10){//silver serpent
    obj=WPF_OBJECT; if (obj->info&8) obj=(object*)obj->more;//get "head" of mover
    wpf_npc=(npc*)obj->more; mmyobj=(mlobj*)wpf_npc->more;
    for (z=1;z<=6;z++){
      wpf_weight[mmyobj->obj[z]->x-WPF_OFFSETX][mmyobj->obj[z]->y-WPF_OFFSETY]=1;
    }//z
    wpf_weight[mmyobj->obj[0]->x-WPF_OFFSETX][mmyobj->obj[0]->y-WPF_OFFSETY]=0;
  }//10
  //wpfdebug2:

  if ((flags&8)==0){

    //add dest node(s)
    x=destx-WPF_OFFSETX; y=desty-WPF_OFFSETY;
    if(wpf_nextto){
      //extra dest nodes can only be added if the square they are on is walkable!

      wpf_weight[x][y]=0;
      y--;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      y++; x++;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      x--; y++;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      y--; x--;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      x++;

      //if (wpf_lastusedn==-1){
      x++; y--;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      y+=2;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      x-=2;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      y-=2;
      if(z=wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=z; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=z;}
      //}


      /*
      //wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;
      x++; y--;
      if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
      y+=2;
      if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
      x-=2;
      if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
      y-=2;
      if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
      */

      if (wpf_lastusedn==-1){ WPF_RETURN=WPF_NOPATHFOUND; return;}
      //could crash be because this line should be ==0???
    }else{
      //singular dest node
      wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;
    }

  }//(flags&8)==0

  //add source node
  wpf_lastusedn++;
  wpf_nx[wpf_lastusedn]=maxdistance+1; wpf_ny[wpf_lastusedn]=maxdistance+1; wpf_nsource[wpf_lastusedn]=1; wpf_nweight[wpf_lastusedn]=1;
  wpf_sourcedest[maxdistance+1][maxdistance+1]=1; wpf_bestweight[maxdistance+1][maxdistance+1]=1;

  static unsigned long ild;
  ild=0;

  static unsigned long wpf_goalweight,wpf_goalx,wpf_goaly;
  wpf_goalweight=0x7FFFFFFF;

  nweight=1;
wpf_pathfindloop:

  if (nweight>=wpf_goalweight){
    x=wpf_goalx; y=wpf_goaly; goto wpf_pathfound;
  }


  ild++;
  if (ild>=65536) MessageBox(NULL,"ild exceeded!","Ultima 6 Online",MB_OK);

  if (wpf_lastusedn==-1){ if (wpf_goalweight==0x7FFFFFFF){
    WPF_RETURN=WPF_NOPATHFOUND; return;
  }}

  wpf_nodeaddflags=0;
  nextnweight=0x7FFFFFFF;
  z=0; zlooplast=wpf_lastusedn; wpf_zloop:
  if (z2=wpf_nweight[z]){
    if (z2==nweight){

      if (wpf_nsource[z]==1){//source node
        z3=1; z4=2;
      }else{//dest node
        z3=2; z4=1;
      }//node type


      //scan adjacent squares
      x=wpf_nx[z]; y=wpf_ny[z];



      //if (wpf_sourcedest[x][y]&4) goto wpf_pathfound;//check for path joined bit




      x++;
      if (x2=wpf_weight[x][y]){//weight exists? if so square is passable
        if (y2=wpf_sourcedest[x][y]){
          if (y2&z4){ if ((z2+x2)<wpf_goalweight){ wpf_goalweight=z2+x2; wpf_goalx=x; wpf_goaly=y;}}
          //if same type ignore
        }else{//wpf_sourcedest==NULL
          //add new node
          //	wpf_naxp1:
          wpf_nodeaddflags|=z3;	
          if (wpf_laststackedn){
            y2=wpf_stackn[wpf_laststackedn]; wpf_nx[y2]=x; wpf_ny[y2]=y; wpf_nsource[y2]=z3; wpf_nweight[y2]=z2+x2; wpf_laststackedn--;
          }else{
            wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=z3; wpf_nweight[wpf_lastusedn]=z2+x2;
          }
          wpf_sourcedest[x][y]=z3; wpf_bestweight[x][y]=z2+x2;//update array
          if ((z2+x2)<nextnweight) nextnweight=z2+x2;
        }//wpf_sourcedest
      }//wpf_weight

      x-=2;
      if (x2=wpf_weight[x][y]){//weight exists? if so square is passable
        if (y2=wpf_sourcedest[x][y]){
          if (y2&z4){ if ((z2+x2)<wpf_goalweight){ wpf_goalweight=z2+x2; wpf_goalx=x; wpf_goaly=y;}}
          //if same type ignore
        }else{//wpf_sourcedest==NULL
          //add new node
          //	wpf_naxm1:
          wpf_nodeaddflags|=z3;
          if (wpf_laststackedn){
            y2=wpf_stackn[wpf_laststackedn]; wpf_nx[y2]=x; wpf_ny[y2]=y; wpf_nsource[y2]=z3; wpf_nweight[y2]=z2+x2; wpf_laststackedn--;
          }else{
            wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=z3; wpf_nweight[wpf_lastusedn]=z2+x2;
          }
          wpf_sourcedest[x][y]=z3; wpf_bestweight[x][y]=z2+x2;//update array
          if ((z2+x2)<nextnweight) nextnweight=z2+x2;
        }//wpf_sourcedest
      }//wpf_weight

      x++; y--;
      if (x2=wpf_weight[x][y]){//weight exists? if so square is passable
        if (y2=wpf_sourcedest[x][y]){
          if (y2&z4){ if ((z2+x2)<wpf_goalweight){ wpf_goalweight=z2+x2; wpf_goalx=x; wpf_goaly=y;}}
          //if same type ignore
        }else{//wpf_sourcedest==NULL
          //add new node
          //	wpf_naym1:
          wpf_nodeaddflags|=z3;
          if (wpf_laststackedn){
            y2=wpf_stackn[wpf_laststackedn]; wpf_nx[y2]=x; wpf_ny[y2]=y; wpf_nsource[y2]=z3; wpf_nweight[y2]=z2+x2; wpf_laststackedn--;
          }else{
            wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=z3; wpf_nweight[wpf_lastusedn]=z2+x2;
          }
          wpf_sourcedest[x][y]=z3; wpf_bestweight[x][y]=z2+x2;//update array
          if ((z2+x2)<nextnweight) nextnweight=z2+x2;
        }//wpf_sourcedest
      }//wpf_weight

      y+=2;
      if (x2=wpf_weight[x][y]){//weight exists? if so square is passable
        if (y2=wpf_sourcedest[x][y]){
          if (y2&z4){ if ((z2+x2)<wpf_goalweight){ wpf_goalweight=z2+x2; wpf_goalx=x; wpf_goaly=y;}}
          //if same type ignore
        }else{//wpf_sourcedest==NULL
          //add new node
          //	wpf_nayp1:
          wpf_nodeaddflags|=z3;
          if (wpf_laststackedn){
            y2=wpf_stackn[wpf_laststackedn]; wpf_nx[y2]=x; wpf_ny[y2]=y; wpf_nsource[y2]=z3; wpf_nweight[y2]=z2+x2; wpf_laststackedn--;
          }else{
            wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=z3; wpf_nweight[wpf_lastusedn]=z2+x2;
          }
          wpf_sourcedest[x][y]=z3; wpf_bestweight[x][y]=z2+x2;//update array
          if ((z2+x2)<nextnweight) nextnweight=z2+x2;
        }//wpf_sourcedest
      }//wpf_weight

      //remove current node (add to stack if necessary)
      if (z==wpf_lastusedn){
        wpf_lastusedn--;
      }else{
        wpf_laststackedn++; wpf_stackn[wpf_laststackedn]=z; wpf_nweight[z]=0;
      }

    }else{//z2==nweight
      //z2!=nweight
      if (z2<nextnweight) nextnweight=z2;
      wpf_nodeaddflags|=wpf_nsource[z];

    }//z2==nweight
  }//z2=wpf_nweight[z]
  z++; if (z<=zlooplast) goto wpf_zloop;

  if (flags&8) wpf_nodeaddflags|=2;//override dest. node available flag
  if (((wpf_nodeaddflags&1)==0)||((wpf_nodeaddflags&2)==0)){
    if (wpf_goalweight==0x7FFFFFFF){
      WPF_RETURN=WPF_NOPATHFOUND; return;
    }
    nextnweight=wpf_goalweight;
  }

  nweight=nextnweight;
  goto wpf_pathfindloop;

wpf_pathfound:

  /*
  //make a file dump of the array
  static file *tfh;
  static txt *t=txtnew();
  static txt *t2=txtnew();
  tfh=open2("wpf.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
  z2=maxdistance*2+2;
  for (y2=0;y2<=z2;y2++){
  txtset(t,"");
  for (x2=0;x2<=z2;x2++){
  txtnumint(t2,wpf_sourcedest[x2][y2]);
  txtadd(t,t2);
  t2->d2[0]=((wpf_bestweight[x2][y2]>>2)&15)+65;
  txtadd(t,t2);
  }
  txtfileout(t,tfh);
  }
  close(tfh);
  */

  WPF_RETURN=WPF_PATHFOUND;
  WPF_PATHLENGTH=0;
  //x and y are where pathfind met
  x4=x; y4=y;//backup

  ild=0;


  //allow for this square to be the source or dest!!!
  x3=sourcex-WPF_OFFSETX; y3=sourcey-WPF_OFFSETY;
  x2=-1;
join2sourcepath_loop:
  if((x!=x3)||(y!=y3)){//not the source
    ild++;
    if (ild>=65536) MessageBox(NULL,"ild2 exceeded!","Ultima 6 Online",MB_OK);


    x2++;
    //check surrounding squares for lowest source bestweight
    y2=0x7FFFFFFF;
    if (wpf_sourcedest[x][y+1]&1){ if (wpf_bestweight[x][y+1]<y2){
      join2sourcepath[x2]=PFup; y2=wpf_bestweight[x][y+1];
    }}
    if (wpf_sourcedest[x-1][y]&1){ if (wpf_bestweight[x-1][y]<y2){
      join2sourcepath[x2]=PFright; y2=wpf_bestweight[x-1][y];
    }}
    if (wpf_sourcedest[x][y-1]&1){ if (wpf_bestweight[x][y-1]<y2){
      join2sourcepath[x2]=PFdown; y2=wpf_bestweight[x][y-1];
    }}
    if (wpf_sourcedest[x+1][y]&1){ if (wpf_bestweight[x+1][y]<y2){
      join2sourcepath[x2]=PFleft; y2=wpf_bestweight[x+1][y];
    }}
    if (join2sourcepath[x2]==PFleft) x++;
    if (join2sourcepath[x2]==PFright) x--;
    if (join2sourcepath[x2]==PFup) y++;
    if (join2sourcepath[x2]==PFdown) y--;
    goto join2sourcepath_loop;
  }//not the source
  //copy join2sourcepath to *d
  for (x3=x2;x3>=0;x3--){
    d[WPF_PATHLENGTH]=join2sourcepath[x3]; WPF_PATHLENGTH++;
  }

  x=x4; y=y4;//restore path connection co-ordinates
  ild=0;
  x3=destx-WPF_OFFSETX; y3=desty-WPF_OFFSETY;
  x2=-1;
join2destpath_loop:

  //if ((wpf_bestweight[x][y]!=1)||(wpf_sourcedest[x][y]&1)){//not the dest

  if ((abs(x-x3)>wpf_nextto)||(abs(y-y3)>wpf_nextto)){//not the dest

    ild++;
    if (ild>=65536) MessageBox(NULL,"ild3 exceeded!","Ultima 6 Online",MB_OK);


    x2++;
    //check surrounding squares for lowest dest bestweight
    y2=0x7FFFFFFF;

    if (wpf_sourcedest[x][y-1]&2){ if (wpf_bestweight[x][y-1]<y2){
      join2destpath[x2]=PFup; y2=wpf_bestweight[x][y-1];
    }}
    if (wpf_sourcedest[x+1][y]&2){ if (wpf_bestweight[x+1][y]<y2){
      join2destpath[x2]=PFright; y2=wpf_bestweight[x+1][y];
    }}

    if (wpf_sourcedest[x][y+1]&2){ if (wpf_bestweight[x][y+1]<y2){
      join2destpath[x2]=PFdown; y2=wpf_bestweight[x][y+1];
    }}
    if (wpf_sourcedest[x-1][y]&2){ if (wpf_bestweight[x-1][y]<y2){
      join2destpath[x2]=PFleft; y2=wpf_bestweight[x-1][y];
    }}
    if (join2destpath[x2]==PFleft) x--;
    if (join2destpath[x2]==PFright) x++;
    if (join2destpath[x2]==PFup) y--;
    if (join2destpath[x2]==PFdown) y++;
    goto join2destpath_loop;
  }//not the dest
  //copy join2destpath to *d
  for (x3=0;x3<=x2;x3++){
    d[WPF_PATHLENGTH]=join2destpath[x3]; WPF_PATHLENGTH++;
  }

  return;
}

//returns a comparitive value re. danger of a square for a mover to walk on/over
unsigned char wpf_dangercheck(short x,short y,object* mover){
  static long z,z2,z3,z4;//temp use variables
  static object *obj,obj2;
  static npc *tnpc;
  z=0;
  if (x>=0){ if (y>=0){ if (x<2048){ if (y<1024){//map boundries check
    tnpc=(npc*)mover->more;
    z4=bt[y][x]&1023;
    if ((z4>=3)&&(z4<=6)){
      z=44;//swamp(no boots)
      if (tnpc->boots){ if (tnpc->boots->type==28){ z=0; }}//swamp boots
    }
    if (z4==221) z=36;//light lava
    if (z4==222) z=44;//medium lava
    if (z4==223) z=52;//heavy lava
    if (obj=od[y][x]){
wpfdc:
      z4=obj->type&1023;
      if (z4==172) z=36;//spikes
      if (z4==173){//trap
        if (obj->info&(1<<9)) z=44;
      }//trap
      if (z4==317) z=36;//fire field
      if (z4==318){//poison field
        if ((tnpc->flags&1)==0) z2=44;//only if not poisoned
      }//318
      if (z4==320) z=36;//sleep field
      if (z4==366) z=28;//tendril
      if (obj=(object*)obj->next) goto wpfdc;
    }//obj
  }}}}
  return z;
}//wpf_dangercheck

bool OBJcheckbolt(unsigned short x,unsigned short y,unsigned short x2,unsigned short y2){
  OBJcheckbolt_x=x2; OBJcheckbolt_y=y2;
  Ocb_x=x; Ocb_y=y;
  Ocb_ix=abs(x2-x);
  Ocb_iy=abs(y2-y);
  Ocb_l=sqrt((float)(Ocb_ix*Ocb_ix+Ocb_iy*Ocb_iy)); //*lookup table
  Ocb_gx=((float)x2-x)/Ocb_l;
  Ocb_gy=((float)y2-y)/Ocb_l;
  Ocb_il=Ocb_l;
  for (Ocb_i=0;Ocb_i<Ocb_il;Ocb_i++){
    Ocb_x+=Ocb_gx; Ocb_y+=Ocb_gy;
    Ocb_ix=Ocb_x+0.5f; Ocb_iy=Ocb_y+0.5f;
    if ((bt[Ocb_iy][Ocb_ix]&8192)==0){
      if ((Ocb_ix==x2)&&(Ocb_iy==y2)) return FALSE;
      OBJcheckbolt_x=Ocb_ix; OBJcheckbolt_y=Ocb_iy;
      return TRUE;
    }
    if ((Ocb_ix==x2)&&(Ocb_iy==y2)) return FALSE; //*remove later
    //Ocbo=OBJnew();
    //Ocbo->type=2;
    //OBJadd(Ocb_ix,Ocb_iy,Ocbo);
  } //i loop
  return FALSE;
}

unsigned long SFnew(unsigned short x,unsigned short y){
  if (sfi[y>>3][x>>3]){
    sfn++;
    sf[sfn].next=sfi[y>>3][x>>3];
    sfi[y>>3][x>>3]=sfn;
  }else{
    sfn++;
    sfi[y>>3][x>>3]=sfn;
    sf[sfn].next=0;
  }
  return sfn;
}

unsigned long WTfind(object* obj){
  if (obj==NULL) return 0;
  WTf_i=0; WTf_n=0; WTf_w=0; WTf_itemn=0;
WTf_next:
  WTf_itemn++;
  WTf_w2=obji[sprlnk[obj->type&1023]].weight;
  if (obji[sprlnk[obj->type&1023]+(obj->type>>10)].flags&4096){
    WTf_w2=obji[sprlnk[obj->type&1023]].weight*obj->more2; //multiple objects
  }
  if (WTf_w2==0) return 0;
  WTf_w+=WTf_w2;
  //1024 container (general)
  if (obji[sprlnk[obj->type&1023]].flags&1024){
    if (obj->more){
      OBJtl[WTf_n]=(object*)obj->more; //add container to todo list
      WTf_n++;
    }
  }
  if ((obj->next!=NULL)&&(WTf_itemn!=1)) {obj=(object*)obj->next; goto WTf_next;}
  if (WTf_i<WTf_n){obj=OBJtl[WTf_i]; WTf_i++; goto WTf_next;}
  return WTf_w;
}

unsigned char houseowner(player *p,long housenumber){
  static long y;
  static npc *tnpc;
  houseowner_FAILVALUE=1;
  if (housenumber<=0) return 0; //range check
  houseowner_FAILVALUE=2;
  if (housenumber>=256) return 0;
  houseowner_FAILVALUE=3;
  if ((housesav[housenumber].flags&1)==0) return 0; //unoccupied
  //*

  houseowner_FAILVALUE=4;
  if (p->party[0]==NULL) return 0; //can't confirm character name
  houseowner_FAILVALUE=5;



  tnpc=(npc*)p->party[0]->more;
  for (y=0;y<=31;y++){
    if (y<p->name->l){
      if (p->name->d2[y]!=housesav[housenumber].username[y]) return 0;
    }else{
      if (housesav[housenumber].username[y]) return 0;
    }
    if (y<tnpc->name->l){
      if (tnpc->name->d2[y]!=housesav[housenumber].charactername[y]) return 0;
    }else{
      if (housesav[housenumber].charactername[y]) return 0;
    }
  }//y
  return 1;
}//houseowner

unsigned char housecheck(unsigned short x,unsigned short y){
  static short x6,y6;
  for (x6=1;x6<=255;x6++){ //scan all houses
    //check if x and y are inside the house.
    if (x>=housex1[x6]){ if (x<=housex2[x6]){ if (y>=housey1[x6]){ if (y<=housey2[x6]){
      for (y6=0;y6<housepnext[x6];y6++){
        if (x==housepx[x6][y6]){ if (y==housepy[x6][y6]){
          return(x6); //return house number.
        }
        }
      }}}}}
  }
  return(0);
}

//save housesav buffer to disk (with current date)
void housesav_update(){
  static txt *t=txtnew(),*t2=txtnew();
  static file *tfh;
  txtset(t2,"??"); t2->ds[0]=12345; //Not Crypted
  txtNEWLEN(t,-17408); memcpy(&t->d2[0],&housesav,17408);
  /*	luteijn: no encryption!
  *
  *      encrypt(t);
  */
  txtadd(t2,t);
  tfh=open2(".\\save\\house.sav",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
  put(tfh,&t2->d2[0],t2->l);
  close(tfh);
}

long CON_gv(long v){
  if (v==(65536+0)) return tplayer->NPCflags[CONnpc]&255; //FLG_JMP
  if (v==(65536+1)) return CONerr; //ERROR
  if (v==(65536+2)) return tnpc->schedule_last;
  if (v==(65536+3)) return (tplayer->NPCflags[CONnpc]&256)>>8; //FLG_NAME
  if (v==(65536+4)) return CONrnd; //RANDOM
  if (v==(65536+5)) return CONnumber; //NUMBER
  if (v==(65536+7)) return CONpartymember; //PARTYMEMBER
  if (v==(65536+8)) return tplayer->male_female; //MALEFEMALE
  if (v==(65536+9)) return (tplayer->NPCflags[CONnpc]&512)>>9; //FLG_INTRO
  if (v==(65536+10)) return (tplayer->NPCflags[CONnpc]&1024)>>10; //FLG_INTRO2
  if (v==(65536+11)) return (tplayer->NPCflags[CONnpc]&0xFF0000)>>16; //FLG_VALUE
  if (v==(65536+12)) return (tplayer->NPCflags[CONnpc]&0xFF000000)>>24; //FLG_VALUE2
  if (v==(65536+13)) return CONport; //PORT
  if (v==(65536+14)) return (tplayer->NPCflags[CONnpc]&2048)>>11; //FLG_KARMA
  if (v==(65536+15)) return tplayer->karma; //KARMA

  if (v==(65536+16)){
    if (CONpartymember){
      tnpc3=NULL;
      if (tplayer->party[CONpartymember-1]){
        tnpc3=(npc*)tplayer->party[CONpartymember-1]->more;
      }else{
        if (tplayer->party[0]) tnpc3=(npc*)tplayer->party[0]->more;
      }
      if (tnpc3) return tnpc3->port;
    }//CONpartymember
    return 187; //blank!
  }

  if (v==(65536+17)){ //PARTYMEMBER_LEVEL
    if (CONpartymember){ if (tplayer->party[CONpartymember-1]){
      tnpc3=(npc*)tplayer->party[CONpartymember-1]->more;
      return tnpc3->lev;
    }}
    return 0;
  }

  if (v==(65536+18)){ //RESURRECT_COST (returns value or 50 if it isn't sure...)
    static long x6,x5,y6,i3;
    for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
      tnpc3=(npc*)tplayer->party[x6]->more;
      x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
    }} //!=NULL,x6
    OBJlist_last=NULL;
    for(y6=0;y6<x5;y6++){
      if ((OBJlist_list[y6]->type&1023)==339){ //dead body
        //scan resurrect list
        for (i3=0;i3<=nresu;i3++){
          if (resu[i3]){
            if (resu_body[i3]==OBJlist_list[y6]){
              if (resu_player[i3]){ //has a player pointer
                //resurrection possible
                tnpc3=(npc*)resu[i3]->more;
                if (resu_partymember[i3]==0){
                  if (resu_player[i3]->GNPCflags[25]){
                    return 0;
                  }//25s
                }//resu_partymember==0
                return tnpc3->lev*50;
              }}}}
      }}
    return 50;
  }

  if (v==(65536+19)) return CONhousecost; //HOUSECOST

  if (v==(65536+20)){ //HOUSEBALANCE (returns an error if no house is owned)
    if (houseowner(tplayer,tplayer->GNPCflags[28])==0) {CONerr=1; return 0;}
    return housesav[tplayer->GNPCflags[28]].gold;
  }

  if (v==(65536+21)) return CONhouseinitialcost; //HOUSEINITIALCOST
  
#pragma region
  if (v == 65536+22)	// BULKPRICE
	return tplayer->bulkprice;
#pragma endregion Bulk sell patch, Xenkan 2010-12-04

  if ((v&0xFFFFFF00)==262144) return tplayer->GNPCflags[v&0xFF];
  if ((v&0xFFFFFF00)==131072) return CONreg[v&0xFF];

  return v;
}

void CON_sv(long v,long v2){ 
  v2=CON_gv(v2);
  if (v==(65536+0)){tplayer->NPCflags[CONnpc]&=0xFFFFFF00; tplayer->NPCflags[CONnpc]|=v2;} //FLG_JMP
  if (v==(65536+1)) CONerr=v2; //ERROR
  if (v==(65536+3)){tplayer->NPCflags[CONnpc]&=(0xFFFFFFFF-256); tplayer->NPCflags[CONnpc]|=(v2*256);} //FLG_NAME
  if (v==(65536+4)) {v2=rnd*(v2+1); CONrnd=v2;} //RANDOM
  if (v==(65536+5)) CONnumber=v2; //NUMBER
  if (v==(65536+6)) CONqual=v2; //CON_QUAL WRITEONLY!
  if (v==(65536+7)){
    CONpartymember=v2; //PARTYMEMBER
    //validate PARTYMEMBER (if invalid set to 0)
    if ((CONpartymember)&&(CONpartymember<=8)){
      if (!tplayer->party[CONpartymember-1]) CONpartymember=0;
    }//CONpartymember
  }//PARTYMEMBER
  if (v==(65536+9)){tplayer->NPCflags[CONnpc]&=(0xFFFFFFFF-512); tplayer->NPCflags[CONnpc]|=(v2*512);} //FLG_INTRO
  if (v==(65536+10)){tplayer->NPCflags[CONnpc]&=(0xFFFFFFFF-1024); tplayer->NPCflags[CONnpc]|=(v2*1024);} //FLG_INTRO2
  if (v==(65536+11)){tplayer->NPCflags[CONnpc]&=0xFF00FFFF; tplayer->NPCflags[CONnpc]|=(v2<<16);} //FLG_VALUE
  if (v==(65536+12)){tplayer->NPCflags[CONnpc]&=0x00FFFFFF; tplayer->NPCflags[CONnpc]|=(v2<<24);} //FLG_VALUE2
  if (v==(65536+13)) CONport=v2; //PORT
  if (v==(65536+14)){tplayer->NPCflags[CONnpc]&=(0xFFFFFFFF-2048); tplayer->NPCflags[CONnpc]|=(v2*2048);} //FLG_KARMA
  if (v==(65536+15)){
    if ((tplayer->NPCflags[CONnpc]&2048)==0){
      tplayer->NPCflags[CONnpc]|=2048;
      if (v2<tplayer->karma){
        if (stealing_txt==NULL) stealing_txt=txtnew();
        txtset(stealing_txt,"?");
        stealing_txt->d2[0]=8;
        txtadd(stealing_txt,"You feel less virtuous than before!");
        NET_send(NETplayer,tplayer->net,stealing_txt);
      }//v2<
      tplayer->karma=v2; //KARMA
    }//FLG_KARMA==0
  }

  if (v==(65536+19)){ //HOUSECOST
    if ((v2>=1)&&(v2<=255)){
      if ((housesav[v2].flags&1)==0){
        CONhousecost=housecost[v2];
        CONhouseinitialcost=houseinitialcost[v2];
        return;
      }
    }
    CONhousecost=0; //house not available
    CONhouseinitialcost=0;
  }

  if (v==(65536+20)){ //HOUSEBALANCE (returns an error if no house is owned)
    if (houseowner(tplayer,tplayer->GNPCflags[28])==0) {CONerr=1; return;}
    housesav[tplayer->GNPCflags[28]].gold=v2;
    housesav_update();
  }

  if ((v&0xFFFFFF00)==131072) CONreg[v&0xFF]=v2;
  if ((v&0xFFFFFF00)==262144) tplayer->GNPCflags[v&0xFF]=v2;
}

void stealing(player *tplayer,object *obj){
  static long x8,x9,s,x5,y6;
  static object *obj2;
  if (obj==NULL) return;
  s=0;
  x5=OBJlist(obj); //x5=last object number
  OBJlist_last=NULL;
  for(y6=0;y6<x5;y6++){
    obj2=OBJlist_list[y6];
    if (obj2->info&128){ //stealing!
      x8=1; for (x9=0;x9<=1023;x9++) {if (tplayer->sitem[x9]==obj2) x8=0;}
      if (x8){
        tplayer->sitem[tplayer->sitem_next++&1023]=obj2;
        if (obj2->type==88) {tplayer->karma-=((float)obj2->more2/16.0f); goto karma_spec;} //gold
        if (obj2->type==55) {tplayer->karma-=((float)obj2->more2/16.0f); goto karma_spec;} //arrows
        if (obj2->type==56) {tplayer->karma-=((float)obj2->more2/16.0f); goto karma_spec;} //bolts
        if (obji[sprlnk[obj2->type&1023]+(obj2->type>>10)].flags&4096){tplayer->karma-=((float)obj2->more2/4.0f); goto karma_spec;} //multiple
        tplayer->karma-=2;
karma_spec:
        s=1;
      }//x8
    }//&128
  }//y6
  if ((s)&&(stealing_MESSAGE==TRUE)){
    if (stealing_txt==NULL) stealing_txt=txtnew();
    txtset(stealing_txt,"?");
    stealing_txt->d2[0]=8;
    txtadd(stealing_txt,"You feel less virtuous than before!");
    NET_send(NETplayer,tplayer->net,stealing_txt);
  }//s
}

unsigned char stormcloakcheck(unsigned short x,unsigned short y){
  static short i,x2,y2;
  for (i=0;i<=stormcloak_last;i++){
    x2=abs(x-stormcloak_x[i]); y2=abs(y-stormcloak_y[i]);
    if (x2<=7){
      if (y2<=7){
        if (stormcloak_mask[y2][x2]) return(1);
      }}//x2,y2<=7
  }//i
  return(0);
}

unsigned char stormcloakcheck2(unsigned short x,unsigned short y,player* p){
  static short i,x2,y2;
  for (i=0;i<=stormcloak_last;i++){
    if (stormcloak_player[i]==p){ //field created by current player
      x2=abs(x-stormcloak_x[i]); y2=abs(y-stormcloak_y[i]);
      if (x2<=7){
        if (y2<=7){
          if (stormcloak_mask[y2][x2]) return(1);
        }}//x2,y2<=7
    }//p
  }//i
  return(0);
}

void inbritanniacheck(){
  if (!inbritannia) inbritannia=txtnew();
  static txt *t=txtnew(),*t2=txtnew();
  static long x,x2,x3;
  static npc *tnpc;
  static file *tfh;

  txtset(t,"");
  inbritannia_totalplayers=0;
  x2=0;
  for (x=0;x<=playerlist_last;x++){ //create and send UPDATE message
    if (playerlist[x]){ //current player
      if (playerlist[x]->net){ //net connection available



        if (playerlist[x]->party[0]){
          tnpc=(npc*)playerlist[x]->party[0]->more;
          if (x2) txtadd(t,", ");
          txtadd(t,tnpc->name);
          txtadd(t," (");
          txtnumint(t2,tnpc->lev); txtadd(t,t2);
          txtadd(t,")");
          x2=1;
          inbritannia_totalplayers++;
        }else{
          for (x3=0;x3<=nresu;x3++){
            if (resu[x3]){
              if (resu_player[x3]==playerlist[x]){
                if (resu_partymember[x3]==0){
                  tnpc=(npc*)resu[x3]->more;
                  if (x2) txtadd(t,", ");
                  txtadd(t,tnpc->name);
                  txtadd(t," (");
                  txtnumint(t2,tnpc->lev); txtadd(t,t2);
                  txtadd(t,", dead)");
                  x2=1;
                  inbritannia_totalplayers++;
                }}}}
        }


      }}}
  for (x=0;x<t->l;x++){
    if (t->d2[x]==34) t->d2[x]=39;
  }

  txtnumint(inbritannia,inbritannia_totalplayers);
  txtadd(inbritannia," in Britannia: ");
  txtadd(inbritannia,t);

  txtset(t2,"c:\\public_html\\who.jss");
  tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
  if (tfh->h!=HFILE_ERROR){
    txtset(t2,"document.write(\x022");
    txtadd(t2,inbritannia);
    txtadd(t2,"\x022);");
    txtfileout(t2,tfh);
    close(tfh);
  }//!=HFILE_ERROR

}//inbritanniacheck

unsigned char ENHANCEget_attack(object* obj){
  static long x2;
  //EXCEPTIONS!
  x2=obj->type&1023;
  if (obji[sprlnk[x2]].flags&(1+2+4+8+16+32+64)){ //can be worn
    if (obji[sprlnk[x2]].flags&4096) return 0; //multiple
    if (x2==78) return 0; //staff
    if (x2==79) return 0; //lightning
    if (x2==80) return 0; //fire
    if (x2==48) return 0; //glass sword
    if (x2==256) return 0; //protection ring
    if (x2==141) return 0; //decorative sword
    if (obji[sprlnk[x2]].v1){ //weapon
      return obj->more2&15;
    }
  }
  return 0;
}

unsigned char ENHANCEget_defense(object* obj){
  static long x2;
  //EXCEPTIONS!
  x2=obj->type&1023;
  if (obji[sprlnk[x2]].flags&(1+2+4+8+16+32+64)){ //can be worn
    if (obji[sprlnk[x2]].flags&4096) return 0; //multiple
    if (x2==78) return 0; //staff
    if (x2==79) return 0; //lightning
    if (x2==80) return 0; //fire
    if (x2==48) return 0; //glass sword
    if (x2==256) return 0; //protection ring
    if (x2==141) return 0; //decorative sword
    if (obji[sprlnk[x2]].v2){ //armour
      return (obj->more2&0xF0)>>4;
    }
  }
  return 0;
}


void OBJsave(unsigned short x,unsigned short y){
  static object *obj,*obj2,*obj3,*obj4;
  static long x2,x3,x4,x5,x8,y2,y8;
  //check if index already exists
  for (x2=0;x2<=objsave_last;x2++){
    if (objsave_obj[x2]){ //valid
      if ((objsave_x[x2]==x)&&(objsave_y[x2]==y)) return; //index already saved
    }//valid
  }//x2
  //add index
  for (x2=0;x2<=objsave_last;x2++){
    if (objsave_obj[x2]==NULL) goto objsave_freeindex;
  }
  if (objsave_last==(65536*4)) return;
  objsave_last++; x2=objsave_last;
objsave_freeindex:
  objsave_x[x2]=x; objsave_y[x2]=y; objsave_obj[x2]=NULL;

  objsave_wait[x2]=8192.0f+rnd*8192.0f; //default values ~2 to ~4h
  //objsave_wait[x2]=10.0f; //debug
  if ((237==x)&&(156==y)) objsave_wait[x2]=256.0f+rnd*256.0f; //default values

  //find something worth backing up!
  //ignore skiffs, dead bodies, and some other wierd stuff (how?)

  objsave_node_last=-1; obj4=NULL;
  if (obj=od[y][x]){
objsave_nextnode:

    //MessageBox(NULL,"node","Ultima 6 Online",MB_OK);

    obj3=NULL;
objsave_next:
    //add it?
    if ((obj->info&112)==0){//permanent object
      if ((obj->info&16384) && !(obji[sprlnk[obj->type&1023]].flags&1024)){//treasurechest object and not chest
        x4=randchestitem();
        x5=x4>>10;
        x4=x4&1023;
        obj->type=x4;
        obj->more2=x5;
        ENHANCEnewn(obj,4,4);
      }
      if ((obji[sprlnk[obj->type&1023]].weight)||(obji[sprlnk[obj->type&1023]].flags&1024)||((obj->type&1023)==OBJ_WEB)||((obj->type&1023)>=OBJ_FIRE_FIELD && (obj->type&1023)<=OBJ_SLEEP_FIELD)){ //weight or container, spider web or field
        //floating object with weight SHOULD BE ADDED, dont ignore floating
        //EXCEPTIONS!
        if ((obj->type&1023)==414) goto skipsaveobj; //skiff
        if (obj->info&256) goto skipsaveobj; //quest item
        if (obj==nuggetsfix) goto skipsaveobj; //newbie nuggets cannot respawn
        //add save obj

        //*** CONTAINER ERROR! ***
        obj2=OBJnew();
        obj2->type=obj->type;
        obj2->info=obj->info;
        obj2->prev=obj3; if (obj3) obj3->next=obj2; //link
        if (obj2->more2=obj->more2){ //!=NULL
          if (obji[sprlnk[obj->type&1023]].flags&1024){ //container
            objsave_node_last++;
            objsave_node[objsave_node_last]=obj2;
          }
        }
        ENHANCEnewn(obj2,8,4);

        if (obj4) {obj4->more=(object*)obj2; obj2->prev=obj4;}

        if ((obj3==NULL)&&(obj4==NULL)) objsave_obj[x2]=obj2;
        obj3=obj2; //set prev

        obj->info|=112; //set temp obj
        obj4=NULL;
        //NEWCODE
        if ((obj2->info&16384)&&((obj2->type&1023)==OBJ_CHEST)) { //random new location for treasure chest
          objsave_wait[x2]=1.0f; //respawn new chest immediately
          x8=randomchestlocation(false);
          y8=x8>>10;
          x8=x8&1023;
          x3=2+rnd*2;
          objsave_x[x2]=x8; objsave_y[x2]=y8; //set new random location
          //objsave_x[x2]=349; objsave_y[x2]=349; //debug location
          obj2->type=OBJ_CHEST+1024*x3;
        }
        //NEWCODEEND

      }//weight or container
    }//permanent object
skipsaveobj:
    if (obj=(object*)obj->next) goto objsave_next;
    if (objsave_node_last!=-1) {obj4=objsave_node[objsave_node_last]; obj=(object*)obj4->more; objsave_node_last--; goto objsave_nextnode;}
  }//obj (first)

}//OBJsave

//calculate mp_max of hired partymember based on INT
unsigned short get_mp_max(unsigned short i){ 
  switch (i) {
    case 0: 
      return 0;
    case 1:
    case 2:
      return 1;
    case 3:
    case 4:
      return 2;
    case 5:
    case 6:
      return 3;
    case 7:
    case 8:
      return 4;
    case 9:
    case 10:
      return 5;
    case 11:
    case 12:
      return 6;
    case 13:
    case 14:
      return 7;
    case 15:
    case 16:
      return 8;
    case 17:
      return 9;
    case 18:
      return 10;
    case 19:
      return 11;
    case 20:
      return 12;
    case 21:
      return 13;
    case 22:
      return 14;
    case 23:
      return 15;
    case 24:
      return 16;
    case 25:
      return 18;
    case 26:
      return 20;
    case 27:
      return 22;
    case 28:
      return 24;
    case 29:
      return 26;
    case 30:
      return 28;
    case 31:
      return 30;
    default: 
      return i;
  }
}

unsigned char objvisible(player *p,object* myobj){
  static long i2;
  static object *myobj2;
  i2=myobj->type&1023; //get object identity
  if (myobj->info&256){//quest
    if (i2==73){ //moonstone
      if (p->GNPCflags[4]&(1<<(myobj->type>>10))) return 0;
    }
    if (myobj->type==(275+2*1024)){//yellow potion
      if (p->GNPCflags[32]&myobj->more2) return 0;
    }//yellow potion
    if ((i2>=242)&&(i2<=249)){ //rune
      if (p->GNPCflags[5]&(1<<(i2-242))) return 0;
    }
    if ((i2>=400)&&(i2<=407)){ //map(0-7)
      if (p->GNPCflags[7]&(1<<(i2-400))) return 0;
    }
    if (i2==62){ //vortex cube
      if ((myobj->x==923)&&(myobj->y==850)){
        if ((p->GNPCflags[18]&1)==0) return 0;
      }
      if ((myobj->x==1939)&&(myobj->y==57)){
        if (p->GNPCflags[6]&(1<<1)) return 0;
      }
    }
    if (i2==389){ //silver tablet
      if (p->GNPCflags[6]&(1<<2)) return 0;
    }
    if (i2==390){ //silver tabletII
      if (p->GNPCflags[6]&(1<<3)) return 0;
    }
    if (i2==270){ //balloon plans
      if (p->GNPCflags[6]&(1<<4)) return 0;
    }
    if (i2==395){ //broken gargoyle lens
      if (p->GNPCflags[6]&(1<<5)) return 0;
    }
    if (i2==394){ //brit lens
      if ((p->GNPCflags[18]&2)==0) return 0;
    }
    if (i2==396){ //garg lens
      if ((p->GNPCflags[18]&4)==0) return 0;
    }
  }//quest
  if (myobj==nuggetsfix){ //nuggets
    if (p->online<1536) return 0;
    if (p->GNPCflags[6]&(1<<6)) return 0;
  }
  if (i2==59){ //codex
    if (p->GNPCflags[18]&8) return 0;
  }
  if (i2==51){ //force field
    myobj2=(object*)myobj->prev;
    if (p->GNPCflags[3]&(1<<(myobj2->type>>10))) return 0;
  }//force field
  if (i2==173){ //trap
    if ((myobj->info&(1<<9))==0) return 0;
  }//trap
  return 1;
}//objvisible

long roundfloat(float f){ 
  __asm {
    fld f
      fistp roundfloat_l
  }
  return roundfloat_l;
}

void getwindspell(long sx,long sy,long dx,long dy){
  static long windspell_i[128];
  static long x,x2,x3,x4;
  static long y,y2,y3,y4;
  static long z,z2,z3,z4;
  static float fx,fx2,fx3,fx4,fy,fy2,fy3,fy4,f,f2,f3,f4;
  WINDSPELL_boltn=-1; WINDSPELL_n=-1;
  z=abs(dx-sx); z2=abs(dy-sy); if (z>z2) z3=z; else z3=z2;
  f=z3;
  if (z3==0) return;
  if (z3==1){
    //3 point fan
    if (z&&(z2==0)){//3 vertical
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy-1;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy+1;
    }
    if (z2&&(z==0)){//3 horizontal
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx-1; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx+1; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
    }
    if (z2&&z){//3 diagonally
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=sx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=sy;
    }
  }else{
    //5 point fan
    x3=abs(dx-sx); y3=abs(dy-sy);
    x=0; y=0;
    if (x3>=y3) x=1;
    if (y3>=x3) y=1;
    if (y3>=(x3/2)){
      if (x3>=(y3/2)){
        x=1;
        y=1;
      }
    }
    if (x&&(y==0)){//5 vertical
      z4=1; if (dx>sx) z4=-1;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx+z4; WINDSPELL_bolty[WINDSPELL_boltn]=dy-2;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy-1;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy+1;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx+z4; WINDSPELL_bolty[WINDSPELL_boltn]=dy+2;
    }
    if (y&&(x==0)){//5 horizontal
      z4=1; if (dy>sy) z4=-1;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx-2; WINDSPELL_bolty[WINDSPELL_boltn]=dy+z4;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx-1; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx+1; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx+2; WINDSPELL_bolty[WINDSPELL_boltn]=dy+z4;
    }
    if (x&&y){//5 diagonally
      //...
      x4=-1; if (dx>sx) x4=1;
      y4=-1; if (dy>sy) y4=1;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx; WINDSPELL_bolty[WINDSPELL_boltn]=dy;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx+x4; WINDSPELL_bolty[WINDSPELL_boltn]=dy-y4;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx-x4; WINDSPELL_bolty[WINDSPELL_boltn]=dy+y4;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx+x4; WINDSPELL_bolty[WINDSPELL_boltn]=dy-y4*2;
      WINDSPELL_boltn++; WINDSPELL_boltx[WINDSPELL_boltn]=dx-x4*2; WINDSPELL_bolty[WINDSPELL_boltn]=dy+y4;
    }
  }//3 or 5 point fan

  //remove out-of-map wind lines
  z3=0;
  z2=WINDSPELL_boltn;
  for (z=0;z<=z2;z++){
    x=WINDSPELL_boltx[z]; y=WINDSPELL_bolty[z];
    if ((x>=0)&&(x<=2047)&&(y>=0)&&(y<=1023)){
      WINDSPELL_boltx[z3]=x; WINDSPELL_bolty[z3]=y;
      z3++;
    }else{
      WINDSPELL_boltn--;
    }
  }//z
  if (WINDSPELL_boltn==-1) return;

  //get xy locations
  f*=8.0f;
  for (z=0;z<=WINDSPELL_boltn;z++){
    fx=sx; fy=sy;
    fx2=(float(WINDSPELL_boltx[z]-sx))/f; fy2=(float(WINDSPELL_bolty[z]-sy))/f;
    x2=sx; y2=sy;
windspelllineloop:
    fx+=fx2; fy+=fy2;
    x=roundfloat(fx); y=roundfloat(fy);
    if ((x!=x2)||(y!=y2)){
      fx3=fabs(fx-x); fy3=fabs(fy-y); if ((fx3<=0.4375)||(fy3<=0.4375)){//non-corner check
        WINDSPELL_n++; WINDSPELL_x[WINDSPELL_n]=x; WINDSPELL_y[WINDSPELL_n]=y; windspell_i[WINDSPELL_n]=0;
        if ((x==WINDSPELL_boltx[z])&&(y==WINDSPELL_bolty[z])) goto windspelllinedone;
        if ((bt[y][x]&8192)==0){//not bolt passable
          WINDSPELL_boltx[z]=x; WINDSPELL_bolty[z]=y; goto windspelllinedone;
        }
        x2=x; y2=y;
      }//non-corner check
    }
    goto windspelllineloop;
windspelllinedone:;
  }//z
  //enumerate each xy location
  z2=0;
windspell_z2loop:
  if (windspell_i[z2]==0){
    x=WINDSPELL_x[z2]; y=WINDSPELL_y[z2];
    windspell_i[z2]=1;
    z3=2;
    for (z=z2+1;z<=WINDSPELL_n;z++){
      if ((WINDSPELL_x[z]==x)&&(WINDSPELL_y[z]==y)){
        windspell_i[z]=z3; z3++;
      }//x&&y
    }//z
  }//z2
  z2++; if (z2<=WINDSPELL_n) goto windspell_z2loop;
  //eliminate all xy locations which aren't the 1st or 3rd
  for (z=0;z<=WINDSPELL_n;z++){
    if ((windspell_i[z]!=1)&&(windspell_i[z]!=3)) WINDSPELL_x[z]=0;
  }//z
}//getwindspell

DWORD WINAPI revive_infiniteloopexit(LPVOID null_value){
revive_infiniteloopexit_loop:
  SleepEx(1000,FALSE);
  if (revive_infiniteloopexit_i==revive_infiniteloopexit_i2) revive_infiniteloopexit_i3++; else revive_infiniteloopexit_i3=0;
  if (revive_infiniteloopexit_i3>=128){
    u6orevive=254; seek(u6orevive_fh,0); put(u6orevive_fh,&u6orevive,1);
    //put(u6orevive_fh,&DEBUGlastline,4);
    put(u6orevive_fh,&u6opl,4);//ALWAYS 0 (left for compatibiliy with previous versions of revive)
    exit(0);
  }
  revive_infiniteloopexit_i2=revive_infiniteloopexit_i;
  goto revive_infiniteloopexit_loop;
  return 0;
}

//returns player->party[0] or resu[...] if dead
object *getprimarypartymemberobj(player *sourceplayer){
  if (sourceplayer->party[0]) return sourceplayer->party[0];
  static unsigned long i;
  for (i=0;i<=nresu;i++){
    if (resu[i]){
      if (resu_player[i]==sourceplayer){
        if (resu_partymember[i]==0){
          return resu[i];
        }}}}
  return NULL;
}

//see getprimarypartymemberobj
object *getpartymemberobj(player *sourceplayer,unsigned char n){
  if (sourceplayer->party[n]) return sourceplayer->party[n];
  static unsigned long i;
  for (i=0;i<=nresu;i++){
    if (resu[i]){
      if (resu_player[i]==sourceplayer){
        if (resu_partymember[i]==n){
          return resu[i];
        }}}}
  return NULL;
}

//used for crts with monster-drop inventories to determine the extra armour they could be using
long getequiparmourvalue(object* obj){
  static long armourmask,armourvalue;
  if (!obj) return 0;
  armourvalue=0;
  armourmask=0;//used stop usaage of 2 or more similar armour types (eg. 2 helms)
getequiparmourvalue_next:
  if (obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64)){//equipable
    if ((obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64))&armourmask) goto getequiparmourvalue_alreadyusing;
    if (obji[sprlnk[obj->type&1023]].v2){
      armourvalue+=(obji[sprlnk[obj->type&1023]].v2+ENHANCEget_defense(obj));
      armourmask|=(obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64));
    }//v2
  }//equippable flags
getequiparmourvalue_alreadyusing:
  if (obj=(object*)obj->next) goto getequiparmourvalue_next;
  return armourvalue;
}


long getequiph2hwepdamage(object* obj){
  static long weapondamage;
  if (!obj) return 0;
  weapondamage=0;
getequiph2hwepdamage_next:

  /*
  if (obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64)){//equipable
  if ((obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64))&armourmask) goto getequiparmourvalue_alreadyusing;
  if (obji[sprlnk[obj->type&1023]].v2){
  armourvalue+=(obji[sprlnk[obj->type&1023]].v2+ENHANCEget_defense(obj));
  armourmask|=(obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64));
  }//v2

  }//equippable flags
  getequiparmourvalue_alreadyusing:
  */

  if (obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64)){//equipable?
    if (obji[sprlnk[obj->type&1023]].v1){//can do damage
      if ((obji[sprlnk[obj->type&1023]].v1+ENHANCEget_attack(obj))>weapondamage){//greater than currently known weapon?

        //eliminate certain types
        if ((obj->type&1023)==33) goto getequiph2hwepdamage_skip;//sling
        if ((obj->type&1023)==41) goto getequiph2hwepdamage_skip;//bow
        if ((obj->type&1023)==42) goto getequiph2hwepdamage_skip;//crossbow
        if ((obj->type&1023)==49) goto getequiph2hwepdamage_skip;//boomerang
        if ((obj->type&1023)==50) goto getequiph2hwepdamage_skip;//triple crossbow!
        if ((obj->type&1023)==49) goto getequiph2hwepdamage_skip;//boomerang
        if ((obj->type&1023)==54) goto getequiph2hwepdamage_skip;//magic bow
        weapondamage=(obji[sprlnk[obj->type&1023]].v1+ENHANCEget_attack(obj));
      }
    }
  }
getequiph2hwepdamage_skip:
  if (obj=(object*)obj->next) goto getequiph2hwepdamage_next;
  return weapondamage;
}

//the purpose of this function is to:
//1. spread out party after using a boat, balloon, ladder, etc.
//2. fix certain piling up bugs
//3. remove any prerecorded paths
//all alive partymembers are assumed to be in a removed but existant state
void partyadd(player *p,long x,long y){
  static long x2,y2,x3,y3,z,z2,i;
  static object *obj;
  static npc *tnpc;
  ZeroMemory(&partyadd_checkarray,sizeof(partyadd_checkarray));
  //prepare array of checkable places
  for (y2=-2;y2<=2;y2++){ for (x2=-2;x2<=2;x2++){
    x3=x+x2; y3=y+y2; if ((x3>=0)&&(x3<=2047)&&(y3>=0)&&(y3<=1023)){
      if (bt[y3][x3]&1024){//walkable location
        partyadd_checkarray[x2+3][y2+3]=1;
      }
    }//boundaries check
  }}
  partyadd_checkarray[1][1]=0; partyadd_checkarray[5][1]=0; partyadd_checkarray[5][5]=0; partyadd_checkarray[1][5]=0;
  z2=0;
  for (i=0;i<=7;i++){
    if (obj=p->party[i]){
      tnpc=(npc*)obj->more;
      if (obj->x||obj->y){//remove if not removed!
        OBJremove(obj);
        if (tnpc->horse) OBJremove((object*)tnpc->more);
      }

      if (tnpc->horse){
        if (partyadd_checkarray[3][2]==1){//up
          partyadd_checkarray[3][2]=2; x2=x; y2=y-1; goto partyadd_foundlocation2;
        }
        if (partyadd_checkarray[4][3]==1){//right
          partyadd_checkarray[4][3]=2; x2=x+1; y2=y; goto partyadd_foundlocation2;
        }
        if (partyadd_checkarray[3][4]==1){//down
          partyadd_checkarray[3][4]=2; x2=x; y2=y+1; goto partyadd_foundlocation2;
        }
        if (partyadd_checkarray[2][3]==1){//left
          partyadd_checkarray[2][3]=2; x2=x-1; y2=y; goto partyadd_foundlocation2;
        }
        if (partyadd_checkarray[3][2]==2){//up
          partyadd_checkarray[3][2]=2; x2=x; y2=y-1; goto partyadd_foundlocation2;
        }
        if (partyadd_checkarray[4][3]==2){//right
          partyadd_checkarray[4][3]=2; x2=x+1; y2=y; goto partyadd_foundlocation2;
        }
        if (partyadd_checkarray[3][4]==2){//down
          partyadd_checkarray[3][4]=2; x2=x; y2=y+1; goto partyadd_foundlocation2;
        }
        if (partyadd_checkarray[2][3]==2){//left
          partyadd_checkarray[2][3]=2; x2=x-1; y2=y; goto partyadd_foundlocation2;
        }
        //No good location was found! So place horse facing right from left square
        OBJadd(x-1,y,obj);
        partyadd_checkarray[2][3]=2; x2=x; y2=y; goto partyadd_foundlocation3;
      }//horse

      if (z2){
        z=128;//try up to 128 times then give up
partyadd_nextlocation:
        if (z>=92){
          x3=rnd*3; y3=rnd*3; x3+=2; y3+=2;
        }else{
          x3=rnd*5; y3=rnd*5; x3++; y3++;
        }
        if (partyadd_checkarray[x3][y3]==1){//possible
          if ((partyadd_checkarray[x3+1][y3]==2)||(partyadd_checkarray[x3-1][y3]==2)||(partyadd_checkarray[x3][y3+1]==2)||(partyadd_checkarray[x3][y3-1]==2)){//next to another partymember
            x2=x+x3-3; y2=y+y3-3;
            goto partyadd_foundlocation;
          }//next to another partymember
        }//possible
        z--; if (z) goto partyadd_nextlocation;
      }
      x2=x; y2=y; x3=3; y3=3;
partyadd_foundlocation:
      partyadd_checkarray[x3][y3]=2;
      //add at x2,y2
partyadd_foundlocation2:
      OBJadd(x,y,obj);
partyadd_foundlocation3:
      if ((x!=x2)||(y!=y2)){
        OBJmove_allow=TRUE;
        OBJmove2(obj,x2,y2);
        OBJmove_allow=FALSE;
      }//((x!=x2)||(y!=y2))
      z2++;
      tnpc->pathn=0;
    }//->party[i]
  }//i
}//partyadd()


void horsedismount(object *myobj){//forces npc to dismount horse
  //variable names are the same as those used for a "normal" dismount
  static npc *tnpc2;
  static object *myobj5,*myobj6;
  HORSEDISMOUNT_HORSEOBJECT=NULL;
  tnpc2=(npc*)myobj->more;
  if (tnpc2->horse==NULL) return;
  //restore the horses head
  myobj5=tnpc2->horse; tnpc2->horse=NULL;
  OBJadd(myobj->x,myobj->y,myobj5);
  if ((myobj->type&1023)==412) {
    myobj5->type=myobj->type-431+430+19;
  }
  else {
    myobj5->type=myobj->type-431+430;
  }
  HORSEDISMOUNT_HORSEOBJECT=myobj5;
  //restore the horses tail
  myobj6=(object*)tnpc2->more;
  myobj6->more=myobj5;
  myobj6->type=myobj6->type-431+430;
  //restore the partymember
  tnpc2->more=NULL;
  myobj->type=tnpc2->originalobjtype; tnpc2->originalobjtype=NULL;
}

unsigned char usinghorse(player *tplayer){
  static long i;
  static npc *tnpc;
  for (i=0;i<=7;i++){
    if (tplayer->party[i]){
      tnpc=(npc*)tplayer->party[i]->more;
      if (tnpc->horse) return 1;
    }//tplayer->party[i]
  }//i
  return 0;
}

void additem(npc* tnpc,object* obj){
  //adds an item to an npc's currently open bag (or root inventory if no bag is open)
  //note: item must be removed first!
  static object *baseitem,*baseitemmore;
  if (obj->next||obj->prev) exit(501);//ERROR! Item to add has not been removed correctly!
  baseitem=tnpc->baseitem;
  baseitemmore=(object*)baseitem->more;
  if (baseitemmore){
    obj->next=baseitemmore; baseitemmore->prev=obj;
  }else{
    obj->next=NULL;
  }
  obj->prev=baseitem; baseitem->more=obj;
}

void additemroot(npc* tnpc,object* obj){
  //adds an item to an npc's ROOT inventory regardless of which bag is open
  //note: item must be removed first!
  static object *baseitem,*baseitemmore;
  if (obj->next||obj->prev) exit(502);//ERROR! Item to add has not been removed correctly!
  baseitem=tnpc->items;
  baseitemmore=(object*)baseitem->more;
  if (baseitemmore){
    obj->next=baseitemmore; baseitemmore->prev=obj;
  }else{
    obj->next=NULL;
  }
  obj->prev=baseitem; baseitem->more=obj;
}


object * npc_to_obj(npc * tnpc,player * tplayer){ // find the object related to the npc, e.g. to use the x,y coordinates.
  int i;
  if (tplayer) {//check all npcs in party 
    for (i=0;i<=7;i++){ 
      if (tplayer->party[i]){
        if (tnpc==(npc*)tplayer->party[i]->more) {
          return tplayer->party[i];
        }
      }
    }
  }
  // TODO: NULL player passed, have to check all objects to find one that refers to the NPC
  return NULL;
}

unsigned long randomchestlocation(bool tmap){ //randomizes overland random treasurechest location and treasuremap chest locations
  static short x3,x2,y2;
  static long x8,y8;
  x3=1;
  while (x3) {
    x3=0;
    x2=rnd*15;
    y2=rnd*15;
    if (((x2<=1 || x2>=13) && y2==0) || ((x2==0 || x2==14 ) && y2==1) || ((x2==11 || x2==12) && y2==4)) { //no valid spawn locations (or very few of them)
      x3=1;
    }
    else if (((x2==10 || x2==11) && y2==5) || ((x2>=7 && x2<=11) && y2==6) || ((x2>=7 && x2<=12) && y2==7) ) { //splitting up to make it less messy
      x3=1;
    }
    else if (((x2==0 || x2==6 || x2==7 || (x2>=9 && x2<=12)) && y2==8) || ((x2<=1 || x2==6 || x2==9 || x2>=11 ) && y2==9)) {
      x3=1;
    }
    else if (((x2==0 || (x2>=7 && x2<=9) || x2>=11) && y2==10) || ((x2<=2 || (x2>=7 && x2<=12)) && y2==11)) {
      x3=1;
    }
    else if (((x2==0 || x2==3 || (x2>=7 && x2<=9) || x2==11 || x2==12) && y2==12) || ((x2==0 || x2==3 || x2==9 || x2==10 || x2==12) && y2==13) ) {
      x3=1;
    }
    else if ((x2==0 || x2==3 || x2==4) && y2==14) {
      x3=1;
    }
  }
  x8=32+x2*64+(unsigned short)(rnd*64);
  y8=26+y2*64+(unsigned short)(rnd*64);
  //two different while loop checks for treasure map and the overland random treasure chest
  while ((!tmap&&(bt[y8][x8]&1024)==0)||(tmap&&(bt[y8][x8]&1024)==0)||!(((bt[y8][x8]&1023)==1)||(((bt[y8][x8]&1023)>=52)&&((bt[y8][x8]&1023)<64))||(((bt[y8][x8]&1023)>=73)&&((bt[y8][x8]&1023)<96))||(((bt[y8][x8]&1023)>=108)&&((bt[y8][x8]&1023)<112)))||((x8==707)&&(y8==835))) {
    x8=32+x2*64+(unsigned short)(rnd*64);
    y8=26+y2*64+(unsigned short)(rnd*64);
  }

  /*x8=408+(unsigned short)(rnd*57)-28;
  y8=535+(unsigned short)(rnd*45)-22;
  while ((bt[y8][x8]&1024)==0) {
  x8=408+(unsigned short)(rnd*57)-28;
  y8=535+(unsigned short)(rnd*45)-22;
  }*/
  //x8=349;
  //y8=349;
  x8=x8+y8*1024;
  return(x8);
}
  /*static unsigned long list[500]; // for debugging
  static unsigned long list2[16]; // for debugging
  unsigned char init=0;*/
unsigned long randchestitem(){ //random item types and quantity for treasure chest contents
  static short x5;
  static long x4;
  //x4=rnd*4;
  x4=rnd*9; //make wep & armour more common
  x5=0;
  switch (x4) { //random new item type
  case 0: //any armour
  case 4:
  case 6:
  case 8:
    x4=rnd*25;
    if (x4==0) {x4=27;}
    break;
  case 1: //any weapon (excluding wands, staves, glass swords and rollingpins, knives etc)
  case 5:
  case 7:
    x4=33+rnd*18;
    if (x4==48) {x4=54;}
    break; 
  case 2: //magic stuff (rings,wands etc)
    x4=78+rnd*8;
    if (x4==82) {x4=48;}
    if (x4>82) {x4+=173;}
    break; 
  case 3: //gold
    x5=rnd*3;
    x4=88+x5;
    x5=rnd*1000/(x5*10+1)+1;
    if (x4==90) {x4=77;}
    break;
  }
  //for testing the loot spread
  /*if (!init) {
      ZeroMemory(list,sizeof(list));
      ZeroMemory(list2,sizeof(list2));
      init=1;
  }
  if(x4<500){
    list[x4]++;
  }*/
  x4=x4+1024*x5;
  return (x4);
}

void ENHANCEnewn(object* obj, unsigned short n, unsigned short n2){
  static long x,x2,x3;
  static unsigned long temp;
  //EXCEPTIONS!
  x2=obj->type&1023;
  if (obji[sprlnk[x2]].flags&(1+2+4+8+16+32+64)){ //can be worn
    if (obji[sprlnk[x2]].flags&4096) return; //multiple
    if (x2==78) return; //staff
    if (x2==79) return; //lightning
    if (x2==80) return; //fire
    if (x2==48) return; //glass sword
    if (x2==256) return; //protection ring
    if (x2==141) return; //decorative sword
    if (x=obji[sprlnk[x2]].v2){ //armour
      obj->more2&=0xFFFFFF00;
      x=0;
      temp=0;
      x2=rnd*n; 
      if (x2==0){
        x++;
        x3=1;
        while ((x2==0)&&(x!=15)) {
          temp=n2;
          x3++;
          while ((temp < 32769)&&(x3!=15)) {
            temp*=n2;
            x3++;
          }
          x2=rnd*temp;
          while ((x2 < temp/n2)&&(x!=15)) {
            x++;
            temp/=n2;
          }
        }
      }//x2
      obj->more2|=(x<<4);
      //list2[x]++; //for debugging
      return;
    }//armour
    if (x=obji[sprlnk[x2]].v1){ //weapon
      obj->more2&=0xFFFFFF00;
      x=0;
      x2=rnd*n;
      if (x2==0){
        x++;
        x3=1;
        while ((x2==0)&&(x!=15)) {
          temp=n2;
          x3++;
          while ((temp < 32769)&&(x3!=15)) {
            temp*=n2;
            x3++;
          }
          x2=rnd*temp;
          while ((x2 < temp/n2)&&(x!=15)) {
            x++;
            temp/=n2;
          }
        }
      }//x2
      //list2[x]++; //for debugging
      obj->more2|=x;
    }//weapon
  }//can be worn
}//ENHANCEnew

/* adds new hireling npc to specified position, if there is a hireling already in that position do nothing*/
void addhireling(unsigned long x3, schedule_i *sched) {
  static unsigned long x,x2,x4,x5,x6;
  static object *myobj,*myobj2;
  static file *tfh;
  static txt *t=txtnew();
  ///NEW npc
  if ((x3 > HIRELINGS_MAX)||(hirl_obj[x3])) {return;}
  myobj=OBJnew();
  x6=x3;
  x2=rnd*2; //x2=0 male 1=female
  myobj->type=OBJ_AVATAR; //type
  OBJmove_allow=TRUE;
  if (x3>=11) {x3=rnd*11;} //
  if (x3<=1) {OBJmove2(myobj,383,406);}  //add to map, <-needs NPC pointer //britain
  else if (x3==2) {OBJmove2(myobj,404,589);} //paws
  else if (x3==3) {OBJmove2(myobj,423,733);} //trinsic
  else if (x3==4) {OBJmove2(myobj,157,847);} //jhelom
  else if (x3==5) {OBJmove2(myobj,592,930);} //serpent's hold
  else if (x3==6||x3==7) {OBJmove2(myobj,926,527);} //moonglow
  else if (x3==8) {OBJmove2(myobj,567,615);} //buccaneer's den
  else if (x3==9) {OBJmove2(myobj,739,691);} //new magincia
  else if (x3==10) {OBJmove2(myobj,233,152);} //yew
  OBJmove_allow=FALSE;
  tnpc=(npc*)malloc(sizeof(npc)); ZeroMemory(tnpc,sizeof(npc));

  myobj->more=tnpc; myobj->info|=2; //<-


  tnpc->port=0;
  //choose port based on x2
  x3=rnd*6;
  if (x2){
    if (x3==0) tnpc->port=194;
    if (x3==1) tnpc->port=195;
    if (x3==2) tnpc->port=197;
    if (x3==3) tnpc->port=198;
    if (x3==4) tnpc->port=202;
    if (x3==5) tnpc->port=203;
  }else{
    if (x3==0) tnpc->port=196;
    if (x3==1) tnpc->port=199;
    if (x3==2) tnpc->port=200;
    if (x3==3) tnpc->port=201;
    if (x3==4) tnpc->port=204;
    if (x3==5) tnpc->port=205;
  }



  tnpc->converse=201;
  tnpc->name=txtnew();

  //select name for character
  //enumerate names
  x3=0;
  if (x2) tfh=open(".\\host\\female.txt"); else tfh=open(".\\host\\male.txt");
enunam_next: txtfilein(t,tfh); 
  if (t->l) {x3++; goto enunam_next;}
  close(tfh);
  x3=rnd*x3;
  if (x2) tfh=open(".\\host\\female.txt"); else tfh=open(".\\host\\male.txt");
  for (x=0;x<=x3;x++){
    txtfilein(tnpc->name,tfh);
  }
  close(tfh);

  if (!sched) {
    newschedule2++;
    for (x=0;x<=31;x++) schedule2[newschedule2][x].hour=255;
    schedule2[newschedule2][0].hour=1;
    schedule2[newschedule2][0].x=myobj->x;
    schedule2[newschedule2][0].y=myobj->y;
    schedule2[newschedule2][0].type=0x8F;
    tnpc->schedule=&schedule2[newschedule2][0];
  }
  else {
    sched->x=myobj->x;
    sched->y=myobj->y;
    tnpc->schedule=sched;
  }
  tnpc->schedule_i=-1; //unknown
  tnpc->order=2; //schedule

  //create an INVISIBLE container for player's items
  myobj2=OBJnew();
  myobj2->type=OBJ_BAG; //bag (INVISIBLE)
  tnpc->items=myobj2;
  tnpc->baseitem=myobj2;

  tnpc->upflags|=1; //full update

  //8 base on all stats + 16 (cheap NPC)
  //16 base on all stats + 16 (high NPC)

  x4=rnd*2;
  if (x4) {tnpc->s=16; tnpc->d=16; tnpc->i=16;} else {tnpc->s=8; tnpc->d=8; tnpc->i=8;}
  for (x=0; x<=15; x++){
    x5=rnd*3;
    if (x5==0) tnpc->s++;
    if (x5==1) tnpc->d++;
    if (x5==2) tnpc->i++;
  }
  tnpc->hp=30; tnpc->hp_max=30;
  tnpc->lev=1; tnpc->exp=256+rnd*256+rnd*(x4*512);

  tnpc->mp_max=get_mp_max(tnpc->i); tnpc->mp=tnpc->mp_max;

  tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;

  static unsigned short bodytype[16];

  if (x2){ //female
    bodytype[0]=OBJ_FIGHTER;
    bodytype[1]=OBJ_SWASHBUCKLER;
    bodytype[2]=OBJ_MAGE;
    bodytype[3]=OBJ_FARMER;
    bodytype[4]=OBJ_MUSICIAN;
    bodytype[5]=OBJ_WOMAN;
    if ((tnpc->s<tnpc->s)&&(tnpc->s<tnpc->d)) bodytype[0]=0;
    if (x4) {bodytype[2]=0; bodytype[3]=0;}
    if ((tnpc->d<tnpc->s)&&(tnpc->d<tnpc->i)) bodytype[4]=0;
fbodytype_retry: x5=rnd*6; if (!bodytype[x5]) goto fbodytype_retry;
    myobj->type=bodytype[x5];
  }else{ //male
    bodytype[0]=OBJ_FIGHTER;
    bodytype[1]=OBJ_SWASHBUCKLER;
    bodytype[2]=OBJ_MAGE;
    bodytype[3]=OBJ_PEASANT;
    bodytype[4]=OBJ_FARMER;
    bodytype[5]=OBJ_MUSICIAN;
    bodytype[6]=OBJ_AVATAR;
    if ((tnpc->s<tnpc->s)&&(tnpc->s<tnpc->d)) bodytype[0]=0;
    if ((tnpc->i<tnpc->s)&&(tnpc->i<tnpc->d)) bodytype[2]=0;
    if (x4) {bodytype[3]=0; bodytype[4]=0;} else {bodytype[6]=0;}
    if ((tnpc->d<tnpc->s)&&(tnpc->d<tnpc->i)) bodytype[5]=0;
mbodytype_retry: x5=rnd*7; if (!bodytype[x5]) goto mbodytype_retry;
    myobj->type=bodytype[x5];
    /*
    Honesty (Truth - Mage) i3
    Compassion (Love - Bard)  d3
    Valor (Courage - Fighter) s3
    Justice (Truth and Love - Druid) s1 i1
    Sacrifice (Love and Courage - Tinker) s1 d1
    Honor (Truth and Courage - Paladin) d1 i1
    Spirituality (Truth and Love and Courage - Ranger) s1 d1 i1
    Humility (None - Shepherd) N/A
    #define U6O_HONESTY tnpc->i+=3; mage, woman(with dress)
    #define U6O_COMPASSION tnpc->d+=3; bard(aka iolo)
    #define U6O_VALOR tnpc->s+=3; fighter,fighter
    #define U6O_JUSTICE tnpc->s+=1; tnpc->i+=1;
    #define U6O_SACRIFICE tnpc->s+=1; tnpc->d+=1; aka shamino
    #define U6O_HONOR tnpc->d+=1; tnpc->i+=1;
    #define U6O_SPIRITUALITY tnpc->s+=1; tnpc->d+=1; tnpc->i+=1;
    #define U6O_HUMILITY farmer
    */
  }
  hirl_obj[x6]=myobj;
  hirl_wait[x6]=8192.0f+rnd*8192.0f; //default values ~2 to ~4h
  //hirl_wait[x6]=150.0f+rnd*150.0f; //debug
}

void crtrespawn(object *obj) {
  static int x2;
  static creature* crt;
  crt=(creature*)obj->more;
  OBJmove_allow=TRUE;
  for (x2=0;x2<=15;x2++){ //2004 fix for silver serpents
    OBJmove2(obj,DEATHPOSX+x2,DEATHPOSY);
  }
  OBJmove_allow=FALSE;
  //obj will now be destroyed
  VLNKremove(obj);
  VLNKsremove(obj);

  if (crt->respawn_hp){
    //respawn info
    for (x2=0;x2<=(respawn_last+1);x2++){
      if (respawn[x2]==NULL){
        respawn[x2]=obj;
        respawn_delay[x2]=1200; //~34 min
        respawn_delay[x2]+=1200; //+~17 min
        if (x2>respawn_last) {
          respawn_last++;
        }
        goto respawn_added;
      }
    }
respawn_added:;
  }
  else { /* free summoned creatures and split slimes*/
    free(crt);
    OBJremove(obj); OBJrelease(obj);
  }
}


// c111 host functions
unsigned int new1_getexpdeduction(npc* npc, int option) {
	unsigned short level = npc->lev;
	unsigned int deductexp;
	int currentlevelexp;
	float deductpercent;

	if (level <= 3) {
		if (option == 1)
			deductpercent = (float) 1 / 16;
		else
			deductpercent = (float) 1 / 8;

		deductexp = npc->exp * deductpercent;
	} else if (level > 3) {
		if (level >= 8) {
			if (option == 1)
				deductpercent = (float)1 / 2;
			else
				deductpercent = (float)1;
		} else {
			if (option == 1)
				deductpercent = (float)1 / 4;
			else
				deductpercent = (float)1 / 2;
		}

		currentlevelexp = npc->exp - new1_getexprequired(level - 1);
		deductexp = 2000 * deductpercent;

		if (currentlevelexp <= deductexp)
			deductexp = currentlevelexp;
	}

	return deductexp;
}

// t111
int getarenaid(player* player) {
	int arenaid = -1;

	if (player) {
		int px = player->x;
		int py = player->y;

		for (int i = 0; i < arenacount; i++) {
			if ( ((px >= arenalocx[i]) && (px < arenalocx[i] + arenasizex[i])) && ((py >= arenalocy[i]) && (py < arenalocy[i] + arenasizey[i])) ) {
				arenaid = i;
				break;
			}
		}
	}

	return arenaid;
}


