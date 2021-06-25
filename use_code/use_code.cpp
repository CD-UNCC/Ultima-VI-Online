#include "use_code.h"
#include "../data_host.h"
#include "../function_host.h"
#include "../function_both.h"

U6O_USE_FUNCTION(craft) {
  int obj_t=myobj->type&1023;
  int obj_q=myobj->type>>10;
  int plr_t=myplr->party[0]->type&1023;
  object* obj_orig=myobj;
  int i=0;
  void * more=NULL;
  npc * tnpc3=NULL;

  while ((myobj->info&8)==8) { /*redirector*/
    myobj=(object*)myobj->more; 
  }

  if (myplr->party[0]==myobj){
    more=NULL; /* usually */
    switch (plr_t) { /* exit craft if already in one. */
      case OBJ_SHIP: /*ship/boat/galleon/nom-du-jour*/
	more=myplr->craft_con;
      case OBJ_SKIFF: /*skiff*/
      case OBJ_RAFT: /*raft*/
	VLNKremove(myplr->party[0]); VLNKsremove(myplr->party[0]); /* remove vlnk(s) */
	myplr->party[0]->more=more;
	myplr->party[0]->info=myplr->party[0]->info&(1<<9);
	myplr->party[0]=myplr->craft;
	myplr->craft=NULL;

	partyadd(myplr,myobj->x,myobj->y);

	return; /* done */
	break;
      case OBJ_BALLOON: /*balloon*/
	VLNKremove(tplayer->party[0]); VLNKsremove(myplr->party[0]); //remove vlnk(s)
	myplr->party[0]->more=myplr->craft_con;
	myplr->party[0]->info=myplr->party[0]->info&(1<<9);
	myplr->party[0]=myplr->craft;
	myplr->craft=NULL;

	partyadd(myplr,myobj->x,myobj->y);

	//remove balloon from map + return item to inventory
	mlobj *mmyobj; //array size varies
	mmyobj=(mlobj*)myobj->more;
	OBJremove(mmyobj->obj[0]); OBJrelease(mmyobj->obj[0]);
	OBJremove(mmyobj->obj[1]); OBJrelease(mmyobj->obj[1]);
	OBJremove(mmyobj->obj[2]); OBJrelease(mmyobj->obj[2]);
	myobj->type=420; myobj->info|=256;
	OBJremove(myobj);
	tnpc3=(npc*)myplr->party[0]->more;
	additemroot(tnpc3,myobj);
	tnpc3->upflags|=32;
	return; /* done */
	break;
    } //switch
  }
  /* use raft, skiff, ship */
  if (myobj->info&2) { /* already in use! */
    return;
  } 
  if ((obj_t!=OBJ_RAFT) && !(myobj->info&(1<<9))) { /* don't check deed for free craft */
    int last_object=0;
    int has_deed=0;
    //DO YOU HAVE THE DEED???
    for (i=0;i<=7;i++){ 
      if (tplayer->party[i]){
	tnpc3=(npc*)myplr->party[i]->more;
	last_object=OBJlist((object*)tnpc3->items->more); 
      }
    } //!=NULL,x6
    OBJlist_last=NULL;
    for(i=0;i<last_object;i++){
      if (OBJlist_list[i]->type==149){ /* deed */
	if (OBJlist_list[i]->more==myobj) { /* matches ship */
	  has_deed=1;
	  break; /* stop looking */
	}
      }
    }
    if (!has_deed) {
      static txt * t=txtnew();
      txtset(t,"?"); t->d2[0]=8; 
      txtadd(t,"A deed is required!");
      NET_send(NETplayer,tplayer->net,t); 
      return;
    }
  }

  if (myplr->selected_partymember){ /*change selected_partymember to 0 */
    if (myplr->party[tplayer->selected_partymember]){
      tnpc3=(npc*)myplr->party[myplr->selected_partymember]->more;
      if (tnpc3->order==0) tnpc3->order=1;
      tnpc3->follow=tplayer->party[0];
    }
    tnpc3=(npc*)myplr->party[0]->more;
    if (tnpc3->order==1) tnpc3->order=0;
    tnpc3->follow=myplr->party[0];
    myplr->selected_partymember=0;
  }

  myplr->craft_con=(object*)myobj->more;//get the pointers to the other graphics of the boat
  myobj->more=myplr->party[0]->more;
  myobj->info=myplr->party[0]->info|(myobj->info&(1<<9));
  myplr->craft=myplr->party[0];
  for (i=7;i>=0;i--){
    if (myplr->party[i]!=NULL){
      tnpc3=(npc*)myplr->party[i]->more; 
      OBJremove(myplr->party[i]);
      if (tnpc3->horse) {
	OBJremove((object*)tnpc3->more);//remove horse's tail
      }
      VLNKremove(myplr->party[i]); VLNKsremove(myplr->party[i]);
      tnpc3=(npc*)myplr->party[i]->more; 
      tnpc3->target=NULL;
    }
  }
  myplr->party[0]=myobj;
  if (myobj->info&32768) {
    myobj->info-=32768;
  }
  return;
}


U6O_USE_FUNCTION(basic_toggle) {
  object * myobj2;
  unsigned short x,y;
  /* this is the basic function of a toggle: */
  myobj->type^=1024; // flip the positio

  /* some toggles (switch, lever) teleport another object into a doorway etc. 
   * It doesn't hurt to try this with candles, 
   * */
  switch (myobj->type&1023) {
    case 223: /* powder keg */
    case 186: /* barrel */
    case 192: /* crate */
    case  98: /* chest */
    /* don't do the next step for powder kegs etc. as they store something else in more (owner, content) */
    return;
    default:
    /* many toggle objects have nothing in the more pointer, but switches and levers teleport in/out blockers,
     * this allows things like light the candle open the door. */
    for (myobj2=(object*)myobj->more;myobj2;myobj2=(object*)myobj2->more){
      x=myobj2->x;
      y=myobj2->y;
      if (y&32768){
	y-=32768;
	OBJadd(x,y,myobj2);
      } else {
	OBJremove(myobj2);
	myobj2->x=x; myobj2->y=y+32768;
      }
    }
  }
}

U6O_USE_FUNCTION(toggle) {
  unsigned short x,y;
  unsigned long sound_effect;

  use_basic_toggle(myplr,myobj);

  x=myobj->x; y=myobj->y;
  sound_effect=SFnew(x,y);
  sf[sound_effect].type=4;
  sf[sound_effect].x=x;
  sf[sound_effect].y=y;
  sf[sound_effect].wait=1.0f; //NULL
  switch (myobj->type&1023) {
    case 268: // lever
      sf[sound_effect].more=4;
      break;
    case 174: // switch
      sf[sound_effect].more=3;
      break;
    case 288: // crank
      sf[sound_effect].more=0; //TODO add effect
      break;
    default: // default to lever, FIXME/TODO probably nice to add fire effect here (whoosh!) when toggling fire. (cool: light fire open related gate)
      sf[sound_effect].more=4;
  }
  return;
}


