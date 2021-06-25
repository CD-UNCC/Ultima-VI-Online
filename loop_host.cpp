#include "define_both.h" /* luteijn: won't really be included again, but helps with auto-completion of variable names. */

if (NEThost){ //host
  //U6O_ERRORTAG

  btime2+=et/150.0f; //non-rolled over btime!
  x=btime2/24;
  btime=btime2-(float)x*24.0f;
  x%=7; bday=x+1;
  
  if (lastsecond==-1) lastsecond=ett; x=ett; //detect frame rate
  if (x!=lastsecond){
    lastsecond=x; framerate=framecount; framecount=0;
  }
  framecount++;

  //Adjust economy values
  for (i=0;i<=1023;i++){ for (i2=0;i2<=3;i2++){
    if (economy_setup[i][i2]==TRUE){
      if (economy_change[i][i2]){
        f2=abs(economy_change[i][i2]); f=3600.0f/f2;
        //f is how often it should occur without the exponential factor
        f3=((float)economy_value[i][i2])/((float)economy_limit[i][i2]);//0-1
        f3*=2.0f;//0-2
        f3+=1.0f;//1-3
        //f3 is a multiplier to make it take 3 times as long to increase as value reaches limit
        f*=f3;

        if (int((ett/f))!=int((ett/f)-(et/f))){
          if (economy_change[i][i2]>0) economy_value[i][i2]++;
          if (economy_change[i][i2]<0) economy_value[i][i2]--;
          if (economy_value[i][i2]<0) economy_value[i][i2]=0;
          if (economy_value[i][i2]>economy_limit[i][i2]) economy_value[i][i2]=economy_limit[i][i2];
        }//f
      }//economy_change
    }//economy_setup==TRUE
  }}//i,i2






  //deduct house payments from housesav
  f=300.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
    housesav[0].flags+=5;
    if (housesav[0].flags>=1440){
      housesav[0].flags=0;
      for (i=1;i<=255;i++){
        if (housesav[i].flags&1){ //currently occupied
          x=housesav[i].gold-housecost[i];
          if (x<=0) x=0;
          housesav[i].gold=x;
          if (housesav[i].gold==0){
            for (x2=0;x2<=playerlist_last;x2++){ if (playerlist[x2]){ //if (playerlist[x2]->net!=INVALID_NET){
              if (playerlist[x2]->GNPCflags[28]==i) goto skipevict; //can't evict online player
              //}
            }}//x2,...
            //evict owner
            //MessageBox(NULL,"HOUSESTORAGECHECK: EVICT","Ultima 6 Online",MB_OK);
            housesav[i].flags--; //remove house occupied flag
            housesav_update();
          }//gold==0
skipevict:;
        }
      }//i
    }//housesav[0].flags>=1440
    housesav_update();
  }//f


  /* hireling respawn*/
  for (i=0;i<=HIRELINGS_MAX -1;i++){
    hirl_wait[i]-=et; 
    if (hirl_wait[i]<=0){
      if (hirl_obj[i]) {
        hirl_obj[i]->info|=112;
        tnpc=(npc*)hirl_obj[i]->more;
        hirl_obj[i]=0;
        addhireling(i,tnpc->schedule);
      }
      else {
        addhireling(i,0);
      }
    }
  }


  //doorclose: relock door after waiting for 2 hours
  for (i=0;i<=doorclose_last;i++){
    if (doorclose_wait[i]>=1.0f){
      doorclose_wait[i]+=et;
      if (doorclose_wait[i]>=12288.0f){
        doorclose_wait[i]=0;
        doorclose_obj[i]->type=doorclose_oldtype[i]; OBJcheckflags(doorclose_obj[i]->x,doorclose_obj[i]->y);
        if (doorclose_obj2[i]){
          doorclose_obj2[i]->type=doorclose_oldtype2[i]; OBJcheckflags(doorclose_obj2[i]->x,doorclose_obj2[i]->y);
        }//doorclose_obj2[i]
      }//wait>=8192.0f
    }//wait>=1.0f
  }//i

  //leverchange: revert lever position after waiting for 2 hours
  for (i=0;i<=leverchange_last;i++){
    if (leverchange_wait[i]>=1.0f){
      leverchange_wait[i]+=et;
      if (leverchange_wait[i]>=12288.0f){
        leverchange_wait[i]=0;
        if (leverchange_obj[i]->type!=leverchange_oldtype[i]){
          use_basic_toggle(NULL,leverchange_obj[i]);
        }
      }//wait>=8192.0f
    }//wait>=1.0f
  }//i

  ouln=-1; //object update/active list num
  //add fixed objects to update
  if (f_ouln!=-1){
    for (i=0;i<=f_ouln;i++){
      myobj=f_oul[i];
      if (myobj->x||myobj->y){
        ouln++;
        oul[ouln]=f_oul[i];
        oul[ouln]->info|=32768;
      }//x&y not null
    }
  }

  /*
  for (i=0;i<=15;i++){
  if (stolenitemwarning[i]){
  stolenitemwarning_wait[i]-=et;
  if (stolenitemwarning_wait[i]<=0.0f) stolenitemwarning[i]=NULL;
  }//stolenitemwarning[i]
  }//i
  */

  //wind: change direction
  windnew=0;
  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
    if (rnd*32.0f<1.0f){
      windnew=1;
      x=rnd*3; x--; windx=x;
      x=rnd*3; x--; windy=x;
    }
  }

  //recalculate moon phase!
  moonphase=255; //NULL moon phase
  f5=32;

  f=btime2*1.125f+3.0f;
  x=f/24; f-=(float)x*24.0f;
  if ((f>5)&&(f<20)){
    if (f>=12.0f) f3=-(f-12.0f); else f3=12.0f-f;
    if (f>=12.0f) f5=f-12.0f; else f5=12.0f-f; //NEW
    f3/=1.125f;
    f2=btime+f3;
    f4=f; f=12.0f;
    if (f2>=24.0f) f2-=24.0f;
    if (f2<0.0f) f2+=24.0f;
    if (f2>=f) f2=f2-f; else f2=f2+(24.0f-f);
    f2+=1.5f; if (f2>=24.0f) f2-=24.0f;
    moonphase=f2/3.0f; //NEW
  }

  f=btime2*1.1875f+5.0f;
  x=f/24; f-=(float)x*24.0f;
  if ((f>5)&&(f<20)){
    if (f>=12.0f) f3=-(f-12.0f); else f3=12.0f-f;
    if (f>=12.0f) f6=f-12.0f; else f6=12.0f-f; //NEW
    if (f6<f5){
      f3/=1.1875f;
      f2=btime+f3; 
      f4=f; f=12.0f;
      if (f2>=24.0f) f2-=24.0f;
      if (f2<0.0f) f2+=24.0f;

      if (f2>=f) f2=f2-f; else f2=f2+(24.0f-f);
      f2+=1.5f; if (f2>=24.0f) f2-=24.0f;
      moonphase=f2/3.0f; //NEW
    }//f6<f5
  }

  if (moonphase!=255){
    //check moongates!
    if (showmoongates==0){
      //object *moongate[8][2];
      //unsigned char showmoongates=0;
      //unsigned short moongatex[8],moongatey[8];
      for (i=0;i<=7;i++){
        OBJadd(moongatex[i],moongatey[i],moongate[i][0]);
        OBJadd(moongatex[i]-1,moongatey[i],moongate[i][1]);
      }
      showmoongates=1;
    }

  }else{ //255

    if (showmoongates){
      for (i=0;i<=7;i++){
        OBJremove(moongate[i][0]);
        OBJremove(moongate[i][1]);
      }
      showmoongates=0;
    }


  }//showmoongates



  stormcloak_last=-1;


  sfbufferwait+=et;
  f=1.0f/8.0f;
  if (sfbufferwait>=f){
    sfbufferwait=0.0f;
    sfbuffersend=1;
  }


  wizardeyesi=0;
  for (tpl=0;tpl<=playerlist_last;tpl++){ if (playerlist[tpl]){ if (playerlist[tpl]->net){
    tplayer=playerlist[tpl];
    if (tplayer->wizardeyetimeleft){
      wizardeyesi++;
      wizardeyesx[wizardeyesi]=tplayer->wizardeyex;
      wizardeyesy[wizardeyesi]=tplayer->wizardeyey;
    }
  }}}




  for (tpl=0;tpl<=playerlist_last;tpl++){ //create and send UPDATE message


    if (playerlist[tpl]){ //current player
      if (playerlist[tpl]->net!=NULL){ //net connection available
        tplayer=playerlist[tpl];

        tplayer->online+=et;

        /*
        // luteijn: keep client in check, but this is too often?
	// luteijn: it is, so now only sending it when saving.
        //send time
        txtset(t2,"?"); t2->d2[0]=32;
        txtset(t3,"12345678"); t3->dd[0]=btime2;
        txtadd(t2,t3);
        NET_send(NETplayer,tplayer->net,t2);
        */

        //doorclose: set/reset wait timer for door if player is near
        if (myobj=tplayer->party[0]){
          for (i=0;i<=doorclose_last;i++){
            x=abs(doorclose_obj[i]->x-myobj->x); y=abs(doorclose_obj[i]->y-myobj->y);
            if ((x<=16)&&(y<=12)){
              doorclose_wait[i]=1.0f;
            }//<=8
          }//i
        }//->party[0]

        //leverchange: set/reset wait timer if player is near lever OR portcullis
        if (myobj=tplayer->party[0]){
          for (i=0;i<=leverchange_last;i++){
            x=abs(leverchange_obj[i]->x-myobj->x); y=abs(leverchange_obj[i]->y-myobj->y);
            if ((x<=16)&&(y<=12)){
              leverchange_wait[i]=1.0f;
            }//<=8
            myobj2=(object*)leverchange_obj[i]->more;
leverchange0:
            if (myobj2){
              x=abs(myobj2->x-myobj->x); y=abs(myobj2->y-myobj->y);
              if ((x<=16)&&(y<=12)){
                leverchange_wait[i]=1.0f;
              }//<=8
              myobj2=(object*)myobj2->next;
              goto leverchange0;
            }




          }//i
        }//->party[0]


        //orb
	// luteijn: this updates the orb destinations a player has visited.
	// visited means he was 8 or less squares from the spot.
        //GNPCflags[22]: bitvector for available orb destinations
        if (myobj=tplayer->party[0]){
          x2=1;
          for (y=0;y<=4;y++){ for (x=0;x<=4;x++){
            x3=abs(myobj->x-orbx[x][y]); y3=abs(myobj->y-orby[x][y]);
            if ((x3<=8)&&(y3<=8)){
              tplayer->GNPCflags[22]|=x2;
            }
            x2=x2<<1;
          }}//x,y
        }//party[0]


        if (tplayer->guardianstatuewait){
          f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
            tplayer->guardianstatuewait--;
          }
        }

        tplayer->wizardeyetimeleft-=et; if (tplayer->wizardeyetimeleft<0.0f) tplayer->wizardeyetimeleft=0.0f;
        tplayer->wizardeyemovewait-=et; if (tplayer->wizardeyemovewait<0.0f) tplayer->wizardeyemovewait=0.0f;


        tplayer->speaking-=et; if (tplayer->speaking<0.0f) tplayer->speaking=0.0f;

        if (tplayer->rest){
          tplayer->rest--;
          if (tplayer->rest==16){
            txtset(t,"??"); t->d2[0]=250; t->d2[1]=0; NET_send(NETplayer,tplayer->net,t);
          }//rest==16
          if (tplayer->rest==0){
            tplayer->GNPCflags[253]|=(1+2);
          }//rest==0
        }//rest

        //check trap indexes
        for (i=0;i<=7;i++){
          if (myobj=tplayer->party[i]){
            if ((myobj->x!=tplayer->trapx[i])||(myobj->y!=tplayer->trapy[i])) {tplayer->trapx[i]=0; tplayer->trapy[i]=0;}
          }
        }

        //add storm cloak fields
        for (i=0;i<=7;i++){
          if (myobj=tplayer->party[i]){
            if (myobj->x){ //not in void
              tnpc=(npc*)myobj->more;
              if (myobj2=tnpc->armour){
                if (myobj2->type==81){
                  stormcloak_last++;
                  stormcloak_x[stormcloak_last]=myobj->x;
                  stormcloak_y[stormcloak_last]=myobj->y;
                  stormcloak_player[stormcloak_last]=tplayer;

                  if (unsigned long(ett)!=unsigned long(ett-et)){
                    myobj2->more2++;
                    if (myobj2->more2>=1024){ //~16-32mins
                      i2=rnd*1024;
                      if (!i2){
                        tnpc->armour=NULL; OBJrelease(myobj2);
                        tnpc->upflags|=64;
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s storm cloak vanished!"); NET_send(NETplayer,tplayer->net,t);
                      }//!i2
                    }//16-32
                  }//et

                }//81
                else if (tnpc->negatemagic!=NULL) {goto negatemagic_field;}
              }//armour
              else if (tnpc->negatemagic!=NULL) {
negatemagic_field:
                stormcloak_last++;
                stormcloak_x[stormcloak_last]=myobj->x;
                stormcloak_y[stormcloak_last]=myobj->y;
                stormcloak_player[stormcloak_last]=tplayer;
              }
            }//->x
          }//party
        }//i




        if (tplayer->party[0]){
          if ((tplayer->party[0]->x==368)&&(tplayer->party[0]->y==386)) tplayer->kallor=0;
          if (tplayer->kallor){
            f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
              tplayer->kallor--;
              if (tplayer->kallor==0){
                static unsigned char tplayer_craft;
                tplayer_craft=FALSE;
                if (tplayer->craft){
                  tplayer_craft=TRUE;
                  if (((tplayer->party[0]->type&1023)==412)||((tplayer->party[0]->type&1023)==414)||((tplayer->party[0]->type&1023)==415)){//exit raft/skiff/boat
                    myobj2=tplayer->party[0]; //boat (temp, used for positioning)
                    VLNKremove(tplayer->party[0]); VLNKsremove(tplayer->party[0]); //remove vlnk(s)
                    if ((tplayer->party[0]->type&1023)==412) tplayer->party[0]->more=tplayer->craft_con; else tplayer->party[0]->more=NULL;
                    tplayer->party[0]->info=tplayer->party[0]->info&(1<<9);
                    tplayer->party[0]=tplayer->craft;
                    tplayer->craft=NULL;
                    for (i4=7;i4>=0;i4--){ if (tplayer->party[i4]!=NULL){
                      OBJadd(myobj2->x,myobj2->y,tplayer->party[i4]);
                    }}
                  }//412/414/415
                  //balloon
                  if ((tplayer->party[0]->type&1023)==423){
                    myobj2=tplayer->party[0]; //boat (temp, used for positioning)
                    use_craft(tplayer,myobj2); 
                  }//423
                }//craft

                for (x4=0;x4<=7;x4++){
                  if (tplayer->party[x4]){//player is alive (or they would not be listed)
                    tnpc=(npc*)tplayer->party[x4]->more;

                    txtset(t,"??"); t->d2[0]=252; t->d2[1]=x4; NET_send(NETplayer,tplayer->net,t);
                    VLNKremove(tplayer->party[x4]); VLNKsremove(tplayer->party[x4]); //remove VLNK(s)

                    //only take xp if player is standing on a non-void-type square and not 0,0
                    if (tplayer->party[x4]->x&&tplayer->party[x4]->y){//not at 0,0
                      if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x]&1023)!=0){
                        if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x]&1023)!=255){
                          if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x]&1023)!=254){
                            if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x]&1023)!=253){
                              if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x]&1023)!=252){
                                //tnpc->exp-=(tnpc->exp/16);//kal lor
								if (easymodehostn1) {
									tnpc->exp -= new1_getexpdeduction(tnpc, 1); // c111
								}
								else { // original
	                                tnpc->exp-=(tnpc->exp/16);//kal lor
								}
                              }}}}}
                    }

                    if (tnpc->horse){
                      horsedismount(tplayer->party[x4]);
                      if (tplayer_craft){
                        if (HORSEDISMOUNT_HORSEOBJECT){//send horse back to Britain's stables free of charge
                          OBJmove_allow=TRUE;
                          OBJmove2(HORSEDISMOUNT_HORSEOBJECT,280,441);
                          OBJmove2(HORSEDISMOUNT_HORSEOBJECT,279,441);
                          OBJmove_allow=FALSE;
                        }
                      }
                    }

                    OBJmove_allow=TRUE;
                    OBJmove2(tplayer->party[x4],256+128-16,256+128+2); //place avatar on the map
                    OBJmove_allow=FALSE;



                    if (tnpc->hp<30) tnpc->hp=30;
                    tnpc->upflags|=1; //update all
                    tnpc->flags&=(65535-1); //remove poisoned flag
                    if (x4==0){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"\x022KAL LOR!\x022");
                      NET_send(NETplayer,tplayer->net,t);
                    }
                  }}
              }//tplayer->kallor==0
            }//f
          }//tplayer->kallor
        }else{//tplayer->party[0]
          tplayer->kallor=0;
        }


        tplayer->ktar_display-=et; if (tplayer->ktar_display<0.0f) tplayer->ktar_display=0;
        for (i=0;i<=9;i++){
          if (tplayer->ktar_wait[i]){
            f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
              tplayer->ktar_wait[i]--;
            }
            //distance check
            if (myobj=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
              if (myobj2=(object*)tplayer->ktar[i]->more){
                x=abs(myobj->x-myobj2->x);
                y=abs(myobj->y-myobj2->y);
                if ((x>16)||(y>12)) tplayer->ktar_wait[i]=0;
              }else{
                tplayer->ktar_wait[i]=0;
              }
            }//->party
          }//wait
        }//i


        //windnew update
        if (windnew){
          tplayer->windx=0; tplayer->windy=0;
          txtset(t,"??"); t->d2[0]=14; t->d2[1]=(windx+1)+(windy+1)*4; NET_send(NETplayer,tplayer->net,t);
        }

        if (tplayer->xray){
          f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
            tplayer->xray--;
          }
        }

        if (tplayer->peer){ //peer time reduce
          f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
            tplayer->peer--;
          }
        }

        //update midi
        if (myobj=tplayer->party[0]){
          x=myobj->x; y=myobj->y;
          tnpc=(npc*)myobj->more;
          // random 50% chance to get theme or forest
          i=rnd*2;
          if(i==1) {
            i=5;//overland: forest midi
          }
          else {
            i=11; //U6 theme
          }

          if (x>=1024) i=3;//underground: dungeon midi
          if ((x>=1024)&&(y>=256)&&(x<1280)&&(y<512)) i=6;//gargoyle lands: xylophone midi
          if ((x>=1280)&&(y>=256)&&(x<1351)&&(y<319)) i=1;//castle floor II: britan theme midi
          if ((x>=243)&&(y>=307)&&(x<406)&&(y<454)) i=1;//britan: britan theme midi
          if ((x>=560)&&(y>=60)&&(x<648)&&(y<120)) i=10;//minoc: stones midi
          if ((x>=1024)&&(y>=616)&&(x<1072)&&(y<656)) i=11;//island floor II: forest theme midi
          if ((x>=1320)&&(y>=319)&&(x<=1326)&&(y<=324)) i=1;//tower II

          if ((x>=1280)&&(y>=344)&&(x<=1336)&&(y<=394)) i=11;//top floor of keep
          if ((x>=1242)&&(y>=512)&&(x<=1265)&&(y<=538)) i=11;//underground of keep
          if ((x>=220)&&(y>=663)&&(x<=243)&&(y<=688)) i=11;//ground floor of keep



          //txtset(t,"??"); t->d2[0]=11; t->d2[1]=7; NET_send(NETplayer,tplayer->net,t);
          i2=bt[y][x]&255;
          if ((i2>=8)&&(i2<=15)) i=7;

          /*
          if (tnpc->target!=NULL){ //*FIX THIS
          i=9;
          }
          */
          if ((i2>=16)&&(i2<=47)) goto nomidichange; //shore

          if (i!=tplayer->midi){
            tplayer->midi=i;
            txtset(t,"??"); t->d2[0]=11; t->d2[1]=i; NET_send(NETplayer,tplayer->net,t);
          }
        }
nomidichange:

        /*
        if (t->d2[0]==11){ //type 11: set next midi
        txtright(t,t->l-1);
        nmidi=t->d2[0];
        goto CLIENT_donemess;
        }//11
        */







        for (x=0;x<=7;x++){
          if (tplayer->party[x]){




            myobj2=tplayer->party[x];
            tnpc=(npc*)myobj2->more;
            if (tnpc->hp<=0){



              //if (tplayer->craft) {tnpc->hp=0; goto exitloop1;} //*2004 TEMP
              //death at sea!
              if (tplayer->craft){

                if (((tplayer->party[0]->type&1023)==412)||((tplayer->party[0]->type&1023)==414)||((tplayer->party[0]->type&1023)==415)){//exit raft/skiff/boat
                  VLNKremove(tplayer->party[0]); VLNKsremove(tplayer->party[0]); //remove vlnk(s)
                  if ((tplayer->party[0]->type&1023)==412) tplayer->party[0]->more=tplayer->craft_con; else tplayer->party[0]->more=NULL;
                  tplayer->party[0]->info=tplayer->party[0]->info&(1<<9);
                  tplayer->party[0]=tplayer->craft;
                  tplayer->craft=NULL;
                  for (i4=7;i4>=0;i4--){
                    if (tplayer->party[i4]!=NULL){

                      OBJadd(myobj2->x,myobj2->y,tplayer->party[i4]);

                      //horse fix for death at sea
                      horsedismount(tplayer->party[i4]);
                      if (HORSEDISMOUNT_HORSEOBJECT){//send horse back to Britain's stables free of charge
                        OBJmove_allow=TRUE;
                        OBJmove2(HORSEDISMOUNT_HORSEOBJECT,280,441);
                        OBJmove2(HORSEDISMOUNT_HORSEOBJECT,279,441);
                        OBJmove_allow=FALSE;
                      }

                    }
                  }
                  myobj2=tplayer->party[0];
                }//412/414/415



                //balloon
                if ((tplayer->party[0]->type&1023)==423){
                  use_craft(tplayer,myobj2); 
                  /*VLNKremove(tplayer->party[0]); VLNKsremove(tplayer->party[0]); //remove vlnk(s)
                  tplayer->party[0]->more=tplayer->craft_con;
                  tplayer->party[0]->info=tplayer->party[0]->info&(1<<9);
                  tplayer->party[0]=tplayer->craft;
                  tplayer->craft=NULL;
                  for (i4=7;i4>=0;i4--){
                  if (tplayer->party[i4]!=NULL){
                  OBJadd(myobj2->x,myobj2->y,tplayer->party[i4]);
                  }
                  }
                  //remove balloon from map + return item to inventory
                  static mlobj *mmyobj; //array size varies
                  mmyobj=(mlobj*)myobj2->more;
                  OBJremove(mmyobj->obj[0]); OBJrelease(mmyobj->obj[0]);
                  OBJremove(mmyobj->obj[1]); OBJrelease(mmyobj->obj[1]);
                  OBJremove(mmyobj->obj[2]); OBJrelease(mmyobj->obj[2]);
                  myobj2->type=420; myobj2->info|=256;
                  OBJremove(myobj2);
                  //tnpc=(npc*)tplayer->party[0]->more; //already set!
                  additemroot(tnpc,myobj2);
                  tnpc->upflags|=32;*/
                  myobj2=tplayer->party[0];
                }//423

              }//death at sea

              horsedismount(myobj2);//this won't work at sea (but this is not currently implemented)

login_dead:


              VLNKremove(myobj2); VLNKsremove(myobj2); //remove VLNK(s)

              x2=myobj2->type;
              x3=myobj2->x; y3=myobj2->y;

              OBJmove_allow=TRUE;
              OBJmove2(myobj2,DEATHPOSX,DEATHPOSY);
              OBJmove_allow=FALSE;

              x4=x2;
              if ((x2&1023)==376) x2=339+6*1024;
              if ((x2&1023)==377) x2=339+5*1024;
              if ((x2&1023)==378) x2=339+3*1024;
              if ((x2&1023)==379) x2=339+2*1024;
              if ((x2&1023)==380) x2=339+2*1024;
              if ((x2&1023)==382) x2=339+4*1024;
              if ((x2&1023)==381) x2=339+8*1024; //2004
              if ((x2&1023)==383) x2=339+8*1024;
              if ((x2&1023)==384) x2=339+5*1024;
              if ((x2&1023)==385) x2=339+8*1024;
              if ((x2&1023)==386) x2=339+7*1024;
              if ((x2&1023)==387) x2=339+3*1024;
              if ((x2&1023)==410) x2=339+7*1024;
              if ((x2&1023)==363) x2=339+0*1024; //gargoyle! Bah Lem
              if ((x2&1023)==354) x2=339+10*1024; //Sherry!

              //if (x2!=x4){
              if (OBJfindlast(x3,y3)==NULL){ //blood
                myobj3=OBJnew();
                myobj3->type=338+(int(rnd*3)*1024); myobj3->info|=112;
                OBJadd(x3,y3,myobj3);
              }
              myobj3=OBJnew();
              myobj3->type=x2;
              myobj3->info|=112;
              OBJadd(x3,y3,myobj3);

              tnpc->baseitem=tnpc->items; tnpc->baseitem_offset=0;

              myobj4=myobj3;

              if (x!=0){ //drop inventory items if NOT primary party member

                if (tplayer->selected_partymember==x){
                  tplayer->pathfind=0;
                  tnpc->order=1;
                  tplayer->selected_partymember=0;
                  if (tplayer->party[0]){
                    tnpc3=(npc*)tplayer->party[0]->more;
                    tnpc3->order=0;
                  }
                }


                if (tnpc->converse!=201){ //only if not a permanent party member
leaveitems2:
                  if (myobj3=(object*)tnpc->items->more){
                    OBJremove(myobj3);
                    OBJadd(myobj4->x,myobj4->y,myobj3);
                    goto leaveitems2;
                  }
                  if (myobj3=tnpc->helm){ tnpc->helm=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  if (myobj3=tnpc->neck){ tnpc->neck=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  if (myobj3=tnpc->wep_left){
                    if (myobj3->type==(90+1024)) myobj3->type=90;
                    tnpc->wep_left=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  if (myobj3=tnpc->wep_right){
                    if (myobj3->type==(90+1024)) myobj3->type=90;
                    tnpc->wep_right=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  if (myobj3=tnpc->armour){ tnpc->armour=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  if (myobj3=tnpc->ring_left){ tnpc->ring_left=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  if (myobj3=tnpc->ring_right){ tnpc->ring_right=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  if (myobj3=tnpc->boots){ tnpc->boots=NULL; OBJadd(myobj4->x,myobj4->y,myobj3);}
                  tnpc->baseitem=tnpc->items; tnpc->baseitem_offset=0;
                }

                tnpc->hp=30;
                tnpc->upflags|=1; //update all upon respawn
                tnpc->flags&=(65535-1); //remove poisoned flag

                //add NPC to resurrect list
                for (x4=0;x4<=nresu;x4++){
                  if (resu[x4]==NULL) goto newresurrect2;
                }//x4
                nresu++; x4=nresu;
newresurrect2:
                resu[x4]=tplayer->party[x];
                resu_body[x4]=myobj4;
                resu_body_type[x4]=myobj4->type;
                resu_wait[x4]=16; //should be 1 hour (but what is host closes during this hour?)
                resu_player[x4]=tplayer;
                resu_partymember[x4]=x;
                resu_x[x4]=myobj4->x; resu_y[x4]=myobj4->y;
              }//x!=0


              //remove NPC

              if (x==0){
                tplayer->pathfind=0;
                tplayer->kallor=0;
                tplayer->px=x3; tplayer->py=y3;
                tplayer->selected_partymember=0;
                for (x4=1;x4<=7;x4++){
                  if (tplayer->party[x4]){
                    horsedismount(tplayer->party[x4]);//this won't work at sea (but this is not currently implemented)
                    txtset(t,"??"); t->d2[0]=252; t->d2[1]=x4; NET_send(NETplayer,tplayer->net,t);
                    VLNKremove(tplayer->party[x4]); VLNKsremove(tplayer->party[x4]); //remove VLNK(s)
                    OBJmove_allow=TRUE;
                    OBJmove2(tplayer->party[x4],DEATHPOSX,DEATHPOSY);
                    OBJmove_allow=FALSE;
                    //tplayer->party[x4]=NULL; ***other party members are not really gone!***
                  }
                }//x4

                //send online message
                txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," killed!");
                for (x4=0;x4<=playerlist_last;x4++){
                  if (playerlist[x4]){ //current player
                    if (playerlist[x4]->net!=NULL){ //net connection available
                      if (playerlist[x4]->party[0]){
                        if (playerlist[x4]!=tplayer){
                          NET_send(NETplayer,playerlist[x4]->net,t);
                        }}}}}

                if (tplayer->karma<=0){ //karma<=0
                  OBJmove_allow=TRUE;
                  OBJmove2(tplayer->party[0],DEATHPOSX,DEATHPOSY);
                  OBJmove_allow=FALSE;
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,"An unending darkness engulfs thee...");
                  NET_send(NETplayer,tplayer->net,t);
                  //tplayer->idle_connect=64;
                  /* if player owns a house, then free the house, but don't make a house chest so that its possible to recover the house items. 
                  and making the house chest would be a problem, because its made when logging in*/
                  if (houseowner(tplayer,tplayer->GNPCflags[28])){
                      housesav[tplayer->GNPCflags[28]].gold=0;
                      housesav[tplayer->GNPCflags[28]].flags--; //remove house occupied flag
                      housesav_update();
                      tplayer->GNPCflags[28]=0;
                  }
                }else{ //karma>0

                  //add player to resurrect list
                  for (x4=0;x4<=nresu;x4++){
                    if (resu[x4]==NULL) goto newresurrect;
                  }//x4
                  nresu++; x4=nresu;
newresurrect:
                  resu[x4]=tplayer->party[0];
                  resu_body[x4]=myobj3;
                  resu_body_type[x4]=myobj3->type;
                  resu_wait[x4]=16; //should be 1 hour (but what is host closes during this hour?)
                  resu_player[x4]=tplayer;
                  resu_partymember[x4]=0;
                  resu_x[x4]=myobj4->x; resu_y[x4]=myobj4->y;

                  //like death
                  OBJmove_allow=TRUE;
                  OBJmove2(tplayer->party[0],DEATHPOSX,DEATHPOSY);
                  OBJmove_allow=FALSE;

                  /*
                  //move player to castle!
                  OBJmove_allow=TRUE;
                  OBJmove2(tplayer->party[0],256+128-16,256+128+2); //place avatar on the map
                  OBJmove_allow=FALSE;
                  */


                  tnpc->hp=30;
                  tnpc->upflags|=1; //update all upon respawn
                  tnpc->flags&=(65535-1); //remove poisoned flag


                  //player *resu_player[65536]; //player object belongs to
                  //unsigned char resu_partymember[65536]; //party member index


                  //txtset(t,"?"); t->d2[0]=8; txtadd(t,"An unending darkness engulfs thee... A voice in the darkness intones, \x022KAL LOR!\x022");
                  if (tplayer->GNPCflags[25]==0) {
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,"An unending darkness engulfs thee... (press F1 to resurrect)");
                  }
                  else {
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,"An unending darkness engulfs thee... (press F1 to use your insurance and resurrect at healer)");
                  }
                  NET_send(NETplayer,tplayer->net,t);


                  //goto exitloop1;
                }//karma
              }//x==0




              //send message stating the NPC has died

              //TESTING
              txtset(t,"??"); t->d2[0]=252; t->d2[1]=x; NET_send(NETplayer,tplayer->net,t);


              tplayer->party[x]=NULL;
              if (!x) inbritanniacheck();

              if (login_dead_callback) goto login_dead2;

              goto exitloop1;
              //replace with dead bodies!!!, and blood!
              //myobj=OBJnew();
              //myobj->type=
              //tplayer->party[x]=NULL;

            }//hp<=0
          }//active
        }//x
exitloop1:;



        playerlist[tpl]->globalmessage_wait-=et; if (playerlist[tpl]->globalmessage_wait<0) playerlist[tpl]->globalmessage_wait=0;
        playerlist[tpl]->idle+=et;
        playerlist[tpl]->idle_connect+=et;
        playerlist[tpl]->save_update+=et;


        if (!playerlist[tpl]->party[0]) playerlist[tpl]->idle=0; //can't idle if dead





        //metalhead*if ((exitrequest)&&(tpl!=0)) {playerlist[tpl]->save_update=-1; goto save_update1;} //update all players!
        if (exitrequest) { //update all players!
#ifdef CLIENT
          if (tpl) {txtset(t,"??"); t->d2[0]=250; t->d2[1]=1; NET_send(NETplayer,playerlist[tpl]->net,t);} //host quit!
#endif
          playerlist[tpl]->save_update=0; goto save_update1;
        }

        //MessageBox(NULL,"Saving #1","Ultima 6 Online",MB_OK);

        //metalhead*if ((playerlist[tpl]->save_update>=512)&&(tpl!=0)) {playerlist[tpl]->save_update=-1; goto save_update1;}
        if (playerlist[tpl]->save_update>=512) {playerlist[tpl]->save_update=-1; goto save_update1;}

        if (playerlist[tpl]->GNPCflags[253]&1) {playerlist[tpl]->GNPCflags[253]--; goto autoleavebritannia;}

        if (playerlist[tpl]->party[0]) x=16; else x=64;
		// r555 idle timeout check???
#ifdef CLIENT
        if ((playerlist[tpl]->idle_connect>=x)&&(tpl!=0)) goto connect_failed; //(16sec)

        if ((playerlist[tpl]->idle>=8192)&&(tpl!=0)){ //remove idle player (not host) (~2 hours)
#else
        if ((playerlist[tpl]->idle_connect>=x)) goto connect_failed; //(16sec)

        if ((playerlist[tpl]->idle>=8192)){ //remove idle player (not host) (~2 hours)
#endif
connect_failed:
          txtset(t,"??"); t->d2[0]=250; t->d2[1]=9; NET_send(NETplayer,playerlist[tpl]->net,t);//leaving britannia (idle)
autoleavebritannia:

          //static txt *u6o_user_name=txtnew(); txtset(u6o_user_name,"UNKNOWN");
          //static txt *u6o_user_password=txtnew(); txtset(u6o_user_password,"UNKNOWN");

          //save player
save_update1:

          //MessageBox(NULL,"Saving","Ultima 6 Online",MB_OK);

          //U6O_ERRORTAG


          if (playerlist[tpl]->name->l==0) goto save_failed;
          if (playerlist[tpl]->password->l==0) goto save_failed;
          txtset(t,"UNKNOWN");
          if (txtsame(playerlist[tpl]->name,t)) goto save_failed;
          if (txtsame(playerlist[tpl]->password,t)) goto save_failed;




          //find prev savefile
          i2=-1;
          for (i=0;i<=SAVESLOTLAST;i++){ //search save buffer for player info!
            if (save_buffer[i]){
              if (txtsame(save_username[i],playerlist[tpl]->name)){
                if (txtsame(save_password[i],playerlist[tpl]->password)){
                  i2=i; //i2=prev file to remove
                  goto gotprevsave;
                }//password
              }//name
            }//save_buffer[i]
          }//i
gotprevsave:

          //died from lack of Karma?
          if ((playerlist[tpl]->party[0]==NULL)&&(playerlist[tpl]->karma<=0)) goto save_complete; //skip save then remove


          //230-237 x,y location of a dead partymember
          for (i3=0;i3<=7;i3++) tplayer->GNPCflags[230+i3]=0;//not dead!


          for (i3=0;i3<=7;i3++) party_ok[i3]=1;
          for (i3=0;i3<=nresu;i3++){
            if (resu[i3]){
              if (resu_player[i3]==playerlist[tpl]){
                party_ok[resu_partymember[i3]]=0;
                //set location of dead body
                tplayer->GNPCflags[230+resu_partymember[i3]]=resu_x[i3]+(resu_y[i3]<<16);
                playerlist[tpl]->party[resu_partymember[i3]]=resu[i3];
              }}}
          if (playerlist[tpl]->party[0]==NULL) goto save_complete; //ERROR! could not find in resurrection table

          tnpc=(npc*)playerlist[tpl]->party[0]->more;
          txtset(t,"New Player"); if (txtsame(tnpc->name,t)) goto save_complete; //if a prev savegame exists it will be deleted



          //find empty savefile
          for (i=0;i<=SAVESLOTLAST;i++){
            if (save_buffer[i]==0) goto save_empty;
          }
          goto save_failed; //ERROR! all savefile slots are full
save_empty:

          //implement new password?
          if (playerlist[tpl]->newpassword->l){
            //check if new password conflicts with existing savegame, if so the password won't change
            for (i3=0;i3<=SAVESLOTLAST;i3++){
              if (save_buffer[i3]){
                if (txtsame(playerlist[tpl]->name,save_username[i3])){
                  if (txtsame(playerlist[tpl]->newpassword,save_password[i3])){
                    goto newpassword_unavailable;
                  }}}}
            txtset(playerlist[tpl]->password,playerlist[tpl]->newpassword);
          }
newpassword_unavailable:
          txtset(playerlist[tpl]->newpassword,"");

          txtset(save_username[i],playerlist[tpl]->name);
          txtset(save_password[i],playerlist[tpl]->password);
          tnpc=(npc*)playerlist[tpl]->party[0]->more; txtset(save_name[i],tnpc->name);
          save_buffer[i]=1;

          txtset(t4,"");
          /*
	     txtset(t,"??"); t->ds[0]=3; txtadd(t4,t); 
	   */
          txtset(t,"??"); t->ds[0]=SAVEVER; txtadd(t4,t); 

          if (SAVEVER==5){
            txtset(t,"?"); t->d2[0]=PW_ENCRYPTION; txtadd(t4,t); 
          }

          txtset(t,"?"); t->d2[0]=playerlist[tpl]->name->l; txtadd(t4,t); txtadd(t4,playerlist[tpl]->name);
          txtset(t,"?"); t->d2[0]=playerlist[tpl]->password->l; txtadd(t4,t); txtadd(t4,playerlist[tpl]->password);

          txtset(t,"??"); t->ds[0]=playerlist[tpl]->party[0]->x;
          if (party_ok[0]==0) t->ds[0]=256+128-16;
          txtadd(t4,t);
          txtset(t,"??"); t->ds[0]=playerlist[tpl]->party[0]->y;
          if (party_ok[0]==0) t->ds[0]=256+128+2;
          txtadd(t4,t);


          txtset(t,"?"); t->d2[0]=playerlist[tpl]->male_female; txtadd(t4,t);
          txtNEWLEN(t,256*4); memcpy(t->d,&playerlist[tpl]->NPCflags,256*4); txtadd(t4,t);
          //GLOBAL FLAG 255=craft type
          playerlist[tpl]->GNPCflags[255]=0;
          tplayer->GNPCflags[254]=ett;
          if (playerlist[tpl]->craft){
            if ((playerlist[tpl]->party[0]->type&1023)==412) playerlist[tpl]->GNPCflags[255]=1; //ship
            if ((playerlist[tpl]->party[0]->type&1023)==414) playerlist[tpl]->GNPCflags[255]=2; //skiff
            if ((playerlist[tpl]->party[0]->type&1023)==415) playerlist[tpl]->GNPCflags[255]=3; //raft
            if ((playerlist[tpl]->party[0]->type&1023)==423) playerlist[tpl]->GNPCflags[255]=4; //balloon
          }

          //*THIS LINE HAS BEEN EDITED TO ONLY ALLOW PRIMARY PLAYER'S CHARACTER TO BE SAVED!
          x3=0; for (x2=1;x2<=7;x2++){
            //ONLY IF NOT A CORE NPC!
            if (playerlist[tpl]->party[x2]){
              tnpc3=(npc*)playerlist[tpl]->party[x2]->more;
              if (tnpc3->converse==201) x3++;
            }
          }
          playerlist[tpl]->GNPCflags[252]=x3; //no. companion party members

          static SYSTEMTIME systime;
          GetLocalTime(&systime);
          x3=(systime.wDay-1)+(systime.wMonth-1)*32+systime.wYear*(32*12);
          playerlist[tpl]->GNPCflags[250]=x3; //250 YMD of last save


          if (houseowner(playerlist[tpl],playerlist[tpl]->GNPCflags[28])){
            //set number of storage positions
            playerlist[tpl]->GNPCflags[29]=housestoragenext[playerlist[tpl]->GNPCflags[28]]; 
          }else{
            //NULL house info
            playerlist[tpl]->GNPCflags[28]=0; playerlist[tpl]->GNPCflags[29]=0;



            //MessageBox(NULL,"HOUSESTORAGECHECK: [28]=0","Ultima 6 Online",MB_OK);
            //txtnumint(t9,houseowner_FAILVALUE);
            //MessageBox(NULL,t9->d,"Ultima 6 Online",MB_OK);

          }
          //if playerlist[tpl]->GNPCflags[28]!=0 house is valid

          //247&248 TEMP food values for npc 0-7, 1 byte each (only used when saving/loading)
          playerlist[tpl]->GNPCflags[247]=0; playerlist[tpl]->GNPCflags[248]=0;
          for (x2=0;x2<=7;x2++){ if (playerlist[tpl]->party[x2]){
            tnpc3=(npc*)playerlist[tpl]->party[x2]->more;
            x3=tnpc3->food;
            if (x2==0) playerlist[tpl]->GNPCflags[247]|=x3;
            if (x2==1) playerlist[tpl]->GNPCflags[247]|=(x3<<8);
            if (x2==2) playerlist[tpl]->GNPCflags[247]|=(x3<<16);
            if (x2==3) playerlist[tpl]->GNPCflags[247]|=(x3<<24);
            if (x2==4) playerlist[tpl]->GNPCflags[248]|=x3;
            if (x2==5) playerlist[tpl]->GNPCflags[248]|=(x3<<8);
            if (x2==6) playerlist[tpl]->GNPCflags[248]|=(x3<<16);
            if (x2==7) playerlist[tpl]->GNPCflags[248]|=(x3<<24);
          }}

          txtNEWLEN(t,256*4); memcpy(t->d,&playerlist[tpl]->GNPCflags,256*4); txtadd(t4,t);
          txtset(t,"????"); t->df[0]=playerlist[tpl]->karma; txtadd(t4,t);



          for(x8=0;x8<=7;x8++){
            if (playerlist[tpl]->party[x8]){
              tnpc=(npc*)playerlist[tpl]->party[x8]->more;
              if ((x8==0)||(tnpc->converse==201)){ //***NOT A U6 NPC***

                txtset(t,"??"); t->ds[0]=playerlist[tpl]->party[x8]->type;
                if (playerlist[tpl]->craft){
                  if (x8==0) t->ds[0]=playerlist[tpl]->craft->type;
                }
                if (tnpc->horse){
                  //z3=playerlist[tpl]->party[x8]->type>>11;//z3 is the direction of the horse

                  t->ds[0]=tnpc->originalobjtype;

                  //exit(tnpc->originalobjtype);
                }

                txtadd(t4,t); //player type!
                txtset(t,"?"); t->d2[0]=tnpc->name->l; txtadd(t4,t); txtadd(t4,tnpc->name);
                txtset(t,"??"); t->ds[0]=tnpc->port; txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->hp; txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->hp_max; txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->mp; txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->mp_max; txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->s; txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->d; txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->i; txtadd(t4,t);
                txtset(t,"????"); t->dl[0]=tnpc->exp;


                //FOLLOWING LINE REMOVED TO ALLOW FOR LOGGING BACK IN AS DEAD
                //if (party_ok[x8]==0) t->dl[0]=tnpc->exp-int(tnpc->exp/8); //deduct xp required for ressurection ***FIXED 2005***



                txtadd(t4,t);
                txtset(t,"??"); t->ds[0]=tnpc->lev; txtadd(t4,t);
                txtset(t,"?"); t->d2[0]=tnpc->flags; txtadd(t4,t);

                //npc->OBJ
                txtset(t,"");
                txtset(t2,"????");
                x5=0;
                for (x2=0;x2<=9;x2++){
                  if (x2==0) myobj=tnpc->helm;
                  if (x2==1) myobj=tnpc->wep_right;
                  if (x2==2) myobj=tnpc->wep_left;
                  if (x2==3) myobj=tnpc->armour;
                  if (x2==4) myobj=tnpc->boots;
                  if (x2==5) myobj=tnpc->neck;
                  if (x2==6) myobj=tnpc->ring_right;
                  if (x2==7) myobj=tnpc->ring_left;
                  if (x2==8) myobj=(object*)tnpc->items; //includes the bag!
                  if (x2==9) myobj=playerlist[tpl]->mobj; //*ONLY PARTY[0]
                  if (x8&&(x2==9)) myobj=NULL;



                  if (myobj){
                    t2->dl[0]=x5+1; txtadd(t,t2);
                  }else{
                    t2->dl[0]=0; txtadd(t,t2); //no object
                  }


                  x5=OBJlist(myobj);
                }
                OBJlist_last=NULL;

housestorageadd0:

                //OBJlist->txtlist
                for(y6=0;y6<x5;y6++){
                  t2->ds[0]=OBJlist_list[y6]->type;
                  t2->ds[1]=OBJlist_list[y6]->info;


                  //invalid item!
                  if (obji[sprlnk[OBJlist_list[y6]->type&1023]].weight==0){
                    if (obji[sprlnk[OBJlist_list[y6]->type&1023]].flags&1024){//CONTAINER!
                      //bag
                      t2->ds[0]=OBJ_BAG;//change item type to a bag
                      t2->ds[1]=112;//set info to avoid conflict
                    }else{
                      //rubber duck
                      t2->ds[0]=169;//ducky
                      t2->ds[1]=112;//set info to avoid conflict
					  txtset(newt1, "invalid item make duck ------------------------ ");
					  txtnumint(t6, OBJlist_list[y6]->type);
					  txtadd(newt1, t6);
					  LOGadd(newt1);
                    }
                  }


                  txtadd(t,t2);
                  t2->dl[0]=OBJlist_list[y6]->more2;


                  //if (housestorageadd){
                  //txtnumint(t5,t2->ds[0]); txtadd(t5,":type"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
                  //}

                  //txtnumint(t5,OBJlist_list[y6]->type&1023);
                  //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


                  if (t2->dl[0]){ //->more!=NULL
                    if (obji[sprlnk[OBJlist_list[y6]->type&1023]].flags&1024){ //container
                      for(y7=0;y7<x5;y7++){ //link
                        if ((object*)OBJlist_list[y6]->more==OBJlist_list[y7]) t2->dl[0]=y7+1;
                      }//y7
                    }//multiple
                  }//->more!=NULL

                  if (OBJlist_list[y6]->type==448){//horse papers (save)
                    myobj2=(object*)OBJlist_list[y6]->more;

                    //latest bug, caused by mose, dunno what he did
                    //myobj2 is obviously invalid! it should point to the horses head... yet clearly did not!

                    //step 1. somehow get rid of deed while on horse (such as asking partymember to leave)
                    //step 2. exit (horse is now released)
                    //step 3. deed is now stuffed

                    //if (myobj2==NULL) exit(123);


                    //NEW GPF THIS LINE! x100!
                    if (myobj2->x){//horse is not being ridden (so it doesn't matter if we are in a boat or not)

                      t2->dl[0]=myobj2->x+myobj2->y*2048;//use as few bits as possible(21 bits total)
                      //21

                      t2->dl[0]+=(((creature*)myobj2->more)->respawn_x<<21);
                      //8

                      //add direction
                      t2->dl[0]+=(((myobj2->type>>11)&3)<<29);
                      //2

                      t->d2[t->l-1]&=225;//11100001b
                      t->d2[t->l-1]+=(8<<1);//horse without rider!

                      if (playerlist[tpl]->save_update!=-1){//player is really leaving, remove horse
                        OBJmove_allow=TRUE;
                        OBJmove2(myobj2,DEATHPOSX,DEATHPOSY);
                        OBJmove_allow=FALSE;
                      }


                    }else{//horse is being ridden by a partymember
                      t2->dl[0]=0;
                      //scan to detect which partymember is on that horse
                      z4=0;
                      for(z3=0;z3<=7;z3++){
                        if (myobj3=playerlist[tpl]->party[z3]){
                          tnpc3=(npc*)myobj3->more;
                          if ((z3==0)||(tnpc3->converse==201)){
                            if (tnpc3->horse==myobj2){


                              //t2->dl[0]=z4;
                              t2->dl[0]=myobj3->x+myobj3->y*2048;//use as few bits as possible(21 bits total)

                              //!horse in boat alert! this will set it to 0!
                              if (playerlist[tpl]->craft){//it MUST be in a boat
                                t2->dl[0]=2097151;//111111111111111111111b
                              }


                              t2->dl[0]+=( ((creature*)((object*)tnpc3->horse)->more)->respawn_x <<21);

                              //add direction
                              t2->dl[0]+=(((myobj3->type>>11)&3)<<29);

                              t->d2[t->l-1]&=225;//11100001b
                              t->d2[t->l-1]+=(z4<<1);//npc#


                              goto save_gotridingnpc;
                            }//tnpc3->horse==myobj2
                            z4++;
                          }//converse
                        }//party[z3]
                      }//z3
                      //WARNING! None of the "savable" members of the party are riding this horse!
                      //at this point the only alternative is that a non-savable partymember is riding the horse
                      for(z3=0;z3<=7;z3++){
                        if (myobj3=playerlist[tpl]->party[z3]){
                          tnpc3=(npc*)myobj3->more;
                          if ((z3!=0)&&(tnpc3->converse!=201)){
                            if (tnpc3->horse==myobj2){
                              t2->dl[0]=myobj3->x+myobj3->y*2048;//use as few bits as possible(21 bits total)

                              if (playerlist[tpl]->craft){//it MUST be in a boat
                                //t2->dl[0]=2097151;//111111111111111111111b

                                //if (HORSEDISMOUNT_HORSEOBJECT){//send horse back to Britain's stables free of charge
                                //OBJmove_allow=TRUE;
                                //OBJmove2(HORSEDISMOUNT_HORSEOBJECT,430,280);
                                //OBJmove2(HORSEDISMOUNT_HORSEOBJECT,429,280);
                                //OBJmove_allow=FALSE;

                                t2->dl[0]=280+441*2048;//return horse belonging to temp. partymember to stables in Brit.
                                //430,280
                              }


                              t2->dl[0]+=( ((creature*)((object*)tnpc3->horse)->more)->respawn_x <<21);
                              //add direction
                              t2->dl[0]+=(((myobj3->type>>11)&3)<<29);

                              t->d2[t->l-1]&=225;//11100001b
                              t->d2[t->l-1]+=(8<<1);//horse without rider!

                              goto save_gotridingnpc;
                            }//tnpc3->horse==myobj2
                          }//converse
                        }//party[z3]
                      }//z3

                      exit (5534);


                    }//myobj2->x
save_gotridingnpc:;
                    //save horse "status"

                  }//horse papers (save)


                  if (OBJlist_list[y6]->type==149){//deed
                    myobj2=(object*)OBJlist_list[y6]->more; //myobj2(boat)
                    if (myobj2==playerlist[tpl]->party[0]){
                      t2->dl[0]=0xFFFFFFFF;
                    }else{
                      t2->dl[0]=myobj2->x+(myobj2->y<<16);
                      if (playerlist[tpl]->save_update!=-1){
                        //remove boat (ship)
                        if ((myobj2->info&2)==0){ //empty boat?
                          if (t2->dl[0]){ //not in inventory
                            OBJmove_allow=TRUE;
                            OBJmove2(myobj2,DEATHPOSX,DEATHPOSY);
                            OBJmove_allow=FALSE;
                          }//not in inventory
                        }//empty
                      }//save_update
                    }
                  }//149

                  txtadd(t,t2);


                  if (OBJlist_list[y6]->next==NULL) {t2->dl[0]=0; txtadd(t,t2);} //group finished
                }//y6
                t2->dl[0]=0; txtadd(t,t2); //0+0=all finished

                //*ADD LOOP FOR MORE PARTY MEMBERS
                txtadd(t4,t);

                if (housestorageadd) goto housestorageadd1;

              }//NOT A U6 NPC
            }//->party[x8]
          }//x8 [252]

          //save house storage objects
          for (x2=0;x2<playerlist[tpl]->GNPCflags[29];x2++){
            myobj=(object*)od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[playerlist[tpl]->GNPCflags[28]][x2]]->next;
            x5=0;
            txtset(t,"");
            txtset(t2,"????");
            x5=OBJlist(myobj);
            OBJlist_last=NULL;
            housestorageadd=1; goto housestorageadd0;
housestorageadd1: housestorageadd=0;
            //remove storage objects ONLY if player is leaving
            if (playerlist[tpl]->save_update!=-1){
              myobj=(object*)od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[playerlist[tpl]->GNPCflags[28]][x2]]->next;
              if (myobj){
                od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[playerlist[tpl]->GNPCflags[28]][x2]]->next=NULL;
                OBJcheckflags(housestoragex[playerlist[tpl]->GNPCflags[28]][x2],housestoragey[playerlist[tpl]->GNPCflags[28]][x2]);
              }
            }//playerlist[tpl]->save_update!=-1
          }//x2

          /* luteijn 20080731: as of version 4.0 (=2.0), skip encrypting the file! 
          //ENCRYPT3.0
          encrypt(t4);
          */

          txtset(t2,".\\save\\"); txtnumint(t,i); txtadd(t,".sav"); txtadd(t2,t);
          tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
          put(tfh,t4->d2,t4->l);
          close(tfh);
/* mose: disabling this new save format for now. I really don't have time to make this work.
           luteijn a new format for naming the savegames 
          txtset(t2,"./save/"); 
          txtset(t,"?/"); 
          t->d[0]=playerlist[tpl]->name->d[0];
          if ( (t->d[0] < 'A') || (t->d[0] > 'Z')) {
            t->d[0]='@'; // weird character names go here
          }
          txtadd(t2,t); 
          txtadd(t2,playerlist[tpl]->name); 
          txtadd(t2,"-"); 
          txtadd(t2,playerlist[tpl]->npcname); 
          txtadd(t2,".u6o");   /*idea is to atomically move this over the real save later 
          LOGadd(t2); // scrlog is client only!
          tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
          if (tfh->h!=HFILE_ERROR) {
            put(tfh,t4->d2,t4->l); /* FIXME: NO RETURN CODE, HOW DO WE KNOW IT WORKED so we can overwrite the existing save?  
            close(tfh);

            /* TODO: if saving went well, move the .u6o file to .sav 
          } else {
            txtset(t,"Failed to open new format savefile"); 
            LOGadd(t); 
            /* TODO: find out what went wrong and retry.  
          }*/


          // luteijn: sent a time update message at this point 
          // This is done to keep the times reasonably in sync without sending time too often
          // FIXME: use a small function for this eventually.
          //send time
          txtset(t2,"?"); t2->d2[0]=32;
          txtset(t3,"12345678"); t3->dd[0]=btime2;
          txtadd(t2,t3);
          NET_send(NETplayer,playerlist[tpl]->net,t2);

save_complete:

          //remove prev save file for USER_NAME & USER_PASSWORD!
          if (i2!=-1){
            txtset(t2,".\\save\\"); txtnumint(t,i2); txtadd(t,".sav"); txtadd(t2,t);
            DeleteFile(t2->d);
            save_buffer[i2]=0;
          }

save_failed:
          if (playerlist[tpl]->save_update==-1){
            for(i3=0;i3<=7;i3++){
              if (party_ok[i3]==0) playerlist[tpl]->party[i3]=NULL; //set back to dead
            }
            playerlist[tpl]->save_update=0;
            goto save_update2;
          }

          //auto-lock house & evict all NPC
          if (i4=playerlist[tpl]->GNPCflags[28]){
            for (i3=0;i3<housepnext[i4];i3++){
              x2=housepx[i4][i3]; y2=housepy[i4][i3];
              if (myobj=od[y2][x2]){
housescan0:
                if ((myobj->type&1023)==301){//doorway
                  if (myobj->info&8) myobj2=(object*)myobj->more; else myobj2=myobj;//redirect
                  if (myobj2->more2){//lock number exists
                    //door WILL be the first object (for player housing, exceptions exist, such an horse ranch in Britain)
                    myobj3=od[myobj2->y][myobj2->x];
housescan1:
                    if (myobj3){
                      x3=myobj3->type&1023;//GPF
                      if ((x3<297)||(x3>300)){myobj3=(object*)myobj3->next; goto housescan1;}
                      myobj3->type&=4095; myobj3->type|=8192;
                      OBJcheckflags(myobj3->x,myobj3->y);
                      myobj3=(object*)myobj3->more;
                      myobj3->type&=4095; myobj3->type|=8192;
                      OBJcheckflags(myobj3->x,myobj3->y);
                    }//lock number exists
                  }//myobj3
                }//doorway
                if (myobj->info&2){//evict all NPC
                  myobj2=(object*)myobj->next;//backup ->next pointer
                  OBJmove_allow=TRUE; OBJmove2(myobj,houseentrancex[i4],houseentrancey[i4]); OBJmove_allow=FALSE;
                  myobj=myobj2; if (myobj) goto housescan0;
                  goto housescan2;
                }//evict all NPC
                if (myobj->info&4 && (myobj->type&1023)==430){//evict horses
                  myobj2=(object*)myobj->next;//backup ->next pointer
                  OBJmove_allow=TRUE; OBJmove2(myobj,houseentrancex[i4],houseentrancey[i4]); OBJmove_allow=FALSE;
                  myobj=myobj2; if (myobj) goto housescan0;
                  goto housescan2;
                }//evict horses
                if (myobj=(object*)myobj->next) goto housescan0; //GPF
              }//myobj
housescan2:;
            }//i3
          }//i4 (housenumber)

          //disconnect socket
          if (playerlist[tpl]->net!=INVALID_NET){
            if (socket_disconnect[playerlist[tpl]->net]==0) socket_disconnect[playerlist[tpl]->net]=1;
          }
          //return CORE NPC party members
          for(i3=1;i3<=7;i3++){
            if (playerlist[tpl]->party[i3]){
              tnpc3=(npc*)playerlist[tpl]->party[i3]->more;
              if ((tnpc3->converse)&&(tnpc3->converse!=201)){


                horsedismount(playerlist[tpl]->party[i3]);
                if (HORSEDISMOUNT_HORSEOBJECT){
                  OBJmove_allow=TRUE;
                  OBJmove2(HORSEDISMOUNT_HORSEOBJECT,DEATHPOSX,DEATHPOSY);
                  OBJmove_allow=FALSE;
                  //ideally horse crt should be deallocated
                }


                tnpc3->player=NULL;
                tnpc3->upflags=0;
                tnpc3->order=2; //schedule
                tnpc3->follow=NULL;
                tnpc3->hp=30;
                //tnpc3->exp-=(tnpc->exp/8); ***REMOVED BECAUSE NPC HAS NOT ACTUALLY DIED, JUST LEFT***
                tnpc3->flags=0;
                OBJmove_allow=TRUE;
                OBJmove2(playerlist[tpl]->party[i3],tnpc3->schedule[0].x,tnpc3->schedule[0].y);
                OBJmove_allow=FALSE;
                playerlist[tpl]->party[i3]=NULL;
              }
            }
          }

          //remove any of player's party from resurrect list
          for (i3=0;i3<=nresu;i3++){
            if (resu[i3]){
              if (resu_player[i3]==playerlist[tpl]){

                //is this a core NPC (if so then resurrect them at original pos!)
                tnpc3=(npc*)resu[i3]->more;
                if ((tnpc3->converse)&&(tnpc3->converse!=201)){
                  tnpc3->player=NULL;
                  tnpc3->upflags=0;
                  tnpc3->order=2; //schedule
                  tnpc3->follow=NULL;
                  tnpc3->hp=30;
                  //tnpc3->exp-=(tnpc->exp/8);
				  if (easymodehostn1) {
					  tnpc3->exp -= new1_getexpdeduction(tnpc3, 2); // c111
				  }
				  else { // original
	                  tnpc3->exp-=(tnpc->exp/8);
				  }
                  tnpc3->flags=0;
                  OBJmove_allow=TRUE;
                  OBJmove2(resu[i3],tnpc3->schedule[0].x,tnpc3->schedule[0].y);
                  OBJmove_allow=FALSE;
                }

                resu[i3]=NULL;
              }//player
            }//resu
          }//i3

          if (playerlist[tpl]->party[0]){
            tnpc2=(npc*)playerlist[tpl]->party[0]->more;
            txtset(t2,"?"); t2->d2[0]=8; txtadd(t2,"Left Britannia: "); txtadd(t2,tnpc2->name);
            for (x=0;x<=playerlist_last;x++){
              if (playerlist[x]){
                if (playerlist[x]->net!=INVALID_NET){
                  if (x!=tpl){
                    NET_send(NETplayer,playerlist[x]->net,t2);
                  }}}}
          }
          for (i=0;i<=7;i++){ //remove party members from map
            if (playerlist[tpl]->party[i]){
              VLNKremove(playerlist[tpl]->party[i]); VLNKsremove(playerlist[tpl]->party[i]); //remove VLNK(s)
              horsedismount(playerlist[tpl]->party[i]);
              if (HORSEDISMOUNT_HORSEOBJECT){
                OBJmove_allow=TRUE;
                OBJmove2(HORSEDISMOUNT_HORSEOBJECT,DEATHPOSX,DEATHPOSY);
                OBJmove_allow=FALSE;
                //ideally horse crt should be deallocated
              }
              OBJmove_allow=TRUE;
              OBJmove2(playerlist[tpl]->party[i],DEATHPOSX,DEATHPOSY);
              OBJmove_allow=FALSE;
            }}

          //check for house sold evict
          if (houseowner(tplayer,tplayer->GNPCflags[28])&&housesav[tplayer->GNPCflags[28]].gold==0){
            housesav[tplayer->GNPCflags[28]].flags--; //remove house occupied flag
            housesav_update();
          }
          //free NPC items and structures
          for (i=0;i<=7;i++){
            x5=0;
            if (playerlist[tpl]->party[i]){
              tnpc=(npc*)playerlist[tpl]->party[i]->more;

              if (i==0){
                txtset(t,"Left_Britannia:"); txtadd(t,tnpc->name); LOGadd(t);
                myobj=playerlist[tpl]->mobj; //*ONLY PARTY[0]
              }
              else {
                myobj=NULL;
              }
              if (myobj) x5=OBJlist(myobj);
              OBJlist_last=NULL;
              for(y6=0;y6<x5;y6++){
                OBJremove(OBJlist_list[y6]); OBJrelease(OBJlist_list[y6]);
              }
              free(tnpc);
              //remove from object update list
              if (ouln!=-1){
                for (i2=0;i2<=ouln;i2++){
                  if (playerlist[tpl]->party[i]==oul[i2]){
                    oul[i2]=NULL;
                  }}}
              OBJremove(playerlist[tpl]->party[i]); OBJrelease(playerlist[tpl]->party[i]);
            }//->party
          }//i
          //free player structure
          free(playerlist[tpl]);
          playerlist[tpl]=NULL;
          inbritanniacheck();

          goto idle_skip;
        }

save_update2:
        //add party to fixed update listq
        for (i=0;i<=7;i++){
          if (playerlist[tpl]->party[i]){
            x=playerlist[tpl]->party[i]->x; y=playerlist[tpl]->party[i]->y;
addupdateobjs_wizardeye:
            if (x||y){ //must not be at 0,0, if so it's not on map

              //tpx=x-15; tpy=y-11;
              //if (tpx<0) tpx=0; if (tpy<0) tpy=0;
              //if (tpx>2016) tpx=2016;
              //if (tpy>1000) tpy=1000;
              getscreenoffset(x,y,&tpx,&tpy);

              for (y=-8;y<=(23+8);y++){ for (x=-8;x<=(31+8);x++){
                if (((x+tpx)<0)||((y+tpy)<0)||((x+tpx)>2047)||((y+tpy)>1023)) myobj=NULL; else myobj=od[y+tpy][x+tpx];


nextobj2: if (myobj!=NULL){


                if (myobj->type==416){//view redirector
                  if (myobj->more2){
                    myobj=od[myobj->more2>>16][myobj->more2&65535];
                    goto nextobj2;
                  }
                }


                i3=0;
                myobj5=myobj;

                //CRASH BANDICOOT
                if (myobj5->info&8) myobj5=(object*)myobj5->more; //<-redirect


                if (myobj5->info&2) i3=1;
                if (myobj5->info&4) i3=1;
                if (myobj5->type==(223+1024)) i3=1; //lit powder keg!!!
                if (i3==1){
                  if ((myobj5->info&32768)==0){ //don't add more than once
                    myobj5->info=myobj5->info|32768; //object is active
                    ouln++; oul[ouln]=myobj5;
                  }
                }

                if (myobj->next!=NULL){myobj=(object*)myobj->next; goto nextobj2;}
          } //object exists
              }}

            }//x||y
          }//party member OK

          if (i==7){
            if (playerlist[tpl]->wizardeyetimeleft){
              i++; x=playerlist[tpl]->wizardeyex; y=playerlist[tpl]->wizardeyey; goto addupdateobjs_wizardeye;
            }
          }

        }//i


        //2005 objects update message
        //assume buffer needs to be resent

        //shift buffer to new location
        /*
        short sobj_bufoffx,sobj_bufoffy; //x,y offset of left corner of entire buffer
        unsigned short sobj_buf2bytes[96][72]; //number of 2 byte elements allocated
        unsigned short sobj_bufsize[96][72]; //number of 2 byte elements actually used
        unsigned short *sobj[96][72]; //scene object raw data
        unsigned long  sobj_tempfixed[96][72]; //temp fixed objects
        */

        static unsigned long bitsi,bitsi2;
        static unsigned short *tp2;
        static unsigned char sceneupdaterequired;

        if (!tplayer->updatemessage){


          txtNEWLEN(t,-1048576); //create 1MB buffer
          txtNEWLEN(t2,-1048576); //create 1MB buffer
          bitsi=0; bitsi2=0;
          sceneupdaterequired=0;

          z=31;
          if (tplayer->resync){
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
            z=35;
            tplayer->resync=0;
          }//resync
          BITSadd(t,&bitsi,z,8);//scene update

          //txtnumint(t3,t->d2[1]);
          //MessageBox(NULL,t3->d,"Ultima 6 Online",MB_OK);



          tplayer=playerlist[tpl];
          //get player offset
          if (myobj=tplayer->party[tplayer->selected_partymember]){
            x=myobj->x;
            y=myobj->y;
            if (tplayer->wizardeyetimeleft){
              x=tplayer->wizardeyex; y=tplayer->wizardeyey;
            }
          }else{
            x=tplayer->px; y=tplayer->py; //location where player died
          }

          //only send if changed
          if ((x!=tplayer->x)||(y!=tplayer->y)){
            z=1; BITSadd(t,&bitsi,z,1); 
            BITSadd(t,&bitsi,x,11);
            BITSadd(t,&bitsi,y,10);
            tplayer->x=x; tplayer->y=y;
            sceneupdaterequired=1;
          }else{
            z=0; BITSadd(t,&bitsi,z,1);
          }





          //get screen offset
          //tpx=x-15; tpy=y-11;
          //if (tpx<0) tpx=0; if (tpy<0) tpy=0;
          //if (tpx>2016) tpx=2016;
          //if (tpy>1000) tpy=1000;
          getscreenoffset(x,y,&tpx,&tpy);

          //does screen+1 fit inside current buffer?
          x3=tpx-1; y3=tpy-1; x4=tpx+32; y4=tpy+24;//screen+1
          x5=tplayer->sobj_bufoffx; y5=tplayer->sobj_bufoffy; x6=x5+96-1; y6=y5+72-1; //current buffer extents
          //i. if the screen+1 buffer fits within buffer don't relocate
          if (x3>=x5){ if (x4<=x6){ if (y3>=y5){ if (y4<=y6){
            goto screen1buffershiftcomplete;
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
            goto screen1buffershiftcomplete;
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
sobj_copyloop1:
          x8=x2-tplayer->sobj_bufoffx+x9; y8=y2-tplayer->sobj_bufoffy+y9;
          if (x8>=0){ if (x8<=95){ if (y8>=0){ if (y8<=71){
            //copy position
            tplayer->sobj_tempfixed[x9][y9]=tplayer->sobj_tempfixed[x8][y8];
            i2=tplayer->sobj_buf2bytes[x9][y9]; tplayer->sobj_buf2bytes[x9][y9]=tplayer->sobj_buf2bytes[x8][y8]; tplayer->sobj_buf2bytes[x8][y8]=i2;
            tp2=tplayer->sobj[x9][y9]; tplayer->sobj[x9][y9]=tplayer->sobj[x8][y8]; tplayer->sobj[x8][y8]=tp2;
            tplayer->sobj_bufsize[x9][y9]=tplayer->sobj_bufsize[x8][y8]; tplayer->sobj_bufsize[x8][y8]=0;
            goto sobj_copiedpos1;
          }}}}
          //clear position
          tplayer->sobj_tempfixed[x9][y9]=0;
          tplayer->sobj_bufsize[x9][y9]=0;
sobj_copiedpos1:;
          if (i==0){
            x9++; if (x9>=96){y9++; x9=0;}
            if (y9<72) goto sobj_copyloop1;
          }
          if (i==1){
            x9--; if (x9<0){y9++; x9=95;}
            if (y9<72) goto sobj_copyloop1;
          }
          if (i==2){
            x9++; if (x9>=96){y9--; x9=0;}
            if (y9>=0) goto sobj_copyloop1;
          }
          if (i==3){
            x9--; if (x9<0){y9--; x9=95;}
            if (y9>=0) goto sobj_copyloop1;
          }
          tplayer->sobj_bufoffx=x2; tplayer->sobj_bufoffy=y2;
screen1buffershiftcomplete:
          //is screen+1 of buffer and visual area EXACTLY THE SAME?
          static unsigned char screenplus1check;
          screenplus1check=1;
          goto screenplus1check_entry;
screenplus1check_return:
          screenplus1check=0;
          //screen+1 was NOT exactly the same, so perform a screen+8 update
          sceneupdaterequired=1;
          z=1; BITSadd(t,&bitsi,z,1);//screen+8 update required


          x3=tpx-8; y3=tpy-8; x4=tpx+32+8-1; y4=tpy+24+8-1;
          x5=tplayer->sobj_bufoffx; y5=tplayer->sobj_bufoffy; x6=x5+96-1; y6=y5+72-1; //current buffer extents
          //i. if the screen+8 buffer fits within buffer don't relocate
          if (x3>=x5){ if (x4<=x6){ if (y3>=y5){ if (y4<=y6){
            goto bufferrelocatecomplete0;
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
            //MessageBox(NULL,"shiftbuffer: entire buffer cleared","Ultima 6 Online",MB_OK);
            goto bufferrelocatecomplete0;
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
sobj_copyloop:
          x8=x2-tplayer->sobj_bufoffx+x9; y8=y2-tplayer->sobj_bufoffy+y9;
          if (x8>=0){ if (x8<=95){ if (y8>=0){ if (y8<=71){
            //copy position
            tplayer->sobj_tempfixed[x9][y9]=tplayer->sobj_tempfixed[x8][y8];
            i2=tplayer->sobj_buf2bytes[x9][y9]; tplayer->sobj_buf2bytes[x9][y9]=tplayer->sobj_buf2bytes[x8][y8]; tplayer->sobj_buf2bytes[x8][y8]=i2;
            tp2=tplayer->sobj[x9][y9]; tplayer->sobj[x9][y9]=tplayer->sobj[x8][y8]; tplayer->sobj[x8][y8]=tp2;
            tplayer->sobj_bufsize[x9][y9]=tplayer->sobj_bufsize[x8][y8]; tplayer->sobj_bufsize[x8][y8]=0;
            goto sobj_copiedpos0;
          }}}}
          //clear position
          tplayer->sobj_tempfixed[x9][y9]=0;
          tplayer->sobj_bufsize[x9][y9]=0;
sobj_copiedpos0:;

          if (i==0){
            x9++; if (x9>=96){y9++; x9=0;}
            if (y9<72) goto sobj_copyloop;
          }
          if (i==1){
            x9--; if (x9<0){y9++; x9=95;}
            if (y9<72) goto sobj_copyloop;
          }
          if (i==2){
            x9++; if (x9>=96){y9--; x9=0;}
            if (y9>=0) goto sobj_copyloop;
          }
          if (i==3){
            x9--; if (x9<0){y9--; x9=95;}
            if (y9>=0) goto sobj_copyloop;
          }
          tplayer->sobj_bufoffx=x2; tplayer->sobj_bufoffy=y2;
          //MessageBox(NULL,"shiftbuffer: buffer shifted","Ultima 6 Online",MB_OK);
bufferrelocatecomplete0:
          //ERROR is it overwriting data it needs to read as it goes
          //OPTOMIZATION!!! switch the pointers to the data and data buffer lengths instead of the data
          //[17:25] galleondragon: and screen+8 area is reloaded in centre
          /*
          for (y9=0;y9<=71;y9++){
          for (x9=0;x9<=95;x9++){
          if (tplayer->sobj_tempfixed[x9][y9]||tplayer->sobj_bufsize[x9][y9]){
          objbufferf->graphic->o2[x9+y9*96]=0xFFFF;
          }else{
          objbufferf->graphic->o2[x9+y9*96]=31;
          }
          }}
          */

          //MessageBox(NULL,"update required","Ultima 6 Online",MB_OK);

screenplus1check_entry:

          //refresh pathfinding array
          //if (tplayer->xray){ //xray!
          //for (x2=0;x2<=65535;x2++) pfa[x2]=PFs+PFd;
          //}else{
          //pathfind2(x,y,NULL);
          //}

          //buffer must be updated while message is created!
          static long mapx,mapy,bufx,bufy;
          static unsigned short vbuf[1024];
          for (y=0;y<=39;y++){ for (x=0;x<=47;x++){
            mapx=tpx+x-8; mapy=tpy+y-8; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;

            /*
            x4=0; z4=2;
            for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
            if (pfa[x+y*256+257]&(PFs+PFd)) z5=1; else z5=0;
            if (z5!=z4){
            if (z4!=2){ //not the first pass
            *p=x4+z4*128; p++;
            }//z4!=2
            x4=0; //number of squares in a row -1
            z4=z5; //type (1 or 0)
            }else{
            if (x4==127){
            *p=127+z4*128; p++;
            x4=0;
            }else{
            x4++;
            }
            }
            }}//x,y
            *p=x4+z4*128; p++; //save last row
            */


            if (screenplus1check){
              if (x<7) goto objbufnoupdate0; if (y<7) goto objbufnoupdate0; if (x>40) goto objbufnoupdate0; if (y>32) goto objbufnoupdate0;
            }

            i=0;

            if (mapx<0) goto objbuffer_outofrange; if (mapx>2047) goto objbuffer_outofrange; if (mapy<0) goto objbuffer_outofrange; if (mapy>1023) goto objbuffer_outofrange;
            myobj=od[mapy][mapx];
objskip0: if (myobj){
            if (objvisible(tplayer,myobj)==NULL){
              myobj=(object*)myobj->next;
              goto objskip0;
            }
            if (myobj->type==416){//view redirector
              if (myobj->more2){
                bt[mapy][mapx]=bt[myobj->more2>>16][myobj->more2&65535];
                mapx=myobj->more2&65535; mapy=myobj->more2>>16;
                myobj=od[myobj->more2>>16][myobj->more2&65535];
                goto objskip0;
              }
            }//view redirector
            if (!tclass_object[myobj->type]){
              myobj=(object*)myobj->next;
              goto objskip0;
            }
          }//myobj

          if (i3=tobjfixed_index[mapy][mapx]){
            i4=tobjfixed_type[i3];
            i5=1;
            i6=1;
            z3=getnbits(i4);
            for (i2=i3+1;i2<=(i3+i4);i2++){
              if (myobj){
                if ((myobj->type&1023)==OBJ_BLOOD) goto noflash0;//blood
                if ((myobj->type&1023)==OBJ_FIRE_FIELD) goto noflash0;//317 fire field
                if ((myobj->type&1023)==OBJ_POISON_FIELD) goto noflash0;//318 poison field
                if ((myobj->type&1023)==OBJ_PROTECTION_FIELD) goto noflash0;//319 protection field
                if ((myobj->type&1023)==OBJ_SLEEP_FIELD) goto noflash0;//320 sleep field
                if ((myobj->type&1023)==OBJ_WEB) goto noflash0;//53 web
                if ((myobj->type&1023)==OBJ_RED_GATE){//red gate
                  if (((myobj->info&112)>>4)>=2) i6=0;
                  goto noflash0;
                }//red gate
                if ((myobj->info&112)==16) i6=0;
noflash0:
                if (objvisible(tplayer,myobj)==NULL) myobj=(object*)myobj->next;
              }

              if (myobj&&i6){
                if (myobj->type==tobjfixed_type[i2]){//object still exists
                  if (tplayer->sobj_tempfixed[bufx][bufy]&i5){//but buffer flag is set, saying it is not there!
                    if (screenplus1check) goto screenplus1check_return; //********SCREENPLUS1CHECK********
                    tplayer->sobj_tempfixed[bufx][bufy]-=i5;
                    z=1; BITSadd(t,&bitsi,z,1);//edit another tempfixed object
                    z=y*48+x; BITSadd(t,&bitsi,z,11);//x,y offset
                    if (z3){z=i2-i3-1; BITSadd(t,&bitsi,z,z3);}//which tempfixed obj

                    //MessageBox(NULL,"tempfixed change","Ultima 6 Online",MB_OK);

                  }//sobj_tempfixed[bufx][bufy]&i5
                  myobj=(object*)myobj->next;
                }else{//object does not exist
                  if ((tplayer->sobj_tempfixed[bufx][bufy]&i5)==0){
                    if (screenplus1check) goto screenplus1check_return; //********SCREENPLUS1CHECK********
                    tplayer->sobj_tempfixed[bufx][bufy]|=i5;
                    z=1; BITSadd(t,&bitsi,z,1);//edit another tempfixed object
                    z=y*48+x; BITSadd(t,&bitsi,z,11);//x,y offset
                    if (z3){z=i2-i3-1; BITSadd(t,&bitsi,z,z3);}//which tempfixed obj

                    //MessageBox(NULL,"tempfixed change","Ultima 6 Online",MB_OK);

                    i6=0;
                  }//(sobj_tempfixed[bufx][bufy]&i5)==0
                }
              }else{
                //object is not present
                if ((tplayer->sobj_tempfixed[bufx][bufy]&i5)==0){
                  if (screenplus1check) goto screenplus1check_return; //********SCREENPLUS1CHECK********
                  tplayer->sobj_tempfixed[bufx][bufy]|=i5;
                  z=1; BITSadd(t,&bitsi,z,1);//edit another tempfixed object
                  z=y*48+x; BITSadd(t,&bitsi,z,11);//x,y offset
                  if (z3){z=i2-i3-1; BITSadd(t,&bitsi,z,z3);}//which tempfixed obj

                  //MessageBox(NULL,"tempfixed change","Ultima 6 Online",MB_OK);

                }//(sobj_tempfixed[bufx][bufy]&i5)==0
              }
              i5<<=1;
            }//i2
          }//tobjfixed_index[mapy][mapx]

          wizardeyesadded=0;

nextbufobj0: if (myobj){
          if (objvisible(tplayer,myobj)==NULL){
            myobj=(object*)myobj->next;
            goto nextbufobj0;
          }
          if (!tclass_object[myobj->type]){
            myobj=(object*)myobj->next;
            goto nextbufobj0;
          }
          vbuf[i]=myobj->type;
          if ((myobj->type&1023)==OBJ_BLOOD) goto noflash1;//blood
          if ((myobj->type&1023)==OBJ_FIRE_FIELD) goto noflash1;//317 fire field
          if ((myobj->type&1023)==OBJ_POISON_FIELD) goto noflash1;//318 poison field
          if ((myobj->type&1023)==OBJ_PROTECTION_FIELD) goto noflash1;//319 protection field
          if ((myobj->type&1023)==OBJ_SLEEP_FIELD) goto noflash1;//320 sleep field
          if ((myobj->type&1023)==OBJ_WEB) goto noflash1;//53 web
          if ((myobj->type&1023)==OBJ_RED_GATE){//red gate
            if (((myobj->info&112)>>4)>=2) vbuf[i]|=32768;
            goto noflash1;
          }//red gate
          if ((myobj->info&112)==16) vbuf[i]|=32768;
noflash1:

          i++;
          myobj=(object*)myobj->next; goto nextbufobj0;
             }//myobj

             //add wizard eye if necessary
             if (!wizardeyesadded){
               for (wizardeyesi2=1; wizardeyesi2<=wizardeyesi; wizardeyesi2++){
                 if (mapx==wizardeyesx[wizardeyesi2]){ if (mapy==wizardeyesy[wizardeyesi2]){
                   if (tplayer->wizardeyetimeleft){
                     if ((tplayer->wizardeyex==mapx)&&(tplayer->wizardeyey==mapy)) goto wizardeyesloopnext;
                   }
                   wizardeyesadded=1;
                   myobj=wizardeyesobj;
                   goto nextbufobj0;
wizardeyesloopnext:;
                 }}//x,y
               }//wizardeyesi2
             }//!wizardeyesadded






             //short sobj_bufoffx,sobj_bufoffy; //x,y offset of left corner of entire buffer
             //unsigned short sobj_buf2bytes[96][72]; //number of 2 byte elements allocated
             //unsigned short sobj_bufsize[96][72]; //number of 2 byte elements actually used
             //unsigned short *sobj[96][72]; //scene object raw data
             //unsigned long  sobj_tempfixed[96][72]; //temp fixed objects
             //compare buffers

             //length check
objbuffer_outofrange:
             if (tplayer->sobj_bufsize[bufx][bufy]!=i) goto objbufupdate0;
             if (i==0) goto objbufnoupdate0;

             tp2=tplayer->sobj[bufx][bufy];
             for (i2=0;i2<i;i2++){
               if (tp2[i2]!=vbuf[i2]) goto objbufupdate0;
             }
             goto objbufnoupdate0;
objbufupdate0:
             if (screenplus1check) goto screenplus1check_return; //********SCREENPLUS1CHECK********

             //update allocated buffer size
             if (tplayer->sobj_buf2bytes[bufx][bufy]<i){
               if (tplayer->sobj[bufx][bufy]) free(tplayer->sobj[bufx][bufy]);
               tplayer->sobj[bufx][bufy]=(unsigned short*)malloc(i*2);
               tplayer->sobj_buf2bytes[bufx][bufy]=i;
             }


             z=1; BITSadd(t2,&bitsi2,z,1);//1 edit objects of another square
             z=y*48+x; BITSadd(t2,&bitsi2,z,11);//11 screen offset of square

             //update buffer info
             tp2=tplayer->sobj[bufx][bufy];
             for (i2=0;i2<i;i2++){
               tp2[i2]=vbuf[i2];

               z=1; BITSadd(t2,&bitsi2,z,1);
               z=vbuf[i2]&(65535-32768); BITSadd(t2,&bitsi2,z,16);//10 object graphic index (optomized ***YET TO BE OPTOMIZED***)
               z=0; if (vbuf[i2]&32768) z=1;
               BITSadd(t2,&bitsi2,z,1);

             }//i2
             z=0; BITSadd(t2,&bitsi2,z,1);


             tplayer->sobj_bufsize[bufx][bufy]=i;







             /*
             1 edit objects of another square
             {
             11 screen offset of square
             {
             1 a/another object exists on this square
             10 object graphic index (optomized)
             1 object is flashing
             }
             }

             */





             //...
objbufnoupdate0:;

             //***** DONT FORGET FLASHING OBJECTS HERE AND IN TEMPFIXED BUFFER
             //***** DONT FORGET NOT TO UPDATE BLACKED OUT SQUARES
             //or off screen

          }}


          if (screenplus1check){
            screenplus1check=0;
            z=0; BITSadd(t,&bitsi,z,1);//screen+8 update unnecessary
          }else{
            z=0; BITSadd(t,&bitsi,z,1);//no more tempfixed objects
            z=0; BITSadd(t2,&bitsi2,z,1);//no more squares of objects to edit
          }

          //add second bitstream to first bitstream
          if (bitsi2){
            i=0; i2=bitsi2>>3;//i2 is number of whole bytes
            for (i=0;i<i2;i++){
              BITSadd(t,&bitsi,t2->d2[i],8);
            }
            i3=bitsi2%8; if (i3) BITSadd(t,&bitsi,t2->d2[i2],i3);
          }



          //mover update

          //create new mover list
          i=-1;
          for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
            mapx=tpx+x-1; mapy=tpy+y-1;

            if (mapx<0) goto moverbuffer_outofrange; if (mapx>2047) goto moverbuffer_outofrange; if (mapy<0) goto moverbuffer_outofrange; if (mapy>1023) goto moverbuffer_outofrange;
            myobj=od[mapy][mapx];
mvobjskip: if (myobj){
            if (myobj->type==416){//view redirector
              if (myobj->more2){
                bt[mapy][mapx]=bt[myobj->more2>>16][myobj->more2&65535];
                mapx=myobj->more2&65535; mapy=myobj->more2>>16;
                myobj=od[myobj->more2>>16][myobj->more2&65535];
                goto mvobjskip;
              }
            }//view redirector
            if (!tclass_mover[myobj->type]){
              myobj=(object*)myobj->next;
              goto mvobjskip;
            }
            //object is a mover class

            //check for npc or crt pointer
            //XXXX UNOCCUPIED BOATS, SKIFFS, RAFTS WON'T HAVE A CRT OR NPC POINTER
            //if ((myobj->info&(2+4))==0){
            //myobj=(object*)myobj->next;
            //goto mvobjskip;
            //}

            if (!myobj->more){ if ((myobj->type&1023)!=414){ if ((myobj->type&1023)!=415){//not a raft or skiff
              myobj=(object*)myobj->next;
              goto mvobjskip;
            }}}


            //add it to the list
            i++;
            mv_object[i]=myobj;
            mv_x[i]=tpx+x-1; mv_y[i]=tpy+y-1;
            mv_type[i]=myobj->type&1023;
            mv_dir[i]=objgetdir(myobj->type);
            mv_frame[i]=OBJGETDIR_FRAME;
            mv_flags[i]=0; mv_playerid[i]=0; mv_hpmp[i]=0; mv_ktar[i]=0; mv_more[i]=0;
            if (mv_type[i]==366) mv_frame[i]=myobj->type>>10;//EXCEPTION: tanglevine tendril
            if (mv_type[i]==413) mv_frame[i]=myobj->type>>10;//EXCEPTION: silver serpent





            if (myobj->info&2){//npc
              tnpc=(npc*)myobj->more;

              //horse with rider? set more
              if (mv_type[i]==431){
                mv_more[i]=tnpc->originalobjtype&1023;
              }

              if (tnpc->protection) mv_flags[i]|=MV_PROTECT;
              if (tnpc->light) mv_flags[i]|=MV_LIGHTBRIGHT;
              if (myobj3=tnpc->armour){
                if (myobj3->type==81) mv_flags[i]|=MV_STORMCLOAK; //storm cloak
              }//armour
              //NEWCODE
              if (tnpc->negatemagic){
                mv_flags[i]|=MV_STORMCLOAK; //storm cloak
              }//negatemagic

              //NEWCODEEND
              if (tnpc->flags&2) mv_flags[i]|=MV_INVISIBLE;
              if (tnpc->flags&4) mv_flags[i]|=MV_SLEEP;
              if (tplayer2=tnpc->player){//has player pointer
                if (tnpc->light==0){
                  if (tnpc->player==tplayer) mv_flags[i]|=MV_LIGHTGLOW;
                }//light
                if (tplayer2->party[0]==myobj){//first player
                  mv_playerid[i]=tplayer2->id;
                  if (tplayer2->typing){
                    if (tplayer!=tplayer2) mv_flags[i]|=MV_TYPING;
                  }//typing
                  if (tplayer2->speaking){
                    if (tplayer!=tplayer2) mv_flags[i]|=MV_SPEAKING;
                  }//speaking
                  //hp/mp bars 0-15hp, 0-7mp, 1poisioned
                  f=(float)tnpc->hp/(float)tnpc->hp_max; z=f*16; if (z==16) z=15;
                  f=(float)tnpc->mp/(float)tnpc->mp_max; z2=f*8; if (z2==8) z2=7;
                  z=z+(z2<<4); if (tnpc->flags&1) z+=128;
                  mv_hpmp[i]=z;
                }//tplayer2->party[0]==myobj
              }else{//no player pointer
                if (tnpc->schedule){
                  if (tnpc->schedule_i!=-1){
                    if (tnpc->schedule[tnpc->schedule_i].type==0x91) mv_flags[i]|=MV_SLEEP; //0x91=sleeping!
                  }//tnpc->schedule_i!=-1
                }//tnpc->schedule
              }//no player pointer
            }//npc

            if (myobj->info&4){//crt
              crt=(creature*)myobj->more;
              if (crt->flags&8) mv_flags[i]|=MV_PARALYZE;
              if (crt->flags&4) mv_flags[i]|=MV_SLEEP;
              if (crt->flags&2){
                if (crt->al==1){i--; goto mover_add_complete;}//creature is not friendly allegiance, don't display at all
                mv_flags[i]|=MV_INVISIBLE;
              }
              for (z=0;z<=9;z++){
                if (tplayer->ktar_wait[z]){
                  if ((object*)tplayer->ktar[z]->more==myobj){
                    mv_ktar[i]=z+1;
                  }
                }
              }//z
            }//crt

mover_add_complete:

            myobj=(object*)myobj->next; goto mvobjskip;
           }//myobj

moverbuffer_outofrange:;
          }}
          mv_i=i+1;



          //compare to current mover list
          ZeroMemory(&mv_last,sizeof(mv_last));
          ZeroMemory(&mv_new,sizeof(mv_new));



          //******this code happens whether or not an update is sent******
          //remove all offscreen objects in client's array
          i=0;
mover_removeoffscreen_next: if (i<tplayer->mv_i){
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
            goto mover_removeoffscreen_next;
          }//x,y boundary check
          i++; goto mover_removeoffscreen_next;
                            }//i check



                            //1. REMOVE
                            i=0;
mover_removecheck_next: if (i<tplayer->mv_i){

                            for (i2=0;i2<mv_i;i2++){
                              if (mv_new[i2]==0){
                                //is new x,y offset out of range? if so the object will be removed and recreated
                                x=mv_x[i2]-tplayer->mv_x[i];
                                if (abs(x)>3) goto mover_remove_outofrange;
                                y=mv_y[i2]-tplayer->mv_y[i];
                                if (abs(y)>3) goto mover_remove_outofrange;
                                if (mover_offseti[x+3][y+3]==255) goto mover_remove_outofrange;
                                if (tplayer->mv_type[i]==mv_type[i2]){

                                  if (tplayer->mv_type[i]==413){//silver serp
                                    if (x||y) goto mover_remove_outofrange;
                                    if (tplayer->mv_frame[i]!=mv_frame[i2]) goto mover_remove_outofrange;
                                    goto mover_remove_link;
                                  }
                                  if (tplayer->mv_type[i]==366){//tanglevine tendril (not core)
                                    if (x||y) goto mover_remove_outofrange;
                                    if (tplayer->mv_frame[i]!=mv_frame[i2]) goto mover_remove_outofrange;
                                    goto mover_remove_link;
                                  }

                                  if (tplayer->mv_object[i]==mv_object[i2]){//obj check
mover_remove_link:

                                    if (tplayer->mv_playerid[i]==mv_playerid[i2]){
                                      mv_last[i]=i2; mv_new[i2]=MOVER_FOUND;//link
                                      i++; goto mover_removecheck_next;
                                    }//playerid

                                  }//object
                                }//type
                              }//mv_new[i2]==0
mover_remove_outofrange:;
                            }//i2

                            //remove mover i message
                            BITSadd(t,&bitsi,1,1);//remove a/another mover
                            BITSadd(t,&bitsi,i,getnbits(tplayer->mv_i));//index of mover to remove based on current buffer
                            sceneupdaterequired=1;

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
                            goto mover_removecheck_next;
                        }//i check
                        z=0; BITSadd(t,&bitsi,z,1);//no more movers to remove

                        //2.1 MOVE MOVER
                        for (i=0;i<tplayer->mv_i;i++){
                          i2=mv_last[i];
                          x=mv_x[i2]-tplayer->mv_x[i]; y=mv_y[i2]-tplayer->mv_y[i];
                          if (x||y){
                            sceneupdaterequired=1;
                            BITSadd(t,&bitsi,1,1);//move a/another mover
                            BITSadd(t,&bitsi,i,getnbits(tplayer->mv_i));//index of mover to move in current buffer
                            BITSadd(t,&bitsi,mover_offseti[x+3][y+3],5);

                            //update client data
                            //HORSE SWAP FIX (HORSE FIX)

                            if ( (tplayer->mv_type[i]==431) &&
                              //( ((mv_dir[i2]==0)&&(mv_x[i2]!=tplayer->mv_x[i])    ) || ((mv_dir[i2]==1)&&(mv_y[i2]!=tplayer->mv_y[i])) || ((mv_dir[i2]==2)&&(mv_x[i2]!=tplayer->mv_x[i])) || ((mv_dir[i2]==3)&&(mv_y[i2]!=tplayer->mv_y[i])) )
                              ( ((mv_dir[i2]==0)&& ((mv_x[i2]!=tplayer->mv_x[i])||(mv_y[i2]>=tplayer->mv_y[i]))          ) || ((mv_dir[i2]==1)&&((mv_y[i2]!=tplayer->mv_y[i])||(mv_x[i2]<=tplayer->mv_x[i]))    ) || ((mv_dir[i2]==2)&&((mv_x[i2]!=tplayer->mv_x[i])||(mv_y[i2]<=tplayer->mv_y[i]))   ) || ((mv_dir[i2]==3)&&((mv_y[i2]!=tplayer->mv_y[i])||(mv_x[i2]>=tplayer->mv_x[i]))   ) )
                              ){

                                //NOHORSEFIXNECESSARY code negates the HORSE SWAP FIX patch under certain conditions to allow for diagonal movement with a frame change
                                if ((mv_dir[i2]==0)&&(mv_y[i2]<tplayer->mv_y[i])) goto NOHORSEFIXNECESSARY;
                                if ((mv_dir[i2]==2)&&(mv_y[i2]>tplayer->mv_y[i])) goto NOHORSEFIXNECESSARY;

                                tplayer->mv_x[i]=mv_x[i2];
                                tplayer->mv_y[i]=mv_y[i2];
                                //change client frame number to trick the server into sending a dir/frame update message too
                                if (tplayer->mv_frame[i]==mv_frame[i2]){
                                  tplayer->mv_frame[i]++; if (tplayer->mv_frame[i]==2) tplayer->mv_frame[i]=0;
                                }
                            }else{
NOHORSEFIXNECESSARY:
                              tplayer->mv_x[i]=mv_x[i2];
                              tplayer->mv_y[i]=mv_y[i2];
                              tplayer->mv_dir[i]=mv_dir[i2];
                              tplayer->mv_frame[i]=mv_frame[i2];
                            }//HORSE SWAP FIX
                          }//x||y
                        }//i
                        BITSadd(t,&bitsi,0,1);//no more movers to move


                        //2.2 MOVER DIRECTION/FRAME CHANGE
                        /*
                        1 change mover frame/dir
                        ? index of mover
                        [2] if mover CAN have a direction it is set here, otherwise this can be omitted and the movers frame only will be incremented
                        */
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




                        //2.3 MOVER STATE CHANGE
                        z3=getnbits(tplayer->mv_i);
                        for (i=0;i<tplayer->mv_i;i++){
                          i2=mv_last[i];
                          i3=0;





                          if ((tplayer->mv_flags[i]&MV_LIGHTBRIGHT)!=(mv_flags[i2]&MV_LIGHTBRIGHT)){
                            if (mv_flags[i2]&MV_LIGHTGLOW){ if (tplayer->mv_flags[i]&MV_LIGHTBRIGHT){
                              goto MV_LIGHTBRIGHT_assumed;
                            }}

                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,0,4);
                          }
MV_LIGHTBRIGHT_assumed:
                          if ((tplayer->mv_flags[i]&MV_LIGHTGLOW)!=(mv_flags[i2]&MV_LIGHTGLOW)){
                            if (mv_flags[i2]&MV_LIGHTBRIGHT){ if (tplayer->mv_flags[i]&MV_LIGHTGLOW){
                              goto MV_LIGHTGLOW_assumed;
                            }}
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,1,4);
                          }
MV_LIGHTGLOW_assumed:
                          if ((tplayer->mv_flags[i]&MV_INVISIBLE)!=(mv_flags[i2]&MV_INVISIBLE)){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,2,4);
                          }
                          if ((tplayer->mv_flags[i]&MV_PARALYZE)!=(mv_flags[i2]&MV_PARALYZE)){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,3,4);
                          }
                          if ((tplayer->mv_flags[i]&MV_SLEEP)!=(mv_flags[i2]&MV_SLEEP)){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,4,4);
                          }
                          if ((tplayer->mv_flags[i]&MV_PROTECT)!=(mv_flags[i2]&MV_PROTECT)){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,5,4);
                          }
                          if ((tplayer->mv_flags[i]&MV_STORMCLOAK)!=(mv_flags[i2]&MV_STORMCLOAK)){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,6,4);
                          }
                          if ((tplayer->mv_flags[i]&MV_TYPING)!=(mv_flags[i2]&MV_TYPING)){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,7,4);
                          }
                          if (tplayer->mv_hpmp[i]!=mv_hpmp[i2]){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,8,4);
                            BITSadd(t,&bitsi,mv_hpmp[i2],8);
                          }
                          if (tplayer->mv_ktar[i]!=mv_ktar[i2]){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,9,4);
                            BITSadd(t,&bitsi,mv_ktar[i2],4);
                          }
                          if ((tplayer->mv_flags[i]&MV_SPEAKING)!=(mv_flags[i2]&MV_SPEAKING)){
                            if (i3==0){i3=1; BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,i,z3);}else{BITSadd(t,&bitsi,1,1);}
                            BITSadd(t,&bitsi,10,4);
                          }
                          if (i3){
                            BITSadd(t,&bitsi,0,1);
                            sceneupdaterequired=1;
                            tplayer->mv_flags[i]=mv_flags[i2];
                            tplayer->mv_hpmp[i]=mv_hpmp[i2];
                            tplayer->mv_ktar[i]=mv_ktar[i2];
                            tplayer->mv_more[i]=mv_more[i2];
                          }
                        }//i
                        BITSadd(t,&bitsi,0,1);//no more movers' states to change/toggle

                        //3. ADD NEW MOVER
                        for (i2=0;i2<mv_i;i2++){
                          if (mv_new[i2]==0){

                            sceneupdaterequired=1;
                            z=1; BITSadd(t,&bitsi,z,1);//add a/another mover
                            x=mv_x[i2]-tpx+1;//0-33
                            y=mv_y[i2]-tpy+1;//0-25
                            z=y*34+x; BITSadd(t,&bitsi,z,10);//x,y offset
                            z=mv_type[i2]; BITSadd(t,&bitsi,z,10);//type YET TO BE OPTOMIZED

                            if (z==413){//silver serp
                              BITSadd(t,&bitsi,mv_frame[i2],4);
                              goto mover_add_special;
                            }
                            if (z==366){//tanglevine tendril (not core)
                              BITSadd(t,&bitsi,mv_frame[i2],3);
                              goto mover_add_special;
                            }
                            //check if direction is required (z3 is number of bits to use to send direction)
                            z3=2;
                            if (obji[sprlnk[z]].v4==3) z3=0;//"bird"
                            if (z==365) z3=0;//tanglevine core
                            if (z==374) z3=0;//hydra core
                            if (z==375) z3=0;//slime
                            if (z==391) z3=0;//farmer hoeing
                            if (z==415) z3=0;//raft
                            BITSadd(t,&bitsi,mv_dir[i2],z3);
mover_add_special:

                            //***STATE CHANGES***
                            if(mv_ktar[i2]){
                              BITSadd(t,&bitsi,1,1);//a/another state change/toggle is required
                              BITSadd(t,&bitsi,9,4);//ktar update
                              BITSadd(t,&bitsi,mv_ktar[i2],4);
                            }
                            if(mv_playerid[i2]){
                              BITSadd(t,&bitsi,1,1);//a/another state change/toggle is required
                              BITSadd(t,&bitsi,8,4);
                              BITSadd(t,&bitsi,mv_hpmp[i2],8);
                              BITSadd(t,&bitsi,mv_playerid[i2],32);
                            }
                            if(mv_flags[i2]&MV_LIGHTBRIGHT){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,0,4);}
                            if(mv_flags[i2]&MV_LIGHTGLOW){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,1,4);}
                            if(mv_flags[i2]&MV_INVISIBLE){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,2,4);}
                            if(mv_flags[i2]&MV_PARALYZE){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,3,4);}
                            if(mv_flags[i2]&MV_SLEEP){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,4,4);}
                            if(mv_flags[i2]&MV_PROTECT){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,5,4);}
                            if(mv_flags[i2]&MV_STORMCLOAK){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,6,4);}
                            if(mv_flags[i2]&MV_TYPING){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,7,4);}
                            if(mv_flags[i2]&MV_SPEAKING){BITSadd(t,&bitsi,1,1); BITSadd(t,&bitsi,10,4);}

                            //mv_more(rider type,...)
                            if(mv_more[i2]){
                              BITSadd(t,&bitsi,1,1);//a/another state change/toggle is required
                              BITSadd(t,&bitsi,11,4);
                              BITSadd(t,&bitsi,mv_more[i2],16);
                            }




                            z=0; BITSadd(t,&bitsi,z,1);//no more state changes required

                            //add temp mover data to client list
                            i=tplayer->mv_i;
                            tplayer->mv_x[i]=mv_x[i2];
                            tplayer->mv_y[i]=mv_y[i2];
                            tplayer->mv_type[i]=mv_type[i2];
                            tplayer->mv_dir[i]=mv_dir[i2];
                            tplayer->mv_frame[i]=mv_frame[i2];
                            tplayer->mv_object[i]=mv_object[i2];
                            tplayer->mv_flags[i]=mv_flags[i2];
                            tplayer->mv_hpmp[i]=mv_hpmp[i2];
                            tplayer->mv_playerid[i]=mv_playerid[i2];
                            tplayer->mv_ktar[i]=mv_ktar[i2];
                            tplayer->mv_more[i]=mv_more[i2];
                            tplayer->mv_i++;

                          }//mv_new[i2]==0
                        }//i2
                        z=0; BITSadd(t,&bitsi,z,1);

                        if (sceneupdaterequired){
                          t->l=bitsi>>3; if (bitsi&7) t->l++; NET_send(NETplayer,tplayer->net,t); tplayer->updatemessage=1;
                        }else{
                          tplayer->updatemessage=0;
                        }

        }else{//tplayer->updatemessage
          tplayer->updatemessage=0;
        }

        /*
        1. remove
        2.1 change
        2.2 move
        3. add


        ***1. REMOVE MOVER***
        1 if =1 remove a/another mover
        {
        [?] index of mover to remove based on current buffer
        }
        ***2.1 CHANGE/TOGGLE STATE***
        1 if =1 change/toggle a/another mover's state
        {
        [?] index of mover in current buffer
        a:
        4 state change id
        0 LIGHTBRIGHT (LIGHTGLOW assumed off)
        1 LIGHTGLOW (LIGHTBRIGHT assumed off)
        2 INVISIBLE
        3 PARALYZE
        4 SLEEP
        5 PROTECT
        6 STORMCLOAK
        7 TYPING
        8 hpmp{		8 new hpmp byte		}
        9 ktar{		4 new ktar number	}
        1 if =1 another state needs to be changed, goto a
        }
        ***2.2 CHANGE FRAME (THIS IS ONLY SENT IF MOVER HAS NOT MOVED TO ANOTHER SQUARE, IN THAT CASE IT CAN BE ASSUMED)***
        1 if =1 change animation frame of a/another mover
        {
        [?] index of mover to change the animation frame of
        }
        ***2.3 MOVE***
        1 if =1 move a/another mover
        {
        [?] index of mover to move in current buffer
        5 number representing a special x,y offset from current location to move to
        }
        ***3. ADD NEW MOVER***
        1 if =1 add a/another mover
        {
        10 x,y offset to add mover
        8-10 (number of bits will depend on yet to be implemented optomization) mover's object type
        2 direction mover is facing
        a:
        1 if =1 a/another state change/toggle is required
        {
        4 state change id
        0 LIGHTBRIGHT (LIGHTGLOW assumed off)
        1 LIGHTGLOW (LIGHTBRIGHT assumed off)
        2 INVISIBLE
        3 PARALYZE
        4 SLEEP
        5 PROTECT
        6 STORMCLOAK
        7 TYPING
        8 hpmp+playerid{	8  new hpmp byte
        32 playerid			}
        9 ktar{				4 new ktar number	}
        goto a
        }
        }
        */








        //flags/pointers used while comparing buffers
        //unsigned long mv_last[MVLISTLAST+1];
        //unsigned long mv_new[MVLISTLAST+1];








        /*
        if (!playerlist[tpl]->updatemessage){
        static unsigned char *p,*p2;
        txtNEWLEN(t,-1048576); //create 1MB buffer
        p=t->d2;

        *p=1; p++;
        *(float*)p=btime2; p+=4;

        //central object offset->x,y
        if (playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
        x=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->x;
        y=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->y;
        }else{
        x=playerlist[tpl]->px; y=playerlist[tpl]->py;
        }
        //screen offset->tpx,tpy
        tpx=x-15; tpy=y-11;
        if (tpx<0) tpx=0; if (tpy<0) tpy=0;
        if (tpx>2016) tpx=2016;
        if (tpy>1000) tpy=1000;

        //add tpx,tpy to message
        *(unsigned short*)p=tpx; p+=2;
        *(unsigned short*)p=tpy; p+=2;

        if (playerlist[tpl]->xray){ //xray!
        for (x2=0;x2<=65535;x2++) pfa[x2]=PFs+PFd;
        }else{
        pathfind2(x,y,NULL);
        }

        //send blank squares
        //with no obstructions this routine will still send 7 bytes (equivalent to only 2 objects!)
        //BUT it is optomized basic obstructions caused by buildings/caves etc.
        //OLD METHOD WAS: starting type(1), number of type(2), (assume type change)number of type(2)
        //NEW METHOD: number of type+type*128(1), ...
        //benefit: halved bandwidth in basic/complex situations
        x4=0; z4=2;
        for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
        if (pfa[x+y*256+257]&(PFs+PFd)) z5=1; else z5=0;
        if (z5!=z4){
        if (z4!=2){ //not the first pass
        *p=x4+z4*128; p++;
        }//z4!=2
        x4=0; //number of squares in a row -1
        z4=z5; //type (1 or 0)
        }else{
        if (x4==127){
        *p=127+z4*128; p++;
        x4=0;
        }else{
        x4++;
        }
        }
        }}//x,y
        *p=x4+z4*128; p++; //save last row


        //update basetiles message
        //update objects message
        static long updatex,updatey;
        x2=0; x3=0;
        p2=p; p+=2; //create 2 bytes space to set number of objects
        for (y=0;y<=23;y++){ for (x=0;x<=31;x++){
        updatex=tpx+x; updatey=tpy+y;

        myobj=od[updatey][updatex];

        nextobj: if (myobj){

        if (myobj->type==416){//view redirector
        if (myobj->more2){
        bt[updatey][updatex]=bt[myobj->more2>>16][myobj->more2&65535]; //update basetile flags (is this best location?)
        updatex=myobj->more2&65535; updatey=myobj->more2>>16;//change currect scan location
        myobj=od[myobj->more2>>16][myobj->more2&65535];//change current object
        goto nextobj;
        }
        }//view redirector



        i2=myobj->type&1023; //get object identity

        if (myobj->info&256){ //quest/don'tsend
        if (obji[sprlnk[i2]].weight==0){
        goto display_skip;
        }

        if (i2==73){ //moonstone
        if (playerlist[tpl]->GNPCflags[4]&(1<<(myobj->type>>10))) goto display_skip;
        }

        if (myobj->type==(275+2*1024)){//yellow potion
        if (myobj->info&256){//quest
        if (playerlist[tpl]->GNPCflags[32]&myobj->more2) goto display_skip;
        }//quest
        }//yellow potion

        if ((i2>=242)&&(i2<=249)){ //rune
        if (playerlist[tpl]->GNPCflags[5]&(1<<(i2-242))) goto display_skip;
        }
        if ((i2>=400)&&(i2<=407)){ //map(0-7)
        if (playerlist[tpl]->GNPCflags[7]&(1<<(i2-400))) goto display_skip;
        }
        if (i2==62){ //vortex cube
        if ((myobj->x==923)&&(myobj->y==850)){
        if ((playerlist[tpl]->GNPCflags[18]&1)==0) goto display_skip;
        }
        if ((myobj->x==1939)&&(myobj->y==57)){
        if (playerlist[tpl]->GNPCflags[6]&(1<<1)) goto display_skip;
        }
        }
        if (i2==389){ //silver tablet
        if (playerlist[tpl]->GNPCflags[6]&(1<<2)) goto display_skip;
        }
        if (i2==390){ //silver tabletII
        if (playerlist[tpl]->GNPCflags[6]&(1<<3)) goto display_skip;
        }
        if (i2==270){ //balloon plans
        if (playerlist[tpl]->GNPCflags[6]&(1<<4)) goto display_skip;
        }
        if (i2==395){ //broken gargoyle lens
        if (playerlist[tpl]->GNPCflags[6]&(1<<5)) goto display_skip;
        }
        if (i2==394){ //brit lens
        if ((playerlist[tpl]->GNPCflags[18]&2)==0) goto display_skip;
        }
        if (i2==396){ //garg lens
        if ((playerlist[tpl]->GNPCflags[18]&4)==0) goto display_skip;
        }
        }//quest/don't send

        if (myobj==nuggetsfix){ //nuggets
        if (playerlist[tpl]->online<1536) goto display_skip;
        if (playerlist[tpl]->GNPCflags[6]&(1<<6)) goto display_skip;
        }

        if (i2==59){ //codex
        if (playerlist[tpl]->GNPCflags[18]&8) goto display_skip;
        }

        if (i2==51){ //force field
        myobj2=(object*)myobj->prev;
        if (playerlist[tpl]->GNPCflags[3]&(1<<(myobj2->type>>10))) goto display_skip;
        }//force field

        if (i2==173){ //trap
        if ((myobj->info&(1<<9))==0) goto display_skip;
        }//trap

        crt=NULL;
        if (myobj->info&4){ //crt
        crt=(creature*)myobj->more;
        if (crt->flags&2) goto display_skip;
        }else{//!crt
        if (myobj->info&8){ //indirect crt
        if (((object*)myobj->more)->info&4) crt=(creature*)((object*)myobj->more)->more;
        }
        }//crt

        if (pfa[x+2+y*256+512]&(PFs+PFd)){

        //DISPLAY DOORS ON RANGE EDGES???

        //SUPPORT FOR VIEW REDIRECTORS
        if (i2==334) goto display_override; //secret door
        if (i2==213) goto display_override; //mousehole
        if (myobj2=OBJfindlastall(updatex,updatey)){
        if ((myobj2->type&1023)==301) goto display_override;
        }
        if (myobj2=OBJfindlastall(updatex,updatey+1)){
        if ((myobj2->type&1023)==301) goto display_override;
        }
        if (myobj2=OBJfindlastall(updatex+1,updatey)){
        if ((myobj2->type&1023)==301) goto display_override;
        }




        if (pfa[x+2+y*256+512+1]&(PFs+PFd)){
        if (pfa[x+2+y*256+512+256]&(PFs+PFd)){
        if (pfa[x+2+y*256+512+256+1]&(PFs+PFd)){
        display_override:

        //length from prev obj (stored extendable from 1 to 2 bytes)
        if (x2<=127){
        *p=x2; p++;
        }else{ //>127
        *p=(x2&127)+128; p++;
        *p=x2>>7; p++;
        }


        if (myobj->info&128){//stolen item
        for (z=0;z<=15;z++){
        if (stolenitemwarning[z]==myobj){
        if (stolenitemwarning_player[z]!=playerlist[tpl]){
        *(unsigned short*)p=1023+7*1024; p+=2;
        }
        }//==myobj
        }//z
        }//stolen item


        if (myobj->info&4){ //crt
        if (crt->flags&8){ //paralyzed!
        *(unsigned short*)p=1023+3*1024; p+=2;
        }//8, paralyzed
        if (playerlist[tpl]->ktar_display){
        for (z=0;z<=9;z++){
        if (playerlist[tpl]->ktar_wait[z]){
        if ((object*)playerlist[tpl]->ktar[z]->more==myobj){
        *(unsigned short*)p=1022+z*1024; p+=2;
        }
        }
        }
        }
        }//crt

        if (crt){ //direct or indirect crt
        //sleeping?
        if (crt->flags&4){
        z=myobj->type&1023; z2=0;
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024;
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        //monsters
        if (z==370) z2=339+1*1024; //headless
        if (z==371) z2=339+0*1024; //troll
        if (z==424){ //cyclops
        z2=340+((myobj->type>>10)&3)*1024;
        if (((myobj->type>>10)&3)!=1){
        if (bt[y+tpy][x+tpx]&1024) bt[y+tpy][x+tpx]-=1024; //set as blocked!
        }
        }
        if (z2){
        *(unsigned short*)p=z2; p+=2; goto type_or;
        }
        }//sleeping?
        }//direct or indirect crt

        if (myobj->info&2){
        tnpc=(npc*)myobj->more;

        if (tnpc->player){ //has player pointer
        if (tnpc->protection){*(unsigned short*)p=1023+4*1024; p+=2;}

        if (tnpc->light){
        *(unsigned short*)p=1023+1*1024; p+=2;
        }else{
        if (tnpc->player==playerlist[tpl]){
        *(unsigned short*)p=1023+2*1024; p+=2;
        }//->light
        }//tnpc->player

        if (myobj3=tnpc->armour){
        if (myobj3->type==81){ //storm cloak
        *(unsigned short*)p=1023+6*1024; p+=2;
        }//81
        }//armour


        if (obji[sprlnk[myobj->type&1023]].v4==1){ //humantype
        tplayer2=(player*)tnpc->player;

        if (tplayer2->party[0]==myobj){ //first player


        //first player of their party
        if (tplayer2->typing){
        if (tplayer!=tplayer2){ //not the same party member
        *(unsigned short*)p=1023+5*1024; p+=2;
        }
        }

        *(unsigned short*)p=myobj->type+16384; //16384=extra info
        if (tnpc->flags&4){ //if asleep switch to sleeping object
        z=myobj->type&1023; z2=0;
        //sleeping NPC schedule, but not in bed
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024; //2004
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        if (z2!=0){
        *(unsigned short*)p=z2+16384; //16384=extra info
        }
        }//asleep
        if (tnpc->flags&2) *(unsigned short*)p|=32768; //invisible
        p+=2;

        //U6OID    4bytes vs name(10)+colour(2)+unique id for messaging
        *(unsigned long*)p=tplayer2->id; p+=4;

        //addhp&mp bars (uses 1 byte of code) 0-15, 0-7, 1bit stating poisioned
        f=(float)tnpc->hp/(float)tnpc->hp_max; z=f*16; if (z==16) z=15;
        f=(float)tnpc->mp/(float)tnpc->mp_max; z2=f*8; if (z2==8) z2=7;
        z=z+(z2<<4); if (tnpc->flags&1) z+=128;
        *p=z; p++;
        goto type_or;
        }

        }
        }

        if (tnpc->flags&(2+4)){//2,4
        *(unsigned short*)p=myobj->type;
        if (tnpc->flags&4){ //if asleep switch to sleeping object
        z=myobj->type&1023; z2=0;
        //sleeping NPC schedule, but not in bed
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024; //2004
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        if (z2!=0){
        *(unsigned short*)p=z2;
        }
        }//asleep
        if (tnpc->flags&2) *(unsigned short*)p|=32768; //invisible
        p+=2;
        goto type_or;
        }//2,4

        if (tnpc->schedule){ if (tnpc->schedule_i!=-1){ if (tnpc->schedule[tnpc->schedule_i].type==0x91){ //sleeping!
        if (od[y+tpy][x+tpx]->type==163){
        *(unsigned short*)p=146; p+=2; goto type_or;
        }
        if (od[y+tpy][x+tpx]->type==(163+6144)){
        *(unsigned short*)p=146+1024; p+=2; goto type_or;
        }
        z=myobj->type&1023;
        z2=0;
        //sleeping NPC schedule, but not in bed
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024; //2004
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        if (z2!=0){
        *(unsigned short*)p=z2; p+=2; goto type_or;
        }
        }}} //sleeping!

        }//->npc

        *(unsigned short*)p=myobj->type;
        if ((myobj->type&1023)==338) goto noflash; //blood
        if ((myobj->type&1023)==317) goto noflash; //317		fire field
        if ((myobj->type&1023)==318) goto noflash; //318		poison field
        if ((myobj->type&1023)==319) goto noflash; //319		protection field
        if ((myobj->type&1023)==320) goto noflash; //320		sleep field
        if ((myobj->type&1023)==84){ //red gate flash
        if (((myobj->info&112)>>4)>=2) goto flash;
        goto noflash;
        }
        if ((myobj->info&112)==16){
        flash:
        *(unsigned short*)p|=32768; //flashing
        }
        noflash:

        p+=2;
        type_or:

        x3++; //number of objects
        x2=0;

        }
        }
        }

        }//pf2!

        display_skip:
        if (myobj->next!=NULL){myobj=(object*)myobj->next; goto nextobj;}
        } //object exists
        x2++;
        }} //x,y loop
        *(unsigned short*)p2=x3;

        //selected partymember map position (if x==0 he is not onscreen)
        if (myobj=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
        *(unsigned short*)p=myobj->x; p+=2;
        *(unsigned short*)p=myobj->y; p+=2;
        //selected partymember type
        *(unsigned short*)p=myobj->type; p+=2;
        }else{
        *(unsigned short*)p=0; p+=2;
        *(unsigned short*)p=0; p+=2;
        //selected partymember type
        *(unsigned short*)p=0; p+=2;
        }



        *p=0; t->l=(unsigned long)p-(unsigned long)t->d2; //finish direct string access


        static unsigned long chksum;
        chksum=0;
        for (x=5;x<t->l;x++){
        chksum=chksum+1+(t->d2[x]+1)*(x+1);
        }
        if (chksum!=playerlist[tpl]->MESSAGEupdate_chksum){
        NET_send(NETplayer,playerlist[tpl]->net,t);
        playerlist[tpl]->MESSAGEupdate_chksum=chksum;
        playerlist[tpl]->updatemessage=1; //a basetile+object update message was sent last "clientframe"
        } //chksum!=

        }else{playerlist[tpl]->updatemessage=0;} //updatemessage
        */


        //2003: fix input fields
        playerlist[tpl]->key2=playerlist[tpl]->key; //2003: required backup key (client and host)
        playerlist[tpl]->readmessage=FALSE; //basic input not overwritten by second message

        //clear some keys by default while keeping others
        if (playerlist[tpl]->key&KEYmb1) playerlist[tpl]->key-=KEYmb1;
        if (playerlist[tpl]->key&KEYu) playerlist[tpl]->key-=KEYu;
        if (playerlist[tpl]->key&KEYl) playerlist[tpl]->key-=KEYl;
        if (playerlist[tpl]->key&KEYmbclick) playerlist[tpl]->key-=KEYmbclick;


        if (sfbuffersend){
          //create SF message 
          // send sfx message
          //[2]how many sfx
          //[sfx2]
          //...
          txtset(mess1,"");
          txtset(t,"?");
          txtset(t2,"??");
          txtset(t3,"????");
          txtadd(mess1,t2); //<-must be updated later
          i=0;

          if (playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
            x=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->x;
            y=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->y;
          }else{
            x=playerlist[tpl]->px; y=playerlist[tpl]->py;
          }

          //tpx=x-15; tpy=y-11;
          //if (tpx<0) tpx=0; if (tpy<0) tpy=0;
          //if (tpx>2016) tpx=2016;
          //if (tpy>1000) tpy=1000;
          getscreenoffset(x,y,&tpx,&tpy);

          tpx>>=3; tpy>>=3;
          for (y=tpy;y<=tpy+3;y++){
            for (x=tpx;x<=tpx+4;x++){
sfxglobal:
              if (sfi[y][x]){
                i3=sfi[y][x];
addsf:
                t2->d2[0]=sf[i3].type&255;
                t2->d2[1]=sf[i3].type>>8; txtadd(mess1,t2);
                t2->d2[0]=sf[i3].x&255;
                t2->d2[1]=sf[i3].x>>8; txtadd(mess1,t2);
                t2->d2[0]=sf[i3].y&255;
                t2->d2[1]=sf[i3].y>>8; txtadd(mess1,t2);
                static txt* dbgt3;
                if (sf[i3].type==SF_TXT_PORTRAIT){
                  dbgt3=(txt*)sf[i3].p; //length of appended text message
                  //dbgt3=(txt*)&dbgt3;
                  //exit (dbgt3->l);
                  t2->ds[0]=dbgt3->l&0xFFFF; txtadd(mess1,t2);
                  t2->ds[0]=0; txtadd(mess1,t2); //unused
                }else{
                  t2->d2[0]=sf[i3].x2&255;
                  t2->d2[1]=sf[i3].x2>>8; txtadd(mess1,t2);
                  t2->d2[0]=sf[i3].y2&255;
                  t2->d2[1]=sf[i3].y2>>8; txtadd(mess1,t2);
                }
                t2->d2[0]=sf[i3].more&255;
                t2->d2[1]=sf[i3].more>>8; txtadd(mess1,t2);
                t2->d2[0]=sf[i3].wait2[0];
                t2->d2[1]=sf[i3].wait2[1]; txtadd(mess1,t2);
                t2->d2[0]=sf[i3].wait2[2];
                t2->d2[1]=sf[i3].wait2[3]; txtadd(mess1,t2);
                if (sf[i3].type==SF_TXT_PORTRAIT){
                  txtadd(mess1,dbgt3); //append message
                }
                i++; //nsf
                if (sf[i3].next) {i3=sf[i3].next; goto addsf;}
              } //sfi!=NULL

              if ((x==(2047>>3))&&(y==(1023>>3))) goto sfxglobal2;
              if ((y==tpy+3)&&(x==tpx+4)) {x=2047>>3; y=1023>>3; goto sfxglobal;}

            }} //x,y
sfxglobal2:
          mess1->d2[0]=i&255; mess1->d2[1]=i>>8; //set number of entries

          //if (playerlist[tpl]->net!=NULL){
          if (mess1->l>2){ //avoid sending a NULL buffer
            txtset(t,"?");
            t->d2[0]=MSG_SF; //update SF
            txtadd(t,mess1);
            NET_send(NETplayer,playerlist[tpl]->net,t);
          }

        }//sfbuffersend



      }//net connection
    }//current player
idle_skip:;
  } //tpl loop

  if (sfbuffersend){
    //clear sf buffer
    sfn=0;//NULL sf pointer (0 is never used)
    for (y=0;y<=255;y++){ for (x=0;x<=255;x++){ //clear sf buffer
      if (i3=sfi[y][x]){
        if (sf[i3].type==SF_TXT_PORTRAIT) free((txt*)sf[i3].p); //free txt
        sfi[y][x]=0;
      }
    }}
    sfbuffersend=0;
  }//sfbuffersend

  //sockets: add new client
  if (newsocket!=INVALID_SOCKET){
    //static unsigned char socket1taken=1;//changes to 2 once socket1 is taken

    for (i=1;i<=socketclientlast;i++){
      if (socketclient[i]==INVALID_SOCKET){goto free_socketclient;}
    }
    socketclientlast++; i=socketclientlast;
free_socketclient:
    //if (i==1) socket1taken=2;

    socketclient[i]=newsocket;
    socketclient_ip[i]=newsocket_ip;

    //(re)init info
    socketclient_packetsize[i]=0;
    socketclient_verified[i]=0;
    socket_timeout[i]=0;
    socket_disconnect[i]=0;
    if (socketclient_si[i]){//reinit thread info

      /*
      unsigned short i;//u6o socket connection array index
      OK txt *d[256];//array of txt*s
      OK unsigned char next;
      OK unsigned char nextfree;
      //other variables for temp use?
      ? long x4,x2,y2;
      txt *t;
      unsigned char exit_thread;
      unsigned long thread_id;
      */

      for (i2=0;i2<=255;i2++){ txtNEWLEN(socketclient_si[i]->d[i2],0); txtNEWLEN(socketclient_ri[i]->d[i2],0);}
      socketclient_si[i]->next=0; socketclient_si[i]->nextfree=0;
      socketclient_ri[i]->next=0; socketclient_ri[i]->nextfree=0;


      socketclient_si[i]->i=newsocket; socketclient_ri[i]->i=newsocket;
      socketclient_si[i]->exit_thread=0; socketclient_ri[i]->exit_thread=0;
      socketclient_ri[i]->t->l=0; socketclient_ri[i]->t->d2[0]=0;

    }else{//init thread info

      socketclient_si[i]=(sockets_info*)malloc(sizeof(sockets_info)); socketclient_ri[i]=(sockets_info*)malloc(sizeof(sockets_info));
      ZeroMemory(socketclient_si[i],sizeof(sockets_info)); ZeroMemory(socketclient_ri[i],sizeof(sockets_info));
      socketclient_si[i]->i=newsocket; socketclient_ri[i]->i=newsocket;
      for (i2=0;i2<=255;i2++){ socketclient_si[i]->d[i2]=txtnew(); socketclient_ri[i]->d[i2]=txtnew();}
      socketclient_si[i]->next=0; socketclient_si[i]->nextfree=0; socketclient_ri[i]->next=0; socketclient_ri[i]->nextfree=0;
      socketclient_ri[i]->t=txtnew(); txtNEWLEN(socketclient_ri[i]->t,-65536);
      socketclient_ri[i]->t->l=0; socketclient_ri[i]->t->d2[0]=0;

    }


    //COULD THIS BE THE BUG? IT DOESN'T CREATE A SEND SOCKET FOR I=1
    //if (i!=1){
    CreateThread(NULL,0,sockets_send,(void*)i,0,(unsigned long*)&socketclient_si[i]->thread_id);
    //}else{
    //socketclient_si[i]->thread_id=0;
    //}
    CreateThread(NULL,0,sockets_receive,(void*)i,0,(unsigned long*)&socketclient_ri[i]->thread_id);

    newsocket=INVALID_SOCKET;
  }

  static unsigned long tnet;
nextclientmessage:

  x3=0;//message available flag

  for (i=1;i<=socketclientlast;i++){ if (socketclient[i]!=INVALID_SOCKET){
    //if socket has not sent any info (including keep-alive messages) for 8 minutes, then it is disconnected
    f=64.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
      socket_timeout[i]++; if (socket_timeout[i]>=8){
        if (socket_disconnect[i]==0) socket_disconnect[i]=1;
      }
    }

    if (socket_disconnect[i]==1){
      for (i2=0;i2<=playerlist_last;i2++){ if (playerlist[i2]){
        if (playerlist[i2]->net==i) playerlist[i2]->net=INVALID_NET;//set ->net pointer as invalid
      }}//playerlist[i2],i2
      CreateThread(NULL,0,sockets_disconnect,(void*)i,0,(unsigned long*)&i2);
      socket_disconnect[i]=2;//socket has been disconnected
      goto socket_disconnected;
    }

    //net_read()
    if (socketclient_ri[i]->d[socketclient_ri[i]->next]->l){ //buffered data available
      socket_timeout[i]=0;
      txtset(t,socketclient_ri[i]->d[socketclient_ri[i]->next]);
      txtNEWLEN(socketclient_ri[i]->d[socketclient_ri[i]->next],0);
      socketclient_ri[i]->next++;
      x3=1; tnet=i;
      goto host_gotmessage;
    }//->l
  }//!=INVALID_SOCKET
socket_disconnected:;
  }//i
host_gotmessage:


  if (x3){
    if (t->d2[0]==255) goto player_add;
    if (t->d2[0]==249) goto player_add; //host character!


    if (t->d2[0]==246){//check if player username AND/OR character name is available
      txtright(t,t->l-1);
      i=t->d2[0]; txtright(t,t->l-1); txtset(t2,t); txtleft(t2,i);//user name
      txtright(t,t->l-i);
      i=t->d2[0]; txtright(t,t->l-1); txtset(t3,t); txtleft(t3,i);//character name
      txtright(t,t->l-i);
      //format names
      txtset(tusername,t2);
      txtset(tname,t3);
      format_usernames=1;
      goto format_usernames0;
format_usernames1:
      format_usernames=0;

      //MessageBox(NULL,tusername->d,"Ultima 6 Online",MB_OK);
      //MessageBox(NULL,tname->d,"Ultima 6 Online",MB_OK);

      //check if names exist in saves
      i2=0;
      for (i=0;i<=SAVESLOTLAST;i++){ //search save buffer for player info!
        if (save_buffer[i]){
          if (txtsamewithoutcase(save_username[i],tusername)){
            i2=1;
          }
          if (txtsamewithoutcase(save_name[i],tname)){
            i2=2;
          }
        }//save_buffer[i]
      }//i
      //check if names exist in currently online players
      //todo!
      txtsetchar(t,245); txtaddchar(t,i2);
      NET_send(NETplayer,tnet,t);
      goto doneclmess;
    }

    //KEEP ALIVE MESSAGE 251 IS POSSIBLE WITH OR WITHOUT A LOGGED IN PLAYER

    //check playerlist[] for tnet
    for (tpl=0;tpl<=playerlist_last;tpl++){
      if (playerlist[tpl]){ //current player
        if (playerlist[tpl]->net==tnet) goto foundclient;
      }//current player
    }

    goto doneclmess; //ignore message
player_add: //add new player



    //send time
    txtset(t2,"?"); t2->d2[0]=32;
    txtset(t3,"12345678"); t3->dd[0]=btime2;
    txtadd(t2,t3);
    NET_send(NETplayer,tnet,t2);

    static unsigned char usetpl0;
    usetpl0=0; if (t->d2[0]==249) usetpl0=1;


    txtset(t5,t); //t5<-t
    txtright(t5,t5->l-1);

    if (t5->ds[0]!=U6O_VERSION){ //version 5.5 check
      txtset(t,"?"); t->d2[0]=254; NET_send(NETplayer,tnet,t); //incorrect version
      txtset(t,"Add_Player->Failed:Incorrect_Version"); LOGadd(t);
      goto doneclmess;
    }
    txtright(t5,t5->l-2);
    
    y9=t5->d2[0];//pw encryption
    txtright(t5,t5->l-1);
    //get setup message info

    x=t5->d2[0]; txtright(t5,t5->l-1); txtNEWLEN(tname,x); memcpy(tname->d2,t5->d2,x); txtright(t5,t5->l-x); //name
    tmale_female=t5->d2[0]; txtright(t5,t5->l-1); if (tmale_female>1) tmale_female=1;
    tport=194; x=t5->ds[0]; if ((x>=0)&&(x<=13)) tport=194+x; txtright(t5,t5->l-2);
    ttype=410; x=t5->d2[0]; if ((x>0)&&(x<=12)) ttype=375+x; txtright(t5,t5->l-1);
    txtset(t6,t5); txtleft(t6,28); txtright(t5,t5->l-28); //t6<-virtue answers
    x=t5->d2[0]; txtright(t5,t5->l-1); txtNEWLEN(tusername,x); memcpy(tusername->d2,t5->d2,x); txtright(t5,t5->l-x); txtucase(tusername); //user_name
    x=t5->d2[0]; txtright(t5,t5->l-1); txtNEWLEN(tuserpassword,x); memcpy(tuserpassword->d2,t5->d2,x); txtright(t5,t5->l-x); //user_password
    /*if (y9==0) {
      txtucase(tuserpassword); //user_password
    }*/
    tcreatecharacter=t5->d2[0];
    txtright(t5,t5->l-1);
    x=t5->d2[0]; txtright(t5,t5->l-1); txtNEWLEN(tnewuserpassword,x); memcpy(tnewuserpassword->d2,t5->d2,x); txtright(t5,t5->l-x); //new_user_password
    /*if (y9==0) {
      txtucase(tnewuserpassword); //new_user_password
    }*/
    tcustomportrait_upload=0;
    tnamecolour=0;
getsetupmessageinfo_more:
    if (t5->l){//... (more info)
      if (t5->d2[0]==1){//upload character portrait
        tcustomportrait_upload=1; txtright(t5,t5->l-1);
        memcpy(&tcustomportrait,t5->d,7168); txtright(t5,t5->l-7168);
        goto getsetupmessageinfo_more;
      }

      if (t5->d2[0]==2){ txtright(t5,t5->l-1);//u6o_namecolour
      tnamecolour=t5->dl[0]&0xFFFFFF; txtright(t5,t5->l-3);
      goto getsetupmessageinfo_more;
      }//u6o_namecolour


      if (u6o_namecolour){
        txtaddchar(t,2);
        txtaddshort(t,u6o_namecolour&65535); txtaddchar(t,(u6o_namecolour>>16)&255);
      }//u6o_namecolour


    }//t5->l



    //autoformat: name, username, password, newuserpassword (all temp)
format_usernames0:
    //name
    txtset(t5,tname);
autoformat_name_cull2:
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
      if (x==95){t5->d2[i3]=32; x=32; x2=0; goto autoformat_name_cull2;} //_ to space
      if ((x==32)&&(t5->d2[i3+1]!=32)){
        if (i3&&(i3!=(t5->l-1))) x2=0;
      }
      if (x2){
        txtset(t6,t5); txtright(t6,t5->l-i3-1);
        txtleft(t5,i3); txtadd(t5,t6);
        goto autoformat_name_cull2;
      }
    }
    if (t5->l>16) txtleft(t5,16);
    txtset(tname,t5);
    //username
    txtset(t5,tusername);
    txtucase(t5);
autoformat_username_cull2:
    for (i3=0;i3<t5->l;i3++){
      x=t5->d2[i3];
      x2=0; //cull?
      if (x<48) x2=1;
      if (x>90) x2=1;
      if ((x>57)&&(x<65)) x2=1;
      if (x2){
        txtset(t6,t5); txtright(t6,t5->l-i3-1);
        txtleft(t5,i3); txtadd(t5,t6);
        goto autoformat_username_cull2;
      }
    }
    if (t5->l>16) txtleft(t5,16);
    txtset(tusername,t5);
    if (format_usernames) goto format_usernames1;
    //password
    if (y9==SHA2_ENCRYPTION) {
      //add some salt and hash it again for save file
      txtset(t9,tusername); 
      txtadd(t9,tuserpassword);
      if (txtsame(tnewuserpassword,tuserpassword)) txtset(tnewuserpassword,"");
      SHA2(t9,tuserpassword);
      txtset(t,"UNKNOWN"); SHA2(t,t2); if (txtsame(tnewuserpassword,t2)) txtset(tnewuserpassword,"");
      if(tnewuserpassword->l){
        txtset(t9,tusername); 
        txtadd(t9,tnewuserpassword);
        SHA2(t9,tnewuserpassword);
      }
    }
    else {
      txtset(t5,tuserpassword);
      if (PW_ENCRYPTION==SHA2_ENCRYPTION) {//encrypt the old password and set as new pw
        SHA2(tuserpassword,tnewuserpassword);
        txtset(t9,tusername); 
        txtadd(t9,tnewuserpassword);
        SHA2(t9,tnewuserpassword);
      }
      txtucase(t5);
autoformat_password_cull2:
      for (i3=0;i3<t5->l;i3++){
        x=t5->d2[i3];
        x2=0; //cull?
        if (x<48) x2=1;
        if (x>90) x2=1;
        if ((x>57)&&(x<65)) x2=1;
        if (x2){
          txtset(t6,t5); txtright(t6,t5->l-i3-1);
          txtleft(t5,i3); txtadd(t5,t6);
          goto autoformat_password_cull2;
        }
      }
      if (t5->l>16) txtleft(t5,16);
      txtset(tuserpassword,t5);
      //new password
      txtset(t5,tnewuserpassword);
      txtucase(t5);
autoformat_password_cull4:
      for (i3=0;i3<t5->l;i3++){
        x=t5->d2[i3];
        x2=0; //cull?
        if (x<48) x2=1;
        if (x>90) x2=1;
        if ((x>57)&&(x<65)) x2=1;
        if (x2){
          txtset(t6,t5); txtright(t6,t5->l-i3-1);
          txtleft(t5,i3); txtadd(t5,t6);
          goto autoformat_password_cull4;
        }
      }
      if (t5->l>16) txtleft(t5,16);
      txtset(tnewuserpassword,t5);
      //really a new password?
      txtset(t,"UNKNOWN"); if (txtsame(tnewuserpassword,t)) txtset(tnewuserpassword,"");
      if (txtsame(tnewuserpassword,tuserpassword)) txtset(tnewuserpassword,"");
      if (PW_ENCRYPTION==SHA2_ENCRYPTION) {//encrypt the old password and set as new pw
        SHA2(t9,tnewuserpassword);
      }
    }

    //NULL username, password or character-name?
    x=0;
    if (tusername->l==0) x=1;
    if (tuserpassword->l==0) x=1;
    if (tname->l==0) x=1;
    //"UNKNOWN" username or password?
    if (txtsame(tusername,t)) x=1;
    if (txtsame(tuserpassword,t)) x=1;
    if (x){
      txtset(t,"??"); t->d2[0]=250; t->d2[1]=2; NET_send(NETplayer,tnet,t); goto doneclmess;
    }

    static long journeyonward_i;

    if (tcreatecharacter){
      //username and character-name must be unique
      for (i=0;i<=SAVESLOTLAST;i++){ if (save_buffer[i]){
        if (txtsame(save_username[i],tusername)){
          txtset(t,"??"); t->d2[0]=250; t->d2[1]=3; NET_send(NETplayer,tnet,t); goto doneclmess;
        }
        txtset(t,save_name[i]); txtucase(t); txtset(t2,tname); txtucase(t2);
        if (txtsame(t,t2)){
          txtset(t,"??"); t->d2[0]=250; t->d2[1]=4; NET_send(NETplayer,tnet,t); goto doneclmess;
        }
      }}
    }else{ //!tcreatecharacter
      //username and password must exist
      for (i=0;i<=SAVESLOTLAST;i++){ if (save_buffer[i]){
        if (txtsame(save_username[i],tusername)){
          if (txtsame(save_password[i],tuserpassword)) {journeyonward_i=i; goto check_username_password_ok;}
          if (y9==PW_ENCRYPTION) {
            txtset(t,"???"); t->d2[0]=250; t->d2[1]=6; t->d2[2]=0; NET_send(NETplayer,tnet,t); goto doneclmess; //request password without encryption (later replace with save file spesific encryption if needed)
          }
          else {
            //journeyonward_i=i; goto check_username_password_ok; //for pw reset
            txtset(t,"??"); t->d2[0]=250; t->d2[1]=5; NET_send(NETplayer,tnet,t); goto doneclmess;
          }
        }
      }}//i,save_buffer
      if (y9==PW_ENCRYPTION) {
        txtset(t,"???"); t->d2[0]=250; t->d2[1]=6; t->d2[2]=0; NET_send(NETplayer,tnet,t); goto doneclmess; //request password without encryption (later replace with save file spesific encryption if needed)
      }
      else {
        txtset(t,"??"); t->d2[0]=250; t->d2[1]=5; NET_send(NETplayer,tnet,t); goto doneclmess;
      }
    }//tcreatecharacter check
check_username_password_ok:

    //if player is online, re-assign net pointer
    if (!tcreatecharacter){
      for (i=0;i<=playerlist_last;i++){
        if (playerlist[i]){ //current player
          if (txtsame(tusername,playerlist[i]->name)){
            if (txtsame(tuserpassword,playerlist[i]->password)){
              if (socket_disconnect[playerlist[i]->net]==0) socket_disconnect[playerlist[i]->net]=1;
              playerlist[i]->net=tnet; //re-establish connection
              for (i2=0;i2<=7;i2++){ if (playerlist[i]->party[i2]){
                tnpc=(npc*)playerlist[i]->party[i2]->more; tnpc->upflags=1;
              }//party
              }//i2

              //flush object & mover buffers
              playerlist[i]->x=0;
              playerlist[i]->y=0;
              playerlist[i]->sobj_bufoffx=0;
              playerlist[i]->sobj_bufoffy=0;
              playerlist[i]->mv_i=0;
              for (y=0;y<=71;y++){ for (x=0;x<=95;x++){
                playerlist[i]->sobj_bufsize[x][y]=0;
                playerlist[i]->sobj_tempfixed[x][y]=0;
              }}

              playerlist[i]->idle=0;
              playerlist[i]->idle_connect=0;

              goto doneclmess; //player never left!
            }}//name,password
        }//current player
      }//i
    }//!tcreatecharacter

    //create player structure (using default values)
    if (usetpl0){tpl=0; i=0; playerlist_last++; goto gotfreeindex;}
    for (i=1;i<=playerlist_last;i++){
      if (playerlist[i]==NULL){ //free index
        tpl=i; goto gotfreeindex;
      }//free index
    }//i
    playerlist_last++; tpl=playerlist_last; i=tpl; //no free index, allocate new index
gotfreeindex:

    playerlist[tpl]=(player*)malloc(sizeof(player)); ZeroMemory(playerlist[tpl],sizeof(player));



    playerlist[tpl]->talk_target=OBJnew();


    for (x=0;x<=9;x++) playerlist[tpl]->ktar[x]=OBJnew(); //setup NULL keyboard targeting objects
    playerlist[tpl]->net=tnet;
    playerlist[tpl]->name=txtnew(); txtset(playerlist[tpl]->name,tusername);
    playerlist[tpl]->password=txtnew(); txtset(playerlist[tpl]->password,tuserpassword);
    playerlist[tpl]->newpassword=txtnew(); txtset(playerlist[tpl]->newpassword,tnewuserpassword);
    playerlist[tpl]->idle=0;
    playerlist[tpl]->idle_connect=0;
    playerlist[tpl]->karma=100;
    playerlist[tpl]->male_female=tmale_female;
    playerlist[tpl]->mixer_volume=255;//assume 100% mixer volume until updated

    //add player's sprite
    myobj=OBJnew();
    playerlist[i]->party[0]=myobj; //player<-
    myobj->type=ttype; //avatar (default)
    OBJadd(256+128-16,256+128+2,myobj); //place avatar on the map

    //Step 3: create a NPC structure, the player's stats
    tnpc=(npc*)malloc(sizeof(npc));
    ZeroMemory(tnpc,sizeof(npc));
    myobj->more=tnpc; myobj->info|=2; //myobj<-
    //create an INVISIBLE container for player's items
    myobj2=OBJnew();
    myobj2->type=188; //bag (INVISIBLE)
    myobj2->info|=256; //quest items OK
    tnpc->items=myobj2;
    tnpc->baseitem=myobj2;
    tnpc->port=tport;
    tnpc->order=0;
    tnpc->follow=playerlist[tpl]->party[0];
    tnpc->name=txtnew();
    tnpc->player=playerlist[tpl];
    tnpc->upflags=1;
    if (tcreatecharacter) goto save_createcharacter;

    i=journeyonward_i;
    txtset(t2,".\\save\\"); txtnumint(t,i); txtadd(t,".sav"); txtadd(t2,t);
    tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT); txtNEWLEN(t4,lof(tfh)); get(tfh,t4->d2,t4->l); close(tfh);

    save_version=t4->ds[0];
    if (save_version==3){ //DECRYPT3.0
      decrypt(t4);
      save_version=2;
    }

    txtright(t4,t4->l-2);
    if (save_version==5){
      txtright(t4,t4->l-1);//for now do nothing with pw encrypt version will be useful later if some one implements a new encryption
    }
    x=t4->d2[0]; txtright(t4,t4->l-1-x); //skip name (already known)
    x=t4->d2[0]; txtright(t4,t4->l-1-x); //skip password (already known)

    x=t4->ds[0]; txtright(t4,t4->l-2); y=t4->ds[0]; txtright(t4,t4->l-2);
    OBJremove(playerlist[tpl]->party[0]); OBJadd(x,y,playerlist[tpl]->party[0]);
    playerlist[tpl]->male_female=t4->d2[0]; txtright(t4,t4->l-1);
    memcpy(&playerlist[tpl]->NPCflags,t4->d,256*4); txtright(t4,t4->l-(256*4));
    memcpy(&playerlist[tpl]->GNPCflags,t4->d,256*4); txtright(t4,t4->l-(256*4));
    playerlist[tpl]->karma=t4->df[0]; txtright(t4,t4->l-4);
    f=playerlist[tpl]->karma; stealing_MESSAGE=FALSE;

    if (save_version==1){
      if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; playerlist[tpl]->mobj=myobj2;} txtright(t4,t4->l-8); //->mobj
    }

    myobj6=NULL; //set to current deed!
    static object *iskiff[256],*ideed[256];
    static short iskiffx,ideedx;
    iskiffx=-1; ideedx=-1;
    static object *npchorsepapers[8];
    ZeroMemory(&npchorsepapers,sizeof(npchorsepapers));


addnewpartymember:
    //KNOWN BUGS: multiple primary boats!, iskiff arrays(position), ->conversation is incorrect!


    //NPC
    //myobj,tnpc
    myobj->type=t4->ds[0]; txtright(t4,t4->l-2);

    x=t4->d2[0]; txtright(t4,t4->l-1); txtset(t,t4); txtleft(t,x); txtright(t4,t4->l-x); txtset(tnpc->name,t);
    if (playerlist[tpl]->party[1]==NULL){ //primary player
      txtset(tnpc->name,save_name[i]); //autoname edit
      txtset(tname,tnpc->name); //for ignore.txt???
    }

    //Kalour FIX
#ifdef CLIENT
    txtset(t,"Kalour");
    if (txtsame(t,tnpc->name)){
      OBJremove(playerlist[tpl]->party[0]); OBJadd(387,372,playerlist[tpl]->party[0]); //in swamp
    }
#endif
    tnpc->port=t4->ds[0]; txtright(t4,t4->l-2);
    if (playerlist[tpl]->party[1]){ //if there is second party member then this is not the primary player!
      tnpc->converse=201;
      tnpc->schedule=&schedule2[0][0];
      tnpc->schedule_i=-1; //unknown
      tnpc->order=1; //schedule
    }

    tnpc->hp=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->hp_max=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->mp=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->mp_max=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->s=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->d=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->i=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->exp=t4->dl[0]; txtright(t4,t4->l-4);
    tnpc->lev=t4->ds[0]; txtright(t4,t4->l-2);
    tnpc->flags=t4->d2[0]; txtright(t4,t4->l-1);


    tnpc->wt=0; tnpc->wt_max=tnpc->s*4*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;

    if (playerlist[tpl]->party[1]){//if set, not primary partymember
      tnpc->mp_max=get_mp_max(tnpc->i);
      //tnpc->wt_max/=2;//halve wt_max for non-primary partymember
      tnpc->wt_max=tnpc->s*3*100;
    }else{
      tnpc->mp_max=tnpc->i*2;
    }

    if (save_version==1) goto sv1_objects;

    //***HUGE VERSION DIFFERENCES AHEAD!***
    /*
    create an array to align contain object pointers
    extract objects from string (pointers will come first)

    */
    static object *sv2o[16384];
    static unsigned long sv2op[10];
    sv2op[0]=t4->dl[0];
    sv2op[1]=t4->dl[1];
    sv2op[2]=t4->dl[2];
    sv2op[3]=t4->dl[3];
    sv2op[4]=t4->dl[4];
    sv2op[5]=t4->dl[5];
    sv2op[6]=t4->dl[6];
    sv2op[7]=t4->dl[7];
    sv2op[8]=t4->dl[8];
    //txtnumint(t5,t4->dl[8]); txtadd(t5,":inv index"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
    sv2op[9]=t4->dl[9];
    //txtnumint(t5,t4->dl[9]); txtadd(t5,":mobj index"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

    txtright(t4,t4->l-10*4);


    /*-7168
    txtset(t2,"418.BIN");
    tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
    put(tfh,t4->d2,t4->l);
    close(tfh);
    exit2(4);
    */

housestoragerestore0:

    //create objects (being careful of object->next & object->prev)
    x=0; //first item in set
    x2=-1; //index
sv2nextitem:
    if (t4->dl[0]!=0){

      myobj2=OBJnew();
      myobj2->type=t4->ds[0];
      myobj2->info=t4->ds[1];

      x2++; sv2o[x2]=myobj2;



      if (housestoragerestore==0){
        if ((x2<(sv2op[9]-1))||(sv2op[9]==0)) stealing(playerlist[tpl],myobj2); //mobj won't be classed as being stolen
      }

      myobj2->more2=t4->dl[1];



      //if (housestoragerestore){
      //txtnumint(t5,myobj2->type); txtadd(t5,":type"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
      //txtnumint(t5,myobj2->more2); txtadd(t5,":more2"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
      //}


      //txtnumint(t5,myobj2->type&1023);
      //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

      //GPF
      if (x){ //link it to prev item
        myobj2->prev=sv2o[x2-1];
        sv2o[x2-1]->next=myobj2;
        //txtset(t5,"linked to prev item"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
      }


      myobj3=myobj2;


      if (myobj3->type==448){//horse papers (load)
        /*
        myobj2=(object*)OBJlist_list[y6]->more;
        if (myobj2->x){//horse is not being ridden
        t2->dl[0]=myobj2->x+myobj2->y*2048;//use as few bits as possible(21 bits total)
        }else{//horse is being ridden by a partymember
        t2->dl[0]=0;
        //scan to detect which partymember
        for(z3=0;z3<7;z3++){
        if (myobj3=playerlist[tpl]->party[z3]){
        tnpc3=(npc*)myobj3->more;
        if (tnpc3->horse==myobj2){
        t2->dl[0]=z3;
        }//tnpc3->horse==myobj2
        }//party[z3]
        }//z3
        }//myobj2->x
        */
        z2=myobj3->more2&2097151;

        //2097151=111111111111111111111b (21bits in binary)
        //static long hc=0;
        //hc++;
        //if (hc==4) exit(z2);
        //if (z2!=903451) if (z2!=0) exit(z2);

        //t->d2[t->l-1]&=225;//11100001b
        //t->d2[t->l-1]+=(z4<<1);//npc#
        //((myobj3->info>>9)&15)

        //oldsave patch
        if (z2<=7){
          npchorsepapers[z2]=myobj3;
          goto oldsave_horse;
        }

        if (((myobj3->info>>9)&15)==8){
          //create horse and set deed to point to horse
          movernew(430,z2&2047,z2>>11,MOVERNEW_FLAG_NORESPAWN);
          ((creature*)MOVERNEW_OBJECT->more)->flags|=128;
          ((creature*)MOVERNEW_OBJECT->more)->al=0;
          ((creature*)MOVERNEW_OBJECT->more)->respawn_y=tpl;
          ((creature*)MOVERNEW_OBJECT->more)->respawn_x=(myobj3->more2>>21)&255;

          OBJmove_allow=TRUE;
          if (((myobj3->more2>>29)&3)==1){
            OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x-1,MOVERNEW_OBJECT->y);
            OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x+1,MOVERNEW_OBJECT->y);
          }
          if (((myobj3->more2>>29)&3)==2){
            OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x,MOVERNEW_OBJECT->y-1);
            OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x,MOVERNEW_OBJECT->y+1);
          }
          if (((myobj3->more2>>29)&3)==3){
            OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x+1,MOVERNEW_OBJECT->y);
            OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x-1,MOVERNEW_OBJECT->y);
          }
          OBJmove_allow=FALSE;

          myobj3->more=MOVERNEW_OBJECT;

          //If horse is inside a house evict it
          x7=MOVERNEW_OBJECT->x; y7=MOVERNEW_OBJECT->y;
          x6=housecheck(x7,y7);
          if(x6) {
            if (houseowner(playerlist[tpl],x6)) goto houseboundrycheck_horse0;
            //move horse to entrance
            OBJmove_allow=TRUE;
            OBJmove2(MOVERNEW_OBJECT,houseentrancex[x6],houseentrancey[x6]);
            OBJmove_allow=FALSE;
            goto houseboundrycheck_horse0;
          }
houseboundrycheck_horse0:;




          //remove horse rear end!
          //OBJremove((object*)((creature*)MOVERNEW_OBJECT->more)->more);

        }else{
          //set horse as being ridden
          //t->d2[t->l-1]&=225;//11100001b
          //t->d2[t->l-1]+=(z4<<1);//npc#
          //((myobj3->info>>9)&15)
          if (npchorsepapers[((myobj3->info>>9)&15)]) exit (576);
          npchorsepapers[((myobj3->info>>9)&15)]=myobj3;
        }
      }//horse papers (load)
oldsave_horse:

      if ((myobj3->type&1023)==414){//skiff
        iskiffx++; iskiff[iskiffx]=myobj3;
      }

      if (myobj3->type==149){//deed

        //ok lets add this ship if it's not current ship
        if (myobj3->more2==NULL){ //inventory deed!
          ideedx++; ideed[ideedx]=myobj3;
          goto ideed_done;
        }

        if (myobj3->more2!=0xFFFFFFFF){
          //create ship/skiff!
          if (myobj3->info&(1<<9)){
            //skiff
            myobj5=OBJnew(); myobj5->type=414; OBJadd(myobj3->more2&65535,myobj3->more2>>16,myobj5);
            myobj3->more=(object*)myobj5;

          }else{
            //ship

            myobj5=OBJnew(); myobj5->type=412+9*1024; OBJadd(myobj3->more2&65535,myobj3->more2>>16,myobj5);
            static mlobj *mmyobj; //array size varies
            mmyobj=(mlobj*)malloc(sizeof(object*)*5);
            myobj5->more=mmyobj;
            myobj=OBJnew(); mmyobj->obj[0]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[1]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[2]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[3]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[4]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj3->more=myobj5; //deed->ship

            OBJmove_allow=TRUE;
            OBJmove2(myobj5,myobj5->x,myobj5->y+1);
            OBJmove2(myobj5,myobj5->x,myobj5->y-1);
            OBJmove_allow=FALSE;
            myobj3->more=(object*)myobj5;

          }
        }else{
          myobj6=myobj3;} //set as current deed
      }//149
ideed_done:


      x=1;

      txtright(t4,t4->l-8); //remove item
      goto sv2nextitem;
    }else{ //==0
      //txtset(t5,"break"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

      if (t4->dl[1]!=0) {x=0; txtright(t4,t4->l-4); goto sv2nextitem;} //remove 0
      txtright(t4,t4->l-4); //remove last 0
      txtright(t4,t4->l-4); //remove last 0

      //txtset(t5,"final break"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
    }
    //txtset(t5,"extraction finished"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


    //but what about container links?
    for (x=0;x<=x2;x++){
      if (obji[sprlnk[sv2o[x]->type&1023]].flags&1024){ //container
        if (sv2o[x]->type==57) playerlist[tpl]->GNPCflags[30]=1; //spellbook exists?
        if (sv2o[x]->more2){
          myobj2=sv2o[sv2o[x]->more2-1];
          sv2o[x]->more=myobj2;
          myobj2->prev=sv2o[x]; //not sure if this is required but hey!
        }//->more2
      }//multiple
    }//x

    if (housestoragerestore) goto housestoragerestore1;

    if (sv2op[0]) tnpc->helm=sv2o[sv2op[0]-1];
    if (sv2op[1]) tnpc->wep_right=sv2o[sv2op[1]-1];
    if (sv2op[2]) tnpc->wep_left=sv2o[sv2op[2]-1];
    if (sv2op[3]) tnpc->armour=sv2o[sv2op[3]-1];
    if (sv2op[4]) tnpc->boots=sv2o[sv2op[4]-1];
    if (sv2op[5]) tnpc->neck=sv2o[sv2op[5]-1];
    if (sv2op[6]) tnpc->ring_right=sv2o[sv2op[6]-1];
    if (sv2op[7]) tnpc->ring_left=sv2o[sv2op[7]-1];
    if (sv2op[8]){
      /* tnpc->items was already set before !! and the bag item was never removed so It caused mem leak */
      OBJremove(tnpc->items); OBJrelease(tnpc->items);
      tnpc->items=sv2o[sv2op[8]-1]; //includes the bag!
      tnpc->baseitem=tnpc->items;
    }
    if (playerlist[tpl]->party[1]==NULL){
      if (sv2op[9]) playerlist[tpl]->mobj=sv2o[sv2op[9]-1];
    }

    if (playerlist[tpl]->GNPCflags[252]){
      playerlist[tpl]->GNPCflags[252]--;
      //create a new NPC
      x8=1;
findemptyparty: if (playerlist[tpl]->party[x8]) {x8++; goto findemptyparty;}

      myobj=OBJnew();
      playerlist[tpl]->party[x8]=myobj; //player<-

      tnpc=(npc*)malloc(sizeof(npc));
      ZeroMemory(tnpc,sizeof(npc));
      myobj->more=tnpc; myobj->info|=2; //myobj<-
      //create an INVISIBLE container for player's items
      myobj2=OBJnew();
      myobj2->type=188; //bag (INVISIBLE)
      tnpc->items=myobj2;
      tnpc->baseitem=myobj2;
      tnpc->order=1;
      tnpc->follow=playerlist[tpl]->party[0];
      tnpc->name=txtnew();
      tnpc->player=playerlist[tpl];
      tnpc->upflags=1;


      OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj);

      //txtnumint(t5,0); txtadd(t5,":objadd"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


      goto addnewpartymember;
    }

    //txtnumint(t5,t4->ds[0]); txtadd(t5,":firstobj"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


    //28 house number (not valid if player has been evicted!)
    //29 number of house storage positions saved

	// s111 increase house storage max slots
//    static unsigned char housestoragerestorei,housechest;
    static unsigned char housechest;
	static unsigned int housestoragerestorei;
    housechest=0;
    if (playerlist[tpl]->GNPCflags[28]){

      //MessageBox(NULL,"HOUSESTORAGECHECK: gflags say still a house owner","Ultima 6 Online",MB_OK);


      if (houseowner(playerlist[tpl],playerlist[tpl]->GNPCflags[28])==0){
        //player no longer owns house registered in player structure
        housechest=1;
        playerlist[tpl]->GNPCflags[28]=0;
      }
    }

    for (housestoragerestorei=0;housestoragerestorei<playerlist[tpl]->GNPCflags[29];housestoragerestorei++){
      //housestoragerestorei=0;

      if (t4->dl[0]){
        housestoragerestore=1; goto housestoragerestore0;
housestoragerestore1: housestoragerestore=0;

        myobj2=sv2o[0];

        if (housechest==0){
housestoragerestore2:
          myobj3=(object*)myobj2->next;

          if (obji[sprlnk[myobj2->type&1023]].weight){//FIX for red gate/balloon and other issues
            OBJadd(housestoragex[playerlist[tpl]->GNPCflags[28]][housestoragerestorei],housestoragey[playerlist[tpl]->GNPCflags[28]][housestoragerestorei],myobj2);
          }


          if (myobj3) {myobj2=myobj3; goto housestoragerestore2;}
        }else{
          //put items in a chest and 
          myobj3=OBJnew(); myobj3->type=98+1024; myobj3->more=myobj2; myobj2->prev=myobj3;
          tnpc3=(npc*)playerlist[tpl]->party[0]->more;
          additemroot(tnpc3,myobj3);
        }//==0

      }else{
        txtright(t4,t4->l-4);
      }

    }

    //housestoragerestore1:

    //for (x2=0;x2<playerlist[tpl]->GNPCflags[29];x2++){
    //myobj=(object*)od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[playerlist[tpl]->GNPCflags[28]][x2]]->next;
    //x5=0;
    //housestorageadd=1; goto housestorageadd0;
    //housestorageadd1: housestorageadd=0;
    //}

    //fix inventory skiff deeds
    for (x=0;x<=iskiffx;x++){
      if (ideedx>=x){
        iskiff[x]->more=ideed[x];
        ideed[x]->more=iskiff[x];
      }else{
        iskiff[x]->info|=(1<<9); //free skiff!
        iskiff[x]->more2=NULL; //null deed
      }
    }

    for (x=0;x<=ideedx;x++){
      if (iskiffx<x){
        OBJremove(ideed[x]); //remove invalid deed!
      }
    }




    goto sv2_done;

sv1_objects:
    //equipment
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->helm=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->wep_right=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->wep_left=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->armour=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->boots=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->neck=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->ring_right=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);
    if (t4->ds[0]) {myobj2=OBJnew(); myobj2->type=t4->ds[0]; myobj2->info=t4->ds[1]; myobj2->more2=t4->dl[1]; tnpc->ring_left=myobj2; stealing(playerlist[tpl],myobj2);} txtright(t4,t4->l-8);



    x=t4->ds[0]; txtright(t4,t4->l-2); //item n
    for (x2=0;x2<x;x2++){
      myobj3=OBJnew(); myobj3->type=t4->ds[0]; myobj3->info=t4->ds[1]; myobj3->more2=t4->dl[1];

      //PATCH! 1.0
      if (myobj3->type==64){ //key A
        if (myobj3->more2&32768) myobj3->more2-=32768;
      }


      stealing(playerlist[tpl],myobj3);
      additem(tnpc,myobj3);


      if (myobj3->type==149){//deed
        //ok lets add this ship if it's not current ship
        if (myobj3->more2!=0xFFFFFFFF){
          //create ship/skiff!
          if (myobj3->info&(1<<9)){
            //skiff
            myobj5=OBJnew(); myobj5->type=414; OBJadd(myobj3->more2&65535,myobj3->more2>>16,myobj5);
            myobj3->more=(object*)myobj5;

          }else{
            //ship

            myobj5=OBJnew(); myobj5->type=412+9*1024; OBJadd(myobj3->more2&65535,myobj3->more2>>16,myobj5);
            static mlobj *mmyobj; //array size varies
            mmyobj=(mlobj*)malloc(sizeof(object*)*5);
            myobj5->more=mmyobj;
            myobj=OBJnew(); mmyobj->obj[0]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[1]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[2]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[3]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj=OBJnew(); mmyobj->obj[4]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
            myobj3->more=myobj5; //deed->ship

            OBJmove_allow=TRUE;
            OBJmove2(myobj5,myobj5->x,myobj5->y+1);
            OBJmove2(myobj5,myobj5->x,myobj5->y-1);
            OBJmove_allow=FALSE;
            myobj3->more=(object*)myobj5;

          }
        }else{
          myobj6=myobj3;} //set as current deed
      }//149




      txtright(t4,t4->l-8);
    }



sv2_done:
    playerlist[tpl]->karma=f; stealing_MESSAGE=TRUE;

    //move player to front door of house if not the owner
    x2=playerlist[tpl]->party[0]->x; y2=playerlist[tpl]->party[0]->y;
    x=housecheck(x2,y2);
    if(x) {
      if (houseowner(playerlist[tpl],x)) goto houseboundrycheck0;
      //trespassing! move player to entrance
      for (x3=7;x3>=0;x3--){ if (playerlist[tpl]->party[x3]){
        OBJremove(playerlist[tpl]->party[x3]);
        OBJadd(houseentrancex[x],houseentrancey[x],playerlist[tpl]->party[x3]);
      }}//x3,->party[x3]
      goto houseboundrycheck0;
    }//x
houseboundrycheck0:

    //place selected partymembers on horses
    for (x=0;x<=7;x++){
      if (npchorsepapers[x]){
        myobj2=playerlist[tpl]->party[x]; tnpc2=(npc*)myobj2->more;

        z2=npchorsepapers[x]->more2&2097151;
        if ((z2<=7)||(z2==2097151)){
          movernew(430,myobj2->x,myobj2->y,MOVERNEW_FLAG_NORESPAWN); myobj=MOVERNEW_OBJECT;
        }else{
          movernew(430,z2&2047,z2>>11,MOVERNEW_FLAG_NORESPAWN); myobj=MOVERNEW_OBJECT;
        }

        ((creature*)MOVERNEW_OBJECT->more)->flags|=128;
        ((creature*)MOVERNEW_OBJECT->more)->al=0;
        ((creature*)MOVERNEW_OBJECT->more)->respawn_y=tpl;
        ((creature*)MOVERNEW_OBJECT->more)->respawn_x=(npchorsepapers[x]->more2>>21)&255;
        //remove horse rear end!
        //OBJremove((object*)((creature*)MOVERNEW_OBJECT->more)->more);

        OBJmove_allow=TRUE;
        if (((npchorsepapers[x]->more2>>29)&3)==1){
          OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x-1,MOVERNEW_OBJECT->y);
          OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x+1,MOVERNEW_OBJECT->y);
        }
        if (((npchorsepapers[x]->more2>>29)&3)==2){
          OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x,MOVERNEW_OBJECT->y-1);
          OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x,MOVERNEW_OBJECT->y+1);
        }
        if (((npchorsepapers[x]->more2>>29)&3)==3){
          OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x+1,MOVERNEW_OBJECT->y);
          OBJmove2(MOVERNEW_OBJECT,MOVERNEW_OBJECT->x-1,MOVERNEW_OBJECT->y);
        }
        OBJmove_allow=FALSE;

        x7=MOVERNEW_OBJECT->x; y7=MOVERNEW_OBJECT->y; //check if the horse is inside a house
        x6=housecheck(x7,y7);
        if(x6) {
          if (houseowner(playerlist[tpl],x6)) goto houseboundrycheck_horse1;
          //move horse to entrance
          OBJmove_allow=TRUE;
          OBJmove2(MOVERNEW_OBJECT,houseentrancex[x6],houseentrancey[x6]);
          OBJmove_allow=FALSE;
          goto houseboundrycheck_horse1;
        }//x
houseboundrycheck_horse1:;



        npchorsepapers[x]->more=myobj;
        tnpc2->originalobjtype=myobj2->type;
        OBJremove(myobj2);
        OBJadd(myobj->x,myobj->y,myobj2);
        myobj2->type=myobj->type-430+431;
        crt2=(creature*)myobj->more;
        tnpc2->more=crt2->more; myobj3=(object*)crt2->more; myobj3->more=myobj2;//make rear end of horse point to partymember(myobj2)
        myobj3->type=myobj3->type-430+431;
        OBJremove(myobj);//remove front end of horse
        tnpc2->horse=myobj;//backup pointer to front half of horse for later

      }
    }

    //version 1+2, myobj6=pointer to deed of current ship
    x=playerlist[tpl]->GNPCflags[255];
    if (x){ //put player into craft (this is the really unfun bit!)

      if (x==1){ //ship
        //build and place skiff facing new direction

        myobj3=OBJnew(); myobj3->type=412+9*1024; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj3);

        static mlobj *mmyobj; //array size varies
        mmyobj=(mlobj*)malloc(sizeof(object*)*5);
        myobj3->more=mmyobj;

        myobj=OBJnew(); mmyobj->obj[0]=myobj; myobj->info|=8; myobj->type=myobj3->type; myobj->more=(object*)myobj3; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj);
        myobj=OBJnew(); mmyobj->obj[1]=myobj; myobj->info|=8; myobj->type=myobj3->type; myobj->more=(object*)myobj3; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj);
        myobj=OBJnew(); mmyobj->obj[2]=myobj; myobj->info|=8; myobj->type=myobj3->type; myobj->more=(object*)myobj3; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj);
        myobj=OBJnew(); mmyobj->obj[3]=myobj; myobj->info|=8; myobj->type=myobj3->type; myobj->more=(object*)myobj3; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj);
        myobj=OBJnew(); mmyobj->obj[4]=myobj; myobj->info|=8; myobj->type=myobj3->type; myobj->more=(object*)myobj3; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj);

        playerlist[tpl]->craft_con=(object*)myobj3->more;
        myobj3->more=playerlist[tpl]->party[0]->more;
        myobj3->info=playerlist[tpl]->party[0]->info;
        playerlist[tpl]->craft=playerlist[tpl]->party[0];
        for (x2=0;x2<=7;x2++){
          if (playerlist[tpl]->party[x2]){
            OBJremove(playerlist[tpl]->party[x2]);
            tnpc3=(npc*)playerlist[tpl]->party[x2]->more;
            if (tnpc3->horse) OBJremove((object*)tnpc3->more);
          }
        }

        playerlist[tpl]->party[0]=myobj3;
        if (myobj3->info&32768) myobj3->info-=32768; //?

        OBJmove_allow=TRUE;
        OBJmove2(myobj3,playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y+1);
        OBJmove2(myobj3,playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y-1);
        OBJmove_allow=FALSE;

        if (myobj6) myobj6->more=(object*)myobj3; //allocate current deed
      }//ship

      if (x==2){ //skiff
        //build and place skiff facing new direction
        myobj3=OBJnew(); myobj3->type=414; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj3);
        myobj3->more=playerlist[tpl]->party[0]->more;
        myobj3->info=playerlist[tpl]->party[0]->info;
        playerlist[tpl]->craft=playerlist[tpl]->party[0];
        for (x2=0;x2<=7;x2++){if(playerlist[tpl]->party[x2])OBJremove(playerlist[tpl]->party[x2]);}
        playerlist[tpl]->party[0]=myobj3;
        if (myobj3->info&32768) myobj3->info-=32768;

        if (myobj6){
          myobj6->more=(object*)myobj3; //allocate current deed
        }else{
          myobj3->info|=(1<<9);
        }


      }//skiff

      if (x==3){ //raft
        //build and place skiff facing new direction
        myobj3=OBJnew(); myobj3->type=415; OBJadd(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj3);
        myobj3->more=playerlist[tpl]->party[0]->more;
        myobj3->info=playerlist[tpl]->party[0]->info;
        playerlist[tpl]->craft=playerlist[tpl]->party[0];
        for (x2=0;x2<=7;x2++){if(playerlist[tpl]->party[x2])OBJremove(playerlist[tpl]->party[x2]);}
        playerlist[tpl]->party[0]=myobj3;
        if (myobj3->info&32768) myobj3->info-=32768; //?
      }//raft

      if (x==4){ //balloon //build balloon
        myobj2=OBJnew();
        //create a balloon where the avatar is standing
        x=playerlist[tpl]->party[0]->x; y=playerlist[tpl]->party[0]->y;
        myobj2->type=423+1024*3; OBJadd(x,y,myobj2);
        static mlobj *mmyobj; //array size varies
        mmyobj=(mlobj*)malloc(sizeof(object*)*3);
        myobj2->more=mmyobj;
        myobj=OBJnew(); mmyobj->obj[0]=myobj; //middle
        myobj->type=423+0*1024; myobj->info|=8; //<-base object
        OBJadd(x-1,y-1,myobj); myobj->more=myobj2;
        myobj=OBJnew(); mmyobj->obj[1]=myobj; //middle
        myobj->type=423+1*1024; myobj->info|=8; //<-base object
        OBJadd(x,y-1,myobj); myobj->more=myobj2;
        myobj=OBJnew(); mmyobj->obj[2]=myobj; //middle
        myobj->type=423+2*1024; myobj->info|=8; //<-base object
        OBJadd(x-1,y,myobj); myobj->more=myobj2;
        //use balloon (the one we just created :)
        playerlist[tpl]->craft_con=(object*)myobj2->more;
        myobj2->more=playerlist[tpl]->party[0]->more;
        myobj2->info=playerlist[tpl]->party[0]->info;
        playerlist[tpl]->craft=playerlist[tpl]->party[0];
        for (x2=0;x2<=7;x2++){if(playerlist[tpl]->party[x2])OBJremove(playerlist[tpl]->party[x2]);}
        playerlist[tpl]->party[0]=myobj2;
      }//x==4

    }//x


    tnpc=(npc*)playerlist[tpl]->party[0]->more;

    if (playerlist[tpl]->GNPCflags[253]&2){//rest
      playerlist[tpl]->GNPCflags[253]-=2;
      x=(unsigned long)ett-playerlist[tpl]->GNPCflags[254];
      if ((x>=1536)||(x<0)){ //~24min offline
        for (x=0;x<=7;x++){ if (playerlist[tpl]->party[x]){
          tnpc2=(npc*)playerlist[tpl]->party[x]->more;
          tnpc2->hp=tnpc2->hp_max; //heal
          tnpc2->mp=tnpc2->mp_max; //full mp
        }}
      }
    }

    playerlist[tpl]->GNPCflags[251]++; //251 number of times entered britannia (as at last save)

    if (playerlist[tpl]->GNPCflags[19]){ //disable showing of lenses
      playerlist[tpl]->GNPCflags[22]|=0xFFFFFFFF; //make all destinations available
      if (playerlist[tpl]->GNPCflags[18]&2) playerlist[tpl]->GNPCflags[18]-=2;
      if (playerlist[tpl]->GNPCflags[18]&4) playerlist[tpl]->GNPCflags[18]-=4;
      if (playerlist[tpl]->GNPCflags[18]&16) playerlist[tpl]->GNPCflags[18]-=16; //allow engame sequence to be replayed!
      //remove amulet of submission FIX
      tnpc=(npc*)playerlist[tpl]->party[0]->more;
      if (tnpc->neck){
        if (tnpc->neck->type==76) tnpc->neck=NULL;
      }
    }else{
      //ORBFIX: if a player was given all gate destinations incorrectly reset to 0
      if (playerlist[tpl]->GNPCflags[22]==0xFFFFFFFF) playerlist[tpl]->GNPCflags[22]=0;
    }//19

    //duplicate inparty name fix
    for (x=0;x<=6;x++){
      if (playerlist[tpl]->party[x]){
        tnpc2=(npc*)playerlist[tpl]->party[x]->more; txtset(t,tnpc2->name); txtucase(t); txtset(t4,tnpc2->name);
        for (x2=x+1;x2<=7;x2++){
          if (playerlist[tpl]->party[x2]){
            tnpc2=(npc*)playerlist[tpl]->party[x2]->more; txtset(t2,tnpc2->name); txtucase(t2);
            if (txtsame(t,t2)){
              //we can assume that the name is not already indexed
              y=2;
inpartynamefix0:
              txtset(t5,t);
              if (y==2) txtset(t3,"II");
              if (y==3) txtset(t3,"III");
              if (y==4) txtset(t3,"IV");
              if (y==5) txtset(t3,"V");
              if (y==6) txtset(t3,"VI");
              if (y==7) txtset(t3,"VII");
              if (y==8) txtset(t3,"VIII");
              txtadd(t5,t3);
              for (x3=0;x3<=7;x3++){
                if (playerlist[tpl]->party[x3]){
                  tnpc3=(npc*)playerlist[tpl]->party[x3]->more; txtset(t6,tnpc3->name); txtucase(t6);
                  if (txtsame(t5,t6)){ //no is unavailable
                    y++; goto inpartynamefix0;
                  }//txtsame
                }//party[x3]
              }//x3
              if (y==2) txtadd(tnpc2->name,"II");
              if (y==3) txtadd(tnpc2->name,"III");
              if (y==4) txtadd(tnpc2->name,"IV");
              if (y==5) txtadd(tnpc2->name,"V");
              if (y==6) txtadd(tnpc2->name,"VI");
              if (y==7) txtadd(tnpc2->name,"VII");
              if (y==8) txtadd(tnpc2->name,"VIII");
            }//txtsame
          }}}}

    //247&248 TEMP food values for npc 0-7, 1 byte each (only used when saving/loading)
    for (x2=0;x2<=7;x2++){ if (playerlist[tpl]->party[x2]){
      tnpc3=(npc*)playerlist[tpl]->party[x2]->more;
      if (x2==0) x3=(playerlist[tpl]->GNPCflags[247])&255;
      if (x2==1) x3=(playerlist[tpl]->GNPCflags[247]>>8)&255;
      if (x2==2) x3=(playerlist[tpl]->GNPCflags[247]>>16)&255;
      if (x2==3) x3=(playerlist[tpl]->GNPCflags[247]>>24)&255;
      if (x2==4) x3=(playerlist[tpl]->GNPCflags[248])&255;
      if (x2==5) x3=(playerlist[tpl]->GNPCflags[248]>>8)&255;
      if (x2==6) x3=(playerlist[tpl]->GNPCflags[248]>>16)&255;
      if (x2==7) x3=(playerlist[tpl]->GNPCflags[248]>>24)&255;
      tnpc3->food=x3;
    }}

    //remove house key(s),...
    //party inventory list(s) & m(ouse)obj(ect) (does not include equipped items!)
    for (x6=0;x6<=7;x6++){ if (playerlist[tpl]->party[x6]){
      tnpc3=(npc*)playerlist[tpl]->party[x6]->more;
      x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
    }} //!=NULL,x6
    x5=OBJlist(playerlist[tpl]->mobj); //include mobj
    OBJlist_last=NULL;
    for(y6=0;y6<x5;y6++){
      if (OBJlist_list[y6]->type==64){ //key
        if ((OBJlist_list[y6]->more2&0xFF00)==256){ //house key
          OBJremove(OBJlist_list[y6]);
        }
      }
    }//y6
    //mouse object
    if (playerlist[tpl]->mobj){
      if (playerlist[tpl]->mobj->type==64){//key
        if ((playerlist[tpl]->mobj->more2&0xFF00)==256){//house key
          playerlist[tpl]->mobj=NULL;
        }
      }
    }


    //add house key
    if (houseowner(playerlist[tpl],playerlist[tpl]->GNPCflags[28])){
      tnpc3=(npc*)playerlist[tpl]->party[0]->more;
      myobj3=OBJnew(); myobj3->type=64; myobj3->info=256; myobj3->more2=256+playerlist[tpl]->GNPCflags[28]; //house key
      additemroot(tnpc3,myobj3);
    }//add house key

	// r555 debug: give house key
	/*
	if (TRUE) {
		//give key (256+housenumber)
		static object *myobj3 = OBJnew(); myobj3->type = 64; myobj3->info = 256; myobj3->more2 = 256 + 47;
		static object *myobj4 = tnpc3->items; static object *myobj5 = (object*)myobj4->more;
		if (myobj5) {
			myobj3->next = myobj5;
			myobj5->prev = myobj3;
			myobj3->prev = myobj4;
			myobj4->more = myobj3;
		}
		else {
			myobj3->next = NULL;
			myobj3->prev = myobj4;
			myobj4->more = myobj3;
		}
		tnpc3->upflags |= 32; //inv
		//tplayer->GNPCflags[28]=x2; //set housenumber
	}
	*/


    /*
    //equipped items
    txtset(t,"????????");
    myobj=tnpc->helm; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->wep_right; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->wep_left; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->armour; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->boots; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->neck; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->ring_right; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->ring_left; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    //items?
    //light?
    */
    txtset(t,"Player_Entered:"); txtadd(t,tnpc->name); LOGadd(t);

    //ban player
    tfh=open2("ban.txt",OF_READWRITE|OF_SHARE_COMPAT); //log
ban_nextplayer:
    txtfilein(t6,tfh);
    if (txtsamewithoutcase(t6,tname)){
      playerlist[tpl]->GNPCflags[253]|=1;
      txtset(t7,"??"); t7->d2[0]=250; t7->d2[1]=8; NET_send(NETplayer,playerlist[tpl]->net,t7);
    }
    if (t6->l) goto ban_nextplayer;
    close(tfh);


    //create custom character portrait (if required)
    /*
    if (t5->d2[0]==1){//upload character portrait
    tcustomportrait_upload=1; txtright(t5,t5->l-1);
    memcpy(&tcustomportrait,t5->d,7168); txtright(t5,t5->l-7168);
    goto getsetupmessageinfo_more;
    }
    */



    tnpc3=(npc*)playerlist[tpl]->party[0]->more;

    //grant full privileges to admin players
    x=0;
    while (admins[x]) {
      if (txtsame(tnpc3->name,admins[x])){
        for (i=40;i<=47;i++) playerlist[tpl]->GNPCflags[i]=0xFFFFFFFF;
        break;
      }
      x++;
      if (x==ADMINSMAX) {break;}   
    }

    //portraits 2.0 backup original portrait so it can be restored
    if (playerlist[tpl]->GNPCflags[34]==NULL) playerlist[tpl]->GNPCflags[34]=tnpc3->port;
    tnpc3->port=playerlist[tpl]->GNPCflags[34];

    if (tcustomportrait_upload){
      if ((playerlist[tpl]->GNPCflags[33]&2)==0){//not blocked from uploading custom portraits
        if (tnpc3->lev>=3){
          if (tnpc3->exp>=1000){ tnpc3->exp-=1000;

          HOST_portrait_data[HOST_portrait_next]=(unsigned short*)malloc(7168);
          memcpy(HOST_portrait_data[HOST_portrait_next],&tcustomportrait,7168);
          HOST_portrait_loaded[HOST_portrait_next]=1;
          tnpc3->port=HOST_portrait_next;
          HOST_portrait_next++;

          //add to database!
          //get/add index
          z=0;
          tfh=open(".\\save\\port\\index.txt");
          txtset(t2,"=");
          txtset(t4,tnpc3->name); txtucase(t4);
saveportindex_loop:
          txtfilein(t,tfh);
          if (x=txtsearch(t,t2)){
            txtset(t3,t); txtleft(t3,x-1); z=txtnum(t3);
            txtright(t,t->l-x);//name
            if (txtsame(t,t4)){
              goto saveportindex_found;
            }//txtsame(t3,t4)
            goto saveportindex_loop;
          }
          seek(tfh,lof(tfh));
          z++; txtnumint(t,z); txtadd(t,t2); txtadd(t,t4);
          txtfileout(t,tfh);
saveportindex_found:
          close(tfh);
          //create file
          static unsigned char bmpdata[54];
          txtset(t,".\\save\\port\\"); txtnumint(t2,z); txtadd(t,t2); txtadd(t,".BMP");
          tfh=open2(t->d,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
          tfh2=open(".\\host\\bmpdata.bmp");
          get(tfh2,&bmpdata,54);
          close(tfh2);
          put(tfh,&bmpdata,54);
          for (x2=63;x2>=0;x2--){ for (x=0;x<56;x++){
            y=tcustomportrait[x2*56+x]&31; y*=8;
            put(tfh,&y,1);
            y2=tcustomportrait[x2*56+x]&2016; y2>>=5; y2*=4;
            put(tfh,&y2,1);
            y3=tcustomportrait[x2*56+x]&63488; y3>>=11; y3*=8;
            put(tfh,&y3,1);
          }}
          close(tfh); 
          playerlist[tpl]->GNPCflags[33]|=1;//check for existing custom portrait on entry
          goto custromportraitload_skip;
          }//tnpc3->exp>=1000
        }//tnpc3->lev>=3
      }//not blocked from uploading custom portraits
    }//tcustomportrait_upload

    //customportraitload
    if (playerlist[tpl]->GNPCflags[33]&1){
      tfh=open(".\\save\\port\\index.txt");
      txtset(t2,"=");
      txtset(t4,tnpc3->name); txtucase(t4);
custromportraitload_loop:
      txtfilein(t,tfh);
      if (x=txtsearch(t,t2)){
        txtset(t3,t); txtleft(t3,x-1); z=txtnum(t3);
        txtright(t,t->l-x);//name
        if (txtsame(t,t4)){
          txtset(t,".\\save\\port\\"); txtadd(t,t3); txtadd(t,".BMP");
          //ADD SECURITY CHECKS HERE
          static surf *custromportraitload_surf;
          custromportraitload_surf=NULL;
          custromportraitload_surf=loadimage(t->d,SURF_SYSMEM16);
          if (custromportraitload_surf){
            HOST_portrait_data[HOST_portrait_next]=(unsigned short*)malloc(7168);
            memcpy(HOST_portrait_data[HOST_portrait_next],custromportraitload_surf->o,7168);
            HOST_portrait_loaded[HOST_portrait_next]=1;
            tnpc3->port=HOST_portrait_next;
            HOST_portrait_next++;
          }
          goto custromportraitload_found;
        }//txtsame(t3,t4)
        goto custromportraitload_loop;
      }
    }//playerlist[tpl]->GNPCflags[33]&1
custromportraitload_found:
custromportraitload_skip:



    goto save_restored;
save_createcharacter: //new player
    txtset(tnpc->name,tname);
    tnpc->lev=1; tnpc->exp=370; //same as in original, incentive not to die!
    tnpc->hp=30; tnpc->hp_max=30;
    tnpc->s=0; tnpc->d=0; tnpc->i=0;
    x=-1;
    x++; if (t6->d[x]==0) {U6O_HONESTY} else {U6O_COMPASSION} //i3,d3
    x++; if (t6->d[x]==0) {U6O_HONESTY} else {U6O_VALOR} //i3,s3
    x++; if (t6->d[x]==0) {U6O_HONESTY} else {U6O_JUSTICE} //i3,X
    x++; if (t6->d[x]==0) {U6O_HONESTY} else {U6O_SACRIFICE} //i3,X
    x++; if (t6->d[x]==0) {U6O_HONESTY} else {U6O_HONOR} //i3,X
    x++; if (t6->d[x]==0) {U6O_HONESTY} else {U6O_SPIRITUALITY} //i3,a1
    x++; if (t6->d[x]==0) {U6O_HONESTY} else {U6O_HUMILITY} //i3,X
    x++; if (t6->d[x]==0) {U6O_COMPASSION} else {U6O_VALOR} //d3,s3
    x++; if (t6->d[x]==0) {U6O_COMPASSION} else {U6O_JUSTICE} //d3,X
    x++; if (t6->d[x]==0) {U6O_COMPASSION} else {U6O_SACRIFICE} //d3,X
    x++; if (t6->d[x]==0) {U6O_COMPASSION} else {U6O_HONOR} //d3,X
    x++; if (t6->d[x]==0) {U6O_COMPASSION} else {U6O_SPIRITUALITY} //d3,a1
    x++; if (t6->d[x]==0) {U6O_COMPASSION} else {U6O_HUMILITY} //d3,X
    x++; if (t6->d[x]==0) {U6O_VALOR} else {U6O_JUSTICE} //s3,X
    x++; if (t6->d[x]==0) {U6O_VALOR} else {U6O_SACRIFICE} //s3,X
    x++; if (t6->d[x]==0) {U6O_VALOR} else {U6O_HONOR} //s3,X
    x++; if (t6->d[x]==0) {U6O_VALOR} else {U6O_SPIRITUALITY} //s3,a1
    x++; if (t6->d[x]==0) {U6O_VALOR} else {U6O_HUMILITY} //s3,X
    x++; if (t6->d[x]==0) {U6O_JUSTICE} else {U6O_SACRIFICE} //i,d
    x++; if (t6->d[x]==0) {U6O_JUSTICE} else {U6O_HONOR} //s,d
    x++; if (t6->d[x]==0) {U6O_JUSTICE} else {U6O_SPIRITUALITY} //X,a1
    x++; if (t6->d[x]==0) {U6O_JUSTICE} else {U6O_HUMILITY} //OK,X
    x++; if (t6->d[x]==0) {U6O_SACRIFICE} else {U6O_HONOR} //s,i
    x++; if (t6->d[x]==0) {U6O_SACRIFICE} else {U6O_SPIRITUALITY} //X,OK
    x++; if (t6->d[x]==0) {U6O_SACRIFICE} else {U6O_HUMILITY} //a1,X
    x++; if (t6->d[x]==0) {U6O_HONOR} else {U6O_SPIRITUALITY} //X,a1
    x++; if (t6->d[x]==0) {U6O_HONOR} else {U6O_HUMILITY} //OK,X
    x++; if (t6->d[x]==0) {U6O_SPIRITUALITY} else {U6O_HUMILITY} //a1,X
    x=tnpc->s-8; if (x<1) x=1; tnpc->s=x;
    x=tnpc->d-8; if (x<1) x=1; tnpc->d=x;
    x=tnpc->i-8; if (x<1) x=1; tnpc->i=x;
    tnpc->mp=tnpc->i*2; tnpc->mp_max=tnpc->i*2;
    tnpc->wt=0; tnpc->wt_max=tnpc->s*4*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;

    //Debug new players get high stats, gold, exp, all spells, 
    if (U6O_DEBUG){
      tnpc->lev=6;
      //tnpc->lev=20;
      tnpc->exp=410000;
      tnpc->s=tnpc->s+3; tnpc->d=tnpc->d+3; tnpc->i=tnpc->i+18;
      //tnpc->s=888; //tnpc->d=888; tnpc->i=888;
      tnpc->wt=0; tnpc->wt_max=tnpc->s*4*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
      //tnpc->mp_max=640; tnpc->mp=640;
      tnpc->hp_max=180; tnpc->hp=180;
      //tnpc->hp_max=8800; tnpc->hp=8800;
      for (x=0;x<=9;x++){
        x2=88; x3=0; x4=0;
        if (x==8) {x2=57;}//spell
        if (x<=7) {
          x2=58;
          for (x4=0;x4<=8;x4++){
            x3=(x<<4)+x4;
            myobj3=OBJnew(); myobj3->type=x2; myobj3->more2=x3; myobj3->info=0;//add item(s)
            additem(tnpc,myobj3);
          }
          x3=(x<<4)+9;
          x4=0;
        }//spell
        if (x2==88) x3=30000;//<-1000 gold
        myobj3=OBJnew(); myobj3->type=x2; myobj3->more2=x3; myobj3->info=x4;//add item(s)
        additem(tnpc,myobj3);
      }//x

      myobj3=OBJnew(); myobj3->type=65; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);
      myobj3=OBJnew(); myobj3->type=66; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);
      myobj3=OBJnew(); myobj3->type=67; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);
      myobj3=OBJnew(); myobj3->type=68; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);
      myobj3=OBJnew(); myobj3->type=69; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);
      myobj3=OBJnew(); myobj3->type=70; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);
      myobj3=OBJnew(); myobj3->type=71; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);
      myobj3=OBJnew(); myobj3->type=72; myobj3->more2=1000; //<-reagents
      additem(tnpc,myobj3);


      myobj3=OBJnew(); myobj3->type=87; myobj3->more2=0; myobj3->info=0;//add item: orb of the moons
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=242; myobj3->more2=0; myobj3->info=0;//rune of honesty
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=243; myobj3->more2=0; myobj3->info=0;//rune of compassion
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=244; myobj3->more2=0; myobj3->info=0;//rune of valor
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=81; myobj3->more2=0; myobj3->info=0;//storm cloack
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=258; myobj3->more2=0; myobj3->info=0;//invis ring
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=420; myobj3->more2=0; myobj3->info=0;//balloon
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=212; myobj3->more2=0; myobj3->info=0;//fan
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=50; myobj3->more2=15; //<-tcrossbow
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=23; myobj3->more2=255; //<-marmour
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=4; myobj3->more2=255; //<-spiked helm
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=35; myobj3->more2=255; //<-main gauche
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=35; myobj3->more2=255; //<-main gauche
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=13; myobj3->more2=255; //<-spiked shield
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=13; myobj3->more2=255; //<-spiked shield
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=24; myobj3->more2=255; //<-spiked collar
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=109; myobj3->more2=15; //<-rolling pin
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=109; myobj3->more2=15; //<-rolling pin
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=8; myobj3->more2=255; //<-magic helm
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=16; myobj3->more2=255; //<-magic shield
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=27; myobj3->more2=255; //<-leather boots
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=54; myobj3->more2=15; //<-magic bow
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=43; myobj3->more2=15; //<-sword
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=43; myobj3->more2=15; //<-sword
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=39; myobj3->more2=15; //<-mace
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=39; myobj3->more2=15; //<-mace
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=40; myobj3->more2=15; //<-morning star
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=40; myobj3->more2=15; //<-morning star
      additem(tnpc,myobj3);

      myobj3=OBJnew(); myobj3->type=47; myobj3->more2=15; //<-halberd
      additem(tnpc,myobj3);


    }//U6O_DEBUG


    //add items
    myobj3=OBJnew(); myobj3->type=90; myobj3->more2=3; //<-torch
    additem(tnpc,myobj3);

    myobj3=OBJnew(); myobj3->type=128; myobj3->more2=2; //<-bread
    additem(tnpc,myobj3);

    myobj3=OBJnew(); myobj3->type=129; myobj3->more2=1; //<-meat
    additem(tnpc,myobj3);

    myobj3=OBJnew(); myobj3->type=210; //<-ribs/big meat
    additem(tnpc,myobj3);

    myobj3=OBJnew(); myobj3->type=74; myobj3->more2=0; //<-ankh
    myobj3->info|=256; //quest item
    additem(tnpc,myobj3);

    myobj3=OBJnew(); myobj3->type=34; myobj3->more2=0; //<-club
    additem(tnpc,myobj3);

    if ((btime>=16)||(btime<=5)){ //only give players who enter at night
      myobj3=OBJnew(); myobj3->type=90+1*1024; myobj3->more2=1; //<-torch in hand
      tnpc->wep_left=myobj3;
    }

    static SYSTEMTIME systime;
    GetLocalTime(&systime);
    x3=(systime.wDay-1)+(systime.wMonth-1)*32+systime.wYear*(32*12);
    playerlist[tpl]->GNPCflags[249]=x3; //249 YMD of character creation

    txtset(t,"New_Player_Entered:"); txtadd(t,tnpc->name); LOGadd(t);
save_restored:

    playerlist[tpl]->port=((npc*)playerlist[tpl]->party[0]->more)->port;
    playerlist[tpl]->npcname=txtnew();
    txtset(playerlist[tpl]->npcname,((npc*)playerlist[tpl]->party[0]->more)->name);



    //if (tpl==0){
    //if (U6O_DEBUG){
    //tnpc->lev=8;
    //tnpc->s=64; tnpc->d=64; tnpc->i=64;
    //tnpc->mp_max=255; tnpc->mp=255;
    //tnpc->hp_max=255; tnpc->hp=255;
    //playerlist[tpl]->GNPCflags[6]=0;

    //playerlist[tpl]->wizardeyetimeleft=0.0f;
    //playerlist[tpl]->wizardeyex=0;
    //playerlist[tpl]->wizardeyey=0;

    //for (x=0;x<=2;x++){
    //x2=88; x3=0; x4=0;
    //if (x==0) {x2=58; x3=(6<<4)+9;}//spell
    //if (x2==88) x3=20000;//<-1000 gold
    //myobj3=OBJnew(); myobj3->type=x2; myobj3->more2=x3; myobj3->info=x4;//add item(s)
    //additem(tnpc,myobj3);
    //}//x

    //}//U6O_DEBUG
    //}//tpl==0



    playerlist[tpl]->net=tnet;
    playerlist[tpl]->id=U6ONEWID; U6ONEWID++;
    playerlist[tpl]->GNPCflags[242]=socketclient_ip[tnet];//update IP

    //log IP
    txtset(t,"Player_IP:");
    txtnumint(t2,playerlist[tpl]->GNPCflags[242]&255); txtadd(t,t2);
    txtadd(t,".");
    txtnumint(t2,(playerlist[tpl]->GNPCflags[242]>>8)&255); txtadd(t,t2);
    txtadd(t,".");
    txtnumint(t2,(playerlist[tpl]->GNPCflags[242]>>16)&255); txtadd(t,t2);
    txtadd(t,".");
    txtnumint(t2,(playerlist[tpl]->GNPCflags[242]>>24)&255); txtadd(t,t2);
    LOGadd(t);

    tnpc=(npc*)playerlist[tpl]->party[0]->more;
    txtset(t,"?"); t->d2[0]=8; txtadd(t,"Entered Britannia: "); txtadd(t,tnpc->name);
    for (x=0;x<=playerlist_last;x++){
      if (playerlist[x]){
        if (playerlist[x]->net!=INVALID_NET){
          if (x!=tpl){
            NET_send(NETplayer,playerlist[x]->net,t);
          }}}}
    inbritanniacheck(); txtset(t,"?"); t->d2[0]=8; txtadd(t,inbritannia); NET_send(NETplayer,playerlist[tpl]->net,t);


    txtset(t3,motd);
    txtset(t,"?"); t->d2[0]=2;
    txtset(t2,"??"); t2->ds[0]=1; txtadd(t,t2);
    t2->ds[0]=3; txtadd(t,t2);
    t2->ds[0]=2047; txtadd(t,t2);
    t2->ds[0]=1023; txtadd(t,t2);
    t2->ds[0]=t3->l; txtadd(t,t2);
    t2->ds[0]=0; txtadd(t,t2); //unused
    t2->ds[0]=187; txtadd(t,t2); //more=port
    txtset(t2,"????"); t2->df[0]=2; txtadd(t,t2); //wait
    txtadd(t,t3);
    NET_send(NETplayer,playerlist[tpl]->net,t);

    txtset(t3,"??"); t3->d2[0]=14; t3->d2[1]=(windx+1)+(windy+1)*4; NET_send(NETplayer,playerlist[tpl]->net,t3);


    //Compensation
    //***KALDOSH BIT0 USED***
    txtset(t,"Kaldosh"); if (txtsame(t,tnpc->name)){
      if ((playerlist[tpl]->GNPCflags[246]&1)==0){ playerlist[tpl]->GNPCflags[246]|=1;
      myobj3=OBJnew(); myobj3->type=87; myobj3->more2=0; myobj3->info=0;//add item: orb of the moons
      additemroot(tnpc,myobj3);
      }
    }

    if (tnamecolour){
      playerlist[tpl]->GNPCflags[245]=tnamecolour;
    }else{
      if (playerlist[tpl]->GNPCflags[245]==0){
setnamecolour_getrandom:
        x=rnd*256; x2=rnd*256; x3=rnd*256;
        if ((x+x2+x3)<256) goto setnamecolour_getrandom;
        playerlist[tpl]->GNPCflags[245]=x+(x2<<8)+(x3<<16);
      }
    }
    //send namecolour to client
    /*
    txtsetchar(t,246);
    txtaddchar(t,playerlist[tpl]->GNPCflags[245]&255);
    txtaddchar(t,(playerlist[tpl]->GNPCflags[245]>>8)&255);
    txtaddchar(t,(playerlist[tpl]->GNPCflags[245]>>16)&255);
    NET_send(NETplayer,playerlist[tpl]->net,t);
    */

    playerlist[tpl]->GNPCflags[36]=0;//cat abuse flag reset!

    //auto-sysban
    txtset(t2,tnpc->name); txtucase(t2);
    tfh=open2("sysban.txt",OF_READWRITE|OF_SHARE_COMPAT);
    if (tfh->h!=HFILE_ERROR){
sysban_nextplayer:
      txtfilein(t,tfh); txtucase(t);
      if (txtsame(t,t2)){
        txtsetchar(t3,247); txtaddchar(t3,1); NET_send(NETplayer,playerlist[tpl]->net,t3);//SYSBAN ON
      }
      if (t->l) goto sysban_nextplayer;
      close(tfh);
    }




    for (x=0;x<=7;x++){
      if (playerlist[tpl]->party[x]){
        if (playerlist[tpl]->GNPCflags[230+x]){
          myobj2=playerlist[tpl]->party[x]; tnpc2=(npc*)myobj2->more;
          //move them to death spot
          OBJmove_allow=TRUE;
          OBJmove2(myobj2,playerlist[tpl]->GNPCflags[230+x]&65535,playerlist[tpl]->GNPCflags[230+x]>>16);
          OBJmove_allow=FALSE;
          playerlist[tpl]->GNPCflags[230+x]=0;
          //now kill them using a callback!
          //the callback will do all the hard work for me
          tplayer=playerlist[tpl];
          myobj2=tplayer->party[x];
          tnpc=(npc*)myobj2->more;
          login_dead_callback=1; goto login_dead;
login_dead2: login_dead_callback=0;
        }
      }
    }
    //tplayer->GNPCflags[230+resu_partymember[i3]]=resu_x[i3]+(resu_y[i3]<<16);



    goto doneclmess;
foundclient:



    playerlist[tpl]->idle_connect=0;
    if (t->d2[0]==251) goto doneclmess;

    if (playerlist[tpl]->playinstrument){ //stop playing instrument?
      x2=0;
      x=t->d2[0];
      if (x==5) x2=1;
      if (x==6) x2=1;
      if (x==7) x2=1;
      if (x==12) x2=1;
      if (x==13) x2=1;
      if (x==15) x2=1;
      if (x==16) x2=1;
      if (x==17) x2=1;
      if (x==19) x2=1;
      if (x==20) x2=1;
      if (x==21) x2=1;
      if (x2){
        playerlist[tpl]->playinstrument--;
        if (playerlist[tpl]->playinstrument==0){
          txtset(t2,"?"); t2->d2[0]=29; NET_send(NETplayer,playerlist[tpl]->net,t2);
        }//==0
      }//x2
    }//playinstrument

    //cancel wizardeye
    if (playerlist[tpl]->wizardeyetimeleft){
      x2=0; x=t->d2[0];
      if (x==6) x2=1;
      if (x==7) x2=1;
      if (x==12) x2=1;
      if (x==13) x2=1;
      if (x==15) x2=1;
      if (x==16) x2=1;
      if (x==17) x2=1;
      if (x==19) x2=1;
      if (x==20) x2=1;
      if (x==21) x2=1;
      if (x2){
        txtset(t2,"??"); t2->d2[0]=36; t2->d2[1]=0; NET_send(NETplayer,playerlist[tpl]->net,t2);
        playerlist[tpl]->wizardeyetimeleft=0.0f;
      }
    }









    //process client message

    //???? if (NETplayer==tnet) CLIENTplayer=playerlist[tpl]; //CLIENTplayer<-tpl switch to player tpl


    if (t->d2[0]==244){//send leaving britannia message
      if (playerlist[tpl]->party[0]){
        myobj=playerlist[tpl]->party[0]; tnpc=(npc*)myobj->more;
        //1. send status message to ALL PLAYERS
        txtsetchar(t2,8);
        txtadd(t2,"Leaving Britannia: ");
        txtadd(t2,tnpc->name);
        for (z=0;z<=playerlist_last;z++){ if (z!=tpl){ if (playerlist[z]){ if (playerlist[z]->net){
          NET_send(NETplayer,playerlist[z]->net,t2);
        }}}}
        //2. send a local text sfx message
        //Virtue @is @leaving @Britannia (the red text is specifically to stop people pretending to leave)
        i3=SFnew(myobj->x,myobj->y);
        sf[i3].type=3;
        sf[i3].x=myobj->x; sf[i3].y=myobj->y-2;
        sf[i3].wait=2;
        sf[i3].more=tnpc->port;
        static txt* inouttxt;
        inouttxt=txtnew();
        txtset(inouttxt,"");
        txtset(t2,tnpc->name); txtadd(t2," is leaving Britannia.");
        txtaddcolouredname(inouttxt,t2,playerlist[tpl]);
        sf[i3].p=inouttxt;
      }//party[0]
      goto doneclmess;
    }

    if (t->d2[0]==248){//respawn
      playerlist[tpl]->respawn=255;
      if (t->l==2) playerlist[tpl]->respawn=t->d2[1];
      goto doneclmess;
    }

    if (t->d2[0]==5){
      playerlist[tpl]->idle=0;








      txtright(t,t->l-1);
      if (playerlist[tpl]->readmessage==FALSE){
        memcpy(&playerlist[tpl]->mf,t->d,(DWORD)&playerlist[tpl]->mobj-(DWORD)&playerlist[tpl]->mf);
        playerlist[tpl]->readmessage=TRUE;
      } //not second general input message
      else{ //second input message (interpret carefully)



        //update arrow keys
        x=t->d2[7]+(t->d2[8]<<8)+(t->d2[9]<<16)+(t->d2[10]<<24);

        if (playerlist[tpl]->key&KEYup) playerlist[tpl]->key-=KEYup;
        playerlist[tpl]->key+=(x&KEYup);
        if (playerlist[tpl]->key&KEYdown) playerlist[tpl]->key-=KEYdown;
        playerlist[tpl]->key+=(x&KEYdown);
        if (playerlist[tpl]->key&KEYleft) playerlist[tpl]->key-=KEYleft;
        playerlist[tpl]->key+=(x&KEYleft);
        if (playerlist[tpl]->key&KEYright) playerlist[tpl]->key-=KEYright;
        playerlist[tpl]->key+=(x&KEYright);

        playerlist[tpl]->key|=(x&KEYup2);
        playerlist[tpl]->key|=(x&KEYdown2);
        playerlist[tpl]->key|=(x&KEYleft2);
        playerlist[tpl]->key|=(x&KEYright2);
      } //second general input message

      //cancel wizardeye
      if (playerlist[tpl]->key&(KEYmbclick+KEYbreakcombat+KEYquan+KEYf1)){
        if (playerlist[tpl]->wizardeyetimeleft){
          if (playerlist[tpl]->key&KEYmbclick) playerlist[tpl]->key-=KEYmbclick;
          txtset(t2,"??"); t2->d2[0]=36; t2->d2[1]=0; NET_send(NETplayer,playerlist[tpl]->net,t2);
          playerlist[tpl]->wizardeyetimeleft=0.0f;
        }
      }
      goto doneclmess;
    } //5








    if (t->d2[0]==37){
      if (t->l<=3072){//IMPORTANT: ANY VOICE DATA ABOVE 3K IS IGNORED
        if (playerlist[tpl]->GNPCflags[33]&4){
          txtsetchar(t,250); txtaddchar(t,10); NET_send(NETplayer,playerlist[tpl]->net,t);
          goto doneclmess;
        }
        if (myobj=getprimarypartymemberobj(playerlist[tpl])){
          tnpc=(npc*)myobj->more;
          txtsetchar(t2,38);
          txtaddshort(t2,tnpc->port);

          txtset(t4,tnpc->name); txtadd(t4,":");
          txtset(t3,""); txtaddcolouredname(t3,t4,playerlist[tpl]);
          txtaddchar(t2,t3->l); txtadd(t2,t3);

          txtaddshort(t2,playerlist[tpl]->x); txtaddshort(t2,playerlist[tpl]->y);

          txtset(t3,t2);//t3=info but no voice data
          txtright(t,t->l-1);
          txtadd(t2,t);//add compressed voice data
          x2=playerlist[tpl]->x; y2=playerlist[tpl]->y;

          for (z=0;z<=playerlist_last;z++){ if (z!=tpl){ if (playerlist[z]){ if (playerlist[z]->net){

            //x3=playerlist[z]->x-15; y3=playerlist[z]->y-11;
            //if (x3<0) x3=0; if (y3<0) y3=0; if (x3>2016) x3=2016; if (y3>1000) y3=1000;
            getscreenoffset(playerlist[z]->x,playerlist[z]->y,&x3,&y3);

            if ((x2>=x3)&&(x2<(x3+32))){ if ((y2>=y3)&&(y2<(y3+24))){
              if (playerlist[z]->mixer_volume){
                NET_send(NETplayer,playerlist[z]->net,t2);
              }else{
                NET_send(NETplayer,playerlist[z]->net,t3);
              }
            }}
          }}}}//x2 loop & checks
        }//myobj
      }//data size limit
      goto doneclmess;
    }//37

    if (t->d2[0]==39){//update player->miver_volume on host
      playerlist[tpl]->mixer_volume=t->d2[1];
      goto doneclmess;
    }//39

    if (t->d2[0]==40){//get volume levels of other players in area
      txtset(t,"?");
      txtset(t2,"????");
      txtset(t3,"?");
      t->d2[0]=41;
      //(x2,y2)=top-left corner of requesting player's screen
      //x2=playerlist[tpl]->x-15; y2=playerlist[tpl]->y-11; if (x2<0) x2=0; if (y2<0) y2=0; if (x2>2016) x2=2016; if (y2>1000) y2=1000;
      getscreenoffset(playerlist[tpl]->x,playerlist[tpl]->y,&x2,&y2);

      for (z=0;z<=playerlist_last;z++){ if (z!=tpl){ if (playerlist[z]){ if (playerlist[z]->net){
        x3=playerlist[z]->x; y3=playerlist[z]->y;
        if ((x3>=x2)&&(x3<(x2+32))){ if ((y3>=y2)&&(y3<(y2+24))){
          if (myobj=playerlist[z]->party[0]){
            txtaddlong(t,playerlist[z]->id);
            txtaddchar(t,playerlist[z]->mixer_volume);
          }else{
            //send volume at last known position
            txtaddlong(t,0xFFFFFFFF);//ID not being sent, use static position
            txtaddshort(t,x3); txtaddshort(t,y3);
            txtaddchar(t,playerlist[z]->mixer_volume);
          }
        }}
      }}}}//z loop & checks
      if (t->l>1){
        NET_send(NETplayer,playerlist[tpl]->net,t);
      }
      if (playerlist[tpl]->GNPCflags[33]&4) goto doneclmess;//don't update "speaking..." if banned
      playerlist[tpl]->speaking=3.0f;
      goto doneclmess;
    }//40


    if (t->d2[0]==42){//request portrait data
      ////goto doneclmess;

      txtright(t,t->l-1);
      x2=t->ds[0];
      if (HOST_portrait_loaded[x2]){
        txtsetchar(t,43); txtaddshort(t,x2);
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
        bitsi=0;

        static unsigned short *sp;
        txtNEWLEN(t2,-16384);

        static long r,g,b,r2,g2,b2,r3,g3,b3,repeat;

        sp=HOST_portrait_data[x2];
        b=0; g=0; r=0;
        repeat=0;

        for (x3=0;x3<=3583;x3++){
          b2=sp[x3]&31; g2=(sp[x3]>>6)&31; r2=(sp[x3]>>11)&31;

          if ((b==b2)&&(g==g2)&&(r==r2)&&(repeat<32)){
            repeat++; goto llc_pixeldone;
          }
          if (repeat){
            BITSadd(t2,&bitsi,0,2); BITSadd(t2,&bitsi,repeat-1,5);
            repeat=0;
          }

          b3=b2; if (b3<b) b3+=32;
          g3=g2; if (g3<g) g3+=32;
          r3=r2; if (r3<r) r3+=32;
          if (((b3-b)<=7)&&((g3-g)<=7)&&((r3-r)<=7)){//compression possible
            if (((b3-b)>=0)&&((g3-g)>=0)&&((r3-r)>=0)){//compression possible

              //if ((b2>=b)&&(g2>=g)&&(r2>=r)){//ascending
              //if (((b2-b)<=7)&&((g2-g)<=7)&&((r2-r)<=7)){//compression possible
              BITSadd(t2,&bitsi,2,2);
              BITSadd(t2,&bitsi,b3-b,3);
              BITSadd(t2,&bitsi,g3-g,3);
              BITSadd(t2,&bitsi,r3-r,3);
              goto llc_pixeldone;
            }
          }

          b3=b2; if (b3>b) b3-=32;
          g3=g2; if (g3>g) g3-=32;
          r3=r2; if (r3>r) r3-=32;
          if (((b3-b)>=-7)&&((g3-g)>=-7)&&((r3-r)>=-7)){//compression possible
            if (((b3-b)<=0)&&((g3-g)<=0)&&((r3-r)<=0)){//compression possible

              //if ((b2<=b)&&(g2<=g)&&(r2<=r)){//decending
              //if (((b-b2)<=7)&&((g-g2)<=7)&&((r-r2)<=7)){//compression possible
              BITSadd(t2,&bitsi,3,2);
              BITSadd(t2,&bitsi,b-b3,3);
              BITSadd(t2,&bitsi,g-g3,3);
              BITSadd(t2,&bitsi,r-r3,3);
              goto llc_pixeldone;
            }
          }

          BITSadd(t2,&bitsi,1,2);//no optomization was possible :(
          BITSadd(t2,&bitsi,b2,5);
          BITSadd(t2,&bitsi,g2,5);
          BITSadd(t2,&bitsi,r2,5);

llc_pixeldone:
          b=b2; g=g2; r=r2;
        }//x3

        if (repeat){
          BITSadd(t2,&bitsi,0,2); BITSadd(t2,&bitsi,repeat-1,5);
        }
        t2->l=bitsi>>3; if (bitsi&7) t2->l++; t2->d2[t2->l]=0;

        //exit(t2->l);
        //4776


        //BITSadd(t,&bitsi,z,8);//scene update
        /*

        3959 Kaldosh
        4447 Mose
        4714 Virtue

        1	if on the last known colour value (default is black) should be repeated
        if(on){

        loop
        }
        1	if on the following colour can't be "created"
        if (on){
        5:5:5 fixed colour
        loop
        }
        1   if on acending else decending











        static unsigned short *sp;
        txtNEWLEN(t2,-7168);
        //memcpy(t2->d,HOST_portrait_data[x2],7168);

        //6233

        //11 11 11
        /*
        bit 0: rle compression (yes/no)
        bit 1: increase brightness (yes/no)
        bits [2-3]: blue
        bits [4-5]: green
        bits [6-7]: red


        0 rle compression
        1 increase brightness
        2 decrease brightness
        3 ?


        bit 0 
        bit 1

        (if sensitive compression means increase brightness)
        bit 2-3 red (0-3)
        bit 2-3 green (0-3)
        bit 2-3 blue (0-3)

        next 6 bits






        sp=HOST_portrait_data[x2];
        y2=-1;
        y4=0;
        x5=0;
        for (x3=0;x3<=3584;x3++){
        y3=sp[x3]; if (y3&32) y3-=32;
        if ((y3!=y2)||(y4==128)||(x3==3584)){
        if (y2!=-1){
        if (y4){
        //repeats
        t2->d2[x5]=(y2&255)+32; x5++;
        t2->d2[x5]=(y2>>8)&255; x5++;
        t2->d2[x5]=y4-1; x5++;
        }else{//y4
        //no repeats
        t2->d2[x5]=y2&255; x5++;
        t2->d2[x5]=(y2>>8)&255; x5++;
        }//y4
        }//y2!=-1
        y2=y3;
        y4=0;
        }else{
        y4++;
        }
        }//x3
        t2->d2[x5]=0;
        t2->l=x5;

        //rescan using sensitive compression avoiding rle content
        static long r,g,b,r2,g2,b2;
        txtNEWLEN(t3,-7168);
        y2=-1;//last known colour value
        x5=0;
        for (x3=0;x3<t2->l;x3++){
        y3=t2->d2[x3];
        if (y3&32){//rle content
        y2=-1; //(y3-32)+(t2->d2[x3+1]<<8);//last known color value
        t3->d2[x5]=y3; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++;
        goto skiprlecontent;
        }
        if (y2!=-1){
        //is y3 close to y2
        b=y2&31; g=(y2>>6)&31; r=(y2>>11)&31;
        b2=y3&31; g2=(y3>>6)&31; r2=(y3>>11)&31;
        y4=0;
        if ((b2>=b)&&(g2>=g)&&(r2>=r)) y4=1;
        if ((b2<=b)&&(g2<=g)&&(r2<=r)) y4=-1;
        if (y4==1){
        if (((b2-b)<=3)&&((g2-g)<=3)&&((r-r2)<=3)){
        MessageBox(NULL,"saved another assending byte","Ultima 6 Online",MB_OK);
        }
        }
        if (y4==-1){
        if (((b-b2)<=3)&&((g-g2)<=3)&&((r-r2)<=3)){
        MessageBox(NULL,"saved another decending byte","Ultima 6 Online",MB_OK);
        }
        }
        //add as is
        t3->d2[x5]=y3; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++;
        }else{
        //add as is
        t3->d2[x5]=y3; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++;
        }
        y2=y3;//last known colour value
        skiprlecontent:;
        }//x3
        t3->d2[x5]=0;
        t3->l=x5;








        exit (t2->l);

        */


        txtadd(t,t2);
        NET_send(NETplayer,playerlist[tpl]->net,t);
      }//HOST_portrait_loaded[x2]
      goto doneclmess;
    }//42


    if (t->d2[0]==45){
      if (myobj=playerlist[tpl]->party[t->d2[1]]){
        tnpc=(npc*)myobj->more;
        x2=t->ds[1]-120; y2=t->ds[2]-120;
        if ((x2>=0)&&(y2>=0)){
          x2/=32;
          y2/=32;
          x2=y2*4+x2;
          x2+=tnpc->baseitem_offset;
          if (myobj=(object*)tnpc->baseitem->more){
setnot4sale:
            if (myobj){
              if (x2){
                x2--;
                myobj=(object*)myobj->next;
                goto setnot4sale;
              }

              if ((obji[sprlnk[myobj->type&1023]].flags&(1+2+4+8+16+32+64))==0){
                txtsetchar(t2,8); txtaddchar(t2,255); txtadd(t2,"Only wieldable items may be marked as not for sale."); NET_send(NETplayer,playerlist[tpl]->net,t2);
                goto doneclmess;
              }

              if (myobj->info&256){
                txtsetchar(t2,8); txtaddchar(t2,255); txtadd(t2,"Quest items cannot be marked as not for sale."); NET_send(NETplayer,playerlist[tpl]->net,t2);
                goto doneclmess;
              }

              if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){
                txtsetchar(t2,8); txtaddchar(t2,255); txtadd(t2,"That cannot be marked as not for sale."); NET_send(NETplayer,playerlist[tpl]->net,t2);
                goto doneclmess;
              }

              if (obji[sprlnk[myobj->type&1023]].flags&1024){ //container
                txtsetchar(t2,8); txtaddchar(t2,255); txtadd(t2,"That cannot be marked as not for sale."); NET_send(NETplayer,playerlist[tpl]->net,t2);
                goto doneclmess;
              }

              if (myobj->type==16){//magic shield
                txtsetchar(t2,8); txtaddchar(t2,255); txtadd(t2,"That cannot be marked as not for sale."); NET_send(NETplayer,playerlist[tpl]->net,t2);
                goto doneclmess;
              }

              myobj->info^=32768;
              tnpc->upflags|=32;

            }//myobj
          }//myobj of baseitem->more
        }//x2,y2
        goto doneclmess;
      }
    }//45

    /*
    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==5)){
    userkey=0; CLIENTplayer->key-=KEYmbclick;
    //send message to host
    if (CLIENTplayer->mf<=7){
    txtsetchar(t2,45); txtaddchar(t2,CLIENTplayer->mf);
    txtaddshort(t2,CLIENTplayer->mx); txtaddshort(t2,CLIENTplayer->my);
    NET_send(NETplayer,NULL,t2);
    }//CLIENTplayer->mf<=7
    }//not4sale
    */





    if ((t->d2[0]==6)||(t->d2[0]==12)){
      playerlist[tpl]->typing=0;
      playerlist[tpl]->idle=0;

      x5=0; if (t->d2[0]==12){
        x5=1; //global message
        if (playerlist[tpl]->globalmessage_wait!=0){
          txtset(t,"?"); t->d2[0]=8; txtaddchar(t,255); txtadd(t,"Wait 10 seconds between sending global messages!");
          NET_send(NETplayer,playerlist[tpl]->net,t);
          goto doneclmess;
        }
        playerlist[tpl]->globalmessage_wait=11-1;
      }

      txtright(t,t->l-1);



      if (t->d2[0]==47){//"/"
        //PRIVILEGES: change
        /*
        PRIVILEGES: notes
        40-47		16x2-bit privilege settings representing privilege level from 0-3
        PRIVILEGE LEVEL 1	You can use the power on others eg. /BAN John
        You can revoke this LEVEL 1 PRIVILEGE from others eg. /UNGRANT BAN John (with the same privilege level or less)
        PRIVILEGE LEVEL 2	You can use the power on others eg. /BAN John
        You can give another the ability to use that power eg. /GRANT BAN John
        You can revoke this LEVEL 2 PRIVILEGE from others eg. /UNGRANT2 BAN John (with the same privilege level or less)
        PRIVILEGE LEVEL 3	You can use the power on others eg. /BAN John
        You can give another the ability to use that power eg. /GRANT BAN John
        You can give another the ability to give that power to others eg. /GRANT2 BAN John
        You can give another the ability to "give another the ability to give that power to others" eg. /GRANT3 BAN John
        You can revoke this LEVEL 3 PRIVILEGE from others eg. /UNGRANT3 BAN John (with the same privilege level or less)
        */
        static unsigned char privileges_index;
        txtset(t2,t);
        txtright(t2,t2->l-1);
        static unsigned char privileges_levelrequiredtochange;
        static unsigned char privileges_set;
        static unsigned char privileges_leveltochange;
        txtset(t3,"GRANT "); if (txtsearch(t2,t3)==1)		{privileges_leveltochange=1;	privileges_set=TRUE;	privileges_levelrequiredtochange=2;	goto privelegechange_requested;}
        txtset(t3,"UNGRANT "); if (txtsearch(t2,t3)==1)		{privileges_leveltochange=1;	privileges_set=FALSE;	privileges_levelrequiredtochange=1;	goto privelegechange_requested;}
        txtset(t3,"GRANT2 "); if (txtsearch(t2,t3)==1)		{privileges_leveltochange=2;	privileges_set=TRUE;	privileges_levelrequiredtochange=3;	goto privelegechange_requested;}
        txtset(t3,"UNGRANT2 "); if (txtsearch(t2,t3)==1)	{privileges_leveltochange=2;	privileges_set=FALSE;	privileges_levelrequiredtochange=2;	goto privelegechange_requested;}
        txtset(t3,"GRANT3 "); if (txtsearch(t2,t3)==1)		{privileges_leveltochange=3;	privileges_set=TRUE;	privileges_levelrequiredtochange=3;	goto privelegechange_requested;}
        txtset(t3,"UNGRANT3 "); if (txtsearch(t2,t3)==1)	{privileges_leveltochange=3;	privileges_set=FALSE;	privileges_levelrequiredtochange=3;	goto privelegechange_requested;}
        goto privelegechange_skip;
privelegechange_requested:
        txtright(t2,t2->l-t3->l);
        txtset(t3,"BAN "); if (txtsearch(t2,t3)==1)	{privileges_index=0; goto privelegechange_gotcommand;}
        txtset(t3,"UNBAN "); if (txtsearch(t2,t3)==1) {privileges_index=1; goto privelegechange_gotcommand;}
        txtset(t3,"KICK "); if (txtsearch(t2,t3)==1) {privileges_index=2; goto privelegechange_gotcommand;}
        txtset(t3,"DELETEPORTRAIT "); if (txtsearch(t2,t3)==1) {privileges_index=3; goto privelegechange_gotcommand;}
        txtset(t3,"BLOCKPORTRAITUPLOAD "); if (txtsearch(t2,t3)==1) {privileges_index=4; goto privelegechange_gotcommand;}
        txtset(t3,"UNBLOCKPORTRAITUPLOAD "); if (txtsearch(t2,t3)==1) {privileges_index=5; goto privelegechange_gotcommand;}
        txtset(t3,"BANVOICECHAT "); if (txtsearch(t2,t3)==1) {privileges_index=6; goto privelegechange_gotcommand;}
        txtset(t3,"UNBANVOICECHAT "); if (txtsearch(t2,t3)==1) {privileges_index=7; goto privelegechange_gotcommand;}
        txtset(t3,"SETMOTD "); if (txtsearch(t2,t3)==1) {privileges_index=8; goto privelegechange_gotcommand;}
        txtset(t3,"SYSBAN "); if (txtsearch(t2,t3)==1) {privileges_index=9; goto privelegechange_gotcommand;}
        txtset(t3,"UNSYSBAN "); if (txtsearch(t2,t3)==1) {privileges_index=9; goto privelegechange_gotcommand;}
        txtset(t3,"BANIP "); if (txtsearch(t2,t3)==1) {privileges_index=10; goto privelegechange_gotcommand;}
        txtset(t3,"UNBANIP "); if (txtsearch(t2,t3)==1) {privileges_index=11; goto privelegechange_gotcommand;}
        txtset(t3,"GETIP "); if (txtsearch(t2,t3)==1) {privileges_index=12; goto privelegechange_gotcommand;}

        //...
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Privileges change failed! (name of privilege to change is invalid)"); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
privelegechange_gotcommand:
        //PRIVILEGES: check requesting player has sufficient privileges
        static unsigned char privileges_requestingplayerlevel;
        x3=privileges_index/16;//globalflags[x3]
        x4=privileges_index-x3*16;//sub-index
        x4*=2;//multiply sub-index to get shifting factor
        privileges_requestingplayerlevel=(playerlist[tpl]->GNPCflags[x3+40]>>x4)&3; if (privileges_requestingplayerlevel<privileges_levelrequiredtochange) goto privilege_denied;





        /*
        //set x6 to (0-2)privilege level required
        if (x2&1){
        x6=x2/2;
        }else{
        if (x2<=1) x6=1; else x6=2;
        }
        x4=x3/32;//DWORD privilege is stored at (0-3)
        x5=x3&31;//bit privilege is stored at
        x6=x4+x6*4+40;
        if ((playerlist[tpl]->GNPCflags[x6]&(1<<x5))==0) goto privilege_denied;
        x6=x4+(x2/2)*4+40;
        */

        txtright(t2,t2->l-t3->l);
        //check name against characters currently in britannia
        for (y2=0;y2<=playerlist_last;y2++){ if (playerlist[y2]){
          if (myobj2=getprimarypartymemberobj(playerlist[y2])){ tnpc2=(npc*)myobj2->more;
          if (txtsamewithoutcase(tnpc2->name,t2)){
            //PRIVILEGES: check requesting player's privilege level is equal or higher than target player's privilege level
            static unsigned char privileges_targetplayerlevel;
            privileges_targetplayerlevel=(playerlist[y2]->GNPCflags[x3+40]>>x4)&3; if (privileges_requestingplayerlevel<privileges_targetplayerlevel) goto privilege_denied;
            x5=privileges_targetplayerlevel;
            //PRIVILEGES: edit privileges
            if (privileges_set){
              if (privileges_targetplayerlevel<privileges_leveltochange){
                privileges_targetplayerlevel=privileges_leveltochange;
              }else{
                txtsetchar(t2,8); txtadd(t2,"Privileges change failed! (player already has these privileges)"); NET_send(NETplayer,playerlist[tpl]->net,t2); goto doneclmess;
              }
            }else{//privileges_set: remove privilege
              if (privileges_targetplayerlevel>(privileges_leveltochange-1)){
                privileges_targetplayerlevel=privileges_leveltochange-1;
              }else{
                txtsetchar(t2,8); txtadd(t2,"Privileges change failed! (player does not currently have this privilege so privileges have not been changed)"); NET_send(NETplayer,playerlist[tpl]->net,t2); goto doneclmess;
              }
            }//privileges_set
            playerlist[y2]->GNPCflags[x3+40]-=(x5<<x4);//remove old privileges
            playerlist[y2]->GNPCflags[x3+40]+=(privileges_targetplayerlevel<<x4);//add new privileges
            txtset(t2,"?"); t2->d2[0]=8; txtadd(t2,"Privileges changed successfully! ("); txtadd(t2,t); txtadd(t2,")"); NET_send(NETplayer,playerlist[tpl]->net,t2);
            //record change on file record in save folder
            txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
            txtadd(t2,": "); txtadd(t2,t);
            tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
            seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
            goto doneclmess;
          }//name==t2
          }//party[0]
        }}//y2 loop
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Privileges change failed! (no player named "); txtadd(t,t2); txtadd(t," is currently online, check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
privelegechange_skip:

        //slash commands
        txtset(t3,"KICK "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=2; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=FIRST_CLIENT;x4<=playerlist_last;x4++){ 
          if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){

          if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
          playerlist[x4]->GNPCflags[253]|=1;
          txtset(t,"??"); t->d2[0]=250; t->d2[1]=7; NET_send(NETplayer,playerlist[x4]->net,t);
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }

        txtset(t3,"BAN "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=0; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        txtset(t9,"Kalour"); if (txtsamewithoutcase(t9,t2)) goto privilege_denied2;
        x5=0;
        for (x4=0;x4<=playerlist_last;x4++){ if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (x4==0){
            txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
          }
          x5=1;
        }}}}
        tfh=open2("ban.txt",OF_READWRITE|OF_SHARE_COMPAT); seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
        if (x5){
          txtset(t3,"?"); t3->d2[0]=8; txtadd(t3,t2); txtadd(t3," has been banned!"); NET_send(NETplayer,playerlist[tpl]->net,t3);
        }else{
          txtset(t3,"?"); t3->d2[0]=8; txtadd(t3,t2); txtadd(t3," is not currently in Britannia but has been banned!"); NET_send(NETplayer,playerlist[tpl]->net,t3);
        }
        txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
        txtadd(t2,": "); txtadd(t2,t);
        tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
        seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
        goto doneclmess;
        }

        txtset(t3,"UNBAN "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=1; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        txtucase(t2);
        tfh=open2("ban.txt",OF_READWRITE|OF_SHARE_COMPAT);
        y=0;
unban_loop:
        x4=seek(tfh); txtfilein(t3,tfh);
        if (t3->l){
          txtucase(t3);
          if (txtsame(t2,t3)){
            y=1;
            seek(tfh,x4);
            for (y2=0;y2<t3->l;y2++) t3->d2[y2]=42;
            txtfileout(t3,tfh);
          }//txtsame(t2,t3)
        }//t3->l
        if (x4<lof(tfh)) goto unban_loop;
        close(tfh);
        if (y){
          txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," was unbanned successfully!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
        }else{
          txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,"No player named "); txtadd(t4,t2); txtadd(t4," is currently banned!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
        }
        txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
        txtadd(t2,": "); txtadd(t2,t);
        tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
        seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
        goto doneclmess;
        }

        txtset(t3,"DELETEPORTRAIT "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=3; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=0;x4<=playerlist_last;x4++){ if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
          if (playerlist[x4]->GNPCflags[33]&1){
            playerlist[x4]->GNPCflags[33]--;
            playerlist[x4]->GNPCflags[253]|=1;//kick to stop player using existing portrait
            tnpc2->exp+=1000;
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4,"'s custom portrait deleted! (1000xp returned)"); NET_send(NETplayer,playerlist[tpl]->net,t4);
            txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
            txtadd(t2,": "); txtadd(t2,t);
            tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
            seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
            txtset(t,"??"); t->d2[0]=250; t->d2[1]=7; NET_send(NETplayer,playerlist[x4]->net,t);
          }else{
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," is not using a custom portrait!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          }
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }

        txtset(t3,"BLOCKPORTRAITUPLOAD "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=4; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=FIRST_CLIENT;x4<=playerlist_last;x4++){ 
          if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
          if ((playerlist[x4]->GNPCflags[33]&2)==0){
            playerlist[x4]->GNPCflags[33]|=2;
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," can no longer upload custom portraits!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
            txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
            txtadd(t2,": "); txtadd(t2,t);
            tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
            seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
          }else{
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," already cannot upload custom portraits!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          }
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }

        txtset(t3,"UNBLOCKPORTRAITUPLOAD "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=5; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=0;x4<=playerlist_last;x4++){ if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (playerlist[x4]->GNPCflags[33]&2){
            playerlist[x4]->GNPCflags[33]-=2;
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," can now upload custom portraits again!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
            txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
            txtadd(t2,": "); txtadd(t2,t);
            tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
            seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
          }else{
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," can already upload custom portraits!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          }
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }


        txtset(t3,"BANVOICECHAT "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=6; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=FIRST_CLIENT;x4<=playerlist_last;x4++){   
          if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
          if ((playerlist[x4]->GNPCflags[33]&4)==0){
            playerlist[x4]->GNPCflags[33]|=4;
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," is now banned from using voice chat!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
            txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
            txtadd(t2,": "); txtadd(t2,t);
            tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
            seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
          }else{
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," is already banned from using voice chat!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          }
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }

        txtset(t3,"UNBANVOICECHAT "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=7; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=0;x4<=playerlist_last;x4++){ if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (playerlist[x4]->GNPCflags[33]&4){
            playerlist[x4]->GNPCflags[33]-=4;
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," can now use voice chat again!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
            txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
            txtadd(t2,": "); txtadd(t2,t);
            tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
            seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
          }else{
            txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," can already use voice chat!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          }
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }

        txtset(t3,"SETMOTD "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=8; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        txtset(motd,t2);
        tfh=open2("motd.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
        txtfileout(motd,tfh);
        close(tfh);
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"MOTD set to \""); txtadd(t,t2); txtadd(t,"\""); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
        }//SETMOTD


        txtset(t3,"SYSBAN "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=9; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=FIRST_CLIENT;x4<=playerlist_last;x4++){   
          if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
          txtsetchar(t,247); txtaddchar(t,1); NET_send(NETplayer,playerlist[x4]->net,t);//SYSBAN ON
          txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," is now system banned! (In Britannia time limited to 8 seconds)"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }//SYSBAN

        txtset(t3,"UNSYSBAN "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=9; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=0;x4<=playerlist_last;x4++){ if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
          txtsetchar(t,247); txtaddchar(t,0); NET_send(NETplayer,playerlist[x4]->net,t);//SYSBAN OFF
          txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,t2); txtadd(t4," has been sent a release system ban message! (If the client has already quit this message may not have been received)"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }//UNSYSBAN

        txtset(t3,"BANIP "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=10; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        txtset(t9,"Kalour"); if (txtsamewithoutcase(t9,t2)) goto privilege_denied2;
        for (x4=FIRST_CLIENT;x4<=playerlist_last;x4++){ 
          if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          tfh=open2("banip.txt",OF_READWRITE|OF_SHARE_COMPAT);
          if (tfh->h==HFILE_ERROR) tfh=open2("banip.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
          seek(tfh,lof(tfh));
          txtset(t4,"");
          txtnumint(t3,playerlist[x4]->GNPCflags[242]&255); txtadd(t4,t3); txtadd(t4,".");
          txtnumint(t3,(playerlist[x4]->GNPCflags[242]>>8)&255); txtadd(t4,t3); txtadd(t4,".");
          txtnumint(t3,(playerlist[x4]->GNPCflags[242]>>16)&255); txtadd(t4,t3); txtadd(t4,".");
          txtnumint(t3,(playerlist[x4]->GNPCflags[242]>>24)&255); txtadd(t4,t3);
          txtfileout(t4,tfh);
          close(tfh);
          txtset(t3,"?"); t3->d2[0]=8; txtadd(t3,t2); txtadd(t3,"'s IP has been banned!"); NET_send(NETplayer,playerlist[tpl]->net,t3);
          txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
          txtadd(t2,": "); txtadd(t2,t);
          tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
          seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }//BANIP

        txtset(t3,"UNBANIP "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=11; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        txtucase(t2);
        tfh=open2("banip.txt",OF_READWRITE|OF_SHARE_COMPAT);
        if (tfh->h==HFILE_ERROR){
          txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,"The IP "); txtadd(t4,t2); txtadd(t4," is not currently banned!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          goto doneclmess;
        }
        y=0;
unbanip_loop:
        x4=seek(tfh); txtfilein(t3,tfh);
        if (t3->l){
          txtucase(t3);
          if (txtsame(t2,t3)){
            y=1;
            seek(tfh,x4);
            for (y2=0;y2<t3->l;y2++) t3->d2[y2]=46;//"."
            txtfileout(t3,tfh);
          }//txtsame(t2,t3)
        }//t3->l
        if (x4<lof(tfh)) goto unbanip_loop;
        close(tfh);
        if (y){
          txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,"The IP "); txtadd(t4,t2); txtadd(t4," was unbanned successfully!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
          txtset(t2,"UNKNOWN"); if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more; txtset(t2,tnpc2->name);}
          txtadd(t2,": "); txtadd(t2,t);
          tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_SHARE_COMPAT); if (tfh->h==HFILE_ERROR) tfh=open2(".\\save\\adminlog.txt",OF_READWRITE|OF_CREATE);
          seek(tfh,lof(tfh)); txtfileout(t2,tfh); close(tfh);
        }else{
          txtset(t4,"?"); t4->d2[0]=8; txtadd(t4,"The IP "); txtadd(t4,t2); txtadd(t4," is not currently banned!"); NET_send(NETplayer,playerlist[tpl]->net,t4);
        }
        goto doneclmess;
        }//UNBANIP

        txtset(t3,"GETIP "); if (txtsearch(t2,t3)==1){ txtright(t2,t2->l-t3->l);
        privileges_index=12; if (((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)==0) goto privilege_denied;
        for (x4=0;x4<=playerlist_last;x4++){ if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ tnpc2=(npc*)myobj2->more; if (txtsamewithoutcase(tnpc2->name,t2)){
          //FOLLOWING LINE IS UNREQUIRED FOR GETIP
          //if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
          txtset(t4,"?"); t4->d2[0]=8; 
          txtadd(t4,t2);
          txtadd(t4,"'s IP is ");
          txtnumint(t,playerlist[x4]->GNPCflags[242]&255); txtadd(t4,t); txtadd(t4,".");
          txtnumint(t,(playerlist[x4]->GNPCflags[242]>>8)&255); txtadd(t4,t); txtadd(t4,".");
          txtnumint(t,(playerlist[x4]->GNPCflags[242]>>16)&255); txtadd(t4,t); txtadd(t4,".");
          txtnumint(t,(playerlist[x4]->GNPCflags[242]>>24)&255); txtadd(t4,t);
          NET_send(NETplayer,playerlist[tpl]->net,t4);
          goto doneclmess;
        }}}}
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"No player named "); txtadd(t,t2); txtadd(t," is in Britannia! (check uppercase/lowercase)"); NET_send(NETplayer,playerlist[tpl]->net,t); goto doneclmess;
        }//GETIP
        txtset(t4,t2); txtucase(t4); txtset(t3,"QUITHOST"); if (txtsame(t4,t3)) {
          if (myobj2=getprimarypartymemberobj(playerlist[tpl])){ tnpc2=(npc*)myobj2->more;}
          x4=0;
          while (admins[x4]) {
            if (txtsame(tnpc2->name,admins[x4])){
              u6orevive=255; seek(u6orevive_fh,0); put(u6orevive_fh,&u6orevive,1);
              exitrequest=TRUE; exitrequest_noconfirm=TRUE; //exitrequest is required for the host + client version to quit properly.
              for (x4=0;x4<=playerlist_last;x4++){ 
                if (playerlist[x4]){ if (myobj2=getprimarypartymemberobj(playerlist[x4])){ /* send host quit message to all players */
                  txtset(t,"??"); t->d2[0]=250; t->d2[1]=1; NET_send(NETplayer,playerlist[x4]->net,t);
                }}
              }
              goto doneclmess;
            }
            x4++;
            if (x4==ADMINSMAX) {break;}   
          }
          goto privilege_denied;
        }
        txtset(t4,t2); txtucase(t4); txtset(t3,"ME "); if (txtsearch(t4,t3)==1) goto anotherslashcommand;
        txtset(t4,t2); txtucase(t4); txtset(t3,"RETURNBODY"); if (txtsearch(t4,t3)==1) goto anotherslashcommand;

        txtsetchar(t,8); txtadd(t,"Unknown slash (/) command!"); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;

privilege_denied:
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Incorrect privileges!"); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
privilege_denied2:
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"This player's privilege level is higher than yours!"); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
      }//"/"
anotherslashcommand:

	  // t111
	  if (easymodehostn1) {

      txtset(t2,t); txtucase(t2);
      txtset(t3,"`ADD");
//    if (txtsame(t2,t3)){
      if ((txtsearch(t2,t3)==1)){
			int mobnum = 0;
			int mobcount = 0;

		    txtset(t3,"`ADD1");
			if (txtsame(t2, t3))
				mobnum = OBJ_SWASHBUCKLER;

		    txtset(t3,"`ADD2");
			if (txtsame(t2, t3))
				mobnum = OBJ_FIGHTER;

		    txtset(t3,"`ADD3");
			if (txtsame(t2, t3))
				mobnum = OBJ_GUARD;
			
		    txtset(t3,"`ADD9");
			if (txtsame(t2, t3))
				mobnum = OBJ_GARGOYLE_WINGED + 1024 * 3;

        //txtsetchar(t,8); //txtaddchar(t,255);
        //txtadd(t,"Mobs spawned!");
		//NET_send(NETplayer,playerlist[tpl]->net,t);

		int basex = 252; // 254
		int basey = 448; // 448
		int hx, hy, hi, hi2;
		//int mi = 0;
		//unsigned long moverflagbackup = MOVERNEW_GLOBALFLAGS;
		//MOVERNEW_GLOBALFLAGS=0;
		//int mobcountnew = 0;
		//object* o;
		//for (hi = 0; hi < mobcountnew; hi++) {
			//OBJremove(moblistnew[hi]);
			//OBJrelease(moblistnew[hi]);
		//}

		//mobcount = 0;
		//mobcountnew = 0;

		/*
		//bucket
		//hx=266; hy=448;
		hx=basex+12; hy=basey;
		OBJaddnew(hx+2,hy+3,OBJ_BUCKET+1024*0,0,0);//bucket
		//OBJaddnew(hx+1,hy+6,OBJ_BUCKET+1024*0,0,0);//bucket
		//mobs
		//OBJaddnew(1132,655,OBJ_SWASHBUCKLER+1024*0,0,0);
		//OBJaddnew(1135,655,OBJ_FIGHTER+1024*0,0,0);
		//hx=255; hy=448+1;
		hx=basex+2; hy=basey+1;
		for (hi2=hy; hi2 < hy+7; hi2++) {
			for (hi=hx; hi < hx+2; hi++) {
				if (mobnum == 0) {
					if (hi2 % 2 == 0)
						//moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_SWASHBUCKLER + 1024 * 0, 0, 0);
					OBJaddnew(hi,hi2,OBJ_SWASHBUCKLER+1024*0,0,0);
					//o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
					//OBJaddnew(hi,hi2,OBJ_DRAKE+1024*0,0,0);
					else
						//moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_FIGHTER + 1024 * 0, 0, 0);
					OBJaddnew(hi,hi2,OBJ_FIGHTER+1024*0,0,0);
					//o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
				//ENHANCEnewn(o, 0, 2);
				}
				else
					//moblistnew[mi] = OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);
					OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);

				//o=OBJaddnew(hi,hi2,OBJ_SWORD+1024*0,0,0);
				//ENHANCEnewn(o, 0, 4);
				mobcount++;
				//mi++;

				if (mobcount >= addmobnum)
					break;
			}

			if (mobcount >= addmobnum)
				break;
		}
		//mobcount = mi;
		//mobcountnew = mi;
		*/

		/*
		hx=basex+12; hy=basey;
		OBJaddnew(hx+1,hy+1,OBJ_DEAD_BODY+1024*1,0,0);//bucket
		OBJaddnew(hx+2,hy+1,OBJ_DEAD_BODY+1024*2,0,0);//bucket
		OBJaddnew(hx+3,hy+1,OBJ_DEAD_BODY+1024*3,0,0);//bucket
		OBJaddnew(hx+1,hy+2,OBJ_DEAD_BODY+1024*4,0,0);//bucket
		OBJaddnew(hx+2,hy+2,OBJ_DEAD_BODY+1024*5,0,0);//bucket
		OBJaddnew(hx+3,hy+2,OBJ_DEAD_BODY+1024*6,0,0);//bucket
		OBJaddnew(hx+1,hy+3,OBJ_WOMAN+1024*0,0,0);//bucket
		OBJaddnew(hx+2,hy+3,OBJ_MAGE+1024*0,0,0);//bucket
		OBJaddnew(hx+3,hy+3,OBJ_PEASANT+1024*0,0,0);//bucket
		*/

		int arenaid = getarenaid(playerlist[tpl]);
		hx=basex+12; hy=basey;
		//OBJaddnew(hx+2,hy+3,OBJ_BUCKET+1024*0,0,0);//bucket

        txtsetchar(t,8); // status msg

		//mobs
		if (arenaid >= 0) {
			hx = arenalocx[arenaid] + 2;
			hy = arenalocy[arenaid] + 1;
			//object* mmm;
			//creature* ccc;

			for (hi2 = hy; hi2 < hy + 7; hi2++) {
				for (hi = hx; hi < hx + 2; hi++) {
					if (mobnum == 0) {
						if (hi2 % 2 == 0)
							//moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_SWASHBUCKLER + 1024 * 0, 0, 0);
							OBJaddnew(hi, hi2, OBJ_SWASHBUCKLER + 1024 * 0, 0, 0);
						//o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
						//OBJaddnew(hi,hi2,OBJ_DRAKE+1024*0,0,0);
						else
							//moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_FIGHTER + 1024 * 0, 0, 0);
							OBJaddnew(hi, hi2, OBJ_FIGHTER + 1024 * 0, 0, 0);
						//o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
					//ENHANCEnewn(o, 0, 2);
					}
					else
						//moblistnew[mi] = OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);
						OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);

					//ccc = (creature*)mmm->more;
					//ccc->al = 1;
					//o=OBJaddnew(hi,hi2,OBJ_SWORD+1024*0,0,0);
					//ENHANCEnewn(o, 0, 4);
					mobcount++;
					//mi++;

					if (mobcount >= arenaaddmobnum[arenaid])
						break;
				}

				if (mobcount >= arenaaddmobnum[arenaid])
					break;
			}

			txtadd(t, "Arena #");
			txtnumint(t3, arenaid+1);
			txtadd(t, t3);
			txtadd(t, ": ");
	        txtadd(t,"Mobs spawned!");
		} else {
			txtadd(t, "You must be in an arena to spawn mobs.");
		}

		NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
      }

	  // t111
      txtset(t2,t); txtucase(t2);
      txtset(t3,"`SET");
//    if (txtsame(t2,t3)){
	  if (num = (txtsearch(t2, t3) == 1)) {
		int l = t2->l - num - 4;
		int arenaid = getarenaid(playerlist[tpl]);

		txtsetchar(t, 8); // status msg

		if (arenaid >= 0) {
			txtadd(t, "Arena #");
			txtnumint(t3, arenaid+1);
			txtadd(t, t3);
			txtadd(t, ": ");

			if (l > 0)
			{
				txtright(t2, l);
				num = (int)txtnum(t2);

				if (num > 0) {
					arenaaddmobnum[arenaid] = num;
					txtadd(t, "Mob spawn amount set to ");
					txtadd(t, t2);
				}
			}
			else
				num = -1;

			if (num <= 0) {
				txtadd(t, "Mob spawn amount is invalid.");
			}
		} else {
			txtadd(t, "You must be in an arena to set the spawn amount.");
		}

		NET_send(NETplayer, playerlist[tpl]->net, t);
		goto doneclmess;
	  }

      txtset(t2,t); txtucase(t2);
      txtset(t3,"`KARMA");
	  if ((txtsearch(t2, t3) == 1)) {
		txtsetchar(t, 8); //txtaddchar(t,255);
		txtadd(t, "Your karma is ");
		//txtnumint(t2, tplayer->karma);
		txtnumint(t2, playerlist[tpl]->karma);
		txtadd(t, t2);

		// 1076,620 to 74,84
		//unsigned long zz = 5505098;
		/*
		unsigned long zz = 25231719;
		txtset(t3, " Coord x=");
		txtnumint(t4, zz&1023); txtadd(t3, t4); txtadd(t3, " y=");
		txtnumint(t4, (zz>>16)&1023); txtadd(t3, t4);
		LOGadd(t3);
		*/
		// 1330, 410
		// 1310, 420 2nd floor view to 1st
		// BTset(1310,420,111);
		// OBJaddnew(1310,420,416+1024*0,0,25231719);//pointing at something // OBJ_NOTHING
		// 1310,320 views to 359, 385
		// 1308,411 2nd floor ladder

		NET_send(NETplayer, playerlist[tpl]->net, t);
		goto doneclmess;
	  }

      txtset(t2,t); txtucase(t2);
      txtset(t3,"`LOC");
	  if ((txtsearch(t2, t3) == 1)) {
		txtsetchar(t, 8); //txtaddchar(t,255);
		txtadd(t, "Your location is x=");
		txtnumint(t2, playerlist[tpl]->x);
		txtadd(t, t2);
		txtadd(t, " y=");
		txtnumint(t2, playerlist[tpl]->y);
		txtadd(t, t2);

		NET_send(NETplayer, playerlist[tpl]->net, t);
		goto doneclmess;
	  }

	  } // t111


      txtset(t2,t); txtucase(t2);
      txtset(t3,"WHO");
      if (txtsame(t2,t3)){
        txtsetchar(t,8); //txtaddchar(t,255);
        txtadd(t,inbritannia); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
      }

      txtset(t3,"RESYNC"); if (txtsame(t2,t3)){
        playerlist[tpl]->resync=1;
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Resync request received."); NET_send(NETplayer,playerlist[tpl]->net,t);
        // luteijn: let's see if this helps.
        //send time
        txtset(t2,"?"); t2->d2[0]=32;
        txtset(t3,"12345678"); t3->dd[0]=btime2;
        txtadd(t2,t3);
        NET_send(NETplayer,tplayer->net,t2);

        goto doneclmess;
      }


      txtset(t3,"/RETURNBODYFOLLOWERS"); if (txtsame(t2,t3)){
        //IMPORTANT NOTE: RETURNBODYFOLLOWERS MAY CAUSE SOME VISUAL INVENTORY PROBLEMS
        //ignore request if player is not dead
        for (i2=1;i2<=7;i2++){
          //locate player's body on resurrect list
          for (i=0;i<=nresu;i++){
            if (resu[i]){
              if (resu_player[i]==playerlist[tpl]){
                if (resu_partymember[i]==i2){
                  goto returnbodyfollowers_getresuindex;
                }}}}
          goto returnbodyfollowers_next;//unlisted!
returnbodyfollowers_getresuindex:
          //remove current body
          //IMPORTANT NOTE: IF A RECREATED OBJECT OF THE SAME TYPE USING SAME POINTER EXISTED THE FOLLOWING CHECK WOULD PRODUCE UNPREDICTABLE RESULTS
          if (resu_body[i]->type==resu_body_type[i]){//object has not been released or reused
            //check that resu_body[i] is not an mobj of a player
            for (z=0;z<=playerlist_last;z++){ if (playerlist[z]){ if (playerlist[z]->net){
              if (playerlist[z]->mobj==resu_body[i]) playerlist[z]->mobj=NULL;
            }}}
            OBJremove(resu_body[i]);
            OBJrelease(resu_body[i]);
          }//resu_body[i]->type==resu_body_type[i]
          //create a new body at death spot
          myobj=OBJnew();
          myobj->type=resu_body_type[i];
          myobj->info|=112;
          OBJadd(resu_x[i],resu_y[i],myobj);
          //link with new dead body
          resu_body[i]=myobj;
returnbodyfollowers_next:;
        }//i2
        goto doneclmess;
      }

      txtset(t3,"/RETURNBODY"); if (txtsame(t2,t3)){
        //IMPORTANT NOTE: RETURNBODY MAY CAUSE SOME VISUAL INVENTORY PROBLEMS
        //ignore request if player is not dead
        if (playerlist[tpl]->party[0]) goto doneclmess;
        //locate player's body on resurrect list
        for (i=0;i<=nresu;i++){
          if (resu[i]){
            if (resu_player[i]==playerlist[tpl]){
              if (resu_partymember[i]==0){
                goto returnbody_getresuindex;
              }}}}
        goto doneclmess;//unlisted!
returnbody_getresuindex:
        //remove current body
        //IMPORTANT NOTE: IF A RECREATED OBJECT OF THE SAME TYPE USING SAME POINTER EXISTED THE FOLLOWING CHECK WOULD PRODUCE UNPREDICTABLE RESULTS
        if (resu_body[i]->type==resu_body_type[i]){//object has not been released or reused
          //check that resu_body[i] is not an mobj of a player
          for (z=0;z<=playerlist_last;z++){ if (playerlist[z]){ if (playerlist[z]->net){
            if (playerlist[z]->mobj==resu_body[i]) playerlist[z]->mobj=NULL;
          }}}
          OBJremove(resu_body[i]);
          OBJrelease(resu_body[i]);
        }//resu_body[i]->type==resu_body_type[i]
        //create a new body at death spot
        myobj=OBJnew();
        myobj->type=resu_body_type[i];
        myobj->info|=112;
        OBJadd(resu_x[i],resu_y[i],myobj);
        //link with new dead body
        resu_body[i]=myobj;
        goto doneclmess;
      }

      if (x5&&(playerlist[tpl]->party[0]==NULL)) goto dglobal3;
      if(myobj=playerlist[tpl]->party[0]){

        txtset(t2,"KAL LOR");
        if (txtsame(t,t2)){
          tplayer=playerlist[tpl];
          if ((tplayer->party[0]->x==368)&&(tplayer->party[0]->y==386)) goto help_skip;
          if (tplayer->kallor) goto help_skip;
          tplayer->kallor=32;
          txtset(t,"?"); t->d2[0]=8; txtadd(t,"\x022KAL LOR!\x022 (You will receive help in 30 seconds.)"); NET_send(NETplayer,tplayer->net,t);
help_skip:
          goto doneclmess;
        }

dglobal3:

        static unsigned char me_message;
        me_message=0;
        txtset(t2,"/ME ");
        txtset(t3,t); txtucase(t3); txtleft(t3,4);
        if (txtsame(t3,t2)){
          txtright(t,t->l-4);
          if (t->l==0) goto doneclmess;
          me_message=1;
        }

        if (x5) i3=SFnew(2047,1023); else i3=SFnew(myobj->x,myobj->y);

        //i3 points to new sf
        sf[i3].type=3; //message to users

        if (playerlist[tpl]->party[0]){sf[i3].x=myobj->x; sf[i3].y=myobj->y-2;}

        if (x5) { sf[i3].x=2047; sf[i3].y=1023; }

        sf[i3].wait=2;
        tnpc=NULL; if (playerlist[tpl]->party[0]) tnpc=(npc*)myobj->more;
        if (playerlist[tpl]->party[0]){
          sf[i3].more=tnpc->port;
          //addnpcport2sf(unsigned long n,npc *tnpc)
          //addnpcport2sf(i3,tnpc);
        }else{
          sf[i3].more=playerlist[tpl]->port;
        }



        static txt* inouttxt;
        inouttxt=txtnew();

        if (playerlist[tpl]->party[0]){
          txtset(t2,tnpc->name);
        }else{
          txtset(t2,playerlist[tpl]->npcname);
        }
        txtset(inouttxt,"");




        if (me_message){
          txtaddcolouredname(inouttxt,t2,playerlist[tpl]);
          txtadd(inouttxt," ");
          txtadd(inouttxt,t);
        }else{
          txtadd(t2,":");
          txtaddcolouredname(inouttxt,t2,playerlist[tpl]);
          txtadd(inouttxt," \x022");
          txtadd(inouttxt,t);
          txtadd(inouttxt,"\x022");
        }

        sf[i3].p=inouttxt;
        //txtset(t,"message ["); txtadd(t,inouttxt); txtadd(t,"] ("); txtnumint(t2,ett); txtadd(t,t2); txtadd(t,")"); txtfileout(t,log2);
        if (tnpc){
          txtset(t,"Message:");txtadd(t,tnpc->name); txtadd(t,":"); txtadd(t,inouttxt); LOGadd(t);
        }

      }
      goto doneclmess;
    } //6

    if (t->d2[0]==34){//chat
      playerlist[tpl]->typing=0;
      playerlist[tpl]->idle=0;
      if (playerlist[tpl]->party[0]==NULL) goto doneclmess;
      if (myobj=(object*)playerlist[tpl]->talk_target->more){
        //check if target is onscreen
        //tpx=playerlist[tpl]->x-15; tpy=playerlist[tpl]->y-11; if (tpx<0) tpx=0; if (tpy<0) tpy=0; if (tpx>2016) tpx=2016; if (tpy>1000) tpy=1000;
        getscreenoffset(playerlist[tpl]->x,playerlist[tpl]->y,&tpx,&tpy);


        x=myobj->x; y=myobj->y;
        if ((x>=tpx)&&(y>=tpy)&&(x<=(tpx+31))&&(y<=(tpy+23))){
          x=x-tpx;
          y=y-tpy;
          txtright(t,t->l-1);
          goto chat;
        }//x,y check
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot chat to people you cannot see."); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
      }//->more
      txtset(t,"?"); t->d2[0]=8; txtadd(t,"Chatting to yourself again?"); NET_send(NETplayer,playerlist[tpl]->net,t);
      goto doneclmess;//no talk target or invalid talk target
    }//34

    if (t->d2[0]==7){ //text message to NPC/player (NPC message)
      playerlist[tpl]->typing=0;
      playerlist[tpl]->idle=0;
      if (playerlist[tpl]->party[0]==NULL) goto doneclmess;

      txtright(t,t->l-1);
      x=t->ds[0]/32; y=t->ds[1]/32;
      txtright(t,t->l-4);

chat:

      //tpx=playerlist[tpl]->x-15; tpy=playerlist[tpl]->y-11; if (tpx<0) tpx=0; if (tpy<0) tpy=0; if (tpx>2016) tpx=2016; if (tpy>1000) tpy=1000;
      getscreenoffset(playerlist[tpl]->x,playerlist[tpl]->y,&tpx,&tpy);

      myobj=OBJfindlast(tpx+x,tpy+y); if (!myobj) myobj=OBJfindlastall(tpx+x,tpy+y);
      if (myobj){
        if (myobj->info&8) myobj=(object*)myobj->more;


        static unsigned char npcmess_shrine=0;
        npcmess_shrine=0;
        if (myobj->type==OBJ_FORCE_FIELD){ //force field
          myobj2=(object*)myobj->prev;
          if (playerlist[tpl]->GNPCflags[3]&(1<<(myobj2->type>>10))) npcmess_shrine=190+(myobj2->type>>10);
        }//force field
        if ((myobj->type&1023)==OBJ_WISP) npcmess_shrine=199; //wisp
        if ((myobj->type&1023)==OBJ_STOCKS){ if ((myobj->type>>11)&1){
          npcmess_shrine=179; //man in stocks
        }}
        if ((myobj->type&1023)==OBJ_SHRINE){
          myobj2=(object*)myobj->prev;
          if ((myobj2->type&1023)==329) npcmess_shrine=198; //alter of singularity
        }
        if ((myobj->type&1023)==OBJ_GUARD){
          if (myobj->info&4){ //crt
            crt=(creature*)myobj->more;
            if (crt->al==4) npcmess_shrine=200; //guard
          }//4
        }//382
        if (npcmess_shrine) goto npcmess_shrine1;


        //assume NPC type
        if ((myobj->info&2)==0) goto talk_skipnullobject;
        tnpc=(npc*)myobj->more;
        if (tnpc==NULL) goto talk_skipnullobject;

        //PRIVATE MESSAGE TO CLOSE PLAYER?
        if (tnpc->player){
          if (tnpc->player==playerlist[tpl]){ //send message to OUR OWN PARTY MEMBER
            if (playerlist[tpl]->party[playerlist[tpl]->selected_partymember]!=myobj) goto talk_skipprivatemessage; //to SELECTED PARTY MEMBER
            goto talk_privatemessage;
          }

          if (tnpc->player->party[0]){
            if ((npc*)tnpc->player->party[0]->more==tnpc){ //party leader
              //send a SPECIAL sfx buffer to a single player (can be tested by clicking on oneself)
talk_privatemessage:

              static unsigned char me_message;
              me_message=0;
              txtset(t2,"/ME ");
              txtset(t3,t); txtucase(t3); txtleft(t3,4);
              if (txtsame(t3,t2)){
                txtright(t,t->l-4);
                me_message=1;
              }

              if (t->l){
                tnpc2=(npc*)playerlist[tpl]->party[0]->more;
                txtset(t4,tnpc2->name);
                txtset(t3,"");

                if (me_message){
                  txtaddcolouredname(t3,t4,playerlist[tpl]);
                  txtadd(t3," ");
                  txtadd(t3,t);
                }else{
                  txtadd(t4,":");
                  txtaddcolouredname(t3,t4,playerlist[tpl]);
                  txtadd(t3," \x022");
                  txtadd(t3,t);
                  txtadd(t3,"\x022");
                }


                //txtadd(t3,": \x022");
                //txtadd(t3,t);
                //txtadd(t3,"\x022");
                txtset(t,"?"); t->d2[0]=2;
                txtset(t2,"??"); t2->ds[0]=1; txtadd(t,t2);
                t2->ds[0]=3; txtadd(t,t2);
                t2->ds[0]=2048; txtadd(t,t2);
                t2->ds[0]=1023; txtadd(t,t2);
                t2->ds[0]=t3->l; txtadd(t,t2);
                t2->ds[0]=0; txtadd(t,t2); //unused
                t2->ds[0]=tnpc2->port; txtadd(t,t2); //more=port
                txtset(t2,"????"); t2->df[0]=2; txtadd(t,t2); //wait
                txtadd(t,t3);
                NET_send(NETplayer,tnpc->player->net,t);
                if (tnpc->player==playerlist[tpl]){
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,"Talking to yourself?"); NET_send(NETplayer,playerlist[tpl]->net,t);
                }else{
                  NET_send(NETplayer,playerlist[tpl]->net,t);
                }
              }//t->l
              goto doneclmess;
            }
          }
        }
talk_skipprivatemessage:

        if ((abs(myobj->x-playerlist[tpl]->party[0]->x)>8)||(abs(myobj->y-playerlist[tpl]->party[0]->y)>8)){
          txtset(t,"?"); t->d2[0]=8; txtadd(t,"Out of range!"); NET_send(NETplayer,playerlist[tpl]->net,t);
          goto doneclmess;
        }

        /*
        if (OBJcheckbolt(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj->x,myobj->y)){
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Blocked!"); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
        }
        */


        x4=1; //bedswap2
        if (tnpc->schedule){ if (tnpc->schedule_i!=-1){ if (tnpc->schedule[tnpc->schedule_i].type==0x91){ x4=0;}}}

        if ((tnpc->converse!=120)&&(tnpc->converse!=121)&&(tnpc->converse!=122)&&(tnpc->converse!=123)){//allow talking to sick warriors in Cove
          if (x4==0){
            txtset(t,"?"); t->d2[0]=8; txtadd(t,"No response!");
            NET_send(NETplayer,playerlist[tpl]->net,t);
            goto doneclmess;
          }//No response!
        }

        //turn and face talker and begin "listening!"
        if (myobj!=playerlist[tpl]->party[0]) OBJdir(myobj,playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y);

        if (tnpc->player==NULL){ //not a party member!
          if (tnpc->wait_walk<16.0f) tnpc->wait_walk=16.0f;
        }

        CONnpc2=tnpc; //only valid if #converse is derived from an NPC pointer!

npcmess_shrine1:
        tplayer=playerlist[tpl];

        //set as talk target!
        VLNKremove(tplayer->talk_target); VLNKsremove(tplayer->talk_target);
        tplayer->talk_target->more=myobj;
        VLNKnew(tplayer->talk_target,myobj,(unsigned long)&tplayer->talk_target->more);


        //RESET temp flags
        ZeroMemory(&CONreg,sizeof(CONreg));
        CONerr=0;
        CONnumber=0;
        CONqual=0xFFFFFFFF;
        CONport=0xFFFFFFFF;
        CONhousecost=0;
        CONhouseinitialcost=0;
        CONnumber=txtnum(t); //NUMBER
        if (CONnumber<0) CONnumber=0;
        if (CONnumber>32767) CONnumber=32767;


        CONpartymember=0;
        for (x2=0;x2<=7;x2++){
          if (tplayer->party[x2]){
            txtset(t2,t); txtucase(t2);
            tnpc3=(npc*)tplayer->party[x2]->more;
            txtset(t3,tnpc3->name); txtucase(t3);
            if (txtsame(t2,t3)){
              CONpartymember=x2+1; //(1-8)
            }
          }
        }



        txtset(t9,t);

        if (t->l==0) txtset(t,"look"); //default

        //remove strange characters from t!
npccon_cull:
        for (x2=0;x2<t->l;x2++){
          x3=t->d2[x2];
          if( (x3<48) || ((x3>57)&&(x3<65)) || ((x3>90)&&(x3<97)) || (x3>122) ){
            txtset(t2,t);
            txtleft(t,x2);
            txtright(t2,t2->l-x2-1);
            txtadd(t,t2);
            goto npccon_cull;
          }
        }

        txtlcase(t);

        //NPC converse interpreter
        //<-NPC's object(myobj) + <-*player(tplayer)






        //if (t->l<=2){
        //txtset(t,"unknown");
        //}
        //next instruction

        static bool lookchk=FALSE;
        lookchk=FALSE;

        if (npcmess_shrine) {x9=npcmess_shrine; goto npcmess_shrine2;}

        tnpc=(npc*)myobj->more;
        x9=tnpc->converse;
        if (x9==255) x9=0; //*Dupre

npcmess_shrine2:

        CONnpc=x9;
        x=npci[CONnpc]; //first instruction index

        //before checking first instruction, check for in-party commands

        if (!npcmess_shrine){
          if (tnpc->player==tplayer){ //partymember
            /*
            i3=0;
            txtset(t2,"dont"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"stop"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"not"); if (txtsearch(t,t2)) i3=1;
            if (i3){

            txtset(t2,"poti");
            if (txtsearch(t,t2)){
            txtset(t2,"yell");
            if (txtsearch(t,t2)){
            if (tnpc->command&1) tnpc->command-=1;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't use yellow potions.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"red");
            if (txtsearch(t,t2)){
            if (tnpc->command&2) tnpc->command-=2;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't use red potions.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"poti"

            txtset(t2,"eat");
            if (txtsearch(t,t2)){
            if (tnpc->command&4) tnpc->command-=4;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I'll refrain from eating.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }

            txtset(t2,"cast");
            if (txtsearch(t,t2)){
            txtset(t2,"cure");
            if (txtsearch(t,t2)){
            if (tnpc->command&32) tnpc->command-=32;
            if (tnpc->command&64) tnpc->command-=64;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't cast cure.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"heal");
            if (txtsearch(t,t2)){
            if (tnpc->command&8) tnpc->command-=8;
            if (tnpc->command&16) tnpc->command-=16;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't cast heal spells.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"cast"

            }//negative

            txtset(t2,"poti");
            if (txtsearch(t,t2)){
            txtset(t2,"yell");
            if (txtsearch(t,t2)){
            tnpc->command|=1;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I shall use yellow potions if heavily wounded.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"red");
            if (txtsearch(t,t2)){
            tnpc->command|=2;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I shall use red potions if poisoned.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"poti"

            txtset(t2,"eat");
            if (txtsearch(t,t2)){
            tnpc->command|=4;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I shall eat or drink when hungry.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }

            txtset(t2,"cast");
            if (txtsearch(t,t2)){
            i3=0;
            txtset(t2,"all"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"ever"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"part"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"cure");
            if (txtsearch(t,t2)){
            if (!i3) tnpc->command|=32; else tnpc->command|=64;
            static txt *txttmp0;
            txttmp0=txtnew();
            if (!i3) txtset(txttmp0,"\x022I shall cast cure if poisoned.\x022"); else txtset(txttmp0,"\x022I shall cast cure if anyone in our party is poisoned.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"heal");
            if (txtsearch(t,t2)){
            if (!i3) tnpc->command|=8; else tnpc->command|=16;
            static txt *txttmp0;
            txttmp0=txtnew();
            if (!i3) txtset(txttmp0,"\x022I shall cast heal spells if heavily wounded.\x022"); else txtset(txttmp0,"\x022I shall cast heal spells if anyone in our party is heavily wounded.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"cast"

            */

            if (tnpc->converse==NULL) goto talk_skipnullobject;
          }//tplayer
        }//!npcmess_shrine (->npc)

nextinstruction:



        if (npcbin[x].l1==1){ //if=keyword goto (ifkey)
          //long txtsearch(txt*t,txt*t2);

          txtNEWLEN(t2,npcbin[x].l3);
          memcpy(t2->d2,&npcinf[npcbin[x].l2],npcbin[x].l3);


ifkey_next:
          txtset(t4,t2);
          txtset(t3,",");
          if (x2=txtsearch(t2,t3)){
            txtleft(t4,x2-1);
            txtright(t2,t2->l-x2);
          }else{
            txtset(t2,"");
          }
          //txtset(t3," "); txtadd(t3,t4); txtset(t4,t3); //add a space before the keyword
          //txtset(t5," "); txtadd(t5,t);
          if (txtsearch(t,t4)){ //t2=keyword string
            txtset(t3,"look"); if (txtsearch(t4,t3)) lookchk=TRUE;
            x++; goto nextinstruction;
          }
          if (t2->l) goto ifkey_next;

          //it is possible t2 is a truncated version of t if it is 4 characters long
          //if (t2->l==4){ //short keyword
          //txtset(t3,t); if (t3->l>4) txtleft(t3,4);
          //if (txtsearch(t2,t3)){
          //x++; goto nextinstruction;
          //}
          //}






          x=npcbin[x].l4; goto nextinstruction;
        }



        if ((npcbin[x].l1==2)||(npcbin[x].l1==22)){ //display txt and exit || display txt and DONT exit
          i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
          sf[i3].type=3; //message to user
          sf[i3].x=myobj->x;
          sf[i3].y=myobj->y-2;
          sf[i3].wait=5;
          tnpc=(npc*)myobj->more;
          if (npcmess_shrine==0){
            //GPFGPF
            sf[i3].more=tnpc->port; //portrait upper byte may be used to store more information (or it could be stored in the message)
            //addnpcport2sf(i3,tnpc);
          }else{
            sf[i3].more=187; //blank!
            if (npcmess_shrine==199) sf[i3].more=191; //wisp
            if (npcmess_shrine==179) sf[i3].more=180; //man in stocks
            if (npcmess_shrine==200) sf[i3].more=192; //guard
          }

          if (CONport!=0xFFFFFFFF) sf[i3].more=CONport;
          CONport=0xFFFFFFFF;
          static txt* inouttxt2;
          inouttxt2=txtnew();
          txtNEWLEN(inouttxt2,npcbin[x].l3);
          memcpy(inouttxt2->d2,&npcinf[npcbin[x].l2],npcbin[x].l3);

          if (lookchk){
            txtset(t2,"look");
            if (txtsame(t,t2)){
              txtset(t2,"You see ");
              txtadd(t2,inouttxt2);
              txtset(inouttxt2,t2);
              lookchk=0;
            }
          }

          txtset(t2,"ambidextrous!"); //ambidextrous (FIX: M'sieur Loubet training +1 DEX)
          if (txtsearch(inouttxt2,t2)){
            tnpc2=(npc*)tplayer->party[0]->more;
            tnpc2->d++;
            tnpc2->upflags|=1;
          }

insert_txt:

          /*
          if ((tnpc->i>=tnpc->s)&&(tnpc->i>=tnpc->d)) myobj->type=378; //i
          if ((tnpc->d>=tnpc->s)&&(tnpc->d>=tnpc->i)) myobj->type=386; //d
          if ((tnpc->s>=tnpc->d)&&(tnpc->s>=tnpc->i)) myobj->type=376; //s
          if ((tnpc->i>tnpc->s)&&(tnpc->d>tnpc->s))  myobj->type=377; //i+d

          txtset(t2,"$NAME"); //$NAME
          if (x2=txtsearch(inouttxt2,t2)){
          txtset(t2,inouttxt2); txtleft(t2,x2-1);
          txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-2);

          //txtnumint(t4,CONreg[0]); txtadd(t2,t4);
          txtadd(t2,


          txtadd(t2,t3); txtset(inouttxt2,t2);
          goto insert_txt;
          }
          */
          //CONnpc=tnpc; //only valid if #converse is derived from an NPC pointer!


          txtset(t2,"$RATE"); //$RATE
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-4);
            //assume +3 per level, anything above min threshold costs double!!!
            x2=37+3*CONnpc2->lev;
            if ((CONnpc2->s+CONnpc2->d+CONnpc2->i)>x2) txtadd(t2,"24"); else txtadd(t2,"8");
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$SDI"); //$SDI
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-3);
            txtadd(t2,"(str:");
            txtnumint(t4,CONnpc2->s); txtadd(t2,t4);
            txtadd(t2,", dex:");
            txtnumint(t4,CONnpc2->d); txtadd(t2,t4);
            txtadd(t2,", int:");
            txtnumint(t4,CONnpc2->i); txtadd(t2,t4);
            txtadd(t2,")");
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$CLASS"); //$CLASS
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-5);
            txtset(t4,"ranger");
            if ((CONnpc2->i>CONnpc2->s)&&(CONnpc2->i>CONnpc2->d)) {txtset(t4,"mage"); goto gotclass;}
            if ((CONnpc2->d>CONnpc2->s)&&(CONnpc2->d>CONnpc2->i)) {txtset(t4,"bard"); goto gotclass;}
            if ((CONnpc2->s>CONnpc2->d)&&(CONnpc2->s>CONnpc2->i)) {txtset(t4,"fighter"); goto gotclass;}
            if ((CONnpc2->s>CONnpc2->d)&&(CONnpc2->i>CONnpc2->d)) {txtset(t4,"paladin"); goto gotclass;}
            if ((CONnpc2->s>CONnpc2->i)&&(CONnpc2->d>CONnpc2->i)) {txtset(t4,"tinker"); goto gotclass;}
            if ((CONnpc2->d>CONnpc2->s)&&(CONnpc2->i>CONnpc2->s)) {txtset(t4,"druid"); goto gotclass;}
            x2=CONnpc2->s+CONnpc2->d+CONnpc2->i;
            if (x2<=52) txtset(t4,"shepard");
gotclass:
            /*
            Honesty (Truth - Mage) i3
            Compassion (Love - Bard)  d3
            Valor (Courage - Fighter) s3
            Justice (Truth and Love - Druid) s1 i1
            Sacrifice (Love and Courage - Tinker) s1 d1
            Honor (Truth and Courage - Paladin) d1 i1
            Spirituality (Truth and Love and Courage - Ranger) s1 d1 i1
            Humility (None - Shepherd) N/A

            */
            txtadd(t2,t4); txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }



          txtset(t2,"$NAME"); //$NAME
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-4);
            txtadd(t2,CONnpc2->name); txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$HIRE"); //$HIRE
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-4);
            txtadd(t2,"24"); txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$V1"); //$V1
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-2);
            txtnumint(t4,CONreg[1]); txtadd(t2,t4); txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$V0"); //$V0
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-2);
            txtnumint(t4,CONreg[0]); txtadd(t2,t4); txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$PARTYMEMBER"); //$PARTYMEMBER party member's name
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-11);
            tnpc2=(npc*)tplayer->party[CONpartymember-1]->more;
            txtadd(t2,tnpc2->name);
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$P"); //$P name
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-1);
            tnpc2=(npc*)tplayer->party[0]->more;
            txtadd(t2,tnpc2->name);
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$Y"); //$Y name (originally of talking party member)
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-1);
            tnpc2=(npc*)tplayer->party[0]->more;
            txtadd(t2,tnpc2->name);
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$G"); //$G
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-1);
            if (tplayer->male_female) txtadd(t2,"milady"); else txtadd(t2,"milord");
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$T"); //$T morning afternoon evening
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-1);
            if (btime<12) txtadd(t2,"morning");
            if ((btime>=12)&&(btime<=18)) txtadd(t2,"afternoon");
            if (btime>18) txtadd(t2,"evening");
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }

          txtset(t2,"$Z"); //$Z
          if (x2=txtsearch(inouttxt2,t2)){
            txtset(t2,inouttxt2); txtleft(t2,x2-1);
            txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-1);
            txtadd(t2,t9);
            txtadd(t2,t3);
            txtset(inouttxt2,t2);
            goto insert_txt;
          }
          sf[i3].p=inouttxt2;

          if (npcbin[x].l1==22) {x++; goto nextinstruction;}
        }

        if (npcbin[x].l1==3){ //jmp
          x=npcbin[x].l4; goto nextinstruction;
        }

        if (npcbin[x].l1==4){ //=
          CON_sv(npcbin[x].l2,npcbin[x].l3);
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==5){ //if=
          x3=CON_gv(npcbin[x].l3);
          x2=CON_gv(npcbin[x].l2);
          if (x2==x3){ //=
            x++; goto nextinstruction;
          }
          x=npcbin[x].l4; goto nextinstruction;
        }

        if (npcbin[x].l1==6){ //item
          x2=CON_gv(npcbin[x].l2); //type
          x3=CON_gv(npcbin[x].l3); //info/qty

          myobj4=myobj; //backup!
          tnpc2=(npc*)tplayer->party[0]->more;
          myobj3=OBJnew(); myobj3->type=x2; myobj3->more2=x3;
          if ((tnpc2->wt+WTfind(myobj3))<=tnpc2->wt_max){
            additemroot(tnpc2,myobj3);
          } //weight
          tnpc2->upflags|=32; //inv
          myobj=myobj4; //<-backup
          x++; goto nextinstruction;
        }


        //buy/give
        if (npcbin[x].l1==7){ //buy
          x2=CON_gv(npcbin[x].l2); //type
          x3=CON_gv(npcbin[x].l3); //number of items (1-?) *0 is invalid
          x4=CON_gv(npcbin[x].l4); //gold



          myobj4=myobj; //backup!
          tnpc2=(npc*)tplayer->party[0]->more;

          //check gold
          for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
            tnpc3=(npc*)tplayer->party[x6]->more;
            x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
          }} //!=NULL,x6
          OBJlist_last=NULL;
          y7=0; y8=0;
          for(y6=0;y6<x5;y6++){
            if (OBJlist_list[y6]->type==88){ //gold
              if (y7<x4){
                if (OBJlist_list[y6]->info&128) y8=1;
              }
              y7+=OBJlist_list[y6]->more2;
            }
          }
          if (y7<x4) {CONerr=1; goto buy_nogold;} //not enough gold

          if (CONqual!=0xFFFFFFFF){
            if (CONqual&16384){ //ignore stolen gold!
              CONqual-=16384;
              if (y8) {CONerr=1; goto buy_nogold;} //not enough gold
            }
          }

          if (x2==0) goto buy_removegold;
buy_newitem:

          myobj3=OBJnew(); myobj3->type=x2;

          if (obji[sprlnk[x2&1023]+(x2>>10)].flags&4096) myobj3->more2=1; //temp =1 to find weight of single object
          f=WTfind(myobj3)*x3;
          myobj3->more2=0; //*can override

          if (CONqual!=0xFFFFFFFF){
            if (CONqual&32768) {CONqual-=32768; myobj3->info|=256;} //quest
            myobj3->more2=CONqual;
          }


          if (obji[sprlnk[x2&1023]+(x2>>10)].flags&4096){
            myobj3->more2=x3;
            x3=1;
          }

          //x3 now equals number of objects to allocate

          if (myobj3->info&256) goto questitem_skipweightcheck2;
          if ((tnpc2->wt+f)<=tnpc2->wt_max){
questitem_skipweightcheck2:
            if (y8) myobj3->info|=128;
            stealing(tplayer,myobj3);

            ENHANCEnewn(myobj3,8,8);

            myobj6=myobj3;
            if (myobj6->type==76){
              if (!tnpc2->neck) goto amuletofsubmission0;
              myobj3=tnpc2->neck;
            }
            additemroot(tnpc2,myobj3);
            x3--;
            if (x3) goto buy_newitem;

            if (myobj6->type==76){
amuletofsubmission0:
              tnpc2->neck=myobj6;
              tnpc2->upflags|=64;
            }

            //buy horse papers
            if (myobj3->type==448){
              if (CONnpc==25) movernew(430,280,441,MOVERNEW_FLAG_NORESPAWN);
              if (CONnpc==78) movernew(430,412,802,MOVERNEW_FLAG_NORESPAWN);
              ((creature*)MOVERNEW_OBJECT->more)->flags|=128;
              ((creature*)MOVERNEW_OBJECT->more)->al=0;
              static long horseval;
              //locate tpl index
              ((creature*)MOVERNEW_OBJECT->more)->respawn_y=0xFFFF;
              for (horseval=0;horseval<=playerlist_last;horseval++){ if (playerlist[horseval]){
                if (playerlist[horseval]==tplayer){
                  ((creature*)MOVERNEW_OBJECT->more)->respawn_y=horseval;
                  goto buyhorse_foundtpl;
                }
              }}
buyhorse_foundtpl:
              //set initial horse status (races won, food, horseshoes)
              horseval=0;
              if (((long)(rnd*2))==0){
buyhorseaddracewon:
                if (horseval<15){
                  if (((long)(rnd*2))==0) {horseval++; goto buyhorseaddracewon;}
                }
              }
              if (CONnpc==25) ((creature*)MOVERNEW_OBJECT->more)->respawn_x=horseval+(2<<4)+(1<<6);
              if (CONnpc==78) ((creature*)MOVERNEW_OBJECT->more)->respawn_x=horseval+(1<<4)+(0<<6);

              myobj3->more=MOVERNEW_OBJECT;
              //the fact that this is a player's horse needs to be set somewhere...
            }//buy horse papers

            //buy skiff/ship?
            if ((myobj3->type&1023)==149){

              if (CONqual==0){
                myobj5=OBJnew(); myobj5->type=412+9*1024;
                if (CONnpc==26) OBJadd(316,441,myobj5); //arty
                if (CONnpc==119) OBJadd(592,622,myobj5); //fentrissa
                if (CONnpc==51) OBJadd(164,872,myobj5); //peer
                if (CONnpc==70) OBJadd(603,119,myobj5); //trebor

                static mlobj *mmyobj; //array size varies
                mmyobj=(mlobj*)malloc(sizeof(object*)*5);
                myobj5->more=mmyobj;
                myobj=OBJnew(); mmyobj->obj[0]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
                myobj=OBJnew(); mmyobj->obj[1]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
                myobj=OBJnew(); mmyobj->obj[2]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
                myobj=OBJnew(); mmyobj->obj[3]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
                myobj=OBJnew(); mmyobj->obj[4]=myobj; myobj->info|=8; myobj->type=myobj5->type; myobj->more=(object*)myobj5; OBJadd(myobj5->x,myobj5->y,myobj);
                myobj3->more=myobj5; //deed->ship
                OBJmove_allow=TRUE;
                OBJmove2(myobj5,myobj5->x,myobj5->y+1);
                OBJmove2(myobj5,myobj5->x,myobj5->y-1);
                OBJmove_allow=FALSE;
              }//CONqual

              if (CONqual==1){
                myobj5=OBJnew(); myobj5->type=414;
                if (CONnpc==26) OBJadd(316,441,myobj5); //arty
                if (CONnpc==119) OBJadd(592,622,myobj5); //fentrissa
                if (CONnpc==51) OBJadd(164,872,myobj5); //peer
                if (CONnpc==70) OBJadd(603,119,myobj5); //trebor
                myobj3->info|=(1<<9);
                myobj3->more=myobj5; //deed->skiff
              }//CONqual

            }//149

            tnpc2->upflags|=32; //inv

buy_removegold:

            //remove gold!
            //recreate OBJlist
            for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
              tnpc3=(npc*)tplayer->party[x6]->more;
              x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
            }} //!=NULL,x6
            OBJlist_last=NULL;

            for(y6=0;y6<x5;y6++){
              if (OBJlist_list[y6]->type==88){ //gold
                myobj5=OBJlist_list[y6];
                x7=myobj5->more2; //25
                x7-=x4; //25-10=10
                x4-=myobj5->more2; //=-15

                //txtnumint(t5,x4);
                //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

                if (x7<=0){
                  OBJremove(myobj5);
                  OBJrelease(myobj5);
                }else{
                  myobj5->more2=x7;
                }
                if (x4<=0) goto buy_gotgold;
              }
            }
buy_gotgold:

            //update ALL! party inventory windows
            for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
              tnpc3=(npc*)tplayer->party[x6]->more;
              tnpc3->upflags|=32; //inv
            }} //!=NULL,x6



          }else{CONerr=2;} //weight


buy_nogold:
          CONqual=0xFFFFFFFF; //reset
          myobj=myobj4; //<-backup
          x++; goto nextinstruction;
        }


        if (npcbin[x].l1==8){ //if!=
          x3=CON_gv(npcbin[x].l3);
          x2=CON_gv(npcbin[x].l2);
          if (x2!=x3){ //=
            x++; goto nextinstruction;
          }
          x=npcbin[x].l4; goto nextinstruction;
        }

        if (npcbin[x].l1==9){ //+
          CON_sv(npcbin[x].l2,CON_gv(npcbin[x].l2)+CON_gv(npcbin[x].l3));
          x++; goto nextinstruction;
        }
        if (npcbin[x].l1==10){ //-
          CON_sv(npcbin[x].l2,CON_gv(npcbin[x].l2)-CON_gv(npcbin[x].l3));
          x++; goto nextinstruction;
        }
        if (npcbin[x].l1==11){ //*
          CON_sv(npcbin[x].l2,CON_gv(npcbin[x].l2)*CON_gv(npcbin[x].l3));
          x++; goto nextinstruction;
        }
        if (npcbin[x].l1==12){ //div /
          CON_sv(npcbin[x].l2,CON_gv(npcbin[x].l2)/CON_gv(npcbin[x].l3));
          x++; goto nextinstruction;
        }


        if (npcbin[x].l1==13){ //ifitem
          tnpc2=(npc*)tplayer->party[0]->more;
          for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
            tnpc3=(npc*)tplayer->party[x6]->more;
            x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
          }} //!=NULL,x6
          OBJlist_last=NULL;
          y7=0; //count
          for(y6=0;y6<x5;y6++){

            if (CON_gv(npcbin[x].l2)==185){
              if ((OBJlist_list[y6]->type&1023)==185) goto ifitem_clothtype;
            }
            if (OBJlist_list[y6]->type==CON_gv(npcbin[x].l2)){
ifitem_clothtype:
              if ((CONqual==0xFFFFFFFF)||(CONqual==OBJlist_list[y6]->more2)){
                if ((OBJlist_list[y6]->info&32768)==0){//skip if not for sale
                  if (obji[sprlnk[OBJlist_list[y6]->type&1023]+(OBJlist_list[y6]->type>>10)].flags&4096) y7+=OBJlist_list[y6]->more2; else y7++;
                }
              }//qual
            }
          }
          CONqual=0xFFFFFFFF; //reset
          if (y7>=CON_gv(npcbin[x].l3)){ //=
            x++; goto nextinstruction;
          }
          x=npcbin[x].l4; goto nextinstruction;
        }



        if (npcbin[x].l1==14){ //sell
          x2=CON_gv(npcbin[x].l2); //type
          x3=CON_gv(npcbin[x].l3); //info/qty
          if (x3==0) x3=1;
          x4=CON_gv(npcbin[x].l4); //gold


          myobj4=myobj; //backup!
          tnpc2=(npc*)tplayer->party[0]->more;
          for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
            tnpc3=(npc*)tplayer->party[x6]->more;
            x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
          }} //!=NULL,x6
          OBJlist_last=NULL;
          y7=0; //quantity
          for(y6=0;y6<x5;y6++){
            if (x2==185){
              if ((OBJlist_list[y6]->type&1023)==185) goto sell_clothtype;
            }
            if (OBJlist_list[y6]->type==x2){ //has item?
sell_clothtype:
              if ((CONqual==0xFFFFFFFF)||(CONqual==OBJlist_list[y6]->more2)){
                if ((OBJlist_list[y6]->info&32768)==0){//skip if not for sale
                  if (obji[sprlnk[OBJlist_list[y6]->type&1023]+(OBJlist_list[y6]->type>>10)].flags&4096) y7+=OBJlist_list[y6]->more2; else y7++;
                }
              }//qual
            }
          }
          if (y7<x3) {CONerr=1; goto sell_noitem;}

          //remove item!
          y8=0;

          for(y6=0;y6<x5;y6++){
            if (x2==185){
              if ((OBJlist_list[y6]->type&1023)==185) goto sell_clothtype2;
            }
            if (OBJlist_list[y6]->type==x2){ //item->type
sell_clothtype2:
              if ((CONqual==0xFFFFFFFF)||(CONqual==OBJlist_list[y6]->more2)){
                if ((OBJlist_list[y6]->info&32768)==0){//skip if not for sale
                  myobj5=OBJlist_list[y6];
                  if (myobj5->info&128) y8=1; //stealing!
                  if (obji[sprlnk[OBJlist_list[y6]->type&1023]+(OBJlist_list[y6]->type>>10)].flags&4096) x8=OBJlist_list[y6]->more2; else x8=1;
                  x7=x8;
                  x7-=x3;
                  x3-=x8;
                  if (x7<=0){
                    OBJremove(myobj5);
                    OBJrelease(myobj5);
                  }else{
                    myobj5->more2=x7;
                  }
                  if (x3<=0) goto sell_gotitem;
                }//not for sale
              }//qual
            }//type
          }//y6
sell_gotitem:

          if (x4){
            myobj3=OBJnew(); myobj3->type=88; myobj3->more2=x4; //gold
            if (y8) myobj3->info|=128; //stealing!
            stealing(tplayer,myobj3);
            additemroot(tnpc2,myobj3);
          }//x4

          tnpc2->upflags|=32; //inv


          //update ALL party inventory windows
          for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
            tnpc3=(npc*)tplayer->party[x6]->more;
            tnpc3->upflags|=32; //inv
          }} //!=NULL,x6

sell_noitem:
          CONqual=0xFFFFFFFF; //reset
          myobj=myobj4; //<-backup
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==15){ //if<
          x3=CON_gv(npcbin[x].l3); x2=CON_gv(npcbin[x].l2);
          if (x2<x3){ x++; goto nextinstruction; }
          x=npcbin[x].l4; goto nextinstruction;
        }
        if (npcbin[x].l1==16){ //if>
          x3=CON_gv(npcbin[x].l3); x2=CON_gv(npcbin[x].l2);
          if (x2>x3){ x++; goto nextinstruction; }
          x=npcbin[x].l4; goto nextinstruction;
        }
        if (npcbin[x].l1==17){ //if<=
          x3=CON_gv(npcbin[x].l3); x2=CON_gv(npcbin[x].l2);
          if (x2<=x3){ x++; goto nextinstruction; }
          x=npcbin[x].l4; goto nextinstruction;
        }
        if (npcbin[x].l1==18){ //if>=
          x3=CON_gv(npcbin[x].l3); x2=CON_gv(npcbin[x].l2);
          if (x2>=x3){ x++; goto nextinstruction; }
          x=npcbin[x].l4; goto nextinstruction;
        }

        //join error messages
        //1 already in party
        //2 party is full
        //3 in different party

        //leave errors
        //1 not in party!
        //2 not near home

        if (npcbin[x].l1==19){ //join!
          if (tnpc->player){
            if (tplayer==tnpc->player){CONerr=1; goto joined;} //(1)already in party
            CONerr=3; goto joined; //(3)in different party
          }

          for (x2=1;x2<=7;x2++){
            if (tplayer->party[x2]==NULL){
              //check no dead player is assigned to this slot
              for (x3=0;x3<=nresu;x3++){
                if (resu[x3]){
                  if (resu_player[x3]==tplayer){
                    if (resu_partymember[x3]==x2) goto joinblocked_indexused;
                  }}}
              tplayer->party[x2]=myobj;
              tnpc->player=tplayer;
              tnpc->order=1;
              tnpc->follow=tplayer->party[0];
              tnpc->upflags|=1; //all
              tnpc->wait_walk=0;
              myobj->info&=(32655); //remove temp flag 32767-112
              for (i=0;i<=HIRELINGS_MAX -1;i++){
                if (hirl_obj[i]==myobj) {
                  hirl_obj[i]=0; //if in hireling list remove
                }
              }
              goto joined;
            }
joinblocked_indexused:;
          }
          CONerr=2; //party is full
joined:;
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==27){ //leave!
          if (tnpc->player!=tplayer) {CONerr=1; goto leavefinished;} //(1)not in party
          for (x2=0;x2<=7;x2++){
            if (tplayer->party[x2]){
              if ((npc*)tplayer->party[x2]->more==tnpc){

                tnpc->pathn=0; //NULL autopath
                for (x3=0;x3<=31;x3++){
                  if (tnpc->schedule[x3].hour!=255){

                    //x4=pathfind(tnpc->path,tplayer->party[x2]->x,tplayer->party[x2]->y,tnpc->schedule[x3].x,tnpc->schedule[x3].y,NULL);
                    //if ((x4>0)&&(x4<=64)) goto leavepathok;
                    WPF_NEXTTO=1; WPF_OBJECT=tplayer->party[x2]; wpf_pathfind(tnpc->path,tplayer->party[x2]->x,tplayer->party[x2]->y,tnpc->schedule[x3].x,tnpc->schedule[x3].y,64,0,4);
                    if (WPF_RETURN==WPF_PATHFOUND) goto leavepathok;

                  }//!=255
                }//x3
                CONerr=2; goto leavefinished; //(2)not near home
leavepathok:

                horsedismount(tplayer->party[x2]);

leaveitems:
                if (myobj3=(object*)tnpc->items->more){
                  OBJremove(myobj3);
                  OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);
                  goto leaveitems;
                }
                if (myobj3=tnpc->helm){ tnpc->helm=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}
                if (myobj3=tnpc->neck){ tnpc->neck=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}
                if (myobj3=tnpc->wep_left){
                  if (myobj3->type==(90+1024)) myobj3->type=90;
                  tnpc->wep_left=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}
                if (myobj3=tnpc->wep_right){
                  if (myobj3->type==(90+1024)) myobj3->type=90;
                  tnpc->wep_right=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}
                if (myobj3=tnpc->armour){ tnpc->armour=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}
                if (myobj3=tnpc->ring_left){ tnpc->ring_left=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}
                if (myobj3=tnpc->ring_right){ tnpc->ring_right=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}
                if (myobj3=tnpc->boots){ tnpc->boots=NULL; OBJadd(tplayer->party[x2]->x,tplayer->party[x2]->y,myobj3);}

                tnpc->baseitem=tnpc->items; tnpc->baseitem_offset=0;

                OBJmove_allow=TRUE;
                OBJmove2(tplayer->party[x2],tplayer->party[x2]->x,tplayer->party[x2]->y+1);
                OBJmove2(tplayer->party[x2],tplayer->party[x2]->x,tplayer->party[x2]->y-1);
                OBJmove_allow=FALSE;

                //send message stating the NPC has died/left
                txtset(t5,"??"); t5->d2[0]=252; t5->d2[1]=x2; NET_send(NETplayer,tplayer->net,t5);
                if ((tnpc->converse)&&(tnpc->converse==201)) { //not core npc
                  tplayer->party[x2]->info|=112; //set as temp item
                }
                tplayer->party[x2]=NULL;
                tplayer->selected_partymember=0;

                tnpc->player=NULL;
                tnpc->order=2; //follow schedule
                tnpc->follow=NULL;

                goto leavefinished;

              }//==tnpc
            }//->party[x2]
          }//x2

leavefinished:
          x++; goto nextinstruction;
        }


        if (npcbin[x].l1==20){ //cure!
          if (CONpartymember==0){
            for (x2=0;x2<=7;x2++){
              if (tplayer->party[x2]){
                tnpc3=(npc*)tplayer->party[x2]->more;
                tnpc3->flags&=(65535-1); //remove poisoned flag
                tnpc3->upflags|=2;
              }
            }//x2
          }else{//!=0
            tnpc3=(npc*)tplayer->party[CONpartymember-1]->more;
            if ((tnpc3->flags&1)==0) CONerr=1;
            tnpc3->flags&=(65535-1); //remove poisoned flag
            tnpc3->upflags|=2;
          }
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==21){ //heal!
          if (CONpartymember==0){
            for (x2=0;x2<=7;x2++){
              if (tplayer->party[x2]){
                tnpc3=(npc*)tplayer->party[x2]->more;
                tnpc3->hp=tnpc3->hp_max;
                tnpc3->mp=tnpc3->mp_max;
                tnpc3->upflags|=(2+4);
              }
            }//x2
          }else{//!=0
            tnpc3=(npc*)tplayer->party[CONpartymember-1]->more;
            if (tnpc3->hp==tnpc3->hp_max){
              if (tnpc3->mp==tnpc3->mp_max){
                CONerr=1; //ERR_ALREADY
              }
            }
            tnpc3->hp=tnpc3->hp_max;
            tnpc3->mp=tnpc3->mp_max;
            tnpc3->upflags|=(2+4);
          }
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==23){ //ifparty
          txtNEWLEN(t2,npcbin[x].l3);
          memcpy(t2->d2,&npcinf[npcbin[x].l2],npcbin[x].l3);
          for (x2=1;x2<=7;x2++){
            if (tplayer->party[x2]){
              tnpc3=(npc*)tplayer->party[x2]->more;
              txtset(t3,tnpc3->name); txtlcase(t3);
              if (txtsame(t2,t3)){
                x++; goto nextinstruction;
              }//txtsame
            }
          }//x2
          x=npcbin[x].l4; goto nextinstruction;
        }


        if (npcbin[x].l1==24){ //levelup!
          CONerr=1;
          if (CONpartymember){ if (tplayer->party[CONpartymember-1]){ //valid value?
            tnpc3=(npc*)tplayer->party[CONpartymember-1]->more;
            //entitled to levelup?
            //x3=(800<<(tnpc3->lev-1)); //required xp to level
			// c111 new required xp to level
			if (easymodehostn1) {
				x3 = new1_getexprequired(tnpc3);
			}
			else { // original
				x3=(800<<(tnpc3->lev-1)); //required xp to level
			}
            if (tnpc3->exp>=x3){ //levelup
              tnpc3->lev++;
              tnpc3->upflags|=1;
              tnpc3->hp_max=tnpc3->lev*30;
              tnpc3->s+=npcbin[x].l2; tnpc3->d+=npcbin[x].l3; tnpc3->i+=npcbin[x].l4;
              tnpc3->wt_max=tnpc3->s*4*100;
              tnpc3->wt2_max=tnpc3->s*50;
              if (CONpartymember==1){
                tnpc3->mp_max=tnpc3->i*2;
              }else{
                tnpc3->mp_max=get_mp_max(tnpc3->i);
                //tnpc3->wt_max/=2;//halve wt_max for non-primary partymember
                tnpc3->wt_max=tnpc3->s*3*100;
              }
              CONerr=0;
              inbritanniacheck();
            }//x3
          }}
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==25){ //rest!
          if (tplayer->rest==0) tplayer->rest=32;
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==26){ //resurrect!
          //scan inventory of all players for bodies & crossreference these with ressurect lists
          CONerr=1;
          for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
            tnpc3=(npc*)tplayer->party[x6]->more;
            x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
          }} //!=NULL,x6
          OBJlist_last=NULL;
          for(y6=0;y6<x5;y6++){
            if ((OBJlist_list[y6]->type&1023)==339){ //dead body
              CONerr=2;
              //scan resurrect list
              for (i3=0;i3<=nresu;i3++){
                if (resu[i3]){
                  if (resu_body[i3]==OBJlist_list[y6]){


                    /*
                    object *resu[65536]; //object
                    object *resu_body[65536]; //dead body object
                    float resu_wait[65536]; //time until object automatically resurrected
                    player *resu_player[65536]; //player object belongs to
                    unsigned char resu_partymember[65536]; //party member index
                    long nresu=-1;
                    */
                    if (resu_player[i3]){ //has a player pointer

                      ////resu_player[i3]->party[0]=resu[i3]; //assume current player
                      resu_player[i3]->party[resu_partymember[i3]]=resu[i3];

                      //tnpc2=(npc*)resu[i3]->more; tnpc2->exp-=int(tnpc2->exp/4);

                      if (resu_partymember[i3]==0){
                        resu_player[i3]->GNPCflags[25]=0; //used insurance!

                        for (x4=1;x4<=7;x4++){
                          if (resu_player[i3]->party[x4]){
                            tnpc3=(npc*)resu_player[i3]->party[x4]->more; tnpc3->upflags=1;
                            OBJmove_allow=TRUE;
                            OBJmove2(resu_player[i3]->party[x4],tplayer->party[tplayer->selected_partymember]->x,tplayer->party[tplayer->selected_partymember]->y); //place party on the map
                            OBJmove_allow=FALSE;
                          }
                        }//x4
                      }

                      OBJmove_allow=TRUE;
                      OBJmove2(resu[i3],tplayer->party[tplayer->selected_partymember]->x,tplayer->party[tplayer->selected_partymember]->y); //place avatar on the map
                      OBJmove_allow=FALSE;

                      OBJremove(OBJlist_list[y6]); //remove body
                      tnpc3=(npc*)resu[i3]->more;
                      resu[i3]=NULL;

                      if (resu_partymember[i3]==0){
                        txtset(t5,"?"); t5->d2[0]=8; txtadd(t5,"A voice in the darkness intones, \x022"); txtadd(t5,"DOMAN THIXUS ANRETU!\x022"); NET_send(NETplayer,resu_player[i3]->net,t5);
                        txtset(t5,"?"); t5->d2[0]=8; txtadd(t5,tnpc3->name); txtadd(t5," resurrected by healer.");
                        for (x2=0;x2<=playerlist_last;x2++){ //create and send UPDATE message
                          if (playerlist[x2]){ //current player
                            if (playerlist[x2]->net!=NULL){ //net connection available
                              if (playerlist[x2]->party[0]){
                                if (playerlist[x2]!=resu_player[i3]){
                                  NET_send(NETplayer,playerlist[x2]->net,t5);
                                }}}}}
                      }
                      inbritanniacheck();
                      CONerr=0;
                      goto resurrect_success;
                    }//player
                  }//resu_body==obj
                }//resu
              }//i


            }//339
          }

resurrect_success:



          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==28){ //ifnpc
          for (y2=myobj->y-8;y2<=(myobj->y+8);y2++){ for (x2=myobj->x-8;x2<=(myobj->x+8);x2++){
            if (myobj3=OBJfindlast(x2,y2)){
              if (myobj3->info&2){
                tnpc3=(npc*)myobj3->more;
                if (tnpc3->converse==npcbin[x].l2){x++; goto nextinstruction;}
              }//2
            }//myobj3
          }}//x,y
          x=npcbin[x].l4; goto nextinstruction;
        }

        if (npcbin[x].l1==29){ //buyhouse!
          if (tplayer->party[0]){
            tnpc3=(npc*)tplayer->party[0]->more;
            //if= ERROR,1/txt He rustles through his papers.\"You must be mistaken, I'd know if that house was available."
            //if= ERROR,2/txt "$G, Lord British allows only one house per person."
            //if= ERROR,3/txt "$G, perhaps you should spend more time exploring Britannia before deciding where to live."
            x2=CON_gv(npcbin[x].l2); //housenumber
            //already owns house?
            for (x3=1;x3<=255;x3++){
              if (houseowner(tplayer,x3)){CONerr=2; goto buyhouse_failed;}
            }
            if (housesav[x2].flags&1){CONerr=1; goto buyhouse_failed;} //already owned?
			// r555 debug: player requirement check for buying house
            if (tnpc3->lev<=2){CONerr=3; goto buyhouse_failed;} //inexperienced?
            //set house username & charactername 32-byte strings
            txtNEWLEN(t3,-32);
            for (x3=0;x3<32;x3++){
              t3->d2[x3]=0; if (tplayer->name->l>x3) t3->d2[x3]=tplayer->name->d2[x3];
            }
            txtNEWLEN(t4,-32);
            for (x3=0;x3<32;x3++){
              t4->d2[x3]=0; if (tnpc3->name->l>x3) t4->d2[x3]=tnpc3->name->d2[x3];
            }
            //set house.sav buffer
            housesav[x2].flags|=1;
            housesav[x2].gold=housecost[x2]*3; //3 days
            for (x3=0;x3<32;x3++){
              housesav[x2].username[x3]=t3->d2[x3];
              housesav[x2].charactername[x3]=t4->d2[x3];
            }
            housesav_update(); //save house.sav buffer to file
            //give key (256+housenumber)
            myobj3=OBJnew(); myobj3->type=64; myobj3->info=256; myobj3->more2=256+x2;
            myobj4=tnpc3->items; myobj5=(object*)myobj4->more;
            if (myobj5){
              myobj3->next=myobj5;
              myobj5->prev=myobj3;
              myobj3->prev=myobj4;
              myobj4->more=myobj3;
            }else{
              myobj3->next=NULL;
              myobj3->prev=myobj4;
              myobj4->more=myobj3;
            }
            tnpc3->upflags|=32; //inv
            tplayer->GNPCflags[28]=x2; //set housenumber
          }//party[0]
buyhouse_failed:
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==30){//economysetup!
          x2=CON_gv(npcbin[x].l2);//index
          x3=CON_gv(npcbin[x].l3);//initial amount
          x4=CON_gv(npcbin[x].l4);//change per Britannian day (+10000 means negative)
          if (economy_setup[CONnpc][x2]==FALSE){
            economy_setup[CONnpc][x2]=TRUE;
            economy_value[CONnpc][x2]=x3;
            economy_limit[CONnpc][x2]=x3*2;//DEFAULT LIMIT IS DOUBLE INIT. VALUE
            economy_change[CONnpc][x2]=x4;
            //adjust value based on hours of already elapsed gameplay
            d=ett/3600.0f;//d=days in Brit.(realtime hours)
            x3=rnd*100.0f;
            d2=((float)x3)/100.0f*3.0f+1.0f;//1-4 (average of 2.5)
            d/=d2;//account for exponential factor
            x3=d;
            x3*=economy_change[CONnpc][x2];
            economy_value[CONnpc][x2]+=x3;
            if (economy_value[CONnpc][x2]<0) economy_value[CONnpc][x2]=0;
            if (economy_value[CONnpc][x2]>economy_limit[CONnpc][x2]) economy_value[CONnpc][x2]=economy_limit[CONnpc][x2];
          }
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==31){//economygetvalue!
          x2=CON_gv(npcbin[x].l2);//index
          CONreg[0]=economy_value[CONnpc][x2];
          x++; goto nextinstruction;
        }

        if (npcbin[x].l1==32){//economysetvalue!
          x2=CON_gv(npcbin[x].l2);//index
          x3=CON_gv(npcbin[x].l3);//new value
          economy_value[CONnpc][x2]=x3;
          if (economy_value[CONnpc][x2]<0) economy_value[CONnpc][x2]=0;
          if (economy_value[CONnpc][x2]>economy_limit[CONnpc][x2]) economy_value[CONnpc][x2]=economy_limit[CONnpc][x2];
          x++; goto nextinstruction;
        }

		// Typo? economysetlimit should be instruction #33 per NPC2BIN4.BAS, Xenkan 2010-12-04
		//if (npcbin[x].l1==32){//economysetlimit!
        if (npcbin[x].l1==33){//economysetlimit!
          x2=CON_gv(npcbin[x].l2);//index
          x3=CON_gv(npcbin[x].l3);//new limit
          economy_limit[CONnpc][x2]=x3;
          x++; goto nextinstruction;
        }
		
#pragma region
		if (npcbin[x].l1 == 34)	// bulkreset
			{
			memset(tplayer->bulktypes, 0, sizeof(tplayer->bulktypes));
			memset(tplayer->bulkqty, 0, sizeof(tplayer->bulkqty));
			tplayer->bulkprice = 0;
			x++; goto nextinstruction;
			}
		if (npcbin[x].l1 == 35)	// bulktype type,price
			{
			uint2 type, price, qty;
			type = npcbin[x].l2;
			price = npcbin[x].l3;
			qty = 0;
			
			// search party inventories for item type
			object** Inv; uint4 InvCount;
			tplayer->GetPartyInventory(&Inv, &InvCount);
			for (int i = 0; i < InvCount; i++)
				{
				if (Inv[i]->type != type ||
					Inv[i]->IsStealing() ||
					Inv[i]->IsNotForSale())
					continue;
				
				if (Inv[i]->IsStackable())
					qty += Inv[i]->GetQuantity();
				else
					qty++;
				}
			if (qty == 0)
				{ x++; goto nextinstruction; }
			
			// find next available sell slot
			int slot;
			for (slot = 0; tplayer->bulktypes[slot] != 0; slot++);
			
			// add this item type to sell list and price
			tplayer->bulktypes[slot] = type;
			tplayer->bulkqty[slot] = qty;
			tplayer->bulkprice += qty * price;
			
			x++; goto nextinstruction;
			}
		if (npcbin[x].l1 == 36)	// bulksell
			{
			// verify player still has all items included in quote
			object** Inv; uint4 InvCount;
			uint2 type, qty;
			int i, j;
			tplayer->GetPartyInventory(&Inv, &InvCount);
			for (i = 0; tplayer->bulkqty[i]; i++)
				{
				type = tplayer->bulktypes[i];
				qty = 0;
				for (j = 0; j < InvCount; j++)
					{
					if (Inv[j]->type != type ||
						Inv[j]->IsStealing() ||
						Inv[j]->IsNotForSale())
						continue;
					
					if (Inv[j]->IsStackable())
						qty += Inv[j]->GetQuantity();
					else
						qty++;
					}
				if (qty < tplayer->bulkqty[i])
					{
					CONerr = 1;	// player dropped item
					x++; goto nextinstruction;
					}
				}
			
			// remove items
			for (i = 0; tplayer->bulkqty[i]; i++)
				{
				type = tplayer->bulktypes[i];
				qty = tplayer->bulkqty[i];
				for (j = 0; j < InvCount && qty; j++)
					{
					if (Inv[j]->type != type ||
						Inv[j]->IsStealing() ||
						Inv[j]->IsNotForSale())
						continue;
					
					if (Inv[j]->IsStackable())
						{
						if (Inv[j]->GetQuantity() > qty)
							{
							Inv[j]->SetQuantity(Inv[j]->GetQuantity() - qty);
							qty = 0;
							}
						else
							{
							qty -= Inv[j]->GetQuantity();
							OBJremove(Inv[j]);
							OBJrelease(Inv[j]);
							}
						}
					else // not stackable
						{
						qty--;
						OBJremove(Inv[j]);
						OBJrelease(Inv[j]);
						}
					}
				}
			
			// give gold
			object* gold;
			gold = OBJnew();
			gold->type = OBJ_GOLD_COIN;
			gold->SetQuantity(tplayer->bulkprice);
			additemroot((npc*)tplayer->party[0]->more, gold);
			
			// update party's inventory
			npc* n;
			for (int p = 0; p < 8 && tplayer->party[p]; p++)
				{
				n = (npc*)tplayer->party[p]->more;
				n->upflags |= 32;	// update inventory
				}
			
			x++; goto nextinstruction;
			}
#pragma endregion Bulk sell patch, Xenkan 2010-12-04

        //more instructions here
      } //myobj!=NULL
talk_skipnullobject:;
      txtset(t2,t);
      //txtset(t,"NPCmessage ["); txtadd(t,t2); txtadd(t,"] ("); txtnumint(t2,ett); txtadd(t,t2); txtadd(t,")"); txtfileout(t,log2);
      goto doneclmess;
    } //7


    //theory is after receiving a message from host's client we can't reply in the same turn


    //txtset(t2,"?"); t2->d2[0]=10; //type 10: U6OID info
    //NET_send(NETplayer,NETplayer,t2);


    if (t->d2[0]==9){ //type 9: request U6OID info



      txtright(t,t->l-1);
      tu6oid=t->dl[0];


      //whose id is that anyway!!
      for (x=0;x<=playerlist_last;x++){
        if (playerlist[x]){ //current
          if (playerlist[x]->id==tu6oid){
            txtset(t2,"?"); t2->d2[0]=10; //type 10: U6OID info
            txtadd(t2,t); //U6OID
            tnpc=(npc*)playerlist[x]->party[0]->more;
            txtset(t3,"?"); t3->d2[0]=tnpc->name->l; txtadd(t2,t3); //->l
            txtadd(t2,tnpc->name);
            //colour
            txtaddchar(t2,playerlist[x]->GNPCflags[245]&255);
            txtaddchar(t2,(playerlist[x]->GNPCflags[245]>>8)&255);
            txtaddchar(t2,(playerlist[x]->GNPCflags[245]>>16)&255);
            NET_send(NETplayer,playerlist[tpl]->net,t2);
            goto doneclmess;
          }//id
        }//current
      }//x
      goto doneclmess;
    } //9


    if (t->d2[0]==13){ //->quan
      txtright(t,2);
      playerlist[tpl]->quan=t->ds[0];
      goto doneclmess;
    }//13


    if (t->d2[0]==15){ //keyboard targeting request
      //scan screen from center to outer edges
      //as each enemy is found check if they are on our list of 10
      //if ktar_wait==0 slot is empty and ready to be filled
      //remember to remove any old virtual links before assigning a new one
      if (playerlist[tpl]->ktar_display<1.0f){
        if (myobj=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
          for (i=1;i<=767;i++){
            x=myobj->x+ktar_x[i]; y=myobj->y+ktar_y[i];
            myobj2=OBJfindlast(x,y); if (myobj2==NULL) myobj2=OBJfindlastall(x,y);
            if (myobj2){
              if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
              if (myobj2->info&4){ //<-crt
                crt=(creature*)myobj2->more;
                for (i2=0;i2<=9;i2++){
                  if (playerlist[tpl]->ktar_wait[i2]){
                    if ((object*)playerlist[tpl]->ktar[i2]->more==myobj2) goto ktar_gottarget;
                  }}

                //add target if room exists
                for (i2=0;i2<=9;i2++){ if (!playerlist[tpl]->ktar_wait[i2]){
                  VLNKremove(playerlist[tpl]->ktar[i2]); VLNKsremove(playerlist[tpl]->ktar[i2]);
                  playerlist[tpl]->ktar[i2]->more=myobj2;
                  VLNKnew(playerlist[tpl]->ktar[i2],myobj2,(unsigned long)&playerlist[tpl]->ktar[i2]->more);
                  playerlist[tpl]->ktar_wait[i2]=64;
                  goto ktar_gottarget;
                }}

              }}
ktar_gottarget:;

          }//i

        }//->party[
      }//1.0f
      playerlist[tpl]->ktar_display=2.0f;
      goto doneclmess;
    }//15

    if (t->d2[0]==16){ //keyboard target
      x=t->d2[1];
      if (playerlist[tpl]->ktar_wait[x]){
        if (playerlist[tpl]->ktar[x]->more){
          //set as target
          if (playerlist[tpl]->craft){
            if ((playerlist[tpl]->party[0]->type&1023)==412){ //ship
              goto kshipattack;
            }
          }
          if (playerlist[tpl]->craft==NULL){
kshipattack:
            myobj=(object*)playerlist[tpl]->ktar[x]->more;
            crt=(creature*)myobj->more; if (crt->flags&2) goto doneclmess;
            for (x2=0;x2<=7;x2++){
              if (playerlist[tpl]->party[x2]){
                myobj2=playerlist[tpl]->party[x2];
                tnpc=(npc*)myobj2->more;
                tnpc->order=3;
                tnpc->target=myobj;
                VLNKsremove(myobj2); //remove prev vlnk
                VLNKnew(myobj2,myobj,(unsigned long)&tnpc->target); //add vlnk
                if (playerlist[tpl]->craft) goto kshipattack2; //exit loop after processing party[0]
              }
            }
          }
kshipattack2:;
        }
      }
      goto doneclmess;
    }//16

    if (t->d2[0]==17){ //break off combat
      for (x2=0;x2<=7;x2++){
        if (playerlist[tpl]->party[x2]){
          VLNKsremove(playerlist[tpl]->party[x2]);
          tnpc2=(npc*)playerlist[tpl]->party[x2]->more;
          tnpc2->target=NULL;
          tnpc2->order=3; if (x2==playerlist[tpl]->selected_partymember) tnpc2->order=0;
        }//->party
      }//x2
      goto doneclmess;
    }//17

    if (t->d2[0]==19){ //typing...
      playerlist[tpl]->typing=1;
      goto doneclmess;
    }//19

    if (t->d2[0]==20){ //typing... CANCEL
      playerlist[tpl]->typing=0;
      goto doneclmess;
    }//20


    //send pathfinding message
    if (t->d2[0]==21){//player pathfind
      if (myobj=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
        tnpc=(npc*)myobj->more;
        txtright(t,t->l-1);
        x2=t->ds[0]; y2=t->ds[1];
        WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,x2,y2,16,0,0);
        if (WPF_RETURN==WPF_PATHFOUND){
          if (WPF_PATHLENGTH){
            tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;
            playerlist[tpl]->pathfind=1; //use pathfinding formula!
            goto doneclmess;
          }
          playerlist[tpl]->pathfind=0; tnpc->path_max=0; tnpc->pathn=0;
        }
      }//party
      goto doneclmess;
    }//21


    if (t->d2[0]==25){ //play MIDI note(s)
      //broadcast instrument and notes to all in-range players instantly
      txtright(t,t->l-1);
      txtset(t2,"??"); t2->d2[0]=27; t2->d2[1]=playerlist[tpl]->instrument; txtadd(t2,t);
      if (myobj=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
        for (x=0;x<=playerlist_last;x++){
          if (playerlist[x]){ //current
            if (playerlist[x]->net!=INVALID_NET){
              if (myobj2=playerlist[x]->party[playerlist[x]->selected_partymember]){
                //proximity check
                if (abs(myobj2->x-myobj->x)<=16){ if (abs(myobj2->y-myobj->y)<=12){
                  NET_send(NETplayer,playerlist[x]->net,t2);
                }}
              }//myobj2
            }}}//x valid
      }//myobj
      goto doneclmess;
    }//25

    if (t->d2[0]==26){ //stop MIDI note(s)
      //broadcast instrument and notes to all in-range players instantly
      txtright(t,t->l-1);
      txtset(t2,"??"); t2->d2[0]=28; t2->d2[1]=playerlist[tpl]->instrument; txtadd(t2,t);
      if (myobj=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
        for (x=0;x<=playerlist_last;x++){
          if (playerlist[x]){ //current
            if (playerlist[x]->net!=INVALID_NET){
              if (myobj2=playerlist[x]->party[playerlist[x]->selected_partymember]){
                //proximity check
                if (abs(myobj2->x-myobj->x)<=16){ if (abs(myobj2->y-myobj->y)<=12){
                  NET_send(NETplayer,playerlist[x]->net,t2);
                }}
              }//myobj2
            }}}//x valid
      }//myobj
      goto doneclmess;
    }//26

    /*
    txtset(t,"?"); t->d2[0]=25; //play MIDI keys
    txtset(t2,"?");
    //multiple keys can point to a single sound
    for (i=0;i<=255;i++){
    if (keyhit(i)){
    if (t->l<9){
    t2->d2[0]=midikeyboard2[i];
    txtadd(t,t2);
    midikeyboard2_keyon[i]=1;
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
    */

    //if (playerlist[tpl]->net==1){
    //goto U6Ohostlink1;
    //U6Ohostlink1return:;
    //}

doneclmess:

    goto nextclientmessage;
  } //read client message



} //host








































































































































































































































































































































































































if (NEThost){



  //host schedule info
  f=16.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
    u6orevive++; if (u6orevive==200) u6orevive=0; 
    seek(u6orevive_fh,0); put(u6orevive_fh,&u6orevive,1);
  }

  u6opi=40;

  f=64.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
    inbritanniacheck();
  }

  u6opi=41;

  //host log report
  f=60.0f; if (int((ett/f))!=int((ett/f)-(et/f))){ //every minute
    txtset(t,"[In_Britania:"); txtadd(t,inbritannia); txtadd(t,"]");
    txtnumint(t2,framerate);
    txtadd(t,"[Framerate:"); txtadd(t,t2); txtadd(t,"]");
    txtnumint(t2,vlnkb_last);
    txtadd(t,"[vlnkb_last(MAX=65536):"); txtadd(t,t2); txtadd(t,"]");
    txtnumint(t2,objb_last);
    txtadd(t,"[objb_last(MAX=524228):"); txtadd(t,t2); txtadd(t,"]");
    LOGadd(t);
  }



  //if (ett>=86400){

  /*
  f=4.0f; if (int((ett/f))!=int((ett/f)-(et/f))){ //8min delay till removal
  txtset(t,"!");
  txtadd(t," vlnkb_last="); txtnumint(t2,vlnkb_last); txtadd(t,t2);
  txtadd(t," vlnkb_free_last="); txtnumint(t2,vlnkb_free_last); txtadd(t,t2);
  txtadd(t," framerate="); txtnumint(t2,framerate); txtadd(t,t2);
  txtadd(t," ("); txtnumint(t2,ett); txtadd(t,t2); txtadd(t,")"); txtfileout(t,log2);
  }
  */


  /*
  long objsave_last=-1;
  unsigned short objsave_x[65536];
  unsigned short objsave_y[65536];
  object *objsave_obj[65536]; //pointer to first saved object
  float objsave_wait[65536];
  */
  u6opi=42;

  //add saved objects
  for (i=0;i<=objsave_last;i++){
    if (objsave_obj[i]){
      objsave_wait[i]-=et; if (objsave_wait[i]<=0){
        //1st pass: destroy ALL temp objects (except deeds and selected critical items)
        myobj3=NULL; //add saved objects after this object
        if (myobj=od[objsave_y[i]][objsave_x[i]]){
addsaveclear:
          myobj2=(object*)myobj->next;
          if ((myobj->type&1023)==414) goto addsaveclearskip; //skiff
          if ((myobj->type&1023)==149) goto addsaveclearskip; //deed
          //add more here
          if (myobj->info&112){//temp object
            OBJremove(myobj);
          }else{
            //object wasn't removed!
            if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==NULL){
              if (obji[sprlnk[myobj->type&1023]].v4==NULL){ //not crt/npc
                myobj3=myobj; //set as base item if NOT floating
              }//not crt/npc
            }//float==NULL
          }//info&112

addsaveclearskip:
          if (myobj2) {myobj=myobj2; goto addsaveclear;}
        }

        u6opi=421;



        if (myobj3){ //temp set to end of stack
          myobj4=(object*)myobj3->next; myobj3->next=NULL;
        }else{
          myobj4=od[objsave_y[i]][objsave_x[i]]; od[objsave_y[i]][objsave_x[i]]=NULL;
        }

        //add objects
        myobj=objsave_obj[i];



addsaveobj:
        myobj2=(object*)myobj->next;

        myobj->next=NULL; myobj->prev=NULL;
        OBJadd(objsave_x[i],objsave_y[i],myobj);

        //multi-tile container FIX
        x=0; y=0;
        if (myobj->type==(176+1024)) x=-1;
        if (myobj->type==(176+3*1024)) y=-1;
        if (myobj->type==(168+1024)) x=-1;
        if (myobj->type==(168+3*1024)) y=-1;
        if (myobj->type==(341+3*1024)) {y=-1; x=-1;}
        if (myobj->type==(340+3*1024)) {y=-1; x=-1;}
        if (x||y){
addsave_mtcfix2:
          if (myobj5=od[myobj->y+y][myobj->x+x]){
addsave_mtcfix:
            if ((myobj5->type&1023)==(myobj->type&1023)){
              if (myobj5->info&8){
                myobj5->more=(object*)myobj;
              }//info&8
            }//type&1023
            if (myobj5=(object*)myobj5->next) goto addsave_mtcfix;
          }//myobj5
          if ((x==-1)&&(y==-1)&&(myobj->type==(341+3*1024))) {y=-1; x=0; goto addsave_mtcfix2;}
          if ((x==0)&&(y==-1)&&(myobj->type==(341+3*1024))) {y=0; x=-1; goto addsave_mtcfix2;}
          if ((x==-1)&&(y==-1)&&(myobj->type==(340+3*1024))) {y=-1; x=0; goto addsave_mtcfix2;}
          if ((x==0)&&(y==-1)&&(myobj->type==(340+3*1024))) {y=0; x=-1; goto addsave_mtcfix2;}
        }//x||y

        if (myobj2) {myobj=myobj2; goto addsaveobj;}
        if (myobj4) {myobj->next=myobj4; myobj4->prev=myobj;}

        OBJcheckflags(objsave_x[i],objsave_y[i]);
        objsave_obj[i]=NULL; //invalidate
      }//wait<=0
    }//objsave_obj
  }//i

  u6opi=43;
  //remove temporary objects
  f=64.0f; if (int((ett/f))!=int((ett/f)-(et/f))){ //8min delay till removal
    //f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){ //8min delay till removal

    i=2048*1024; while (i--){

      if (myobj=od[objr_y][objr_x]){
tempcheck:
        myobj2=(object*)myobj->next;




        //skip SPECIAL temp objects
        for (i2=0;i2<=nresu;i2++){ if (resu[i2]){ //if in resurrect list DO NOT remove!
          if (resu_body[i2]==myobj) goto skiptempobj;
        }}
        if ((myobj->type&1023)==412) goto skiptempobj; //ship
        if ((myobj->type&1023)==414) goto skiptempobj; //skiff
        if ((myobj->type&1023)==149) goto skiptempobj; //deed
        if (myobj->type==OBJ_HORSE_PAPERS) goto skiptempobj; //horse papers
        if (obji[sprlnk[myobj->type&1023]].flags&1024){ //container
          if ((myobj->type&1023)!=OBJ_DEAD_CYCLOPS){
            if ((myobj->type&1023)!=OBJ_DEAD_GARGOYLE){
              if ((myobj->type&1023)!=OBJ_DEAD_REAPER){
                if ((myobj->type&1023)!=OBJ_HORSE_CARCASS){//carcass
                  if (obji[sprlnk[myobj->type&1023]].weight==NULL) goto skiptempobj; //NULL weight container
                }
              }
            }
          }
        }

        //house storage exception
        for (i3=1;i3<=255;i3++){
          for (i4=0;i4<housestoragenext[i3];i4++){
            if (myobj->x==housestoragex[i3][i4]){ if (myobj->y==housestoragey[i3][i4]){
              goto skiptempobj;
            }}//x,y
          }//i4
        }//i3


        //-1/remove object
        i2=(myobj->info>>4)&7;
        if (i2){
          i2--; myobj->info&=(32767-112); myobj->info+=(i2<<4);
          if (i2==0){
            if (myobj->info&2) { //NPC
              tnpc=(npc*)myobj->more;
              free(tnpc);
            }
            OBJremove(myobj);
            OBJrelease(myobj);
          }
        }

skiptempobj:
        if (myobj2) {myobj=myobj2; goto tempcheck;}
      }
      objr_x++;
      if (objr_x>2047){objr_x=0; objr_y++;}
      if (objr_y>1023) objr_y=0;
    }

  }

  u6opi=44;
  //respawn info
  //respawn_last++;
  //respawn[respawn_last]=myobj2;
  //respawn_delay[respawn_last]=16;
  //fruit tart, dog lead?, dog home?
  /*
  void *respawn[16384];
  unsigned short respawn_delay[16384]; //number of seconds till creature will respawn
  //*note: creature will not respawn if player is too near (eg. 8 squares or less)
  long respawn_last=-1;
  */


  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){

    for (i=0;i<=respawn_last;i++){
      if (respawn[i]){
        respawn_delay[i]--;
        if (respawn_delay[i]==0){
          crt=(creature*)respawn[i]->more;

          u6opi=441;
          //player nearby?

          x=crt->respawn_x; y=crt->respawn_y; //GPF

          z=12; if ((x>=1024)&&(!((y>=256)&&(x<1280)&&(y<512)))) z=8; //not on overland map or garglands
          for (i2=0;i2<=playerlist_last;i2++){
            if (playerlist[i2]){ //current player
              if (playerlist[i2]->net!=INVALID_NET){ //net connection available
                if (playerlist[i2]->party[0]){
                  //if abs(playerlist[i2]->party[0]->x-crt->respawn_x)<=
                  x2=playerlist[i2]->party[0]->x; y2=playerlist[i2]->party[0]->y;
                  x2=abs(x-x2); y2=abs(y-y2);
                  if ((x2<=z)||(y2<=z)){
                    respawn_delay[i]=512; //~8mins
                    goto respawn_failed;
                  }//x2<=z,y2<=z
                }
              }
            }//net
          }//i2
          u6opi=442;
          if (crt->flags&32){ //revert charmed creature!
            crt->al=1; crt->charm=0;
          }

          //revert passive->aggresive
          if ((respawn[i]->type&1023)==382){ //guard
            if (crt->respawn_x<1024) crt->al=4; //not underground
          }
          if ((respawn[i]->type&1023)==373) crt->al=0; //wisp
          if ((respawn[i]->type&1023)==430) crt->al=0; //horse
          if ((respawn[i]->type&1023)==428) crt->al=0; //cow
          if ((respawn[i]->type&1023)==356) crt->al=0; //bird
          if ((respawn[i]->type&1023)==348) crt->al=0; //sheep
          if ((respawn[i]->type&1023)==350) crt->al=0; //deer
          if ((respawn[i]->type&1023)==388){ //cat
            if ((crt->respawn_x>=400)&&(crt->respawn_y>=576)&&(crt->respawn_x<=415)&&(crt->respawn_y<=591)) crt->al=0; //Dr Cat's cats
          }
          if ((respawn[i]->type&1023)==352){ //ghost
            if ((crt->respawn_x==139)&&(crt->respawn_y==196)) crt->al=0; //abby ghosts
          }

          crt->hp=crt->respawn_hp;
          crt->mp=rnd*9;
          crt->flags=0;
          myobj2=respawn[i];



          //2006 monster drops
          static object *invbag,*invobj,*xpgoldobj;
          invbag=crt->items;
          if (crt->al==1){//aggressive
            addu6monsterdropitems(myobj2);

            //integrate XP based gold
            x9=rnd*8; if (x9==0){
              x9=obji[sprlnk[myobj2->type&1023]].v5; x9=rnd*(x9+1); if (x9){
                if (xpgoldobj=(object*)invbag->more){
respawn_goldscan:
                  if (xpgoldobj->type==OBJ_GOLD_COIN){
                    xpgoldobj->more2+=x9;
                    goto respawn_goldadded;
                  }
                  xpgoldobj=(object*)xpgoldobj->next; if (xpgoldobj) goto respawn_goldscan;
                }
                invobj=OBJnew(); invobj->type=88; invobj->more2=x9;
                invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;
              }
            }
respawn_goldadded:

            goto respawn_u6inventoryadded;
          }//al==1
          if (((myobj2->type&1023)==428)||((myobj2->type&1023)==348)||((myobj2->type&1023)==350)){//cow/sheep/deer
            x9=rnd*2; if (x9){
              invobj=OBJnew(); invobj->type=129; invobj->more2=1;//add meat
              invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;
            }//x9
          }//cow/sheep/deer
respawn_u6inventoryadded:
          /*
          //add gold/items to inventory
          //add gold
          if (crt->al==1){
          x9=obji[sprlnk[myobj2->type&1023]].v5>>2;
          //x9++; //plus 1 for rookie luck
          x9=rnd*(x9+1);
          if (x9){
          myobj8=OBJnew(); myobj8->type=88; myobj8->more2=x9;
          //items<-gold
          myobj6=crt->items; myobj7=(object*)myobj6->more;
          if (myobj7!=NULL){
          myobj8->next=myobj7; myobj7->prev=myobj8; myobj8->prev=myobj6; myobj6->more=myobj8;
          }else{
          myobj8->next=NULL; myobj8->prev=myobj6; myobj6->more=myobj8;
          }
          }//x9
          }//al==1
          //add meat
          if (((myobj2->type&1023)==428)||((myobj2->type&1023)==348)||((myobj2->type&1023)==350)){//cow&sheep&deer
          x9=rnd*2; if (x9){
          myobj8=OBJnew(); myobj8->type=129; myobj8->more2=1; //<-meat
          myobj6=crt->items; myobj7=(object*)myobj6->more;
          if (myobj7!=NULL){
          myobj8->next=myobj7; myobj7->prev=myobj8; myobj8->prev=myobj6; myobj6->more=myobj8;
          }else{
          myobj8->next=NULL; myobj8->prev=myobj6; myobj6->more=myobj8;
          }
          }//x9
          }//cow
          */

          if ((myobj2->type&1023)==375) myobj2->info|=(3<<9); //slime divide!

          OBJmove_allow=TRUE;
          OBJmove2(respawn[i],crt->respawn_x,crt->respawn_y);
          //silver serpent "recoil"
          if ((respawn[i]->type&1023)==413){
            OBJmove2(respawn[i],crt->respawn_x,crt->respawn_y+1);
            OBJmove2(respawn[i],crt->respawn_x+1,crt->respawn_y+1);
            OBJmove2(respawn[i],crt->respawn_x+1,crt->respawn_y);
            OBJmove2(respawn[i],crt->respawn_x,crt->respawn_y);
            OBJmove2(respawn[i],crt->respawn_x,crt->respawn_y+1);
            OBJmove2(respawn[i],crt->respawn_x+1,crt->respawn_y+1);
            OBJmove2(respawn[i],crt->respawn_x+1,crt->respawn_y);
            OBJmove2(respawn[i],crt->respawn_x,crt->respawn_y);
          }
          OBJmove_allow=FALSE;

          crt->target=NULL;
          respawn[i]=NULL;

respawn_failed:;
        }//delay
      }//respawn[i]
    }//i
  }//f

  u6opi=5;
  //update objects/creatures/npc
  if (ouln!=-1){
    for (i=0;i<=ouln;i++){
      if (myobj=oul[i]){

        //skip objects in void!
        if ((myobj->y>=1000)&&(myobj->x>=2016)) goto skip_ouli;
        //if ((myobj->y>=256)&&(myobj->x>=1280)) goto skip_ouli; //2005: free map area


        i2=myobj->type&1023;
        //i,i2 reserved, all other free

        if (myobj->type==(223+1024)){ //powder keg
          f=0.25f; if (int((ett/f))!=int((ett/f)-(et/f))){
            x=(myobj->info>>9)&15;
            x--;
            myobj->info=(myobj->info&(0xFFFF-7680))+(x<<9);
            if (!x){

              static object *powderkeg;
              powderkeg=myobj;

              myobj=NULL; tnpc=NULL;
              for (x2=0;x2<=playerlist_last;x2++){
                if (playerlist[x2]){
                  tplayer2=playerlist[x2];
                  if (tplayer2->id==powderkeg->more2){
                    if (tplayer2->party[0]){
                      myobj=tplayer2->party[0];
                      tnpc=(npc*)myobj->more;
                    }
                    goto powderkeg_gotsource;
                  }
                }
              }
powderkeg_gotsource:

              static long spellx,spelly;

              //copied from "explosion spell"
              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  if (OBJcheckbolt(powderkeg->x,powderkeg->y,powderkeg->x+spellx,powderkeg->y+spelly)) goto explosionblocked2;

                  myobj2=OBJfindlast(powderkeg->x+spellx,powderkeg->y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(powderkeg->x+spellx,powderkeg->y+spelly);
                  if (myobj2){
                    if ((myobj2->info&8)==0){ //no redirector

                      x2=myobj2->type&1023; x3=myobj2->type>>10;

                      if ((x2>=297)&&(x2<=300)){ //door
                        if ((x3>=4)&&(x3<12)){ //closed but not magically locked
                          x4=4;
                          if (x3>=8){ //door is locked and needs unlocking
                            myobj3=OBJfindlastall(myobj2->x,myobj2->y);
                            if (myobj3->info&8) myobj3=(object*)myobj3->more; //<-redirect
                            if (myobj3->more2) goto powderkeg_needskey;
                            x4=8;
                          }
                          myobj2->type=(myobj2->type&1023)+(((myobj2->type>>10)-x4)<<10); OBJcheckflags(myobj2->x,myobj2->y);
                          myobj2=(object*)myobj2->more;
                          myobj2->type=(myobj2->type&1023)+(((myobj2->type>>10)-x4)<<10); OBJcheckflags(myobj2->x,myobj2->y);
                          if (tnpc){ if (tnpc->player){
                            txtset(t,"?The door was blown open!"); t->d2[0]=8; NET_send(NETplayer,tnpc->player->net,t);
                          }}
                          goto powderkeg_dooropened;
                        }
                      }
powderkeg_needskey:

                      if ((myobj2->type&1023)==98){ //chest
                        if ((myobj2->type>>10)<=2){ //not magically locked
                          //destroy chest and extract objects
                          x3=myobj2->x; y3=myobj2->y;
                          OBJsave(x3,y3);
                          VLNKremove(myobj2);
                          OBJremove(myobj2);
extractobj6:
                          myobj3=(object*)myobj2->more;
                          if (myobj3) {OBJremove(myobj3); myobj3->info|=112; OBJadd(x3,y3,myobj3); goto extractobj6;}
                        }
                      }//98

                      if ((myobj2->type&1023)==223){ //powder keg
                        if (myobj2!=powderkeg){
                          if ((myobj2->more2==0)||(myobj2->more2==powderkeg->more2)){
                            OBJsave(myobj2->x,myobj2->y);
                            myobj2->info=(myobj2->info&(0xFFFF-7680))+(1<<9);
                            myobj2->type=223+1*1024;
                            myobj2->more2=powderkeg->more2;
                          }
                        }
                      }

                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        x2=rnd*96;
                        if (x2==0) goto explosion_spelldone2;
                        //fire resistant???
                        i3=myobj2->type&1023;
                        if (i3==411) {x2=0; goto IFimmuneflame3;} //dragon
                        if (i3==369) {x2=0; goto IFimmuneflame3;} //drake
                        if (i3==352) {x2=0; goto IFimmuneflame3;} //ghost
                        if (i3==374) {x2=0; goto IFimmuneflame3;} //hydra
                        if (i3==369) {x2=0; goto IFimmuneflame3;} //drake
                        if (i3==368) {x2=0; goto IFimmuneflame3;} //skeleton
                        if (i3==373) {x2=0; goto IFimmuneflame3;} //wisp
                        if (i3==367) {x2=0; goto IFimmuneflame3;} //daemon
                        //double damage?
                        if (i3==364) x2*=2; //acid slug
                        if (i3==357) x2*=2; //corpser
                        if (i3==347) x2*=2; //reaper
                        if (i3==360) x2*=2; //rot worms
                        if (i3==375) x2*=2; //slime
                        if (i3==365) x2*=2; //tangle vine
                        i3=SFnew(myobj2->x,myobj2->y);
                        sf[i3].type=1; //attack
                        sf[i3].x2=x2;
                        sf[i3].x=myobj2->x;
                        sf[i3].y=myobj2->y;
                        sf[i3].wait=0.125f; //NULL
                        sf[i3].more=1;
                        CASTSPELL_SPELLTYPE=65535;
                        spellattcrt=TRUE; goto spellattcrt0;
IFimmuneflame3:;
                      }//crt

                      if (myobj2->info&2){ //npc
                        tnpc2=(npc*)myobj2->more;
                        if (tnpc2->player){
                          if (tnpc2->player->id==powderkeg->more2){
                            x2=rnd*48;
                            x3=rnd*(tnpc2->arm+1);
                            x2-=x3; if (x2<0) x2=0;
                            if (x2){
                              tnpc2->hp-=x2;
                              tnpc2->upflags|=2; //hp
                              i3=SFnew(myobj2->x,myobj2->y);
                              sf[i3].type=1; //attack
                              sf[i3].x2=x2;
                              sf[i3].x=myobj2->x;
                              sf[i3].y=myobj2->y;
                              sf[i3].wait=0.125f; //NULL
                              sf[i3].more=1;
                            }//x2
                          }//id
                        }//player
                      }//npc
                    }//no redirector
                  }//myobj2
explosion_spelldone2:
powderkeg_dooropened:

                  if ((abs(spellx)==2)||(abs(spelly)==2)){
                    i9=SFnew(powderkeg->x+spellx,powderkeg->y+spelly);
                    sf[i9].type=8; //wand
                    sf[i9].x=powderkeg->x;
                    sf[i9].y=powderkeg->y;
                    sf[i9].x2=powderkeg->x+spellx;
                    sf[i9].y2=powderkeg->y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }

explosionblocked2:;
                }//!=4
              }}//spell x,y

              OBJremove(powderkeg);

            }//!x
          }//f
          goto skip_ouli;
        }



        if (myobj->info&4){ //<-crt
          static unsigned char target_struct; target_struct=2;

          x=myobj->x; y=myobj->y;
          if (crt=(creature*)myobj->more){
            if (crt->hp!=NULL){

              if (crt->flags&2){ //invisible!
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  x2=rnd*64; if (!x2) crt->flags-=2;
                  x2=rnd*16; if (!x2) VLNKremove(myobj);
                }//1.0
              }//2



              if (crt->flags&32){ //charmed!
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  if (crt->charm>0){
                    crt->charm--;
                  }else{crt->flags-=32; crt->al=1;}
                }//1.0
              }//32

              if (crt->flags&1){ //poisoned?
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  x2=rnd*2; if (x2==0){
                    if (crt->hp) crt->hp--;
                    i3=SFnew(myobj->x,myobj->y);
                    sf[i3].type=1; //attack
                    sf[i3].x2=1;
                    sf[i3].x=myobj->x;
                    sf[i3].y=myobj->y;
                    sf[i3].wait=0.125f; //NULL
                    sf[i3].more=1;
                  }}}


              if (myobj2=(object*)myobj->prev){

                if (myobj2->type==OBJ_POISON_FIELD){ //poison field
                  crt->flags|=1; //poison creature
                }

                if (myobj2->type==OBJ_FIRE_FIELD){ //fire field -8hp/sec
                  i3=myobj->type&1023;
                  x2=2;
                  if (i3==411) x2=0; //dragon
                  if (i3==369) x2=0; //drake
                  if (i3==352) x2=0; //ghost
                  if (i3==374) x2=0; //hydra
                  if (i3==369) x2=0; //drake
                  if (i3==368) x2=0; //skeleton
                  if (i3==373) x2=0; //wisp
                  if (i3==367) x2=0; //daemon
                  //double damage?
                  if (i3==364) x2*=2; //acid slug
                  if (i3==357) x2*=2; //corpser
                  if (i3==347) x2*=2; //reaper
                  if (i3==360) x2*=2; //rot worms
                  if (i3==375) x2*=2; //slime
                  if (i3==365) x2*=2; //tangle vine
                  if (x2){
                    f=0.5f; if (int((ett/f))!=int((ett/f)-(et/f))){
                      i3=SFnew(myobj->x,myobj->y);
                      sf[i3].type=1; //attack
                      sf[i3].x2=x2; //attack
                      sf[i3].x=myobj->x;
                      sf[i3].y=myobj->y;
                      sf[i3].wait=0.125f; //NULL
                      sf[i3].more=1;
                      crt->hp--;
                    }//f
                  }//x2
                }//fire field


                if (myobj2->type==OBJ_SLEEP_FIELD){ //sleep field
                  x2=4+rnd*16;
                  //immune to sleep?
                  i3=myobj->type&1023;
                  if (i3==364) x2=0; //acid slug
                  if (i3==427) x2=0; //ant
                  if (i3==357) x2=0; //corpser
                  if (i3==367) x2=0; //daemon
                  if (i3==362) x2=0; //winged garg
                  if (i3==363) x2=0; //garg
                  if (i3==355) x2=0; //gazer
                  if (i3==352) x2=0; //ghost
                  if (i3==374) x2=0; //hydra
                  if (i3==343) x2=0; //insects
                  if (i3==347) x2=0; //reaper
                  if (i3==360) x2=0; //rotworms
                  if (i3==426) x2=0; //scorpion
                  if (i3==368) x2=0; //skeleton
                  if (i3==375) x2=0; //slime
                  if (i3==345) x2=0; //squid
                  if (i3==365) x2=0; //tanglevine
                  if (i3==373) x2=0; //wisp
                  if ((crt->flags&4)==0){
                    //NOTE: sleep uses a paralyze effect, but to identify the difference visually (sleeping body) asleep flag is set
                    f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                      crt->wait+=x2; crt->flags|=(4+8); //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                    }//f
                  }


                }//sleep field


                if (myobj2->type==OBJ_WEB){ //web paralyze monsters
                  x2=rnd*3;
                  if (x2==0) {
                    x2=4+rnd*8;
                    //immune to sleep?
                    i3=myobj->type&1023;
                    if (i3==OBJ_GIANT_SPIDER) x2=0; //giant spider
                    if ((crt->flags&8)==0){
                      //make creature stuck in the web
                      f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                        crt->wait+=x2; crt->flags|=8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                      }//f
                    }
                  }
                }//web


              }//prev object

              if (crt->respawn_hp==0){
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  crt->respawn_x--; if (crt->respawn_x==0) goto summonedcreature_remove;
                }
              }

              if ((myobj->type&1023)==366) crt->hp=32767; //invincible tanglevine tendril!
              if ((myobj->type&1023)==430) if (crt->flags&128) crt->hp=32767;//invincible horse


              //creature died!
              if (crt->hp<=0){
summonedcreature_remove:
                myobj2=myobj;
                myobj4=(object*)crt->items->more; //will point to first item
dropgoldmore2:
                if (myobj4){
                  myobj5=(object*)myobj4->next;
                  OBJmove_allow=TRUE;
                  OBJmove(myobj4,myobj2->x,myobj2->y);
                  OBJmove_allow=FALSE;
                  myobj4->info|=112; //set flags as temp OBJ
                  if (myobj5) {myobj4=myobj5; goto dropgoldmore2;}
                }
                crtrespawn(myobj2);
                goto donemove;
              }//hp<=0



              static unsigned char crt_casti=0;


              f=0.25f; if (int((ett/f))!=int((ett/f)-(et/f))) { //cast
                x2=rnd*32;
                if (x2==0){
                  crt->cast=1;
                }//x2==0
              }//cast

              f=8.0f-((float)(obji[sprlnk[myobj->type&1023]].v7&255))/8.0f; //mp+ interval
              if (int((ett/f))!=int((ett/f)-(et/f))) { //increase mp
                crt->mp++; if (crt->mp>8) crt->mp=8;
                if (crt->mp==8) crt->cast=1; //force cast on max mp
              }

              static object* validate_obj;
              validate_obj=NULL;

              crt->wait-=et; if (crt->wait<0) crt->wait=0;

              if (crt->target){
                f=2.0f; if (int((ett/f))!=int((ett/f)-(et/f))) { //reselect target
                  if ((crt->al==1)||(crt->al==4)){ //not al==3(passive turned aggresive)
                    x2=rnd*32;
                    if (x2==0){
                      f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                      VLNKsremove(myobj); crt->target=NULL; crt->pathn=0; goto donemove;
                    }
                  }//al
                }//reselect target


                f=0.5f; if (int((ett/f))!=int((ett/f)-(et/f))){ //validate target (~once every 4 sec)
                  if ((crt->al==1)||(crt->al==4)){ //not al==3(passive turned aggresive)
                    x2=rnd*8;
                    if (x2==0){
                      validate_obj=crt->target;
                      VLNKsremove(myobj); crt->target=NULL; crt->pathn=0;
                    }
                  }
                }//validate target

              }//crt->target

              if (crt->wait==0){
                if (crt->flags&8) crt->flags-=8; //unparalyze //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                if (crt->flags&4) crt->flags-=4; //awaken //1=poison, 2=invisible, 4=asleep, 8=paralyzed


                if (crt->al==0){
                  x2=rnd*4; x3=x; y3=y;
                  if (x2==0) x3--;
                  if (x2==1) x3++;
                  if (x2==2) y3--;
                  if (x2==3) y3++;
                  f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                  goto crt_walk; //wander
                }//al==0

                //STEP 1: ENUMERATE TARGETS
                /*
                struct crtenum_struct{
                char x;
                char y;
                };
                crtenum_struct crtenum[1073];
                object* crtenum_pathok_castok[1024]; short crtenum_pathok_castok_i;
                object* crtenum_pathok[1024]; short crtenum_pathok_i;
                object* crtenum_castok[1024]; short crtenum_castok_i;


                significant changes:
                can't target on diagonals (goes for party npc too)
                should CHECK for best target randomly every 8 seconds, keeping current target only if it is the best
                MUST ALWAYS SELECT A TARGET FROM THE "BEST" GROUP
                BEST ORDER:
                1. PATH+MAGIC
                2. PATH ONLY
                3. MAGIC ONLY
                these 3 changes will fix 99% of combat issues


                */
                if (!crt->target){
                  x4=myobj->type&1023; x5=1; //movement type
                  if (x4==365) x5=-1; //tanglevine
                  if (x4==347) x5=-1; //reaper
                  if (x4==345) x5=2; //squid
                  if (x4==346) x5=2; //sea serpent
                  if (x4==343) x5=3; //insects
                  if (x4==344) x5=3; //bat
                  if (x4==356) x5=3; //bird
                  if (x4==362) x5=3; //winged gargoyle
                  if (x4==369) x5=3; //drake
                  if (x4==372) x5=3; //mongbat
                  if (x4==373) x5=3; //wisp
                  if (x4==411) x5=3; //dragon
                  if (x4==352) x5=4; //GHOST (4=unlimited!)
                  if (x4==366) x5=5; //tanglevine tendril

                  //ERROR!!! DO PASSIVE MONSTERS NEED TO ENUMERATE??? NO THEY DO NOT!!
                  //on surface max distance is 18
                  //max distance underground is 10
                  i4=18; if ((x>=1024)&&(!((y>=256)&&(x<1280)&&(y<512)))) i4=10; // fixed so that excludes gargland properly
                  ZeroMemory(&wpf_sourcedest,sizeof(wpf_sourcedest));

                  WPF_OBJECT=myobj; if (x5!=-1) wpf_pathfind(NULL,x,y,0,0,i4,x5,8);//find all accessable squares

                  target_struct=2+4; //target NPC + CRT
                  if (crt->al==4) target_struct=4; //target CRT

                  crtenum_pathok_castok_i=-1; crtenum_pathok_i=-1; crtenum_castok_i=-1;
                  i4=1072; if ((x>=1024)&&(!((y>=256)&&(x<1280)&&(y<512)))) i4=440;
                  for (x2=0;x2<=i4;x2++){


                    x3=x+crtenum[x2].x; y3=y+crtenum[x2].y;
                    if (myobj2=od[y3][x3]){
                      if ((x3>=0)&&(x3<=2047)&&(y3>=0)&&(y3<=1023)){
crtenum_next:
                        if (myobj2->info&target_struct){ //gpg top right corner of map dead guys
                          tnpc2=(npc*)myobj2->more;
                          i3=1;
                          if (target_struct==4){ //targeting CRT only, crt is friendly
                            if (tnpc2->al!=1) i3=0;
                          }else{ //targeting NPC + CRT, crt is NOT friendly
                            if (myobj2->info&2){
                              //target_struct=2;
                              if (tnpc2->player){
                                if (((myobj->type&1023)==362)||((myobj->type&1023)==363)){//gargoyle
                                  if (myobj->x>=1024){ if (myobj->y>=256){
                                    if (myobj->x<1280){ if (myobj->y<512){
                                      //amulet of submission check
                                      tplayer2=tnpc2->player;
                                      if (tplayer2->GNPCflags[12]) i3=0; //has amulet of submission or has finished game
                                      //beh lem check! (the tricky one) (if he is dead gargs attack, and this is only natural)
                                      for (x4=1;x4<=7;x4++){
                                        if (tplayer2->party[x4]){
                                          tnpc3=(npc*)tplayer2->party[x4]->more;
                                          if (tnpc3->converse==162) i3=0;
                                        }//tplayer2->party[x4]
                                      }//x4
                                    }}}}//x,y,x,y
                                }//gargoyle

                              }else{
                                i3=0; //crt can't attack passive NPC
                              }

                            }else{
                              //target_struct=4;
                              if (tnpc2->al!=4) i3=0; //crt can't attack al==1 or passive monsters
                            }//info&2
                          }//target_struct==4
                          if (tnpc2->flags&2) i3=0; //crt can't target invisible CRT/NPC
                          if (i3){
                            i3=3;
                            //check path finding
                            x4=abs(myobj->x-myobj2->x); y4=abs(myobj->y-myobj2->y);
                            if ((x4<=1)&&(y4<=1)) goto crtenum_close;
                            if (x5==-1){i3--; goto crtenum_close;}
                            i5=1;
                            if (wpf_sourcedest[myobj2->x-WPF_OFFSETX][myobj2->y-WPF_OFFSETY]&1) i5=0;
                            if (wpf_sourcedest[myobj2->x-WPF_OFFSETX-1][myobj2->y-WPF_OFFSETY]&1) i5=0;
                            if (wpf_sourcedest[myobj2->x-WPF_OFFSETX+1][myobj2->y-WPF_OFFSETY]&1) i5=0;
                            if (wpf_sourcedest[myobj2->x-WPF_OFFSETX][myobj2->y-WPF_OFFSETY-1]&1) i5=0;
                            if (wpf_sourcedest[myobj2->x-WPF_OFFSETX][myobj2->y-WPF_OFFSETY+1]&1) i5=0;
                            i3-=i5;
crtenum_close:
                            //check casting
                            if ((abs(x-x3)<=8)&&(abs(y-y3)<=8)){ //in spell range!
                              if (OBJcheckbolt(x,y,x3,y3)) i3-=2; //blocked
                            }else{
                              i3-=2;
                            }
                            if (i3==3) {crtenum_pathok_castok_i++; crtenum_pathok_castok[crtenum_pathok_castok_i]=myobj2;}
                            if (i3==2) {crtenum_castok_i++; crtenum_castok[crtenum_castok_i]=myobj2;}
                            if (i3==1) {crtenum_pathok_i++; crtenum_pathok[crtenum_pathok_i]=myobj2;}
                          }//i3
                        }//myobj2->info&target_struct
                        if (myobj2->next){ myobj2=(object*)myobj2->next; goto crtenum_next;}
                      }//x&y check
                    }//myobj2
                  }//x2


                  //choose a target based on enumerated info
                  if ((crtenum_pathok_castok_i==-1)&&(crtenum_pathok_i==-1)&&(crtenum_castok_i==-1)){ //no target!
                    validate_obj=NULL;
                    x2=rnd*4; x3=x; y3=y;
                    if (x2==0) x3--;
                    if (x2==1) x3++;
                    if (x2==2) y3--;
                    if (x2==3) y3++;
                    f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                    goto crt_walk; //wander
                  }

                  if (crtenum_pathok_castok_i!=-1){
                    if (validate_obj){
                      for (x2=0;x2<=crtenum_pathok_castok_i;x2++){
                        if (crtenum_pathok_castok[x2]==validate_obj){
                          validate_obj=NULL; myobj2=crtenum_pathok_castok[x2]; goto crtenum_gottarget;
                        }
                      }//x2
                      validate_obj=NULL;
                    }//validate_obj
                    x2=0;
crtenum_selecttarget:
                    myobj2=crtenum_pathok_castok[x2];
                    y2=rnd*2; if (y2||(crtenum_pathok_castok_i==x2)) goto crtenum_gottarget;
                    if (crtenum_pathok_castok_i>x2){ x2++; goto crtenum_selecttarget;}
                  }

                  if (crtenum_pathok_i!=-1){
                    if (validate_obj){
                      for (x2=0;x2<=crtenum_pathok_i;x2++){
                        if (crtenum_pathok[x2]==validate_obj){
                          validate_obj=NULL; myobj2=crtenum_pathok[x2]; goto crtenum_gottarget;
                        }
                      }//x2
                      validate_obj=NULL;
                    }//validate_obj
                    x2=0;
crtenum_selecttarget2:
                    myobj2=crtenum_pathok[x2];
                    y2=rnd*2; if (y2||(crtenum_pathok_i==x2)) goto crtenum_gottarget;
                    if (crtenum_pathok_i>x2){ x2++; goto crtenum_selecttarget2;}
                  }

                  if (crtenum_castok_i!=-1){
                    if (validate_obj){
                      for (x2=0;x2<=crtenum_castok_i;x2++){
                        if (crtenum_castok[x2]==validate_obj){
                          validate_obj=NULL; myobj2=crtenum_castok[x2]; goto crtenum_gottarget;
                        }
                      }//x2
                      validate_obj=NULL;
                    }//validate_obj
                    x2=0;
crtenum_selecttarget3:
                    myobj2=crtenum_castok[x2];
                    y2=rnd*2; if (y2||(crtenum_castok_i==x2)) goto crtenum_gottarget;
                    if (crtenum_castok_i>x2){ x2++; goto crtenum_selecttarget3;}
                  }
crtenum_gottarget:
                  VLNKsremove(myobj); //remove any prev vlnk(s) created
                  crt->target=myobj2; crt->pathn=0;
                  VLNKnew(myobj,crt->target,(unsigned long)&crt->target); //add new vlnk to target
                  f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;

                  if (myobj2->info&2){ //play battle music!
                    tnpc=(npc*)myobj2->more;
                    if (tnpc->player){
                      if (obji[sprlnk[myobj->type&1023]].v5>=10){
                        //battle music: melee
                        txtset(t,"????");
                        t->d2[0]=30;
                        t->d2[1]=9; //melee.mid
                        t->ds[1]=32; //seconds to play midi for
                        NET_send(NETplayer,tnpc->player->net,t);
                      }
                    }
                  }
                  goto donemove;
                }//->target==NULL

                //STEP 2: REVALIDATE TARGET
                myobj2=crt->target;
                //empty skiff/boat/...?
                if ((myobj2->info&(2+4))==0) goto invalidatetarget;
                tnpc=(npc*)myobj2->more; crt2=(creature*)tnpc; target_struct=myobj2->info&(2+4);
                //invisible?
                if (tnpc->flags&2) goto invalidatetarget;
                //recheck allegiance
                if (target_struct==4){ //targeting CRT
                  if ((crt->al==1)&&(crt2->al==1)) goto invalidatetarget;
                  if ((crt->al==4)&&(crt2->al==4)) goto invalidatetarget;
                }else{ //targeting NPC
                  if (!tnpc->player) goto invalidatetarget;
                  if (crt->al==4) goto invalidatetarget;
                }
                goto revalidatetargetok;
invalidatetarget:
                f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                VLNKsremove(myobj);
                crt->target=NULL; crt->pathn=0;
                goto donemove;
revalidatetargetok:

                //STEP 3: CAST
                //after casting if nothing else is possible we go back to the enumeration phase



                //have target!
                //attack with a spell?
                //crt spell
                //in range?
                x2=crt->target->x; y2=crt->target->y;

                if (crt->cast){
                  if (stormcloakcheck(x,y)) goto crtcantcast;
                  crt_casti=rnd*8;
                  crt->cast=NULL;
                  x4=myobj->type&1023;

                  //summon! KXC
                  x3=0;

                  if ((x4==411)&&((crt_casti&1)==1)) x3=1; //dragon

                  if (x3){ //75% change of unselecting spell
                    x3=rnd*4;
                    if (x3) x3=0; else x3=1;
                  }

                  if (x3){
                    if (crt->mp>=8){ crt->mp-=8; //mp
                    x3=rnd*3; x3--; y3=rnd*3; y3--;
                    x3+=x; y3+=y;
                    if ((bt[y3][x3]>>10)&1){
                      myobj3=OBJnew();
                      myobj3->type=367; //daemon!
                      myobj3->more=malloc(sizeof(creature));
                      ZeroMemory(myobj3->more,sizeof(creature));
                      crt=(creature*)myobj3->more;
                      crt->crt_struct=TRUE;
                      crt->hp=(obji[sprlnk[myobj3->type&1023]].v8>>8)*4;
                      crt->mp=rnd*9;
                      crt->al=1;
                      x4=rnd*4; if (x4==0) crt->al=4; //25% chance of good allegiance!
                      crt->respawn_x=1024; //seconds till spawned crt will disappear
                      myobj9=OBJnew(); myobj9->type=188; crt->items=myobj9; //(not included)bag
                      myobj3->info|=4; //<-crt
                      OBJadd(x3,y3,myobj3);
                      crt->wait+=1.0f+rnd*1.0f; goto donemove;
                    }//land passable
                    }//mp
                  }//x3==1 summon

                  x3=0; //invisibility! SL
                  if ((x4==367)&&((crt_casti&2)==2)) x3=1; //deamon
                  if (x4==352) x3=1; //ghost

                  if (x3){
                    if (crt->mp>=5){ crt->mp-=5; //mp
                    i3=SFnew(x,y); //destination is more important than the source
                    sf[i3].type=19;
                    sf[i3].x=x;
                    sf[i3].y=y;
                    sf[i3].x2=x;
                    sf[i3].y2=y;
                    sf[i3].more=0xFFFF;
                    sf[i3].wait=1;
                    crt->flags|=2;
                    crt->wait+=1.0f+rnd*1.0f; goto donemove;
                    }//mp
                  }//x3==1, invisibility done

                  //type 2: direct attack spells
                  if ((abs(x-x2)<=8)&&(abs(y-y2)<=8)){ //in spell range!
                    if (OBJcheckbolt(x,y,x2,y2)==NULL){ //not blocked
                      if (stormcloakcheck(x2,y2)) goto crtcantcast;

                      //paralyze! AXP
                      x3=0;
                      if ((x4==362)&&((crt_casti&1)==1)) x3=1; //winged gargoyle
                      if (x3){ //50% change of unselecting spell
                        x3=rnd*2;
                        if (x3) x3=0; else x3=1;
                      }

                      if ((x4==378)&&((crt_casti&2)==2)) x3=1; //mage
                      if (x3){

                        if (target_struct==2){
                          if (tnpc->wait_disable) goto already_paralyzed;
                        }

                        if (crt->mp>=5){ crt->mp-=5; //mp
                        i3=SFnew(x2,y2); //destination is more important than the source
                        sf[i3].type=14; //blue ball
                        sf[i3].x=x;
                        sf[i3].y=y;
                        sf[i3].x2=x2;
                        sf[i3].y2=y2;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=1;
                        x3=obji[sprlnk[myobj->type&1023]].v7&255;
                        x3/=4;
                        x3=x3+(rnd*x3);
                        if (target_struct==2){
                          if (x3){
                            tnpc->wait_disable+=x3;
                            tnpc->flags|=8; //paralyzed
                            if (tnpc->player){
                              txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," paralyzed!"); NET_send(NETplayer,tnpc->player->net,t);
                            }
                          }
                        }else{
                          tnpc->wait_walk+=x3;
                        }
                        crt->wait+=1.0f+rnd*1.0f; goto donemove;
                        }//mp
                      }//x3==1 paralyze
already_paralyzed:

                      //lightning! OG
                      x3=0;
                      if ((x4==347)&&((crt_casti&1)==0)) x3=1; //reaper
                      if ((x4==362)&&((crt_casti&1)==0)) x3=1; //winged gargoyle
                      if (x4==355) x3=1; //gazer
                      if ((x4==378)&&((crt_casti&1)==0)) x3=1; //mage
                      if ((x4==369)&&((crt_casti&1)==0)) x3=1; //drake
                      if (x3){
                        if (crt->mp>=5){ crt->mp-=5; //mp
                        i3=SFnew(x2,y2); //destination is more important than the source
                        sf[i3].type=5; //wand
                        sf[i3].x=x;
                        sf[i3].y=y;
                        sf[i3].x2=x2;
                        sf[i3].y2=y2;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=1;
                        if (tnpc->lev==1 && (tnpc->exp<1600)){
                          goto donemove;
                        }
                        x3=rnd*(48); 
                        //it should do same damage as player lightning spell and armour definelty doesn't reduce the damage it didn't in U6 so why it should in u6o.
                        //maybe armour should reduce some damage, but definetly not much.

                        //x3=rnd*(16+((obji[sprlnk[myobj->type&1023]].v7&255)/2));
                        //x3=rnd*(obji[sprlnk[myobj->type&1023]].v7&255);

                        if (target_struct==2) x5=rnd*(tnpc->arm+1); else x5=rnd*((obji[sprlnk[crt->target->type&1023]].v7>>8)+1+1+getequiparmourvalue((object*)crt2->items->more));
                        x3-=x5/4; if (x3<0) x3=0; //npc armour!

                        //repel!
                        //if (target_struct==2) x5=rnd*tnpc->i; else x5=rnd*(obji[sprlnk[crt->target->type&1023]].v7&255);
                        //x6=rnd*(32+4*5);
                        //if (x5>=x6) x3=0;
                        if (x3){
                          tnpc->hp-=x3;

                          if (target_struct==2){
                            tnpc->upflags|=2; //hp
                          }else{//target_struct!=2
                            if (tnpc->flags&4){ //wake creature if asleep!
                              ((creature*)tnpc)->wait=0;
                              if (tnpc->flags&8) tnpc->flags-=8; //unparalyze //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                              if (tnpc->flags&4) tnpc->flags-=4; //awaken //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                            }//wake creature if asleep!
                          }//target_struct==2

                          i3=SFnew(x2,y2);
                          sf[i3].type=1; //attack
                          sf[i3].x2=x3;
                          sf[i3].x=x2;
                          sf[i3].y=y2;
                          sf[i3].wait=0.125f; //NULL
                          sf[i3].more=1;
                        }
                        crt->wait+=1.0f+rnd*1.0f; goto donemove;
                        }//mp
                      }//x3==1
                      //lightning done

                      //magic arrow! OJ
                      x3=0;
                      if ((x4==347)&&((crt_casti&1)==1)) x3=1; //reaper
                      if ((x4==367)&&((crt_casti&1)==0)) x3=1; //deamon
                      if (x4==345) x3=1; //squid
                      if ((x4==369)&&((crt_casti&1)==0)) x3=1; //drake
                      if (x3){
                        if (crt->mp>=2){ crt->mp-=2; //mp
                        i3=SFnew(x2,y2); //destination is more important than the source
                        sf[i3].type=2;
                        sf[i3].x=x;
                        sf[i3].y=y;
                        sf[i3].x2=x2;
                        sf[i3].y2=y2;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=1;
                        if (tnpc->lev==1 && tnpc->exp<1600){
                          goto donemove;
                        }
                        x3=rnd*24;
                        //x3=rnd*(8+((obji[sprlnk[myobj->type&1023]].v7&255)/4));
                        //x3=rnd*((obji[sprlnk[myobj->type&1023]].v7&255)/2);

                        if (target_struct==2) x5=rnd*(tnpc->arm+1); else x5=rnd*((obji[sprlnk[crt->target->type&1023]].v7>>8)+1+1+getequiparmourvalue((object*)crt2->items->more));
                        x3-=x5/4; if (x3<0) x3=0; //npc armour!

                        //repel!
                        //if (target_struct==2) x5=rnd*tnpc->i; else x5=rnd*(obji[sprlnk[crt->target->type&1023]].v7&255);
                        //x6=rnd*(32+4*2);
                        //if (x5>=x6) x3=0;

                        if (x3){
                          tnpc->hp-=x3;
                          if (target_struct==2) tnpc->upflags|=2; //hp
                          i3=SFnew(x2,y2);
                          sf[i3].type=1; //attack
                          sf[i3].x2=x3;
                          sf[i3].x=x2;
                          sf[i3].y=y2;
                          sf[i3].wait=0.125f; //NULL
                          sf[i3].more=1;
                        }
                        crt->wait+=1.0f+rnd*1.0f; goto donemove;
                        }//mp
                      }//x3==1
                      //magic arrow done


                      x3=0; //fireball! PF
                      if (x4==346) x3=1; //sea serpant
                      if ((x4==411)&&((crt_casti&1)==0)) x3=1; //dragon
                      if ((x4==369)&&((crt_casti&1)==1)) x3=1; //drake
                      if ((x4==367)&&((crt_casti&1)==1)) x3=1; //deamon
                      if ((x4==378)&&((crt_casti&1)==1)) x3=1; //mage
                      if (x3){
                        if (crt->mp>=3){ crt->mp-=3; //mp
                        i3=SFnew(x2,y2); //destination is more important than the source
                        sf[i3].type=8;
                        sf[i3].x=x;
                        sf[i3].y=y;
                        sf[i3].x2=x2;
                        sf[i3].y2=y2;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=1;

                        if (tnpc->lev==1 && tnpc->exp<1600){
                          goto donemove;
                        }
                        x3=rnd*32;
                        //x3=rnd*(12+((obji[sprlnk[myobj->type&1023]].v7&255)/3));
                        //x3=rnd*((obji[sprlnk[myobj->type&1023]].v7&255)*24/32);

                        //txtnumint(t5,tnpc->arm);
                        //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

                        if (target_struct==2) x5=rnd*(tnpc->arm+1); else x5=rnd*((obji[sprlnk[crt->target->type&1023]].v7>>8)+1+1+getequiparmourvalue((object*)crt2->items->more));
                        x3-=x5/4; if (x3<0) x3=0; //npc armour!

                        //repel!
                        //if (target_struct==2) x5=rnd*tnpc->i; else x5=rnd*(obji[sprlnk[crt->target->type&1023]].v7&255);
                        //x6=rnd*(32+4*3);
                        //if (x5>=x6) x3=0;

                        if (x3){
                          tnpc->hp-=x3;
                          if (target_struct==2) tnpc->upflags|=2; //hp
                          i3=SFnew(x2,y2);
                          sf[i3].type=1; //attack
                          sf[i3].x2=x3;
                          sf[i3].x=x2;
                          sf[i3].y=y2;
                          sf[i3].wait=0.125f; //NULL
                          sf[i3].more=1;
                        }
                        crt->wait+=1.0f+rnd*1.0f; goto donemove;
                        }//mp
                      }//x3==1
                      //fireball done

                    }//not blocked
                  }//in spell range
                }//castnow!
crtcantcast:


                if ((abs(x-x2)<=1)&&(abs(y-y2)<=1)){ //close ->attack
                  //lower hp of target

                  //tnpc=(npc*)crt->target->more; THERE IS NO PURPOSE FOR THIS LINE OF CODE!


                  if (target_struct==2) x5=rnd*(tnpc->arm+1); else x5=rnd*((obji[sprlnk[crt->target->type&1023]].v7>>8)+1+1+getequiparmourvalue((object*)crt2->items->more));



                  //x5=rnd*(tnpc->arm+1);

                  //x6=rnd*((obji[sprlnk[myobj->type&1023]].v5/2+1)+1);


                  //old
                  //x6=rnd*((obji[sprlnk[myobj->type&1023]].v5>>2)+1);

                  //enemies have the same deficiets on their attacks as players

                  //x6=rnd*(((obji[sprlnk[myobj->type&1023]].v8&255)+1)*2+1)-((obji[sprlnk[myobj->type&1023]].v8&255)+1);

                  //damage enemy can do
                  //x6=rnd*((obji[sprlnk[myobj->type&1023]].v8&255)+1+1); ***LATEST***


                  x6=obji[sprlnk[myobj->type&1023]].v8&255;

                  //GPF!
                  //if (((object*)crt->items)==NULL) exit( 789);
                  x7=getequiph2hwepdamage((object*)crt->items->more);
                  if (x7>x6){

                    //txtnumint(t7,x6);
                    //txtadd(t7,"->");
                    //txtnumint(t8,x7);
                    //txtadd(t7,t8);
                    //MessageBox(NULL,t7->d,"Ultima 6 Online",MB_OK);

                    x6=x7;
                  }


                  x6++;
                  x6=rnd*x6;
                  x6-=x5;

                  static unsigned char crtatt_damage=0;
                  crtatt_damage=0;

                  if (x6>0){ //attack will do damage!






                    //can NPC dodge the attack?

                    //generate rnd number up to ?
                    //if its not 0 you dodged the attack!

                    //even with dex 64 you still get hit half the time!!!

                    //generate 2 random numbers
                    //m is 0-31
                    //d is 0-tnpc->d
                    //if d is larger than m you dodged the attack

                    //if you have 32 dex you'll dodge half attacks on average
                    //if you have 64 dex you'll dodge all attacks on average!!

                    //currently 32 dex only dodges 1/4 of attacks



                    x7=rnd*32; //0-127

                    if (target_struct==2) x8=rnd*(tnpc->d+1); else x8=rnd*((obji[sprlnk[crt->target->type&1023]].v6>>8)+1);


                    //if (int(rnd*32)<=int(rnd*((obji[sprlnk[myobj2->type&1023]].v6>>8)+1))){


                    if (x7>x8){ //couldn't dodge the attack!
                      tnpc->hp-=x6;
                      crtatt_damage=1;



                      //poison target?
                      x5=myobj->type&1023;
                      x7=0;
                      if (x5==354) x7=1; //mouse
                      if (x5==342) x7=1; //rat
                      if (x5==360) x7=1; //rotworms
                      if (x5==426) x7=1; //scorp
                      if (x5==413) x7=1; //silver serp
                      if (x5==375) x7=1; //slime
                      if (x5==358) x7=1; //snake
                      if (x5==361) x7=1; //spider
                      if (x5==345) x7=1; //squid
                      if (x7){
                        x7=rnd*32;
                        if (x7==0){
                          if (target_struct==2){
                            if (tnpc->player){
                              if ((tnpc->flags&1)==0){
                                txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," poisoned!"); NET_send(NETplayer,tnpc->player->net,t);
                              }//flags&1
                            }//player
                            tnpc->upflags|=2; //hp
                          }//2
                          tnpc->flags|=1; //poison NPC
                        }//x7==0
                      }//x7

                      if ((myobj->type&1023)==357){ //corpser
                        x7=rnd*16;
                        if (x7==0){
                          if (target_struct==2){
                            if (tnpc->player){
                              txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," dragged under!"); NET_send(NETplayer,tnpc->player->net,t);
                            }//player
                          }//2
                          tnpc->hp=tnpc->hp>>1;
                          crt->wait+=8.0f;
                        }//x7
                      }//357


                      //txtnumint(t5,x6);
                      //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

                    }
                  }


                  //unsigned long upflags; //update NPC flags
                  //1		name, portriat, str, dex, int, lev, hp_max, mp_max
                  //2		hp
                  //4     mp
                  //8     xp
                  //16    weight (automatically set if inventory/equipped items change)
                  //32    inventory, bag icon
                  //64    equipped items
                  if (target_struct==2) tnpc->upflags|=2; //hp
                  i3=SFnew(x2,y2);
                  sf[i3].type=1; //attack
                  if (crtatt_damage==0) sf[i3].type=16;
                  sf[i3].x=x2;
                  sf[i3].x2=0; if (crtatt_damage) sf[i3].x2=x6;
                  sf[i3].y=y2;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;


                  //change direction
                  OBJdir(myobj,x2,y2);

                  f=(float)(obji[sprlnk[myobj->type&1023]].v6>>8)/64.0f; if (f>1) f=1;
                  crt->wait+=2.0-(f*0.25f);
                  //crt->wait+=1.0f;

                } //close
                else{ //far ->move towards

                  if ((myobj->type&1023)==365){ //tanglevine
                    f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                    VLNKsremove(myobj); crt->target=NULL; crt->pathn=0; goto donemove;
                  }
                  if ((myobj->type&1023)==347){ //reaper
                    f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                    VLNKsremove(myobj); crt->target=NULL; crt->pathn=0; goto donemove;
                  }
                  x5=1; //flags
                  if ((myobj->type&1023)==345) x5=2; //squid
                  if ((myobj->type&1023)==346) x5=2; //sea serpent
                  if ((myobj->type&1023)==343) x5=3; //insects
                  if ((myobj->type&1023)==344) x5=3; //bat
                  if ((myobj->type&1023)==356) x5=3; //bird
                  if ((myobj->type&1023)==362) x5=3; //winged gargoyle
                  if ((myobj->type&1023)==369) x5=3; //drake
                  if ((myobj->type&1023)==372) x5=3; //mongbat
                  if ((myobj->type&1023)==373) x5=3; //wisp
                  if ((myobj->type&1023)==411) x5=3; //dragon
                  if ((myobj->type&1023)==352) x5=4; //GHOST (4=unlimited!)
                  if ((myobj->type&1023)==366){ //tanglevive tendril
                    x5=5;
                    x4=rnd*4; if (x4){
                      f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                      crt->pathn=0;
                      goto donemove;
                    }
                  }
                  /*
                  WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,x2,y2,16,0,0);
                  if (WPF_RETURN==WPF_PATHFOUND){
                  if (WPF_PATHLENGTH){
                  tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;
                  playerlist[tpl]->pathfind=1; //use pathfinding formula!
                  goto doneclmess;
                  }
                  playerlist[tpl]->pathfind=0; tnpc->path_max=0; tnpc->pathn=0;
                  }
                  }//party
                  */


                  if (crt->pathn) goto crt_gotpath;
                  x4=32; if ((x>=1024)&&(!((y>=256)&&(x<1280)&&(y<512)))) x4=16;
                  WPF_NEXTTO=1; WPF_OBJECT=myobj; wpf_pathfind(crt->path,x,y,x2,y2,x4,x5,0);
                  //SPECIAL NOTE: REAPER AND TANGLEVINE SKIP THE ABOVE LINE OF CODE

                  x4=0;
                  if (WPF_RETURN==WPF_PATHFOUND){
                    if (WPF_PATHLENGTH){
                      x4=1;
                      if (WPF_PATHLENGTH>4) WPF_PATHLENGTH=4;
                      crt->path_max=WPF_PATHLENGTH; crt->pathn=WPF_PATHLENGTH;
                    }
                  }


                  if (x4==0){
                    //PATHFIND_NEXTTO=1; x4=pathfind(crt->path,x,y,x2,y2,x5);
                    //x5=28+4; if ((x2>=1024)&&(y2<256)) x5=16;
                    //if ((x4==0)||(x4>x5)){
                    crt->pathn=0;
                    f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                    VLNKsremove(myobj); crt->target=NULL; crt->pathn=0;
                    x2=rnd*4; x3=x; y3=y;
                    if (x2==0) x3--;
                    if (x2==1) x3++;
                    if (x2==2) y3--;
                    if (x2==3) y3++;
                    goto crt_walk; //wander
                  }

crt_gotpath:
                  x3=x; y3=y;
                  /*
                  if (crt->path[0]==PFright) x3++;
                  if (crt->path[0]==PFleft) x3--;
                  if (crt->path[0]==PFup) y3--;
                  if (crt->path[0]==PFdown) y3++;
                  */
                  if (crt->path[crt->path_max-crt->pathn]==PFright) x3++;
                  if (crt->path[crt->path_max-crt->pathn]==PFleft) x3--;
                  if (crt->path[crt->path_max-crt->pathn]==PFup) y3--;
                  if (crt->path[crt->path_max-crt->pathn]==PFdown) y3++;
                  crt->pathn--;




crt_walk:
                  if ((myobj->type&1023)==366){ //tendril
                    if (abs(x3-crt->respawn_x)>=24){
                      f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                      goto donemove;
                    }
                    if (abs(y3-crt->respawn_y)>=24){
                      f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                      goto donemove;
                    }
                  }
                  if ((myobj->type&1023)==365){
                    f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                    VLNKsremove(myobj); crt->target=NULL; crt->pathn=0; goto donemove;
                  } //tanglevine
                  if ((myobj->type&1023)==347){
                    f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                    VLNKsremove(myobj); crt->target=NULL; crt->pathn=0; goto donemove;
                  } //reaper
                  if (OBJmove2(myobj,x3,y3)){ //blocked!
                    f=0.5f+rnd*0.5f; if (crt->wait<f) crt->wait=f;
                    VLNKsremove(myobj); crt->target=NULL; crt->pathn=0; goto donemove;
                  }
                  f=(float)(obji[sprlnk[myobj->type&1023]].v6>>8)/64.0f; if (f>1.0f) f=1.0f;
                  crt->wait+=(0.125f+0.25f-0.0625f-f*0.125);


                } //move towards
              } //wait
            } //alive check

          } //crt!=NULL
          goto donemove;
        } //crt

        i3=0;
        //if ((i2>=376)&&(i2<=387))i3=1;
        //if ((i2>=409)&&(i2<=410))i3=1;
        if (myobj->info&2) i3=1; //2003 has NPC struct

        if (i3==1){//NPC type (player/partymember/NPC)
          tnpc=(npc*)myobj->more;
          if (tnpc){ //tnpc!=NULL

            //walk
            f=et;
            if (tnpc->wait_attack||tnpc->wait_cast) f=et/2.0f;
            if (tnpc->wait_attack&&tnpc->wait_cast) f=et/3.0f;
            //cast
            f2=et;
            if (tnpc->wait_walk||tnpc->wait_attack) f2=et/1.333333f;
            if (tnpc->wait_walk&&tnpc->wait_attack) f2=et/1.666666f;
            //attack
            f3=et;
            if (tnpc->wait_walk||tnpc->wait_cast) f3=et/1.333333f;
            if (tnpc->wait_walk&&tnpc->wait_cast) f3=et/1.666666f;


            if (tnpc->horse){
              if (tnpc->player->craft==NULL){

                //walk
                f=et;//walk speed is never slowed by other activities
                //cast
                f2=et;
                if (tnpc->wait_attack) f2=et/1.333333f;
                //attack
                f3=et;
                if (tnpc->wait_cast) f3=et/1.333333f;
              }//!craft
            }//tnpc->horse


            tnpc->wait_walk-=f; if (tnpc->wait_walk<0) tnpc->wait_walk=0;
            f5=tnpc->wait_cast;
            tnpc->wait_cast-=f2; if (tnpc->wait_cast<0) tnpc->wait_cast=0;
            tnpc->wait_attack-=f3; if (tnpc->wait_attack<0) tnpc->wait_attack=0;
            tnpc->wait_disable-=et; if (tnpc->wait_disable<0) tnpc->wait_disable=0;
            if (tnpc->flags&8){
              if (tnpc->wait_disable==0){
                tnpc->flags-=8;
                if (tnpc->player){
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," free!"); NET_send(NETplayer,tnpc->player->net,t);
                }
              }
            }

            tplayer=tnpc->player;

            if (tplayer){

              if (myobj2=tplayer->party[tplayer->selected_partymember]){
                if (myobj==myobj2) goto npcactiondone; //selected partymember
              }

              f=0.5f; if (int((ett/f))!=int((ett/f)-(et/f))){
                x2=rnd*8; if (!x2){

                  if (tnpc->command&(8+16)){ //heal
                    for (x2=0;x2<=7;x2++){
                      if (myobj2=tplayer->party[x2]){
                        tnpc2=(npc*)myobj2->more;
                        if ((tnpc->command&16)||(tnpc==tnpc2)){
                          if (tnpc2->hp<=(tnpc2->hp_max>>1)) goto castheal_selected;
                        }
                      }
                    }
                    goto castheal_done;
castheal_selected:
                    static object *castheal_target;
                    castheal_target=myobj2;
                    x2=((0<<4)+6); //heal
                    if (tnpc2->lev>=3){ //attempt a great heal
                      x2=((3<<4)+4); //great heal
                    }
castheal_retry:
                    tpx=myobj2->x; tpy=myobj2->y; x=0; y=0;
                    cast_spell=1; goto cast_spell0; //goto spell
cast_spell1: //return #1
                    if ((i2!=1)&&(i2!=3)){
                      if (castheal_target){
                        myobj2=castheal_target; tnpc2=(npc*)myobj2;
                        castheal_target=NULL;
                        x2=((0<<4)+6); //heal
                        goto castheal_retry;
                      }
                    }
                    castheal_target=NULL;
                    goto npcactiondone;
                  }//heal
castheal_done:

                  if (tnpc->command&(32+64)){ //cure
                    for (x2=0;x2<=7;x2++){
                      if (myobj2=tplayer->party[x2]){
                        tnpc2=(npc*)myobj2->more;
                        if ((tnpc->command&64)||(tnpc==tnpc2)){
                          if (tnpc2->flags&1) goto castcure_selected;
                        }
                      }
                    }
                    goto castcure_done;
castcure_selected:
                    tpx=myobj2->x; tpy=myobj2->y; x=0; y=0;
                    x2=((0<<4)+3); //cure
                    cast_spell=2; goto cast_spell0; //goto spell
cast_spell2: //return #2
                    goto npcactiondone;
                  }//cure
castcure_done:

                  if (tnpc->command&1){ //yellowpotion
                    if (tnpc->hp<=(tnpc->hp_max>>1)){ //hp half or less?
                      //scan inventory for a potion
                      x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                      OBJlist_last=NULL;
                      for(y6=0;y6<x5;y6++){
                        if (OBJlist_list[y6]->type==(275+1024*2)){
                          tnpc->hp+=30; if (tnpc->hp>tnpc->hp_max) tnpc->hp=tnpc->hp_max;
                          tnpc->upflags|=2;
                          OBJremove(OBJlist_list[y6]);
                          tnpc->upflags|=32;
                          goto npcactiondone;
                        }//type
                      }//y6
                    }//hp
                  }//yellowpotion

                  if (tnpc->command&2){ //redpotion
                    if (tnpc->flags&1){ //poisoned
                      //scan inventory for a potion
                      x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                      OBJlist_last=NULL;
                      for(y6=0;y6<x5;y6++){
                        if (OBJlist_list[y6]->type==(275+1024*1)){
                          tnpc->flags&=(65535-1); //remove poisoned flag
                          tnpc->upflags|=2;
                          OBJremove(OBJlist_list[y6]);
                          tnpc->upflags|=32;
                          goto npcactiondone;
                        }//type
                      }//y6
                    }//poisoned
                  }//redpotion


                  if (tnpc->command&4){ //eat
                    if (!tnpc->food){
                      x6=OBJlist((object*)tnpc->items->more); //x6=last object number
                      OBJlist_last=NULL;
                      for(y6=0;y6<x6;y6++){
                        x2=0;
                        x3=OBJlist_list[y6]->type&1023;
                        if (x3==95) x2=6; //95 grapes 3 B *6
                        if (x3==96) x2=1; //96 butter 1 C *1
                        if (x3==115) {x2=5; x5=1;} //115 wine 6 6 5 5 4 6 6 4 4 =5 C *5
                        if (x3==116) {x2=4; x5=1;} //116 mead 4 5 3 2 5 4 4 4 3 =4 C *4
                        if (x3==117) {x2=4; x5=1;} //117 ale 3 7 2 5 3 3 3 4 2 =4 C *4
                        if (x3==128) x2=12; //128 bread 3 3 (2 loaves) c A *12
                        if (x3==129) x2=16; //129 meat (legs) c 4 3 3 4 4 4 4 4 4 A *16
                        if (x3==130) x2=8; //130 rolls 1 2 2 A *m
                        if (x3==131) x2=12; //131 cake 8 5 B *12
                        if (x3==132) x2=10; //132 cheese 6 5 B *10
                        if (x3==133) x2=16; //133 ham (looks like steak) 3 5 A *16
                        if (x3==135) x2=28; //135 horse chops (gargoyle food) A *28
                        if (x3==179) {x2=2; x5=1;} //179 bucket of water *this item needs modifying after use (1) B *2
                        if (x3==180) {x2=8; x5=1;} //180 bucket of milk *this item needs modifying after use 3 5 =4 B *8
                        if (x3==184) x2=10; //184 jar of honey *this item needs modifying after use 10 C *10
                        if (x3==209) x2=12; //209 small hanging meat 6 B *12
                        if (x3==210) x2=28; //210 big hanging meat 9 6 A =7 *28
                        if (x3==265) x2=6; //265 fish 3 B *6
                        if (x2){
                          x2*=4;
                          tnpc->food=x2;
                          txtset(t,"?"); t->d2[0]=8; txtaddchar(t,255); txtadd(t,tnpc->name);
                          if (x5) txtadd(t," drinks the "); else txtadd(t," eats the ");
                          y2=objname[(OBJlist_list[y6]->type&1023)*2];
                          y3=objname[(OBJlist_list[y6]->type&1023)*2+1];
                          txtNEWLEN(t2,y3);
                          memcpy(&t2->d2[0],&objname2[y2],y3);
                          txtadd(t,t2); txtadd(t,".");
                          NET_send(NETplayer,tnpc->player->net,t);
                          tnpc->upflags|=32;
                          if (obji[sprlnk[OBJlist_list[y6]->type&1023]+(OBJlist_list[y6]->type>>10)].flags&4096){
                            if (OBJlist_list[y6]->more2!=1){ //more than 1!
                              OBJlist_list[y6]->more2--; goto npcactiondone;
                            }
                          }
                          if ((x3==179)||(x3==180)) {OBJlist_list[y6]->type=178; goto npcactiondone;}
                          if (x3==184) {OBJlist_list[y6]->type=183; goto npcactiondone;}
                          OBJremove(OBJlist_list[y6]);
                          goto npcactiondone;
                        }//x2
                      }//y6
                    }//food
                  }//eat


                }//8
              }//0.5
npcactiondone:


              //$rate$
              //x2=37+3*CONnpc2->lev;
              //if ((CONnpc2->s+CONnpc2->d+CONnpc2->i)>x2) txtadd(t2,"16"); else txtadd(t2,"8");
              //deduct gold

              //1day=1hour of real time
              if (tnpc->converse==201){
                if ((tnpc->s+tnpc->d+tnpc->i)>(37+3*tnpc->lev)) f=24.0f; else f=8.0f;
                f=4096.0f/f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                  for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
                    tnpc3=(npc*)tplayer->party[x6]->more;
                    if (tnpc3!=tnpc){
                      x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
                    }
                  }} //!=NULL,x6
                  OBJlist_last=NULL;

                  for(y6=0;y6<x5;y6++){
                    if (OBJlist_list[y6]->type==88){ //gold
                      OBJlist_list[y6]->more2--;
                      if (!OBJlist_list[y6]->more2){
                        OBJremove(OBJlist_list[y6]);
                        OBJrelease(OBJlist_list[y6]);
                      }//more2
                      for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
                        tnpc3=(npc*)tplayer->party[x6]->more;
                        tnpc3->upflags|=32; //inv
                      }}
                      goto paidnpc;
                    }//88
                  }//y6
                  if (tplayer->GNPCflags[64]) tplayer->GNPCflags[64]--; //take payment from bank!
                }//f
              }//201
paidnpc:

              if (f5&&(tnpc->wait_cast==0)){
                //but which party member are we??
                for (i4=0;i4<=7;i4++){
                  if (tplayer->party[i4]==myobj){
                    txtset(t,"??"); t->d2[0]=18; t->d2[1]=i4; NET_send(NETplayer,tplayer->net,t); //release cast icon!
                  }//==myobj
                }//i4
              }//f5&&(tnpc->wait_cast==0)


              if (myobj2=(object*)myobj->prev){

                /* MOONGATES MUST BE USED 2005
                if (myobj2->type==(84+1024)){ //red gate
                if (tplayer->party[0]==myobj){ //primary player
                if (tplayer->selected_partymember==0){
                i4=(myobj2->info&112)>>4;
                if (i4==1){
                for (i4=7;i4>=0;i4--){
                if (tplayer->party[i4]!=NULL){
                OBJremove(tplayer->party[i4]);
                OBJadd(256+128-16,256+128+2,tplayer->party[i4]);
                }
                }
                }//i4
                }//partymember
                }//==party[0]
                }//red gate

                if (myobj2->type==(85+1024)){ //moongate
                if (tplayer->party[0]==myobj){ //primary player
                if (tplayer->selected_partymember==0){
                //move party, if not already at correct phase!
                if (moonphase!=255){
                if ((myobj->x!=moongatex[moonphase])||(myobj->y!=moongatey[moonphase])){
                for (i4=7;i4>=0;i4--){
                if (tplayer->party[i4]!=NULL){
                OBJremove(tplayer->party[i4]);
                OBJadd(moongatex[moonphase],moongatey[moonphase],tplayer->party[i4]);
                }
                }
                }//x!= y!=
                }//moonphase
                }//partymember
                }//==party[0]
                }//moongate
                */

                if (myobj2->type==172){ //spikes
                  f=0.5f; if (int((ett/f))!=int((ett/f)-(et/f))){
                    if (tnpc->protection==0){
                      if (tnpc->hp) tnpc->hp--;
                      if (tnpc->hp) tnpc->hp--;
                      if (tnpc->hp) tnpc->hp--;
                      if (tnpc->hp) tnpc->hp--;
                      i3=SFnew(myobj->x,myobj->y);
                      sf[i3].type=1; //attack
                      sf[i3].x2=4;
                      sf[i3].x=myobj->x;
                      sf[i3].y=myobj->y;
                      sf[i3].wait=0.125f; //NULL
                      sf[i3].more=1;
                      tnpc->upflags|=2; //hp
                    }//protection
                  }//f
                }//spikes

                if (myobj2->type==173){ //trap
                  if ((!myobj2->more2)||(myobj2->more2==tplayer->id)){
                    if (tnpc->protection==0){
                      for (x2=0;x2<=7;x2++){
                        if (tplayer->party[x2]==myobj){
                          if ((myobj->x==tplayer->trapx[x2])&&(myobj->y==tplayer->trapy[x2])) goto trap_alreadyhurt;
                          goto trap_gotpartymemberindex;
                        }
                      }
                      goto trap_alreadyhurt;
trap_gotpartymemberindex:
                      tplayer->trapx[x2]=myobj2->x; tplayer->trapy[x2]=myobj2->y;
                      if ((myobj2->info&(1<<9))==0){
                        OBJsave(myobj2->x,myobj2->y);
                        myobj2->info|=(1<<9); //make trap visible
                      }
                      x2=tnpc->hp>>2; if (!x2) x2=1; //-25% health
                      tnpc->hp-=x2;
                      i3=SFnew(myobj->x,myobj->y);
                      sf[i3].type=1; //attack
                      sf[i3].x2=x2;
                      sf[i3].x=myobj->x;
                      sf[i3].y=myobj->y;
                      sf[i3].wait=0.125f; //NULL
                      sf[i3].more=1;
                      tnpc->upflags|=2; //hp
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," wounded!"); NET_send(NETplayer,tplayer->net,t);
                    }//protection
                  }
                }//trap
trap_alreadyhurt:

                if (myobj2->type==320){ //sleep field
                  if ((myobj2->more2==tplayer->id)||(myobj2->more2==0)){
                    if ((tnpc->flags&4)==0){
                      f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                        if (tnpc->protection==0){
                          tnpc->flags|=4; //asleep!
                          f=8.0f+rnd*24.0f; if (tnpc->wait_disable<f) tnpc->wait_disable=f;
                        }
                      }
                    }
                  }
                }

                if (myobj2->type==53){ //web paralyze player
                  if ((myobj2->more2==tplayer->id)||(myobj2->more2==0)){
                    if ((tnpc->flags&8)==0){
                      f=0.5f; if (int((ett/f))!=int((ett/f)-(et/f))){
                        x3=rnd*3;
                        if (x3==0){
                          f=4.0f+rnd*12.0f; if (tnpc->wait_disable<f) tnpc->wait_disable=f;
                          tnpc->flags|=8; //paralyzed
                          if (tnpc->player){
                            txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," is stuck in a web!"); NET_send(NETplayer,tnpc->player->net,t);
                          }
                        }
                      }
                    }
                  }
                }

                if (myobj2->type==318){ //poison field
                  if ((myobj2->more2==tplayer->id)||(myobj2->more2==0)){
                    if (tnpc->protection==0){
                      if ((tnpc->flags&1)==0){
                        tnpc->flags|=1; //poison NPC
                        tnpc->upflags|=2; //hp
                      }
                    }
                  }
                }

                if (myobj2->type==317){ //fire field -4hp/sec
                  if ((myobj2->more2==tplayer->id)||(myobj2->more2==0)){
                    f=0.5f; if (int((ett/f))!=int((ett/f)-(et/f))){
                      if (tnpc->protection==0){
                        if (tnpc->hp) tnpc->hp--; //-2!
                        if (tnpc->hp) tnpc->hp--;
                        i3=SFnew(myobj->x,myobj->y);
                        sf[i3].type=1; //attack
                        sf[i3].x2=2;
                        sf[i3].x=myobj->x;
                        sf[i3].y=myobj->y;
                        sf[i3].wait=0.125f; //NULL
                        sf[i3].more=1;
                        tnpc->upflags|=2; //hp
                      }
                    }//f
                  }//id
                }//fire field


                if ((myobj2->type&1023)==366){ //tanglevine tendril
                  f=0.5f; if (int((ett/f))!=int((ett/f)-(et/f))){
                    if (tnpc->protection==0){
                      if (tnpc->horse==NULL){
                        if (tnpc->hp) tnpc->hp--;
                        i3=SFnew(myobj->x,myobj->y);
                        sf[i3].type=1; //attack
                        sf[i3].x2=1;
                        sf[i3].x=myobj->x;
                        sf[i3].y=myobj->y;
                        sf[i3].wait=0.125f; //NULL
                        sf[i3].more=1;
                        tnpc->upflags|=2; //hp
                      }//not on horse
                    }
                  }//f
                }//tanglevine tendril


                //sleep field?
                //concept when player first steps onto sleep field they fall asleep, but eventually they awake
                //and have a brief oportunity to escape, before being put back to sleep again
                //sleep usually lasts a very long and annoying time, like 16 seconds!


              }

              //experiential xp
              f=256.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                if (tplayer->idle<=256){
                  if (tplayer->party[0]){
                    if ((object*)tplayer->party[0]->more==myobj){
                      tnpc->exp++; tnpc->upflags|=8;
                    }
                  }
                }//idle
              }

              x=bt[myobj->y][myobj->x]&255;
              if ((x>=3)&&(x<=6)){
                if (tplayer->craft) goto poison_skip;
                if (tnpc->horse) goto poison_skip;
                if (tnpc->boots!=NULL){
                  if (tnpc->boots->type==28){
                    goto poison_skip;
                  }//->type!=SWAMPBOOTS
                }//->boots!=NULL
                if (tnpc->protection) goto poison_skip;
                if ((tnpc->flags&1)==0){
                  tnpc->flags|=1; //poison NPC
                  tnpc->upflags|=2; //hp
                }
poison_skip:;
              }

              if ((x>=221)&&(x<=223)){ //lava!
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  if ((tnpc->protection==0)&&(tplayer->craft==NULL)){
                    x2=0;
                    if (x==221) x2=8;
                    if (x==222) x2=16;
                    if (x==223) x2=32;
                    tnpc->hp-=x2;
                    i3=SFnew(myobj->x,myobj->y);
                    sf[i3].type=1; //attack
                    sf[i3].x2=x2;
                    sf[i3].x=myobj->x;
                    sf[i3].y=myobj->y;
                    sf[i3].wait=0.125f; //NULL
                    sf[i3].more=1;
                    tnpc->upflags|=2; //hp
                  }//->protection==0
                }
              }



              if (tnpc->flags&1){ //poisoned?
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  if (tnpc->lev==1 && tnpc->exp<1600){
                    if ((rnd*32.0f)<1.0f){
                      if (tplayer->party[0]){
                        if ((npc*)tplayer->party[0]->more==tnpc){
                          tnpc->flags&=(65535-1); //remove poisoned flag
                          tnpc->upflags|=2;
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"Inexperienced character's poison cured!"); NET_send(NETplayer,tplayer->net,t);
                          goto poisonremoved;
                        }
                      }
                    }
                  }
                  x=rnd*8; if (x==0) {
                    tnpc->hp--;
                    i3=SFnew(myobj->x,myobj->y);
                    sf[i3].type=1; //attack
                    sf[i3].x2=1;
                    sf[i3].x=myobj->x;
                    sf[i3].y=myobj->y;
                    sf[i3].wait=0.125f; //NULL
                    sf[i3].more=1;
                    tnpc->upflags|=2;
                  }
                }
              }//poisoned
poisonremoved:

              //horse food reduction
              if (tnpc->horse){
                f=60.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  x=rnd*180; if (x==0){
                    crt2=(creature*)tnpc->horse->more;
                    x=(crt2->respawn_x>>4)&3; if (x) x--;
                    crt2->respawn_x&=(0xFFFF-(3<<4));
                    crt2->respawn_x+=(x<<6);
                  }//x==0
                }//f
              }//->horse

              if (tplayer->karma<=0){ if (tplayer->party[0]){ if ((npc*)tplayer->party[0]->more==tnpc){
                tplayer->karma=0;
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))) {tnpc->hp--; tnpc->upflags|=2;}
              }}}


              //add mp
              //x3=rnd*(32+(tnpc->i>>1)); //int can only double spell effectiveness!

              if ((float)tnpc->i/4.0 >= 32.0) {
                f=0.05f;
              }else {
                f=32.0f-((float)tnpc->i/4.0f);

              }
              if (int((ett/f))!=int((ett/f)-(et/f))) {
                if (tnpc->mp!=tnpc->mp_max){
                  tnpc->mp++; if (tnpc->mp>tnpc->mp_max) tnpc->mp=tnpc->mp_max;
                  tnpc->upflags|=4;
                }
              }

              //sleeping? WAKE UP!
              if (tnpc->flags&4){
                if (tnpc->wait_disable<=0){
                  tnpc->flags-=4;
                }
              }


              //light ->~17min
              //great light ->~68min
              if (tnpc->light>=2){
                f=32.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  tnpc->light--;
                }
              }


              if (tnpc->light==1) tnpc->light=0;

              //tnpc->light=0;
              //burn down torch?
              //f=64.0f; if (int((ett/f))!=int((ett/f)-(et/f))){


              if (tnpc->wep_left!=NULL){
                if (tnpc->wep_left->type==(90+1024)){
                  x=(tnpc->wep_left->info>>9)&15;
                  if (x==1){ //burn down
                    tnpc->wep_left=NULL;
                    tnpc->upflags|=64;
                  }else{
                    f=64.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                      if (x==0) x=16;
                      x--; tnpc->wep_left->info&=(65535-(15<<9)); tnpc->wep_left->info|=(x<<9);
                    }
                    if (tnpc->light==0) tnpc->light=1;
                  }
                }
              }

              if (tnpc->wep_right!=NULL){
                if (tnpc->wep_right->type==(90+1024)){
                  x=(tnpc->wep_right->info>>9)&15;
                  if (x==1){ //burn down
                    tnpc->wep_right=NULL; tnpc->upflags|=64;
                  }else{
                    f=64.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                      if (x==0) x=16;
                      x--; tnpc->wep_right->info&=(65535-(15<<9)); tnpc->wep_right->info|=(x<<9);
                    }
                    if (tnpc->light==0) tnpc->light=1;
                  }
                }
              }


              //}//f64

              //f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){

              //if (tnpc->flags&2) tnpc->flags-=2;


              if (tnpc->protection){ //protection!
                f=4.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  tnpc->protection--; if (tnpc->protection==0) tnpc->upflags|=64; //update ->arm value
                }
              }

              //NEWCODE
              if (tnpc->negatemagic){ //negatemagic
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  tnpc->negatemagic--;
                }
              }
              //NEWCODEEND

              //invisibility ring(s)
              tnpc->flags&=(255-2);
              if (tnpc->invisibility){
                tnpc->flags|=2;
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  tnpc->invisibility--;
                }
                goto skipinvisrings;
              }


              if (tnpc->ring_left!=NULL){ if (tnpc->ring_left->type==258){ //invis
                x=tnpc->ring_left->more2; if (x==0) x=64*4; //~4min
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))) x--;
                tnpc->ring_left->more2=x;
                if (x==0){
                  tnpc->ring_left=NULL; tnpc->upflags|=64;
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s invisibility ring vanished!"); NET_send(NETplayer,tplayer->net,t);
                }else{tnpc->flags|=2; goto skipinvisrings;}
              }}
              if (tnpc->ring_right!=NULL){ if (tnpc->ring_right->type==258){
                x=tnpc->ring_right->more2; if (x==0) x=64*4; //~4min
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))) x--;
                tnpc->ring_right->more2=x;
                if (x==0){
                  tnpc->ring_right=NULL; tnpc->upflags|=64;
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s invisibility ring vanished!"); NET_send(NETplayer,tplayer->net,t);
                }else{tnpc->flags|=2;}
              }}
skipinvisrings:
              if (tnpc->flags&2){
                if (tplayer->craft) tnpc->flags&=(255-2);
              }

              if (tnpc->ring_left!=NULL){ if (tnpc->ring_left->type==257){ //regeneration ring
                if ((tnpc->hp>0)&&(tnpc->hp<tnpc->hp_max)){
                  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                    x=tnpc->ring_left->more2; if (x==0) x=128; //128hp
                    x--; tnpc->ring_left->more2=x;
                    tnpc->hp++; tnpc->upflags|=2;
                    if (x==0){
                      tnpc->ring_left=NULL; tnpc->upflags|=64;
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s regeneration ring vanished!"); NET_send(NETplayer,tplayer->net,t);
                    }
                  }//f
                }//need hp
              }}
              if (tnpc->ring_right!=NULL){ if (tnpc->ring_right->type==257){ //regeneration ring
                if ((tnpc->hp>0)&&(tnpc->hp<tnpc->hp_max)){
                  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                    x=tnpc->ring_right->more2; if (x==0) x=128; //128hp
                    x--; tnpc->ring_right->more2=x;
                    tnpc->hp++; tnpc->upflags|=2;
                    if (x==0){
                      tnpc->ring_right=NULL; tnpc->upflags|=64;
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s regeneration ring vanished!"); NET_send(NETplayer,tplayer->net,t);
                    }
                  }//f
                }//need hp
              }}
			  //increase int as well attempt - Cear
			  if (tnpc->ring_right!=NULL)
			  { 
				  if (tnpc->ring_right->type==257)//regeneration ring
				  {
					  if ((tnpc->mp>0)&&(tnpc->mp<tnpc->mp_max))
					  {
						  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f)))
						  {
								x=tnpc->ring_right->more2; if (x==0) x=128; //128hp
								x--; tnpc->ring_right->more2=x;
								tnpc->mp++; tnpc->upflags|=4;
									if (x==0)
										{
											tnpc->ring_right=NULL; tnpc->upflags|=64;
											txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s regeneration ring vanished!"); NET_send(NETplayer,tplayer->net,t);
										}
						}//f
					}//need hp
				}
			  }
			   if (tnpc->ring_left!=NULL)
			  { 
				  if (tnpc->ring_left->type==257)//regeneration ring
				  {
					  if ((tnpc->mp>0)&&(tnpc->mp<tnpc->mp_max))
					  {
						  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f)))
						  {
								x=tnpc->ring_left->more2; if (x==0) x=128; //128hp
								x--; tnpc->ring_left->more2=x;
								tnpc->mp++; tnpc->upflags|=4;
									if (x==0)
										{
											tnpc->ring_right=NULL; tnpc->upflags|=64;
											txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s regeneration ring vanished!"); NET_send(NETplayer,tplayer->net,t);
										}
						}//f
					}//need hp
				}
			  }


              if (tnpc->ring_left!=NULL){ if (tnpc->ring_left->type==256){ //protection ring
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  x=tnpc->ring_left->more2; if (x==0) x=4096; //~1hr
                  x--; tnpc->ring_left->more2=x;
                  if (x==0){
                    tnpc->ring_left=NULL; tnpc->upflags|=64;
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s protection ring vanished!"); NET_send(NETplayer,tplayer->net,t);
                  }
                }//f
              }}
              if (tnpc->ring_right!=NULL){ if (tnpc->ring_right->type==256){ //
                f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){
                  x=tnpc->ring_right->more2; if (x==0) x=4096; //~1hr
                  x--; tnpc->ring_right->more2=x;
                  if (x==0){
                    tnpc->ring_right=NULL; tnpc->upflags|=64;
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t,"'s protection ring vanished!"); NET_send(NETplayer,tplayer->net,t);
                  }
                }//f
              }}




              if (tnpc->food){//has food
                f=16.0f;

                //make fighters (high STR players) heal faster using food
                //STR 16 is same, STR 48 is double
                if (tnpc->s>16){
                  f2=tnpc->s;
                  f2=(f2-16.0f)/4.0f;
                  f-=f2;
                }
                if (int((ett/f))!=int((ett/f)-(et/f))){
                  tnpc->food--;
                  if (!tnpc->food){
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," is hungry!"); NET_send(NETplayer,tplayer->net,t);
                  }
                  if (tnpc->hp<tnpc->hp_max) tnpc->hp++;
                  tnpc->upflags|=2;
                }
              }





              //}//f1

            }//tplayer


            if (tplayer==NULL){ //NPC, not party member
              if (tnpc->wait_walk==0){ //wait
                if (tnpc->order==2){ //schedule

                  //*IMPORTANT* needs to check for nearest schedule, regardless of order, 
                  //*MORE IMPORTANTLY* it needs to be day indepentant, reading a schedule from the prev
                  //day if necessary

                  //assuming a valid schedule exists...
                  x3=-1; f=btime+(bday*24-24);
SCHFND_retry:
                  f3=65536;

                  //find nearest prev schedule in the list
                  for (x2=0;x2<=31;x2++){ //find schedule
                    if (tnpc->schedule[x2].hour!=255){
                      f2=tnpc->schedule[x2].hour;
                      if (tnpc->schedule[x2].day){
                        f2+=(tnpc->schedule[x2].day*24-24); x4=1;
                      }else{
                        f2+=(bday*24-24); x4=0; //use current day
                      }
                      if (f2<f){
                        if ((f-f2)<=f3){
                          if(x4){
                            x3=x2;
                          }else{
                            if ((f-f2)<f3) x3=x2;
                          }//x4
                          f3=(f-f2); //new closest sch time
                        }//(f-f2)<=f3
                      }//f2<f
                    }//!=255
                  }//x2
                  if (x3==-1) {x3--; f=24*8; goto SCHFND_retry;}
                  if (x3==-2) goto NPC_skip1;
                  x2=x3;
                  tnpc->schedule_last=tnpc->schedule[x2].type;

                  if ((abs(myobj->x-tnpc->schedule[x2].x)==0)&&(abs(myobj->y-tnpc->schedule[x2].y)==0)){
                    tnpc->schedule_i=x2;
                  }
                  if (x2!=tnpc->schedule_i) tnpc->schedule_i=-1;

                  if (tnpc->schedule_i==x2){
                    if ((tnpc->schedule[x2].type==0x8F)||(tnpc->schedule[x2].type==0x90)){
                      //wander aimlessly!!!
                      if ((abs(myobj->x-tnpc->schedule[x2].x)<=4)&&(abs(myobj->y-tnpc->schedule[x2].y)<=4)){
                        x=rnd*4; x2=0; y2=0;
                        if (x==0) x2--;
                        if (x==1) x2++;
                        if (x==2) y2--;
                        if (x==3) y2++;
                        tnpc->wait_walk+=1.0f+rnd*2.0f;
                        tnpc->pathn=0; //clear pathfind
                        goto npc_walk; //walk randomly
                      }//<4
                      tnpc->schedule_i=-1;
                    }
                  }

                  /*

                  for (x2=0;x2<=31;x2++){ //find schedule
                  if (x2==31) goto gotschedule; //last index


                  if ((tnpc->schedule[x2].day==0)||(tnpc->schedule[x2].day==bday)){ //valid day
                  f=tnpc->schedule[x2].hour;
                  x3=1;
                  sch_sametimeday:
                  f2=tnpc->schedule[x2+x3].hour;
                  if (f2==255) goto gotschedule; //next index is NULL (use the current schedule)
                  if ((tnpc->schedule[x2+x3].day!=0)&&(tnpc->schedule[x2+x3].day!=bday)){x3++; goto sch_sametimeday;} //next sch is for a different day, ignore
                  if (f2==f) {x3++; goto sch_sametimeday;} //next sch is for the same time, ignore
                  if ((btime>=f)&&(btime<=f2)) goto gotschedule;
                  } //valid day
                  }//schedule 0-31 loop
                  goto NPC_skip1; //schedule not found ->exit
                  gotschedule: //index x2
                  if (btime<tnpc->schedule[x2].hour) NPC_skip1; //false schedule read by accident!
                  */

                  //gotschedule:

                  //goto .x,.y

                  x3=myobj->x-tnpc->schedule[x2].x;
                  y3=myobj->y-tnpc->schedule[x2].y;
                  if ((x3==0)&&(y3==0)) goto NPC_skip1;

                  if (tnpc->pathn==0){






                    /*
                    tnpc->path_max=pathfind(tnpc->path,myobj->x,myobj->y,tnpc->schedule[x2].x,tnpc->schedule[x2].y,NULL);
                    //if (tnpc->path_max==0) tnpc->path_max=pathfind(tnpc->path,myobj->x,myobj->y,tnpc->schedule[x2].x,tnpc->schedule[x2].y,NULL);
                    if (tnpc->path_max==0){ tnpc->pathn=0; tnpc->wait_walk+=1.0f+rnd*2.0f; goto NPC_skip1;}
                    tnpc->pathn=tnpc->path_max;
                    */
                    /*
                    z=abs(myobj2->x-myobj->x); z2=abs(myobj2->y-myobj->y); if (z2>z) z=z2;
                    z+=32; if (z>250) z=250;
                    WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,z,0,1);//|1|PATHFIND THROUGH OWN PARTY+16
                    if (WPF_RETURN==WPF_PATHFOUND){
                    if (WPF_PATHLENGTH){
                    tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;
                    goto follow_getpath;
                    }
                    }
                    */
                    /*
                    x3=abs(tnpc->schedule[x2].x-myobj->x); y3=abs(tnpc->schedule[x2].y-myobj->y); if (y3>x3) x3=y3;
                    x3+=32;
                    //if (x2>250) exit2(55);

                    if (x3>250){
                    txtnumint(t5,tnpc->converse); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
                    tnpc->pathn=0; tnpc->wait_walk+=1.0f+rnd*2.0f; goto NPC_skip1;
                    }
                    */

                    //if (x3>dv) dv=x3;

                    x3=abs(tnpc->schedule[x2].x-myobj->x); y3=abs(tnpc->schedule[x2].y-myobj->y); if (y3>x3) x3=y3;
                    x3+=64;
                    if (x3>250){
                      x3=250;
                    }
                    WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,tnpc->schedule[x2].x,tnpc->schedule[x2].y,x3,0,4);
                    if (WPF_RETURN==WPF_NOPATHFOUND){
                      tnpc->pathn=0; tnpc->wait_walk+=1.0f+rnd*2.0f; goto NPC_skip1;
                    }
                    tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;



                  }
                  //path
                  x2=0; y2=0;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFright) x2=1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFleft) x2=-1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFup) y2=-1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFdown) y2=1;
                  if (tnpc->pathn>0) { //fix for pathfinding crash.
                    tnpc->pathn--;
                  }

npc_walk:

                  myobj2=OBJfindlastall(myobj->x,myobj->y);
                  x3=OBJmove2(myobj,myobj->x+x2,myobj->y+y2);

                  if (myobj2!=NULL){
                    if (((myobj2->type&1023)==301)&&(x3==0)){ //close door behind NPC
                      myobj2=OBJfindlast(myobj2->x,myobj2->y);
                      if (myobj2!=NULL){
                        x2=myobj2->type&1023;
                        if ((x2>=297)&&(x2<=300)){
                          //assume not locked
                          x=myobj2->type>>10;
                          if (x&1) {myobj3=myobj2; myobj2=(object*)myobj2->more;} else {myobj3=(object*)myobj2->more;}
                          if (x<4){
                            x=x>>1;
                            x=x<<1;
                            x=x&2;
                            myobj2->type=x2+((x+4)*1024);
                            myobj3->type=x2+((x+5)*1024);
                            OBJcheckflags(myobj2->x,myobj2->y); OBJcheckflags(myobj3->x,myobj3->y);
                          }else{
                            x=x>>1;
                            x=x<<1;
                            x=x&2;
                            myobj2->type=x2+(x*1024);
                            myobj3->type=x2+((x+1)*1024);
                            OBJcheckflags(myobj2->x,myobj2->y); OBJcheckflags(myobj3->x,myobj3->y);
                          } //<4

                          x2=myobj2->x; y2=myobj2->y;
                          i3=SFnew(x2,y2);
                          //i3 points to new sf
                          sf[i3].type=4;
                          sf[i3].x=x2;
                          sf[i3].y=y2;
                          sf[i3].wait=1.0f; //NULL
                          sf[i3].more=2;

                          tnpc->wait_walk+=0.0625f;
                          tnpc->wait_walk+=(rnd*0.125f);
                        }
                      }
                    }//close door
                  }//!=NULL

                  if (x3){ //blocked
                    myobj2=myobj;
                    if (myobj3=OBJfindlast(myobj2->x+x2,myobj2->y+y2)){

                      //nonplayernpc->horse swap
                      if ((myobj3->type&1023)==431){
                        if (myobj3->info&8){//S->H(back)
                          myobj4=(object*)myobj3->more;
                          tnpc2=(npc*)myobj4->more;
                          x3=myobj4->x; y3=myobj4->y;
                          x4=0; y4=0;
                          //check adjacent squares of horse's head for walkability
                          if (bt[y3-1][x3]&1024){//walkable
                            x4=x3; y4=y3-1;
                          }
                          if (bt[y3][x3+1]&1024){//walkable
                            if ((x4==0)&&(y4==0)){x4=x3+1; y4=y3;}
                            x5=rnd*2; if (x5){x4=x3+1; y4=y3;}
                          }
                          if (bt[y3+1][x3]&1024){//walkable
                            if ((x4==0)&&(y4==0)){x4=x3; y4=y3+1;}
                            x5=rnd*2; if (x5){x4=x3; y4=y3+1;}
                          }
                          if (bt[y3][x3-1]&1024){//walkable
                            if ((x4==0)&&(y4==0)){x4=x3-1; y4=y3;}
                            x5=rnd*2; if (x5){x4=x3-1; y4=y3;}
                          }
                          if (x4||y4){
                            OBJmove_allow=TRUE;
                            OBJmove2(myobj2,myobj3->x,myobj3->y);
                            OBJmove2(myobj4,x4,y4);
                            OBJmove_allow=FALSE;
                            goto doneswap2;
                          }
                          //if no adjacent square is available then move on top of the back of the horse
                          OBJmove_allow=TRUE;
                          OBJmove2(myobj2,myobj3->x,myobj3->y);
                          OBJmove_allow=FALSE;
                          goto doneswap2;
                        }
                        //else S->H(front)
                        //S->H(front)
                        myobj4=myobj3;
                        tnpc2=(npc*)myobj4->more;
                        x3=((object*)tnpc2->more)->x; y3=((object*)tnpc2->more)->y;
                        //if (x3==0) goto badhorseswap;
                        x4=0; y4=0;
                        //check adjacent squares of horse's head for walkability
                        if (bt[y3-1][x3]&1024){//walkable
                          x4=x3; y4=y3-1;
                        }
                        if (bt[y3][x3+1]&1024){//walkable
                          if ((x4==0)&&(y4==0)){x4=x3+1; y4=y3;}
                          x5=rnd*2; if (x5){x4=x3+1; y4=y3;}
                        }
                        if (bt[y3+1][x3]&1024){//walkable
                          if ((x4==0)&&(y4==0)){x4=x3; y4=y3+1;}
                          x5=rnd*2; if (x5){x4=x3; y4=y3+1;}
                        }
                        if (bt[y3][x3-1]&1024){//walkable
                          if ((x4==0)&&(y4==0)){x4=x3-1; y4=y3;}
                          x5=rnd*2; if (x5){x4=x3-1; y4=y3;}
                        }
                        if (x4||y4){
                          OBJmove_allow=TRUE;
                          OBJmove2(myobj2,myobj4->x,myobj4->y);
                          OBJmove2(myobj4,x3,y3); OBJmove2(myobj4,x4,y4);
                          OBJmove_allow=FALSE;
                          goto doneswap2;
                        }
                        //if no adjacent square is available a regular swap will occur...
                      }//horse swap
                      //badhorseswap:

                      if (myobj3->info&(2+4)){ //swap with npc/crt

                        if (tnpc->converse==130){ //smith
                          tnpc->wait_walk+=1.0f+rnd*2.0f; //wait to reduce code overhead
                          tnpc->pathn=0; //path was blocked but another path may exist!
                          goto NPC_skip1;
                        }

                        //if ((myobj2->type&1023)==354){ //mouse
                        if (myobj4=OBJfindlastall(myobj2->x,myobj2->y)){
                          if ((myobj4->type&1023)==213) goto NPC_skip1; //mousehole
                        }
                        if (myobj4=OBJfindlastall(myobj2->x,myobj2->y)){
                          if (myobj4=(object*)myobj4->prev){
                            if (((myobj4->type&1023)==310)&&(myobj4->type&1024)) goto NPC_skip1; //portcullis
                          }
                        }
                        if (myobj4=OBJfindlastall(myobj2->x+x2,myobj2->y+y2)){
                          if ((myobj4->type&1023)==213) goto NPC_skip1; //mousehole
                        }
                        if (myobj4=OBJfindlastall(myobj2->x+x2,myobj2->y+y2)){
                          if (myobj4=(object*)myobj4->prev){
                            if (((myobj4->type&1023)==310)&&(myobj4->type&1024)) goto NPC_skip1; //portcullis
                          }
                        }
                        //}//mouse




                        //swap with NPC
                        x2=myobj2->x; y2=myobj2->y;
                        x3=myobj3->x; y3=myobj3->y;



                        //if ((abs(myobj->x-tnpc->schedule[x2].x)==0)&&(abs(myobj->y-tnpc->schedule[x2].y)==0)){
                        //tnpc->schedule_i=x2;
                        //if (schedule_i==0x91){


                        x4=1; //bedswap2

                        if (myobj3->info&2){
                          tnpc2=(npc*)myobj3->more;
                          if (tnpc2->schedule){ if (tnpc2->schedule_i!=-1){ if (tnpc2->schedule[tnpc2->schedule_i].type==0x91){ x4=0;}}}
                          if (tnpc2->converse==130) x4=0; //smith (other npc cannot swap with smith)
                        }

                        if (x4){


                          //myobj4=od[y3][x3]; if ((myobj4->type&1023)!=163){ //bedswap

                          OBJmove_allow=TRUE;
                          OBJmove2(myobj2,x3,y3);
                          OBJmove2(myobj3,x2,y2);
                          OBJmove_allow=FALSE;

doneswap2:

                          tnpc->wait_walk+=0.0625f; //regular move minimum wait
                          //tnpc->wait+=(rnd*0.125f);
                          tnpc->wait_walk+=(rnd*1.0f); //+swap wait
                          //tnpc->pathn++; //failed to move

                          if (myobj3->info&2) tnpc2->pathn=0; //other NPC's path needs updating

                          goto NPC_skip1;

                        } //bedswap2
                      }//<-npc
                      //}
                    }//!=NULL


                    //blocked! by a passthrough? ->use
                    myobj2=OBJfindlast(myobj->x+x2,myobj->y+y2);
                    if (myobj2!=NULL){
                      x3=myobj2->type;
                      //if ((x2==278)||(x2==280)){

                      if (x3==OBJ_PASSTHROUGH_V){ //v-passthrough
                        if (myobj2->more==NULL){
                          myobj2->more=OBJnew();
                          myobj3=(object*)myobj2->more;
                          myobj3->more=myobj2; //back link
                          myobj3->type=OBJ_PASSTHROUGH_V+1*1024;
                          OBJadd(myobj2->x-1,myobj2->y-1,myobj3);
                        }
                        else
                        {
                          OBJadd(myobj2->x-1,myobj2->y-1,(object*)myobj2->more);
                        }
                        x=myobj2->x;
                        y=myobj2->y-1;
                        OBJremove(myobj2);
                        myobj2->type=OBJ_PASSTHROUGH_V+2*1024;
                        OBJadd(x,y,myobj2);

                        tnpc->wait_walk+=0.0625f;
                        tnpc->wait_walk+=(rnd*0.125f);
                        tnpc->pathn++; //failed to move
                        goto NPC_skip1;
                      }//278


                      if (x3==OBJ_PASSTHROUGH_H){ //h-passthrough
                        if (myobj2->more==NULL){
                          myobj2->more=OBJnew();
                          myobj3=(object*)myobj2->more;
                          myobj3->more=myobj2; //back link
                          myobj3->type=OBJ_PASSTHROUGH_H+1*1024;
                          OBJadd(myobj2->x-1,myobj2->y-1,myobj3);
                        }
                        else
                        {
                          OBJadd(myobj2->x-1,myobj2->y-1,(object*)myobj2->more);
                        }
                        x=myobj2->x-1;
                        y=myobj2->y;
                        OBJremove(myobj2);
                        myobj2->type=OBJ_PASSTHROUGH_H+2*1024;
                        OBJadd(x,y,myobj2);

                        tnpc->wait_walk+=0.0625f;
                        tnpc->wait_walk+=(rnd*0.125f);
                        tnpc->pathn++; //failed to move
                        goto NPC_skip1;

                      }//280


                    }//myobj2



                    //blocked! by a door? ->use
                    myobj2=OBJfindlast(myobj->x+x2,myobj->y+y2);
                    if (myobj2!=NULL){
                      x3=myobj2->type&1023;
                      if ((x3>=297)&&(x3<=300)){
                        x2=x3;
                        //assume not locked
                        x=myobj2->type>>10;
                        if ((myobj2->type&1024)){//ignore the secondary door parts
                          if (x<8){
                            if (x&1) {myobj3=myobj2; myobj2=(object*)myobj2->more;} else {myobj3=(object*)myobj2->more;}
                            if (x<4){
                              x=x>>1;
                              x=x<<1;
                              x=x&2;
                              myobj2->type=x2+((x+4)*1024);
                              myobj3->type=x2+((x+5)*1024);
                              OBJcheckflags(myobj2->x,myobj2->y); OBJcheckflags(myobj3->x,myobj3->y);
                            }else{
                              x=x>>1;
                              x=x<<1;
                              x=x&2;
                              myobj2->type=x2+(x*1024);
                              myobj3->type=x2+((x+1)*1024);
                              OBJcheckflags(myobj2->x,myobj2->y); OBJcheckflags(myobj3->x,myobj3->y);
                            } //<4

                            x2=myobj2->x; y2=myobj2->y;
                            i3=SFnew(x2,y2);
                            //i3 points to new sf
                            sf[i3].type=4;
                            sf[i3].x=x2;
                            sf[i3].y=y2;
                            sf[i3].wait=1.0f; //NULL
                            sf[i3].more=1;

                            tnpc->wait_walk+=0.0625f;
                            tnpc->wait_walk+=(rnd*0.125f);
                            tnpc->pathn++; //failed to move
                            goto NPC_skip1;
                          }//x2<8
                        }
                      }
                    }


                    tnpc->wait_walk+=1.0f+rnd*2.0f; //wait to reduce code overhead
                    tnpc->pathn=0; //path was blocked but another path may exist!
                    goto NPC_skip1;
                  } //door

                  //f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                  //tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);

                  //npc walk speed
                  //tnpc->wait_walk+=0.0625f;
                  //tnpc->wait_walk+=(rnd*0.125f);

                  tnpc->wait_walk+=(0.1875f+(rnd*0.0625f));
                  //tnpc->wait_walk+=(0.25f+(rnd*0.125f));

                  goto NPC_skip1;
                } //order2

              } //waitOK
              goto NPC_skip1;
            }

            //have valid tplayer pointer! myobj=our object!
            //if (tplayer->party[tplayer->selected_partymember]!=myobj){


            //tplayer is valid



            static unsigned char horse_movediagonal;
            static short horse_movediagonalx,horse_movediagonaly;
            horse_movediagonal=0;

            //dv++;
            //dv=tnpc->wait_walk;
            //dv2=et/32.0f;
            //tnpc->wait_walk-=(et/32.0f);
            //if (tnpc->wait_walk<0) tnpc->wait_walk=0;

            static unsigned char selected_partymember;
            selected_partymember=0; if (tplayer->party[tplayer->selected_partymember]==myobj) selected_partymember=1;


            if ((selected_partymember)&&(keyon[VK_SPACE]&&U6O_DEBUG)) tnpc->wait_walk=0;




            //partymember npc
            //AUTOPICKUPadd(myobj,myobj3);
            //check surrounding squares for items to pickup


            if (tnpc->wait_disable==0.0f){
              if (tnpc->wait_attack==0.0f){

                for (y2=-1;y2<=1;y2++){ for (x2=-1;x2<=1;x2++){ if (x2||y2){
                  //autopickupcheckloop:
                  if (myobj3=OBJfindlast(myobj->x+x2,myobj->y+y2)){
                    if (AUTOPICKUP_OBJECTVALID[myobj3->type&1023]){


                      if (myobj4=AUTOPICKUPcheck(tplayer,myobj3)){
                        //MessageBox(NULL,"myobj4 could have been recovered","Ultima 6 Online",MB_OK);
                        OBJremove(myobj3);
                        tnpc3=(npc*)myobj4->more;

                        static object *invbag,*invobj;
                        invbag=tnpc3->items;
                        invobj=myobj3;

                        //inventory add
                        txtsetchar(t,2);//sfx message
                        txtaddshort(t,1);//number of sfx effects
                        txtaddshort(t,27);//inventory add
                        txtaddshort(t,myobj4->x);//x
                        txtaddshort(t,myobj4->y);//y
                        txtaddshort(t,myobj3->type);//graphic (x2)
                        txtaddshort(t,0);//unused placeholder for y2
                        txtaddshort(t,0);//unused placeholder for more
                        txtaddfloat(t,0.25f);//time to display message for (in this case half a second)
                        NET_send(NETplayer,tplayer->net,t);

                        tnpc3->upflags|=32;//inv

                        //join/combine items in inventory
                        if (obji[sprlnk[invobj->type&1023]+(invobj->type>>10)].flags&4096){//multiple/stackable
                          if (myobj5=(object*)invbag->more){
joinitem2:
                            if (myobj5->type==invobj->type){//same type
                              if ((myobj5->info&128)==(invobj->info&128)){
                                if (((invobj->type&1023)==90)&&(myobj5->info!=invobj->info)) goto cantjoin2;//cant join half burnt torches
                                myobj5->more2+=invobj->more2;
                                OBJrelease(invobj);
                                goto joinitem_finished2;
                              }//same stealing type
                            }//same type
                            if (myobj5->next) {myobj5=(object*)myobj5->next; goto joinitem2;}
                          }//myobj5=(object*)invbag->more
                        }//4096
cantjoin2:
                        invobj->info|=112; invobj->next=invbag->more; invobj->prev=invbag; invbag->more=invobj; if (invobj->next) ((object*)invobj->next)->prev=invobj;

                        //autoready item?
                        if (obji[sprlnk[invobj->type&1023]].flags&8){
                          //check for any two handed weapons!
                          if (myobj5=tnpc3->wep_left){
                            if (obji[sprlnk[myobj5->type&1023]].flags&16) goto cantautoready;
                          }
                          if (myobj5=tnpc3->wep_right){
                            if (obji[sprlnk[myobj5->type&1023]].flags&16) goto cantautoready;
                          }
                          //ready item if possible
                          if (tnpc3->wep_left==NULL){
                            OBJremove(invobj);
                            tnpc3->wep_left=invobj;
                            tnpc3->upflags|=64;
                            goto autoreadycomplete;
                          }
                          if (tnpc3->wep_right==NULL){
                            OBJremove(invobj);
                            tnpc3->wep_right=invobj;
                            tnpc3->upflags|=64;
                            goto autoreadycomplete;
                          }
                        }//autoready item
autoreadycomplete:
cantautoready:

joinitem_finished2:
                        tnpc->wait_attack+=0.25f;
                        goto autopickup_gotitem;
                      }
                    }//AUTOPICKUP_OBJECTVALID
                  }
                }}}
              }//wait_attack
            }//wait_disable
autopickup_gotitem:

            //wizard eye movement
            if (selected_partymember){
              if (tplayer->wizardeyetimeleft){
                x2=0; y2=0;
                if ((tplayer->key&KEYleft)||(tplayer->key&KEYleft2)) x2--;
                if ((tplayer->key&KEYright)||(tplayer->key&KEYright2)) x2++;
                if ((tplayer->key&KEYup)||(tplayer->key&KEYup2)) y2--;
                if ((tplayer->key&KEYdown)||(tplayer->key&KEYdown2)) y2++;
                if (tplayer->key&KEYdown2) tplayer->key-=KEYdown2; //clear dir2 key
                if (tplayer->key&KEYup2) tplayer->key-=KEYup2;
                if (tplayer->key&KEYleft2) tplayer->key-=KEYleft2;
                if (tplayer->key&KEYright2) tplayer->key-=KEYright2;

                if (tplayer->wizardeyemovewait==0.0f){
                  tplayer->wizardeyemovewait=0.1f/0.8f;

                  if (x2||y2){
                    x2+=tplayer->wizardeyex; y2+=tplayer->wizardeyey;
                    //map boundries check
                    if (x2>=1){ if (y2>=1){ if (x2<2047){ if (y2<1023){
                      x3=1;

                      if ((bt[y2][x2]&255)>=252) x3=0;//cannot enter void!

                      if (x2>=0){ if (y2>=0){ if (x2<1024){ if (y2<1024){//britannia mainland
                        goto wizeye_norestriction;
                      }}}}
                      if (x2>=1024){ if (y2>=256){ if (x2<(1024+256)){ if (y2<512){//garg lands
                        goto wizeye_norestriction;
                      }}}}

                      //restricted area checks
                      if ((btflags[bt[y2][x2]&1023]&(1+2+4+8))==0){
                        //1=land passabe
                        //2=air passable
                        //4=sea passable
                        //8=bolt passable
                        x3=0;
                      }
wizeye_norestriction:



                      if (x3){

                        if (myobj2=od[y2][x2]){
wizeye_laddercheck:
                          myobj3=myobj2; if ((myobj3->info&8)==8) myobj3=(object*)myobj3->more;//redirector
                          x4=myobj3->type&1023;
                          if ((x4==305)||(x4==326)||(x4==308)){ //ladder or dungeon or hole
                            x2=myobj3->more2&0xFFFF;
                            y2=myobj3->more2>>16;
                            goto wizeye_gotladder;
                          }
                          if (myobj2->next){myobj2=(object*)myobj2->next; goto wizeye_laddercheck;}
                        }
wizeye_gotladder:

                        tplayer->wizardeyex=x2; tplayer->wizardeyey=y2;
                      }//x3

                    }}}}
                  }//x2||y2
                }//f
                goto dontmove;
              }
            }





            if ((!tnpc->wait_walk)&&(!tnpc->wait_disable)){
              if (selected_partymember){ //selected party member


                x=0; x2=0; y2=0;

                if ((tplayer->key&KEYleft)||(tplayer->key&KEYleft2)) x2--;
                if ((tplayer->key&KEYright)||(tplayer->key&KEYright2)) x2++;
                if ((tplayer->key&KEYup)||(tplayer->key&KEYup2)) y2--;
                if ((tplayer->key&KEYdown)||(tplayer->key&KEYdown2)) y2++;

                if (tplayer->key&KEYdown2) tplayer->key-=KEYdown2; //clear dir2 key
                if (tplayer->key&KEYup2) tplayer->key-=KEYup2;
                if (tplayer->key&KEYleft2) tplayer->key-=KEYleft2;
                if (tplayer->key&KEYright2) tplayer->key-=KEYright2;



                if (tnpc->horse&&(tplayer->craft==NULL)){//horse diagonal movement

                  //1. precheck all three related squares
                  if (x2&&y2){
                    x3=myobj->x+x2; y3=myobj->y+y2;
                    static unsigned char xok,yok,xyok; xok=0; yok=0; xyok=0;



                    if (bt[y3][x3]&1024){
                      //walkable
                      xyok=1;
                    }else{
                      //not walkable
                      //check if there is an npc to swap with
                      if (myobj3=OBJfindlast(x3,y3)){
                        if (myobj3->info&8) myobj3=(object*)myobj3->more;
                        if (myobj3->info&2) xyok=1;
                      }//myobj3
                    }//xy

                    if (bt[myobj->y][x3]&1024){
                      //walkable
                      xok=1;
                    }else{
                      //not walkable
                      //check if there is an npc to swap with
                      if (myobj3=OBJfindlast(x3,myobj->y)){
                        if (myobj3->info&8) myobj3=(object*)myobj3->more;
                        if (myobj3->info&2) xok=1;
                      }//myobj3
                    }//x

                    if (bt[y3][myobj->x]&1024){
                      //walkable
                      yok=1;
                    }else{
                      //not walkable
                      //check if there is an npc to swap with
                      if (myobj3=OBJfindlast(myobj->x,y3)){
                        if (myobj3->info&8) myobj3=(object*)myobj3->more;
                        if (myobj3->info&2) yok=1;
                      }//myobj3
                    }//y

                    if (xyok){
                      if (xok){
                        horse_movediagonal=1; horse_movediagonalx=0; horse_movediagonaly=y2;
                        y2=0;//mask y (it will be done on the second move)
                        goto horsemovepossible;
                      }

                      if (yok){
                        horse_movediagonal=1; horse_movediagonalx=x2; horse_movediagonaly=0;
                        x2=0;//mask x (it will be done on the second move)
                        goto horsemovepossible;
                      }
                      x2=0; y2=0;
                    }

                    if (xyok==0){
                      //check if horse can slide along a wall
                      if (yok==1){
                        x2=0;
                        goto horsemovepossible;
                      }
                      if (xok==1){
                        y2=0;
                        goto horsemovepossible;
                      }
                      x2=0; y2=0;
                    }

                    //OLD CODE: if (x2&&y2){x2=0; y2=0;} //can't move diagonally on a horse
                  }//diagonal movement
                }//horse


horsemovepossible:
horse_finishdiagonalmove:

                if (x2||y2){
                  if (((myobj->x+x2)==923)&&((myobj->y+y2)==872)){ //416 sacred quest
                    static txt *txttmp1;

                    if (!tplayer->GNPCflags[17]){
                      if (!tplayer->guardianstatuewait){
                        txttmp1=txtnew();
                        txtset(txttmp1,"\x022Thou art not upon a Sacred Quest!\x022\\\x022Passage denied!\x022");
                        i3=SFnew(923,872); //i3 points to new sf
                        sf[i3].type=3; //message to user
                        sf[i3].x=923;
                        sf[i3].y=872-2;
                        sf[i3].wait=5;
                        sf[i3].more=187;
                        sf[i3].p=txttmp1;
                        tplayer->guardianstatuewait=8;
                      }
                      goto dontmove;
                    }
                    //sacred quest
                    x2=923;
                    if (y2==-1) y2=871;
                    if (y2==1) y2=873;
                    if (tplayer->craft==NULL){
                      //for (i4=7;i4>=0;i4--){
                      //if (tplayer->party[i4]){
                      //OBJremove(tplayer->party[i4]);
                      //OBJadd(x2,y2,tplayer->party[i4]);
                      //}
                      //}
                      partyadd(tplayer,x2,y2);

                      if (!tplayer->guardianstatuewait){
                        txttmp1=txtnew();
                        txtset(txttmp1,"\x022Passage granted!\x022");
                        i3=SFnew(923,872); //i3 points to new sf
                        sf[i3].type=3; //message to user
                        sf[i3].x=923;
                        sf[i3].y=872-2;
                        sf[i3].wait=5;
                        sf[i3].more=187;
                        sf[i3].p=txttmp1;
                        tplayer->guardianstatuewait=8;
                      }

                    }//craft
                    goto dontmove;
                  }//x,y
                }//x2||y2

                if (tplayer->pathfind){
                  tplayer->pathfind=0;
                  if ((x2==0)&&(y2==0)){
                    tplayer->pathfind=1;
                    //events which should stop a pathfind:
                    //i. switch party members
                    //ii. party member dies
                    //iii. path becomes blocked
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFright) x2=1;
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFleft) x2=-1;
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFup) y2=-1;
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFdown) y2=1;
                    if (tnpc->pathn>0) { //fix for pathfinding crash.
                      tnpc->pathn--;
                    }
                    if (tnpc->pathn==0) tplayer->pathfind=0;
                  }//x2==0,y2==0
                }//pathfind

                //BALLOON OVERRIDE!
                if ((tplayer->party[0]->type&1023)==423){
                  //free flying with fan
                  if (tplayer->mobj && tplayer->mobj->type==212) {
                    goto selected_partymember_move;
                  }
                  x2=windx; y2=windy;
                  if ((tplayer->windx)||(tplayer->windy)){
                    x2=tplayer->windx; y2=tplayer->windy;
                  }
                }
                x3=tplayer->party[0]->x; y3=tplayer->party[0]->y;
                if ((abs(x3-(myobj->x+x2))<=8)&&(abs(y3-(myobj->y+y2))<=8)){
                  if ((x2!=0)||(y2!=0)) goto selected_partymember_move;
                }
              } //selected party member

              if (tnpc->order==1){ //try to move towards avatar
                if (myobj2=tnpc->follow){
                  x2=0; y2=0;
                  x3=myobj->x-myobj2->x; y3=myobj->y-myobj2->y;
                  if (abs(x3)>abs(y3)) x4=abs(x3); else x4=abs(y3);//x4=distance from selected partymember
                  if (x4<=1) goto dontmove;
                  //get party order of npc
                  y5=1;
                  for (y4=0;y4<=7;y4++){ if (tplayer->party[y4]){
                    if (tplayer->party[y4]==myobj) goto follow_gotpartyorder;//<-GPF HERE (latest)
                    if (tnpc->follow!=tplayer->party[y4]){
                      y5++;
                    }
                  }}
follow_gotpartyorder:

                  if ((x4==2)&&(y5>=3)) goto dontmove;
                  if ((x4==3)&&(y5>=6)) goto dontmove;
                  /*
                  if (x4==3){
                  x4=rnd*2; if (x4) goto dontmove;
                  }
                  if (x4==2){
                  x4=rnd*3; if (x4) goto dontmove;
                  }
                  */

                  //get path
                  /*
                  WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,x2,y2,16,0,0);
                  if (WPF_RETURN==WPF_PATHFOUND){
                  if (WPF_PATHLENGTH){
                  tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;
                  playerlist[tpl]->pathfind=1; //use pathfinding formula!
                  goto doneclmess;
                  }
                  playerlist[tpl]->pathfind=0; tnpc->path_max=0; tnpc->pathn=0;
                  }
                  }//party
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFright) x2=1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFleft) x2=-1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFup) y2=-1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFdown) y2=1;
                  tnpc->pathn--;
                  */

                  if (tnpc->pathn){//use current path
follow_getpath:
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFright) x2=1;
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFleft) x2=-1;
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFup) y2=-1;
                    if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFdown) y2=1;
                    if (tnpc->pathn>0) { //fix for pathfinding crash.
                      tnpc->pathn--;
                    }
                    goto follow_gotpath;
                  }

                  z=abs(myobj2->x-myobj->x); z2=abs(myobj2->y-myobj->y); if (z2>z) z=z2;
                  z+=32; if (z>250) z=250;
                  WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,z,0,1);//|1|PATHFIND THROUGH OWN PARTY+16
                  if (WPF_RETURN==WPF_PATHFOUND){
                    if (WPF_PATHLENGTH){
                      tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;
                      goto follow_getpath;
                    }
                  }
                  //no path possible/(already at destination[can't be this value!])
                  tnpc->pathn=0; x2=0; y2=0;
                  tnpc->wait_walk+=0.5f;
                  goto donemove;

follow_gotpath:
                  //avoid hazards
                  z=abs(myobj2->x-myobj->x); z2=abs(myobj2->y-myobj->y); if ((z<=5)&&(z2<=5)){
                    z=wpf_dangercheck(myobj->x,myobj->y,myobj); z2=wpf_dangercheck(myobj->x+x2,myobj->y+y2,myobj); if (z2>z){x2=0; y2=0;}
                  }//z&&z2

                  //integrate player move and partymember->follow move
selected_partymember_move:
                  x3=0;

                  //wait! diagonal?!?
                  if ((abs(x2)+abs(y2))>1){ if (tplayer->craft==NULL){
                    if ((bt[myobj->y+y2][myobj->x]&1024)+(bt[myobj->y][myobj->x+x2]&1024)==0) goto NPC_skip1;
                  }}
                  if (OBJmove2(myobj,myobj->x+x2,myobj->y+y2)==0) x3=1;



                  if (x3==0){ //failed!


                    if ((abs(x2)+abs(y2))>1){ //diagonal
                      //attempt single movement along a plane
                      if (OBJmove2(myobj,myobj->x,myobj->y+y2)==0) {x3=1; x2=0; goto slidemove;}
                      if (OBJmove2(myobj,myobj->x+x2,myobj->y)==0) {x3=1; y2=0; goto slidemove;}
                    }
                  }
slidemove:



                  if (x3){


                    //if (tnpc->horse){


                    if (tnpc->horse&&(tplayer->craft==NULL)){

                      //wear down horseshoes?
                      x4=rnd*10000; if (x4==0){
                        crt2=(creature*)tnpc->horse->more;
                        x4=(crt2->respawn_x>>6)&3; if (x4) x4--;
                        crt2->respawn_x&=(0xFFFF-(3<<6));
                        crt2->respawn_x+=(x4<<6);
                      }//x4==0

                      //NEW horse wait
                      //8fps-16fps
                      //food 0-3 *2 0
                      //shoe 0-3 *2 0-6
                      //race 0-7(15) 0-8
                      //total 200.25->0.0625
                      //(0.25-0.0625=0.1875)
                      //(0.1875/20=0.009375)


                      crt2=(creature*)tnpc->horse->more; //this should fix the "all horse speeds same bug" and random crashes caused by uninitialized crt2
                      f=0.125;
                      f2=((crt2->respawn_x>>4)&3)*3;//food(0-15)
                      f2+=(((crt2->respawn_x>>6)&3)*3);//horseshoes(0-15)
                      f2+=(crt2->respawn_x&15);//races won(0-8) ***note: if horse races won>7 wait becomes less than 0.0625!***
                      f2*=0.00260417f;//0.0625/24
                      f-=f2;

                      if (horse_movediagonal==1) f*=1.4142f;
                      if (horse_movediagonal!=2) tnpc->wait_walk+=f;

                      goto wait_override;
                    }//->horse;

                    if ((myobj->type&1023)==423){ //balloon
                      f=0.25f;
                      if ((abs(x2)+abs(y2))>1) f*=1.4142f; //diagonal!
                      tnpc->wait_walk+=f;
                      goto wait_override;
                    }
                    if ((myobj->type&1023)==412){ //boat
                      //take into account wind direction & sea direction
                      //x2 and y2 are relative direction
                      f=0.0625f;
                      if ((abs(x2)+abs(y2))>1) f*=1.4142f; //diagonal!
                      tnpc->wait_walk+=f;
                      //additional current wait?


                      goto wait_override;
                    }

                    f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                    //f=(0.25f-0.0625f-f*0.125f); //pre Mar
                    f=(0.25f-0.0625f-f*0.125f);
                    f=f*0.875f;
                    if ((abs(x2)+abs(y2))>1) f*=1.4142f; //diagonal!
                    tnpc->wait_walk+=f;

                    //if (selected_partymember){
                    //f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                    //tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);
                    //}else{
                    ////tnpc->wait_walk+=(0.0625f+(rnd*0.125f));
                    //f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                    //tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);
                    //}

wait_override:;

                  }//x3




                  if ((x3==0)&&(tplayer->craft==NULL)){

                    //NPC is blocked! try swap
                    myobj2=myobj; //temp fix
                    myobj3=OBJfindlast(myobj2->x+x2,myobj2->y+y2);


                    if (myobj3){

                      //swapping into the back of a horse
                      if ((myobj3->type&1023)==431){


                        if (myobj3->info&8){
                          myobj4=(object*)myobj3->more;
                          tnpc2=(npc*)myobj4->more;
                          if ((tnpc2->player!=tnpc->player)||(selected_partymember)){
                            //if (tnpc->horse){
                            if (tnpc->horse&&(tplayer->craft==NULL)){

                              //MessageBox(NULL,"horse 2 horse swap!","Ultima 6 Online",MB_OK);

                              //swap both "types"
                              //invert both horse directions
                              x2=myobj2->x; y2=myobj2->y; z2=myobj2->type>>11;
                              x3=myobj4->x; y3=myobj4->y; z3=myobj4->type>>11;
                              OBJmove_allow=TRUE;

                              OBJmove2(myobj2,x3,y3);
                              z=z3; x4=0; y4=0;
                              if (z==0) y4=-1;
                              if (z==1) x4=1;
                              if (z==2) y4=1;
                              if (z==3) x4=-1;
                              OBJmove2(myobj2,myobj2->x+x4,myobj2->y+y4);
                              OBJmove2(myobj2,myobj2->x-x4*2,myobj2->y-y4*2);


                              OBJmove2(myobj4,x2,y2);
                              z=z2; x4=0; y4=0;
                              if (z==0) y4=-1;
                              if (z==1) x4=1;
                              if (z==2) y4=1;
                              if (z==3) x4=-1;
                              OBJmove2(myobj4,myobj4->x+x4,myobj4->y+y4);
                              OBJmove2(myobj4,myobj4->x-x4*2,myobj4->y-y4*2);

                              OBJmove_allow=FALSE;
                              goto doneswap;
                            }else{
                              //S->H(back)
                              x3=myobj4->x; y3=myobj4->y;
                              x4=0; y4=0;
                              //check adjacent squares of horse's head for walkability
                              if (bt[y3-1][x3]&1024){//walkable
                                x4=x3; y4=y3-1;
                              }
                              if (bt[y3][x3+1]&1024){//walkable
                                if ((x4==0)&&(y4==0)){x4=x3+1; y4=y3;}
                                x5=rnd*2; if (x5){x4=x3+1; y4=y3;}
                              }
                              if (bt[y3+1][x3]&1024){//walkable
                                if ((x4==0)&&(y4==0)){x4=x3; y4=y3+1;}
                                x5=rnd*2; if (x5){x4=x3; y4=y3+1;}
                              }
                              if (bt[y3][x3-1]&1024){//walkable
                                if ((x4==0)&&(y4==0)){x4=x3-1; y4=y3;}
                                x5=rnd*2; if (x5){x4=x3-1; y4=y3;}
                              }
                              if (x4||y4){
                                OBJmove_allow=TRUE;
                                OBJmove2(myobj2,myobj3->x,myobj3->y);
                                OBJmove2(myobj4,x4,y4);
                                OBJmove_allow=FALSE;
                                goto doneswap;
                              }
                              //if no adjacent square is available then move on top of the back of the horse
                              OBJmove_allow=TRUE;
                              OBJmove2(myobj2,myobj3->x,myobj3->y);
                              OBJmove_allow=FALSE;
                              goto doneswap;
                            }//->horse
                          }//not in same party or is leader
                          goto NPC_skip1;
                        }//&8

                        //swap with head of a horse
                        myobj4=myobj3;
                        tnpc2=(npc*)myobj4->more;
                        //if (((object*)(tnpc2->more))->x==0) goto NPC_skip1;//!

                        if ((tnpc2->player!=tnpc->player)||(selected_partymember)){
                          //if (tnpc->horse){
                          if (tnpc->horse&&(tplayer->craft==NULL)){

                            //swap both "types"
                            //invert both horse directions
                            x2=myobj2->x; y2=myobj2->y; z2=myobj2->type>>11;
                            x3=myobj4->x; y3=myobj4->y; z3=myobj4->type>>11;
                            OBJmove_allow=TRUE;

                            OBJmove2(myobj2,x3,y3);
                            z=z3; x4=0; y4=0;
                            if (z==0) y4=-1;
                            if (z==1) x4=1;
                            if (z==2) y4=1;
                            if (z==3) x4=-1;
                            OBJmove2(myobj2,myobj2->x-x4,myobj2->y-y4);
                            OBJmove2(myobj2,myobj2->x+x4,myobj2->y+y4);

                            OBJmove2(myobj4,x2,y2);
                            z=z2; x4=0; y4=0;
                            if (z==0) y4=-1;
                            if (z==1) x4=1;
                            if (z==2) y4=1;
                            if (z==3) x4=-1;
                            OBJmove2(myobj4,myobj4->x-x4,myobj4->y-y4);
                            OBJmove2(myobj4,myobj4->x+x4,myobj4->y+y4);

                            OBJmove_allow=FALSE;
                            goto doneswap;
                          }else{
                            //S->H(front)
                            x3=((object*)tnpc2->more)->x; y3=((object*)tnpc2->more)->y;
                            //if (x3==0) goto NPC_skip1;//!
                            x4=0; y4=0;
                            //check adjacent squares of horse's head for walkability
                            if (bt[y3-1][x3]&1024){//walkable
                              x4=x3; y4=y3-1;
                            }
                            if (bt[y3][x3+1]&1024){//walkable
                              if ((x4==0)&&(y4==0)){x4=x3+1; y4=y3;}
                              x5=rnd*2; if (x5){x4=x3+1; y4=y3;}
                            }
                            if (bt[y3+1][x3]&1024){//walkable
                              if ((x4==0)&&(y4==0)){x4=x3; y4=y3+1;}
                              x5=rnd*2; if (x5){x4=x3; y4=y3+1;}
                            }
                            if (bt[y3][x3-1]&1024){//walkable
                              if ((x4==0)&&(y4==0)){x4=x3-1; y4=y3;}
                              x5=rnd*2; if (x5){x4=x3-1; y4=y3;}
                            }
                            if (x4||y4){
                              OBJmove_allow=TRUE;
                              OBJmove2(myobj2,myobj4->x,myobj4->y);
                              OBJmove2(myobj4,x3,y3); OBJmove2(myobj4,x4,y4);
                              OBJmove_allow=FALSE;
                              goto doneswap;
                            }
                            //if no adjacent square is available a regular swap will occur...
                          }//->horse
                        }//not in same party or is leader
                      }//431







                      if (myobj3->info&2){ //<-npc
                        tnpc2=(npc*)myobj3->more;


                        if (tnpc2->player){
                          if (tnpc2->player->craft) goto NPC_skip1;
                        }

                        if (tnpc2->converse==128) goto NPC_skip1; //pushmepullyou
                        if (tnpc2->converse==130) goto NPC_skip1; //smith
                        if (tnpc2->converse==187) goto NPC_skip1;
                        if (tnpc2->converse==188) goto NPC_skip1;
                        if (tnpc2->converse==189) goto NPC_skip1;

                        if ((tnpc2->player!=tnpc->player)||(selected_partymember)){


                          //if ((myobj2->type&1023)==354){ //mouse
                          if (myobj4=OBJfindlastall(myobj2->x,myobj2->y)){
                            if ((myobj4->type&1023)==213) goto NPC_skip1; //mousehole
                          }
                          if (myobj4=OBJfindlastall(myobj2->x,myobj2->y)){
                            if (myobj4=(object*)myobj4->prev){
                              if (((myobj4->type&1023)==310)&&(myobj4->type&1024)) goto NPC_skip1; //portcullis
                            }
                          }
                          if (myobj4=OBJfindlastall(myobj2->x+x2,myobj2->y+y2)){
                            if ((myobj4->type&1023)==213) goto NPC_skip1; //mousehole
                          }
                          if (myobj4=OBJfindlastall(myobj2->x+x2,myobj2->y+y2)){
                            if (myobj4=(object*)myobj4->prev){
                              if (((myobj4->type&1023)==310)&&(myobj4->type&1024)) goto NPC_skip1; //portcullis
                            }
                          }


                          x2=myobj2->x; y2=myobj2->y;
                          x3=myobj3->x; y3=myobj3->y;




                          //}//mouse

                          //swap with NPC

                          //myobj4=od[y3][x3];
                          //if ((myobj4->type&1023)!=163){
                          //tnpc2=(npc*)myobj3->more;
                          x4=1; //bedswap2
                          if (tnpc2->schedule){ if (tnpc2->schedule_i!=-1){ if (tnpc2->schedule[tnpc2->schedule_i].type==0x91){ x4=0;}}}
                          if (x4){

                            //if (tnpc->horse){//player is on a horse, however target is not! (or it would have already been checked)
                            if (tnpc->horse&&(tplayer->craft==NULL)){
                              //CHECKED: can't swap diagonally
                              z=myobj2->type>>11; x4=0; y4=0;
                              if (z==0) y4=-1;
                              if (z==1) x4=1;
                              if (z==2) y4=1;
                              if (z==3) x4=-1;
                              x2-=x4; y2-=y4;
                              OBJmove_allow=TRUE;
                              OBJmove2(myobj2,x3,y3);
                              OBJmove2(myobj3,x2,y2);
                              OBJmove_allow=FALSE;
                              goto doneswap;
                            }


                            OBJmove_allow=TRUE;
                            OBJmove2(myobj2,x3,y3);
                            OBJmove2(myobj3,x2,y2);
                            OBJmove_allow=FALSE;


doneswap:
                            if (selected_partymember) tnpc->wait_walk+=0.125f; else tnpc->wait_walk+=(rnd*1.0f);
                            //***************

                            if (horse_movediagonal==1) {horse_movediagonal=2; x2=horse_movediagonalx; y2=horse_movediagonaly; goto horse_finishdiagonalmove;}


                            //tnpc->pathn++; //failed to move
                            //error: other characters path has been altered
                            tnpc2->pathn=0;
                            goto NPC_skip1;

                          }//bedswap
                        }//not in party
                      }//<-npc
                    }//!=NULL



                    //blocked! by a door? ->use
                    if (selected_partymember==0){
                      myobj2=OBJfindlast(myobj->x+x2,myobj->y+y2);
                      if (myobj2!=NULL){
                        x2=myobj2->type&1023;
                        if ((x2>=297)&&(x2<=300)){
                          if ((myobj2->type>>10)<8){//check if locked!
                            if ((myobj2->type&1024)){//ignore the secondary door parts
                              x=myobj2->type>>10;
                              if (x&1) {myobj3=myobj2; myobj2=(object*)myobj2->more;} else {myobj3=(object*)myobj2->more;}
                              if (x<4){
                                x=x>>1;
                                x=x<<1;
                                x=x&2;
                                myobj2->type=x2+((x+4)*1024);
                                myobj3->type=x2+((x+5)*1024);
                                OBJcheckflags(myobj2->x,myobj2->y); OBJcheckflags(myobj3->x,myobj3->y);
                              }else{
                                x=x>>1;
                                x=x<<1;
                                x=x&2;
                                myobj2->type=x2+(x*1024);
                                myobj3->type=x2+((x+1)*1024);
                                OBJcheckflags(myobj2->x,myobj2->y); OBJcheckflags(myobj3->x,myobj3->y);
                              } //<4

                              x2=myobj2->x; y2=myobj2->y;
                              i3=SFnew(x2,y2);
                              //i3 points to new sf
                              sf[i3].type=4;
                              sf[i3].x=x2;
                              sf[i3].y=y2;
                              sf[i3].wait=1.0f; //NULL
                              sf[i3].more=1;

                              f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                              tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);
                              tnpc->pathn++; //failed to move
                              goto donemove;
                            }
                          }
                        }
                      }
                    }//end (blocked by door)

                    tnpc->pathn=0; if (selected_partymember) tplayer->pathfind=0;
dontmove:
                    if (!selected_partymember) tnpc->wait_walk+=0.5f;
                  }//x3==0 (blocked)

                  if (horse_movediagonal==1) {horse_movediagonal=2; x2=horse_movediagonalx; y2=horse_movediagonaly; goto horse_finishdiagonalmove;}


                }//tnpc->follow
                goto donemove;
              }//order==1 (follow)
            }//wait_walk

            if (tnpc->order==3){ //attack target, if target==NULL find target
				//txtset(t, "ATTACK!"); LOGadd(t); // t222

              if (tnpc->target==NULL){ //chk vlnk
                if (tplayer->party[tplayer->selected_partymember]==myobj) tnpc->order=0; else tnpc->order=1;
                tnpc->follow=tplayer->party[tplayer->selected_partymember]; goto skipnpcattack;
              }

              static unsigned char spikedhelm_attack;
              static unsigned char spikedhelm_onlyattack;
              static object *spikedhelm;
              spikedhelm_attack=0;
              spikedhelm_onlyattack=0;
              if (spikedhelm=tnpc->helm){
                if (spikedhelm->type==4){
                  if (tplayer->craft==NULL){
                    spikedhelm_attack=1;
                  }
                }
              }

              myobj3=tnpc->wep_left; myobj4=tnpc->wep_right;
              if (myobj3&&myobj4){ //object in both hands
                //can't attack with equiped items!
                if ((spikedhelm_attack==0)&&(obji[sprlnk[myobj3->type&1023]].v1==0)&&(obji[sprlnk[myobj4->type&1023]].v1==0)) goto skipnpcattack;

                if (spikedhelm_attack){
                  if ((obji[sprlnk[myobj3->type&1023]].v1==0)&&(obji[sprlnk[myobj4->type&1023]].v1==0)) spikedhelm_onlyattack=1;
                }

              }

              static unsigned char attack_item; attack_item=FALSE;
              myobj2=tnpc->target; if (tnpc->target->info&4) crt=(creature*)myobj2->more; else attack_item=TRUE;

              if (tnpc->wait_disable) goto skipnpcattack; //can't move or attack!

              if (tnpc->wait_attack){
                if (selected_partymember) goto skipnpcattack; //movement is manual!
                if (tnpc->wait_walk) goto skipnpcattack; //can't move closer anyway!
                //i. check minimum range of all equipped weapons
                x2=65536; //distance required to attack (-1=can't attack!)
                for (x3=0;x3<=1;x3++){
                  if (x3==0) myobj3=tnpc->wep_right; //right hand weapon
                  if (x3==1) myobj3=tnpc->wep_left; //left hand weapon
                  if (myobj3){
                    if (obji[sprlnk[myobj3->type&1023]].v1) x4=1; else x4=65536;
                    x5=myobj3->type&1023;
                    if (x5==OBJ_BOW) x4=8;
                    if (x5==OBJ_MAGIC_BOW) x4=8;
                    if (x5==OBJ_TRIPLE_CROSSBOW) x4=8;
                    if (x5==OBJ_SLING) x4=4;
                    if (x5==OBJ_LIGHTNING_WAND) x4=8;
                    if (x5==OBJ_FIRE_WAND) x4=8;
                    if (x5==OBJ_CROSSBOW) x4=8;
                    if (x5==OBJ_BOOMERANG) x4=8;
                    if (x5==OBJ_FLASK_OF_OIL) x4=8;
                    if (x5==OBJ_DAGGER) x4=8;
                    if (x5==OBJ_THROWING_AXE) x4=8;
                    if (x5==OBJ_SPEAR) x4=8;
                    if (x5==OBJ_HALBERD) x4=2;//halberd
                    if (x5==OBJ_MORNING_STAR) x4=2;//morning star
                    if (obji[sprlnk[myobj3->type&1023]].flags&16){
                      x2=x4; goto npcattack2hnd;
                    }
                    if (x4<x2) x2=x4;
                  }else{
                    //bare hands attack possible, close in on target
                    x2=1;
                  }
                }//x3
npcattack2hnd:
                if (spikedhelm_attack) x2=1;//set minimum range required to use weapon
                if (tplayer->craft) x2=8;

                //ii. if distance 2 target is more than minimum range close in on target
                x=abs(myobj->x-myobj2->x); y=abs(myobj->y-myobj2->y);
                if (x>y) x4=x; else x4=y; //x4=distance to target
                if ((x4>x2)&&(x2!=65536)) goto npcattackclose;
                goto skipnpcattack; //doesn't need to move and can't attack!
              }//tnpc->wait_attack



              //if (crt->hp<=0){
              //tnpc->order=1; tnpc->follow=tplayer->party[tplayer->selected_partymember]; goto skipnpcattack;
              //}

              //check distance?
              x=abs(myobj->x-myobj2->x);
              y=abs(myobj->y-myobj2->y);
              if (x>y) x2=x; else x2=y; //x2=distance to target


              //if ((x<=1)&&(y<=1)) x2=1; //close


              //add l&r hand dam
              x=0; x9=0;

              static unsigned char miss;
              static unsigned char wep_dt[2];
              wep_dt[0]=0; wep_dt[1]=0;
              //RESERVED! x,x9
              miss=0;


              static unsigned char triplecrossbow_shot;
              triplecrossbow_shot=0;
              static unsigned short triplecrossbow_shot_x2;
              static unsigned short triplecrossbow_shot_y2;
              static unsigned short triplecrossbow_shot_x3;
              static unsigned short triplecrossbow_shot_y3;

              static unsigned short npcattackweptype;


              if (spikedhelm_onlyattack){
                if (x2>1) goto attacknotpossible;//close in
                spikedhelm_attack=0; spikedhelm_onlyattack=0;//clear set variables
                myobj3=NULL;
                i3=1;
                wep_dt[1]=1;
                x=4;

                if (tplayer->craft==NULL) OBJdir(myobj,myobj2->x,myobj2->y);

                //add wait
                f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                f=2.0f-(f*0.25f);
                tnpc->wait_attack+=f;

                //add sfx
                i4=SFnew(myobj2->x,myobj2->y);
                sf[i4].type=1; //attack
                sf[i4].x=myobj2->x;
                sf[i4].y=myobj2->y;
                sf[i4].x2=0;
                sf[i4].wait=0.125f; //NULL
                sf[i4].more=1;

                goto spikedhelm_entrypoint;
              }else{
                if (spikedhelm_attack){
                  if (x2>1) spikedhelm_attack=0;//not within range
                }
              }

              for (x3=0;x3<=1;x3++){
                npcattackweptype=0;


                if (x3==0) myobj3=tnpc->wep_right; //right hand weapon
                if (x3==1) myobj3=tnpc->wep_left; //left hand weapon

                if (tplayer->craft){ //ship cannons
                  myobj3=NULL;
                  if (x3==0){ if (x2<=8){
                    //do a MAJOR jump into the attack section!!!
                    if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)) goto skip_hth1;
                    i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                    sf[i3].type=17;
                    sf[i3].x=myobj->x;
                    sf[i3].y=myobj->y;
                    sf[i3].x2=myobj2->x;
                    sf[i3].y2=myobj2->y;
                    sf[i3].more=0xFFFF;
                    sf[i3].wait=1;
                    x+=30;
                    goto skip_hth2;
                  }}
                  goto skip_hth1;
                }

                if (myobj3){
                  npcattackweptype=myobj3->type&1023;


                  if (x3) wep_dt[0]=obji[sprlnk[myobj3->type&1023]].v5; else wep_dt[1]=obji[sprlnk[myobj3->type&1023]].v5;


                  //special case(s)


                  if (x2<=8){ //long range

                    if ((myobj3->type&1023)==47){ //halberd
                      if (x2==2){//only check if range 2
                        if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                          //blocked! (will be processed as out of range)
                        }else{

                          x7=myobj2->x;
                          if (myobj->x>(myobj2->x+1)) x7++;
                          if (myobj->x<(myobj2->x-1)) x7--;
                          y7=myobj2->y;
                          if (myobj->y>(myobj2->y+1)) y7++;
                          if (myobj->y<(myobj2->y-1)) y7--;

                          if (myobj2->x!=x7){ //discrepancy x
                            x5=x7; y5=myobj2->y; x6=0;
                            if (myobj9=OBJfindlast(x5,y5)){
                              if (myobj9->info&8) myobj9=(object*)myobj9->more;
                              if (myobj9->info&(2+4)) x6=1; //something living is in the way
                            }//myobj9
                            if (bt[y5][x5]&1024) x6=1; //character can walk there
                            if (x6==0) goto halberd_failedcheck;
                          }

                          if (myobj2->y!=y7){ //discrepancy y
                            x5=myobj2->x; y5=y7; x6=0;
                            if (myobj9=OBJfindlast(x5,y5)){
                              if (myobj9->info&8) myobj9=(object*)myobj9->more;
                              if (myobj9->info&(2+4)) x6=1; //something living is in the way
                            }//myobj9
                            if (bt[y5][x5]&1024) x6=1; //character can walk there
                            if (x6==0) goto halberd_failedcheck;
                          }

                          x7=myobj2->x;
                          if (myobj->x>myobj2->x) x7++;
                          if (myobj->x<myobj2->x) x7--;
                          y7=myobj2->y;
                          if (myobj->y>myobj2->y) y7++;
                          if (myobj->y<myobj2->y) y7--;

                          if ((x7!=myobj2->x)&&(y7!=myobj2->y)){
                            x5=x7; y5=y7; x6=0;
                            if (myobj9=OBJfindlast(x5,y5)){
                              if (myobj9->info&8) myobj9=(object*)myobj9->more;
                              if (myobj9->info&(2+4)) x6=1; //something living is in the way
                            }//myobj9
                            if (bt[y5][x5]&1024) x6=1; //character can walk there
                            if (x6==0) goto halberd_failedcheck;
                          }

                          goto hth_norangecheck;
halberd_failedcheck:;
                        }//OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)
                      }//==2
                    }//halberd

                    if ((myobj3->type&1023)==40){ //morning star
                      if (x2==2){//only check if range 2
                        if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                          //blocked! (will be processed as out of range)
                        }else{


                          x7=myobj2->x;
                          if (myobj->x>(myobj2->x+1)) x7++;
                          if (myobj->x<(myobj2->x-1)) x7--;
                          y7=myobj2->y;
                          if (myobj->y>(myobj2->y+1)) y7++;
                          if (myobj->y<(myobj2->y-1)) y7--;

                          if (myobj2->x!=x7){ //discrepancy x
                            x5=x7; y5=myobj2->y; x6=0;
                            if (myobj9=OBJfindlast(x5,y5)){
                              if (myobj9->info&8) myobj9=(object*)myobj9->more;
                              if (myobj9->info&(2+4)) x6=1; //something living is in the way
                            }//myobj9
                            if (bt[y5][x5]&1024) x6=1; //character can walk there
                            if (x6==0) goto morningstar_failedcheck;
                          }

                          if (myobj2->y!=y7){ //discrepancy y
                            x5=myobj2->x; y5=y7; x6=0;
                            if (myobj9=OBJfindlast(x5,y5)){
                              if (myobj9->info&8) myobj9=(object*)myobj9->more;
                              if (myobj9->info&(2+4)) x6=1; //something living is in the way
                            }//myobj9
                            if (bt[y5][x5]&1024) x6=1; //character can walk there
                            if (x6==0) goto morningstar_failedcheck;
                          }

                          x7=myobj2->x;
                          if (myobj->x>myobj2->x) x7++;
                          if (myobj->x<myobj2->x) x7--;
                          y7=myobj2->y;
                          if (myobj->y>myobj2->y) y7++;
                          if (myobj->y<myobj2->y) y7--;

                          if ((x7!=myobj2->x)&&(y7!=myobj2->y)){
                            x5=x7; y5=y7; x6=0;
                            if (myobj9=OBJfindlast(x5,y5)){
                              if (myobj9->info&8) myobj9=(object*)myobj9->more;
                              if (myobj9->info&(2+4)) x6=1; //something living is in the way
                            }//myobj9
                            if (bt[y5][x5]&1024) x6=1; //character can walk there
                            if (x6==0) goto morningstar_failedcheck;
                          }

                          goto hth_norangecheck;
morningstar_failedcheck:;
                        }//OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)
                      }//==2
                    }//morning star

                    if ((myobj3->type&1023)==41){ //bow
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                        OBJlist_last=NULL;
                        for(y6=0;y6<x5;y6++){
                          if (OBJlist_list[y6]->type==55){ //arrow
                            OBJlist_list[y6]->more2--;
                            if (OBJlist_list[y6]->more2==0) OBJremove(OBJlist_list[y6]);
                            tnpc->upflags|=32;
                            i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                            sf[i3].type=2; //arrow
                            sf[i3].x=myobj->x;
                            sf[i3].y=myobj->y;
                            sf[i3].x2=myobj2->x;
                            sf[i3].y2=myobj2->y;
                            sf[i3].more=0xFFFF;
                            sf[i3].wait=1;
                            x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                            if (x3) wep_dt[0]=3; else wep_dt[1]=3;
                            goto skip_hth2;
                          }//->type==55
                        }//y6
                        goto skipnpcattack;
                      }//OBJcheckbolt
                      goto skip_hth2;
                    }//bow

                    if ((myobj3->type&1023)==54){ //magic bow
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                        OBJlist_last=NULL;
                        for(y6=0;y6<x5;y6++){
                          if (OBJlist_list[y6]->type==55){ //arrow
                            OBJlist_list[y6]->more2--;
                            if (OBJlist_list[y6]->more2==0) OBJremove(OBJlist_list[y6]);
                            tnpc->upflags|=32;
                            i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                            sf[i3].type=2; //arrow
                            sf[i3].x=myobj->x;
                            sf[i3].y=myobj->y;
                            sf[i3].x2=myobj2->x;
                            sf[i3].y2=myobj2->y;
                            sf[i3].more=0xFFFF;
                            sf[i3].wait=1;
                            x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                            if (x3) wep_dt[0]=3; else wep_dt[1]=3;
                            goto skip_hth2;
                          }//->type==55
                        }//y6
                        goto skipnpcattack;
                      }//OBJcheckbolt
                      goto skip_hth2;
                    }//magic bow



                    if ((myobj3->type&1023)==50){ //triple crossbow
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                        OBJlist_last=NULL;
                        for(y6=0;y6<x5;y6++){
                          if (OBJlist_list[y6]->type==56){ //bolt
                            if (OBJlist_list[y6]->more2>=3){ //3 bolts
                              OBJlist_list[y6]->more2-=3;
                              if (OBJlist_list[y6]->more2==0) OBJremove(OBJlist_list[y6]);
                              tnpc->upflags|=32;

                              i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                              sf[i3].type=6; //bolt
                              sf[i3].x=myobj->x;
                              sf[i3].y=myobj->y;
                              sf[i3].x2=myobj2->x;
                              sf[i3].y2=myobj2->y;
                              sf[i3].more=0xFFFF;
                              sf[i3].wait=1;

                              triplecrossbow_shot=2;

                              //2 more shots
                              x6=myobj2->x; y6=myobj2->y; x7=myobj2->x; y7=myobj2->y;
                              if ((x6>myobj->x)&&(y6>myobj->y)){y6--; x7--; goto crossbow0;}
                              if ((x6>myobj->x)&&(y6<myobj->y)){x6--; y7++; goto crossbow0;}
                              if ((x6<myobj->x)&&(y6<myobj->y)){y6++; x7++; goto crossbow0;}
                              if ((x6<myobj->x)&&(y6>myobj->y)){x6++; y7--; goto crossbow0;}
                              if (x6>myobj->x){y6--; y7++; goto crossbow0;}
                              if (x6<myobj->x){y6++; y7--; goto crossbow0;}
                              if (y6<myobj->y){x6--; x7++; goto crossbow0;}
                              if (y6>myobj->y){x6++; x7--; goto crossbow0;}
crossbow0:
                              triplecrossbow_shot_x2=x6; triplecrossbow_shot_y2=y6;
                              triplecrossbow_shot_x3=x7; triplecrossbow_shot_y3=y7;

                              i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                              sf[i3].type=6; //bolt
                              sf[i3].x=myobj->x;
                              sf[i3].y=myobj->y;
                              sf[i3].x2=x6;
                              sf[i3].y2=y6;
                              sf[i3].more=0xFFFF;
                              sf[i3].wait=1;

                              i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                              sf[i3].type=6; //bolt
                              sf[i3].x=myobj->x;
                              sf[i3].y=myobj->y;
                              sf[i3].x2=x7;
                              sf[i3].y2=y7;
                              sf[i3].more=0xFFFF;
                              sf[i3].wait=1;


                              x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                              if (x3) wep_dt[0]=3; else wep_dt[1]=3;

                              goto skip_hth2;
                            }//3 arrows
                          }//->type==56
                        }//y6
                        goto skipnpcattack;
                      }//OBJcheckbolt
                      goto skip_hth2;
                    }//triple crossbow





                    if ((myobj3->type&1023)==33){ //sling
                      if (x2<=4){ //short range
                        if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                          //it would hit the wall (ignore)
                        }else{
                          i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                          sf[i3].type=13; //arrow
                          sf[i3].x=myobj->x;
                          sf[i3].y=myobj->y;
                          sf[i3].x2=myobj2->x;
                          sf[i3].y2=myobj2->y;
                          sf[i3].more=0xFFFF;
                          sf[i3].wait=1;
                          x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);

                        }//OBJcheckbolt
                      }//short range
                      goto skip_hth2;
                    }//sling

                    if ((myobj3->type&1023)==79){ //lightning wand
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        //u6ob
                        i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                        sf[i3].type=5; //wand
                        sf[i3].x=myobj->x;
                        sf[i3].y=myobj->y;
                        sf[i3].x2=myobj2->x;
                        sf[i3].y2=myobj2->y;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=1;
                        x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                        if (x3) wep_dt[0]=5; else wep_dt[1]=5;
                        //destroy item???
                        x8=myobj3->more2;


                        if (x8==0) x8=256;
                        x8--;
                        myobj3->more2=x8;
                        if (x8==0){
                          //if (x3==0) {OBJrelease(tnpc->wep_right); tnpc->wep_right=NULL;} //right hand weapon
                          //if (x3==1) {OBJrelease(tnpc->wep_left); tnpc->wep_left=NULL;} //left hand weapon
                          if (x3==0) tnpc->wep_right=NULL; //right hand weapon
                          if (x3==1) tnpc->wep_left=NULL; //left hand weapon
                          tnpc->upflags|=64;
                        }
                      }
                      goto skip_hth2;
                    }

                    if ((myobj3->type&1023)==80){ //fire wand
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        //u6ob
                        i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                        sf[i3].type=8; //wand
                        sf[i3].x=myobj->x;
                        sf[i3].y=myobj->y;
                        sf[i3].x2=myobj2->x;
                        sf[i3].y2=myobj2->y;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=1;
                        x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                        if (x3) wep_dt[0]=5; else wep_dt[1]=5;
                        //destroy item???
                        x8=myobj3->more2;
                        if (x8==0) x8=256;
                        x8--;
                        myobj3->more2=x8;

                        if (x8==0){
                          //if (x3==0) {OBJrelease(tnpc->wep_right); tnpc->wep_right=NULL;} //right hand weapon
                          //if (x3==1) {OBJrelease(tnpc->wep_left); tnpc->wep_left=NULL;} //left hand weapon
                          if (x3==0) tnpc->wep_right=NULL; //right hand weapon
                          if (x3==1) tnpc->wep_left=NULL; //left hand weapon

                          tnpc->upflags|=64;
                        }

                      }
                      goto skip_hth2;
                    }



                    if ((myobj3->type&1023)==42){ //crossbow
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                        OBJlist_last=NULL;
                        for(y6=0;y6<x5;y6++){
                          if (OBJlist_list[y6]->type==56){ //bolt
                            OBJlist_list[y6]->more2--;
                            if (OBJlist_list[y6]->more2==0) OBJremove(OBJlist_list[y6]);
                            tnpc->upflags|=32;
                            i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                            sf[i3].type=6; //crossbow
                            sf[i3].x=myobj->x;
                            sf[i3].y=myobj->y;
                            sf[i3].x2=myobj2->x;
                            sf[i3].y2=myobj2->y;
                            sf[i3].more=0xFFFF;
                            sf[i3].wait=1;
                            x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                            if (x3) wep_dt[0]=3; else wep_dt[1]=3;
                            goto skip_hth2;
                          }//==56
                        }//y6
                        goto skipnpcattack;
                      }
                      goto skip_hth2;
                    }

                    if ((myobj3->type&1023)==49){ //boomerang
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        //u6ob
                        i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                        sf[i3].type=7; //boomerang
                        sf[i3].x=myobj->x;
                        sf[i3].y=myobj->y;
                        sf[i3].x2=myobj2->x;
                        sf[i3].y2=myobj2->y;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=2;
                        x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);

                        //drop it by accident?
                        x8=rnd*(tnpc->d+1);
                        if (x8==0){
                          if (x3==0) tnpc->wep_right=NULL; //right hand weapon
                          if (x3==1) tnpc->wep_left=NULL; //left hand weapon
                          //insert myobj3
                          myobj3->prev=myobj2->prev; myobj3->x=myobj2->x; myobj3->y=myobj2->y;
                          if (myobj2->prev){
                            myobj4=(object*)myobj2->prev;
                            myobj4->next=myobj3;
                          }else{
                            od[myobj2->y][myobj2->x]=myobj3;
                          }
                          myobj2->prev=myobj3;
                          myobj3->next=myobj2;
                          myobj3->info|=112; //set flags as temp OBJ
                          AUTOPICKUPadd(myobj,myobj3);
                          tnpc->upflags|=64;
                          //equip new one
                          x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                          OBJlist_last=NULL;
                          for(y6=0;y6<x5;y6++){
                            if (OBJlist_list[y6]->type==49){//boomerang
                              OBJremove(OBJlist_list[y6]);
                              if (x3==0) tnpc->wep_right=OBJlist_list[y6]; //right hand weapon
                              if (x3==1) tnpc->wep_left=OBJlist_list[y6]; //left hand weapon
                              tnpc->upflags|=32;
                              goto boomerang_new;
                            }}
boomerang_new:;
                        }

                      }
                      goto skip_hth2;
                    }//boomerang



                    if ((myobj3->type&1023)==83){ //oil flask
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //it would hit the wall (ignore)
                      }else{
                        //u6ob

                        i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                        sf[i3].type=9; //GENERIC throw object
                        sf[i3].x=myobj->x;
                        sf[i3].y=myobj->y;
                        sf[i3].x2=myobj2->x;
                        sf[i3].y2=myobj2->y;
                        sf[i3].more=0xFFFF;
                        sf[i3].wait=83;

                        x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                        //toss it!
                        //x9=rnd*32;
                        //if (x9==0){

                        if (x3==0) tnpc->wep_right=NULL; //right hand weapon
                        if (x3==1) tnpc->wep_left=NULL; //left hand weapon


                        myobj9=OBJnew();
                        myobj9->type=317; //fire
                        //insert object9
                        myobj9->prev=myobj2->prev; myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                        if (myobj2->prev){
                          myobj4=(object*)myobj2->prev;
                          myobj4->next=myobj9;
                        }else{
                          od[myobj2->y][myobj2->x]=myobj9;
                        }
                        myobj2->prev=myobj9;
                        myobj9->next=myobj2;

                        myobj9->more2=tnpc->player->id;
                        myobj9->info&=(0xFFFF-112); myobj9->info|=(2<<4);

                        myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                        OBJcheckflags(myobj9->x,myobj9->y);


                        //equip new one?
                        x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                        OBJlist_last=NULL;
                        for(y6=0;y6<x5;y6++){
                          if (OBJlist_list[y6]->type==83){ //flask
                            OBJlist_list[y6]->more2--;
                            if (OBJlist_list[y6]->more2==0){
                              OBJremove(OBJlist_list[y6]);
                            }

                            myobj9=OBJnew();
                            myobj9->type=83;
                            myobj9->more2=1;

                            if (x3==0) tnpc->wep_right=myobj9; //right hand weapon
                            if (x3==1) tnpc->wep_left=myobj9; //left hand weapon

                            tnpc->upflags|=32;
                            goto oilflask_new;
                          }}
oilflask_new:







                        tnpc->upflags|=64;
                        //32    inventory, bag icon
                        //64    equipped items
                        //}
                      }
                      goto skip_hth2;
                    }


                    if ((myobj3->type&1023)==38){ //dagger
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //blocked!
                      }else{
                        x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                        if (x2>1){ //throw
                          i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                          sf[i3].type=10; //dagger
                          sf[i3].x=myobj->x;
                          sf[i3].y=myobj->y;
                          sf[i3].x2=myobj2->x;
                          sf[i3].y2=myobj2->y;
                          sf[i3].more=0xFFFF;
                          sf[i3].wait=1;
                          if (x3==0) tnpc->wep_right=NULL; //right hand weapon
                          if (x3==1) tnpc->wep_left=NULL; //left hand weapon
                          //insert object3
                          myobj3->prev=myobj2->prev; myobj3->x=myobj2->x; myobj3->y=myobj2->y;
                          if (myobj2->prev){
                            myobj4=(object*)myobj2->prev;
                            myobj4->next=myobj3;
                          }else{
                            od[myobj2->y][myobj2->x]=myobj3;
                          }
                          myobj2->prev=myobj3;
                          myobj3->next=myobj2;
                          myobj3->info|=112; //set flags as temp OBJ
                          AUTOPICKUPadd(myobj,myobj3);
                          tnpc->upflags|=64;
                          //equip new one?
                          x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                          OBJlist_last=NULL;
                          for(y6=0;y6<x5;y6++){
                            if (OBJlist_list[y6]->type==38){ //dagger
                              OBJremove(OBJlist_list[y6]);
                              if (x3==0) tnpc->wep_right=OBJlist_list[y6]; //right hand weapon
                              if (x3==1) tnpc->wep_left=OBJlist_list[y6]; //left hand weapon
                              tnpc->upflags|=32;
                              goto dagger_new;
                            }}
dagger_new:;
                        }else{goto skip_hthx;} //throw
                      }
                      goto skip_hth2;
                    }

                    if ((myobj3->type&1023)==37){ //throwing axe
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //blocked!
                      }else{
                        x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                        if (x2>1){ //throw
                          i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                          sf[i3].type=11; //throwing axe
                          sf[i3].x=myobj->x;
                          sf[i3].y=myobj->y;
                          sf[i3].x2=myobj2->x;
                          sf[i3].y2=myobj2->y;
                          sf[i3].more=0xFFFF;
                          sf[i3].wait=1;
                          if (x3==0) tnpc->wep_right=NULL; //right hand weapon
                          if (x3==1) tnpc->wep_left=NULL; //left hand weapon
                          //insert object3
                          myobj3->prev=myobj2->prev; myobj3->x=myobj2->x; myobj3->y=myobj2->y;
                          if (myobj2->prev){
                            myobj4=(object*)myobj2->prev;
                            myobj4->next=myobj3;
                          }else{
                            od[myobj2->y][myobj2->x]=myobj3;
                          }
                          myobj2->prev=myobj3;
                          myobj3->next=myobj2;
                          myobj3->info|=112; //set flags as temp OBJ
                          AUTOPICKUPadd(myobj,myobj3);
                          tnpc->upflags|=64;
                          //equip new one?
                          x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                          OBJlist_last=NULL;
                          for(y6=0;y6<x5;y6++){
                            if (OBJlist_list[y6]->type==37){ //throwing axe
                              OBJremove(OBJlist_list[y6]);
                              if (x3==0) tnpc->wep_right=OBJlist_list[y6]; //right hand weapon
                              if (x3==1) tnpc->wep_left=OBJlist_list[y6]; //left hand weapon
                              tnpc->upflags|=32;
                              goto axe_new;
                            }}
axe_new:;
                        }else{goto skip_hthx;} //throw
                      }
                      goto skip_hth2;
                    }

                    if ((myobj3->type&1023)==36){ //spear
                      if (OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)){
                        //blocked!
                      }else{
                        x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                        if (x2>1){ //throw
                          i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                          sf[i3].type=12; //spear
                          sf[i3].x=myobj->x;
                          sf[i3].y=myobj->y;
                          sf[i3].x2=myobj2->x;
                          sf[i3].y2=myobj2->y;
                          sf[i3].more=0xFFFF;
                          sf[i3].wait=1;
                          if (x3==0) tnpc->wep_right=NULL; //right hand weapon
                          if (x3==1) tnpc->wep_left=NULL; //left hand weapon
                          //insert object3
                          myobj3->prev=myobj2->prev; myobj3->x=myobj2->x; myobj3->y=myobj2->y;
                          if (myobj2->prev){
                            myobj4=(object*)myobj2->prev;
                            myobj4->next=myobj3;
                          }else{
                            od[myobj2->y][myobj2->x]=myobj3;
                          }
                          myobj2->prev=myobj3;
                          myobj3->next=myobj2;
                          myobj3->info|=112; //set flags as temp OBJ
                          AUTOPICKUPadd(myobj,myobj3);

                          tnpc->upflags|=64;
                          //equip new one?
                          x5=OBJlist((object*)tnpc->items->more); //x5=last object number
                          OBJlist_last=NULL;
                          for(y6=0;y6<x5;y6++){
                            if (OBJlist_list[y6]->type==36){ //spear
                              OBJremove(OBJlist_list[y6]);
                              if (x3==0) tnpc->wep_right=OBJlist_list[y6]; //right hand weapon
                              if (x3==1) tnpc->wep_left=OBJlist_list[y6]; //left hand weapon
                              tnpc->upflags|=32;
                              goto spear_new;
                            }}
spear_new:;
                        }else{goto skip_hthx;} //throw

                      }

                      goto skip_hth2;
                    }

                    goto skip_hthx; //not long range!


skip_hth2: //skipped other weapon types, projectile weapon

                    //miss?
                    if (x){ //can do damage
                      x7=rnd*(tnpc->d+1);
                      x8=rnd*16; /* this should be dependant on target being attacked I think and with some weapons it should be easier to hit (ranged weapons need higher dex and heavy melee weapons lower) */
                      if (x7<x8) {
                        x=0; miss=1;

                        //if bow, magic bow, or crossbow, drop an arrow at the destination 50% of the time
                        if ((npcattackweptype==41)||(npcattackweptype==54)||(npcattackweptype==42)||(npcattackweptype==50)){
                          x7=rnd*2; if (x7){ //50% chance
                            myobj9=OBJnew();
                            AUTOPICKUPadd(myobj,myobj9);
                            myobj9->type=55; //arrow
                            if ((npcattackweptype==42)||(npcattackweptype==50)) myobj9->type=56; //bolt
                            myobj9->more2=1;
                            //insert object9
                            myobj9->prev=myobj2->prev; myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                            if (myobj2->prev){
                              myobj4=(object*)myobj2->prev;
                              myobj4->next=myobj9;
                            }else{
                              od[myobj2->y][myobj2->x]=myobj9;
                            }
                            myobj2->prev=myobj9;
                            myobj9->next=myobj2;
                            myobj9->info|=112; //set flags as temp OBJ
                            myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                            OBJcheckflags(myobj9->x,myobj9->y);


                          }//x7
                        }//npcattackweptype

                        if (triplecrossbow_shot){
                          if (tnpc->wait_attack||tnpc->wait_disable) goto skipnpcattack;
                          f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                          tnpc->wait_attack+=2.0-(f*0.25f);
                          goto triplecrossbow_miss;
                        }
                      }//missed!
                    }//x
                    goto skip_hth1;
                  } //x2<=8 (long range)

skip_hthx: //not a long range weapon

                  if (x2<=1){
hth_norangecheck:
                    if (x==0) x+=obji[sprlnk[myobj3->type&1023]].v1+ENHANCEget_attack(myobj3);
                    if (myobj3->type==48) x=1023;
                  }//x2<=1

                }//myobj3

                if ((x3==1)&&(x==0)&&(x9==0)&&(x2<=1)){ //attack with bare hands!
                  wep_dt[1]=1;
                  x=1;
                }

skip_hth1:;


                if (x3==0) {x9=x; x=0;}

              }//x3





              if (x||x9){

                //attack possible

                //ERROR! it fires the arrow but can't actually attack because attack waiting in ON!
                //this could cause hell for NPC!
                //



                if (tplayer->craft==NULL) OBJdir(myobj,myobj2->x,myobj2->y);

                f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                f=2.0f-(f*0.25f);
                if (spikedhelm_attack){
                  if (x&&x9) f*=1.25f; else f*=1.125f;
                }else{
                  if (x&&x9) f*=1.125f;
                }
                tnpc->wait_attack+=f;

                i4=SFnew(myobj2->x,myobj2->y);
                sf[i4].type=1; //attack
                sf[i4].x=myobj2->x;
                sf[i4].y=myobj2->y;
                sf[i4].x2=0;
                sf[i4].wait=0.125f; //NULL
                sf[i4].more=1;







                for (i3=0;i3<=1;i3++){
                  if (i3) x=x9;
                  if (x){
                    if (i3==0) myobj3=tnpc->wep_left; else myobj3=tnpc->wep_right; //NOTE: this value may not point to an actual weapon!
triplecrossbow_refire:
spikedhelm_entrypoint:





                    if (attack_item){
                      if (myobj2->type==53){

                        if (myobj3){ if (myobj3->type==48){ //smash glass sword!
                          if (i3==0) tnpc->wep_left=NULL; else tnpc->wep_right=NULL;
                          tnpc->upflags|=64; }}

                        //web strength
                        x2=rnd*(x+1); //attack strength
                        x3=rnd*(128+1); //web strength
                        if (x2>x3){
                          OBJsave(myobj2->x,myobj2->y);
                          VLNKremove(myobj2);
                          OBJremove(myobj2);
                          myobj2=NULL; goto attackdone;
                        }//x2>x3
                      }//53

                      if ((myobj2->type&1023)==98){ //chest
                        x2=0;
                        if (myobj2->type<=(98+1024)) x2=1; //closed/opened
                        if (myobj2->type==(98+2048)) x2=1; //locked
                        if (x2){

                          if (myobj3){ if (myobj3->type==48){ //smash glass sword!
                            if (i3==0) tnpc->wep_left=NULL; else tnpc->wep_right=NULL;
                            tnpc->upflags|=64; }}

                          x2=rnd*(x+1); //attack 
                          x3=rnd*(256+1); //chest
                          if (x2>x3){
                            x3=myobj2->x; y3=myobj2->y;
                            OBJsave(x3,y3);

                            VLNKremove(myobj2);
                            //VLNKsremove(myobj2);
                            OBJremove(myobj2);

                            //extract items
                            myobj=myobj2;
extractobj5:
                            myobj2=(object*)myobj->more;
                            if (myobj2!=NULL) {OBJremove(myobj2); myobj2->info|=112; OBJadd(x3,y3,myobj2); goto extractobj5;}
                            myobj2=NULL; goto attackdone;

                          }//x2
                        }//x2>x3
                      }//98

                      goto attack_item_skip2;
                    }




                    if (myobj3){
                      if (myobj3->type==OBJ_FIRE_WAND){//fire wand
                        x2=myobj2->type&1023;
                        if (x2==411) x=0;//dragon
                        if (x2==369) x=0;//drake
                        if (x2==352) x=0;//ghost
                        if (x2==374) x=0;//hydra
                        if (x2==369) x=0;//drake
                        if (x2==368) x=0;//skeleton
                        if (x2==373) x=0;//wisp
                        if (x2==367) x=0;//daemon
                        //double damage?
                        if (x2==364) x*=2; //acid slug
                        if (x2==357) x*=2; //corpser
                        if (x2==347) x*=2; //reaper
                        if (x2==360) x*=2; //rot worms
                        if (x2==375) x*=2; //slime
                        if (x2==365) x*=2; //tangle vine
                      }
                    }


                    //slime divide!
                    if ((myobj2->type&1023)==OBJ_SLIME){
                      static unsigned long slimex,slimey;
                      if ((myobj2->info&(15<<9))==0) goto slimedivide_failed;
                      //test surrounding squares
                      slimex=myobj2->x+1; slimey=myobj2->y;
                      if ((od[slimey][slimex]==NULL)&&((bt[slimey][slimex]>>10)&1)) goto slimedivide;
                      slimex=myobj2->x-1; slimey=myobj2->y;
                      if ((od[slimey][slimex]==NULL)&&((bt[slimey][slimex]>>10)&1)) goto slimedivide;
                      slimex=myobj2->x; slimey=myobj2->y+1;
                      if ((od[slimey][slimex]==NULL)&&((bt[slimey][slimex]>>10)&1)) goto slimedivide;
                      slimex=myobj2->x; slimey=myobj2->y-1;
                      if ((od[slimey][slimex]==NULL)&&((bt[slimey][slimex]>>10)&1)) goto slimedivide;
                      goto slimedivide_failed;
slimedivide:
                      myobj3=OBJnew();
                      myobj3->type=375;
                      myobj3->more=malloc(sizeof(creature));
                      ZeroMemory(myobj3->more,sizeof(creature));
                      crt3=(creature*)myobj3->more;
                      crt3->crt_struct=TRUE;
                      crt3->hp=(obji[sprlnk[myobj3->type&1023]].v8>>8)*4;
                      crt3->mp=0;
                      crt3->al=1;
                      crt3->respawn_x=1024; //seconds till spawned crt will disappear
                      myobj9=OBJnew(); myobj9->type=188; crt3->items=myobj9; //(not included)bag
                      myobj3->info|=4; //<-crt
                      OBJmove2(myobj3,slimex,slimey);

                      x2=(myobj2->info>>9)&15; x2--;
                      myobj2->info&=(0xFFFF-(15<<9));
                      myobj2->info|=(x2<<9);

                      myobj3->info|=(x2<<9);

slimedivide_failed:;
                    }


                    f=x; //default (unclassified) bonus
                    //OLD VALUES
                    //if (wep_dt[i3]==1) f=(float)x/32.0f*(float)tnpc->s; //100% STR
                    //if (wep_dt[i3]==2) f=(float)x/32.0f*(((float)tnpc->s/2.0f)+((float)tnpc->d/2.0f)); //50% STR + 50% DEX
                    //if (wep_dt[i3]==3) f=(float)x/32.0f*((float)tnpc->d*2.0f); //200% DEX (bows, crossbows)
                    if (wep_dt[i3]==1) f=(float)x/32.0f*((float)tnpc->s+(float)tnpc->d*0.25f);//STR*100%+DEX*25%
                    if (wep_dt[i3]==2) f=(float)x/32.0f*((float)tnpc->d*0.75f+(float)tnpc->s*0.25f);//DEX*75%+STR*25%
                    if (wep_dt[i3]==3) f=(float)x/32.0f*((float)tnpc->d*1.75f);//DEX*175%
                    if (wep_dt[i3]==4){
                      //DEX*100% or STR*100%, whichever is lower
                      if (tnpc->s<=tnpc->d){
                        f=(float)x/32.0f*( (float)tnpc->s + ((float)tnpc->d-(float)tnpc->s)*0.25f );
                      }else{
                        f=(float)x/32.0f*( (float)tnpc->d + ((float)tnpc->s-(float)tnpc->d)*0.25f );
                      }
                    }
                    if (wep_dt[i3]==5) f=(float)x/32.0f*((float)tnpc->i*0.125f+8);//INT*12.5% wands are magical weapons, so they should have some base damage bonus in u6 they were quite good */
                    x2=rnd*((float)(x*2)+f+1.0f)-(float)x; //weapon damage
                    if (x2>x) x2=x;


                    if (tplayer->craft) x2=rnd*31.0f; //cannons (can't graze target!)

                    x2-=(int)(rnd*((obji[sprlnk[myobj2->type&1023]].v7>>8)+1+1+getequiparmourvalue((object*)crt->items->more))); //enemy arm +1
                    if (x2<0) x2=0;

                    if (tplayer->craft) goto nododge;
                    if (wep_dt[i3]==3) goto nododge;
                    if (int(rnd*32)<=int(rnd*((obji[sprlnk[myobj2->type&1023]].v6>>8)+1))){
                      x2=0; //enemy dodged the attack!
                    }
nododge:

                    sf[i4].x2+=x2;
                    if (sf[i4].x2==0) sf[i4].type=16; else sf[i4].type=1; //change attack to a miss type?

                    if (x2){
                      if (myobj3){ if (myobj3->type==48){ //smash glass sword!
                        if (i3==0) tnpc->wep_left=NULL; else tnpc->wep_right=NULL;
                        tnpc->upflags|=64; }}
                    }

                    //x7=rnd*32; //0-127
                    //x8=rnd*(tnpc->d+1);
                    //if (x7>x8){ //couldn't dodge the attack!

spellattcrt0:

                    if (myobj){ //valid source of attack exists
                      if (crt->al==0){ //passive
                        if ( ((myobj2->type&1023)!=430) || ((crt->flags&128)==0) ){//horse exception
                          crt->al=3; //passive turned aggressive
                          crt->target=myobj;
                          VLNKsremove(myobj2); //remove prev vlnk
                          VLNKnew(myobj2,crt->target,(unsigned long)&crt->target); //add vlnk
                        }//horse exception
                      }
                      if (crt->al==4){ //defend itself!
                        crt->target=myobj;
                        VLNKsremove(myobj2); //remove prev vlnk
                        VLNKnew(myobj2,crt->target,(unsigned long)&crt->target); //add vlnk
                      }
                    }

                    if (crt->flags&4){ if (x2){ //wake creature if asleep!
                      crt->wait=0;
                      if (crt->flags&8) crt->flags-=8; //unparalyze //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                      if (crt->flags&4) crt->flags-=4; //awaken //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                    }}

                    crt->hp-=x2; if (crt->hp<0) crt->hp=0;



                    if ((myobj2->type&1023)==366) crt->hp=32767; //invincible tanglevine tendril!
                    if ((myobj2->type&1023)==430) if (crt->flags&128) crt->hp=32767;//invincible horse

                    if (crt->hp<=0){

                      if (tnpc){ //valid tnpc
                        tnpc->player->midi=255; //invalid number!
                        if (tnpc->player->craft==NULL){ //*2004 TEMP

                          if (myobj2->type==355) {
                            myobj4=OBJnew();
                            myobj4->type=343; //insects
                            myobj4->more=malloc(sizeof(creature));
                            ZeroMemory(myobj4->more,sizeof(creature));
                            crt3=(creature*)myobj4->more;
                            crt3->crt_struct=TRUE;
                            crt3->hp=(obji[sprlnk[myobj4->type&1023]].v8>>8)*4;
                            crt3->mp=rnd*9;
                            crt3->al=1;
                            crt3->respawn_x=1024; //seconds till spawned crt will disappear
                            myobj9=OBJnew(); myobj9->type=188; crt3->items=myobj9; //(not included)bag
                            myobj4->info|=4; //<-crt
                            OBJadd(myobj2->x,myobj2->y,myobj4);
                          }
                          myobj4=NULL;
                          if (mover_blood[myobj2->type&1023]){
                            myobj4=OBJnew();
                            myobj4->type=338+(int(rnd*3)*1024);
                            myobj4->info|=112;
                          }


                          if (mover_body[myobj2->type&1023]){

                            //if it's a cyclops or a winged garg, check surrounding squares for practicality
                            //of a body drop
                            if ((mover_body[myobj2->type&1023]==(340+1024*3))||(mover_body[myobj2->type&1023]==(341+1024*3))){
                              //RULE 1: all basetiles must have an object walkable flag
                              if ((btflags[bt[myobj2->y][myobj2->x]&1023]&1)==0) goto nospaceforbody;
                              if ((btflags[bt[myobj2->y-1][myobj2->x]&1023]&1)==0) goto nospaceforbody;
                              if ((btflags[bt[myobj2->y][myobj2->x-1]&1023]&1)==0) goto nospaceforbody;
                              if ((btflags[bt[myobj2->y-1][myobj2->x-1]&1023]&1)==0) goto nospaceforbody;
                              //RULE 2: only 1 object may exist on the square at the time of the drop
                              //FACT: the object is a 4 part mover, there WILL be a first object on each square
                              if (od[myobj2->y][myobj2->x]->next) goto nospaceforbody;
                              if (od[myobj2->y-1][myobj2->x]->next) goto nospaceforbody;
                              if (od[myobj2->y][myobj2->x-1]->next) goto nospaceforbody;
                              if (od[myobj2->y-1][myobj2->x-1]->next) goto nospaceforbody;
                            }
                            if (mover_body[myobj2->type&1023]==(134+1024*1)){//carcass
                              if ((btflags[bt[myobj2->y][myobj2->x]&1023]&1)==0) goto nospaceforbody;
                              if ((btflags[bt[myobj2->y][myobj2->x-1]&1023]&1)==0) goto nospaceforbody;
                              if (od[myobj2->y][myobj2->x]->next) goto nospaceforbody;
                              if (od[myobj2->y][myobj2->x-1]) goto nospaceforbody;
                            }

                            myobj5=OBJnew();
                            myobj5->type=mover_body[myobj2->type&1023];
                            if ((mover_body[myobj2->type&1023]>=432)&&(mover_body[myobj2->type&1023]<=447)){
                              myobj5->type|=(myobj2->type>>10<<10);
                            }
                            myobj5->info|=112;
                            //edit type if/as necessary
                            //4 part
                            if (((myobj2->type&1023)==344)||((myobj2->type&1023)==347)||((myobj2->type&1023)==353)||((myobj2->type&1023)==356)){
                              myobj5->type=(myobj5->type&1023)+(int(rnd*4)*1024);
                            }
                            //8 part flappers
                            if (((myobj2->type&1023)==369)||((myobj2->type&1023)==372)){
                              if ((myobj5->type&1024)==0){
                                myobj5->type=myobj5->type+(int(rnd*2)*1024);
                              }
                            }

                            if (mover_blood[myobj2->type&1023]){
                              if ((mover_body[myobj2->type&1023]>=432)&&(mover_body[myobj2->type&1023]<=447)){
                                myobj4->type=338+(((myobj5->type>>10)%3)<<10);
                              }
                            }

                            if (myobj4){
                              if (myobj6=od[myobj2->y][myobj2->x]){
                                od[myobj2->y][myobj2->x]=myobj4; myobj4->next=myobj6; myobj6->prev=myobj4;
                                myobj4->x=myobj2->x; myobj4->y=myobj2->y;
                              }else{
                                OBJadd(myobj2->x,myobj2->y,myobj4);
                              }
                            }

                            /*
                            if (((obj->type&1023)>=432)&&((obj->type&1023)<=447)){//blood of dead crt
                            im32z(ps,GSx,GSy,spr84[3],171+((obj->type>>10)%3));
                            }
                            */

                            OBJadd(myobj2->x,myobj2->y,myobj5);


                            if ((mover_body[myobj2->type&1023]==(340+1024*3))||(mover_body[myobj2->type&1023]==(341+1024*3))){
                              myobj4=OBJnew(); myobj4->type=myobj5->type-1024; myobj4->info|=112; myobj4->info|=8; myobj4->more=myobj5;
                              OBJadd(myobj2->x-1,myobj2->y,myobj4);
                              myobj4=OBJnew(); myobj4->type=myobj5->type-2048; myobj4->info|=112; myobj4->info|=8; myobj4->more=myobj5;
                              OBJadd(myobj2->x,myobj2->y-1,myobj4);
                              myobj4=OBJnew(); myobj4->type=myobj5->type-3072; myobj4->info|=112; myobj4->info|=8; myobj4->more=myobj5;
                              OBJadd(myobj2->x-1,myobj2->y-1,myobj4);
                            }
                            if (mover_body[myobj2->type&1023]==(134+1024*1)){
                              myobj4=OBJnew(); myobj4->type=myobj5->type-1024; myobj4->info|=112; myobj4->info|=8; myobj4->more=myobj5;
                              OBJadd(myobj2->x-1,myobj2->y,myobj4);
                            }


                            if (myobj4=(object*)crt->items->more){
                              myobj4->prev=myobj5;
                              myobj5->more=myobj4;
                              crt->items->more=NULL;
                            }





                          }else{
nospaceforbody:

                            if (myobj4){
                              if (myobj6=od[myobj2->y][myobj2->x]){
                                od[myobj2->y][myobj2->x]=myobj4; myobj4->next=myobj6; myobj6->prev=myobj4;
                                myobj4->x=myobj2->x; myobj4->y=myobj2->y;
                              }else{
                                OBJadd(myobj2->x,myobj2->y,myobj4);
                              }
                            }

                            //drop gold/items at death spot!
                            myobj4=(object*)crt->items->more; //will point to first item
dropgoldmore:
                            if (myobj4){
                              myobj5=(object*)myobj4->next;
                              OBJmove_allow=TRUE;
                              OBJmove(myobj4,myobj2->x,myobj2->y);
                              OBJmove_allow=FALSE;
                              myobj4->info|=112; //set flags as temp OBJ
                              if (myobj5) {myobj4=myobj5; goto dropgoldmore;}
                            }

                          }

                          if (crt->al==1){ //only add exp if agressive

                            //amulet of submission check (no exp + karma loss for attacking gargoyles)
                            if (((myobj2->type&1023)==362)||((myobj2->type&1023)==363)){//gargoyle
                              if (myobj2->x>=1024){ if (myobj2->y>=256){
                                if (myobj2->x<1280){ if (myobj2->y<512){
                                  tplayer3=tnpc->player;
                                  if (tplayer3->GNPCflags[12]){
                                    tplayer3->karma-=2; txtset(t,"?You feel less virtuous than before!"); t->d2[0]=8; NET_send(NETplayer,tplayer3->net,t);
                                    goto noexp;
                                  }
                                  for (x2=1;x2<=7;x2++){
                                    if (tplayer3->party[x2]){
                                      if (((npc*)tplayer3->party[x2]->more)->converse==162){
                                        tplayer3->karma-=2; txtset(t,"?You feel less virtuous than before!"); t->d2[0]=8; NET_send(NETplayer,tplayer3->net,t);
                                        goto noexp;
                                      }
                                    }//tplayer3->party[x2]
                                  }//x2
                                }}}}//x,y,x,y
                            }//gargoyle

                            tnpc->exp+=obji[sprlnk[myobj2->type&1023]].v5; //*needs to change
                            //Inexperienced character armour bonus changed to 50%
                            if (tnpc->player){ if (tnpc->player->party[0]){ if ((npc*)tnpc->player->party[0]->more==tnpc){ if (tnpc->exp<1600){ if (tnpc->lev<=2){
                              //tnpc->arm+=(20-tnpc->exp/80);
                              tnpc->upflags|=64;
                              txtset(t,"?"); t->d2[0]=8; txtadd(t,"Inexperienced character armour bonus is "); txtnumint(t2,100-tnpc->exp/16); txtadd(t,t2); txtadd(t,"%"); NET_send(NETplayer,tnpc->player->net,t);
                            }}}}}
                            tnpc->upflags|=8;
                          }//al==1

                        }//->craft
                      }//tnpc
noexp:

                      //passive turned agressive cat? if so set global flag
                      if (tplayer3=tnpc->player){
                        if (crt->al==3){//passive turned agressive
                          if ((myobj2->type&1023)==388){//cat
                            tplayer3->GNPCflags[36]=1;
                          }//cat
                        }//3
                      }//->player
                      crtrespawn(myobj2);
                      if (spellattcrt) {spellattcrt=FALSE; goto spellattcrt1;}



                      //object can now safely be destroyed (if not in update list)

                      myobj2=NULL; goto attackdone;

                    }
                    if (spellattcrt) {spellattcrt=FALSE; goto spellattcrt1;}
attack_item_skip2:;
                  }//(x)
                }//i3

attackdone:

                if (spikedhelm_attack){
                  spikedhelm_attack=0;//clear set variables
                  if (myobj2){
                    myobj3=NULL;
                    i3=1;
                    wep_dt[1]=1;
                    x=4;
                    goto spikedhelm_entrypoint;
                  }
                }


                if (triplecrossbow_shot){
triplecrossbow_miss:
                  triplecrossbow_shot--;

                  if (triplecrossbow_shot){ //2nd shot
                    x6=4; //75% chance of bolt recovery
                    myobj2=OBJfindlast(triplecrossbow_shot_x2,triplecrossbow_shot_y2); if (myobj2==NULL) myobj2=OBJfindlastall(triplecrossbow_shot_x2,triplecrossbow_shot_y2);
                    if (myobj2){
                      //if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                      if (myobj2->info&4){ //<-crt
                        x7=rnd*(tnpc->d+1); x8=rnd*16; if (x7>=x8){ //didn't miss!
                          i3=1;
                          wep_dt[0]=3; wep_dt[1]=3;
                          x=12;

                          i4=SFnew(myobj2->x,myobj2->y);
                          sf[i4].type=1; //attack
                          sf[i4].x=triplecrossbow_shot_x2;
                          sf[i4].y=triplecrossbow_shot_y2;
                          sf[i4].x2=0;
                          sf[i4].wait=0.125f; //NULL
                          sf[i4].more=1;

                          goto triplecrossbow_refire;
                        }//didn't miss
                        x6=2; //50% chance of bolt recovery
                      }//crt
                    }//myobj2

                    x7=rnd*x6; if (x7){ //recover bolt
                      if (myobj2) if (myobj2->info&2) x6=2; //npc
                      if (x6==2){ //x6: bolt under crt/npc
                        myobj9=OBJnew();
                        AUTOPICKUPadd(myobj,myobj9);
                        myobj9->type=56;
                        myobj9->more2=1;
                        myobj9->prev=myobj2->prev; myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                        if (myobj2->prev){
                          myobj4=(object*)myobj2->prev;
                          myobj4->next=myobj9;
                        }else{
                          od[myobj2->y][myobj2->x]=myobj9;
                        }
                        myobj2->prev=myobj9;
                        myobj9->next=myobj2;
                        myobj9->info|=112; //set flags as temp OBJ
                        myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                        OBJcheckflags(myobj9->x,myobj9->y);
                      }else{ //x6: bolt on ground
                        if (bt[triplecrossbow_shot_y2][triplecrossbow_shot_x2]&1024){ //walkable
                          myobj9=OBJnew(); myobj9->type=56; myobj9->more2=1; myobj9->info=112; OBJadd(triplecrossbow_shot_x2,triplecrossbow_shot_y2,myobj9);
                          AUTOPICKUPadd(myobj,myobj9);
                        }//walkable
                      }//x6
                    }//x7

                    triplecrossbow_shot--;
                  }//triplecrossbow_shot

                  //3rd/final shot

                  /*
                  x7=rnd*(tnpc->d+1); x8=rnd*16; if (x7<x8) goto triplecrossbow_miss2;
                  myobj2=OBJfindlast(triplecrossbow_shot_x3,triplecrossbow_shot_y3); if (myobj2==NULL) myobj2=OBJfindlastall(triplecrossbow_shot_x3,triplecrossbow_shot_y3);
                  if (myobj2){
                  //if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                  if (myobj2->info&4){ //<-crt
                  i3=1;
                  x=12;
                  wep_dt[0]=3; wep_dt[1]=3;
                  i4=SFnew(myobj2->x,myobj2->y);
                  sf[i4].type=1; //attack
                  sf[i4].x=triplecrossbow_shot_x3;
                  sf[i4].y=triplecrossbow_shot_y3;
                  sf[i4].x2=0;
                  sf[i4].wait=0.125f; //NULL
                  sf[i4].more=1;
                  goto triplecrossbow_refire;
                  }
                  }
                  */

                  x6=4; //75% chance of bolt recovery
                  myobj2=OBJfindlast(triplecrossbow_shot_x3,triplecrossbow_shot_y3); if (myobj2==NULL) myobj2=OBJfindlastall(triplecrossbow_shot_x3,triplecrossbow_shot_y3);
                  if (myobj2){
                    //if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                    if (myobj2->info&4){ //<-crt
                      x7=rnd*(tnpc->d+1); x8=rnd*16; if (x7>=x8){ //didn't miss!
                        i3=1;
                        wep_dt[0]=3; wep_dt[1]=3;
                        x=12;

                        i4=SFnew(myobj2->x,myobj2->y);
                        sf[i4].type=1; //attack
                        sf[i4].x=triplecrossbow_shot_x3;
                        sf[i4].y=triplecrossbow_shot_y3;
                        sf[i4].x2=0;
                        sf[i4].wait=0.125f; //NULL
                        sf[i4].more=1;
                        goto triplecrossbow_refire;
                      }//didn't miss
                      x6=2; //50% chance of bolt recovery
                    }//crt
                  }//myobj2

                  x7=rnd*x6; if (x7){ //recover bolt
                    if (myobj2) if (myobj2->info&2) x6=2; //npc
                    if (x6==2){ //x6: bolt under crt/npc
                      myobj9=OBJnew();
                      AUTOPICKUPadd(myobj,myobj9);
                      myobj9->type=56;
                      myobj9->more2=1;
                      myobj9->prev=myobj2->prev; myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                      if (myobj2->prev){
                        myobj4=(object*)myobj2->prev;
                        myobj4->next=myobj9;
                      }else{
                        od[myobj2->y][myobj2->x]=myobj9;
                      }
                      myobj2->prev=myobj9;
                      myobj9->next=myobj2;
                      myobj9->info|=112; //set flags as temp OBJ
                      myobj9->x=myobj2->x; myobj9->y=myobj2->y;
                      OBJcheckflags(myobj9->x,myobj9->y);
                    }else{ //x6: bolt on ground
                      if (bt[triplecrossbow_shot_y3][triplecrossbow_shot_x3]&1024){ //walkable
                        myobj9=OBJnew(); myobj9->type=56; myobj9->more2=1; myobj9->info=112; OBJadd(triplecrossbow_shot_x3,triplecrossbow_shot_y3,myobj9);
                        AUTOPICKUPadd(myobj,myobj9);
                      }//walkable
                    }//x6
                  }//x7

                }//triplecrossbow_shot: end






              }//x>0 attack possible
              else
              {//attack was NOT possible, close in on target!

                if (miss){ //miss!
                  if (tplayer->craft==NULL) OBJdir(myobj,myobj2->x,myobj2->y);
                  f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                  tnpc->wait_attack+=2.0-(f*0.25f);
                  goto skipnpcattack;
                }

attacknotpossible:

                if (selected_partymember) goto skipnpcattack; //movement is manual
                if (tnpc->wait_walk) goto skipnpcattack; //can't move closer anyway!

npcattackclose:

                myobj2=tnpc->target; //activeplayer->party[activeplayer->selected_partymember];
                x2=0; y2=0;



                WPF_NEXTTO=1; WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,32,0,2);
                x3=0;
                if (WPF_RETURN==WPF_PATHFOUND){
                  x3=WPF_PATHLENGTH;
                }
                if (x3==0){//can't get within rage of target! or ALREADY THERE!
                  tnpc->order=1; tnpc->follow=tplayer->party[tplayer->selected_partymember]; goto skipnpcattack;
                }


                /*
                PATHFIND_NEXTTO=1; x3=pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,1);
                if ((x3>32)||(x3==0)) {PATHFIND_NEXTTO=1; x3=pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,NULL);}
                if ((x3>32)||(x3==0)){ //can't get within rage of target! or ALREADY THERE!
                tnpc->order=1; tnpc->follow=tplayer->party[tplayer->selected_partymember]; goto skipnpcattack;
                }
                */

                if (tnpc->path[0]==PFright) x2=1;
                if (tnpc->path[0]==PFleft) x2=-1;
                if (tnpc->path[0]==PFup) y2=-1;
                if (tnpc->path[0]==PFdown) y2=1;

                x3=0;
                if (x2){
                  if (OBJmove2(myobj,myobj->x+x2,myobj->y)==0) x3++;
                }
                if ((y2)&&(x3==0)){
                  if (OBJmove2(myobj,myobj->x,myobj->y+y2)==0) x3++;
                }
                if (x3){
                  f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                  tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);
                }

              } //close in on target (couldn't attack)

skipnpcattack:;
            } //end attack



            //} //NPC, not selected partymember
NPC_skip1:; //NPC (not player/party) skip

          } //CRITICAL ERROR CORRECT
        } //NPCtype

donemove:;

skip_ouli:;
      }//oul[i]!=NULL
    }//i
  }//oul!=0

  u6opi=5;
  for (tpl=0;tpl<=playerlist_last;tpl++){ //process INPUT
    if (playerlist[tpl]){ //current player

      tplayer=playerlist[tpl];

      if (tplayer->mobj){

        tplayer->mobj->info|=112; //set mobj flags as temp OBJ

        if ((tplayer->mobj->type&1023)==223){ //powder keg
          tplayer->mobj->more2=tplayer->id; //id
          if (tplayer->mobj->type!=223){
            tplayer->mobj->type=223;
            tplayer->mobj_last=NULL; //update message: mobj has changed
          }
        }//223

        if ((tplayer->mobj->type&1023)==173){ //trap
          tplayer->mobj->more2=tplayer->id; //id
        }//173

        //set "new" horse owner in respawn_y
        if (tplayer->mobj->type==448){
          if (myobj=(object*)tplayer->mobj->more){

            //GPF latest
            if (myobj->info&4){//crt
              crt=(creature*)myobj->more;
              crt->respawn_y=tpl;
            }//crt
          }//->more
        }//448

      }//mobj





      //followers resurrection
      if (tplayer->respawn==254){
        tplayer->respawn=0;
        if (tplayer->party[0]==NULL){
          txtsetchar(t,8); txtadd(t,"You must first resurrect yourself!"); NET_send(NETplayer,tplayer->net,t);
          goto resurrectfollowers_failed;
        }
        if (tplayer->craft){
          txtsetchar(t,8); txtadd(t,"Not now!"); NET_send(NETplayer,tplayer->net,t);
          goto resurrectfollowers_failed;
        }
        i2=0;
        for (i=0;i<=nresu;i++){
          if (resu[i]){
            if (resu_player[i]==tplayer){
              if (resu_partymember[i]>=1){
                i2=1;
                tplayer->party[resu_partymember[i]]=resu[i];
                tnpc=(npc*)resu[i]->more;
				//tnpc->exp-=(tnpc->exp/8);
				if (easymodehostn1) {
					tnpc->exp -= new1_getexpdeduction(tnpc, 2); // c111
				}
				else { // original
					tnpc->exp-=(tnpc->exp/8);
				}
                resu[i]=NULL;
                OBJmove_allow=TRUE;
                OBJmove2(tplayer->party[resu_partymember[i]],tplayer->party[0]->x,tplayer->party[0]->y);
                OBJmove_allow=FALSE;
                txtset(t5,"?"); t5->d2[0]=8; txtadd(t5,tnpc->name); txtadd(t5," resurrected. (F1)");
                for (x2=0;x2<=playerlist_last;x2++){ //create and send UPDATE message
                  if (playerlist[x2]){ //current player
                    if (playerlist[x2]->net!=INVALID_NET){ //net connection available
                      if (playerlist[x2]->party[0]){
                        NET_send(NETplayer,playerlist[x2]->net,t5);
                      }}}}
              }//0
            }//player
          }//resu
        }//i
        if (i2) {inbritanniacheck(); goto resurrectfollowers_failed;}
        txtsetchar(t,8); txtadd(t,"None of your followers are dead!"); NET_send(NETplayer,tplayer->net,t);
      }//respawn
resurrectfollowers_failed:

      //self-resurrection?
      if (tplayer->respawn==255){
        tplayer->respawn=0;
        //check ressurection tables for player's pointer
        //return player to party[0]
        for (i=0;i<=nresu;i++){
          if (resu[i]){
            if (resu_player[i]==tplayer){
              if (resu_partymember[i]==0){
                /*
                object *resu[65536]; //object
                object *resu_body[65536]; //dead body object
                float resu_wait[65536]; //time until object automatically resurrected
                player *resu_player[65536]; //player object belongs to
                unsigned char resu_partymember[65536]; //party member index
                long nresu=-1;
                */
                tplayer->party[0]=resu[i];
                if (tplayer->GNPCflags[25]==0) {
                  tnpc=(npc*)resu[i]->more;
				  //tnpc->exp-=(tnpc->exp/8);//res by f1
				  if (easymodehostn1) {
					  tnpc->exp -= new1_getexpdeduction(tnpc, 2); // c111
				  }
				  else { // original
					  tnpc->exp-=(tnpc->exp/8);//res by f1
				  }
                  x2=256+128-16;
                  y2=256+128+2;
                }
                else { //resurrect at brit healer's bed and remove insurance
                  tplayer->GNPCflags[25]=0;
                  x2=362;
                  y2=362;
                }

                resu[i]=NULL;

                for (x4=1;x4<=7;x4++){
                  if (tplayer->party[x4]){
                    tnpc2=(npc*)tplayer->party[x4]->more; tnpc2->upflags=1;
                    OBJmove_allow=TRUE;
                    OBJmove2(tplayer->party[x4],x2,y2); //place party on the map
                    OBJmove_allow=FALSE;
                  }
                }//x4

                OBJmove_allow=TRUE;
                OBJmove2(tplayer->party[0],x2,y2); //place avatar on the map
                OBJmove_allow=FALSE;

                txtset(t,"?"); t->d2[0]=8; txtadd(t,"A voice in the darkness intones, \x022KAL LOR!\x022"); NET_send(NETplayer,tplayer->net,t);

                txtset(t5,"?"); t5->d2[0]=8; txtadd(t5,tnpc->name); txtadd(t5," resurrected. (F1)");
                for (x2=0;x2<=playerlist_last;x2++){ //create and send UPDATE message
                  if (playerlist[x2]){ //current player
                    if (playerlist[x2]->net!=INVALID_NET){ //net connection available
                      if (playerlist[x2]->party[0]){
                        if (playerlist[x2]!=tplayer){
                          NET_send(NETplayer,playerlist[x2]->net,t5);
                        }}}}}
                inbritanniacheck();
                goto resurrect_failed;
              }//0
            }//player
          }//resu
        }//i
        txtsetchar(t,8); txtadd(t,"You are not dead!"); NET_send(NETplayer,tplayer->net,t);
      }//respawn
resurrect_failed:



      if (playerlist[tpl]->party[0]){

        //move selected party member



        //tplayer=playerlist[tpl];

        myobj=tplayer->party[tplayer->selected_partymember];
        tnpc=(npc*)myobj->more;

        //tpx=myobj->x-15; tpy=myobj->y-11;
        //if (tpx<0) tpx=0;
        //if (tpy<0) tpy=0;
        //if (tpx>2016) tpx=2016;
        //if (tpy>1000) tpy=1000;
        getscreenoffset(myobj->x,myobj->y,&tpx,&tpy);




        //cast spell
        if (tplayer->key&KEYmbclick){


          i=tplayer->mf;

          //change party member?
          if ((i>=16)&&(i<=23)){
            if (tplayer->craft==NULL){
              i-=16;
              x=0;
              for (i2=0;i2<=7;i2++){
                if (tplayer->party[i2]){
                  if (x==i){
                    if (tplayer->party[i2]->x){ //not in the void!
                      tplayer->selected_partymember=i2;
                      tplayer->pathfind=0;
                      goto switchpartymember;
                    }
                  }
                  x++;
                }//->party[i2]
              }//i2
            }//craft
            goto skip_pickup;
switchpartymember:
            for (x2=0;x2<=7;x2++){ if (tplayer->party[x2]){
              myobj2=tplayer->party[x2]; tnpc=(npc*)myobj2->more;
              tnpc->follow=tplayer->party[tplayer->selected_partymember];
              if (x2==tplayer->selected_partymember){
                if (tnpc->order!=3) tnpc->order=0;
              }else{
                if (tnpc->order!=3) tnpc->order=1;
              }
            }}
            goto skip_pickup;
          }

          if (i==24){ //staff cast
            myobj=tplayer->staffcast_npc;
            tnpc=(npc*)myobj->more;
            //x2=tplayer->staffcast_staff->more2&65535;
            staff_cast_spell=1;
            goto staffcast0;
          }

          if ((i>=8)&&(i<=15)){


            static unsigned char iparty; iparty=i-8;
            myobj=tplayer->party[i-8];
            if(!myobj) {
              goto castfailed;
            }
            tnpc=(npc*)myobj->more;
cast_spell0:
            staff_cast_spell=0;
staffcast0:

            if (stormcloakcheck2(myobj->x,myobj->y,tplayer)){
              if (!cast_spell){
                txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," cannot cast magic at this time!"); NET_send(NETplayer,tplayer->net,t);
              }
              goto castfailed;
            }



            CASTSPELL_SPELLTYPE=0;

            static long CASTSPELL_ENUMERATEDINVENTORYOBJS;
            CASTSPELL_ENUMERATEDINVENTORYOBJS=0;

            if (!staff_cast_spell){
              //enumerate reagents!
              ZeroMemory(&treagent,sizeof(treagent));
              x4=OBJlist((object*)tnpc->items->more);
              OBJlist_last=NULL;
              for(x5=0;x5<x4;x5++){
                x3=OBJlist_list[x5]->type;
                if ((x3>=65)&&(x3<=72)){
                  treagent[x3-65]+=OBJlist_list[x5]->more2;
                }
              }
              CASTSPELL_ENUMERATEDINVENTORYOBJS=x4; //number of inventory objects
            }//!staff_cast_spell

            if (!cast_spell){
              x=tplayer->mx; y=tplayer->my&255;
              //fix x and y if using ktar
              if (x&32768){ //spell ktar
                x-=32768;
                if ((x>=0)&&(x<=9)){
                  if (tplayer->ktar_wait[x]){
                    if (tplayer->ktar[x]->more){
                      myobj3=(object*)tplayer->ktar[x]->more;
                      crt=(creature*)myobj3->more; if (crt->flags&2) goto castfailed;
                      y=myobj3->y-tpy;
                      x=myobj3->x-tpx;
                      goto ktarcast2;
                    }//ktar
                  }//wait
                }//x
                goto castfailed;
              }
ktarcast2:
              CASTSPELL_SPELLTYPE=tplayer->my>>8;
            }
            if (staff_cast_spell) CASTSPELL_SPELLTYPE=tplayer->staffcast_staff->more2&65535;

            i2=0; //set to 1 if cast successful

            if (tplayer->craft){
              if (!cast_spell){
                txtset(t,"?"); t->d2[0]=8; txtadd(t,"Not while aboard ship!"); NET_send(NETplayer,tplayer->net,t);
              }
              goto castfailed;
            }

            if (!staff_cast_spell){
              if (tnpc->lev<((CASTSPELL_SPELLTYPE>>4)+1)){
                if (!cast_spell){
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," is not ready for this circle!"); NET_send(NETplayer,tplayer->net,t);
                }
                goto castfailed;
              }

              //check if spell actually exists in spellbook (safeguard!)
              ZeroMemory(&tspell,sizeof(tspell));
              //enumerate spells
              if (myobj2=(object*)tnpc->wep_left){ if (myobj2->type==57){
                myobj2=(object*)myobj2->more;
nextspell3:
                if (myobj2){
                  tspell[myobj2->more2]=1;
                  if (myobj2->next) {myobj2=(object*)myobj2->next; goto nextspell3;}
                }
              }}
              if (myobj2=(object*)tnpc->wep_right){ if (myobj2->type==57){
                myobj2=(object*)myobj2->more;
nextspell4:
                if (myobj2){
                  tspell[myobj2->more2]=1;
                  if (myobj2->next) {myobj2=(object*)myobj2->next; goto nextspell4;}
                }
              }}

			  // c222 spells lookup from lastused spellbook
			  if (easymodehostn1) {
				  if (tspell[CASTSPELL_SPELLTYPE] == 0) {
					  if (myobj2 = playerspellbook) {
						  if (myobj2->type == 57) {
							  myobj2 = (object*)myobj2->more;
							  while (myobj2) {
								  tspell[myobj2->more2] = 1;
								  myobj2 = (object*)myobj2->next;
								  //if (myobj2->next) { myobj2 = (object*)myobj2->next; }
							  }
						  }
					  }
				  }
			  }


              if (tspell[CASTSPELL_SPELLTYPE]==0){
                if (!cast_spell){
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,"No spell!"); NET_send(NETplayer,tplayer->net,t);
                }
                goto castfailed;
              }

              //check reagents for spell
              for (x3=0;x3<=7;x3++){
                if (spellreagent[CASTSPELL_SPELLTYPE]&(1<<x3)){
                  if (treagent[x3]==0){
                    if (!cast_spell){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"No reagents!"); NET_send(NETplayer,tplayer->net,t);
                    }
                    goto castfailed;
                  }
                }
              }

              //check spellpoints for spell
              if (tnpc->mp<((CASTSPELL_SPELLTYPE>>4)+1)){
                if (!cast_spell){
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,"Not enough magic points!"); NET_send(NETplayer,tplayer->net,t);
                }
                goto castfailed;
              }

            }//!staff_cast_spell

            //in range?
            if ((abs((tpx+x)-myobj->x)>8)||(abs((tpy+y)-myobj->y)>8)){
              if (!cast_spell){
                txtset(t,"?"); t->d2[0]=8; txtadd(t,"Out of range!"); NET_send(NETplayer,tplayer->net,t);
              }
              goto castfailed;
            }

            if (tnpc->wait_cast||tnpc->wait_disable){
              if (!cast_spell){
                txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," busy!"); NET_send(NETplayer,tplayer->net,t);
              }
              goto castfailed;
            }

            //staff enchant?
            if (CASTSPELL_SPELLTYPE!=((6<<4)+1)){//not the enchant spell
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){
                i2=2;
                if (myobj2->type==78){//dest location has a staff
                  if ((CASTSPELL_SPELLTYPE==(myobj2->more2&65535))||((myobj2->more2&65535)==0)){
                    x3=(myobj2->info>>9)&7;
                    if (x3){//enchantment possible!

                      if (OBJcheckbolt(myobj->x,myobj->y,tpx+x,tpy+y)){
                        if (!cast_spell){
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"Blocked!"); NET_send(NETplayer,tplayer->net,t);
                        }
                        goto castfailed;
                      }

                      i2=1;
                      myobj2->more2|=CASTSPELL_SPELLTYPE;
                      myobj2->info=myobj2->info&(0xFFFF-(7<<9)); x3--; myobj2->info|=(x3<<9);
                      x3=myobj2->more2>>16;
                      myobj2->more2=myobj2->more2&0xFFFF; x3++; myobj2->more2|=(x3<<16);
                      txtset(t,"?"); t->d2[0]=8; 
                      txtadd(t,"Staff successfully enchanted with the ");
                      txtset(t2,spellname[CASTSPELL_SPELLTYPE]);
                      txtlcase(t2);
                      txtadd(t,t2);
                      txtadd(t," spell (x");
                      txtnumint(t2,myobj2->more2>>16); txtadd(t,t2);
                      txtadd(t,").");
                      if ((myobj2->info>>9)&7){
                        txtadd(t," ");
                        txtnumint(t2,(myobj2->info>>9)&7); txtadd(t,t2);
                        txtadd(t," more enchantments left.");
                      }
                      NET_send(NETplayer,tplayer->net,t);
                      goto spelldone;
                    }//x3
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,"You must cast enchant upon this staff before enchanting it with spells!"); NET_send(NETplayer,tplayer->net,t);
                    goto spelldone;
                  }//CASTSPELL_SPELLTYPE
                  goto spelldone;
                }//staff
              }//findlast
            }//CASTSPELL_SPELLTYPE!=enchant spell

            //3. cast spell
            static long spellx,spelly,spellx2,spelly2,spellz,spellz2;

            if (CASTSPELL_SPELLTYPE==((6<<4)+9)){ //wizard eye //txtset(spellname[(6<<4)+9],"WizardEye"); spellreagent[(6<<4)+9]=BLOO|NIGH|MAND|SULF|BLAC|SPID;
              i2=2; //implemented!
              if (tnpc->player->wizardeyetimeleft==0.0f){
                if (i9=rnd*(tnpc->i+1)){
                  i2=1;
                  tnpc->player->wizardeyetimeleft=i9; tnpc->player->wizardeyemovewait=0.0f;
                  tnpc->player->wizardeyex=myobj->x; tnpc->player->wizardeyey=myobj->y;
                  txtset(t,"??"); t->d2[0]=36; t->d2[1]=i9; NET_send(NETplayer,tnpc->player->net,t);
                }else{
                  i2=3;
                }
              }//timeleft==0
              goto spelldone;
            }//wizardeye

            if (CASTSPELL_SPELLTYPE==((2<<4)+7)){ //peer txtset(spellname[(2<<4)+7],"Peer"); spellreagent[(2<<4)+7]=NIGH|MAND;
              i2=2; //implemented!
              x3=rnd*(8+5*3);
              x4=rnd*(tnpc->i+10);
              if (x4>=x3){//success
                x3=rnd*(32+(tnpc->i>>3));
                if (x3){
                  if (x3>255) x3=255; //cap
                  if (x3>tnpc->player->peer){
                    tnpc->player->peer=x3;
                    //send peer update message, changed to 48, 34 was already in use, but not in client side, so I don't know if it messed up anything or not. probably not
                    txtset(t,"??"); t->d2[0]=48; t->d2[1]=tnpc->player->peer; NET_send(NETplayer,tnpc->player->net,t);
                  }
                  i2=1;
                }
                if (x3==0) i2=3;
              } else {i2=3;} //fail
              goto spelldone;
            }//peer





            //txtset(spellname[(1<<4)+5],"Telekinesis"); spellreagent[(1<<4)+5]=BLOO|MAND|BLAC;
            if (CASTSPELL_SPELLTYPE==((1<<4)+5)) { //telekinesis   
              i2=spell_telekinesis(tplayer,tnpc,NULL,tpx+x,tpy+y); 
              goto spelldone;
            }//telekinesis


            if (CASTSPELL_SPELLTYPE==((7<<4)+0)){//death wind //txtset(spellname[(7<<4)+0],"DeathWind"); spellreagent[(7<<4)+0]=MAND|NIGH|SULF|BLOO; //level 8
              i2=2;
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              spellx=tpx+x; spelly=tpy+y;
              getwindspell(myobj->x,myobj->y,spellx,spelly);
              //add special effects
              for (z=0;z<=WINDSPELL_boltn;z++){
                z2=SFnew(WINDSPELL_boltx[z],WINDSPELL_bolty[z]);
                sf[z2].type=25;//multiple death bolts
                sf[z2].x=myobj->x;
                sf[z2].y=myobj->y;
                sf[z2].x2=WINDSPELL_boltx[z];
                sf[z2].y2=WINDSPELL_bolty[z];
                sf[z2].more=0xFFFF;
                sf[z2].wait=1;
              }//z
              for (spellz=0;spellz<=WINDSPELL_n;spellz++){
                if (spellx=WINDSPELL_x[spellz]){
                  spelly=WINDSPELL_y[spellz];
                  if (stormcloakcheck2(spellx,spelly,tplayer)==0){
                    myobj2=OBJfindlast(spellx,spelly); if (myobj2==NULL) myobj2=OBJfindlastall(spellx,spelly);
                    if (myobj2){
                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        i9=i3; i8=x2; //backup!
                        x2=rnd*(512+tnpc->i*8);
                        x3=rnd*(512+tnpc->i*8);
                        x2=((x2+x3)>>1);
                        if (x2<crt->hp) {i2=1; x2=i8; i3=i9; goto deathwind_spelldone;}
                        i3=myobj2->type&1023;
                        if (i3==352) {x2=0; goto IDimmunedeath8;} //ghost
                        if (i3==368) {x2=0; goto IDimmunedeath8;} //skeleton
                        if (i3==373) {x2=0; goto IDimmunedeath8;} //wisp
                        if (i3==367) {x2=0; goto IDimmunedeath8;} //daemon
                        i3=SFnew(myobj2->x,myobj2->y);
                        sf[i3].type=1; //attack
                        sf[i3].x2=x2;
                        sf[i3].x=myobj2->x;
                        sf[i3].y=myobj2->y;
                        sf[i3].wait=0.125f; //NULL
                        sf[i3].more=1;
IDimmunedeath8:
                        spellattcrt=TRUE; goto spellattcrt0;
                      }//myobj2
                    }//crt
                  }//stormcloakcheck2
                }//spellx check
deathwind_spelldone:;
              }//spellz
              i2=1;
              goto spelldone;
            }//deathwind




            if (CASTSPELL_SPELLTYPE==((6<<4)+2)){//energy wind //txtset(spellname[(6<<4)+2],"EnergyWind"); spellreagent[(6<<4)+2]=MAND|NIGH|SULF|BLOO;
              i2=2;
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              spellx=tpx+x; spelly=tpy+y;
              getwindspell(myobj->x,myobj->y,spellx,spelly);
              //add special effects
              for (z=0;z<=WINDSPELL_boltn;z++){
                z2=SFnew(WINDSPELL_boltx[z],WINDSPELL_bolty[z]);
                sf[z2].type=24;//multiple lightning!
                sf[z2].x=myobj->x;
                sf[z2].y=myobj->y;
                sf[z2].x2=WINDSPELL_boltx[z];
                sf[z2].y2=WINDSPELL_bolty[z];
                sf[z2].more=0xFFFF;
                sf[z2].wait=1;
              }//z
              for (spellz=0;spellz<=WINDSPELL_n;spellz++){
                if (spellx=WINDSPELL_x[spellz]){
                  spelly=WINDSPELL_y[spellz];
                  if (stormcloakcheck2(spellx,spelly,tplayer)==0){
                    myobj2=OBJfindlast(spellx,spelly); if (myobj2==NULL) myobj2=OBJfindlastall(spellx,spelly);
                    if (myobj2){
                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        i9=i3; i8=x2; //backup!
                        x2=rnd*(64+tnpc->i*2);
                        if (x2==0) {i2=3; x2=i8; i3=i9; goto energywind_spelldone;}
                        i3=SFnew(myobj2->x,myobj2->y);
                        sf[i3].type=1; //attack
                        sf[i3].x2=x2;
                        sf[i3].x=myobj2->x;
                        sf[i3].y=myobj2->y;
                        sf[i3].wait=0.125f; //NULL
                        sf[i3].more=1;
                        spellattcrt=TRUE; goto spellattcrt0;
                      }//myobj2
                    }//crt
                  }//stormcloakcheck2
                }//spellx check
energywind_spelldone:;
              }//spellz
              i2=1;
              goto spelldone;
            }//energywind

            if (CASTSPELL_SPELLTYPE==((5<<4)+7)){//poison wind //txtset(spellname[(5<<4)+7],"PoisonWind"); spellreagent[(5<<4)+7]=NIGH|SULF|BLOO;
              i2=2;
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              spellx=tpx+x; spelly=tpy+y;
              getwindspell(myobj->x,myobj->y,spellx,spelly);
              //add special effects
              for (z=0;z<=WINDSPELL_boltn;z++){
                z2=SFnew(WINDSPELL_boltx[z],WINDSPELL_bolty[z]);
                sf[z2].type=23;//multiple green balls
                sf[z2].x=myobj->x;
                sf[z2].y=myobj->y;
                sf[z2].x2=WINDSPELL_boltx[z];
                sf[z2].y2=WINDSPELL_bolty[z];
                sf[z2].more=0xFFFF;
                sf[z2].wait=1;
              }//z
              for (spellz=0;spellz<=WINDSPELL_n;spellz++){
                if (spellx=WINDSPELL_x[spellz]){
                  spelly=WINDSPELL_y[spellz];
                  if (stormcloakcheck2(spellx,spelly,tplayer)==0){
                    myobj2=OBJfindlast(spellx,spelly); if (myobj2==NULL) myobj2=OBJfindlastall(spellx,spelly);
                    if (myobj2){
                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        x3=rnd*(8+5*2);
                        x4=rnd*(tnpc->i+10);
                        if (x4>=x3){//success
                          crt->flags|=1;
                          z2=SFnew(myobj2->x,myobj2->y);
                          sf[z2].type=1; //attack
                          sf[z2].x2=0;
                          sf[z2].x=myobj2->x;
                          sf[z2].y=myobj2->y;
                          sf[z2].wait=0.125f; //NULL
                          sf[z2].more=1;
                        }//success
                      }//crt
                    }//myobj2
                  }//stormcloakcheck2
                }//spellx check
              }//spellz
              i2=1;
              goto spelldone;
            }//poisonwind

            if (CASTSPELL_SPELLTYPE==((5<<4)+3)){//flame wind //txtset(spellname[(5<<4)+3],"FlameWind"); spellreagent[(5<<4)+3]=SULF|BLOO|MAND;
              i2=2;
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              spellx=tpx+x; spelly=tpy+y;
              getwindspell(myobj->x,myobj->y,spellx,spelly);
              //add special effects
              for (z=0;z<=WINDSPELL_boltn;z++){
                z2=SFnew(WINDSPELL_boltx[z],WINDSPELL_bolty[z]);
                sf[z2].type=22;//multiple fireballs
                sf[z2].x=myobj->x;
                sf[z2].y=myobj->y;
                sf[z2].x2=WINDSPELL_boltx[z];
                sf[z2].y2=WINDSPELL_bolty[z];
                sf[z2].more=0xFFFF;
                sf[z2].wait=1;
              }//z
              for (spellz=0;spellz<=WINDSPELL_n;spellz++){
                if (spellx=WINDSPELL_x[spellz]){
                  spelly=WINDSPELL_y[spellz];
                  if (stormcloakcheck2(spellx,spelly,tplayer)==0){
                    myobj2=OBJfindlast(spellx,spelly); if (myobj2==NULL) myobj2=OBJfindlastall(spellx,spelly);
                    if (myobj2){
                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        i9=i3; i8=x2; //backup!
                        x2=rnd*(48+tnpc->i+(tnpc->i>>1));
                        if (x2==0) {i2=1; x2=i8; i3=i9; goto flamewind_spelldone;}
                        //fire resistant???
                        i3=myobj2->type&1023;
                        if (i3==411) {x2=0; goto IFimmuneflame4;} //dragon
                        if (i3==369) {x2=0; goto IFimmuneflame4;} //drake
                        if (i3==352) {x2=0; goto IFimmuneflame4;} //ghost
                        if (i3==374) {x2=0; goto IFimmuneflame4;} //hydra
                        if (i3==369) {x2=0; goto IFimmuneflame4;} //drake
                        if (i3==368) {x2=0; goto IFimmuneflame4;} //skeleton
                        if (i3==373) {x2=0; goto IFimmuneflame4;} //wisp
                        if (i3==367) {x2=0; goto IFimmuneflame4;} //daemon
                        //double damage?
                        if (i3==364) x2*=2; //acid slug
                        if (i3==357) x2*=2; //corpser
                        if (i3==347) x2*=2; //reaper
                        if (i3==360) x2*=2; //rot worms
                        if (i3==375) x2*=2; //slime
                        if (i3==365) x2*=2; //tangle vine
                        i3=SFnew(myobj2->x,myobj2->y);
                        sf[i3].type=1; //attack
                        sf[i3].x2=x2;
                        sf[i3].x=myobj2->x;
                        sf[i3].y=myobj2->y;
                        sf[i3].wait=0.125f; //NULL
                        sf[i3].more=1;
IFimmuneflame4:
                        spellattcrt=TRUE; goto spellattcrt0;
                      }//myobj2
                    }//crt
                  }//stormcloakcheck2
                }//spellx check
flamewind_spelldone:;
              }//spellz
              i2=1;
              goto spelldone;
            }//flamewind


            if (OBJcheckbolt(myobj->x,myobj->y,tpx+x,tpy+y)){
              if (!cast_spell){
                txtset(t,"?"); t->d2[0]=8; txtadd(t,"Blocked!"); NET_send(NETplayer,tplayer->net,t);
              }
              goto castfailed;
            }
            //spell blocked???????????????????????????????????????????????

            if (CASTSPELL_SPELLTYPE==((7<<4)+4)){ //resurrect //txtset(spellname[(7<<4)+4],"Resurrect"); spellreagent[(7<<4)+4]=GARL|GINS|SPID|SULF|BLOO|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2){
                if ((myobj2->type&1023)==339){ //dead body

                  //***RESRTICTIONS: SOME PLACES IN BRITANNIA DEAD BODIES CANNOT BE RESURRECTED***
                  x4=tpx+x; y4=tpy+y;
                  //1. SACRED QUEST AREA x1 899 y1 811 x2 948 y2 872
                  if (x4>=899){ if (x4<=948){ if (y4>=811){ if (y4<=872){
                    goto spelldone;
                  }}}}
                  //2. PLAYER HOUSING
                  x6=housecheck(x4,y4);
                  if(x6) {
                    goto spelldone;
                  }
                  //3. PLAYER HOUSING - STORAGE
                  for (x5=1;x5<=255;x5++){
                    for (y5=0;y5<housestoragenext[x5];y5++){
                      if (x4==housestoragex[x5][y5]){ if (y4==housestoragey[x5][y5]){
                        goto spelldone;
                      }}//x,y
                    }//y5
                  }//x5

                  //4. WALKABLE SQUARE
                  if ((bt[y4][x4]&1024)==0){
                    goto spelldone;
                  }

                  //5. SHRINE IN THE VOID
                  if (x4>=1024){ if (x4<=1071){ if (y4>=0){ if (y4<=47){
                    goto spelldone;
                  }}}}


                  //scan resurrect list
                  for (i3=0;i3<=nresu;i3++){
                    if (resu[i3]){
                      if (resu_body[i3]==myobj2){
                        if (resu_player[i3]){ //has a player pointer

                          //resurrection can be performed
                          i2=1;
                          x3=rnd*(8+5*8);
                          x4=rnd*(tnpc->i+50);
                          if (x4>=x3){//success
                            //resu_player[i3]->party[0]=resu[i3]; //assume current player
                            resu_player[i3]->party[resu_partymember[i3]]=resu[i3];

                            if (resu_partymember[i3]==0){
                              for (x4=1;x4<=7;x4++){
                                if (resu_player[i3]->party[x4]){
                                  tnpc3=(npc*)resu_player[i3]->party[x4]->more; tnpc3->upflags=1;
                                  OBJmove_allow=TRUE;
                                  OBJmove2(resu_player[i3]->party[x4],myobj2->x,myobj2->y); //place party on the map
                                  OBJmove_allow=FALSE;
                                }
                              }//x4
                            }

                            OBJmove_allow=TRUE;
                            OBJmove2(resu[i3],myobj2->x,myobj2->y); //place avatar on the map
                            OBJmove_allow=FALSE;
                            OBJremove(myobj2); //remove body
                            tnpc3=(npc*)resu[i3]->more;
                            resu[i3]=NULL;
                            txtset(t5,"?"); t5->d2[0]=8; txtadd(t5,"A voice in the darkness intones, \x022"); txtadd(t5,"IN MANI CORP!\x022"); NET_send(NETplayer,resu_player[i3]->net,t5);

                            txtset(t5,"?"); t5->d2[0]=8; txtadd(t5,tnpc3->name); txtadd(t5," resurrected by "); txtadd(t5,tnpc->name); txtadd(t5,".");
                            for (x3=0;x3<=playerlist_last;x3++){ //create and send UPDATE message
                              if (playerlist[x3]){ //current player
                                if (playerlist[x3]->net!=NULL){ //net connection available
                                  if (playerlist[x3]->party[0]){
                                    if (playerlist[x3]!=tplayer){
                                      NET_send(NETplayer,playerlist[x3]->net,t5);
                                    }}}}}
                            inbritanniacheck();
                            i2=1;
                          }else{i2=3;
                          if (resu_partymember[i3]==0) goto spelldone_donttakereagents;//add bug!
                          }
                          goto resurrect_spell_done;
                        }//player
                      }//resu_body==obj
                    }//resu
                  }//i3
                }//dead body
              }//myobj2
resurrect_spell_done:
              goto spelldone;
            }//resurrect



            if (CASTSPELL_SPELLTYPE==((3<<4)+4)){ //great heal //txtset(spellname[(3<<4)+4],"GreatHeal"); spellreagent[(3<<4)+4]=GINS|SPID|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    //perform light heal (1 to 64->128 hp)
                    x3=rnd*(64+tnpc->i); //int can only double spell effectiveness!
                    x4=rnd*(64+tnpc->i); //int can only double spell effectiveness!
                    x3=(x3+x4)>>1;

                    tnpc2->hp+=x3; if (tnpc2->hp>tnpc2->hp_max) tnpc2->hp=tnpc2->hp_max;
                    tnpc2->upflags|=2;
                    i2=1;
                    if (x3==0) i2=3;
                    if (x3){
                      i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                      sf[i9].type=14; //blue ball
                      sf[i9].x=myobj->x;
                      sf[i9].y=myobj->y;
                      sf[i9].x2=myobj2->x;
                      sf[i9].y2=myobj2->y;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=1;
                    }
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//great heal



            if (CASTSPELL_SPELLTYPE==((7<<4)+3)){ //mass kill! //txtset(spellname[(7<<4)+3],"MassKill"); spellreagent[(7<<4)+3]=BLAC|NIGH|MAND|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;

              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto masskillblocked;

                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                  if (myobj2){
                    if (myobj2->info&4){ //<-crt

                      crt=(creature*)myobj2->more;
                      i9=i3; i8=x2; //backup!
                      x2=rnd*(512+tnpc->i*8);
                      x3=rnd*(512+tnpc->i*8);
                      x2=((x2+x3)>>1);
                      if (x2<crt->hp) {i2=1; x2=i8; i3=i9; goto masskill_spelldone;} //failed!
                      i3=myobj2->type&1023;
                      if (i3==352) {x2=0; goto IDimmunedeath2;} //ghost
                      if (i3==368) {x2=0; goto IDimmunedeath2;} //skeleton
                      if (i3==373) {x2=0; goto IDimmunedeath2;} //wisp
                      if (i3==367) {x2=0; goto IDimmunedeath2;} //daemon

                      i3=SFnew(myobj2->x,myobj2->y);
                      sf[i3].type=1; //attack
                      sf[i3].x2=x2;
                      sf[i3].x=myobj2->x;
                      sf[i3].y=myobj2->y;
                      sf[i3].wait=0.125f; //NULL
                      sf[i3].more=1;
IDimmunedeath2:
                      spellattcrt=TRUE; goto spellattcrt0;
                    }//crt
                  }//myobj2

masskill_spelldone:;

                  if ((abs(spellx)==2)||(abs(spelly)==2)){
                    i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                    sf[i9].type=15; //kill blast
                    sf[i9].x=tpx+x;
                    sf[i9].y=tpy+y;
                    sf[i9].x2=tpx+x+spellx;
                    sf[i9].y2=tpy+y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }

masskillblocked:;
                }//!=4
              }}//spell x,y
              i2=1;

              goto spelldone;
            }//mass kill!

            if (CASTSPELL_SPELLTYPE==((3<<4)+2)){ //disable! //txtset(spellname[(3<<4)+2],"Disable"); spellreagent[(3<<4)+2]=NIGH|SPID|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  //x2=rnd*(256+tnpc->i*4);
                  //x3=rnd*(256+tnpc->i*4);
                  x2=rnd*(192+tnpc->i*3);
                  x3=rnd*(192+tnpc->i*3);
                  x2=((x2+x3)>>1);
                  if (x2<crt->hp) {i2=3; x2=i8; i3=i9; goto spelldone;} //failed!
                  x2=crt->hp-1;
                  i3=SFnew(myobj2->x,myobj2->y);
                  sf[i3].type=1; //attack
                  sf[i3].x2=x2;
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;
                  i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i3].type=15; //kill/disable blast
                  sf[i3].x=myobj->x;
                  sf[i3].y=myobj->y;
                  sf[i3].x2=myobj2->x;
                  sf[i3].y2=myobj2->y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=1;
                  spellattcrt=TRUE; goto spellattcrt0;
                }//crt
              }//myobj2
              goto spelldone;
            }//disable!

            if (CASTSPELL_SPELLTYPE==((6<<4)+5)){ //kill! //txtset(spellname[(6<<4)+5],"Kill"); spellreagent[(6<<4)+5]=BLAC|NIGH|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  x2=rnd*(512+tnpc->i*8);
                  x3=rnd*(512+tnpc->i*8);
                  x2=((x2+x3)>>1);
                  if (x2<crt->hp) {i2=3; x2=i8; i3=i9; goto spelldone;} //failed!
                  i3=myobj2->type&1023;
                  if (i3==352) {x2=0; goto IDimmunedeath;} //ghost
                  if (i3==368) {x2=0; goto IDimmunedeath;} //skeleton
                  if (i3==373) {x2=0; goto IDimmunedeath;} //wisp
                  if (i3==367) {x2=0; goto IDimmunedeath;} //daemon

                  i3=SFnew(myobj2->x,myobj2->y);
                  sf[i3].type=1; //attack
                  sf[i3].x2=x2;
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;
IDimmunedeath:
                  i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i3].type=15; //kill blast
                  sf[i3].x=myobj->x;
                  sf[i3].y=myobj->y;
                  sf[i3].x2=myobj2->x;
                  sf[i3].y2=myobj2->y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=1;
                  if (x2==0) {i2=3; x2=i8; i3=i9; goto spelldone;}
                  spellattcrt=TRUE; goto spellattcrt0;
                }//crt
              }//myobj2
              goto spelldone;
            }//kill!

            if (CASTSPELL_SPELLTYPE==((4<<4)+4)){ //lightning //txtset(spellname[(4<<4)+4],"Lightning"); spellreagent[(4<<4)+4]=BLAC|MAND|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  x2=rnd*(64+tnpc->i*2);
                  if (x2==0) {i2=3; x2=i8; i3=i9; goto spelldone;}
                  i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i3].type=5;
                  sf[i3].x=myobj->x;
                  sf[i3].y=myobj->y;
                  sf[i3].x2=myobj2->x;
                  sf[i3].y2=myobj2->y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=1;
                  i3=SFnew(myobj2->x,myobj2->y);
                  sf[i3].type=1; //attack
                  sf[i3].x2=x2;
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;
                  spellattcrt=TRUE; goto spellattcrt0;
                }//crt
              }//myobj2
              goto spelldone;
            }//lightning

            if (CASTSPELL_SPELLTYPE==((6<<4)+0)){ //chain bolt //txtset(spellname[(6<<4)+0],"ChainBolt"); spellreagent[(6<<4)+0]=BLAC|MAND|SULF|BLOO; //level 7
              static long chainbolt_hits,chainbolt_miss; chainbolt_hits=0; chainbolt_miss=0;
              i2=2;//implemented!
              spellx=tpx+x; spelly=tpy+y;
              if (stormcloakcheck2(spellx,spelly,tplayer)) goto spelldone;
              myobj2=OBJfindlast(spellx,spelly); if (myobj2==NULL) myobj2=OBJfindlastall(spellx,spelly);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  i2=3;
chainbolt_gotnexttarget:
                  chainbolt_miss=0;
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  x2=rnd*(64+tnpc->i*2);
                  if (x2==0) {x2=i8; i3=i9; goto chainbolt_spelldone;}
                  i2=1;
                  i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i3].type=5+chainbolt_hits*2*256;
                  if (chainbolt_hits){
                    sf[i3].x=spellx2;
                    sf[i3].y=spelly2;
                  }else{
                    sf[i3].x=myobj->x;
                    sf[i3].y=myobj->y;
                  }
                  sf[i3].x2=myobj2->x;
                  sf[i3].y2=myobj2->y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=1;
                  i3=SFnew(myobj2->x,myobj2->y);
                  sf[i3].type=1+chainbolt_hits*2*256; //attack
                  sf[i3].x2=x2;
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;
                  spellattcrt=TRUE; goto spellattcrt0;
chainbolt_spelldone:
                  if (chainbolt_hits<=7){
                    //random scanning
                    spellx2=((long(rnd*17)-8)+(long(rnd*17)-8))/2; spelly2=((long(rnd*17)-8)+(long(rnd*17)-8))/2; 
                    if (spellx2&&spelly2){//both not 0
                      spellx2+=spellx; spelly2+=spelly;
                      if (spellx2>=0){ if (spelly2>=0){ if (spellx2<2048){ if (spelly2<1024){//map boundries check
                        if (!stormcloakcheck2(spellx2,spelly2,tplayer)){
                          if (!OBJcheckbolt(spellx,spelly,spellx2,spelly2)){
                            myobj2=OBJfindlast(spellx2,spelly2); if (myobj2==NULL) myobj2=OBJfindlastall(spellx2,spelly2);
                            if (myobj2){
                              if (myobj2->info&4){ //<-crt
                                chainbolt_hits++;
                                i9=spellx; spellx=spellx2; spellx2=i9;
                                i9=spelly; spelly=spelly2; spelly2=i9;
                                goto chainbolt_gotnexttarget;
                              }//crt
                            }//myobj2
                          }//checkbolt
                        }//stormcloakcheck
                      }}}}//boundries
                    }//both not 0
                    chainbolt_miss++;
                    i9=128-chainbolt_hits*16+tnpc->i*2; if (chainbolt_miss<i9) goto chainbolt_spelldone;
                  }//chainbolthits
                }//crt
              }//myobj2
              goto spelldone;
            }//chainbolt

            if (CASTSPELL_SPELLTYPE==((7<<4)+8)){ //tremor //txtset(spellname[(7<<4)+8],"Tremor"); spellreagent[(7<<4)+8]=BLOO|SULF|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              i2=1;
              for (spelly2=tpy+y-23;spelly2<=tpy+y+23;spelly2++){
                for (spellx2=tpx+x-31;spellx2<=tpx+x+31;spellx2++){
                  myobj2=OBJfindlast(spellx2,spelly2); if (myobj2==NULL) myobj2=OBJfindlastall(spellx2,spelly2);
                  if (myobj2){
                    if (myobj2->info&4){ //<-crt
                      if (spellx2>=0){ if (spelly2>=0){ if (spellx2<2048){ if (spelly2<1024){//map boundries check
                        if (!stormcloakcheck2(spellx2,spelly2,tplayer)){
                          crt=(creature*)myobj2->more;
                          i9=i3; i8=x2; //backup!
                          x3=abs(spellx2-(tpx+x)); x4=abs(spelly2-(tpy+y)); if (x4>x3) x3=x4;
                          x3*=3;
                          x2=rnd*(80+tnpc->i+(tnpc->i>>1));
                          x2-=x3;
                          if (x2<=0) {x2=i8; i3=i9; goto tremor_spelldone;}
                          i3=SFnew(myobj2->x,myobj2->y);
                          sf[i3].type=1; //attack
                          sf[i3].x2=x2;
                          sf[i3].x=myobj2->x;
                          sf[i3].y=myobj2->y;
                          sf[i3].wait=0.125f; //NULL
                          sf[i3].more=1;
                          spellattcrt=TRUE; goto spellattcrt0;
                        }//stormcloakcheck
                      }}}}//boundries
                    }}//myobj2,crt
tremor_spelldone:;
                }}
              i9=SFnew(tpx+x,tpy+y);
              sf[i9].type=26;//tremor
              sf[i9].wait=1;
              goto spelldone;
            }//tremor


            if (CASTSPELL_SPELLTYPE==((4<<4)+1)){ //explosion //txtset(spellname[(4<<4)+1],"Explosion"); spellreagent[(4<<4)+1]=MAND|SULF|BLAC|BLOO;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;

              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto explosionblocked;

                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                  if (myobj2){
                    if (myobj2->info&4){ //<-crt

                      crt=(creature*)myobj2->more;
                      i9=i3; i8=x2; //backup!
                      x2=rnd*(48+tnpc->i+(tnpc->i>>1));
                      if (x2==0) {i2=1; x2=i8; i3=i9; goto explosion_spelldone;}
                      //fire resistant???
                      i3=myobj2->type&1023;
                      if (i3==411) {x2=0; goto IFimmuneflame2;} //dragon
                      if (i3==369) {x2=0; goto IFimmuneflame2;} //drake
                      if (i3==352) {x2=0; goto IFimmuneflame2;} //ghost
                      if (i3==374) {x2=0; goto IFimmuneflame2;} //hydra
                      if (i3==369) {x2=0; goto IFimmuneflame2;} //drake
                      if (i3==368) {x2=0; goto IFimmuneflame2;} //skeleton
                      if (i3==373) {x2=0; goto IFimmuneflame2;} //wisp
                      if (i3==367) {x2=0; goto IFimmuneflame2;} //daemon

                      //double damage?
                      if (i3==364) x2*=2; //acid slug
                      if (i3==357) x2*=2; //corpser
                      if (i3==347) x2*=2; //reaper
                      if (i3==360) x2*=2; //rot worms
                      if (i3==375) x2*=2; //slime
                      if (i3==365) x2*=2; //tangle vine

                      i3=SFnew(myobj2->x,myobj2->y);
                      sf[i3].type=1; //attack
                      sf[i3].x2=x2;
                      sf[i3].x=myobj2->x;
                      sf[i3].y=myobj2->y;
                      sf[i3].wait=0.125f; //NULL
                      sf[i3].more=1;

IFimmuneflame2:
                      spellattcrt=TRUE; goto spellattcrt0;
                    }//crt
                  }//myobj2

explosion_spelldone:;

                  if ((abs(spellx)==2)||(abs(spelly)==2)){
                    i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                    sf[i9].type=8; //wand
                    sf[i9].x=tpx+x;
                    sf[i9].y=tpy+y;
                    sf[i9].x2=tpx+x+spellx;
                    sf[i9].y2=tpy+y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }

explosionblocked:;
                }//!=4
              }}//spell x,y
              i2=1;

              goto spelldone;
            }//explosion


            if (CASTSPELL_SPELLTYPE==((2<<4)+2)){ //fireball //txtset(spellname[(2<<4)+2],"Fireball"); spellreagent[(2<<4)+2]=SULF|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  x2=rnd*(48+tnpc->i+(tnpc->i>>1));
                  if (x2==0) {i2=3; x2=i8; i3=i9; goto spelldone;}
                  //fire resistant???
                  i3=myobj2->type&1023;
                  if (i3==411) {x2=0; goto IFimmuneflame;} //dragon
                  if (i3==369) {x2=0; goto IFimmuneflame;} //drake
                  if (i3==352) {x2=0; goto IFimmuneflame;} //ghost
                  if (i3==374) {x2=0; goto IFimmuneflame;} //hydra
                  if (i3==369) {x2=0; goto IFimmuneflame;} //drake
                  if (i3==368) {x2=0; goto IFimmuneflame;} //skeleton
                  if (i3==373) {x2=0; goto IFimmuneflame;} //wisp
                  if (i3==367) {x2=0; goto IFimmuneflame;} //daemon

                  //double damage?
                  if (i3==364) x2*=2; //acid slug
                  if (i3==357) x2*=2; //corpser
                  if (i3==347) x2*=2; //reaper
                  if (i3==360) x2*=2; //rot worms
                  if (i3==375) x2*=2; //slime
                  if (i3==365) x2*=2; //tangle vine

                  i3=SFnew(myobj2->x,myobj2->y);
                  sf[i3].type=1; //attack
                  sf[i3].x2=x2;
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;

IFimmuneflame:
                  i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i3].type=8; //wand
                  sf[i3].x=myobj->x;
                  sf[i3].y=myobj->y;
                  sf[i3].x2=myobj2->x;
                  sf[i3].y2=myobj2->y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=1;
                  spellattcrt=TRUE; goto spellattcrt0;
                }//crt
              }//myobj2
              goto spelldone;
            }//fireball



            if (CASTSPELL_SPELLTYPE==((1<<4)+1)){ //magic arrow //txtset(spellname[(1<<4)+1],"MagicArrow"); spellreagent[(1<<4)+1]=SULF|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  x2=rnd*(32+tnpc->i);
                  if (x2==0) {i2=3; x2=i8; i3=i9; goto spelldone;}
                  i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i3].type=2; //arrow
                  sf[i3].x=myobj->x;
                  sf[i3].y=myobj->y;
                  sf[i3].x2=myobj2->x;
                  sf[i3].y2=myobj2->y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=1;
                  i3=SFnew(myobj2->x,myobj2->y);
                  sf[i3].type=1; //attack
                  sf[i3].x2=x2;
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;
                  spellattcrt=TRUE; goto spellattcrt0;
                }//crt
              }//myobj2
              goto spelldone;
            }//magic arrow

            if (CASTSPELL_SPELLTYPE==((5<<4)+0)){ //charm //txtset(spellname[(5<<4)+0],"Charm"); spellreagent[(5<<4)+0]=BLAC|NIGH|SPID; //level 6
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  if (crt->al==1){
                    x3=rnd*(128+(tnpc->i*2)); if (x3>255) x3=255;
                    if (x3==0) {i2=3; goto spelldone;}
                    i2=1;
                    crt->al=4;
                    crt->target=NULL;
                    crt->flags|=32;
                    if (x3>crt->charm) crt->charm=x3;
                    i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                    sf[i9].type=14; //blue ball
                    sf[i9].x=myobj->x;
                    sf[i9].y=myobj->y;
                    sf[i9].x2=myobj2->x;
                    sf[i9].y2=myobj2->y;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }//al==1
                }//crt
              }//myobj2
              goto spelldone;
            }//charm

            if (CASTSPELL_SPELLTYPE==((7<<4)+2)){ //mass charm //txtset(spellname[(7<<4)+2],"MassCharm"); spellreagent[(7<<4)+2]=BLAC|NIGH|SPID|MAND;
              i2=2;
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              i2=1;
              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto masscharmblocked;

                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                  if (myobj2){
                    if (myobj2->info&4){ //<-crt
                      crt=(creature*)myobj2->more;
                      if (crt->al==1){
                        x3=rnd*(128+(tnpc->i*2)); if (x3>255) x3=255;
                        if (x3){
                          crt->al=4;
                          crt->target=NULL;
                          crt->flags|=32;
                          if (x3>crt->charm) crt->charm=x3;
                        }//x3
                      }//al==1
                    }//crt
                  }//myobj2
                  if ((abs(spellx)==2)||(abs(spelly)==2)){
                    i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                    sf[i9].type=14; //blue ball
                    sf[i9].x=tpx+x;
                    sf[i9].y=tpy+y;
                    sf[i9].x2=tpx+x+spellx;
                    sf[i9].y2=tpy+y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }
masscharmblocked:;
                }}}//4,x,y
              goto spelldone;
            }//mass charm

            if (CASTSPELL_SPELLTYPE==((0<<4)+5)){ //harm //txtset(spellname[(0<<4)+5],"Harm"); spellreagent[(0<<4)+5]=NIGH|SPID;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  x2=rnd*(24+(tnpc->i>>1)); //32
                  if (x2==0) {i2=3; x2=i8; i3=i9; goto spelldone;}
                  i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i3].type=14; //blue ball
                  sf[i3].x=myobj->x;
                  sf[i3].y=myobj->y;
                  sf[i3].x2=myobj2->x;
                  sf[i3].y2=myobj2->y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=1;
                  i3=SFnew(myobj2->x,myobj2->y);
                  sf[i3].type=1; //attack
                  sf[i3].x2=x2;
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].wait=0.125f; //NULL
                  sf[i3].more=1;
                  spellattcrt=TRUE; goto spellattcrt0;

                  //return branch for varied spells
spellattcrt1: i2=1; x2=i8; i3=i9; //<-
                  if (CASTSPELL_SPELLTYPE==((4<<4)+1)) goto explosion_spelldone;
                  if (CASTSPELL_SPELLTYPE==((7<<4)+3)) goto masskill_spelldone;
                  if (CASTSPELL_SPELLTYPE==((5<<4)+3)) goto flamewind_spelldone;
                  if (CASTSPELL_SPELLTYPE==((6<<4)+2)) goto energywind_spelldone;
                  if (CASTSPELL_SPELLTYPE==((7<<4)+0)) goto deathwind_spelldone;
                  if (CASTSPELL_SPELLTYPE==((6<<4)+0)) goto chainbolt_spelldone;
                  if (CASTSPELL_SPELLTYPE==((7<<4)+8)) goto tremor_spelldone;
                  if (CASTSPELL_SPELLTYPE==65535) goto explosion_spelldone2;
                  //NEWCODE
                  if (CASTSPELL_SPELLTYPE==((5<<4)+4)) goto hailstorm_spelldone;
                  //NEWCODEEND
                }//crt
              }//myobj2
              goto spelldone;
            }//harm

            if (CASTSPELL_SPELLTYPE==((7<<4)+7)){ //time stop //txtset(spellname[(7<<4)+7],"TimeStop"); spellreagent[(7<<4)+7]=MAND|GARL|BLOO;
              //creates a localized vortex, strongest at the centre
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              x3=rnd*(64+tnpc->i);
              if (x3==0) {i2=3; goto spelldone;}
              for (spellx=-32;spellx<=32;spellx++){ for (spelly=-32;spelly<=32;spelly++){
                if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto timestopblocked;

                myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                if (myobj2){
                  if (myobj2->info&4){ //<-crt
                    crt=(creature*)myobj2->more;
                    f=(float)x3/32.0f;
                    if (abs(spellx)>abs(spelly)) f2=abs(spellx); else f2=abs(spelly);
                    crt->wait+=((float)x3-f2*f); crt->flags|=8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                    i9=SFnew(myobj2->x,myobj2->y);
                    sf[i9].type=1; //attack
                    sf[i9].x2=0;
                    sf[i9].x=myobj2->x;
                    sf[i9].y=myobj2->y;
                    sf[i9].wait=0.125f; //NULL
                    sf[i9].more=1;
                  }//crt
                }//myobj2
timestopblocked:;
              }}//spellx,spelly
              i2=1;
              goto spelldone;
            }//time stop

            if (CASTSPELL_SPELLTYPE==((1<<4)+4)){ //sleep //txtset(spellname[(1<<4)+4],"Sleep"); spellreagent[(1<<4)+4]=NIGH|SPID|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  x3=rnd*(32+(tnpc->i>>1));
                  if (x3==0) {i2=3; goto spelldone;}
                  i2=1;
                  i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i9].type=20; //pink ball
                  sf[i9].x=myobj->x;
                  sf[i9].y=myobj->y;
                  sf[i9].x2=myobj2->x;
                  sf[i9].y2=myobj2->y;
                  sf[i9].more=0xFFFF;
                  sf[i9].wait=1;
                  //immune to sleep?
                  x4=myobj2->type&1023;
                  if (x4==364) goto spelldone; //acid slug
                  if (x4==427) goto spelldone; //ant
                  if (x4==357) goto spelldone; //corpser
                  if (x4==367) goto spelldone; //daemon
                  if (x4==362) goto spelldone; //winged garg
                  if (x4==363) goto spelldone; //garg
                  if (x4==355) goto spelldone; //gazer
                  if (x4==352) goto spelldone; //ghost
                  if (x4==374) goto spelldone; //hydra
                  if (x4==343) goto spelldone; //insects
                  if (x4==347) goto spelldone; //reaper
                  if (x4==360) goto spelldone; //rotworms
                  if (x4==426) goto spelldone; //scorpion
                  if (x4==368) goto spelldone; //skeleton
                  if (x4==375) goto spelldone; //slime
                  if (x4==345) goto spelldone; //squid
                  if (x4==365) goto spelldone; //tanglevine
                  if (x4==373) goto spelldone; //wisp
                  //NOTE: sleep uses a paralyze effect, but to identify the difference visually (sleeping body) asleep flag is set
                  crt->wait+=x3; crt->flags|=(4+8); //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                  i9=SFnew(myobj2->x,myobj2->y);
                  sf[i9].type=1; //attack
                  sf[i9].x2=0;
                  sf[i9].x=myobj2->x;
                  sf[i9].y=myobj2->y;
                  sf[i9].wait=0.125f; //NULL
                  sf[i9].more=1;
                }//crt
              }//myobj2
              goto spelldone;
            }//sleep

            if (CASTSPELL_SPELLTYPE==((4<<4)+5)){ //paralyze //txtset(spellname[(4<<4)+5],"Paralyze"); spellreagent[(4<<4)+5]=SPID|SULF|NIGH|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  x3=rnd*(32+(tnpc->i>>1));
                  if (x3==0) {i2=3; goto spelldone;}
                  crt->wait+=x3; crt->flags|=8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                  i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                  sf[i9].type=14; //blue ball
                  sf[i9].x=myobj->x;
                  sf[i9].y=myobj->y;
                  sf[i9].x2=myobj2->x;
                  sf[i9].y2=myobj2->y;
                  sf[i9].more=0xFFFF;
                  sf[i9].wait=1;
                  i9=SFnew(myobj2->x,myobj2->y);
                  sf[i9].type=1; //attack
                  sf[i9].x2=0;
                  sf[i9].x=myobj2->x;
                  sf[i9].y=myobj2->y;
                  sf[i9].wait=0.125f; //NULL
                  sf[i9].more=1;
                  i2=1;
                }//crt
              }//myobj2
              goto spelldone;
            }//paralyze

            if (CASTSPELL_SPELLTYPE==((1<<4)+2)){ //poison //txtset(spellname[(1<<4)+2],"Poison"); spellreagent[(1<<4)+2]=NIGH|BLOO|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                if (myobj2->info&4){ //<-crt
                  crt=(creature*)myobj2->more;
                  i9=i3; i8=x2; //backup!
                  x3=rnd*(8+5*2);
                  x4=rnd*(tnpc->i+1);
                  if (x4>=x3){//success
                    crt->flags|=1;
                    i3=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                    sf[i3].type=18; //green ball
                    sf[i3].x=myobj->x;
                    sf[i3].y=myobj->y;
                    sf[i3].x2=myobj2->x;
                    sf[i3].y2=myobj2->y;
                    sf[i3].more=0xFFFF;
                    sf[i3].wait=1;
                    i3=SFnew(myobj2->x,myobj2->y);
                    sf[i3].type=1; //attack
                    sf[i3].x2=0;
                    sf[i3].x=myobj2->x;
                    sf[i3].y=myobj2->y;
                    sf[i3].wait=0.125f; //NULL
                    sf[i3].more=1;
                    x2=0; spellattcrt=TRUE; goto spellattcrt0;
                  }else{i2=3;}//spell failed
                }//crt
              }//myobj2
              goto spelldone;
            }//poison


            if (CASTSPELL_SPELLTYPE==((2<<4)+8)){ //PROTECTION //txtset(spellname[(2<<4)+8],"Protection"); spellreagent[(2<<4)+8]=SULF|GINS|GARL;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    x3=rnd*(32+(tnpc->i>>1)); //int can only double spell effectiveness!
                    if (x3){
                      if (x3>255) x3=255; //cap
                      if (x3>tnpc2->protection) {tnpc2->protection=x3; tnpc->upflags|=64;}
                      i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                      sf[i9].type=14; //blue ball
                      sf[i9].x=myobj->x;
                      sf[i9].y=myobj->y;
                      sf[i9].x2=myobj2->x;
                      sf[i9].y2=myobj2->y;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=1;
                      i2=1;
                    }
                    if (x3==0) i2=3;
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//protection

            if (CASTSPELL_SPELLTYPE==((4<<4)+9)){ //xray //txtset(spellname[(4<<4)+9],"X-ray"); spellreagent[(4<<4)+9]=MAND|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    x3=rnd*(8+(tnpc->i>>3)); //int can only double spell effectiveness!
                    if (x3){
                      if (x3>255) x3=255; //cap
                      if (x3>tnpc2->player->xray){
                        tnpc2->player->xray=x3;
                        //send xray update message
                        txtset(t,"??"); t->d2[0]=33; t->d2[1]=tnpc2->player->xray; NET_send(NETplayer,tnpc2->player->net,t);
                      }
                      i2=1;
                    }
                    if (x3==0) i2=3;
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//xray

            if (CASTSPELL_SPELLTYPE==((5<<4)+5)){ //mass protect //txtset(spellname[(5<<4)+5],"MassProtect"); spellreagent[(5<<4)+5]=SULF|GINS|GARL|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              x3=rnd*(32+(tnpc->i>>1)); //int can only double spell effectiveness!
              if (x3){
                if (x3>255) x3=255; //cap
                i2=1;
                for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                  if ((abs(spellx)+abs(spelly))!=4){
                    if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto massprotectblocked;
                    myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly);
                    if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                    if (myobj2){
                      if (myobj2->info&2){ //<-npc
                        tnpc2=(npc*)myobj2->more;
                        if (tnpc2->player){
                          if (x3>tnpc2->protection) {tnpc2->protection=x3; tnpc->upflags|=64;}
                        }//player
                      }//npc
                    }//myobj2
                    if ((abs(spellx)==2)||(abs(spelly)==2)){
                      i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                      sf[i9].type=14; //blue ball
                      sf[i9].x=tpx+x;
                      sf[i9].y=tpy+y;
                      sf[i9].x2=tpx+x+spellx;
                      sf[i9].y2=tpy+y+spelly;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=1;
                    }
massprotectblocked:;
                  }//4
                }}//spellx,spelly
              }else{i2=3;} //failed
              goto spelldone;
            }//mass protect

            if (CASTSPELL_SPELLTYPE==((6<<4)+7)){ //mass invis //txtset(spellname[(6<<4)+7],"MassInvisibility"); spellreagent[(6<<4)+7]=MAND|NIGH|BLOO|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;

              x3=rnd*(32+(tnpc->i>>1)); //int can only double spell effectiveness!
              if (x3){
                if (x3>255) x3=255; //cap
                i2=1;

                for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                  if ((abs(spellx)+abs(spelly))!=4){
                    if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto invisibilityblocked;

                    myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly);
                    if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                    if (myobj2){
                      if (myobj2->info&2){ //<-npc
                        tnpc2=(npc*)myobj2->more;
                        if (tnpc2->player){
                          if (x3>tnpc2->invisibility) tnpc2->invisibility=x3;
                        }//player
                      }//npc
                    }//myobj2
                    if ((abs(spellx)==2)||(abs(spelly)==2)){
                      i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                      sf[i9].type=19; //blue bolt
                      sf[i9].x=tpx+x;
                      sf[i9].y=tpy+y;
                      sf[i9].x2=tpx+x+spellx;
                      sf[i9].y2=tpy+y+spelly;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=1;
                    }
invisibilityblocked:;
                  }//4
                }}//spellx,spelly
              }else{i2=3;} //failed
              goto spelldone;
            }//mass invis

            if (CASTSPELL_SPELLTYPE==((4<<4)+3)){ //invis //txtset(spellname[(4<<4)+3],"Invisibility"); spellreagent[(4<<4)+3]=NIGH|BLOO;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    x3=rnd*(32+(tnpc->i>>1)); //int can only double spell effectiveness!
                    if (x3){
                      if (x3>255) x3=255; //cap
                      if (x3>tnpc2->invisibility) tnpc2->invisibility=x3;
                      i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                      sf[i9].type=19; //blue bolt
                      sf[i9].x=myobj->x;
                      sf[i9].y=myobj->y;
                      sf[i9].x2=myobj2->x;
                      sf[i9].y2=myobj2->y;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=1;
                      i2=1;
                    }
                    if (x3==0) i2=3;
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//invis

            if (CASTSPELL_SPELLTYPE==((2<<4)+3)){ //great light //txtset(spellname[(2<<4)+3],"GreatLight"); spellreagent[(2<<4)+3]=SULF|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    x3=rnd*(64+tnpc->i); //int can only double spell effectiveness!
                    if (x3){
                      x4=1+x3; if (x4>255) x4=255; //cap
                      tnpc2->light=x4;
                      i2=1;
                    }
                    if (x3==0) i2=3;
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//great light


            if (CASTSPELL_SPELLTYPE==((0<<4)+9)){ //light //txtset(spellname[(0<<4)+9],"Light"); spellreagent[(0<<4)+9]=SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    x3=rnd*(16+(tnpc->i>>2)); //int can only double spell effectiveness!
                    if (x3){
                      x4=1+x3; if (x4>255) x4=255; //cap
                      tnpc2->light=x4;
                      i2=1;
                    }
                    if (x3==0) i2=3;
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//light


            if (CASTSPELL_SPELLTYPE==((0<<4)+7)){ //awaken //txtset(spellname[(0<<4)+7],"Awaken"); spellreagent[(0<<4)+7]=GINS|GARL;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player==NULL){
                    if (tnpc2->schedule_last==0x91){
                      if (((tpx+x)==568)&&((tpy+y)==364)) goto spelldone;
                      if (((tpx+x)==570)&&((tpy+y)==364)) goto spelldone;
                      if (((tpx+x)==572)&&((tpy+y)==364)) goto spelldone;
                      if (((tpx+x)==574)&&((tpy+y)==364)) goto spelldone;
                      i2=1;
                      x3=rnd*(8+5*1);
                      x4=rnd*(tnpc->i+1);
                      if (x4>=x3){//success //set npc schedule to unknown! and make them just stand there
                        tnpc2->schedule_i=-1;
                        tnpc2->schedule_last=0;
                        tnpc2->wait_walk+=64; //1mins
                        //NEWCODE
                        i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                        sf[i9].type=14; //blue ball
                        sf[i9].x=myobj->x;
                        sf[i9].y=myobj->y;
                        sf[i9].x2=myobj2->x;
                        sf[i9].y2=myobj2->y;
                        sf[i9].more=0xFFFF;
                        sf[i9].wait=1;
                        //NEWCODEEND
                      }else{i2=3;}//spell failed
                    }//sleeping
                  }//!player
                  if (tnpc2->player){
                    if (tnpc2->flags&4){
                      i2=1;
                      x3=rnd*(8+5*1);
                      x4=rnd*(tnpc->i+1);
                      if (x4>=x3){//success
                        tnpc2->flags-=4; tnpc2->wait_disable=0;
                        //NEWCODE
                        i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                        sf[i9].type=14; //blue ball
                        sf[i9].x=myobj->x;
                        sf[i9].y=myobj->y;
                        sf[i9].x2=myobj2->x;
                        sf[i9].y2=myobj2->y;
                        sf[i9].more=0xFFFF;
                        sf[i9].wait=1;
                        //NEWCODEEND
                      }else{i2=3;}//spell failed
                    }//4, sleeping
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//awaken

            if (CASTSPELL_SPELLTYPE==((7<<4)+6)){ //summon //txtset(spellname[(7<<4)+6],"Summon"); spellreagent[(7<<4)+6]=MAND|GARL|BLOO;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              spellx=tpx+x; spelly=tpy+y;
              if ((spellx<0)||(spellx>2047)||(spelly<0)||(spelly>1023)) goto summonfail;
              if (((bt[spelly][spellx]>>10)&1)==0) goto summonfail; //land passable
              if (od[spelly][spellx]) goto summonfail;
              x3=rnd*(8+5*8); //int can only double spell effectiveness!
              x4=rnd*(tnpc->i+10);
              if (x4>=x3){//success
                i2=1;
                myobj2=OBJnew();
                myobj2->type=367; //daemon!
                myobj2->more=malloc(sizeof(creature));
                ZeroMemory(myobj2->more,sizeof(creature));
                crt=(creature*)myobj2->more;
                crt->crt_struct=TRUE;
                crt->hp=(obji[sprlnk[myobj2->type&1023]].v8>>8)*4;
                crt->mp=rnd*9;
                crt->al=1; x3=rnd*4; //25% chance of evil allegiance!
                if (x3) crt->al=4;
                crt->respawn_x=1024; //seconds till spawned crt will disappear
                myobj9=OBJnew(); myobj9->type=188; crt->items=myobj9; //(not included)bag
                myobj2->info|=4; //<-crt
                OBJadd(spellx,spelly,myobj2);
              }else{i2=3;}//spell failed
summonfail:
              goto spelldone;
            }//summon

            if (CASTSPELL_SPELLTYPE==((7<<4)+5)){ //slime //txtset(spellname[(7<<4)+5],"Slime"); spellreagent[(7<<4)+5]=BLOO|NIGH|MAND;
              i2=2;
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;

              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto slimeblocked;

                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                  if (myobj2){
                    if ((myobj2->info&8)==0){ //no redirector
                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        if (crt->al==1){
                          if (((tpx+x+spellx)<0)||((tpx+x+spellx)>2047)||((tpy+y+spelly)<0)||((tpy+y+spelly)>1023)) goto slimefail;
                          if ((btflags[bt[tpy+y+spelly][tpx+x+spellx]&1023]&1)==0) goto slimefail; //land passable

                          if ((myobj2->type&1023)==366) goto slimefail; //invincible tanglevine tendril!
                          if ((myobj2->type&1023)==430) if (crt->flags&128) goto slimefail;//invincible horse


                          x3=rnd*(640+tnpc->i*10); //128 better than kill spell!
                          x4=rnd*(640+tnpc->i*10);
                          x3=((x3+x4)>>1);

                          if (x3>=crt->hp){//success
                            i2=1;

                            //kill/remove creature
                            x3=x2;

                            myobj4=(object*)crt->items->more; //will point to first item
dropgoldmore3:
                            if (myobj4){
                              myobj5=(object*)myobj4->next;
                              OBJremove(myobj4);
                              if (myobj5) {myobj4=myobj5; goto dropgoldmore3;}
                            }
                            crtrespawn(myobj2);
                            x2=x3;

                            myobj2=OBJnew();
                            myobj2->type=375; //slime
                            myobj2->more=malloc(sizeof(creature));
                            ZeroMemory(myobj2->more,sizeof(creature));
                            crt=(creature*)myobj2->more;
                            crt->crt_struct=TRUE;
                            crt->hp=(obji[sprlnk[myobj2->type&1023]].v8>>8)*4;
                            crt->mp=rnd*9;
                            crt->al=1;
                            crt->respawn_x=1024; //seconds till spawned crt will disappear
                            myobj9=OBJnew(); myobj9->type=188; crt->items=myobj9; //(not included)bag
                            myobj2->info|=4; //<-crt
                            myobj2->info|=(3<<9); //slime divide!
                            OBJadd(tpx+x+spellx,tpy+y+spelly,myobj2);

                          }else{
                            if (i2==2) i2=3; //spell failed
                          }

                        }//al==1
                      }}}//myobj2
slimefail:;
slimeblocked:;
                }}}//4,x,y

              goto spelldone;
            }//slime


            if (CASTSPELL_SPELLTYPE==((3<<4)+1)){ //conjure //txtset(spellname[(3<<4)+1],"Conjure"); spellreagent[(3<<4)+1]=SPID|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              spellx=tpx+x; spelly=tpy+y;
              if ((spellx<0)||(spellx>2047)||(spelly<0)||(spelly>1023)) goto conjfail;
              if (((bt[spelly][spellx]>>10)&1)==0) goto conjfail; //land passable
              if (od[spelly][spellx]) goto conjfail;
              x3=rnd*(8+5*4); //int can only double spell effectiveness!
              x4=rnd*(tnpc->i+10);
              if (x4>=x3){//success
                i2=1;
                myobj2=OBJnew();
                x3=rnd*5;
                myobj2->type=342; //rat
                if (x3==1) myobj2->type=358; //snake
                if (x3==2) myobj2->type=344; //bat
                if (x3==3) myobj2->type=371; //troll
                if (x3==4) myobj2->type=361; //spider
                myobj2->more=malloc(sizeof(creature));
                ZeroMemory(myobj2->more,sizeof(creature));
                crt=(creature*)myobj2->more;
                crt->crt_struct=TRUE;
                crt->hp=(obji[sprlnk[myobj2->type&1023]].v8>>8)*4;
                crt->mp=rnd*9;
                crt->al=4;
                crt->respawn_x=1024; //seconds till spawned crt will disappear
                myobj9=OBJnew(); myobj9->type=188; crt->items=myobj9; //(not included)bag
                myobj2->info|=4; //<-crt
                OBJadd(spellx,spelly,myobj2);
              }else{i2=3;}//spell failed
conjfail:
              goto spelldone;
            }//conjure

            if (CASTSPELL_SPELLTYPE==((4<<4)+2)){ //insect swarm //txtset(spellname[(4<<4)+2],"InsectSwarm"); spellreagent[(4<<4)+2]=BLOO|SPID|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;

              x3=rnd*(4+(tnpc->i/16)); //int can only double spell effectiveness!
              if (x3==0) {i2=3; goto spelldone;}
              for (x4=1;x4<=x3;x4++){
                x5=16;
isretry: spellx=tpx+x+rnd*5-2; spelly=tpy+y+rnd*5-2;
                x5--;
                if (x5){
                  if ((spellx<0)||(spellx>2047)||(spelly<0)||(spelly>1023)) goto isretry;
                  if (((bt[spelly][spellx]>>10)&(1+2+4))==0) goto isretry; //land, air OR sea passable
                  if (od[spelly][spellx]) goto isretry;
                  if (stormcloakcheck2(spellx,spelly,tplayer)) goto isretry;

                  myobj2=OBJnew();
                  myobj2->type=343; //insects
                  myobj2->more=malloc(sizeof(creature));
                  ZeroMemory(myobj2->more,sizeof(creature));
                  crt=(creature*)myobj2->more;
                  crt->crt_struct=TRUE;
                  crt->hp=(obji[sprlnk[myobj2->type&1023]].v8>>8)*4;
                  crt->mp=rnd*9;
                  crt->al=4;
                  crt->respawn_x=1024; //seconds till spawned crt will disappear
                  myobj9=OBJnew(); myobj9->type=188; crt->items=myobj9; //(not included)bag
                  myobj2->info|=4; //<-crt
                  OBJadd(spellx,spelly,myobj2);
                }//x5
              }//x4
              i2=1;
              goto spelldone;
            }//insect swarm


            if (CASTSPELL_SPELLTYPE==((5<<4)+8)){ //replicate //txtset(spellname[(5<<4)+8],"Replicate"); spellreagent[(5<<4)+8]=SULF|SPID|BLOO|GINS|NIGH;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              //COST OF SPELL IS reagents=8 gold, 50% fail rate ->16gold per cast!

              if (myobj2=OBJfindlast(tpx+x,tpy+y)){
                if (myobj2->info&256) goto cantreplicate; //quest item
                if (obji[sprlnk[myobj2->type&1023]].weight==0) goto cantreplicate; //no weight!
                if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].v1) goto cantreplicate; //v1 (damage)
                if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].v2) goto cantreplicate; //v2 (armour)
                if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].flags&127) goto cantreplicate; //can be equipped
                if (obji[sprlnk[myobj2->type&1023]].flags&1024) goto cantreplicate; //container
                //EXCEPTIONS!
                if ((myobj2->type&1023)==87) goto cantreplicate; //orb of the moons(kals)
                if (myobj2->type==(223+1024)) goto cantreplicate; //lit powderkeg
                if ((myobj2->type&1023)==414) goto cantreplicate; //skiff
                if ((myobj2->type&1023)==149) goto cantreplicate; //deed
                if ((myobj2->type&1023)==275) goto cantreplicate; //potion
                if ((myobj2->type&1023)==58) goto cantreplicate; //spell
                if ((myobj2->type&1023)==64) goto cantreplicate; //key
                if ((myobj2->type&1023)==151) goto cantreplicate; //book
                if ((myobj2->type&1023)==152) goto cantreplicate; //scroll
                if ((myobj2->type&1023)==77) goto cantreplicate; //gem
                if ((myobj2->type&1023)==93) goto cantreplicate; //sextant
                if ((myobj2->type&1023)>=342) goto cantreplicate; //giant rat->horse
                //***THE ABOVE LINE NEGATES ALL OBJECTS ABOVE TYPE 342, THIS HAS BEEN LEFT AS IS TO AVOID MISTAKES WHEN ADDING NEW ITEMS LATER***
                if (myobj2->type==448) goto cantreplicate;//horse papers

                //PENDING quest items
                if ((myobj2->type&1023)==61) goto cantreplicate; //book of circles
                if ((myobj2->type&1023)==212) goto cantreplicate; //fan
                if ((myobj2->type&1023)==277) goto cantreplicate; //yew board (***possibly not a quest item)

                //find safe location for replicated object!
                x3=tpx+x; x4=tpy+y; x3++;
                if (myobj3=OBJfindlast(x3,x4)){
                  if (obji[sprlnk[myobj3->type&1023]+(myobj3->type>>10)].flags&512) goto gotreploc;
                }
                if (bt[x4][x3]&1024) goto gotreploc;
                x3=tpx+x; x4=tpy+y; x3--;
                if (myobj3=OBJfindlast(x3,x4)){
                  if (obji[sprlnk[myobj3->type&1023]+(myobj3->type>>10)].flags&512) goto gotreploc;
                }
                if (bt[x4][x3]&1024) goto gotreploc;
                x3=tpx+x; x4=tpy+y; x4++;
                if (myobj3=OBJfindlast(x3,x4)){
                  if (obji[sprlnk[myobj3->type&1023]+(myobj3->type>>10)].flags&512) goto gotreploc;
                }
                if (bt[x4][x3]&1024) goto gotreploc;
                x3=tpx+x; x4=tpy+y; x4--;
                if (myobj3=OBJfindlast(x3,x4)){
                  if (obji[sprlnk[myobj3->type&1023]+(myobj3->type>>10)].flags&512) goto gotreploc;
                }
                if (bt[x4][x3]&1024) goto gotreploc;
                x3=tpx+x; x4=tpy+y;
                if (myobj3=OBJfindlast(x3,x4)){
                  if (obji[sprlnk[myobj3->type&1023]+(myobj3->type>>10)].flags&512) goto gotreploc;
                }
                if (bt[x4][x3]&1024) goto gotreploc;
                goto cantreplicate; //no space!

gotreploc:
                spellx=rnd*(8+5*6);
                spelly=rnd*(tnpc->i+1);
                if (spelly>=spellx){//success
                  i2=1;
                  myobj3=OBJnew();
                  myobj3->type=myobj2->type;
                  myobj3->info=myobj2->info;
                  myobj3->more2=myobj2->more2;
                  if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].flags&4096) myobj3->more2=1;
                  myobj3->info|=112; //set flags as temp OBJ
                  if (myobj3->info&128) myobj3->info-=128; //remove stealing flag!
                  OBJadd(x3,x4,myobj3);
                }else{
                  i2=3;
                }
              }//myobj2
cantreplicate:
              goto spelldone;
            }//replicate



            if (CASTSPELL_SPELLTYPE==((6<<4)+4)){ //gate travel txtset(spellname[(6<<4)+4],"GateTravel"); spellreagent[(6<<4)+4]=SULF|BLAC|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              if (od[tpy+y][tpx+x]) goto gatetravel_cantcreate;
              if ((bt[tpy+y][tpx+x]&1024)==0) goto gatetravel_cantcreate;
              i2=1;
              x3=rnd*(8+5*7);
              x4=rnd*(tnpc->i+100);
              if (x4>=x3){//success
                myobj2=OBJnew();
                myobj2->type=84+1024;
                myobj2->info=3<<4; //set flags as temp OBJ
                OBJadd(tpx+x,tpy+y,myobj2);
                myobj2=OBJnew();
                myobj2->type=84;
                myobj2->info=3<<4; //set flags as temp OBJ
                OBJadd(tpx+x-1,tpy+y,myobj2);
              }else{i2=3;}//spell failed

gatetravel_cantcreate:
              goto spelldone;
            }//gatetravel

            if (CASTSPELL_SPELLTYPE==((1<<4)+8)){ //untrap //txtset(spellname[(1<<4)+8],"Untrap"); spellreagent[(1<<4)+8]=SULF|BLOO;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              if (myobj2=od[tpy+y][tpx+x]){
untrap_nextobj:
                if (myobj2->type==173) goto untrap_trapfound;
                if (myobj2->next){myobj2=(object*)myobj2->next; goto untrap_nextobj;}
              }
              goto spelldone; //no trap to remove!
untrap_trapfound:
              i2=3;
              x3=rnd*(8+5*2);
              x4=rnd*(tnpc->i+1);
              if (x4<x3) goto spelldone;
              i2=1;
              OBJremove(myobj2);
              txtset(t,"?"); t->d2[0]=8; txtadd(t,"Trap destroyed!");
              NET_send(NETplayer,tplayer->net,t);
              goto spelldone;
            }//untrap

            if (CASTSPELL_SPELLTYPE==((0<<4)+2)){ //detect trap txtset(spellname[(0<<4)+2],"DetectTrap"); spellreagent[(0<<4)+2]=NIGH|SULF;
	      object * caster_obj=npc_to_obj(tnpc,tplayer);
              i2=spell_detect_trap(tplayer,tnpc,NULL,caster_obj->x,caster_obj->y);  // Always relative to position of caster, not casting point or player
              goto spelldone;
            }//detect trap

            if (CASTSPELL_SPELLTYPE==((0<<4)+0)){ //create food txtset(spellname[(0<<4)+0],"CreateFood"); spellreagent[(0<<4)+0]=GARL|GINS|MAND; //level 1
              i2=spell_create_food(tplayer,tnpc,NULL,tpx+x,tpy+y); 
              goto spelldone;
            }//create food

            if (CASTSPELL_SPELLTYPE==((3<<4)+3)){ //fire field txtset(spellname[(3<<4)+3],"FireField"); spellreagent[(3<<4)+3]=BLAC|SULF|SPID;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              //NEWCODE
              x3=0;
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){x3=myobj2->type&1023;}
              if ((x3==317) || (x3==318) || (x3==320) || (x3==173) || (x3==53)) {goto spelldone;}
              //NEWCODEEND
              if (bt[tpy+y][tpx+x]&1024){
                i2=1;
                x3=rnd*(8+5*4);
                x4=rnd*(tnpc->i+1);
                if (x4>=x3){//success
                  myobj2=OBJnew();
                  myobj2->type=317; //fire
                  myobj2->more2=tnpc->player->id;
                  OBJadd(tpx+x,tpy+y,myobj2);
                  myobj2->info+=(2<<4);
                  OBJcheckflags(myobj2->x,myobj2->y);
                }else{i2=3;}//spell failed
              }//location OK
              goto spelldone;
            }//fire field

            if (CASTSPELL_SPELLTYPE==((4<<4)+0)){ //energy field //txtset(spellname[(4<<4)+0],"EnergyField"); spellreagent[(4<<4)+0]=MAND|SPID|BLAC; //level 5
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              //NEWCODE
              x3=0;
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){x3=myobj2->type&1023;}
              if ((x3==317) || (x3==318) || (x3==320) || (x3==173)) {goto spelldone;}
              //NEWCODEEND
              if (bt[tpy+y][tpx+x]&1024){
                i2=1;
                x3=rnd*(8+5*5);
                x4=rnd*(tnpc->i+1);
                if (x4>=x3){//success
                  myobj2=OBJnew();
                  myobj2->type=319; //protection field
                  myobj2->more2=tnpc->player->id;
                  OBJadd(tpx+x,tpy+y,myobj2);
                  myobj2->info+=(2<<4);
                  OBJcheckflags(myobj2->x,myobj2->y);
                }else{i2=3;}//spell failed
              }//location OK
              goto spelldone;
            }//energy field

            if (CASTSPELL_SPELLTYPE==((3<<4)+7)){ //poison field txtset(spellname[(3<<4)+7],"PoisonField"); spellreagent[(3<<4)+7]=NIGH|SPID|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              x3=0;
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){x3=myobj2->type&1023;}
              if ((x3==317) || (x3==318) || (x3==320) || (x3==173) || (x3==53)) {goto spelldone;}
              if (bt[tpy+y][tpx+x]&1024){
                i2=1;
                x3=rnd*(8+5*4);
                x4=rnd*(tnpc->i+1);
                if (x4>=x3){//success
                  myobj2=OBJnew();
                  myobj2->type=318; //poison field
                  myobj2->more2=tnpc->player->id;
                  OBJadd(tpx+x,tpy+y,myobj2);
                  myobj2->info+=(2<<4);
                  OBJcheckflags(myobj2->x,myobj2->y);
                }else{i2=3;}//spell failed
              }//location OK
              goto spelldone;
            }//poison field

            if (CASTSPELL_SPELLTYPE==((2<<4)+1)){ //dispel field //txtset(spellname[(2<<4)+1],"DispelField"); spellreagent[(2<<4)+1]=BLAC|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){
                x3=myobj2->type&1023;
                if ((x3>=317)&&(x3<=320)){
                  i2=1;
                  x3=rnd*(8+5*3);
                  x4=rnd*(tnpc->i+1);
                  if (x4>=x3){//success
                    OBJsave(myobj2->x,myobj2->y);
                    OBJremove(myobj2);
                  }else{i2=3;}//spell failed
                }//x3
              }//myobj2
              goto spelldone;
            }//dispel field


            if (CASTSPELL_SPELLTYPE==((3<<4)+5)){ //locate //txtset(spellname[(3<<4)+5],"Locate"); spellreagent[(3<<4)+5]=NIGH;
              i2=2; //implemented!
              x3=rnd*(8+5*4);
              x4=rnd*(tnpc->i+1);
              if (x4>=x3){//success
                i2=1;
                if (myobj->x<1024){
                  x4=(myobj->x+1024-304)/8; x4-=128;
                  y4=(myobj->y+1024-360)/8; y4-=128;
                }else{
                  x4=((myobj->x&255)+256-76)/2; x4-=128;
                  y4=((myobj->y&255)+256-90)/2; y4-=128;
                }
                txtset(t,"?"); t->d2[0]=8;
                txtnumint(t2,abs(y4)); txtadd(t,t2);
                if (y4>0) txtadd(t,"'S, "); else  txtadd(t,"'N, ");
                txtnumint(t2,abs(x4)); txtadd(t,t2);
                if (x4>0) txtadd(t,"'E"); else  txtadd(t,"'W");
                NET_send(NETplayer,tplayer->net,t);
              }else{i2=3;}
              goto spelldone;
            }//locate

            if (CASTSPELL_SPELLTYPE==((4<<4)+6)){ //pickpocket //txtset(spellname[(4<<4)+6],"PickPocket"); spellreagent[(4<<4)+6]=BLOO|SPID|NIGH;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&8) myobj2=(object*)myobj2->more;

                x5=0; x6=0;
                if (myobj2->info&2){ //<-npc
                  x5=1; tnpc2=(npc*)myobj2->more;
                }//npc
                if (myobj2->info&4){ //<-crt
                  x5=2; crt=(creature*)myobj2->more;
                }//crt
                if (x5){
                  x3=rnd*(8+5*5);
                  x4=rnd*(tnpc->i+1);
                  if (x4>=x3){//success
                    i2=1;
                    if (x5==1){ //->npc
                      if (tnpc2->converse==180){
                        if ((tplayer->GNPCflags[6]&(1<<0))==0){
                          tplayer->GNPCflags[6]|=(1<<0);
                          myobj3=OBJnew(); myobj3->type=25; myobj3->info|=256; //guild belt
                          i9=SFnew(myobj->x,myobj->y); //destination is more important than the source
                          sf[i9].type=9; //GENERIC throw object
                          sf[i9].x=myobj2->x;
                          sf[i9].y=myobj2->y;
                          sf[i9].x2=myobj->x;
                          sf[i9].y2=myobj->y;
                          sf[i9].more=0xFFFF;
                          sf[i9].wait=myobj3->type;
                          //add to inventory
                          additemroot(tnpc,myobj3);

                          tnpc->upflags|=32; //inv
                          goto spelldone;
                        }//global flag
                      }//80
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"Nothing!"); NET_send(NETplayer,tplayer->net,t);
                      goto spelldone;
                    }//npc

                    //crt
                    if (myobj3=(object*)crt->items->more){

                      if ((myobj3->type&1023)==98){
                        if (myobj3->next==NULL) goto pickpocketcrtnothing;
                      }
pickpocketcrtnext: x3=rnd*2; if (myobj3->next&&x3){myobj3=(object*)myobj3->next; goto pickpocketcrtnext;}
                      if ((myobj3->type&1023)==98){
                        myobj3=(object*)crt->items->more;
                        goto pickpocketcrtnext;
                      }

                      if (myobj3->type==88){
                        if (myobj3->more2>32){
                          myobj4=OBJnew();
                          myobj4->type=88; myobj4->info=112;
                          myobj4->more2=(rnd*(myobj3->more2-1))+1;
                          myobj3->more2-=myobj4->more2;
                          myobj3=myobj4;
                          goto pickpocketcrtskipremove;
                        }
                      }

                      OBJremove(myobj3);
pickpocketcrtskipremove:

                      i9=SFnew(myobj->x,myobj->y); //destination is more important than the source
                      sf[i9].type=9; //GENERIC throw object
                      sf[i9].x=myobj2->x;
                      sf[i9].y=myobj2->y;
                      sf[i9].x2=myobj->x;
                      sf[i9].y2=myobj->y;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=myobj3->type;
                      //add to inventory
                      additemroot(tnpc,myobj3);
                      tnpc->upflags|=32; //inv
                    }else{
pickpocketcrtnothing:
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"Nothing!"); NET_send(NETplayer,tplayer->net,t);
                    }
                  }else{i2=3;}//failed
                }//x3
              }//myobj2
              goto spelldone;
            }//pickpocket

            if (CASTSPELL_SPELLTYPE==((0<<4)+3)){ //cure //txtset(spellname[(0<<4)+3],"Cure"); spellreagent[(0<<4)+3]=GARL|GINS;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    x3=rnd*(8+5*1);
                    x4=rnd*(tnpc->i+1);
                    if (x4>=x3){//success
                      i2=1;
                      tnpc2->flags&=(65535-1); //remove poisoned flag
                      tnpc2->upflags|=2;
                      i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                      sf[i9].type=14; //blue ball
                      sf[i9].x=myobj->x;
                      sf[i9].y=myobj->y;
                      sf[i9].x2=myobj2->x;
                      sf[i9].y2=myobj2->y;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=1;
                    }else{i2=3;}//success
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//cure

            if (CASTSPELL_SPELLTYPE==((0<<4)+6)){ //heal
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    //perform light heal (1 to 16->32 hp)
                    x3=rnd*(16+(tnpc->i>>2)); //int can only double spell effectiveness!
                    x4=rnd*(16+(tnpc->i>>2)); //int can only double spell effectiveness!
                    x3=(x3+x4)>>1;

                    tnpc2->hp+=x3; if (tnpc2->hp>tnpc2->hp_max) tnpc2->hp=tnpc2->hp_max;
                    tnpc2->upflags|=2;
                    i2=1;
                    if (x3==0) i2=3;
                    if (x3){
                      i9=SFnew(myobj2->x,myobj2->y); //destination is more important than the source
                      sf[i9].type=14; //blue ball
                      sf[i9].x=myobj->x;
                      sf[i9].y=myobj->y;
                      sf[i9].x2=myobj2->x;
                      sf[i9].y2=myobj2->y;
                      sf[i9].more=0xFFFF;
                      sf[i9].wait=1;
                    }
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//heal

            if (CASTSPELL_SPELLTYPE==((6<<4)+1)){ //enchant //txtset(spellname[(6<<4)+1],"Enchant"); spellreagent[(6<<4)+1]=SPID|MAND|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){
                if (myobj2->type==78){ //staff
                  if ((myobj2->more2>>16)<=20){//25 spell limit (20+5=25)
                    if ((myobj2->info&(7<<9))==0){//no pending enchantments
                      x3=rnd*(8+5*7);
                      x4=rnd*(tnpc->i+1);
                      if (x4>=x3){//success
                        i2=1;
                        //allow for 5 new spells to be added
                        myobj2->info|=(5<<9);
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,"The staff glows brightly!"); NET_send(NETplayer,tplayer->net,t);
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,"To enchant this staff cast spells upon it. (You can now add up to 5 identical spells to this staff)"); NET_send(NETplayer,tplayer->net,t);
                        goto spelldone;
                      }else{i2=3; goto spelldone;}//fail
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"The staff already glows! To enchant this staff cast other spells upon it."); NET_send(NETplayer,tplayer->net,t);
                      goto spelldone;
                    }//no pending enchantments
                    txtset(t,"?"); t->d2[0]=8; 
                    txtnumint(t2,myobj2->more2>>16); txtadd(t,t2);
                    txtadd(t," spells are already held within this staff! No more may be added at this time."); NET_send(NETplayer,tplayer->net,t);
                    goto spelldone;
                  }//25 spell limit
                }//staff
              }//myobj2
              goto spelldone;
            }//enchant

            if (CASTSPELL_SPELLTYPE==((1<<4)+7)){ //unlockmagic
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){
                if (myobj2->info&8) myobj2=(object*)myobj2->more; //<-redirect
                x3=myobj2->type&1023;
                if ((x3>=OBJ_OAKEN_DOOR)&&(x3<=OBJ_STEEL_DOOR)){
                  if ((myobj2->type>>10)>=12){
                    i2=1;
                    x3=rnd*(8+5*2);
                    x4=rnd*(tnpc->i+1);
                    if (x4>=x3){//success
                      myobj2->type-=(8*1024);
                      myobj3=(object*)myobj2->more;
                      myobj3->type-=(8*1024);
                    }else{i2=3;}//success
                  }//>=12
                }//x3

                if (myobj2->type==(OBJ_CHEST+(3*1024))){
                  i2=1;
                  x3=rnd*(8+5*2);
                  x4=rnd*(tnpc->i+1);
                  if (x4>=x3){//success
                    OBJsave(myobj2->x,myobj2->y);
                    myobj2->type-=(2*1024);
                  }else{i2=3;}//success
                }//chest

              }//myobj2
              goto spelldone;
            }//unlock

            if (CASTSPELL_SPELLTYPE==((3<<4)+9)){ //WindChange txtset(spellname[(3<<4)+9],"WindChange"); spellreagent[(3<<4)+9]=SULF|BLOO;
              i2=2; //implemented!
              x3=rnd*(8+5*4);
              x4=rnd*(tnpc->i+1);
              if (x4>=x3){//success
                i2=1;
                x3=tplayer->windx; y3=tplayer->windy;
                if ((x3==0)&&(y3==0)) {x3=windx; y3=windy;}
                if ((x3==0)&&(y3==0)) {x3=0; y3=1; goto windchange_cast;}
                if ((x3==0)&&(y3==1)) {x3=-1; y3=1; goto windchange_cast;}
                if ((x3==-1)&&(y3==1)) {x3=-1; y3=0; goto windchange_cast;}
                if ((x3==-1)&&(y3==0)) {x3=-1; y3=-1; goto windchange_cast;}
                if ((x3==-1)&&(y3==-1)) {x3=0; y3=-1; goto windchange_cast;}
                if ((x3==0)&&(y3==-1)) {x3=1; y3=-1; goto windchange_cast;}
                if ((x3==1)&&(y3==-1)) {x3=1; y3=0; goto windchange_cast;}
                if ((x3==1)&&(y3==0)) {x3=1; y3=1; goto windchange_cast;}
                if ((x3==1)&&(y3==1)) {x3=0; y3=1; goto windchange_cast;}
windchange_cast:
                tplayer->windx=x3; tplayer->windy=y3;
                txtset(t,"??"); t->d2[0]=14; t->d2[1]=(tplayer->windx+1)+(tplayer->windy+1)*4; NET_send(NETplayer,tplayer->net,t);
              }else{i2=3;}
              goto spelldone;
            }//WindChange

            if (CASTSPELL_SPELLTYPE==((3<<4)+8)){ //sleep field txtset(spellname[(3<<4)+8],"SleepField"); spellreagent[(3<<4)+8]=GINS|SPID|BLAC;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              x3=0;
              if (myobj2=OBJfindlast(tpx+x,tpy+y)){x3=myobj2->type&1023;}
              if ((x3==317) || (x3==318) || (x3==320) || (x3==173)) {goto spelldone;}
              if (bt[tpy+y][tpx+x]&1024){
                i2=1;
                x3=rnd*(8+5*4);
                x4=rnd*(tnpc->i+1);
                if (x4>=x3){//success
                  myobj2=OBJnew();
                  myobj2->type=320; //sleep
                  myobj2->more2=tnpc->player->id;
                  OBJadd(tpx+x,tpy+y,myobj2);
                  myobj2->info+=(2<<4);
                  OBJcheckflags(myobj2->x,myobj2->y);
                }else{i2=3;}//spell failed
              }//location OK
              goto spelldone;
            }//sleep field

            if (CASTSPELL_SPELLTYPE==((3<<4)+6)){ //mass cure //txtset(spellname[(3<<4)+6],"MassCure"); spellreagent[(3<<4)+6]=GARL|GINS;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              i9=SFnew(tpx+x,tpy+y); //destination is more important than the source
              sf[i9].type=19; //blue bolt
              sf[i9].x=myobj->x;
              sf[i9].y=myobj->y;
              sf[i9].x2=tpx+x;
              sf[i9].y2=tpy+y;
              sf[i9].more=0xFFFF;
              sf[i9].wait=1;
              i2=1;
              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto masscureblocked;
                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly);
                  if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                  if (myobj2){
                    if (myobj2->info&2){ //<-npc
                      tnpc2=(npc*)myobj2->more;
                      if (tnpc2->player){
                        x3=rnd*(8+5*1);
                        x4=rnd*(tnpc->i+10);
                        if (x4>=x3){//success
                          tnpc2->flags&=(65535-1); //remove poisoned flag
                          tnpc2->upflags|=2;
                        }
                      }//player
                    }//npc
                  }//myobj2
                  if ((abs(spellx)==2)||(abs(spelly)==2)){
                    i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                    sf[i9].type=14; //blue ball
                    sf[i9].x=tpx+x;
                    sf[i9].y=tpy+y;
                    sf[i9].x2=tpx+x+spellx;
                    sf[i9].y2=tpy+y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }
masscureblocked:;
                }//4
              }}//spellx,spelly
              goto spelldone;
            }//mass cure




            if (CASTSPELL_SPELLTYPE==((2<<4)+5)){ //mass awaken //txtset(spellname[(2<<4)+5],"MassAwaken"); spellreagent[(2<<4)+5]=GINS|GARL;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              i9=SFnew(tpx+x,tpy+y); //destination is more important than the source
              sf[i9].type=19; //blue bolt
              sf[i9].x=myobj->x;
              sf[i9].y=myobj->y;
              sf[i9].x2=tpx+x;
              sf[i9].y2=tpy+y;
              sf[i9].more=0xFFFF;
              sf[i9].wait=1;
              i2=1;
              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly);
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto massawakenblocked;
                  if (myobj2){
                    if (myobj2->info&2){ //<-npc
                      x3=rnd*(8+5*3);
                      x4=rnd*(tnpc->i+10);
                      if (x4>=x3){//success
                        tnpc2=(npc*)myobj2->more;
                        if (tnpc2->player==NULL){
                          if (tnpc2->schedule_last==0x91){
                            if (((tpx+x)==568)&&((tpy+y)==364)) goto massawaken_npca;
                            if (((tpx+x)==570)&&((tpy+y)==364)) goto massawaken_npca;
                            if (((tpx+x)==572)&&((tpy+y)==364)) goto massawaken_npca;
                            if (((tpx+x)==574)&&((tpy+y)==364)) goto massawaken_npca;
                            tnpc2->schedule_i=-1;
                            tnpc2->schedule_last=0;
                            tnpc2->wait_walk+=64; //1mins
                          }//sleeping
                        }//!player
                        if (tnpc2->player){
                          if (tnpc2->flags&4){
                            tnpc2->flags-=4; tnpc2->wait_disable=0;
                          }//4, sleeping
                        }//player
                      }//fail
                    }//npc
                  }//myobj2

massawaken_npca:
                  if ((abs(spellx)==2)||(abs(spelly)==2)){
                    i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                    sf[i9].type=14+4*256; //blue ball
                    sf[i9].x=tpx+x;
                    sf[i9].y=tpy+y;
                    sf[i9].x2=tpx+x+spellx;
                    sf[i9].y2=tpy+y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }
massawakenblocked:;
                }//4
              }}//spellx,spelly
              goto spelldone;
            }//mass awaken


            if (CASTSPELL_SPELLTYPE==((2<<4)+6)){ //mass sleep //txtset(spellname[(2<<4)+6],"MassSleep"); spellreagent[(2<<4)+6]=GINS|NIGH|SPID;
              i2=2;
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              i9=SFnew(tpx+x,tpy+y); //destination is more important than the source
              sf[i9].type=19; //blue bolt
              sf[i9].x=myobj->x;
              sf[i9].y=myobj->y;
              sf[i9].x2=tpx+x;
              sf[i9].y2=tpy+y;
              sf[i9].more=0xFFFF;
              sf[i9].wait=1;
              i2=1;
              for (spellx=-2;spellx<=2;spellx++){ for (spelly=-2;spelly<=2;spelly++){
                if ((abs(spellx)+abs(spelly))!=4){
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto masssleepblocked;

                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                  if (myobj2){
                    if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                    if (myobj2->info&4){ //<-crt
                      crt=(creature*)myobj2->more;
                      x3=rnd*(32+(tnpc->i>>1));
                      //immune to sleep?
                      x4=myobj2->type&1023;
                      if (x4==364) goto masssleep_immune; //acid slug
                      if (x4==427) goto masssleep_immune; //ant
                      if (x4==357) goto masssleep_immune; //corpser
                      if (x4==367) goto masssleep_immune; //daemon
                      if (x4==362) goto masssleep_immune; //winged garg
                      if (x4==363) goto masssleep_immune; //garg
                      if (x4==355) goto masssleep_immune; //gazer
                      if (x4==352) goto masssleep_immune; //ghost
                      if (x4==374) goto masssleep_immune; //hydra
                      if (x4==343) goto masssleep_immune; //insects
                      if (x4==347) goto masssleep_immune; //reaper
                      if (x4==360) goto masssleep_immune; //rotworms
                      if (x4==426) goto masssleep_immune; //scorpion
                      if (x4==368) goto masssleep_immune; //skeleton
                      if (x4==375) goto masssleep_immune; //slime
                      if (x4==345) goto masssleep_immune; //squid
                      if (x4==365) goto masssleep_immune; //tanglevine
                      if (x4==373) goto masssleep_immune; //wisp
                      //NOTE: sleep uses a paralyze effect, but to identify the difference visually (sleeping body) asleep flag is set
                      crt->wait+=x3; crt->flags|=(4+8); //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                      i9=SFnew(myobj2->x,myobj2->y);
                      sf[i9].type=1; //attack
                      sf[i9].x2=0;
                      sf[i9].x=myobj2->x;
                      sf[i9].y=myobj2->y;
                      sf[i9].wait=0.125f; //NULL
                      sf[i9].more=1;
                    }//crt
                  }//myobj2
masssleep_immune:
                  if ((abs(spellx)==2)||(abs(spelly)==2)){
                    i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                    sf[i9].type=20+4*256; //pink ball
                    sf[i9].x=tpx+x;
                    sf[i9].y=tpy+y;
                    sf[i9].x2=tpx+x+spellx;
                    sf[i9].y2=tpy+y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
                  }
masssleepblocked:;
                }}}//4,x,y
              goto spelldone;
            }//mass sleep


            if (CASTSPELL_SPELLTYPE==((5<<4)+6)){ //negate magic //txtset(spellname[(5<<4)+6],"NegateMagic"); spellreagent[(5<<4)+6]=GARL|MAND|SULF;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              myobj2=OBJfindlast(tpx+x,tpy+y);
              if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x,tpy+y);
              if (myobj2){
                if (myobj2->info&2){ //<-npc
                  tnpc2=(npc*)myobj2->more;
                  if (tnpc2->player){
                    x3=rnd*(32+(tnpc->i>>1)); //int can only double spell effectiveness!
                    if (x3){
                      if (x3>255) x3=255; //cap
                      if (x3>tnpc2->negatemagic) {tnpc2->negatemagic=x3;}
                      i2=1;
                    }
                    if (x3==0) i2=3;
                  }//player
                }//npc
              }//myobj2
              goto spelldone;
            }//negate magic


            if (CASTSPELL_SPELLTYPE==((5<<4)+4)){ //hail storm //txtset(spellname[(5<<4)+4],"HailStorm"); spellreagent[(5<<4)+4]=BLOO|BLAC|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              x4=4;
              for (x4;x4>0;x4--) {
                x3=3+rnd*(1+(tnpc->i-24)/8);
                for (x3;x3>0;x3--){
                  spellx=rnd*(9)-6;
                  spelly=rnd*(9)-6;
                  if (spellx <= -3) {spellx=rnd*(3)-1;}
                  if (spelly <= -3) {spelly=rnd*(3)-1;}
                  if ((abs(spellx)+abs(spelly))!=4){
                    if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto hailstorm_blocked;
                    myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                    if (myobj2){
                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        i9=i3; i8=x2; //backup!
                        x2=rnd*(16+(tnpc->i>>1));
                        if (x2==0) {i2=1; x2=i8; i3=i9; goto hailstorm_spelldone;}
                        i3=SFnew(myobj2->x,myobj2->y);
                        sf[i3].type=1; //attack
                        sf[i3].x2=x2;
                        sf[i3].x=myobj2->x;
                        sf[i3].y=myobj2->y;
                        sf[i3].wait=0.125f; //NULL
                        sf[i3].more=1;
                        spellattcrt=TRUE; goto spellattcrt0;
                      }//crt
                    }//myobj2
hailstorm_spelldone:;
                    x5=x4/2;
                    i9=SFnew(tpx+x+spellx,tpy+y+spelly); //destination is more important than the source
                    sf[i9].type=13+x5*2*256; //sling bullet
                    sf[i9].x=tpx+x+spellx-2;
                    sf[i9].y=tpy+y+spelly-1-x4;
                    sf[i9].x2=tpx+x+spellx;
                    sf[i9].y2=tpy+y+spelly;
                    sf[i9].more=0xFFFF;
                    sf[i9].wait=1;
hailstorm_blocked:;
                  }//!=4
                }//x3
              }//x4
              i2=1;

              goto spelldone;
            }//hail storm


            if (CASTSPELL_SPELLTYPE==((0<<4)+8)){ //ignite //txtset(spellname[(0<<4)+8],"Ignite"); spellreagent[(0<<4)+8]=SULF|BLAC;
              i2=spell_ignite(tplayer,tnpc,NULL,tpx+x,tpy+y); 
              goto spelldone;
            }//ignite


            if (CASTSPELL_SPELLTYPE==((0<<4)+4)){ //douse //txtset(spellname[(0<<4)+4],"Douse"); spellreagent[(0<<4)+4]=GARL|BLAC;
              i2=spell_douse(tplayer,tnpc,NULL,tpx+x,tpy+y); 
              goto spelldone;
            }//douse


            if (CASTSPELL_SPELLTYPE==((4<<4)+7)){ //reveal //txtset(spellname[(4<<4)+7],"Reveal"); spellreagent[(4<<4)+7]=SPID|NIGH|MAND;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              i2=1;
              x3=rnd*(8+5*5);
              x4=rnd*(tnpc->i+1);
              if (x4>=x3) {
                i2=1;
                for (spellx=-7;spellx<=7;spellx++){ for (spelly=-7;spelly<=7;spelly++){
                  if ((abs(spellx)+abs(spelly))<=10){
                    myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly); if (myobj2==NULL) myobj2=OBJfindlastall(tpx+x+spellx,tpy+y+spelly);
                    if (myobj2){
                      if (myobj2->info&4){ //<-crt
                        crt=(creature*)myobj2->more;
                        if (crt->flags&2){ 
                          crt->flags-=2; //if creature invisible make visible.
                        }//flags
                      }//crt
                    }//myobj2
                  }
                }} //spellx,y
              }//success
              else {i2=3;} //fail
            }//reveal

            if (CASTSPELL_SPELLTYPE==((5<<4)+9)){ //web //txtset(spellname[(5<<4)+9],"Web"); spellreagent[(5<<4)+9]=SPID;
              i2=2; //implemented!
              if (stormcloakcheck2(tpx+x,tpy+y,tplayer)) goto spelldone;
              x3=rnd*(8+5*6);
              x4=rnd*(tnpc->i+100);
              if (x4>=x3){//success
                i2=1;
                i9=SFnew(tpx+x,tpy+y); //destination is more important than the source
                sf[i9].type=9; //generic throw object
                sf[i9].x=myobj->x;
                sf[i9].y=myobj->y;
                sf[i9].x2=tpx+x;
                sf[i9].y2=tpy+y;
                sf[i9].more=0xFFFF;
                sf[i9].wait=53;
                for (spellx=-1;spellx<=1;spellx++){ for (spelly=-1;spelly<=1;spelly++){
                  if (stormcloakcheck2(tpx+x+spellx,tpy+y+spelly,tplayer)) goto web_blocked;
                  if ((x3==317) || (x3==318) || (x3==320) || (x3==173) || (x3==53)) {goto web_blocked;}
                  myobj2=OBJfindlast(tpx+x+spellx,tpy+y+spelly);
                  if (bt[tpy+y+spelly][tpx+x+spellx]&1024) {
                    myobj2=OBJnew();
                    myobj2->type=53; //web
                    myobj2->more2=tnpc->player->id;
                    myobj2->info|=32;
                    //if tile has an object, place web under it 
                    OBJtmp=od[tpy+y+spelly][tpx+x+spellx];
                    if (OBJtmp) {
                      OBJtmp->prev=myobj2;
                      myobj2->next=OBJtmp;
                      myobj2->x=tpx+x+spellx;
                      myobj2->y=tpy+y+spelly;
                      od[tpy+y+spelly][tpx+x+spellx]=myobj2;
                    } else {
                      OBJadd(tpx+x+spellx,tpy+y+spelly,myobj2);
                    }
                  }
                  else if (myobj2 && myobj2->info&4) {
                    crt=(creature*)myobj2->more;
                    if ((myobj2->type&1023)!=361) {
                      x3=rnd*(32+(tnpc->i>>1)/2);
                      crt->wait+=x3; crt->flags|=8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                    }
                    myobj2=OBJnew();
                    myobj2->type=53; //web
                    myobj2->more2=tnpc->player->id;
                    myobj2->info|=112; //set flags as temp OBJ
                    //if tile has an object, place web under it 
                    OBJtmp=od[tpy+y+spelly][tpx+x+spellx];
                    if (OBJtmp) {
                      OBJtmp->prev=myobj2;
                      myobj2->next=OBJtmp;
                      od[tpy+y+spelly][tpx+x+spellx]=myobj2;
                    } else {
                      OBJadd(tpx+x+spellx,tpy+y+spelly,myobj2);
                    }

                  }//location OK
web_blocked:;
                }}//spellx,spelly
              }else{i2=3;}//spell failed
            }//web


            //NEWCODEEND


spelldone:

            if (i2==0){ //not implemented!
              txtset(t,"?"); t->d2[0]=8; txtadd(t,"U6O can't cast this spell yet!"); NET_send(NETplayer,tplayer->net,t);
            }

            //4. display words of power if spell was a success


            //5. remove mp&reagents if successful
            if ((i2==1)||(i2==3)){

              if (!staff_cast_spell){
                //subtract mp
                tnpc->mp-=((CASTSPELL_SPELLTYPE>>4)+1);
                tnpc->upflags|=4;
                //remove reagents
                x4=spellreagent[CASTSPELL_SPELLTYPE];
                for(x2=0;x2<CASTSPELL_ENUMERATEDINVENTORYOBJS;x2++){
                  x3=OBJlist_list[x2]->type; //gpf
                  if ((x3>=65)&&(x3<=72)){
                    x3-=65;
                    if (x4&(1<<x3)){
                      x4-=(1<<x3);
                      OBJlist_list[x2]->more2--;
                      if (OBJlist_list[x2]->more2==0) OBJremove(OBJlist_list[x2]);
                    }//x4
                  }//x3
                }//x2
                tnpc->upflags|=32;
              }//!staff_cast_spell

spelldone_donttakereagents:

              if (staff_cast_spell){
                x3=tplayer->staffcast_staff->more2>>16;
                x3--;
                if (x3){
                  tplayer->staffcast_staff->more2&=65535;
                  tplayer->staffcast_staff->more2|=(x3<<16);
                }else tplayer->staffcast_staff->more2=0; //all spells used, revert to unclassified staff
              }

              //spell delay
              tnpc->wait_cast+=4.0f-((float)tnpc->i/32.0f);
              //txtset(t,"??"); t->d2[0]=18; t->d2[1]=iparty; NET_send(NETplayer,tplayer->net,t); //set cast icon!

            }


            if (i2==2){ //invalid target
              if (!cast_spell){
                txtset(t,"?"); t->d2[0]=8; txtadd(t,"Can't cast there!"); NET_send(NETplayer,tplayer->net,t);
              }
            }

            if (i2==3){ //spell failed!
              if (!cast_spell){
                txtset(t,"?"); t->d2[0]=8; txtadd(t,"Spell failed!"); NET_send(NETplayer,tplayer->net,t);
              }
            }


castfailed:;
            if (cast_spell){
              if (cast_spell==1){ cast_spell=0; goto cast_spell1;}
              if (cast_spell==2){ cast_spell=0; goto cast_spell2;}
            }

            goto skip_pickup;
          }//i
        }//mbclick





        //use and other stuff
        if (tplayer->mobj!=NULL){ //drop item
          i=tplayer->mf;
          if (i<=7){ if(tplayer->party[i]){
            if (tplayer->key&KEYmbclick){
              tnpc=(npc*)tplayer->party[i]->more; //*tnpc


              //scroll inventory down
              if ((tplayer->mx>=(111-3))&&(tplayer->my>=(228-3))){
                if ((tplayer->mx<=(121+3))&&(tplayer->my<=(243+3))){
                  tnpc->upflags|=32;
                  //is scroll down available?
                  myobj=(object*)tnpc->baseitem;
                  myobj=(object*)myobj->more;
                  x=61; y=61; x3=tnpc->baseitem_offset;
dniz2:
                  if (myobj!=NULL){
                    if (x3>0) {x3--;goto skipbio1z2;}
                    x=x+16;
                    if (x>109) {x=61;y=y+16;}
                    if (y>109){
                      if (myobj->next!=NULL) 
                      {
                        tnpc->baseitem_offset+=4;
                      }
                      goto diskipz2;
                    }
                    if (myobj->next!=NULL){
skipbio1z2:
                      myobj=(object*)myobj->next;
                      goto dniz2;
                    }
                  }
diskipz2:;
                  goto skip_pickup;
                }}

              //scroll inventory up
              if ((tplayer->mx>=(111-3))&&(tplayer->my>=(197-3))){ if ((tplayer->mx<=(121+3))&&(tplayer->my<=(212+3))){
                if (tnpc->baseitem_offset>=4) {tnpc->baseitem_offset-=4;}else{tnpc->baseitem_offset=0;}
                tnpc->upflags|=32;
                goto skip_pickup;
              }}



              //clicked on opened "bag" icon ->close bag
              if ((tplayer->mx>=(90))&&(tplayer->my>=(128+3))&&(tplayer->mx<((90)+32))&&(tplayer->my<((128+3)+32))){
                if (tnpc->baseitem!=tnpc->items){ //backwards traverse
                  myobj2=tnpc->baseitem;
                  myobj=(object*)tnpc->baseitem->prev;
getprev9:
                  if ((myobj->more!=myobj2)||((obji[sprlnk[myobj->type&1023]].flags&1024)==0)) {myobj2=myobj; myobj=(object*)myobj->prev; goto getprev9;}
                  tnpc->baseitem=myobj;
                  tnpc->baseitem_offset=0;
                  tnpc->upflags|=32;
                  goto skip_pickup;
                }//tnpc->baseitem!=tnpc->items
              }//x,y

              if (tplayer->action==2){ //look, opens inventory container items 2004
                tnpc->upflags|=32;
                if ((tplayer->mx>=122)&&(tplayer->my>=122)){
                  x=(tplayer->mx-122)/32;
                  y=(tplayer->my-122)/32;
                  i2=x+(y*4);
                  myobj=tnpc->baseitem;
                  myobj=(object*)myobj->more;
                  i3=tnpc->baseitem_offset;
fo2c:
                  if (i3>0){i3--;myobj=(object*)myobj->next;goto fo2c;}
                  if (myobj!=NULL){
                    if (i2>0){
                      if (myobj->next!=NULL){
                        myobj=(object*)myobj->next;
                        i2--;
                        goto fo2c;
                      }else{myobj=NULL;}
                    }
                  }
                  if (myobj!=NULL){

                    if ((myobj->type==(OBJ_CHEST+1024*2))||(myobj->type==(OBJ_CHEST+1024*3))) goto inventory_look; //magically/locked chests

                    for (i3=0;i3<=nresu;i3++){
                      if (resu[i3]){
                        if (resu_body[i3]==myobj){
                          goto inventory_look; //block looking inside resurrectable bodies
                        }}}

                    if (obji[sprlnk[myobj->type&1023]].flags&1024){ //2003: check container flag
                      tnpc->baseitem=myobj;
                      tnpc->baseitem_offset=0;
                      goto skip_pickup;
                    }
                    goto inventory_look;
                  }//myobj!=NULL
                  goto skip_pickup;
                } //if x&y

                //what about equipped items?
                x2=tplayer->mx; y2=tplayer->my;
                x3=helmx; y3=helmy;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->helm) goto inventory_look;
                }
                x3=wep_rightx; y3=wep_righty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->wep_right) goto inventory_look;
                }
                x3=wep_leftx; y3=wep_lefty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->wep_left) goto inventory_look;
                }
                x3=armourx; y3=armoury;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->armour) goto inventory_look;
                }
                x3=bootsx; y3=bootsy;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->boots) goto inventory_look;
                }
                x3=neckx; y3=necky;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->neck) goto inventory_look;
                }
                x3=ring_leftx; y3=ring_lefty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->ring_left) goto inventory_look;
                }
                x3=ring_rightx; y3=ring_righty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (myobj=tnpc->ring_right) goto inventory_look;
                }

                goto skip_pickup;
              } //action==2


              //tnpc->update=1;

              if (tplayer->my<122) goto port_drop;
              if (tplayer->mx>=122){
                if (tplayer->my>=122){
port_drop:
sprite_drop:

                  if (tplayer->mobj->info&256) goto questitem_skipweightcheck;
                  //NEWCODE
                  if (U6O_DEBUG){
                    goto questitem_skipweightcheck; //for debugging
                  }
                  //NEWCODEEND
                  if ((tnpc->wt+WTfind(tplayer->mobj))<=tnpc->wt_max){
questitem_skipweightcheck:


                    if (tnpc->baseitem->type==57){
                      if (tplayer->mobj->type!=58){
                        txtset(t,"?");
                        t->d2[0]=8;
                        txtadd(t,"Only spells can go into the spellbook!");
                        NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }
                    }

                    if (tnpc->baseitem->type==62){ //vortex cube
                      if ((tplayer->mobj->type&1023)!=73){
                        txtset(t,"?");
                        t->d2[0]=8;
                        txtadd(t,"Only moonstones can go into the vortex cube!");
                        NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }
                    }

                    if (tplayer->mobj->info&256){ //drop quest item
                      if ((i!=0)||((tnpc->baseitem->info&256)==0)){
                        txtset(t,"?");
                        t->d2[0]=8;
                        txtadd(t,"You cannot put a quest item here!");
                        NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }//not primary player OR not quest bag
                    }


                    if ((tplayer->mobj->type&1023)==414){ //skiff
                      if (tnpc->baseitem!=tnpc->items){
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a skiff in there!"); NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }
                    }//414

                    if (tplayer->mobj->type==149){ //deed
                      if (tnpc->baseitem!=tnpc->items){
                        if ((tnpc->baseitem->info&256)==0){
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a deed in there!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }
                      }
                      if (i){//not the primary player
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," cannot be given a deed!"); NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }
                      //if (tnpc->converse!=201){ if (tnpc->converse!=0){
                      //txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," cannot be given a deed!"); NET_send(NETplayer,tplayer->net,t);
                      //goto skip_pickup;
                      //}}
                    }//149
					
                    if (tplayer->mobj->type==448){ //horse papers
                      if (tnpc->baseitem!=tnpc->items){
                        if ((tnpc->baseitem->info&256)==0){
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put the horse papers in there!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }
                      }
                      if (i){//not the primary player
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," cannot carry horse papers!"); NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }
                      //if (tnpc->converse!=201){ if (tnpc->converse!=0){
                      //txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," cannot carry horse papers!"); NET_send(NETplayer,tplayer->net,t);
                      //goto skip_pickup;
                      //}}
                    }//448


                    //check that current container is NOT a dead monster (they are not to be used as containers)
                    if (((tnpc->baseitem->type&1023)>432)&&((tnpc->baseitem->type&1023)<=447)){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put items in here!"); NET_send(NETplayer,tplayer->net,t);
                      goto skip_pickup;
                    }


                    tnpc->upflags|=32; //inv
                    stealing(tplayer,tplayer->mobj);


                    myobj=tnpc->baseitem;
                    myobj2=(object*)myobj->more;




                    if (myobj2!=NULL){

                      //scan for multiple item
                      if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&4096){ //multiple
                        myobj3=myobj2;
joinitem:
                        if (myobj3->type==tplayer->mobj->type){ //same type
                          if ((tplayer->mobj->info&128)==(myobj3->info&128)){
                            if (((myobj3->type&1023)==90)&&(myobj3->info!=tplayer->mobj->info)) goto cantjoin; //cant join half burnt torches
                            myobj3->more2+=tplayer->mobj->more2;
                            OBJrelease(tplayer->mobj);
                            tplayer->mobj=NULL;
                            goto joinitem_finished;
                          }//same stealing type
                        }//same type
                        if (myobj3->next) {myobj3=(object*)myobj3->next; goto joinitem;}
                      }//4096
cantjoin:

                      //if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].flags&4096){
                      //scan inventory list and append object if possible
                      //}else{

                      tplayer->mobj->next=myobj2;
                      myobj2->prev=tplayer->mobj;
                      tplayer->mobj->prev=myobj;
                      myobj->more=tplayer->mobj;
                      tplayer->mobj=NULL; //clear selected mouse object
                      //}
                    }else{
                      tplayer->mobj->next=NULL;
                      tplayer->mobj->prev=myobj;
                      myobj->more=tplayer->mobj;
                      tplayer->mobj=NULL; //clear selected mouse object
                    }
joinitem_finished:;

                  } //weight

                  goto skip_pickup; //mobj may have been set to 0!
                }}//mx,my
              //quest_skipdrop:

              //drop equip item
              //2003: no switching
              x2=tplayer->mx; y2=tplayer->my;
              //NEWCODE
              if (U6O_DEBUG){
                goto debug_skipweightcheck; //for debugging
              }

              if ((tnpc->wt+WTfind(tplayer->mobj))<=tnpc->wt_max){
debug_skipweightcheck:;
                //NEWCODEEND
                x3=helmx; y3=helmy;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->helm==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&1){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}
                        stealing(tplayer,tplayer->mobj);
                        tnpc->helm=tplayer->mobj;
                        tplayer->mobj=NULL;
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

                x3=wep_rightx; y3=wep_righty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->wep_right==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&(8+16)){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tnpc->wep_left!=NULL){
                          if (obji[sprlnk[tnpc->wep_left->type&1023]+(tnpc->wep_left->type>>10)].flags&16) goto skip_pickup;
                        }
                        if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&16){ //2 handed
                          if (tnpc->wep_left!=NULL) goto skip_pickup;
                        }
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}

                        stealing(tplayer,tplayer->mobj);

                        if ((tplayer->mobj->type&1023)==83){ //oil flask
                          if (tplayer->mobj->more2>1){ //seperate item
                            myobj9=OBJnew();
                            myobj9->type=83;
                            myobj9->more2=tplayer->mobj->more2-1;
                            tplayer->mobj->more2=1;
                            tnpc->wep_right=tplayer->mobj;
                            tplayer->mobj=myobj9;
                            goto wep_right_skip;
                          }//>1
                        }//83

                        if ((tplayer->mobj->type&1023)==90){ //torch
                          //tnpc->light=1;
                          tplayer->mobj->type=90+1024;
                          if (tplayer->mobj->more2>1){ //seperate item
                            myobj9=OBJnew();
                            myobj9->type=90;
                            myobj9->more2=tplayer->mobj->more2-1;
                            tplayer->mobj->more2=1;
                            tnpc->wep_right=tplayer->mobj;
                            tplayer->mobj=myobj9;
                            goto wep_right_skip;
                          }//>1
                        }//90

                        if (tplayer->mobj->type==57){ //ready spellbook
                          tnpc->upflags|=128;
						  // c222 keep lastused spellbook
						  if (easymodehostn1)
							playerspellbook = tplayer->mobj;
                        }

                        tnpc->wep_right=tplayer->mobj;
                        tplayer->mobj=NULL;
wep_right_skip:
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

                x3=wep_leftx; y3=wep_lefty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->wep_left==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&(8+16)){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tnpc->wep_right!=NULL){
                          if (obji[sprlnk[tnpc->wep_right->type&1023]+(tnpc->wep_right->type>>10)].flags&16) goto skip_pickup;
                        }
                        if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&16){ //2 handed
                          if (tnpc->wep_right!=NULL) goto skip_pickup;
                        }
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}

                        stealing(tplayer,tplayer->mobj);

                        if ((tplayer->mobj->type&1023)==83){ //oil flask
                          if (tplayer->mobj->more2>1){ //seperate item
                            myobj9=OBJnew();
                            myobj9->type=83;
                            myobj9->more2=tplayer->mobj->more2-1;
                            tplayer->mobj->more2=1;
                            tnpc->wep_left=tplayer->mobj;
                            tplayer->mobj=myobj9;
                            goto wep_left_skip;
                          }//>1
                        }//83

                        if ((tplayer->mobj->type&1023)==90){ //torch
                          //tnpc->light=1;
                          tplayer->mobj->type=90+1024;
                          if (tplayer->mobj->more2>1){ //seperate item
                            myobj9=OBJnew();
                            myobj9->type=90;
                            myobj9->more2=tplayer->mobj->more2-1;
                            tplayer->mobj->more2=1;
                            tnpc->wep_left=tplayer->mobj;
                            tplayer->mobj=myobj9;
                            goto wep_left_skip;
                          }//>1
                        }//90

                        if (tplayer->mobj->type==57){ //ready spellbook
                          tnpc->upflags|=128;
						  // c222 keep lastused spellbook
						  if (easymodehostn1)
							playerspellbook = tplayer->mobj;
                        }

                        tnpc->wep_left=tplayer->mobj;
                        tplayer->mobj=NULL;
wep_left_skip:
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

                x3=armourx; y3=armoury;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->armour==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&4){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}

                        stealing(tplayer,tplayer->mobj);

                        tnpc->armour=tplayer->mobj;
                        tplayer->mobj=NULL;
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

                x3=bootsx; y3=bootsy;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->boots==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&64){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}
                        stealing(tplayer,tplayer->mobj);

                        tnpc->boots=tplayer->mobj;
                        tplayer->mobj=NULL;
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

                x3=neckx; y3=necky;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->neck==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&2){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}
                        stealing(tplayer,tplayer->mobj);

                        tnpc->neck=tplayer->mobj;
                        tplayer->mobj=NULL;
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

                x3=ring_leftx; y3=ring_lefty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->ring_left==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&32){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}

                        stealing(tplayer,tplayer->mobj);

                        //if ((tplayer->mobj->type&1023)==258){ //invisibility ring (oooooh!)
                        //tnpc->flags|=2; //set invisibility flag
                        //}//258

                        tnpc->ring_left=tplayer->mobj;
                        tplayer->mobj=NULL;
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

                x3=ring_rightx; y3=ring_righty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->ring_right==NULL){
                    if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&32){
                      if ((tnpc->wt2+WTfind(tplayer->mobj))<=tnpc->wt2_max){
                        if (tplayer->mobj->info&256){ if (i){ //quest item->party member
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a quest item here!"); NET_send(NETplayer,tplayer->net,t);
                          goto skip_pickup;
                        }}

                        stealing(tplayer,tplayer->mobj);


                        tnpc->ring_right=tplayer->mobj;
                        tplayer->mobj=NULL;
                        tnpc->upflags|=64; //update flags
                        goto skip_pickup;
                      }}}}

              }//weight check!

            } //mbclick
          } //valid i(mf)
          } //i(mf)<=7





          //drop item on primary surface
          if ((tplayer->mf==255)&&(tplayer->key&KEYmbclick)){








            x=tplayer->mx/32;
            y=tplayer->my/32;
            x+=tpx;
            y+=tpy;
            myobj=OBJfindlast(x,y);







            if (myobj!=NULL){
              i=myobj->type; i2=sprlnk[i&1023];
              //i2=i>>10; //26/11/2004 removed for Sherry compatibility


              if (tplayer->action==1){ //use item on item


                //PROXIMITY CHECK REQUIRED HERE!
                z2=1;
                for (z=0;z<=7;z++){
                  if (myobj2=tplayer->party[z]){
                    if (abs(myobj2->x-myobj->x)<=1){
                      if (abs(myobj2->y-myobj->y)<=1){
                        z2=0;
                      }} }}
                if (z2) goto skip_pickup;

                if ((tplayer->mobj->type&1023)==178){ //empty bucket
                  if ((myobj->type&1023)==428){ //cow
                    tplayer->mobj->type=180; //milk bucket
                    tplayer->mobj_last=NULL; //->send message
                    goto skip_pickup;
                  }
                  if ((myobj->type&1023)==233){ //well
                    tplayer->mobj->type=179; //water bucket
                    tplayer->mobj_last=NULL; //->send message
                    goto skip_pickup;
                  }
                }

                if ((tplayer->mobj->type&1023)==183){ //empty jar
                  if ((myobj->type&1023)==182){ //beehive
                    if ((myobj->info&(15<<9))==0){ //empty?
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"Empty!"); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                    }

                    tplayer->mobj->type=184; //jar of honey
                    tplayer->mobj_last=NULL; //->send message

                    if (myobj->info&112){ //temp object?
                      x2=(myobj->info>>9)&15; x2--;
                      myobj->info&=(0xFFFF-(15<<9));
                      myobj->info|=(x2<<9);
                    }

                    goto skip_pickup;
                  }
                }

                //grain on horse
                if (tplayer->mobj->type==166){//grain
                  if ((myobj->type&1023)==431){
                    txtsetchar(t,8); txtadd(t,"Not now!"); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                  }
                  if ((myobj->type&1023)==430){
                    if (myobj->info&8) myobj=(object*)myobj->more;
                    if (myobj->info&4){
                      crt=(creature*)myobj->more;
                      if (crt->flags&128){
                        if (((crt->respawn_x>>4)&3)!=3){
                          crt->respawn_x|=(3<<4);
                          stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                          txtsetchar(t,8); txtaddchar(t,255); txtadd(t,"You pour the grain into the horse's feed bag."); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                        }//!=3
                        txtsetchar(t,8); txtaddchar(t,255); txtadd(t,"This horse's feed bag is already full!"); NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }else{
                        stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                        txtsetchar(t,8); txtaddchar(t,255); txtadd(t,"You feed the horse the sack of grain."); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                      }
                    }//crt
                  }//horse(without rider)
                }//grain

                //horseshoes on horse
                if (tplayer->mobj->type==202){//horseshoes
                  if ((myobj->type&1023)==431){
                    txtsetchar(t,8); txtadd(t,"Not now!"); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                  }
                  if ((myobj->type&1023)==430){
                    if (myobj->info&8) myobj=(object*)myobj->more;
                    if (myobj->info&4){
                      crt=(creature*)myobj->more;
                      if (crt->flags&128){
                        if (((crt->respawn_x>>6)&3)!=3){
                          crt->respawn_x|=(3<<6);
                          stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                          txtsetchar(t,8); txtaddchar(t,255); txtadd(t,"You fit a new horseshoe upon each hoof."); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                        }//!=3
                        txtsetchar(t,8); txtaddchar(t,255); txtadd(t,"This horse is already fitted with new horseshoes!"); NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }else{
                        stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                        txtsetchar(t,8); txtaddchar(t,255); txtadd(t,"You fit the horse with new horseshoes."); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                      }
                    }//crt
                  }//horse(without rider)
                }//horseshoes



                //potions
                if (tplayer->mobj->type==(275+1*1024)){ //red potion
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player){
                      tnpc->flags&=(65535-1); //remove poisoned flag
                      tnpc->upflags|=2;
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                      goto skip_pickup;
                    }
                  }
                }

                //use fan on balloon while flying
                if (tplayer->mobj->type==(212)){ //fan
                  if (myobj->info&2 && (myobj->type&1023)==423){ //<-NPC
                    //check if walking passable without balloon
                    i4=myobj->x; i5=myobj->y; OBJremove(myobj); 
                    if (bt[i5][i4]&1024){
                      OBJadd(i4,i5,myobj);
                      use_craft(tplayer,myobj); 
                    }else{
                      OBJadd(i4,i5,myobj);
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"Not here!"); NET_send(NETplayer,tplayer->net,t);
                    }
                    goto skip_pickup;
                  }
                }

                if (tplayer->mobj->type==(275+2*1024)){ //yellow potion
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player){
                      tnpc->hp+=30; if (tnpc->hp>tnpc->hp_max) tnpc->hp=tnpc->hp_max;
                      tnpc->upflags|=2;
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                      goto skip_pickup;
                    }
                  }
                }

                if (tplayer->mobj->type==(275+3*1024)){ //green potion
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player==tplayer){
                      tnpc->flags|=1; //set poisoned flag
                      tnpc->upflags|=2;
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                    }
                    goto skip_pickup;
                  }
                }

                //if (tnpc->invisibility){
                if (tplayer->mobj->type==(275+6*1024)){ //black potion
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player){
                      if (tnpc->invisibility<32) tnpc->invisibility=32;
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                      goto skip_pickup;
                    }
                  }
                }

                if (tplayer->mobj->type==(275+5*1024)){ //purple potion
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player){
                      if (tnpc->protection<32) tnpc->protection=32; //~2mins
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                      tnpc->upflags|=64; //update ->arm value
                      goto skip_pickup;
                    }
                  }
                }

                if (tplayer->mobj->type==(275+7*1024)){ //white potion
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player){
                      if (tnpc->player->xray<8){
                        tnpc->player->xray=8;
                        //send xray update message
                        txtset(t,"??"); t->d2[0]=33; t->d2[1]=tnpc->player->xray; NET_send(NETplayer,tnpc->player->net,t);
                      }
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                      goto skip_pickup;
                    }
                  }
                }

                if (tplayer->mobj->type==(275+0*1024)){ //blue potion
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player){
                      if (tnpc->flags&4){ //sleep flag
                        tnpc->flags-=4; tnpc->wait_disable=0;
                      }//4
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                      goto skip_pickup;
                    }
                  }
                }

                if (tplayer->mobj->type==(275+4*1024)){ //orange potion (sleep)
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player==tplayer){
                      tnpc->flags|=4; //set sleep flag
                      f=rnd*32.0f+32.0f; if (tnpc->wait_disable<f) tnpc->wait_disable=f;
                      stealing(tplayer,tplayer->mobj); tplayer->mobj=NULL;
                    }
                    goto skip_pickup;
                  }
                }


                x2=0;
                x3=tplayer->mobj->type;
                x5=0;
                if (x3==95) x2=6; //95 grapes 3 B *6
                if (x3==96) x2=1; //96 butter 1 C *1
                if (x3==115) {x2=5; x5=1;} //115 wine 6 6 5 5 4 6 6 4 4 =5 C *5
                if (x3==116) {x2=4; x5=1;} //116 mead 4 5 3 2 5 4 4 4 3 =4 C *4
                if (x3==117) {x2=4; x5=1;} //117 ale 3 7 2 5 3 3 3 4 2 =4 C *4
                if (x3==128) x2=12; //128 bread 3 3 (2 loaves) c A *12
                if (x3==129) x2=16; //129 meat (legs) c 4 3 3 4 4 4 4 4 4 A *16
                if (x3==130) x2=8; //130 rolls 1 2 2 A *m
                if (x3==131) x2=12; //131 cake 8 5 B *12
                if (x3==132) x2=10; //132 cheese 6 5 B *10
                if (x3==133) x2=16; //133 ham (looks like steak) 3 5 A *16
                if (x3==135) x2=28; //135 horse chops (gargoyle food) A *28
                if (x3==179) {x2=2; x5=1;} //179 bucket of water *this item needs modifying after use (1) B *2
                if (x3==180) {x2=8; x5=1;} //180 bucket of milk *this item needs modifying after use 3 5 =4 B *8
                if (x3==184) x2=10; //184 jar of honey *this item needs modifying after use 10 C *10
                if (x3==209) x2=12; //209 small hanging meat 6 B *12
                if (x3==210) x2=28; //210 big hanging meat 9 6 A =7 *28
                if (x3==265) x2=6; //265 fish 3 B *6
                if (x2){
                  x2*=4;
                  if (myobj->info&2){ //<-NPC
                    tnpc=(npc*)myobj->more;
                    if (tnpc->player==tplayer){
                      x4=tnpc->food;
                      if ((x4+x2)>255){
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," full!"); NET_send(NETplayer,tplayer->net,t); goto skip_pickup;
                      }
                      x4+=x2;
                      tnpc->food=x4;
                      txtset(t,"?"); t->d2[0]=8; txtaddchar(t,255); txtadd(t,tnpc->name);
                      if (x5) txtadd(t," drinks the "); else txtadd(t," eats the ");
                      y2=objname[(tplayer->mobj->type&1023)*2];
                      y3=objname[(tplayer->mobj->type&1023)*2+1];
                      txtNEWLEN(t2,y3);
                      memcpy(&t2->d2[0],&objname2[y2],y3);
                      txtadd(t,t2); txtadd(t,".");
                      NET_send(NETplayer,tplayer->net,t);

                      stealing(tplayer,tplayer->mobj);

                      //change or destroy item?
                      if (obji[sprlnk[tplayer->mobj->type&1023]+(tplayer->mobj->type>>10)].flags&4096){
                        if (tplayer->mobj->more2!=1){ //more than 1!
                          tplayer->mobj->more2--; goto skip_pickup;
                        }
                      }

                      if ((x3==179)||(x3==180)) {tplayer->mobj->type=178; tplayer->mobj_last=NULL; goto skip_pickup;}
                      if (x3==184) {tplayer->mobj->type=183; tplayer->mobj_last=NULL; goto skip_pickup;}
                      OBJremove(tplayer->mobj); OBJrelease(tplayer->mobj);
                      tplayer->mobj=NULL; //?
                      //stupid comment like in ultima7??
                    }
                    goto skip_pickup;
                  }
                }



                if ((tplayer->mobj->type>=242)&&(tplayer->mobj->type<=249)){ //rune
                  if (myobj2=(object*)myobj->prev){ if ((myobj2->type&1023)==73){ //moonstone
                    if ((myobj2->type>>10)==(tplayer->mobj->type-242)){ //same type
                      tplayer->GNPCflags[3]|=(1<<(myobj2->type>>10));
                      goto skip_pickup;
                    }//same type
                  }}//moonstone
                }//rune


                if (tplayer->mobj->type==63){ //lockpick
                  if (myobj->type==(98+2048)){ //locked chest
                    myobj3=(object*)tplayer->party[0]; tnpc=(npc*)myobj3->more;
                    stealing(tplayer,tplayer->mobj);
                    if (((rnd*64)+(rnd*tnpc->i))>=56){ //success
                      OBJsave(myobj->x,myobj->y);
                      myobj->type=OBJ_CHEST+1024;
					  if (enhancehostn1) {
						  txtsetchar(t, 8); txtaddchar(t, 255); txtadd(t, "Chest unlocked!"); NET_send(NETplayer, tplayer->net, t); // f111
					  }
                    }else{
                      x2=rnd*2; if (x2){ //break pick!
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Key broke!"); NET_send(NETplayer,tplayer->net,t);
                        if (tplayer->mobj->more2==1) tplayer->mobj=NULL; else tplayer->mobj->more2--;
                      }//x2
                    }
                    goto skip_pickup;
                  }
                }



                //lock pick on chest??

                //WOW is that a door at all???

                x2=myobj->type&1023;
                if ((x2>=297)&&(x2<=300)){ //door
                  if ((myobj->type>>10)>=4){ //closed
                    if (tplayer->mobj->type==63) goto lockpick; //lockpick
                    if (tplayer->mobj->type==64){ //key
lockpick:
                      myobj2=OBJfindlastall(x,y);
                      if (myobj2->info&8) myobj2=(object*)myobj2->more; //<-redirect
                      myobj2=OBJfindlastall(myobj2->x,myobj2->y); //2004
                      //GPF!


                      //txtnumint(t5,myobj2->more2);
                      //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

                      //txtnumint(t5,tplayer->mobj->more2);
                      //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
                      if ((myobj2->more2==0)&&(tplayer->mobj->type==63)){
                        if ((myobj->type>>12)!=2) goto skip_pickup;
                        myobj3=(object*)tplayer->party[0];
                        tnpc=(npc*)myobj3->more;
                        stealing(tplayer,tplayer->mobj);
                        if (((rnd*64)+(rnd*tnpc->i))>=56) goto lockpick2; //success
                        x2=rnd*2; if (x2){ //break pick!
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"Key broke!"); NET_send(NETplayer,tplayer->net,t);
                          if (tplayer->mobj->more2==1) tplayer->mobj=NULL; else tplayer->mobj->more2--;
                          goto skip_pickup;
                        }
                      }
                      if ((myobj2->more2)&&(tplayer->mobj->type==63)) goto skip_pickup;
                      if (myobj2->more2==tplayer->mobj->more2){
                        stealing(tplayer,tplayer->mobj);
lockpick2:
                        //unlock/relock door!
                        //myobj->more2=
                        if (myobj->info&8) myobj=(object*)myobj->more; //<-redirect
                        x2=myobj->type>>12;
                        if ((x2!=2)&&(tplayer->mobj->type==63)) goto skip_pickup;
                        if (x2==1){ //unlocked->lock
                          myobj->type=(myobj->type&4095)+(2<<12);
                          myobj3=(object*)myobj->more;
                          myobj3->type=(myobj3->type&4095)+(2<<12);
						  if (enhancehostn1) {
							  txtsetchar(t, 8); txtaddchar(t, 255); txtadd(t, "Door locked!"); NET_send(NETplayer, tplayer->net, t); // f111
						  }
                        }
                        if (x2==2){
                          myobj->type=(myobj->type&4095)+(1<<12);
                          myobj3=(object*)myobj->more;
                          myobj3->type=(myobj3->type&4095)+(1<<12);
						  if (enhancehostn1) {
							  txtsetchar(t, 8); txtaddchar(t, 255); txtadd(t, "Door unlocked!"); NET_send(NETplayer, tplayer->net, t); // f111
						  }
                        }
                        goto skip_pickup;
                      }
                    }
                  }//closed
                }//door




                goto skip_pickup;
              }//use item on item





              //}else{


              if (myobj->info&2){
                tnpc2=(npc*)myobj->more;
                if (tnpc2->player==tplayer){
                  tnpc=tnpc2;
                  i=-1;
                  if (tnpc->player){ if (tnpc->player->party[0]){ if ((npc*)tnpc->player->party[0]->more==tnpc){
                    i=0;
                  }}}
                  goto sprite_drop;
                }
              }

              if (i==394){
                if ((x==921)&&(y==851)){
                  if (tplayer->mobj->type==394){

                    goto dropallow;
                  }}}

              if (i==396){
                if ((x==925)&&(y==851)){
                  if (tplayer->mobj->type==396){
                    goto dropallow;
                  }}}

              if (i==62){
                if ((x==923)&&(y==850)){
                  if (tplayer->mobj->type==62){
                    goto dropallow;
                  }}}

              if (obji[i2].flags&512){
dropallow:
                z2=9; myobj2=NULL;
                for (z=0;z<=7;z++){
                  if (myobj3=tplayer->party[z]){
                    if (OBJcheckbolt(myobj3->x,myobj3->y,x,y)==FALSE){
                      z3=abs(myobj3->x-x); z4=abs(myobj3->y-y); if (z4>z3) z3=z4;
                      if (z3<z2){
                        myobj2=myobj3;
                        z2=z3;
                      }
                    }
                  }
                }
                if (myobj2){

                  if ((tplayer->mobj->type&1023)==420){ //skiff drop
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a balloon here!"); NET_send(NETplayer,tplayer->net,t);
                    goto itemdropped2;
                  }



                  if (tplayer->mobj->info&256){ //drop quest item

                    //brit lens
                    if (tplayer->mobj->type==394){
                      if ((x==921)&&(y==851)){
                        tplayer->GNPCflags[18]|=2;
                        tplayer->mobj=NULL; //clear selected mouse object
                        goto itemdropped2;
                      }
                    }

                    //garg lens
                    if (tplayer->mobj->type==396){
                      if ((x==925)&&(y==851)){
                        tplayer->GNPCflags[18]|=4;
                        tplayer->mobj=NULL; //clear selected mouse object
                        goto itemdropped2;
                      }
                    }

                    //vortex cube
                    if (tplayer->mobj->type==62){
                      if ((x==923)&&(y==850)){
                        //check internal contents of vortex cube!
                        x2=0;
                        myobj2=(object*)tplayer->mobj->more;
checkvortexcube:
                        if (myobj2){
                          x2|=(1<<(myobj2->type>>10));
                          if (myobj2=(object*)myobj2->next) goto checkvortexcube;
                        }
                        if (x2!=255){
                          txtset(t,"?Place the eight moonstones within the Vortex Cube.");
                          t->d2[0]=8;
                          NET_send(NETplayer,tplayer->net,t);
                          goto itemdropped2;
                        }
                        tplayer->GNPCflags[18]|=1;
                        tplayer->mobj=NULL; //clear selected mouse object
                        goto itemdropped2;
                      }
                    }

                    txtset(t,"?");
                    t->d2[0]=8;
                    txtadd(t,"You cannot put a quest item here!");
                    NET_send(NETplayer,tplayer->net,t);
                    goto itemdropped2;
                  }



                  if ((tplayer->mobj->type&1023)==414){ //skiff drop
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a skiff here!"); NET_send(NETplayer,tplayer->net,t);
                    goto itemdropped2;
                  }

                  if (tplayer->mobj->type==149){ //deed drop
                    //in current boat?
                    if ((object*)tplayer->mobj->more==tplayer->party[0]){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot drop this deed now!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped2;
                    }
                    //currently holding the boat this deed points to?
                    myobj3=(object*)tplayer->mobj->more;
                    //txtnumint(t5,myobj->more2);
                    if ((myobj3->x==0)&&(myobj3->y==0)){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You must drop the skiff first!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped2;
                    }
                  }
				 
                  if (tplayer->mobj->type==448){//horse papers
                    if (((object*)tplayer->mobj->more)->x==0){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot drop these horse papers now!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped2;
                    }
                    crt2=(creature*)((object*)tplayer->mobj->more)->more;
                    crt2->respawn_y=0xFFFF;
                  }//448
				 
                  x5=OBJlist(tplayer->mobj); //x5=last object number
                  OBJlist_last=NULL;

                  //check if drop location is a house storage shelf
                  for (i3=1;i3<=255;i3++){
                    for (i4=0;i4<housestoragenext[i3];i4++){
                      if (x==housestoragex[i3][i4]){ if (y==housestoragey[i3][i4]){
                        //stolen items check
                        for(y6=0;y6<x5;y6++){
                          if (OBJlist_list[y6]->info&128){ //stolen item!
                            txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put stolen things here!"); NET_send(NETplayer,tplayer->net,t);
                            goto itemdropped2;
                          }//stolen item!
                        }//y6

                        //8 stack limit check
                        myobj=od[housestoragey[i3][i4]][housestoragex[i3][i4]];
                        x2=0;
countstacklimit: if (myobj->next){x2++; myobj=(object*)myobj->next; goto countstacklimit;}
                        if (x2>=8){
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot stack more than 8 items here! Put items in containers to store more here."); NET_send(NETplayer,tplayer->net,t);
                          goto itemdropped2;
                        }
                      }}//x,y
                    }//i4
                  }//i3


                  //clear notforsale marks
                  for(y6=0;y6<x5;y6++){
                    OBJlist_list[y6]->info|=32768; OBJlist_list[y6]->info^=32768;
                  }//y6


                  /*
                  if (tplayer->mobj->info&128){//stolen item
                  for (i3=0;i3<=15;i3++) if (stolenitemwarning[i3]==tplayer->mobj) goto stolenitem_alreadyadded;
                  for (i3=0;i3<=15;i3++){
                  if (stolenitemwarning[i3]==NULL){
                  stolenitemwarning[i3]=tplayer->mobj;
                  stolenitemwarning_wait[i3]=2.0f;
                  stolenitemwarning_player[i3]=tplayer;
                  goto stolenitem_alreadyadded;
                  }//==NULL
                  }//i3
                  }//stolen item
                  stolenitem_alreadyadded:
                  */
                  if (tplayer->mobj->info&128){//stolen item
                    i3=SFnew(x,y); //destination is more important than the source
                    sf[i3].type=21;//stolen item warning
                    sf[i3].x=x;
                    sf[i3].y=y;
                    sf[i3].more=tplayer->mobj->type;
                    sf[i3].wait=2.0f;
                  }


                  //drop sfx!
                  i3=SFnew(x,y); //destination is more important than the source
                  sf[i3].type=9; //GENERIC throw object
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].x2=x;
                  sf[i3].y2=y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=tplayer->mobj->type;

                  //drop sound effect
                  if ((tplayer->mobj->type&1023)==88){//gold
                    if (tplayer->mobj->more2>=5){
                      i3=SFnew(x,y);
                      sf[i3].type=4; //sound
                      sf[i3].x=x;
                      sf[i3].y=y;
                      sf[i3].wait=1.0f; //NULL
                      sf[i3].more=6; //coinmany
                    }else{
                      i3=SFnew(x,y);
                      sf[i3].type=4; //sound
                      sf[i3].x=x;
                      sf[i3].y=y;
                      sf[i3].wait=1.0f; //NULL
                      sf[i3].more=7; //coinfew
                    }
                  }

                  //smash it?
                  i4=0;
                  if ((tplayer->mobj->type&1023)==275) i4=1; //potions
                  if (tplayer->mobj->type==115) i4=1; //
                  if (tplayer->mobj->type==116) i4=1; //
                  if (tplayer->mobj->type==117) i4=1; //
                  if (tplayer->mobj->type==118) i4=1; //
                  if (tplayer->mobj->type==119) i4=1; //
                  if (tplayer->mobj->type==120) i4=1; //mug
                  if (tplayer->mobj->type==123){ //mirror
                    if ((abs(myobj2->x-x)>1)||(abs(myobj2->y-y)>1)){
                      sf[i3].wait+=32768; tplayer->mobj->type=123+1024*2;
                      stealing(tplayer,tplayer->mobj);
                    }
                  }
                  if ((tplayer->mobj->type==179)||(tplayer->mobj->type==180)){ //bucket of milk/water
                    if ((abs(myobj2->x-x)>1)||(abs(myobj2->y-y)>1)){
                      tplayer->mobj->type=178;
                      sf[i3].wait=tplayer->mobj->type;
                      stealing(tplayer,tplayer->mobj);
                    }
                  }
                  if (tplayer->mobj->type==92) i4=1; //silver snake venom
                  if (tplayer->mobj->type==161) i4=1; //jug
                  if (tplayer->mobj->type==83) i4=1; //oil flask
                  if (tplayer->mobj->type==48) i4=1; //glass sword
                  if (tplayer->mobj->type==183) i4=1; //jar
                  if (tplayer->mobj->type==184) i4=1; //jar with honey
                  if ((abs(myobj2->x-x)<=1)&&(abs(myobj2->y-y)<=1)) i4=0;
                  if (i4){
                    sf[i3].wait+=32768; OBJrelease(tplayer->mobj);
                    stealing(tplayer,tplayer->mobj);
                  }else{
                    OBJadd(x,y,tplayer->mobj);
                  }
                  tplayer->mobj=NULL; //clear selected mouse object

                  goto itemdropped2;

                  //}//<=8
                  //}
                  //}}
                }//myobj2

itemdropped2:;
              } //bt flag








            }//myobj!=NULL
            else
            {
              //myobj==NULL hence no item underneath!

              if (tplayer->action==1){ //use item on basetile

                if (tplayer->mobj->type==87){ //orb of the moons
                  if (tplayer->GNPCflags[23]==0){
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,"You can't figure out how to use it!"); NET_send(NETplayer,tplayer->net,t);
                    goto skip_pickup;
                  }
                  if (bt[y][x]&1024){ //walkable location
                    if (myobj2=tplayer->party[tplayer->selected_partymember]){
                      if (abs(myobj2->x-x)<=2){ if (abs(myobj2->y-y)<=2){
                        tnpc=(npc*)myobj2->more;
                        if (tnpc->wait_disable<=0){
                          myobj3=OBJnew();
                          myobj3->type=84+1024;
                          myobj3->info=3<<4; //set flags as temp OBJ
                          myobj3->more2=(x-myobj2->x+2)+(y-myobj2->y+2)*5+1;
                          OBJadd(x,y,myobj3);
                          myobj3=OBJnew();
                          myobj3->type=84;
                          myobj3->info=3<<4; //set flags as temp OBJ
                          myobj3->more2=(x-myobj2->x+2)+(y-myobj2->y+2)*5+1;
                          OBJadd(x-1,y,myobj3);
                          tnpc->wait_disable+=8.0f;
                        }else{
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," busy!"); NET_send(NETplayer,tplayer->net,t);
                        }
                      }}//x,y
                    }//myobj2
                  }//&1024
                  goto skip_pickup;
                }//orb

                //myobj3=OBJnew(); myobj3->type=87; myobj3->more2=0; myobj3->info=256; //orb of moons
                /*
                myobj2=OBJnew();
                myobj2->type=84+1024;
                myobj2->info=3<<4; //set flags as temp OBJ
                OBJadd(tpx+x,tpy+y,myobj2);
                myobj2=OBJnew();
                myobj2->type=84;
                myobj2->info=3<<4; //set flags as temp OBJ
                OBJadd(tpx+x-1,tpy+y,myobj2);
                */


                z2=1;
                for (z=0;z<=7;z++){
                  if (myobj2=tplayer->party[z]){
                    if (abs(myobj2->x-x)<=1){
                      if (abs(myobj2->y-y)<=1){
                        z2=0;
                      }} }}
                if (z2) goto skip_pickup;

                if ((tplayer->mobj->type&1023)==264){ //fishing line
                  if (((bt[y][x]&1023)>=8)&&((bt[y][x]&1023)<16)){ //in water
                    tnpc=(npc*)tplayer->party[0]->more;
                    if (tnpc->wait_disable<=0){
                      if ((rnd*8.0f)<1.0f){
                        //add fish!
                        myobj3=OBJnew(); myobj3->type=265; //<-fish
                        f=WTfind(myobj3);
                        if ((tnpc->wt+f)<=tnpc->wt_max){
                          additemroot(tnpc,myobj3);
                          tnpc->upflags|=32;
                        }//weight check
                      }else{
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Didn't get a fish."); NET_send(NETplayer,tplayer->net,t);
                      }
                      tnpc->wait_disable+=8.0f;
                    }else{//wait
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," busy!"); NET_send(NETplayer,tplayer->net,t);
                    }
                  }
                  goto skip_pickup;
                }

                if ((tplayer->mobj->type&1023)==104){ //shovel
                  if (od[y][x]==NULL){
                    if (((bt[y][x]&1023)>=108)&&((bt[y][x]&1023)<112)){ //on soil
                      if ((x==707)&&(y==835)){ //hole location 707 835
                        if (tplayer->craft==NULL){
                          if (usinghorse(tplayer)==NULL){

                            if (tplayer->GNPCflags[8]==1){//know location

                              //if another resurrectable body is being carried drop it near entrance
                              x5=0;
                              for (x6=0;x6<=7;x6++){
                                if (myobj3=getpartymemberobj(tplayer,x6)){
                                  tnpc3=(npc*)myobj3->more;
                                  tnpc3->upflags|=32;
                                  x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
                                }} //!=NULL,x6
                              OBJlist_last=NULL;
                              for(y6=0;y6<x5;y6++){
                                if ((OBJlist_list[y6]->type&1023)==339){ //dead body
                                  //scan resurrect list
                                  for (i3=0;i3<=nresu;i3++){
                                    if (resu[i3]){
                                      if (resu_body[i3]==OBJlist_list[y6]){
                                        OBJremove(OBJlist_list[y6]);
                                        OBJadd(704,832,OBJlist_list[y6]);
                                        goto pirateholefix_bodyremoved;
                                      }}}
pirateholefix_bodyremoved:;
                                }}

                              //transport party to underground
                              for (i4=7;i4>=0;i4--){
                                if (tplayer->party[i4]!=NULL){
                                  OBJremove(tplayer->party[i4]);
                                  OBJadd(179+1024,211,tplayer->party[i4]);
                                }//->party
                              }//i4
                              txtset(t,"?"); t->d2[0]=8; txtadd(t,"You dig a hole."); NET_send(NETplayer,tplayer->net,t);
                            }else{//dont know location
                              txtset(t,"?"); t->d2[0]=8; txtadd(t,"Why dig here?"); NET_send(NETplayer,tplayer->net,t);
                            }
                            goto skip_pickup;
                          }//horse
                        }//craft
                      }//hole x,y
                      if ((tplayer->party[0]->x>=1024)&&(tplayer->party[0]->y<256)){ //underground //TODO do real underground check
                        tnpc=(npc*)tplayer->party[0]->more;
                        if (tnpc->wait_disable<=0){
                          if ((rnd*12.0f)<1.0f){
                            myobj3=OBJnew(); myobj3->type=89; myobj3->more2=1; 
                            //fountain?
                            myobj3->info|=112; OBJadd(x,y,myobj3);
                          }else{
                            txtset(t,"?"); t->d2[0]=8; txtadd(t,"Nothing!"); NET_send(NETplayer,tplayer->net,t);
                          }
                          tnpc->wait_disable+=8.0f-((tplayer->mobj->more2)&15)*0.5f;
                        }else{//wait
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," busy!"); NET_send(NETplayer,tplayer->net,t);
                        }
                      }//underground
                    }//soil
                  }//no objects
                  goto skip_pickup;
                }//shovel


                if ((tplayer->mobj->type&1023)==OBJ_TMAP){ //treasure map
                  if (od[y][x]==NULL){
                    //if (((bt[y][x]&1023)==1)||(((bt[y][x]&1023)>=52)&&((bt[y][x]&1023)<64))||(((bt[y][x]&1023)>=73)&&((bt[y][x]&1023)<96))||(((bt[y][x]&1023)>=108)&&((bt[y][x]&1023)<112))){
                    // f111 fix for unable to dig up treasure if it's on a river/coast tile (make it consistent with treasure map creation)
					// from TMAP creation: while (((bt[y7+(x2-3)*2][x7+(x1-3)*2]&1023)>=8)&&((bt[y7+(x2-3)*2][x7+(x1-3)*2]&1023)<16)) { //check that the map marker is not in the sea.
					x2 = 0;
					if (enhancehostn1) {
						if (!(((bt[y][x] & 1023) >= 8) && ((bt[y][x] & 1023) < 16)))
							x2 = 1;
					} else {
						if (((bt[y][x]&1023)==1)||(((bt[y][x]&1023)>=52)&&((bt[y][x]&1023)<64))||(((bt[y][x]&1023)>=73)&&((bt[y][x]&1023)<96))||(((bt[y][x]&1023)>=108)&&((bt[y][x]&1023)<112)))
							x2 = 1;
					}
                    //if (((bt[y][x]&1023)==1)||(((bt[y][x]&1023)>=52)&&((bt[y][x]&1023)<64))||(((bt[y][x]&1023)>=73)&&((bt[y][x]&1023)<96))||(((bt[y][x]&1023)>=108)&&((bt[y][x]&1023)<112))){
                    if (x2){
                      tnpc=(npc*)tplayer->party[0]->more;
                      x2=0;
                      if ((tnpc->wep_right)&&(tnpc->wep_right->type&1023)==OBJ_SHOVEL) {x2=1;}
                      if ((tnpc->wep_left)&&(tnpc->wep_left->type&1023)==OBJ_SHOVEL) {x2=2;}
                      if (x2){
                        if (tnpc->wait_disable<=0){
                          x3=1;
                          if (abs((int)(((tplayer->mobj->more2&1023)-x)<=2)) && (abs(((int)((tplayer->mobj->more2>>10)&1023)-y)<=2))){
                            x3=2+rnd*2;
                            object* item000001=OBJaddnew(x,y,OBJ_CHEST+1024*x3,112,0);//chest
                            for (x3=0;x3<24;x3++) {
                              x4=randchestitem();
                              x5=x4>>10;
                              x4=x4&1023;
                              object* item000002=OBJaddnew(x,y,x4+1024*0,112,x5);
                              /*for (y7=0;y7<1000000000;y7++) { //for testing the new enhance system
                                ENHANCEnewn(item000002,4,4);
                              }*/
                              ENHANCEnewn(item000002,2,4);
                              OBJremove(item000002);//remove from the map (so it can go in a container)
                              OBJaddtocontainer(item000001, item000002); //put this item inside its parent
                            }
                            tplayer->mobj=NULL;
                            txtset(t,"?"); t->d2[0]=8; txtadd(t,"You dig up a chest."); NET_send(NETplayer,tplayer->net,t);
                            x3=2; //double wait time for digging up the treasure.
                          }else{
                            //txtset(t,"?"); t->d2[0]=8; txtadd(t,"Nothing!"); NET_send(NETplayer,tplayer->net,t);
                            // f111 for incorrect treasure map markers
							if (enhancehostn1) {
								txtset(t,"?"); t->d2[0]=8;
								txtadd(t,"Nothing! You dig x=");
								txtnumint(t2, x); txtadd(t, t2); txtadd(t, " y=");
								txtnumint(t2, y); txtadd(t, t2); txtadd(t, " | Map marker x=");
								txtnumint(t2, tplayer->mobj->more2&1023); txtadd(t, t2); txtadd(t, " y=");
								txtnumint(t2, (tplayer->mobj->more2>>10)&1023); txtadd(t, t2);
								NET_send(NETplayer,tplayer->net,t);
							} else {
								txtset(t, "?"); t->d2[0] = 8; txtadd(t, "Nothing!"); NET_send(NETplayer, tplayer->net, t);
							}
                          }
                          if(x2==1){
                            tnpc->wait_disable+=(8.0f-((tnpc->wep_right->more2)&15)*0.5f)*x3;
                          }else{
                            tnpc->wait_disable+=(8.0f-((tnpc->wep_left->more2)&15)*0.5f)*x3;
                          }
                        }else{//wait
                          txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," busy!"); NET_send(NETplayer,tplayer->net,t);
                        }
                      }else{
                        txtset(t,"?"); t->d2[0]=8; txtadd(t,"You need to equip a shovel to dig."); NET_send(NETplayer,tplayer->net,t);
                      }
                    }
                  }//no objects
                  goto skip_pickup;
                }//treasure map
                //other items?


                goto skip_pickup;
              }//end use item on basetile

              if ((tplayer->mobj->type&1023)==414){ //skiff drop
                z2=0;
                if (od[y][x]) z2=1;
                z=bt[y][x]&1023;
                if ((z<8)||(z>=48)){//not water
                  if ((bt[y][x]&1024)==0){//not walkable
                    z2=1;
                  }
                }
                if (z2){
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot put a skiff here!"); NET_send(NETplayer,tplayer->net,t);
                  goto itemdropped;
                }
                goto skiffdrop;
              }

              if (bt[y][x]&1024){
skiffdrop:
                //can it be dropped from any party member in the location?
                //for (z=0;z<=7;z++){
                //myobj2=tplayer->party[z];
                //if (myobj2!=NULL){
                //if (OBJcheckbolt(myobj2->x,myobj2->y,x,y)==FALSE){
                //if ((abs(myobj2->x-x)<=8)&&(abs(myobj2->y-y)<=8)){

                z2=9; myobj2=NULL;
                for (z=0;z<=7;z++){
                  if (myobj3=tplayer->party[z]){
                    if (OBJcheckbolt(myobj3->x,myobj3->y,x,y)==FALSE){
                      z3=abs(myobj3->x-x); z4=abs(myobj3->y-y); if (z4>z3) z3=z4;
                      if (z3<z2){
                        myobj2=myobj3;
                        z2=z3;
                      }
                    }
                  }
                }
                if (myobj2){



                  if ((tplayer->mobj->type&1023)==420){ //balloon drop
                    if ((abs(myobj2->x-x)>1)||(abs(myobj2->y-y)>1)){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot throw a balloon!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped;
                    }
                    if (tplayer->craft==NULL){
                      if (usinghorse(tplayer)==NULL){
                        if ((tplayer->party[tplayer->selected_partymember]->x<1024)||(tplayer->party[tplayer->selected_partymember]->y>=256)){
                          //use balloon
                          myobj=tplayer->mobj; itemused=2; i=myobj->type&1023; i2=myobj->type>>10; tplayer->mobj=NULL; goto useinventoryitem;
                        }
                      }
                    }
                    goto itemdropped;
                  }

                  if (tplayer->mobj->info&256){ //drop quest item
                    txtset(t,"?");
                    t->d2[0]=8;
                    txtadd(t,"You cannot put a quest item here!");
                    NET_send(NETplayer,tplayer->net,t);
                    goto itemdropped;
                  }

                  if ((tplayer->mobj->type&1023)==414){//skiff drop
                    z2=0;
                    if (abs(myobj2->x-x)==1){
                      z=bt[myobj2->y][x]&1023;
                      if ((z<8)||(z>=48)){//not water
                        if ((bt[myobj2->y][x]&1024)==0){//not walkable
                          z2=1;
                        }
                      }
                    }
                    if (abs(myobj2->y-y)==1){
                      z=bt[y][myobj2->x]&1023;
                      if ((z<8)||(z>=48)){//not water
                        if ((bt[y][myobj2->x]&1024)==0){//not walkable
                          z2=1;
                        }
                      }
                    }
                    if ((abs(myobj2->x-x)>1)||(abs(myobj2->y-y)>1)) z2=1;
                    if (z2){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot throw a skiff!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped;
                    }
                  }

                  if (tplayer->mobj->type==149){ //deed drop
                    //in current boat?
                    if ((object*)tplayer->mobj->more==tplayer->party[0]){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot drop this deed now!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped;
                    }
                    //currently holding the boat this deed points to?
                    myobj3=(object*)tplayer->mobj->more;
                    if ((myobj3->x==0)&&(myobj3->y==0)){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You must drop the skiff first!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped;
                    }

                  }
				  
                  if (tplayer->mobj->type==448){//horse papers
                    if (((object*)tplayer->mobj->more)->x==0){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot drop these horse papers now!"); NET_send(NETplayer,tplayer->net,t);
                      goto itemdropped;
                    }
                    crt2=(creature*)((object*)tplayer->mobj->more)->more;
                    crt2->respawn_y=0xFFFF;
                  }//448
				  
				  
                  x5=OBJlist(tplayer->mobj); //x5=last object number
                  OBJlist_last=NULL;
                  //clear notforsale marks
                  for(y6=0;y6<x5;y6++){
                    OBJlist_list[y6]->info|=32768; OBJlist_list[y6]->info^=32768;
                  }//y6



                  /*
                  if (tplayer->mobj->info&128){//stolen item
                  for (i3=0;i3<=15;i3++) if (stolenitemwarning[i3]==tplayer->mobj) goto stolenitem_alreadyadded2;
                  for (i3=0;i3<=15;i3++){
                  if (stolenitemwarning[i3]==NULL){
                  stolenitemwarning[i3]=tplayer->mobj;
                  stolenitemwarning_wait[i3]=2.0f;
                  stolenitemwarning_player[i3]=tplayer;
                  goto stolenitem_alreadyadded2;
                  }//==NULL
                  }//i3
                  }//stolen item
                  stolenitem_alreadyadded2:
                  */
                  if (tplayer->mobj->info&128){//stolen item
                    i3=SFnew(x,y); //destination is more important than the source
                    sf[i3].type=21;//stolen item warning
                    sf[i3].x=x;
                    sf[i3].y=y;
                    sf[i3].more=tplayer->mobj->type;
                    sf[i3].wait=2.0f;
                  }

                  //OBJadd(x,y,tplayer->mobj);
                  //drop sfx!
                  i3=SFnew(x,y); //destination is more important than the source
                  sf[i3].type=9; //GENERIC throw object
                  sf[i3].x=myobj2->x;
                  sf[i3].y=myobj2->y;
                  sf[i3].x2=x;
                  sf[i3].y2=y;
                  sf[i3].more=0xFFFF;
                  sf[i3].wait=tplayer->mobj->type;

                  //drop sound effect
                  if ((tplayer->mobj->type&1023)==88){//gold
                    if (tplayer->mobj->more2>=5){
                      i3=SFnew(x,y);
                      sf[i3].type=4; //sound
                      sf[i3].x=x;
                      sf[i3].y=y;
                      sf[i3].wait=1.0f; //NULL
                      sf[i3].more=6; //coinmany
                    }else{
                      i3=SFnew(x,y);
                      sf[i3].type=4; //sound
                      sf[i3].x=x;
                      sf[i3].y=y;
                      sf[i3].wait=1.0f; //NULL
                      sf[i3].more=7; //coinfew
                    }
                  }

                  //smash it?
                  i4=0;
                  if ((tplayer->mobj->type&1023)==275) i4=1; //potions
                  if (tplayer->mobj->type==115) i4=1; //
                  if (tplayer->mobj->type==116) i4=1; //
                  if (tplayer->mobj->type==117) i4=1; //
                  if (tplayer->mobj->type==118) i4=1; //
                  if (tplayer->mobj->type==119) i4=1; //
                  if (tplayer->mobj->type==120) i4=1; //mug
                  if (tplayer->mobj->type==123){ //mirror
                    if ((abs(myobj2->x-x)>1)||(abs(myobj2->y-y)>1)){
                      sf[i3].wait+=32768; tplayer->mobj->type=123+1024*2;
                      stealing(tplayer,tplayer->mobj);
                    }
                  }
                  if ((tplayer->mobj->type==179)||(tplayer->mobj->type==180)){ //bucket of milk/water
                    if ((abs(myobj2->x-x)>1)||(abs(myobj2->y-y)>1)){
                      tplayer->mobj->type=178;
                      sf[i3].wait=tplayer->mobj->type;
                      stealing(tplayer,tplayer->mobj);
                    }
                  }
                  if (tplayer->mobj->type==92) i4=1; //silver snake venom
                  if (tplayer->mobj->type==161) i4=1; //jug
                  if (tplayer->mobj->type==83) i4=1; //oil flask
                  if (tplayer->mobj->type==48) i4=1; //glass sword
                  if (tplayer->mobj->type==183) i4=1; //jar
                  if (tplayer->mobj->type==184) i4=1; //jar with honey
                  if ((abs(myobj2->x-x)<=1)&&(abs(myobj2->y-y)<=1)) i4=0;
                  if (i4){
                    sf[i3].wait+=32768; OBJrelease(tplayer->mobj);
                    stealing(tplayer,tplayer->mobj);
                  }else{
                    OBJadd(x,y,tplayer->mobj);
                  }

                  tplayer->mobj=NULL; //clear selected mouse object
                  goto itemdropped;
                  //}//<=8
                  //}
                  //}}
                }//myobj2

itemdropped:;
              } //bt flag
            } //myobj==NULL
          } //clicked in vf

          goto skip_pickup;
        } //mobj!=NULL
        //mobj==NULL

        //pickup:
        if ((tplayer->mf==255)&&(tplayer->key&KEYmbclick)){
          if (tplayer->action==0){
            x=tpx+tplayer->mx/32;

            y=tpy+tplayer->my/32;
            myobj=OBJfindlast(x,y);
            if (myobj!=NULL){


              if (myobj->type==51){ //force field
                myobj2=(object*)myobj->prev;
                if (playerlist[tpl]->GNPCflags[3]&(1<<(myobj2->type>>10))) myobj=(object*)myobj->prev;
              }//force field





              i=myobj->type;
              //i2=i>>10; //26/11/2004 removed for Sherry compatibility
              i2=sprlnk[i&1023];


              if (((obji[i2].flags&128)!=0)||(keyon[VK_SPACE]&&U6O_DEBUG)){


                //check if item is next to player
                static unsigned char pickup_partymember;
                z2=0;
                for (z=7;z>=0;z--){
                  myobj2=tplayer->party[z];
                  if (myobj2!=NULL){
                    if (abs(myobj2->x-myobj->x)<=1){
                      if (abs(myobj2->y-myobj->y)<=1){
                        z2=1; pickup_partymember=z;
                      }} }}
                if (z2==1){
                  if ((myobj->info&112)==0){ //this is a fixed map object

                    //void OBJsave(unsigned short x,unsigned short y){

                    OBJsave(myobj->x,myobj->y);
                    //MessageBox(NULL,"OBJsave() called","Ultima 6 Online",MB_OK);

                    //copy everythng! yes everything!
                    //*note no temp flash is needed as a warning

                    //scan through the next list, if we find a container, add it to the list
                    //if there were any container items copy them, then try again
                    //idea! set temp flag while scanning!! (if possible)




                    //set temp flag of any object that can be picked up/used


                  }





                  if ((myobj->type&1023)==73){ //moonstone
                    if ((tplayer->GNPCflags[4]&(1<<(myobj->type>>10)))==0){
                      tplayer->GNPCflags[4]|=(1<<(myobj->type>>10));
                      //duplicate it
                      myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                      tplayer->mobj=myobj2;
                    }
                    goto nopickup;
                  }

                  if ((myobj->type>=242)&&(myobj->type<=249)){ //rune
                    if (myobj->info&256){ //must be a quest item
                      if ((tplayer->GNPCflags[5]&(1<<(myobj->type-242)))==0){
                        tplayer->GNPCflags[5]|=(1<<(myobj->type-242));
                        //duplicate it
                        myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                        tplayer->mobj=myobj2;
                      }
                      goto nopickup;
                    }
                  }

                  if (myobj->type==(275+2*1024)){ //potions
                    if (myobj->info&256){
                      if ((tplayer->GNPCflags[32]&myobj->more2)==0){
                        tplayer->GNPCflags[32]|=myobj->more2;
                        //duplicate it
                        myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                        tplayer->mobj=myobj2;
                      }
                      goto nopickup;
                    }
                  }



                  if ((myobj->type>=400)&&(myobj->type<=407)){ //map(0-7)
                    if (myobj->info&256){ //must be a quest item
                      if ((tplayer->GNPCflags[7]&(1<<(myobj->type-400)))==0){
                        tplayer->GNPCflags[7]|=(1<<(myobj->type-400));
                        //duplicate it
                        myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                        tplayer->mobj=myobj2;
                      }
                      goto nopickup;
                    }
                  }

                  if (myobj->type==62){ //vortex cube
                    if ((myobj->x==1939)&&(myobj->y==57)){
                      if ((tplayer->GNPCflags[6]&(1<<1))==0){
                        tplayer->GNPCflags[6]|=(1<<1);
                        //duplicate it
                        myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                        tplayer->mobj=myobj2;
                      }
                      goto nopickup;
                    }
                    if ((myobj->x==923)&&(myobj->y==850)){
                      goto nopickup;
                    }
                  }

                  if (myobj->type==389){ //silver tablet
                    if ((tplayer->GNPCflags[6]&(1<<2))==0){
                      tplayer->GNPCflags[6]|=(1<<2);
                      //duplicate it
                      myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                      tplayer->mobj=myobj2;
                    }
                    goto nopickup;
                  }
                  if (myobj->type==390){ //silver tabletII
                    if ((tplayer->GNPCflags[6]&(1<<3))==0){
                      tplayer->GNPCflags[6]|=(1<<3);
                      //duplicate it
                      myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                      tplayer->mobj=myobj2;
                    }
                    goto nopickup;
                  }

                  if (myobj->type==270){ //balloon plans
                    if ((tplayer->GNPCflags[6]&(1<<4))==0){
                      tplayer->GNPCflags[6]|=(1<<4);
                      //duplicate it
                      myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                      tplayer->mobj=myobj2;
                    }
                    goto nopickup;
                  }

                  if (myobj->type==395){ //broken gargoyle lens
                    if ((tplayer->GNPCflags[6]&(1<<5))==0){
                      tplayer->GNPCflags[6]|=(1<<5);
                      //duplicate it
                      myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info;
                      tplayer->mobj=myobj2;
                    }
                    goto nopickup;
                  }

                  if (myobj==nuggetsfix){ //nuggets
                    if (tplayer->online>=1536){
                      if ((tplayer->GNPCflags[6]&(1<<6))==0){
                        tplayer->GNPCflags[6]|=(1<<6);
                        //duplicate it
                        myobj2=OBJnew(); myobj2->type=myobj->type; myobj2->info=myobj->info; myobj2->more2=myobj->more2;
                        tplayer->mobj=myobj2;
                      }
                    }
                    goto nopickup;
                  }

                  if (myobj->type==394){ //brit lens
                    goto nopickup;
                  }
                  if (myobj->type==396){ //garg lens
                    goto nopickup;
                  }

                  if (myobj->type==173){ //trap
                    if ((myobj->info&(1<<9))==0) goto nopickup;
                    OBJsave(myobj->x,myobj->y);
                    myobj->more2=tplayer->id; //id
                    myobj->info|=(1<<9);
                    if (tnpc->protection==0){
                      tnpc=(npc*)tplayer->party[pickup_partymember]->more;
                      x6=tnpc->hp>>3; if (!x6) x6=1; //-1/8th health
                      tnpc->hp-=x6;
                      i3=SFnew(tplayer->party[pickup_partymember]->x,tplayer->party[pickup_partymember]->y);
                      sf[i3].type=1; //attack
                      sf[i3].x2=x6;
                      sf[i3].x=tplayer->party[pickup_partymember]->x;
                      sf[i3].y=tplayer->party[pickup_partymember]->y;
                      sf[i3].wait=0.125f; //NULL
                      sf[i3].more=1;
                      tnpc->upflags|=2; //hp
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," wounded!"); NET_send(NETplayer,tplayer->net,t);
                    }//protection
                  }//173

                  if ((myobj->type&1023)==414){ //skiff pickup
                    if (myobj->info&2) goto nopickup; //in use!
                    if ((myobj->info&(1<<9))==0){ //NOT free skiff
                      //deed check
                      for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
                        tnpc3=(npc*)tplayer->party[x6]->more;
                        x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
                      }} //!=NULL,x6
                      OBJlist_last=NULL;
                      for(y6=0;y6<x5;y6++){
                        if (OBJlist_list[y6]->type==149){ //deed
                          if (OBJlist_list[y6]->more==myobj) goto skiffpickup;
                        }
                      }
                    }//NOT free skiff
                    txtset(t,"?"); t->d2[0]=8; txtadd(t,"A deed is required!"); NET_send(NETplayer,tplayer->net,t);
                    goto nopickup;
                  }
skiffpickup:

                  //if (keyon[VK_SHIFT]) CLIENTplayer->key|=KEYquan;
                  if (tplayer->key&KEYquan){
                    if (tplayer->quan){
                      if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){
                        if (tplayer->quan<myobj->more2){
                          myobj2=OBJnew();
                          myobj2->type=myobj->type;
                          myobj2->info=myobj->info;
                          myobj2->more2=tplayer->quan;
                          myobj->more2-=tplayer->quan;
                          tplayer->mobj=myobj2;
                          goto nopickup;
                        }//<
                      }//multiple
                    }//->quan
                  }//KEYquan
                  tplayer->mobj=myobj;
                  OBJremove(tplayer->mobj);

                  //if (objb_last>28039) MessageBox(NULL,"OBJnew: prep1","Ultima 6 Online",MB_OK);

nopickup:;

                }
              } //flag check
            } //mobj!=NULL
          } //action0

          if (tplayer->action==1){ //use!

            x=tpx+tplayer->mx/32;
            y=tpy+tplayer->my/32;

            z2=0;
            for (z=0;z<=7;z++){
              myobj2=tplayer->party[z];
              if (myobj2!=NULL){
                if (abs(myobj2->x-x)<=1){
                  if (abs(myobj2->y-y)<=1){
                    z2=1;
                  }} }}
            if (z2==0) goto finishuse;

            myobj=tplayer->party[tplayer->selected_partymember];
            //tnpc=(npc*)myobj->more;

            /*
            x2=myobj->x;
            y2=myobj->y;
            if (OBJcheckbolt(x2,y2,x,y)){
            myobj=OBJnew();
            myobj->type=338+2048;
            OBJadd(OBJcheckbolt_x,OBJcheckbolt_y,myobj);
            }else{
            //u6ob
            i3=SFnew(x,y); //destination is more important than the source
            sf[i3].type=2; //arrow
            sf[i3].x=x2;
            sf[i3].y=y2;
            sf[i3].x2=x;
            sf[i3].y2=y;
            sf[i3].more=0xFFFF;
            sf[i3].wait=1;
            }
            */



            itemused=0;
            myobj=OBJfindlast(x,y);
retryuse:
            if (myobj==NULL) {myobj=OBJfindlastall(x,y); itemused=1;}//check floating objects
            if (myobj==NULL){
              //basetile exceptions
              /* disabled until katish's dungeon is implemented
              //secret maze entry/exit
              if ((x==193)&&(y==515)){
                if (tplayer->craft==NULL){//not in a balloon (or a boat!)
                  if (usinghorse(tplayer)==NULL){//noone is riding a horse
                    partyadd(tplayer,1876,438);
                  }}}
              if ((x==1876)&&(y==437)){
                if (tplayer->craft==NULL){//not in a balloon (or a boat!)
                  if (usinghorse(tplayer)==NULL){//noone is riding a horse
                    partyadd(tplayer,192,514);
                  }}}*/
              goto finishuse;
            }


            //next items are not people, scan backwards through living creatures to top item!
use_getprevitem:
            if ((myobj->type&1023)!=430){//horses excepted(because you can use a horse)
              if ((myobj->type&1023)!=431){//horses excepted(because you can use a horse)
                if (obji[sprlnk[myobj->type&1023]].v4){
                  myobj=(object*)myobj->prev;
                  if (myobj) goto use_getprevitem;
                  goto  finishuse;
                }
              }}

            static object *use_beforeredirect;

            i=myobj->type&1023;
            i2=myobj->type>>10;
            use_beforeredirect=myobj;
            if ((myobj->info&8)==8) myobj=(object*)myobj->more; //redirector



            //***primary player only objects
            //crafts, ladders, etc




            if (tplayer->craft){//if in a craft player can only exit that craft
              if (myobj!=tplayer->party[0]) goto finishuse;
            }


            //horse exception: you cannot enter a craft if any partymember is on a horse
            static unsigned char use_noladdercheck;
            use_noladdercheck=0;
            for (x3=0;x3<=7;x3++){
              if (myobj3=tplayer->party[x3]){
                tnpc3=(npc*)myobj3->more;
                if (tnpc3->horse){use_noladdercheck=1; goto use_horseexceptionskip;}
              }//myobj3
            }//x3



            myobj2=tplayer->party[0];
            if (abs(myobj2->x-x)>1) goto playeronlyuse_skip;
            if (abs(myobj2->y-y)>1) goto playeronlyuse_skip;



            //skiff or raft
            if (i==414 || i==415){
              use_craft(tplayer,myobj); /* luteijn: replaces whole shebang */
              goto finishuse;
            }

            //exit balloon
            if (i==423){
              //check if walking passable without balloon
              i4=myobj->x; i5=myobj->y; OBJremove(myobj); 
              if (bt[i5][i4]&1024){
                OBJadd(i4,i5,myobj);
                use_craft(tplayer,myobj); 
              }else{
                OBJadd(i4,i5,myobj);
                txtset(t,"?"); t->d2[0]=8; txtadd(t,"Not here!"); NET_send(NETplayer,tplayer->net,t);
              }
              goto finishuse;
            }


            if ((i==305)||(i==326)||(i==308)){ //ladder or dungeon or hole
lastchanceuse_ladder:
              if (tplayer->craft==NULL){
                //choose a point at the ladder and relocate the whole party to that square
                x2=myobj->more2&0xFFFF;
                y2=myobj->more2>>16;

                //for (i4=7;i4>=0;i4--){
                //if (tplayer->party[i4]!=NULL){
                //OBJremove(tplayer->party[i4]);
                //OBJadd(x2,y2,tplayer->party[i4]);
                //}
                //}
                partyadd(tplayer,x2,y2);

              }//->craft
              goto finishuse;
            }


use_horseexceptionskip:

            if ((myobj->type&1023)==84){ //red gate, use gate, use moongate


              if (myobj->more2==0xFFFFFFFF){//to toth's fantastic house ;)
                x2=1342; y2=339; goto tothsgate;
              }
              if (myobj->more2==0xFFFFFFFE){//from toth's fantastic house ;)
                x2=1362; y2=322; goto tothsgate;
              }


              i4=(myobj->info&112)>>4;
              if (i4==1){
                x3=1; y3=2;
                if (myobj->more2){ //created by orb?


                  x3=myobj->more2-1; y3=x3/5; x3=x3-y3*5;
                }
                x2=orbx[x3][y3]; y2=orby[x3][y3];
                //NEWCODE make one square left from avatar moongate go to player's house.
                if (x3==1 && y3==2) {
                  if (i5=playerlist[tpl]->GNPCflags[28]){
                    x2=houseentrancex[i5];
                    y2=houseentrancey[i5];
                  }
                }
                if (U6O_DEBUG){
                  goto tothsgate; //for debugging
                }
                //NEWCODEEND
                x3=1<<(x3+y3*5); if (tplayer->GNPCflags[22]&x3){ //been to dest?
tothsgate:

                  //for (i4=7;i4>=0;i4--){
                  //if (tplayer->party[i4]!=NULL){
                  //OBJremove(tplayer->party[i4]);
                  //OBJadd(x2,y2,tplayer->party[i4]);
                  //}//!=NULL
                  //}//i4
                  partyadd(tplayer,x2,y2);

                }else{//&x3
                  txtset(t,"?"); t->d2[0]=8; txtadd(t,"You cannot enter a red moongate with a destination you have never visited!"); NET_send(NETplayer,tplayer->net,t);
                }//&x3
              }//i4
            }//red gate

            if ((myobj->type&1023)==85){ //moongate
              if (moonphase!=255){

                //for (i4=7;i4>=0;i4--){
                //if (tplayer->party[i4]!=NULL){
                //OBJremove(tplayer->party[i4]);
                //OBJadd(moongatex[moonphase],moongatey[moonphase],tplayer->party[i4]);
                //}
                //}
                partyadd(tplayer,moongatex[moonphase],moongatey[moonphase]);

              }//moonphase
            }//moongate

            //boat
            if (i==412){
              use_craft(tplayer,myobj);
              goto finishuse;
            }


playeronlyuse_skip:


            if (i==431){//horse(with rider)
              tnpc2=(npc*)myobj->more;
              if (tnpc2->player==tplayer){//so you can't dismount other players!

                //find a walkable location to dismount

                x3=myobj->x; y3=myobj->y;//set dismount location

                //set as current location for ease of coding for now

                //restore the horses head
                myobj5=tnpc2->horse; tnpc2->horse=NULL;
                crt2=(creature*)myobj5->more; crt2->wait+=8.0f;
                OBJadd(myobj->x,myobj->y,myobj5);
                myobj5->type=myobj->type-431+430;

                //restore the horses tail
                myobj6=(object*)tnpc2->more;
                myobj6->more=myobj5;
                myobj6->type=myobj6->type-431+430;

                //restore the partymember
                tnpc2->more=NULL;
                myobj->type=tnpc2->originalobjtype; tnpc2->originalobjtype=NULL;
                //move partymember to the side of the horse if possible

                x4=0; y4=0;
                //check adjacent squares of horse's head for walkability
                if (bt[y3-1][x3]&1024){//walkable
                  x4=x3; y4=y3-1;
                }
                if (bt[y3][x3+1]&1024){//walkable
                  if ((x4==0)&&(y4==0)){x4=x3+1; y4=y3;}
                  x5=rnd*2; if (x5){x4=x3+1; y4=y3;}
                }
                if (bt[y3+1][x3]&1024){//walkable
                  if ((x4==0)&&(y4==0)){x4=x3; y4=y3+1;}
                  x5=rnd*2; if (x5){x4=x3; y4=y3+1;}
                }
                if (bt[y3][x3-1]&1024){//walkable
                  if ((x4==0)&&(y4==0)){x4=x3-1; y4=y3;}
                  x5=rnd*2; if (x5){x4=x3-1; y4=y3;}
                }
                if ((x4==0)&&(y4==0)){//try from horse's tail
                  x3=myobj6->x; y3=myobj6->y;
                  if (bt[y3-1][x3]&1024){//walkable
                    x4=x3; y4=y3-1;
                  }
                  if (bt[y3][x3+1]&1024){//walkable
                    if ((x4==0)&&(y4==0)){x4=x3+1; y4=y3;}
                    x5=rnd*2; if (x5){x4=x3+1; y4=y3;}
                  }
                  if (bt[y3+1][x3]&1024){//walkable
                    if ((x4==0)&&(y4==0)){x4=x3; y4=y3+1;}
                    x5=rnd*2; if (x5){x4=x3; y4=y3+1;}
                  }
                  if (bt[y3][x3-1]&1024){//walkable
                    if ((x4==0)&&(y4==0)){x4=x3-1; y4=y3;}
                    x5=rnd*2; if (x5){x4=x3-1; y4=y3;}
                  }
                }//try from horse's tail
                if (x4||y4){
                  OBJmove_allow=TRUE;
                  OBJmove2(myobj,x4,y4);
                  OBJmove_allow=FALSE;
                }

              }//so you can't dismount other players
              goto finishuse;
            }//horse(with rider)


            if (i==430){//horse(unbridled)
              for (x3=-1;x3<=7;x3++){
                x4=x3; if (x3==-1) x4=tplayer->selected_partymember;
                if (myobj2=tplayer->party[x4]){
                  if ( ((abs(myobj2->x-use_beforeredirect->x)<=1)&&(abs(myobj2->y-use_beforeredirect->y)<=1)) || ((abs(myobj2->x-myobj->x)<=1)&&(abs(myobj2->y-myobj->y)<=1)) ){
                    tnpc2=(npc*)myobj2->more;
                    if (tnpc2->horse==NULL){//check that npc is not currently on a horse
                      //lastly, check if we own the deed to that horse!
                      for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
                        tnpc3=(npc*)tplayer->party[x6]->more;
                        x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
                      }}//!=NULL,x6
                      OBJlist_last=NULL;
                      for(y6=0;y6<x5;y6++){
                        if (OBJlist_list[y6]->type==448){//horse papers
                          if ((object*)OBJlist_list[y6]->more==myobj) goto usehorse_gotpapers;
                        }
                      }
                      x5=0;
                      //is this a wild horse?
                      if ((abs(myobj->x-280)<=32)&&(abs(myobj->y-441)<=32)) x5=1;
                      if (myobj->info&4){crt2=(creature*)myobj->more; if (crt2->flags&128) x5=1;}
                      txtset(t,"?"); t->d2[0]=8;
                      if (x5) txtadd(t,"You don't own this horse!"); else txtadd(t,"You cannot ride a wild horse!");
                      NET_send(NETplayer,tplayer->net,t); goto finishuse;

usehorse_gotpapers:
                      tnpc2->originalobjtype=myobj2->type;
                      OBJremove(myobj2);
                      OBJadd(myobj->x,myobj->y,myobj2);
                      myobj2->type=myobj->type-430+431;
                      crt2=(creature*)myobj->more;
                      tnpc2->more=crt2->more; myobj3=(object*)crt2->more; myobj3->more=myobj2;//make rear end of horse point to partymember(myobj2)
                      myobj3->type=myobj3->type-430+431;
                      OBJremove(myobj);//remove front end of horse
                      tnpc2->horse=myobj;//backup pointer to front half of horse for later
                      goto finishuse;
                    }
                  }
                }
              }
              goto finishuse;
            }//horse(unbridled)


            if (i==163) { //use bed in your own house to rest
              if (i4=playerlist[tpl]->GNPCflags[28]){
                //check if the bed is within boundaries
                x7=myobj->x; y7=myobj->y;
                if (x7>=housex1[i4]){ if (x7<=housex2[i4]){ if (y7>=housey1[i4]){ if (y7<=housey2[i4]){
                  for (y6=0;y6<housepnext[i4];y6++){
                    if (x7==housepx[i4][y6]){ if (y7==housepy[i4][y6]){
                      if (houseowner(playerlist[tpl],i4)) {tplayer->rest=17;}
                      goto finishuse;
                    }}//housepx,housepy
                  }//y
                }}}}//boundries check
              }
              goto finishuse;
            }

            if (i==223){ //powder keg
              OBJsave(myobj->x,myobj->y);
              if (i2) {myobj->type=i; myobj->more2=tplayer->id; goto finishuse;}
              myobj->type=i+1*1024;
              myobj->info|=7680; //1111000000000 obj reserved timer
              myobj->more2=tplayer->id;
              stealing(tplayer,myobj);
              goto finishuse;
            }

            if (i==334){ //secret door
              use_basic_toggle(NULL,myobj);
              OBJcheckflags(x,y);
              goto finishuse;
            }

            if (i==164){ //fireplace
              use_basic_toggle(NULL,myobj);
              goto finishuse;
            }

            if (i==206){ //brazier
              if (myobj->type>>10<2) {
                use_basic_toggle(NULL,myobj);
              } else {
                /* purple/blue brazier can't be doused */
              }
              goto finishuse;
            }

            if (i==253){ //campfire
              use_basic_toggle(NULL,myobj);
              goto finishuse;
            }

            if (i==268){ //lever
              use_toggle(NULL,myobj); 
              goto finishuse;
            }

            if (i==288){ //crank
              use_toggle(NULL,myobj); 
              goto finishuse;
            }

            if (i==174){ //switch
              use_toggle(NULL,myobj);
              goto finishuse;
            }
            if (i==122){ //candle
              OBJsave(myobj->x,myobj->y);
              use_basic_toggle(NULL,myobj);
              goto finishuse;
            }
            if (i==145){ //candelabra
              OBJsave(myobj->x,myobj->y);
              use_basic_toggle(NULL,myobj);
              goto finishuse;
            }
            if (i==186){ //barrel
              OBJsave(myobj->x,myobj->y);
              use_basic_toggle(NULL,myobj);
              goto finishuse;
            }
            if (i==192){ //crate
              OBJsave(myobj->x,myobj->y);
              use_basic_toggle(NULL,myobj);
              goto finishuse;
            }
            if (i==98){ //chest
              if (myobj->type>>10<2) { //can only use unlocked chests
                OBJsave(myobj->x,myobj->y);
                use_basic_toggle(NULL,myobj);
              }
              goto finishuse;
            }

            if (i==62){ //vortex cube use

              if (tplayer->GNPCflags[19]){ //replay endgame sequence?
                if ((myobj->x==923)&&(myobj->y==850)){
                  if ((tplayer->GNPCflags[18]&16)==0){
                    tplayer->GNPCflags[18]|=(2+4); //restore viewing of lenses
                    txtset(t,"?"); t->d2[0]=22; NET_send(NETplayer,tplayer->net,t); //play endgame sequence!
                    tplayer->GNPCflags[18]|=16; //endgame sequence played!
                  }
                }
                goto finishuse;
              }

              if ((tplayer->GNPCflags[18]&7)==7){
                if ((myobj->x==923)&&(myobj->y==850)){
                  if (tplayer->party[0]){
                    tplayer->GNPCflags[18]|=8; //don't show codex!
                    tplayer->GNPCflags[19]=1; //sacred quest complete!!!

                    tplayer->karma+=20.0f;
                    tnpc3=(npc*)tplayer->party[0]->more;
                    tnpc3->s+=3; tnpc3->d+=3; tnpc3->i+=3;
                    tnpc3->mp_max=tnpc3->i*2; //primary party member INT*2
                    tnpc3->wt_max=tnpc3->s*4*100;
                    tnpc3->wt2_max=tnpc3->s*50;
                    //remove amulet of submission
                    if (tnpc3->neck){
                      if (tnpc3->neck->type==76) tnpc3->neck=NULL;
                    }
                    tnpc3->upflags|=1;

                    myobj3=OBJnew(); myobj3->type=87; myobj3->more2=0; myobj3->info=256; //orb of moons
                    additemroot(tnpc3,myobj3);
                    tplayer->GNPCflags[22]|=0xFFFFFFFF; //make all destinations available

                    txtset(t,"?"); t->d2[0]=22; NET_send(NETplayer,tplayer->net,t); //play endgame sequence!
                    tplayer->GNPCflags[18]|=16; //endgame sequence played!

                  }
                }
              }
              goto finishuse;
            }




            /*
            if (i==90){ //torch
            if (i2==0) myobj->type=i+1*1024;
            if (i2>=1) myobj->type=i;
            goto finishuse;
            }
            */
            if (i==OBJ_PASSTHROUGH_V){ //v-passthrough
              if (i2==0){
                if (myobj->more==NULL)
                {
                  myobj->more=OBJnew();
                  myobj2=(object*)myobj->more;
                  myobj2->more=myobj; //back link
                  myobj2->type=OBJ_PASSTHROUGH_V+1*1024;
                  OBJadd(myobj->x-1,myobj->y-1,myobj2);
                }
                else
                {
                  OBJadd(myobj->x-1,myobj->y-1,(object*)myobj->more);
                }
                x=myobj->x;
                y=myobj->y-1;
                OBJremove(myobj);
                myobj->type=OBJ_PASSTHROUGH_V+2*1024;
                OBJadd(x,y,myobj);
                goto finishuse;
              }
              else
              {
                if (i2==1){myobj2=myobj; myobj=(object*)myobj->more;}else{myobj2=(object*)myobj->more;}
                if (myobj2!=NULL) OBJremove(myobj2);
                x=myobj->x; y=myobj->y+1;
                OBJremove(myobj);
                myobj->type=OBJ_PASSTHROUGH_V;
                OBJadd(x,y,myobj);
                goto finishuse;
              }
            }

            if (i==OBJ_PASSTHROUGH_H){ //h-passthrough
              if (i2==0){
                if (myobj->more==NULL)
                {
                  myobj->more=OBJnew();
                  myobj2=(object*)myobj->more;
                  myobj2->more=myobj; //back link
                  myobj2->type=OBJ_PASSTHROUGH_H+1*1024;
                  OBJadd(myobj->x-1,myobj->y-1,myobj2);
                }
                else
                {
                  OBJadd(myobj->x-1,myobj->y-1,(object*)myobj->more);
                }
                x=myobj->x-1;
                y=myobj->y;
                OBJremove(myobj);
                myobj->type=OBJ_PASSTHROUGH_H+2*1024;
                OBJadd(x,y,myobj);
                goto finishuse;
              }
              else
              {
                if (i2==1){myobj2=myobj; myobj=(object*)myobj->more;}else{myobj2=(object*)myobj->more;}
                if (myobj2!=NULL) OBJremove(myobj2);
                x=myobj->x+1; y=myobj->y;
                OBJremove(myobj);
                myobj->type=OBJ_PASSTHROUGH_H;
                OBJadd(x,y,myobj);
                goto finishuse;
              }
            }

            for (i4=297;i4<=300;i4++){
              if (i==i4){ //oak door, ...
                if (i2>=8){
                  txtsetchar(t,8); txtaddchar(t,255); txtadd(t,"It's locked!"); NET_send(NETplayer,tplayer->net,t);
                  goto finishuse; //door is locked!
                }
                if (myobj->info&8) myobj=(object*)myobj->more; //<-
                myobj2=(object*)myobj->more;
                i2=myobj->type>>10;

                if (i2&1) {myobj2=myobj; myobj=(object*)myobj->more;} else {myobj2=(object*)myobj->more;}

                if (i2<4){
                  i2=i2>>1;
                  i2=i2<<1;
                  i2=i2&2;
                  myobj->type=i+((i2+4)*1024);
                  myobj2->type=i+((i2+5)*1024);
                  OBJcheckflags(myobj->x,myobj->y);
                  OBJcheckflags(myobj2->x,myobj2->y);

                  x2=myobj->x; y2=myobj->y;
                  i3=SFnew(x2,y2);
                  //i3 points to new sf
                  sf[i3].type=4;
                  sf[i3].x=x2;
                  sf[i3].y=y2;
                  sf[i3].wait=1.0f; //NULL
                  sf[i3].more=2;

                  goto finishuse;
                }else{
                  i2=i2>>1;
                  i2=i2<<1;
                  i2=i2&2;
                  myobj->type=i+(i2*1024);
                  myobj2->type=i+((i2+1)*1024);
                  OBJcheckflags(myobj->x,myobj->y);
                  OBJcheckflags(myobj2->x,myobj2->y);

                  x2=myobj->x; y2=myobj->y;
                  i3=SFnew(x2,y2);
                  //i3 points to new sf
                  sf[i3].type=4;
                  sf[i3].x=x2;
                  sf[i3].y=y2;
                  sf[i3].wait=1.0f; //NULL
                  sf[i3].more=1;

                  goto finishuse;
                } //<4
              } //i==i4
            } //i4 loop



useinventoryitem:

            //if musical instrument is in inventory, then it can be used
            if ((i==153)||(i==158)||(i==296)||(i==156)||(i==157)){ //panpipes, lute, xylophone, harpsichord, harp
              //***assume proximity ok***
              //set client in music mode with selected instrument
              txtset(t,"??"); t->d2[0]=24;
              if (i==153) t->d2[1]=3;
              if (i==158) t->d2[1]=0;
              if (i==296) t->d2[1]=4;
              if (i==156) t->d2[1]=1;
              if (i==157) t->d2[1]=2;
              tplayer->instrument=t->d2[1];
              tplayer->playinstrument=2;
              NET_send(NETplayer,tplayer->net,t);
              goto finishuse;
            }

            /*
            //if musical instrument is in inventory, then it can be used
            if ((i==153)||(i==158)||(i==296)){ //panpipes, lute, xylophone
            //set client in music mode




            goto finishuse;
            }
            */


            if (i==169){ //ducky
              myobj3=tplayer->party[0];
              i3=SFnew(myobj3->x,myobj3->y);
              sf[i3].type=4; //sound
              sf[i3].x=myobj3->x;
              sf[i3].y=myobj3->y;
              sf[i3].wait=1.0f; //NULL
              sf[i3].more=5; //ducky
              goto finishuse;
            }

            if (i==OBJ_GEM){ //gems
              x3=10;
              if (x3>255) x3=255; //cap
              if (x3>tplayer->peer){
                tplayer->peer=x3;
                txtset(t,"??"); t->d2[0]=48; t->d2[1]=tplayer->peer; NET_send(NETplayer,tplayer->net,t);
              }
              if (myobj->more2!=1){ //more than 1!
                myobj->more2--; goto finishuse;
              }
              OBJremove(myobj); OBJrelease(myobj);
              myobj=NULL;
              goto finishuse;
            }

            if (i==212){ //magic fan
              x3=tplayer->windx; y3=tplayer->windy;
              if ((x3==0)&&(y3==0)) {x3=windx; y3=windy;}
              if ((x3==0)&&(y3==0)) {x3=0; y3=1; goto magicfan_used;}
              if ((x3==0)&&(y3==1)) {x3=-1; y3=1; goto magicfan_used;}
              if ((x3==-1)&&(y3==1)) {x3=-1; y3=0; goto magicfan_used;}
              if ((x3==-1)&&(y3==0)) {x3=-1; y3=-1; goto magicfan_used;}
              if ((x3==-1)&&(y3==-1)) {x3=0; y3=-1; goto magicfan_used;}
              if ((x3==0)&&(y3==-1)) {x3=1; y3=-1; goto magicfan_used;}
              if ((x3==1)&&(y3==-1)) {x3=1; y3=0; goto magicfan_used;}
              if ((x3==1)&&(y3==0)) {x3=1; y3=1; goto magicfan_used;}
              if ((x3==1)&&(y3==1)) {x3=0; y3=1; goto magicfan_used;}
magicfan_used:
              tplayer->windx=x3; tplayer->windy=y3;
              txtset(t,"??"); t->d2[0]=14; t->d2[1]=(tplayer->windx+1)+(tplayer->windy+1)*4; NET_send(NETplayer,tplayer->net,t);
              goto finishuse;
            }

            if (i==93){ //sextant
              //*sextants 304,360
              myobj=tplayer->party[tplayer->selected_partymember];
              x2=0;
              if (myobj->y>=256) x2=2;
              if (myobj->x<1024) x2=1;
              if (x2){
                if (x2==1){
                  x=(myobj->x+1024-304)/8; x-=128;
                  y=(myobj->y+1024-360)/8; y-=128;
                }else{
                  x=((myobj->x-1024)+256-76)/2; x-=128;
                  y=((myobj->y-256)+256-90)/2; y-=128;
                }
                txtset(t,"?"); t->d2[0]=8;
                txtnumint(t2,abs(y)); txtadd(t,t2);
                if (y>0) txtadd(t,"'S, "); else  txtadd(t,"'N, ");
                txtnumint(t2,abs(x)); txtadd(t,t2);
                if (x>0) txtadd(t,"'E"); else  txtadd(t,"'W");
                NET_send(NETplayer,tplayer->net,t);
              }else{
                txtset(t,"?Not usable here!"); t->d2[0]=8; NET_send(NETplayer,tplayer->net,t);
              }
              goto finishuse;
            }


            if (i==270){ //use balloon plans
              //scan for required items
              for (x6=0;x6<=7;x6++){ if (tplayer->party[x6]){
                tnpc3=(npc*)tplayer->party[x6]->more;
                x5=OBJlist((object*)tnpc3->items->more); //x5=last object number
              }} //!=NULL,x6
              OBJlist_last=NULL;
              y7=0;
              for(y6=0;y6<x5;y6++){
                if ((OBJlist_list[y6]->type&1023)==421) y7|=1; //silk bag
                if ((OBJlist_list[y6]->type&1023)==422) y7|=2; //basket
                if ((OBJlist_list[y6]->type&1023)==284) y7|=4; //rope
                if ((OBJlist_list[y6]->type&1023)==283) y7|=8; //anchor
                if ((OBJlist_list[y6]->type&1023)==147) y7|=16; //cauldron
              }
              if (y7!=31){
                txtset(t,"?Requires a balloon basket, a mammoth silk bag, rope, an anchor and a cauldron."); t->d2[0]=8; NET_send(NETplayer,tplayer->net,t);
                goto finishuse;
              }
              //remove required items
              for(y6=0;y6<x5;y6++){
                if (((OBJlist_list[y6]->type&1023)==421)&&(y7&1)) {y7-=1; OBJremove(OBJlist_list[y6]);}
                if (((OBJlist_list[y6]->type&1023)==422)&&(y7&2)) {y7-=2; OBJremove(OBJlist_list[y6]);}
                if (((OBJlist_list[y6]->type&1023)==284)&&(y7&4)) {y7-=4; OBJremove(OBJlist_list[y6]);}
                if (((OBJlist_list[y6]->type&1023)==283)&&(y7&8)) {y7-=8; OBJremove(OBJlist_list[y6]);}
                if (((OBJlist_list[y6]->type&1023)==147)&&(y7&16)) {y7-=16; OBJremove(OBJlist_list[y6]);}
              }
              //add balloon to inventory (make sure it's a quest item)
              myobj3=OBJnew(); myobj3->type=420; myobj3->info|=256; //quest item
              tnpc3=(npc*)tplayer->party[0]->more;
              additemroot(tnpc3,myobj3);
              goto finishuse;
            }

            if (i==420){ //balloon
              if (tplayer->craft==NULL){
                if (usinghorse(tplayer)==NULL){

                  if ((tplayer->party[0]->x<1024)||((tplayer->party[0]->y>=256)&&(tplayer->party[0]->y<=512)&&(tplayer->party[0]->x<=1280) ) ){

                    if (tplayer->selected_partymember){ //change selected_partymember to 0
                      if (tplayer->party[tplayer->selected_partymember]){
                        tnpc3=(npc*)tplayer->party[tplayer->selected_partymember]->more;
                        if (tnpc3->order==0) tnpc3->order=1;
                        tnpc3->follow=tplayer->party[0];
                      }
                      tnpc3=(npc*)tplayer->party[0]->more;
                      if (tnpc3->order==1) tnpc3->order=0;
                      tnpc3->follow=tplayer->party[0];
                      tplayer->selected_partymember=0;
                    }//tplayer->selected_partymember





                    //remove item from inventory
                    myobj2=myobj; OBJremove(myobj2);

                    //create a balloon where the avatar is standing
                    myobj=tplayer->party[0]; x=myobj->x; y=myobj->y;

                    myobj2->type=423+1024*3; OBJadd(x,y,myobj2);
                    static mlobj *mmyobj; //array size varies
                    mmyobj=(mlobj*)malloc(sizeof(object*)*3);
                    myobj2->more=mmyobj;
                    myobj=OBJnew(); mmyobj->obj[0]=myobj; //middle
                    myobj->type=423+0*1024; myobj->info|=8; //<-base object
                    OBJadd(x-1,y-1,myobj); myobj->more=myobj2;
                    myobj=OBJnew(); mmyobj->obj[1]=myobj; //middle
                    myobj->type=423+1*1024; myobj->info|=8; //<-base object
                    OBJadd(x,y-1,myobj); myobj->more=myobj2;
                    myobj=OBJnew(); mmyobj->obj[2]=myobj; //middle
                    myobj->type=423+2*1024; myobj->info|=8; //<-base object
                    OBJadd(x-1,y,myobj); myobj->more=myobj2;

                    //use balloon (the one we just created :)
                    tplayer->craft_con=(object*)myobj2->more;
                    myobj2->more=tplayer->party[0]->more;
                    myobj2->info=tplayer->party[0]->info;
                    tplayer->craft=tplayer->party[0];
                    for (i4=7;i4>=0;i4--){
                      if (tplayer->party[i4]!=NULL){
                        OBJremove(tplayer->party[i4]);
                        VLNKremove(tplayer->party[i4]); VLNKsremove(tplayer->party[i4]);
                        tnpc3=(npc*)tplayer->party[i4]->more; tnpc3->target=NULL;
                      }
                    }
                    tplayer->party[0]=myobj2;
                    if (myobj2->info&32768) myobj2->info-=32768;

                  }//x,y
                }//usinghorse
              }//->craft==NULL
              goto finishuse;
            }

            if (itemused==0){
              myobj=NULL; goto retryuse;
            }

            if (itemused==1){
              //No use-able object has been located on top, scan for items which may be used whilst not the top item
              myobj=od[y][x];
lastchanceuse_loop:
              i=myobj->type&1023;
              myobj2=tplayer->party[0];
              if (abs(myobj2->x-x)>1) goto lastchanceuse_playeronlyuse_skip;
              if (abs(myobj2->y-y)>1) goto lastchanceuse_playeronlyuse_skip;

              if (use_noladdercheck==0){
                if ((i==305)||(i==326)||(i==308)) goto lastchanceuse_ladder;//ladder or dungeon or hole
              }

lastchanceuse_playeronlyuse_skip:
              if (myobj->next){ myobj=(object*)myobj->next; goto lastchanceuse_loop;}
            }//itemused==1

finishuse:;
            goto skip_pickup;
          } //action==1


          if (tplayer->action==2){ //look ->primary surface
            x=tpx+tplayer->mx/32;
            y=tpy+tplayer->my/32;
            myobj=OBJfindlast(x,y);
            if (myobj==NULL){
              myobj=OBJfindlastall(x,y);
              if (myobj==NULL) goto looknullobject;
            }
            if (myobj->info&8) myobj=(object*)myobj->more; //redirect!


            if (myobj->type==51){ //force field
              myobj2=(object*)myobj->prev;
              if (tplayer->GNPCflags[3]&(1<<(myobj2->type>>10))) myobj=(object*)myobj->prev;
              if (tplayer->GNPCflags[4]&(1<<(myobj2->type>>10))) myobj=(object*)myobj->prev;
            }//force field

            if ((myobj->type>=242)&&(myobj->type<=249)){ //skip rune LOOK
              if (tplayer->GNPCflags[5]&(1<<(myobj->type-242))) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==(275+2*1024)){//yellow potion
              if (myobj->info&256){//quest
                if (tplayer->GNPCflags[32]&myobj->more2) myobj=(object*)myobj->prev;
                if (myobj==NULL) goto looknullobject;
              }//quest
            }//yellow potion

            if ((myobj->type>=400)&&(myobj->type<=407)){ //skip map(0-7) LOOK
              if (tplayer->GNPCflags[7]&(1<<(myobj->type-400))) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==62){ //vortex cube SKIPLOOK
              if ((myobj->x==1939)&&(myobj->y==57)){
                if (tplayer->GNPCflags[6]&(1<<1)) myobj=(object*)myobj->prev;
                if (myobj==NULL) goto looknullobject;
              }
              if ((myobj->x==923)&&(myobj->y==850)){
                if ((tplayer->GNPCflags[18]&1)==0) myobj=(object*)myobj->prev;
                if (myobj==NULL) goto looknullobject;
              }
            }

            if (myobj->type==389){ //silver tablet
              if (tplayer->GNPCflags[6]&(1<<2)) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }
            if (myobj->type==390){ //silver tabletII
              if (tplayer->GNPCflags[6]&(1<<3)) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==270){ //balloon plans
              if (tplayer->GNPCflags[6]&(1<<4)) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==395){ //broken gargoyle lens
              if (tplayer->GNPCflags[6]&(1<<5)) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj==nuggetsfix){ //nuggets
              if ((tplayer->GNPCflags[6]&(1<<6))||(tplayer->online<1536)) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==59){ //codex
              if (tplayer->GNPCflags[18]&8) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==394){ //brit lens SKIPLOOK
              if ((tplayer->GNPCflags[18]&2)==0) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }
            if (myobj->type==396){ //garg lens SKIPLOOK
              if ((tplayer->GNPCflags[18]&4)==0) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==416){ //nothing!
              myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->type==173){ //trap
              if ((myobj->info&(1<<9))==0) myobj=(object*)myobj->prev;
              if (myobj==NULL) goto looknullobject;
            }

            if (myobj->info&4){ //invisible crt
              crt=(creature*)myobj->more;
              if (crt->flags&2){
                myobj=(object*)myobj->prev;
                if (myobj==NULL) goto looknullobject;
              }//2
            }//4

            for (x2=0;x2<=nresu;x2++){ //dead NPC? (only player who owns the dead npc can extract)
              if (resu[x2]){
                if (resu_body[x2]==myobj){
                  if (tplayer==resu_player[x2]){

                    //check if item is next to player
                    z2=0; for (z=0;z<=7;z++){
                      if (myobj2=tplayer->party[z]){
                        if (abs(myobj2->x-myobj->x)<=1){
                          if (abs(myobj2->y-myobj->y)<=1){
                            z2=1;
                          }}}}
                    if (z2==1){

                      x3=0;
                      tnpc=(npc*)resu[x2]->more;
leaveitems3:
                      if (myobj3=(object*)tnpc->items->more){
                        x3=1;
                        OBJremove(myobj3);
                        OBJadd(myobj->x,myobj->y,myobj3);
                        goto leaveitems3;
                      }
                      if (myobj3=tnpc->helm){ x3=1; tnpc->helm=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (myobj3=tnpc->neck){ x3=1; tnpc->neck=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (myobj3=tnpc->wep_left){ x3=1;
                      if (myobj3->type==(90+1024)) myobj3->type=90;
                      tnpc->wep_left=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (myobj3=tnpc->wep_right){ x3=1;
                      if (myobj3->type==(90+1024)) myobj3->type=90;
                      tnpc->wep_right=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (myobj3=tnpc->armour){ x3=1; tnpc->armour=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (myobj3=tnpc->ring_left){ x3=1; tnpc->ring_left=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (myobj3=tnpc->ring_right){ x3=1; tnpc->ring_right=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (myobj3=tnpc->boots){ x3=1; tnpc->boots=NULL; OBJadd(myobj->x,myobj->y,myobj3);}
                      if (x3){
                        tnpc->baseitem=tnpc->items; tnpc->baseitem_offset=0;
                        txtset(t,"?You search "); txtadd(t,tnpc->name); txtadd(t,"'s dead body."); t->d2[0]=8; NET_send(NETplayer,tplayer->net,t);
                        goto skip_pickup;
                      }//x3
                    }//z2
                  }
                }
              }
            }


            //if it is a container, retrieve items inside (if empty give stats)
            if (myobj->more){
              x2=myobj->type&1023;
              x3=myobj->type>>10;
              x4=0;
              if ((obji[sprlnk[x2]].flags&1024)&&(x3==0)) x4=1;
              if (x2==168) x4=1; //remains
              if (x2==339) x4=1; //dead body
              if (x2==176) x4=1; //drawers
              if (x2==177) x4=1; //desk
              if (x2==335) x4=1;
              if (x2==134) x4=1; //carcass
              if (x2==340) x4=1; //garg
              if (x2==341) x4=1; //cyclops
              if ((x2>=432)&&(x2<=447)) x4=1;

              //other special cases?
              if (x4==1){ //extract items out of container
                //check if item is next to player
                z2=0; for (z=0;z<=7;z++){
                  myobj2=tplayer->party[z];
                  if (myobj2!=NULL){
                    if (abs(myobj2->x-myobj->x)<=1){
                      if (abs(myobj2->y-myobj->y)<=1){
                        z2=1;
                      }} }}
                if (z2==1){
                  if (myobj->more) OBJsave(myobj->x,myobj->y);
extractobj:
                  myobj2=(object*)myobj->more;
                  if (myobj2) {OBJremove(myobj2); myobj2->info|=112; OBJadd(myobj->x,myobj->y,myobj2); goto extractobj;}
                  goto skip_pickup;
                }//z2
              }//x4==1
            }//->more


            //f9=0;
inventory_look:

            //if it's a player give a different description with their name
            if (myobj->info&2){//npc
              tnpc2=(npc*)myobj->more;
              if (tnpc2->player){
                tplayer2=(player*)tnpc2->player;

                /*
                if (tplayer2!=tplayer){//not a member of our party
                if (tnpc2->port){
                txtset(t5,"?"); t5->d2[0]=2;
                txtset(t7,"??"); t7->ds[0]=1; txtadd(t5,t7);
                t7->ds[0]=28; txtadd(t5,t7);//type=portrait look
                t7->ds[0]=myobj->x; txtadd(t5,t7);
                t7->ds[0]=myobj->y; txtadd(t5,t7);
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=tnpc2->port; txtadd(t5,t7);//more=port
                txtset(t7,"????"); t7->df[0]=1.0f; txtadd(t5,t7); //wait
                NET_send(NETplayer,tplayer->net,t5);
                }
                }
                */

                //send npc look message
                /*
                [44]
                [portrait]
                [length of name]
                [name data(if any!)]
                [1 byte value with bits indicating what is equipped]
                order is:
                helm,wep-right,wep-left,armour,boots,ring right, ring left,neck
                2 bytes for type, and 1 byte for a plus value per item



                */

                if (tnpc2->port){
                  txtsetchar(t5,44);
                  txtaddshort(t5,tnpc2->port);
                  //name is ALWAYS known for npcs with a ->player pointer
                  txtaddchar(t5,tnpc2->name->l);
                  txtadd(t5,tnpc2->name);

                  i3=0xFFFFFF;
                  if (tplayer2->party[0]==myobj){
                    i3=tplayer2->GNPCflags[245];
                  }
                  txtaddchar(t5,i3&0xFF);
                  txtaddchar(t5,(i3>>8)&0xFF);
                  txtaddchar(t5,(i3>>16)&0xFF);

                  txtset(t6,"");
                  i3=0;
                  i5=1;
                  for (i4=0;i4<8;i4++){
                    if (i4==0) myobj3=tnpc2->helm;
                    if (i4==1) myobj3=tnpc2->wep_right;
                    if (i4==2) myobj3=tnpc2->wep_left;
                    if (i4==3) myobj3=tnpc2->armour;
                    if (i4==4) myobj3=tnpc2->boots;
                    if (i4==5) myobj3=tnpc2->ring_right;
                    if (i4==6) myobj3=tnpc2->ring_left;
                    if (i4==7) myobj3=tnpc2->neck;
                    if (myobj3){
                      i3+=i5;
                      //type
                      txtaddshort(t6,myobj3->type);
                      //+ bonus
                      i6=ENHANCEget_attack(myobj3);
                      if (!i6) i6=ENHANCEget_defense(myobj3);
                      txtaddchar(t6,i6);
                    }//myobj3
                    i5<<=1;
                  }//i4
                  txtaddchar(t5,i3);
                  txtadd(t5,t6);
                  NET_send(NETplayer,tplayer->net,t5);
                }//tnpc2->port








                if (tplayer2->party[0]==myobj){

                  txtsetchar(t,8); txtaddchar(t,255);
                  txtadd(t,"Thou dost see ");
                  if (tplayer==tplayer2) {txtadd(t,"yourself."); goto skip_playername2;}
                  txtadd(t,tnpc2->name);
                  txtadd(t,". (Level ");
                  txtnumint(t2,tnpc2->lev); txtadd(t,t2);
                  i3=tnpc2->hp_max;
                  txtset(t2,")");
                  if (tnpc2->hp<i3) txtset(t2,", grazed)");
                  if (tnpc2->hp<(i3*4/5)) txtset(t2,", barely wounded)");
                  if (tnpc2->hp<(i3*3/5)) txtset(t2,", lightly wounded)");
                  if (tnpc2->hp<(i3*2/5)) txtset(t2,", heavily wounded)");
                  if (tnpc2->hp<(i3/5)) txtset(t2,", critically hurt)"); 
                  txtadd(t,t2);
skip_playername2:
                  goto skip_playername;
                }
                if (tnpc2->name){
                  //MUST BE A PARTY MEMBER!
                  txtsetchar(t,8); txtaddchar(t,255);
                  txtadd(t,"Thou dost see ");
                  txtadd(t,tnpc2->name);
                  txtadd(t,". (Level ");
                  txtnumint(t2,tnpc2->lev); txtadd(t,t2);
                  i3=tnpc2->hp_max;
                  txtset(t2,")");
                  if (tnpc2->hp<i3) txtset(t2,", grazed)");
                  if (tnpc2->hp<(i3*4/5)) txtset(t2,", barely wounded)");
                  if (tnpc2->hp<(i3*3/5)) txtset(t2,", lightly wounded)");
                  if (tnpc2->hp<(i3*2/5)) txtset(t2,", heavily wounded)");
                  if (tnpc2->hp<(i3/5)) txtset(t2,", critically hurt)"); 
                  txtadd(t,t2);
                  goto skip_playername;
                }

              }else{
                //no player pointer npc!









                //portrait look (non-player npc)
                if (tnpc2->port){
                  txtsetchar(t5,44);
                  txtaddshort(t5,tnpc2->port);




                  //name is ALWAYS known for npcs with a ->player pointer
                  //txtaddchar(t5,tnpc2->name->l);
                  //txtadd(t5,tnpc2->name);

                  //set t6 to stock answer
                  i3=objname[(myobj->type&1023)*2];
                  i4=objname[(myobj->type&1023)*2+1];
                  txtNEWLEN(t6,i4);
                  memcpy(&t6->d2[0],&objname2[i3],i4);

                  //retrieve name if known
                  //to have a name they must have a converse number
                  if (i4=tnpc2->converse){
                    if (i4!=201){
                      if (i4==255) i4=0;//Dupre correction
                      if ((tplayer->NPCflags[i4]&256)>>8){
                        tfh=open(".\\host\\npcname.txt");
                        for (i3=0;i3<=i4;i3++){
                          txtfilein(t6,tfh);
                        }
                        close(tfh);
                      }
                    }else{
                      //201!
                      txtset(t6,tnpc2->name);
                    }
                  }

                  txtaddchar(t5,t6->l);
                  txtadd(t5,t6);


                  i3=0xFFFFFF;
                  txtaddchar(t5,i3&0xFF);
                  txtaddchar(t5,(i3>>8)&0xFF);
                  txtaddchar(t5,(i3>>16)&0xFF);

                  txtset(t6,"");
                  i3=0;
                  i5=1;
                  for (i4=0;i4<8;i4++){
                    if (i4==0) myobj3=tnpc2->helm;
                    if (i4==1) myobj3=tnpc2->wep_right;
                    if (i4==2) myobj3=tnpc2->wep_left;
                    if (i4==3) myobj3=tnpc2->armour;
                    if (i4==4) myobj3=tnpc2->boots;
                    if (i4==5) myobj3=tnpc2->ring_right;
                    if (i4==6) myobj3=tnpc2->ring_left;
                    if (i4==7) myobj3=tnpc2->neck;
                    if (myobj3){
                      i3+=i5;
                      //type
                      txtaddshort(t6,myobj3->type);
                      //+ bonus
                      i6=ENHANCEget_attack(myobj3);
                      if (!i6) i6=ENHANCEget_defense(myobj3);
                      txtaddchar(t6,i6);
                    }//myobj3
                    i5<<=1;
                  }//i4
                  txtaddchar(t5,i3);
                  txtadd(t5,t6);
                  NET_send(NETplayer,tplayer->net,t5);
                }//tnpc2->port













                /*
                if (tnpc2->port){
                txtset(t5,"?"); t5->d2[0]=2;
                txtset(t7,"??"); t7->ds[0]=1; txtadd(t5,t7);
                t7->ds[0]=28; txtadd(t5,t7);//type=portrait look
                t7->ds[0]=myobj->x; txtadd(t5,t7);
                t7->ds[0]=myobj->y; txtadd(t5,t7);
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=tnpc2->port; txtadd(t5,t7);//more=port
                txtset(t7,"????"); t7->df[0]=1.0f; txtadd(t5,t7); //wait
                NET_send(NETplayer,tplayer->net,t5);
                }
                */


              }

            }

            //have object
            //create a description & send a message




            //txtset(t,"?");
            //t->d2[0]=8;
            txtsetchar(t,8); txtaddchar(t,255);


            //t2<-object name
            x2=objname[(myobj->type&1023)*2];
            x3=objname[(myobj->type&1023)*2+1];
            txtNEWLEN(t2,x3);
            memcpy(&t2->d2[0],&objname2[x2],x3);

            txtset(t3,"dead body");
            if (txtsame(t2,t3)){
              for (i3=0;i3<=nresu;i3++){
                if (resu[i3]){
                  if (resu_body[i3]==myobj){
                    tnpc2=(npc*)resu[i3]->more;
                    txtset(t2,"Thou dost see ");
                    txtadd(t2,tnpc2->name);
                    goto gotgrammar;
                  }
                }
              }
            }



            //check for crt pointer
            if (myobj->info&4){
              crt2=(creature*)myobj->more;

              //horse (owned by a player)
              if ((myobj->type&1023)==430){
                if (crt2->flags&128){
                  txtset(t,"?");
                  txtsetchar(t,8); txtaddchar(t,255);
                  txtadd(t,"Thou dost see ");

horsepaperslook_getmoreinfo:

                  if (crt2->respawn_y==0xFFFF){
                    txtadd(t,"a ");
                  }else{
                    if (playerlist[crt2->respawn_y]){ if (playerlist[crt2->respawn_y]->net){ if (myobj2=getprimarypartymemberobj(playerlist[crt2->respawn_y])){
                      txtadd(t,((npc*)myobj2->more)->name);
                      txtadd(t,"'s ");
                      goto houselook_foundownersname;
                    }}}
                    txtadd(t,"a ");
                  }
houselook_foundownersname:
                  //grain state
                  if (((crt2->respawn_x>>4)&3)==3) txtadd(t,"full horse ");
                  if (((crt2->respawn_x>>4)&3)==2) txtadd(t,"well fed horse ");
                  if (((crt2->respawn_x>>4)&3)==1) txtadd(t,"hungry horse ");
                  if (((crt2->respawn_x>>4)&3)==0) txtadd(t,"starving horse ");
                  //horseshoes state
                  if (((crt2->respawn_x>>6)&3)==3) txtadd(t,"with new horseshoes");
                  if (((crt2->respawn_x>>6)&3)==2) txtadd(t,"with good horseshoes");
                  if (((crt2->respawn_x>>6)&3)==1) txtadd(t,"with reasonable horseshoes");
                  if (((crt2->respawn_x>>6)&3)==0) txtadd(t,"with worn horseshoes");
                  if (crt2->respawn_x&15){
                    txtnumint(t2,crt2->respawn_x&15);
                    txtadd(t,", it has won ");
                    txtadd(t,t2);
                    if ((crt2->respawn_x&15)==1) txtadd(t," race!"); else txtadd(t," races!");
                  }else{
                    txtadd(t,".");
                  }
                  goto skiphorsewithowner;
                }
              }







              i3=(obji[sprlnk[myobj->type&1023]].v8>>8)*4;
              txtset(t5,t2);
              txtset(t2,"");
              if (crt2->hp<i3) txtset(t2,"grazed ");
              if (crt2->hp<(i3*4/5)) txtset(t2,"barely wounded ");
              if (crt2->hp<(i3*3/5)) txtset(t2,"lightly wounded ");
              if (crt2->hp<(i3*2/5)) txtset(t2,"heavily wounded ");
              if (crt2->hp<(i3/5)) txtset(t2,"critically hurt "); 
              txtadd(t2,t5);
            }


            if (myobj->type==78){ //staff
              txtset(t2,"");
              if (myobj->more2&65535){
                txtset(t3,spellname[myobj->more2&65535]);
                txtlcase(t3);
                txtadd(t2,t3);
                txtadd(t2," ");
              }
              txtadd(t2,"staff");
              if (myobj->more2&65535){
                txtadd(t2," (x");
                txtnumint(t3,myobj->more2>>16);
                txtadd(t2,t3);
                txtadd(t2,")");
              }
            }


            if (myobj->type==81){
              txtset(t2,"new");
              if (myobj->more2!=0) txtset(t2,"barely used");
              if (myobj->more2>=512) txtset(t2,"worn");
              if (myobj->more2>=1024) txtset(t2,"old");
              txtadd(t2," storm cloak");
            }

            if ((myobj->type&1023)==98){
              i3=myobj->type>>10;
              if (i3==0) txtset(t2,"open chest");
              if (i3==2) txtset(t2,"locked chest");
              if (i3==3) txtset(t2,"magically locked chest");
            }

            i3=myobj->type&1023;
            if ((i3>=297)&&(i3<=300)){
              txtset(t5,t2);
              i3=myobj->type>>12;
              if (i3==0) txtset(t2,"open ");
              if (i3==1) txtset(t2,"");
              if (i3==2) txtset(t2,"locked ");
              if (i3==3) txtset(t2,"magically locked ");
              txtadd(t2,t5);
            }//i3

            //check if it's a spell
            txtset(t3,"spell");
            if (txtsame(t2,t3)){
              //txtset(t3,
              txtset(t2,spellname[myobj->more2]);
              txtlcase(t2);
              txtadd(t2," spell");
            }

            txtset(t3,"fire wand");
            if (txtsame(t2,t3)){
              //txtset(t3,
              txtset(t2,"new");
              if (myobj->more2!=0) txtset(t2,"smoldering");
              if (myobj->more2>=64) txtset(t2,"flaming");
              if (myobj->more2>=192) txtset(t2,"blazing");
              txtadd(t2," fire wand");
            }

            txtset(t3,"lightning wand");
            if (txtsame(t2,t3)){
              //txtset(t3,
              txtset(t2,"new");
              if (myobj->more2!=0) txtset(t2,"glowing");
              if (myobj->more2>=64) txtset(t2,"sparkling");
              if (myobj->more2>=192) txtset(t2,"brilliant");
              txtadd(t2," lightning wand");
            }

            txtset(t3,"invisibility ring");
            if (txtsame(t2,t3)){
              txtset(t2,"new");
              if (myobj->more2!=0) txtset(t2,"glowing");
              if (myobj->more2>=64) txtset(t2,"shining");
              if (myobj->more2>=192) txtset(t2,"brilliant");
              txtadd(t2," invisibility ring");
            }

            txtset(t3,"regeneration ring");
            if (txtsame(t2,t3)){
              txtset(t2,"new");
              if (myobj->more2!=0) txtset(t2,"glowing");
              if (myobj->more2>=32) txtset(t2,"shining");
              if (myobj->more2>=96) txtset(t2,"brilliant");
              txtadd(t2," regeneration ring");
            }

            txtset(t3,"protection ring");
            if (txtsame(t2,t3)){
              txtset(t2,"new");
              if (myobj->more2!=0) txtset(t2,"glowing");
              if (myobj->more2>=1024) txtset(t2,"shining");
              if (myobj->more2>=3072) txtset(t2,"brilliant");
              txtadd(t2," protection ring");
            }

            txtset(t3,"torch");
            if (txtsame(t2,t3)){
              x3=(myobj->info>>9)&15;
              txtset(t2,"new");
              if (x3!=0) txtset(t2,"old");
              if (x3>=4) txtset(t2,"half used");
              if (x3>=12) txtset(t2,"almost new");
              txtadd(t2," torch");
              if (myobj->more2>1) txtadd(t2,"e");
            }

            txtset(t3,"ship deed");
            if (txtsame(t2,t3)){
              if (myobj->info&(1<<9)) txtset(t2,"skiff deed"); else txtset(t2,"ship deed");
            }


            if (myobj->type==OBJ_TMAP){ //treasure map
              x5=(myobj->more2)&1023;
              y2=(myobj->more2>>10)&1023;
              x6=(myobj->more2>>20)&7;
              y3=(myobj->more2>>23)&7;
              x4=(myobj->more2>>26)&7;
              y4=(myobj->more2>>29)&7;
              x5+=(x6-3)*2;
              if(x5<0) {x5=0;}
              if(x5>=1024) {x5=1023;}
              y2+=(y3-3)*2;
              if(y2<0) {y2=0;}
              if(y2>=1024) {y2=1023;}
              txtset(t8,"?");
              t8->d2[0]=49;
              txtset(t9,"????");
              t9->dl[0]=(x5+1024*y2+1024*1024*x4+1024*1024*8*y4);
              txtadd(t8,t9);
              NET_send(NETplayer,tnpc->player->net,t8);
            }

            //apply grammar rules // luteijn Original uses special flags for this!
            //exceptions
            if ((myobj->type&1023)==409) {txtadd(t,"Thou dost see "); goto gotgrammar;} //LB
            if ((myobj->type&1023)==360) {txtadd(t,"Thou dost see "); goto gotgrammar;} //rot worms
            if ((myobj->type&1023)==343) {txtadd(t,"Thou dost see "); goto gotgrammar;} //insects
            if ((myobj->type&1023)==375) {txtadd(t,"Thou dost see "); goto gotgrammar;} //slime
            if ((myobj->type&1023)==135) {txtadd(t,"Thou dost see "); goto gotgrammar;} //horse chops
            //monsters
            if (obji[sprlnk[myobj->type&1023]].v4) goto amonster;
            //nc non-countable
            if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].v6&1){
              txtadd(t,"Thou dost see "); goto gotgrammar;
            }
            if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){ //multiple
              if (myobj->more2>1){
                txtadd(t,"Thou dost see ");
                txtnumint(t3,myobj->more2);
                txtadd(t,t3);
                txtadd(t," ");
                //of-> +s of
                txtset(t3," of ");
                if (x2=txtsearch(t2,t3)){
                  txtset(t3,t2); txtleft(t3,x2-1);
                  if ((myobj->type&1023)==128) txtset(t3,"loave"); //loaf->loaves
                  txtset(t4,t2); txtright(t4,t2->l-x2+1);
                  txtadd(t3,"s"); txtadd(t3,t4); txtset(t2,t3);
                  goto gotgrammar;
                }
                //default: +s
                txtadd(t2,"s");
                goto gotgrammar;
              }//>1
            }//multiple
amonster:
            //the + CAPITAL LETTER
            x2=t2->d2[0];
            if ((x2>=65)&&(x2<=90)) {txtadd(t,"Thou dost see the "); goto gotgrammar;}
            // vowel?
            x3=0;
            if (x2==97) x3=1; //a
            if (x2==101) x3=1; //e
            if (x2==105) x3=1; //i
            if (x2==111) x3=1; //o
            if (x2==117) x3=1; //u
            if (x3) {txtadd(t,"Thou dost see an "); goto gotgrammar;}
            txtadd(t,"Thou dost see a ");



gotgrammar:
            txtadd(t,t2);

            x2=ENHANCEget_attack(myobj); if (!x2) x2=ENHANCEget_defense(myobj);
            if (x2){
              txtadd(t," +");
              txtnumint(t2,x2);
              txtadd(t,t2);
            }

            if ((myobj->type&1023)==OBJ_POCKET_WATCH && myobj->more2!=0) { //different construction phases in the quest
              if (myobj->more2==1) {
                txtadd(t," mold");
              }
              if (myobj->more2==2) {
                txtadd(t," frame");
              }
              if (myobj->more2==3) {
                txtadd(t," that is missing the glass crystal");
              }
              if (myobj->more2==4) {
                txtadd(t," that is almost done");
              }
            }

            f=WTfind(myobj);
            f9=f;
            if (f){
              txtadd(t,", weighing ");
              x2=f/100;
              txtnumint(t2,x2);
              txtadd(t,t2);
              if ((float)x2!=(f/100.0f)){
                txtadd(t,".");
                txtnumint(t2,(int)f%100);
                if (t2->l==1) txtadd(t,"0");
                if (t2->l==2){
                  if (t2->d2[1]==48) txtleft(t2,1);
                }
                txtadd(t,t2);
              }
              if (f==1.0f) txtadd(t," stone"); else txtadd(t," stones");
            }

            if (myobj->type==448){//horse papers
              txtadd(t,", for ");
              myobj=(object*)myobj->more; crt2=(creature*)myobj->more;
              goto horsepaperslook_getmoreinfo;
            }


            txtadd(t,".");



            if ((obji[sprlnk[myobj->type&1023]].v1)&&(obji[sprlnk[myobj->type&1023]].v2)){
              txtadd(t," It can do ");
              x2=obji[sprlnk[myobj->type&1023]].v1+ENHANCEget_attack(myobj);
              txtnumint(t2,x2);
              txtadd(t,t2);
              txtadd(t," and absorb ");
              x2=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              txtnumint(t2,x2);
              txtadd(t,t2);
              if (x2==1) txtadd(t," point of damage"); else txtadd(t," points of damage");
              txtadd(t,".");
              goto complex_done;
            }

            if (obji[sprlnk[myobj->type&1023]].v1){
              txtadd(t," It can do ");
              x2=obji[sprlnk[myobj->type&1023]].v1+ENHANCEget_attack(myobj);
              txtnumint(t2,x2);
              txtadd(t,t2);
              if (x2==1) txtadd(t," point of damage"); else txtadd(t," points of damage");
              txtadd(t,".");
            }

            if (obji[sprlnk[myobj->type&1023]].v2+ ENHANCEget_defense(myobj)){
              if ((myobj->type&1023)==27) txtadd(t," They can absorb "); else txtadd(t," It can absorb "); //boots exception
              x2=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              txtnumint(t2,x2);
              txtadd(t,t2);
              if (x2==1) txtadd(t," point of damage"); else txtadd(t," points of damage");txtadd(t,".");
            }
complex_done:



            //if ((ol[i].type&1)==0) myobj->info|=128; //128 1=stealing, 0=ok to take
            if (myobj->info&128){
              if (f9){ //has weight
                txtadd(t," Taking this is stealing!");
              }
            }

            if (myobj->info&256){
              if (f9){ //has weight
                txtadd(t," This is a quest item!");
              }
            }


            if (myobj->type==OBJ_SUNDIAL){ //sundial
              if ((btime>5)&&(btime<20)) goto thetimeis;
            }
            if ((myobj->type&1023)==OBJ_CLOCK || ((myobj->type&1023)==OBJ_POCKET_WATCH && myobj->more2==0)){ //clock
thetimeis:
              txtadd(t," The time is ");
              x=btime;
              x2=(btime-(float)x)*60.0f;
              if (x>12) x-=12;
              if (x==0) x=12;
              txtnumint(t2,x);
              txtadd(t,t2);
              txtadd(t,":");
              txtnumint(t2,x2);
              if (t2->l==1) txtadd(t,"0");
              txtadd(t,t2);
              if (btime>=12.0f) txtadd(t," P.M."); else txtadd(t," A.M.");
            }

            //txtadd(t,".");

skiphorsewithowner:
skip_playername:
            NET_send(NETplayer,tplayer->net,t);

            //sign //scroll //picture //book //gsign //codex //circles //tombstone //cross
            if (((myobj->type&1023)==332)||((myobj->type&1023)==152)||((myobj->type&1023)==143)||((myobj->type&1023)==151)||((myobj->type&1023)==333)||((myobj->type&1023)==59)||((myobj->type&1023)==61)||((myobj->type&1023)==254)||((myobj->type&1023)==255)){
              if (myobj->more2){

                if (myobj->more2&65536){//house sign!
                  x2=myobj->more2&65535;
                  if (housesav[x2].flags&1){//occupied
                    txtset(t,"?"); txtset(t3,"");
                    for (x3=0;x3<=31;x3++){
                      if (housesav[x2].charactername[x3]){t->d2[0]=housesav[x2].charactername[x3]; txtadd(t3,t);}
                    }
                    txtadd(t3,"'s House");

                    if (x2==47&&(myobj->x)!=79) txtadd(t3,"\\Beware of the drakes!");
                    goto housesign;
                  }

                  txtset(t3,"For Sale\\No.");
                  txtnumint(t4,x2);
                  txtadd(t3,t4);
                  if (x2==47&&(myobj->x)!=79) txtadd(t3,"\\Beware of the drakes!");
                  goto housesign;
                }



                x2=tsign[(myobj->more2-1)*2];
                x3=tsign[(myobj->more2-1)*2+1];
                txtNEWLEN(t,x3);
                memcpy(&t->d2[0],&tsign2[x2],x3);
                txtset(t3,t);
                //1garg 2normal 3runes
                txtset(t,"?"); t->d2[0]=2; if (x=txtsearch(t3,t)){
                  if (tnpc->player->GNPCflags[9]){
                    txtset(t,t3); txtleft(t3,x-1); txtright(t,t->l-x); txtadd(t3,t);
                  }else{
                    txtleft(t3,x-2);
                  }
                }//2
housesign:

                //edit t3?
                //MessageBox(NULL,t3->d,"Ultima 6 Online",MB_OK);

                //create PRIVATE sfx message
                txtset(t,"?"); t->d2[0]=2;
                txtset(t2,"??"); t2->ds[0]=1; txtadd(t,t2);
                t2->ds[0]=3; txtadd(t,t2);
                t2->ds[0]=myobj->x-1;
                if (myobj->x==0) t2->ds[0]=tnpc->player->party[tnpc->player->selected_partymember]->x-1;
                txtadd(t,t2);
                t2->ds[0]=myobj->y-1;
                if (myobj->y==0) t2->ds[0]=tnpc->player->party[tnpc->player->selected_partymember]->y-1;
                txtadd(t,t2);
                t2->ds[0]=t3->l; txtadd(t,t2);
                t2->ds[0]=0; txtadd(t,t2); //unused
                t2->ds[0]=187; txtadd(t,t2); //more=port blank!
                txtset(t2,"????"); t2->df[0]=2; txtadd(t,t2); //wait
                txtadd(t,t3);
                NET_send(NETplayer,tplayer->net,t);


              }
            }


            /*
            if (t->d2[0]==8){ //set mouse object
            exit2(8);
            goto CLIENT_donemess;
            }
            */


            goto skip_pickup;
looknullobject:; //2003: skip null objects
            //report info about the basetile being looked at
            x2=bt[y][x]&1023;
            if (x2==0) goto skip_pickup;//nothing!
            if (x2==1) txtset(t,"grass");
            if (x2>=2) txtset(t,"swamp");
            if (x2>=5) txtset(t,"a shrub");
            if (x2>=6) txtset(t,"a bush");
            if (x2>=8) txtset(t,"water");
            if (x2>=16) txtset(t,"shore");
            if (x2>=48) txtset(t,"a tree");
            if (x2>=52) txtset(t,"grass");
            if (x2>=64) txtset(t,"a tree");
            if (x2>=72) txtset(t,"a dead tree");
            if (x2>=73) txtset(t,"scrub");
            if (x2>=78) txtset(t,"a shrub");
            if (x2>=79) txtset(t,"earth");
            if (x2>=96) txtset(t,"an embankment");
            if (x2>=108) txtset(t,"earth");
            if (x2>=112) txtset(t,"a mountain");
            if (x2>=125) txtset(t,"snowcaps");
            if (x2>=140) txtset(t,"a wall");
            if (x2>=188) txtset(t,"a field");
            if (x2>=189) txtset(t,"crops");
            if (x2>=192) txtset(t,"an arrow slit");
            if (x2>=196) txtset(t,"a window");
            if (x2>=198) txtset(t,"a crenellation");
            if (x2>=200) txtset(t,"a window");
            if (x2==206) goto skip_pickup;//nothing!
            if (x2>=207) txtset(t,"a wall");
            if (x2>=208) txtset(t,"the floor");
            if (x2>=217) txtset(t,"wet earth");
            if (x2>=218) txtset(t,"a puddle");
            if (x2>=219) txtset(t,"a pool");
            if (x2>=220) txtset(t,"the floor");
            if (x2>=221) txtset(t,"lava");
            if (x2>=224) txtset(t,"a pyramid");
            if (x2>=240) txtset(t,"an earthen wall");
            if (x2>=252) txtset(t,"the ethereal void");
            if (x2>=256) goto skip_pickup;//nothing!
            txtset(t2,"Thou dost see "); txtadd(t2,t); txtadd(t2,".");
            txtsetchar(t,8); txtaddchar(t,255); txtadd(t,t2);
            NET_send(NETplayer,tplayer->net,t);
            goto skip_pickup;
          } //action==2 look

          if (tplayer->action==3){ //attack

            if (tplayer->craft){
              if ((tplayer->party[0]->type&1023)==412){ //ship
                goto shipattack;
              }
            }

            if (tplayer->craft==NULL){
shipattack:
              x=tpx+tplayer->mx/32;
              y=tpy+tplayer->my/32;
              myobj=OBJfindlast(x,y);
              if (myobj!=NULL){

                //is it a member of our party? if so break off combat
                //for (x2=0;x2<=7;x2++){
                //if (tplayer->party[x2]==myobj){

                if (myobj->info&2){ //if NPC break off combat
                  for (x2=0;x2<=7;x2++){
                    if (tplayer->party[x2]){
                      VLNKsremove(tplayer->party[x2]);
                      tnpc2=(npc*)tplayer->party[x2]->more;
                      tnpc2->target=NULL;
                      tnpc2->order=3; if (x2==tplayer->selected_partymember) tnpc2->order=0;
                    }//->party
                  }//x2
                  goto skip_pickup;
                }

                //}
                //}

                if (myobj->info&8) myobj=(object*)myobj->more; //redirect
                if (myobj->info&4){ //<-crt

                  //myobj2=tplayer->party[0];
                  //tnpc=(npc*)myobj2->more;
                  //tnpc->order=3;
                  //tnpc->target=myobj;
                  //set other party members to attack as well
                  for (x2=0;x2<=7;x2++){
                    if (tplayer->party[x2]){
                      myobj2=tplayer->party[x2];
                      tnpc=(npc*)myobj2->more;
                      tnpc->order=3;
                      tnpc->target=myobj;
                      VLNKsremove(myobj2); //remove prev vlnk
                      VLNKnew(myobj2,myobj,(unsigned long)&tnpc->target); //add vlnk
                      if (tplayer->craft) goto shipattack2; //exit loop after processing party[0]
                    }
                  }
                } //<-crt
shipattack2:

                //attack item (locked chest, door, spider web, other breakable items) (lets take spider web as an example)

                if ((myobj->type&1023)==OBJ_CHEST) goto targetitem;
                if (myobj->type==OBJ_WEB){ //web
targetitem:
                  myobj2=tplayer->party[tplayer->selected_partymember];
                  tnpc=(npc*)myobj2->more;
                  tnpc->order=3;
                  tnpc->target=myobj;
                  VLNKsremove(myobj2); //remove prev vlnk
                  VLNKnew(myobj2,myobj,(unsigned long)&tnpc->target); //add vlnk
                }

              } //!=NULL

              //set creature as a target for attack oh no! it's gonna die

              //unsigned char order;
              //0=none
              //1=follow party leader
              //2=schedule
              //3=attack target

            }//craft

            goto skip_pickup;
          } //action==3 attack

        } //primary surface click

        if (tplayer->mf<=7){ if (tplayer->party[tplayer->mf]){

          //2003 Xtnpc=(npc*)activeplayer->party[tplayer->mf]->more;
          tnpc=(npc*)tplayer->party[tplayer->mf]->more;


          if (tplayer->key&KEYmbclick){
            //tnpc->upflags|=(32+64); //inv+eqp

            //tnpc->update=1;

            //clicked on opened "bag" icon ->close bag
            if ((tplayer->mx>=(90))&&(tplayer->my>=(128+3))&&(tplayer->mx<((90)+32))&&(tplayer->my<((128+3)+32))){
              if (tnpc->baseitem!=tnpc->items){ //backwards traverse
                myobj2=tnpc->baseitem;
                myobj=(object*)tnpc->baseitem->prev;
getprev10:

                //TROLL GPF NEXT LINE (BACKSCANNING THROUGH INVENTORY?)
                if ((myobj->more!=myobj2)||((obji[sprlnk[myobj->type&1023]].flags&1024)==0)) {myobj2=myobj; myobj=(object*)myobj->prev; goto getprev10;}

                tnpc->baseitem=myobj;
                tnpc->baseitem_offset=0;
                tnpc->upflags|=32;
                goto skip_pickup;
              }
            }//x,y

            if (tplayer->action==0){

              //scroll inventory down
              if ((tplayer->mx>=(111-3))&&(tplayer->my>=(228-3))){
                if ((tplayer->mx<=(121+3))&&(tplayer->my<=(243+3))){
                  tnpc->upflags|=32;
                  //is scroll down available?
                  myobj=(object*)tnpc->baseitem;
                  myobj=(object*)myobj->more;
                  x=61; y=61; x3=tnpc->baseitem_offset;
dniz:
                  if (myobj!=NULL){
                    if (x3>0) {x3--;goto skipbio1z;}
                    x=x+16;
                    if (x>109) {x=61;y=y+16;}
                    if (y>109){
                      if (myobj->next!=NULL) 
                      {
                        tnpc->baseitem_offset+=4;
                      }
                      goto diskipz;
                    }
                    if (myobj->next!=NULL){
skipbio1z:
                      myobj=(object*)myobj->next;
                      goto dniz;
                    }
                  }
diskipz:;
                  goto skip_pickup;
                }}

              //scroll inventory up
              if ((tplayer->mx>=(111-3))&&(tplayer->my>=(197-3))){ if ((tplayer->mx<=(121+3))&&(tplayer->my<=(212+3))){
                if (tnpc->baseitem_offset>=4) {tnpc->baseitem_offset-=4;}else{tnpc->baseitem_offset=0;}
                tnpc->upflags|=32;
                goto skip_pickup;
              }}




              if ((tplayer->mx>=122)&&(tplayer->my>=122)){
                x=(tplayer->mx-122)/32;
                y=(tplayer->my-122)/32;
                i2=x+(y*4);
                myobj=tnpc->baseitem;
                myobj=(object*)myobj->more;
                i3=tnpc->baseitem_offset;
fo2:
                if (i3>0){i3--;myobj=(object*)myobj->next;goto fo2;}
                if (myobj!=NULL){
                  if (i2>0){
                    if (myobj->next!=NULL){
                      myobj=(object*)myobj->next;
                      i2--;
                      goto fo2;
                    }else{myobj=NULL;}
                  }

                  if (myobj){
                    if (tplayer->key&KEYquan){
                      if (tplayer->quan){
                        if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){
                          if (tplayer->quan<myobj->more2){
                            myobj2=OBJnew();
                            myobj2->type=myobj->type;
                            myobj2->info=myobj->info;
                            myobj2->more2=tplayer->quan;
                            myobj->more2-=tplayer->quan;
                            tplayer->mobj=myobj2;
                            goto nopickup2;
                          }//<
                        }//multiple
                      }//->quan
                    }//KEYquan
                  }

                  tplayer->mobj=myobj;
                  if (tplayer->mobj!=NULL) OBJremove(tplayer->mobj);

nopickup2:
                  tnpc->upflags|=32;
                }
              } //inventory block
              else
              { //click outside inventory

                //2003: new position pickup (no mouse object)
                x2=tplayer->mx; y2=tplayer->my;
                x3=helmx; y3=helmy;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  tplayer->mobj=tnpc->helm;
                  tnpc->helm=NULL;
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }
                x3=wep_rightx; y3=wep_righty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  tplayer->mobj=tnpc->wep_right;
                  tnpc->wep_right=NULL;
                  if (tplayer->mobj){
                    //if ((tplayer->mobj->type&1023)==258) {tnpc->flags&=(255-2);} //invisibility ring
                    if ((tplayer->mobj->type&1023)==90) {tplayer->mobj->type=90;}
                    if (tplayer->mobj->type==57){ //unready spellbook
                      tnpc->upflags|=128;
                    }


                  }
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }
                x3=wep_leftx; y3=wep_lefty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  tplayer->mobj=tnpc->wep_left;
                  tnpc->wep_left=NULL;
                  if (tplayer->mobj){
                    //if ((tplayer->mobj->type&1023)==258) {tnpc->flags&=(255-2);} //invisibility ring
                    if ((tplayer->mobj->type&1023)==90) {tplayer->mobj->type=90;}
                    if (tplayer->mobj->type==57){ //unready spellbook
                      tnpc->upflags|=128;
                    }

                  }
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }
                x3=armourx; y3=armoury;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  tplayer->mobj=tnpc->armour;
                  tnpc->armour=NULL;
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }
                x3=bootsx; y3=bootsy;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  tplayer->mobj=tnpc->boots;
                  tnpc->boots=NULL;
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }
                x3=neckx; y3=necky;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  if (tnpc->neck){
                    if (tnpc->neck->type==76){
                      txtset(t,"?"); t->d2[0]=8; txtadd(t,"Magical energy prevents you from removing the amulet!"); NET_send(NETplayer,tplayer->net,t);
                      goto mobj_gotitem;
                    }
                  }
                  tplayer->mobj=tnpc->neck;
                  tnpc->neck=NULL;
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }
                x3=ring_leftx; y3=ring_lefty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  tplayer->mobj=tnpc->ring_left;
                  tnpc->ring_left=NULL;
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }
                x3=ring_rightx; y3=ring_righty;
                if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                  tplayer->mobj=tnpc->ring_right;
                  tnpc->ring_right=NULL;
                  tnpc->upflags|=64;
                  goto mobj_gotitem;
                }

mobj_gotitem:;
              } //else outside inventory

            } //action==0





            if (tplayer->action==2){ //look, opens inventory container items
              tnpc->upflags|=32;
              if ((tplayer->mx>=122)&&(tplayer->my>=122)){
                x=(tplayer->mx-122)/32;
                y=(tplayer->my-122)/32;
                i2=x+(y*4);
                myobj=tnpc->baseitem;
                myobj=(object*)myobj->more;
                i3=tnpc->baseitem_offset;
fo2b:
                if (i3>0){i3--;myobj=(object*)myobj->next;goto fo2b;}
                if (myobj!=NULL){
                  if (i2>0){
                    if (myobj->next!=NULL){
                      myobj=(object*)myobj->next;
                      i2--;
                      goto fo2b;
                    }else{myobj=NULL;}
                  }
                }
                if (myobj){

                  if ((myobj->type==(98+1024*2))||(myobj->type==(98+1024*3))) goto inventory_look; //magically/locked chests
                  for (i3=0;i3<=nresu;i3++){
                    if (resu[i3]){
                      if (resu_body[i3]==myobj){
                        goto inventory_look; //block looking inside resurrectable bodies
                      }}}
                  if (obji[sprlnk[myobj->type&1023]].flags&1024){ //2003: check container flag
                    tnpc->baseitem=myobj;
                    tnpc->baseitem_offset=0;
                    goto inventory_look;
                    //goto skip_pickup;
                  }
                  goto inventory_look;
                }//myobj
                goto skip_pickup;
              } //if x&y

              //what about equipped items?
              x2=tplayer->mx; y2=tplayer->my;
              x3=helmx; y3=helmy;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->helm) goto inventory_look;
              }
              x3=wep_rightx; y3=wep_righty;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->wep_right) goto inventory_look;
              }
              x3=wep_leftx; y3=wep_lefty;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->wep_left) goto inventory_look;
              }
              x3=armourx; y3=armoury;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->armour) goto inventory_look;
              }
              x3=bootsx; y3=bootsy;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->boots) goto inventory_look;
              }
              x3=neckx; y3=necky;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->neck) goto inventory_look;
              }
              x3=ring_leftx; y3=ring_lefty;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->ring_left) goto inventory_look;
              }
              x3=ring_rightx; y3=ring_righty;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->ring_right) goto inventory_look;
              }

              goto skip_pickup;
            } //action==2


            if (tplayer->action==1){ //use inventory item
              tnpc->upflags|=32;
              if ((tplayer->mx>=122)&&(tplayer->my>=122)){
                x=(tplayer->mx-122)/32;
                y=(tplayer->my-122)/32;
                i2=x+(y*4);
                myobj=tnpc->baseitem;
                myobj=(object*)myobj->more;
                i3=tnpc->baseitem_offset;
fo2d:
                if (i3>0){i3--;myobj=(object*)myobj->next;goto fo2d;}
                if (myobj!=NULL){
                  if (i2>0){
                    if (myobj->next!=NULL){
                      myobj=(object*)myobj->next;
                      i2--;
                      goto fo2d;
                    }else{myobj=NULL;}
                  }
                }
                if (myobj!=NULL){
                  itemused=2; i=myobj->type&1023; i2=myobj->type>>10; goto useinventoryitem;
                }
              }//if x&y

              //use staff //staff use
              x2=tplayer->mx; y2=tplayer->my;
              x3=wep_rightx; y3=wep_righty;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->wep_right){
                  if (myobj->type==78) goto staff_use0;
                }
              }
              x3=wep_leftx; y3=wep_lefty;
              if ((x2>=x3)&&(y2>=y3)&&(x2<(x3+32))&&(y2<(y3+32))){
                if (myobj=tnpc->wep_left){
                  if (myobj->type==78) goto staff_use0;
                }
              }
              myobj=NULL;
staff_use0:
              if (myobj){
                if (myobj->more2>>16){

                  txtset(t,"?"); t->d2[0]=23; //staff cast
                  txtset(t2,"??"); t2->ds[0]=myobj->more2&65535; txtadd(t,t2); //spell type
                  NET_send(NETplayer,tplayer->net,t);
                  tplayer->staffcast_npc=tplayer->party[tplayer->mf]; //object* of npc using the staff
                  tplayer->staffcast_staff=myobj; //object* of staff




                }//num. spells !=0
              }//myobj

              goto skip_pickup;
            }//use inventory item


          } //mbclick
        } //valid mf value
        } //mf<=7
skip_pickup:;


      }//check if player is alive and selected party member is valid
    }//current player
  }//player INPUT update (HOST controlled)

  //2003: send NPC-frame update message to client
  for (tpl=0;tpl<=playerlist_last;tpl++){


    if (playerlist[tpl]){ //current player
      if (playerlist[tpl]->net!=NULL){ //net connection exists


        //2003: mobj update (this should be incorporated into the NPC update message if possible)
        if (playerlist[tpl]->mobj!=playerlist[tpl]->mobj_last){


          txtset(t,"?");
          txtset(t2,"??");
          t->d2[0]=4;
          if (playerlist[tpl]->mobj!=NULL){
            t2->ds[0]=playerlist[tpl]->mobj->type;
          }else{t2->ds[0]=0xFFFF;}
          txtadd(t,t2);
          NET_send(NETplayer,playerlist[tpl]->net,t);
        }
        playerlist[tpl]->mobj_last=playerlist[tpl]->mobj;




        for (i=0;i<=7;i++){ if ((playerlist[tpl]->party[i]!=NULL)&&((playerlist[tpl]->party[0]!=NULL))){
          tnpc=(npc*)playerlist[tpl]->party[i]->more; //shortcut
          if (tnpc->upflags){ //upflags (any)
            //1		name, portriat, str, dex, int, lev, hp_max, mp_max (all)
            //2		hp
            //4     mp
            //8     xp
            //16    weight (automatically set if inventory/equipped items change)
            //32    inventory, bag icon
            //64    equipped items

            //tnpc->upflags=1;
            if (tnpc->upflags&1) tnpc->upflags|=(2+4+8+16+32+64);
            if (tnpc->upflags&32) tnpc->upflags|=16;
            if (tnpc->upflags&64) tnpc->upflags|=16;

            if (tnpc->upflags&32){ //reagents changed? spellbook active?
              if (tnpc->wep_right){ if (tnpc->wep_right->type==57){
                tnpc->upflags|=128;
              }}
              if (tnpc->wep_left){ if (tnpc->wep_left->type==57){
                tnpc->upflags|=128;
              }}
            }
            //WHAT ABOUT WHEN WE TAKE THE SPELLBOOK AWAY!!!!!



            //if (tplayer->mobj->type==57){ //ready spellbook
            //tnpc->upflags|=128;
            //}
            //tnpc->wep_right=tplayer->mobj;



            if (tnpc->upflags&16){ //recalculate the weight the player is carrying
              x=0;
              x+=WTfind((object*)tnpc->helm);
              x+=WTfind((object*)tnpc->neck);
              x+=WTfind((object*)tnpc->wep_left);
              x+=WTfind((object*)tnpc->wep_right);
              x+=WTfind((object*)tnpc->armour);
              x+=WTfind((object*)tnpc->ring_left);
              x+=WTfind((object*)tnpc->ring_right);
              x+=WTfind((object*)tnpc->boots);
              tnpc->wt2=x;
              x+=(WTfind((object*)tnpc->items)-20);
              tnpc->wt=x;
            }


            //need an internal loop for each NPC
            txtset(t,"????");
            txtset(t2,"??");
            txtset(t3,"?");
            txtset(t4,"????");
            txtset(t6,"?");
            t->d2[0]=3; //update NPC frame message
            t->d2[1]=i; //frame index 0-7
            t->ds[1]=tnpc->upflags;


            if (tnpc->upflags&32){

              //validate inventory position (scroll up as necessary)
revalinv:
              x=-1; myobj=(object*)tnpc->baseitem; myobj=(object*)myobj->more;
invval: if (myobj){x++; myobj=(object*)myobj->next; goto invval;}
              if (x==-1) x=0;
              if (x<tnpc->baseitem_offset){
                if (tnpc->baseitem_offset>=4) {tnpc->baseitem_offset-=4;}else{tnpc->baseitem_offset=0;}
                goto revalinv;
              }

              t3->d2[0]=0; txtadd(t,t3); //x2 <--
              //x2(64)   +display up arrow (only if flag32 set)
              //x2(128)   +display down arrow (only if flag32 set)
              //x2(?)  +bag icon (only if flag32 set)

              txtsetchar(t5,46);//tnpc non-sellable objects message (currently seperate)
              txtaddchar(t5,i);
              txtaddshort(t5,0);
              y3=0;

              //11111 111
              //tnpc=tnpc;
              //player inventory
              myobj=(object*)tnpc->baseitem;
              myobj=(object*)myobj->more;
              x2=0; //number of items to display
              x=61; y=61; x3=tnpc->baseitem_offset;
              if (x3) x2+=64; //up arrow
dnib:
              if (myobj!=NULL){
                if (x3>0) {x3--;goto skipbio1b;}


                t2->ds[0]=myobj->type;

                if (myobj->info&32768){
                  t5->ds[1]|=(1<<y3);
                }
                y3++;

                txtadd(t,t2);
                //quantity,keys and item + can't exist in same object so changed to else if this should fix oild flask bug also
                i6=0;
                if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){ //items that have quantity
                  t2->ds[0]=myobj->more2;
                  txtadd(t,t2);
                }
                else if ((myobj->type&1023)==64){ //key
                  t2->ds[0]=myobj->more2;
                  txtadd(t,t2);
                }
                else if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].v1 || obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].v2){ //v1 (damage), send + info to client
                  i6=ENHANCEget_attack(myobj);
                  if (i6==0) {
                    i6=ENHANCEget_defense(myobj);
                  }
                  t6->d2[0]=i6;
                  txtadd(t,t6);
                }
                else if ((myobj->type&1023)==448) { //horse papers, send + info to client 
                  myobj2=(object*)myobj->more; crt2=(creature*)myobj2->more;
                  if (crt2->respawn_x&15){
                    i6=crt2->respawn_x&15;
                  }
                  t6->d2[0]=i6;
                  txtadd(t,t6);
                }

                x2++;
                x=x+16;
                if (x>109) {x=61;y=y+16;}
                if (y>109) {
                  if (myobj->next!=NULL) x2+=128; 
                  goto diskipb;}
                if (myobj->next!=NULL){
skipbio1b:
                  myobj=(object*)myobj->next;
                  goto dnib;
                }
              }
diskipb:;
              //return (x2&0xF);
              if (tnpc->baseitem!=tnpc->items){
                t2->ds[0]=tnpc->baseitem->type;
                txtadd(t,t2);
                x2+=32; //bag item
              }
              t->d2[4]=x2; //<--x2

              NET_send(NETplayer,playerlist[tpl]->net,t5); //send not4sale info


            }//tnpc->upflags&32

            if (tnpc->upflags&64){ //eqp items update
              if (tnpc->helm!=NULL) t2->ds[0]=tnpc->helm->type; else t2->ds[0]=0;
              txtadd(t,t2);
              if (tnpc->wep_right!=NULL) t2->ds[0]=tnpc->wep_right->type; else t2->ds[0]=0;
              txtadd(t,t2);
              if (tnpc->wep_left!=NULL) t2->ds[0]=tnpc->wep_left->type; else t2->ds[0]=0;
              txtadd(t,t2);
              if (tnpc->armour!=NULL) t2->ds[0]=tnpc->armour->type; else t2->ds[0]=0;
              txtadd(t,t2);
              if (tnpc->boots!=NULL) t2->ds[0]=tnpc->boots->type; else t2->ds[0]=0;
              txtadd(t,t2);
              if (tnpc->neck!=NULL) t2->ds[0]=tnpc->neck->type; else t2->ds[0]=0;
              txtadd(t,t2);
              if (tnpc->ring_right!=NULL) t2->ds[0]=tnpc->ring_right->type; else t2->ds[0]=0;
              txtadd(t,t2);
              if (tnpc->ring_left!=NULL) t2->ds[0]=tnpc->ring_left->type; else t2->ds[0]=0;
              txtadd(t,t2);
              t2->ds[0]=tnpc->wt2;
              txtadd(t,t2);
              //as equipment has changed recalculate arm value!
              static unsigned short npc_arm;
              npc_arm=0;
              myobj=tnpc->helm; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              myobj=tnpc->wep_right; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              myobj=tnpc->wep_left; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              myobj=tnpc->armour; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              myobj=tnpc->boots; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              myobj=tnpc->neck; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              myobj=tnpc->ring_right; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              myobj=tnpc->ring_left; if (myobj!=NULL) npc_arm+=obji[sprlnk[myobj->type&1023]].v2+ENHANCEget_defense(myobj);
              tnpc->arm=npc_arm;
              if (tnpc->protection) tnpc->arm+=10;
              if (tnpc->player){ if (tnpc->player->party[0]){ if ((npc*)tnpc->player->party[0]->more==tnpc){ if (tnpc->exp<1600){ if (tnpc->lev<=2){
                tnpc->arm+=(20-tnpc->exp/80);
              }}}}}
            } //tnpc->upflags&64

            if (tnpc->upflags&1){
              //1		name, portriat, str, dex, int, lev, hp_max, mp_max (does not change often)
              t3->d2[0]=tnpc->name->l; txtadd(t,t3);
              txtadd(t,tnpc->name);
              t2->ds[0]=tnpc->port; txtadd(t,t2);
              t2->ds[0]=tnpc->s; txtadd(t,t2);
              t2->ds[0]=tnpc->d; txtadd(t,t2);
              t2->ds[0]=tnpc->i; txtadd(t,t2);
              t2->ds[0]=tnpc->lev; txtadd(t,t2);
              t2->ds[0]=tnpc->hp_max; txtadd(t,t2);
              t2->ds[0]=tnpc->mp_max; txtadd(t,t2);
              t2->ds[0]=tnpc->wt_max; txtadd(t,t2);
              t2->ds[0]=tnpc->wt2_max; txtadd(t,t2);
            } //tnpc->upflags&1

            if (tnpc->upflags&16){ //wt
              t2->ds[0]=tnpc->wt; txtadd(t,t2);
            } //tnpc->upflags&16

            if (tnpc->upflags&2){ //hp
              if (tnpc->hp<0) tnpc->hp=0;
              if (tnpc->flags&1) t2->ds[0]=(unsigned long)tnpc->hp|32768; else t2->ds[0]=tnpc->hp;
              txtadd(t,t2);
            } //tnpc->upflags&2

            if (tnpc->upflags&4){ //mp
              t2->ds[0]=tnpc->mp;
              txtadd(t,t2);
            } //tnpc->upflags&4

            if (tnpc->upflags&8){ //xp
              t4->dl[0]=tnpc->exp;
              txtadd(t,t4);
            } //tnpc->upflags&8


            if (tnpc->upflags&128){
              ZeroMemory(&tspell,sizeof(tspell));
              //enumerate spells
              if (myobj=(object*)tnpc->wep_left){ if (myobj->type==57){
                myobj=(object*)myobj->more;
nextspell:
                if (myobj){
                  tspell[myobj->more2]=1;
                  if (myobj->next) {myobj=(object*)myobj->next; goto nextspell;}
                }
              }}
              if (myobj=(object*)tnpc->wep_right){ if (myobj->type==57){
                myobj=(object*)myobj->more;
nextspell2:
                if (myobj){

                  //NEW GPF THIS LINE!
                  tspell[myobj->more2]=1;

                  if (myobj->next) {myobj=(object*)myobj->next; goto nextspell2;}
                }
              }}

			  // c222 lastused spellbook always available (even after unreadying)
			  if (easymodehostn1) {
				  if (myobj = (object*)playerspellbook) {
					  myobj = (object*)myobj->more;
					  while (myobj) {
						  tspell[myobj->more2] = 1;
						  myobj = (object*)myobj->next;
						  //if (myobj->next) {myobj=(object*)myobj->next; goto nextspell2;}
					  }
				  }
			  }


              //enumerate reagents
              ZeroMemory(&treagent,sizeof(treagent));
              x=OBJlist((object*)tnpc->items->more); //x5=last object number???
              OBJlist_last=NULL;
              for(x2=0;x2<x;x2++){
                x3=OBJlist_list[x2]->type;
                if ((x3>=65)&&(x3<=72)){
                  treagent[x3-65]+=OBJlist_list[x2]->more2;
                }
              }

              //add spell number
              //spellreagent[..]
              x4=0; //number of spells in spellbook(s)
              for (x=0;x<=255;x++){
                if (tspell[x]){
                  x4++;

                  x3=65536;
                  for (x2=0;x2<=7;x2++){
                    if (spellreagent[x]&(1<<x2)){
                      if (treagent[x2]<x3) x3=treagent[x2];
                    }//reagent
                  }//x2

                  if (x3<65536){ //add spell number
                    tspell[x]+=x3;
                  }

                }//tspell
              }//x spell

              //add message
              t3->d2[0]=x4; txtadd(t,t3); //numbers of spells
              for (x=0;x<=255;x++){
                if (tspell[x]){

                  //txtnumint(t5,x);
                  //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


                  t3->d2[0]=x; txtadd(t,t3);
                  t2->ds[0]=tspell[x]; txtadd(t,t2);
                }
              }

            }//128

            NET_send(NETplayer,playerlist[tpl]->net,t); //send update message
            tnpc->upflags=0;

          } //upflags
        }} //party member loop
      } //connection exists


    }//current player
  }//tpl





  if (ouln!=-1){ for (i=0;i<=ouln;i++){ if (oul[i]){
    if (oul[i]->info&32768) oul[i]->info-=32768;
  }}}

#ifndef _DEBUG
  /* this was originally at client loop so I moved it here */
  if (ett>=86400){ /*quit every 24h if not debugging*/
    for (i=1;i<=playerlist_last;i++){
      if (playerlist[i]){ //current player
        if (playerlist[i]->net!=INVALID_NET){ //net connection available
          goto autoexit_deny;
        }}}
    u6orevive=255; seek(u6orevive_fh,0); put(u6orevive_fh,&u6orevive,1);
    exitrequest=TRUE; exitrequest_noconfirm=TRUE;
  }//ett
#endif
autoexit_deny:

  revive_infiniteloopexit_i++;

} //NEThost!=NULL
