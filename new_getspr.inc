void getspr(object* obj){ //loads bt32 with a spr (uses keyframe) 
  unsigned short obj_type=obj->type&1023;
  unsigned short obj_index=obj->type>>10;

  /* default: */
  gs_i=sprlnk[obj_type]+obj_index-1; /* nr 1 vs index 0 */

  //check spr with ani
  gs_t=obj_type; 

  /* a few more special than other special cases: */

  if (obj->type==(339+(10*1024))){ //Sherry!
    gs_i=sprlnk[354]-1;
  } 


  if (obji[sprlnk[obj_type]].v4==3){ //cycling objects
    gs_i=sprlnk[obj_type]+(keyframe>>1)-1;
  }
  if (obji[sprlnk[obj_type]].v4==6){ //drake

    gs_x=((obj_index)&(7-1))+(keyframe>>2);
    gs_i=sprlnk[obj_type]-1+gs_x;
    goto sprnopal;
  }

  if (obji[sprlnk[obj_type]].v4==9){ //dragon wings
    gs_x=obj->type>>13;
    if ((gs_x>=3)||(gs_x==0)){
      gs_x=obj->type;
      if (gs_x&1024) gs_x-=1024;
      gs_i=sprlnk[gs_x&1023]+(gs_x>>10)+(keyframe>>2)-1;
    }
  }

  if (obji[sprlnk[obj_type]].v4==11){ //cycling objects
    gs_i=sprlnk[obj_type]+(obj_index)+(keyframe>>1)-1;
  }

  /* obj_type based */
  switch (obj_type) {
    case 357:  //corpser
      gs_i=sprlnk[357]-1;
      if (keyframe31==0) gs_i++;
      if (keyframe31==1) gs_i+=2;
      if (keyframe31==2) gs_i+=3;
      if (keyframe31==3) gs_i+=2;
      if (keyframe31==4) gs_i++;
      break;

    case 356:  //bird
      gs_i=sprlnk[356]-1;
      if ((keyframe>>1)==0) gs_i++;
      if ((keyframe>>1)==1) gs_i+=2;
      break;

    case 364:  //acid slug
      gs_i=sprlnk[364]-1;
      if (keyframe==0) gs_i+=1;
      if (keyframe==1) gs_i+=2;
      if (keyframe==2) gs_i+=3;
      if (keyframe==3) gs_i+=3;
      if (keyframe==4) gs_i+=2;
      if (keyframe==5) gs_i+=1;
      if (keyframe==6) gs_i+=0;
      if (keyframe==7) gs_i+=0;
      break;

    case 383: //jester juggling 
      if ((obj->type&(2048+1024))!=(2048+1024)){
	gs_x=int(ett*3.0f)%3;
	gs_i=sprlnk[obj_type]+(obj->type>>12<<2)+gs_x-1;
      }
      break;

      //tanglevine core
    case 365: 
      gs_i=sprlnk[obj_type]+(keyframe>>2)-1;
      break;

    case 291:  //shaft (blue, override spr)
      gs_i=500+(obj_index);
      goto sprnopal;
      break;
    case 288:  //crank
      gs_y=0;
      gs_x=16+(keyframe>>1);
      goto sprsfx;
      break;
    case 293:  //chain
      gs_y=1;
      gs_x=22+(keyframe>>2);
      goto sprsfx;
      break;
    case 292:  //gears
      gs_y=2;
      gs_x=16+((obj_index)*2)+((keyframe>>1)&1);
      goto sprsfx;
      break;
    case 287:  //water wheel
      gs_y=0;
      gs_x=28+(keyframe>>1);
      goto sprsfx;
      break;
    case 289:  //saw
      gs_y=0;
      gs_x=20+(keyframe>>1);
      goto sprsfx;
      break;
    //add more ani here

    case 432: //dead giant rat
      gs_t=342; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 433: //dead giant bat
      gs_t=344; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 434: //dead reaper
      gs_t=347; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 435: //dead sheep
      gs_t=348; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 436: //dead dog
      gs_t=349; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 437: //dead deer
      gs_t=350; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 438: //dead wolf
      gs_t=351; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 439: //dead gremlin
      gs_t=353; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 440: //dead mouse
      gs_t=354; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 441: //dead bird
      gs_t=356; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 442: //dead snake
      gs_t=358; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 443: //dead rabbit
      gs_t=359; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 444: //dead spider
      gs_t=361; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 445: //dead drake
      gs_t=369; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 446: //dead mongbat
      gs_t=372; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    case 447: //dead cat
      gs_t=388; gs_i=sprlnk[gs_t]+(obj_index)-1;
      break;
    /* default: */
  }

/* gs_i based */
  switch (gs_i) {
    case 348:{ //fountain
      gs_y=1;
      gs_x=12+(keyframe>>1);
      goto sprsfx;
    }
    case 334:{ //small flags
      gs_y=2;
      gs_x=(keyframe>>1);
      goto sprsfx;
    }
    case 335:{ //small flags 2
      gs_y=2;
      gs_x=4+(keyframe>>1);
      goto sprsfx;
    }
    case 319:{ //big flag
      gs_y=1;
      gs_x=4+(keyframe>>1);
      goto sprsfx;
    }
    case 350:{ //bell
      gs_y=1;
      gs_x=keyframe>>1;
      goto sprsfx;
    }
    case 317:{ //bellows
      gs_y=1;
      gs_x=20+(keyframe>>2);
      goto sprsfx;
    }
    case 390:{ //stocks man (left)
      gs_y=1;
      gs_x=24+(keyframe>>2);
      goto sprsfx;
    }
    case 391:{ //stocks man (right)
      gs_y=1;
      gs_x=24+2+(keyframe>>2);
      goto sprsfx;
    }
    case 654:{ //protection field
      gs_y=1;
      gs_x=16+(keyframe>>1);
      goto sprsfx;
    }
    case 223:{ //electric field
      gs_y=2;
      gs_x=28+(keyframe>>1);
      goto sprsfx;
    }
    case 509:{ //lightsource
      gs_y=2;
      gs_x=24+(keyframe>>1);
      goto sprsfx;
    }
    case 187:{ //clock bottom
      gs_y=1;
      gs_x=10+(keyframe>>2);
      goto sprsfx;
    }
    case 505:{ //millstone
      gs_y=0;
      gs_x=24+(keyframe>>1);
      goto sprsfx;
    }
    /* default: */
  }

/* need to emulate pallette? */
  if (sprpi[gs_i]!=255){ //spr with pal
    if (GSs==1){
      switch (obj->type) { // uses the complete type,index combination
      //fields
	case 317:
	case 318:
	case 320:
	  img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]);
	  img75t0(ps,GSx,GSy,bt32);
	  imgt0(ps,GSx,GSy,bt32);
	  GSs=0;
	  return;
      //fire (such as in bakery)
	case 304+1024*4: 
	case 304+1024*5: 
	case 304+1024*10: 
	case 304+1024*11:
	  img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]);
	  img75t0(ps,GSx,GSy,bt32);
	  GSs=0;
	  return;
      //gems
      case 77:
      //shrine forcefield
      case 51:
      //lightning wand
      case 79:
	  img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]);
	  img75t0(ps,GSx,GSy,bt32);
	  GSs=0;
	  return;
      //wiz eye
      case 52:
	  img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]);
	  imgt0(ps,GSx,GSy,bt32);
	  GSs=0;
	  return;
	  /* default */
      }

      im32z(ps,GSx,GSy,spr8[keyframe],sprpi[gs_i]);
      GSs=0;}
    else {
      img(bt32,0,sprpi[gs_i]*-32,spr8[keyframe]);
    }
    return;
  }
sprnopal:
  gs_i2=gs_i/192; //spr no pal
  gs_y=gs_i-gs_i2*192;
  if (GSs==1){
    switch (obj->type) { // uses complete type,index combo again
      case 118: //glass
      case 338: //blood
      case 343: //insects
      case 375: //slime
      case 364: //acid slug
      case 229: //shadows
      case 238: //shadows
      case 262: //shadows
	img(bt32,0,gs_y*-32,spr84[gs_i2]);
	img75t0(ps,GSx,GSy,bt32);
	GSs=0;
	return;
      case 53: //web
      case 352: //ghost
      case 357: //corpser (ONLY FIRST FRAME)
	img(bt32,0,gs_y*-32,spr84[gs_i2]); 
	imgt0(ps,GSx,GSy,bt32);
	GSs=0;
	return;
      case 231: //shadows
      case 240: //shadows
      case 251: //shadows
      case 251+1024: //shadows
      case 260: //shadows
	img(bt32,0,gs_y*-32,spr84[gs_i2]);
	imgt0(ps,GSx,GSy,bt32);
	img75t0(ps,GSx,GSy,bt32);
	GSs=0;
	return;
	/* default: */
    }

    im32z(ps,GSx,GSy,spr84[gs_i2],gs_y);

    if (((obj_type)>=432)&&((obj_type)<=447)){//blood of dead crt
      if ((obj_type)!=434){//but not a reaper
	img(bt32,0,(171+((obj_index)%3))*-32,spr84[3]);
	img75t0(ps,GSx,GSy,bt32);
      }
    }

    GSs=0;
  }else{
    img(bt32,0,gs_y*-32,spr84[gs_i2]);

    if (((obj_type)>=432)&&((obj_type)<=447)){//blood of dead crt
      if ((obj_type)!=434){//but not a reaper
	img0(bt32,0,(171+((obj_index)%3))*-32,spr84[3]);
      }
    }
  }
  return;

sprsfx:
  if (GSs==1){
    switch (obj_type) {
      case 319: //protection field
      case 234: //fountain
      case 294: //lightsource
      case 175://electric field
	img(bt32,gs_x*-32,gs_y*-32,sfx8);
	img75t0(ps,GSx,GSy,bt32);
	GSs=0;
	return;
	/* default: */
    }
    sf32z(ps,GSx,GSy,sfx8,gs_y*32+gs_x);
    GSs=0;}else{img(bt32,gs_x*-32,gs_y*-32,sfx8);}
}

