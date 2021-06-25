
void getspr(object* obj){ //loads bt32 with a spr (uses keyframe) FIXME: candidate for optimising (work in progress)
  gs_i=sprlnk[obj->type&1023]+(obj->type>>10)-1;

  //check spr with ani
  gs_t=obj->type&1023;

  if (obj->type==(339+(10*1024))){ //Sherry!
    gs_i=sprlnk[354]-1;
  }

  if (obji[sprlnk[obj->type&1023]].v4==6){ //drake

    gs_x=((obj->type>>10)&(7-1))+(keyframe>>2);
    gs_i=sprlnk[obj->type&1023]-1+gs_x;
    goto sprnopal;
  }

  if (obji[sprlnk[obj->type&1023]].v4==3){ //cycling objects
    gs_i=sprlnk[obj->type&1023]+(keyframe>>1)-1;
  }

  if (obji[sprlnk[obj->type&1023]].v4==11){ //cycling objects
    gs_i=sprlnk[obj->type&1023]+(obj->type>>10)+(keyframe>>1)-1;
  }

  if ((obj->type&1023)==357){ //corpser
    gs_i=sprlnk[357]-1;
    if (keyframe31==0) gs_i++;
    if (keyframe31==1) gs_i+=2;
    if (keyframe31==2) gs_i+=3;
    if (keyframe31==3) gs_i+=2;
    if (keyframe31==4) gs_i++;
  }

  if ((obj->type&1023)==356){ //bird
    gs_i=sprlnk[356]-1;
    if ((keyframe>>1)==0) gs_i++;
    if ((keyframe>>1)==1) gs_i+=2;
  }

  if ((obj->type&1023)==364){ //acid slug
    gs_i=sprlnk[364]-1;
    if (keyframe==0) gs_i+=1;
    if (keyframe==1) gs_i+=2;
    if (keyframe==2) gs_i+=3;
    if (keyframe==3) gs_i+=3;
    if (keyframe==4) gs_i+=2;
    if (keyframe==5) gs_i+=1;
    if (keyframe==6) gs_i+=0;
    if (keyframe==7) gs_i+=0;
  }

  if (obji[sprlnk[obj->type&1023]].v4==9){ //dragon wings
    gs_x=obj->type>>13;
    if ((gs_x>=3)||(gs_x==0)){
      gs_x=obj->type;
      if (gs_x&1024) gs_x-=1024;
      gs_i=sprlnk[gs_x&1023]+(gs_x>>10)+(keyframe>>2)-1;
    }
  }

  //jester juggling
  if ((obj->type&1023)==383){
    if ((obj->type&(2048+1024))!=(2048+1024)){
      gs_x=int(ett*3.0f)%3;
      gs_i=sprlnk[obj->type&1023]+(obj->type>>12<<2)+gs_x-1;
    }
  }

  //tanglevine core
  if ((obj->type&1023)==365){
    gs_i=sprlnk[obj->type&1023]+(keyframe>>2)-1;
  }

  if (gs_t==291){ //shaft (blue, override spr)
    gs_i=500+(obj->type>>10);
    goto sprnopal;
  }
  if (gs_i==348){ //fountain
    gs_y=1;
    gs_x=12+(keyframe>>1);
    goto sprsfx;
  }
  if (gs_i==334){ //small flags
    gs_y=2;
    gs_x=(keyframe>>1);
    goto sprsfx;
  }
  if (gs_i==335){ //small flags 2
    gs_y=2;
    gs_x=4+(keyframe>>1);
    goto sprsfx;
  }
  if (gs_i==319){ //big flag
    gs_y=1;
    gs_x=4+(keyframe>>1);
    goto sprsfx;
  }
  if (gs_t==288){ //crank
    gs_y=0;
    gs_x=16+(keyframe>>1);
    goto sprsfx;
  }
  if (gs_t==293){ //chain
    gs_y=1;
    gs_x=22+(keyframe>>2);
    goto sprsfx;
  }
  if (gs_t==292){ //gears
    gs_y=2;
    gs_x=16+((obj->type>>10)*2)+((keyframe>>1)&1);
    goto sprsfx;
  }
  if (gs_i==350){ //bell
    gs_y=1;
    gs_x=keyframe>>1;
    goto sprsfx;
  }
  if (gs_i==317){ //bellows
    gs_y=1;
    gs_x=20+(keyframe>>2);
    goto sprsfx;
  }

  if (gs_i==390){ //stocks man (left)
    gs_y=1;
    gs_x=24+(keyframe>>2);
    goto sprsfx;
  }
  if (gs_i==391){ //stocks man (right)
    gs_y=1;
    gs_x=24+2+(keyframe>>2);
    goto sprsfx;
  }

  if (gs_i==654){ //protection field
    gs_y=1;
    gs_x=16+(keyframe>>1);
    goto sprsfx;
  }


  if (gs_i==223){ //electric field
    gs_y=2;
    gs_x=28+(keyframe>>1);
    goto sprsfx;
  }
  if (gs_i==509){ //lightsource
    gs_y=2;
    gs_x=24+(keyframe>>1);
    goto sprsfx;
  }

  if (gs_i==187){ //clock bottom
    gs_y=1;
    gs_x=10+(keyframe>>2);
    goto sprsfx;
  }

  if (gs_i==505){ //millstone
    gs_y=0;
    gs_x=24+(keyframe>>1);
    goto sprsfx;
  }
  if (gs_t==287){ //water wheel
    gs_y=0;
    gs_x=28+(keyframe>>1);
    goto sprsfx;
  }
  if (gs_t==289){ //saw
    gs_y=0;
    gs_x=20+(keyframe>>1);
    goto sprsfx;
  }
  //add more ani here

  if ((obj->type&1023)==432){//dead giant rat
    gs_t=342; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==433){//dead giant bat
    gs_t=344; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==434){//dead reaper
    gs_t=347; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==435){//dead sheep
    gs_t=348; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==436){//dead dog
    gs_t=349; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==437){//dead deer
    gs_t=350; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==438){//dead wolf
    gs_t=351; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==439){//dead gremlin
    gs_t=353; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==440){//dead mouse
    gs_t=354; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==441){//dead bird
    gs_t=356; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==442){//dead snake
    gs_t=358; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==443){//dead rabbit
    gs_t=359; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==444){//dead spider
    gs_t=361; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==445){//dead drake
    gs_t=369; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==446){//dead mongbat
    gs_t=372; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }
  if ((obj->type&1023)==447){//dead cat
    gs_t=388; gs_i=sprlnk[gs_t]+(obj->type>>10)-1;
  }








  if (sprpi[gs_i]!=255){ //spr with pal
    if (GSs==1){


      //fields
      if (obj->type==317){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); imgt0(ps,GSx,GSy,bt32); GSs=0; return;}
      if (obj->type==318){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      if (obj->type==320){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}

      //fire (such as in bakery)
      if (obj->type==(304+1024*4)){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      if (obj->type==(304+1024*5)){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      if (obj->type==(304+1024*10)){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      if (obj->type==(304+1024*11)){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      //gems
      if (obj->type==77){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      //shrine forcefield
      if (obj->type==51){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      //lightning wand
      if (obj->type==79){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
      //wiz eye
      if (obj->type==52){img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]); imgt0(ps,GSx,GSy,bt32); GSs=0; return;}


      im32z(ps,GSx,GSy,spr8[keyframe],sprpi[gs_i]);

      GSs=0;}else{img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]);}
      return;
  }
sprnopal:



  gs_i2=gs_i/192; //spr no pal
  gs_y=gs_i-gs_i2*192;
  if (GSs==1){
    //glass
    if (obj->type==118){img(bt32,0,gs_y*-32,spr84[gs_i2]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //blood
    if ((obj->type&1023)==338){img(bt32,0,gs_y*-32,spr84[gs_i2]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //web
    if (obj->type==53){img(bt32,0,gs_y*-32,spr84[gs_i2]); imgt0(ps,GSx,GSy,bt32); GSs=0; return;}
    //slime
    if ((obj->type&1023)==375){img(bt32,0,gs_y*-32,spr84[gs_i2]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //acid slug
    if ((obj->type&1023)==364){img(bt32,0,gs_y*-32,spr84[gs_i2]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //ghost
    if ((obj->type&1023)==352){img(bt32,0,gs_y*-32,spr84[gs_i2]);  imgt0(ps,GSx,GSy,bt32); GSs=0; return;}
    //insects
    if ((obj->type&1023)==343){img(bt32,0,gs_y*-32,spr84[gs_i2]);  img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //corpser (ONLY FIRST FRAME)
    if (obj->type==357){img(bt32,0,gs_y*-32,spr84[gs_i2]);  imgt0(ps,GSx,GSy,bt32); GSs=0; return;}


    //shadows
    if (obj->type==229){img(bt32,0,gs_y*-32,spr84[gs_i2]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    if (obj->type==231){img(bt32,0,gs_y*-32,spr84[gs_i2]); imgt0(ps,GSx,GSy,bt32);img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    if (obj->type==238){img(bt32,0,gs_y*-32,spr84[gs_i2]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    if (obj->type==240){img(bt32,0,gs_y*-32,spr84[gs_i2]); imgt0(ps,GSx,GSy,bt32);img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    if (obj->type==251){img(bt32,0,gs_y*-32,spr84[gs_i2]); imgt0(ps,GSx,GSy,bt32);img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    if (obj->type==(251+1024)){img(bt32,0,gs_y*-32,spr84[gs_i2]); imgt0(ps,GSx,GSy,bt32);img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    if (obj->type==260){img(bt32,0,gs_y*-32,spr84[gs_i2]); imgt0(ps,GSx,GSy,bt32); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    if (obj->type==262){img(bt32,0,gs_y*-32,spr84[gs_i2]); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}


    im32z(ps,GSx,GSy,spr84[gs_i2],gs_y);

    if (((obj->type&1023)>=432)&&((obj->type&1023)<=447)){//blood of dead crt
      if ((obj->type&1023)!=434){//but not a reaper



	img(bt32,0,(171+((obj->type>>10)%3))*-32,spr84[3]);
	img75t0(ps,GSx,GSy,bt32);

      }
    }

    GSs=0;
  }else{
    img(bt32,0,gs_y*-32,spr84[gs_i2]);

    if (((obj->type&1023)>=432)&&((obj->type&1023)<=447)){//blood of dead crt
      if ((obj->type&1023)!=434){//but not a reaper
	img0(bt32,0,(171+((obj->type>>10)%3))*-32,spr84[3]);
      }
    }

  }
  return;

sprsfx:
  if (GSs==1){


    //protection field
    if ((obj->type&1023)==319){img(bt32,gs_x*-32,gs_y*-32,sfx8); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //fountain
    if ((obj->type&1023)==234){img(bt32,gs_x*-32,gs_y*-32,sfx8); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //lightsource
    if ((obj->type&1023)==294){img(bt32,gs_x*-32,gs_y*-32,sfx8); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}
    //electric field
    if ((obj->type&1023)==175){img(bt32,gs_x*-32,gs_y*-32,sfx8); img75t0(ps,GSx,GSy,bt32); GSs=0; return;}



    sf32z(ps,GSx,GSy,sfx8,gs_y*32+gs_x);
    GSs=0;}else{img(bt32,gs_x*-32,gs_y*-32,sfx8);}
}

