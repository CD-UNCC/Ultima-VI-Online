#include "function_client.h"
//#include <windows.h>
#pragma warning(disable: 4018 4244)
void function_client_init(void){
  GETINPUT_tab_pressed=0;
  GETINPUT_txt=NULL;
  GETINPUT_enterpressed=NULL;
  GETINPUT_old=NULL; //used to detect new pointers
  GSs=0;GSx=0;GSy=0;
  midikeyboard_set=-1;
  musickeyboard_set=255;
  clientinstrument=0;
  playinstrument=0;
  midipause=0;
  getsound_MOVERSOUND=0;
  AMBIENTLIGHT_LIGHTVALUE=15;
  AMBIENTLIGHT_SHOWSUN=FALSE;
  STATUSMESSdisplaying=txtnew();//the message currently being displayed
  STATUSMESSwait=0.0f;
  STATUSMESSskipok=0;//the message will be skipped if any messages are pending
  STATUSMESSt=txtnew();//temp txt for building messages (included to aid conversion from older system)
  STATUSMESSpending=txtnew();
  GETSETTING_RAW=txtnew();//the actualt text between the square brackets [...]
  li2_t=txtnew();

}; 

//for compatibility the client supports this function
object *OBJnew_local(){
static object *newobject;
newobject=(object*)malloc(sizeof(object));
ZeroMemory(newobject,sizeof(object));
return newobject;
}

//GetInput functions
//tab_pressed allows program to trap the tab key
//once trapped it also counts as an enterpressed, so serves a dual purpose
//otherwise tab key inserts an undefined amount of spaces
//it MUST be set after a call to getinput_setup
void GETINPUT_setup(txt* input_pointer,void* enterpressed_pointer,unsigned long maxlength){
GETINPUT_txt=input_pointer;
GETINPUT_enterpressed=(unsigned char*) enterpressed_pointer;
GETINPUT_maxlength=maxlength;
GETINPUT_tab_pressed=0;
}
txt* GETINPUT_current(){
return GETINPUT_txt;
}
void GETINPUT_stop(){
if (GETINPUT_enterpressed) {GETINPUT_enterpressed[0]=TRUE; GETINPUT_enterpressed=NULL;}
GETINPUT_txt=NULL;
}

// luteijn: which of these? :)
#include "new_getspr.inc"
//#include "old_getspr.inc"

bool keyhit(unsigned short k){ //returns TRUE if key has been pressed
  if (key[k]==FALSE) return FALSE;
  key[k]=FALSE; return TRUE;
}

bool u6okeyhit(unsigned char i){ //returns TRUE if key has been pressed
  if (U6OK[i][0]){
    if (U6OK[i][1]==0){ //only single key required
      if (key[U6OK[i][0]]==FALSE) return FALSE;
      key[U6OK[i][0]]=FALSE; return TRUE;
    }
    //double key combo?
    if (keyon[U6OK[i][0]]){
      if (key[U6OK[i][1]]==FALSE) return FALSE;
      key[U6OK[i][1]]=FALSE; return TRUE;
    }//first key held
  }//exists
  return FALSE;
}

bool u6okeyon(unsigned char i){ //returns TRUE if key has been pressed
  if (U6OK[i][0]){
    if (U6OK[i][1]==0){ //only single key required
      if (keyon[U6OK[i][0]]) return TRUE;
      return FALSE;
    }
    //double key combo?
    if (keyon[U6OK[i][0]]){
      if (keyon[U6OK[i][1]]) return TRUE;
    }//first key held
  }//exists
  return FALSE;
}

void u6okeyseton(unsigned char i){
  if (U6OK[i][0]){//exists
    if (U6OK[i][1]==0){//only single key required
      keyon[U6OK[i][0]]=1;
    }
    //double key combo?
    keyon[U6OK[i][0]]=1;
    keyon[U6OK[i][1]]=1;
  }//exists
}

void u6okeysetoff(unsigned char i){
  if (U6OK[i][0]){//exists
    if (U6OK[i][1]==0){//only single key required
      keyon[U6OK[i][0]]=0;
    }
    //double key combo?
    keyon[U6OK[i][0]]=0;
    keyon[U6OK[i][1]]=0;
  }//exists
}

void GETINPUT_update(){
  static long i,i2;
  if (GETINPUT_txt!=NULL){ //GETINPUT active, trap ascii keys
    if (GETINPUT_txt!=GETINPUT_old){
      for (i=0;i<=65535;i++) {keyasc[i]=FALSE;} //clear ascii buffer
    }

    //check for PASTE
    if (keyon[VK_SHIFT]){
      if (keyhit(VK_INSERT)){
	goto getinput_paste;
      }
    }
    if (keyon[VK_CONTROL]){
      keyasc[86]=FALSE; keyasc[118]=FALSE;
      if (keyhit(86)){
getinput_paste:
	static unsigned char *getinput_clipboardtxt;
	if(OpenClipboard(NULL))
	{
	  getinput_clipboardtxt=(unsigned char*)GetClipboardData(CF_TEXT);
	  if (getinput_clipboardtxt){
	    i=0;
getinput_paste_next:
	    if (i2=getinput_clipboardtxt[i]){
	      if ((i2>=32)&&(i2<=126)){
		if ((GETINPUT_txt->l<GETINPUT_maxlength)||GETINPUT_maxlength==0){
		  txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l+1);
		  GETINPUT_txt->d[GETINPUT_txt->l-1]=i2;
		}
	      }
	      i++; goto getinput_paste_next;
	    }
	  }
	  CloseClipboard();
	}
	for (i=0;i<=65535;i++){
	  if ((i<0x25)||(i>0x28)){
	    if (i!=VK_CONTROL){
	      if (i!=VK_SHIFT){
		key[i]=FALSE; keyon[i]=FALSE;
	      }
	    }
	  }
	} //clear pressed key buffers
      }
    }



    for (i=32;i<=126;i++){
      if (keyasc[i]){
	keyasc[i]=FALSE;
	if ((GETINPUT_txt->l<GETINPUT_maxlength)||GETINPUT_maxlength==0){
	  txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l+1);
	  GETINPUT_txt->d[GETINPUT_txt->l-1]=i;
	}
	for (i2=0;i2<=65535;i2++) if ((i2<0x25)||(i2>0x28)) {key[i2]=FALSE; keyon[i2]=FALSE; } //clear pressed key buffers
      }}
    if (keyasc[8]){
      keyasc[8]=FALSE;
      if (GETINPUT_txt->l!=0) txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l-1);
      for (i=0;i<=65535;i++) if ((i<0x25)||(i>0x28)) {key[i]=FALSE; keyon[i]=FALSE; } //clear pressed key buffers
    }

    if (keyasc[9]){//tab
      keyasc[9]=FALSE;
      if (GETINPUT_tab_pressed==GETINPUT_TAB_PRESSED_INIT){
	GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED;
	goto getinput_tabreturn;
      }
      if (GETINPUT_tab_pressed==GETINPUT_TAB_PRESSED_STOP){
	for (i=1;i<=8;i++){
	  if ((GETINPUT_txt->l<GETINPUT_maxlength)||GETINPUT_maxlength==0){
	    txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l+1);
	    GETINPUT_txt->d[GETINPUT_txt->l-1]=32;
	  }
	}//i
      }
      for (i=0;i<=65535;i++) if ((i<0x25)||(i>0x28)) {key[i]=FALSE; keyon[i]=FALSE; } //clear pressed key buffers
    }//tab

    if (u6okeyhit(U6OK_TALK_DONE)){
getinput_tabreturn:
      GETINPUT_txt=NULL;
      if (GETINPUT_enterpressed!=NULL){
	GETINPUT_enterpressed[0]=TRUE;
	GETINPUT_enterpressed=NULL;
      }
      for (i=0;i<=65535;i++) if ((i<0x25)||(i>0x28)) {key[i]=FALSE; keyon[i]=FALSE; } //clear pressed key buffers
    }

  }
  GETINPUT_old=GETINPUT_txt;
}




void mididown(unsigned char instrument,unsigned char key){



  if (midiout_setup) midiOutShortMsg(midiout_handle,0x007E0090+key*256+instrument);

  if (midipause==0){
    midipause=1;
    if (U6O_DISABLEMUSIC==FALSE){
      u6omidi->Stop();
    }
  }

  midikeystack[instrument][key]++;
  midikeywait[instrument][key]=ett;
}

void midiup(unsigned char instrument,unsigned char key){
  if (key==255) return;


  if (midikeystack[instrument][key]){
    midikeystack[instrument][key]--;
    if (midikeystack[instrument][key]==0){

      if (instrument==3) if (midiout_setup) midiOutShortMsg(midiout_handle,0x00000080+key*256+instrument);


    }//midikeystack[instrument][key]==0
  }//midikeystack[instrument][key]

}//midiup

void LIGHTnew(unsigned short x,unsigned short y,unsigned long light_data_offset, unsigned short x_axis_size){
  if (timelval==0) return;
  static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
  static long x2,y2,x3,y3,x4,y4;
  static long xoff,yoff;
  xoff=x-x_axis_size/2;
  xoff<<=5;
  yoff=y-x_axis_size/2;
  yoff<<=5;
  x_axis_size<<=5;
  x2=xoff;//start dest x offset
  x3=0;//start source x offset
  x4=x_axis_size; //displayed "pixels" of x axis
  if (xoff<0){x4+=xoff; x2=0; x3=-xoff;}
  if ((xoff+x_axis_size)>1024) x4-=xoff+x_axis_size-1024;
  y2=yoff;//starting dest y offset
  y3=0;//starting source y offset
  y4=x_axis_size; //rows on screen
  if (yoff<0){y4+=yoff; y2=0; y3=-yoff;}
  if ((yoff+x_axis_size)>768) y4-=yoff+x_axis_size-768;
  asm_copy_vc_bytesx=x4;
  asm_copy_vc_sourceskip=x_axis_size-asm_copy_vc_bytesx;
  asm_copy_vc_destskip=1024-asm_copy_vc_bytesx;
  asm_copy_vc_sourceoffset=y3*x_axis_size+light_data_offset+x3;
  asm_copy_vc_destoffset=(y2<<10)+(unsigned long)&ls+x2;
  asm_copy_vc_rows=y4;
  _asm{
    push esi
      push edi
      push ebx
      mov ecx,asm_copy_vc_rows
      mov edx,asm_copy_vc_bytesx
      mov esi,asm_copy_vc_sourceoffset
      mov edi,asm_copy_vc_destoffset
      asm_light1:
      mov ebx,esi
      add ebx,edx
      asm_light0:
      mov eax,[esi]
      add [edi],eax
      add esi,4
      add edi,4
      cmp esi,ebx
      jne asm_light0
      add edi,asm_copy_vc_destskip
      add esi,asm_copy_vc_sourceskip
      dec ecx
      jnz asm_light1
      pop ebx
      pop edi
      pop esi
  }//_asm
}//LIGHTnew


// s333 backup player mv info
void backupplayermvinfon1(player* tplayer) {
	combatinfoplayerprevinit = 1;
	combatinfoplayerprev.mv_i = tplayer->mv_i;
	for (int i = 0; i < tplayer->mv_i; i++) {
		if ((tplayer->mv_x[i] != tplayer->x) && (tplayer->mv_y[i] != tplayer->y)) {
			combatinfoplayerprev.mv_x[i] = tplayer->mv_x[i];
			combatinfoplayerprev.mv_y[i] = tplayer->mv_y[i];
			combatinfoplayerprev.mv_type[i] = tplayer->mv_type[i];
		} else {
			combatinfoplayerprev.mv_x[i] = 0;
			combatinfoplayerprev.mv_y[i] = 0;
			combatinfoplayerprev.mv_type[i] = 0;
		}
	}
}



// s333 get best-guess type of object at coordinate (if possible)
int getobjtypen1b(player p, int mapx, int mapy, int exactcoord) {
	int objtype = -1;
	int distx, disty; // , dist;
	int bestdistx = 6;
	int bestdisty = 6;
	//int bestdist = 100;
	int bestdisti = -1;

	resultinfon1 = 0;
	if (combatinfoplayerprevinit) {
		for (int i = 0; i < p.mv_i; i++) {
			if ((p.mv_x[i] == mapx) && (p.mv_y[i] == mapy)) {
				//objtype = objgettype(tplayer->mv_type[i], tplayer->mv_dir[i], tplayer->mv_frame[i]);
				objtype = p.mv_type[i];
				break;
			}
		}

		if (objtype == 0)
			objtype = -1;
		else
			resultinfon1 = 1;

		if (!exactcoord && (objtype == -1)) {
			for (int i = 0; i < p.mv_i; i++) {
				distx = abs(p.mv_x[i] - mapx);
				disty = abs(p.mv_y[i] - mapy);

				if ( ((distx < bestdistx) && (disty <= bestdisty)) || ((distx <= bestdistx) && (disty < bestdisty)) ) {
				//dist = distx + disty;
				//if (dist < bestdist) {
					//bestdist = dist;
					bestdistx = distx;
					bestdisty = disty;
					bestdisti = i;
				}
			}

			if (bestdisti != -1) {
				objtype = p.mv_type[bestdisti];
				resultinfon1 = 2;
			}
		}
	}

	/*
	txt* t3 = txtnew();
	txtset(t3, "using best guess");
	LOGadd(t3);
	*/

	return objtype;
}

// s333 get type of object at coordinate
int getobjtypen1(player* tplayer, int mapx, int mapy) {
	int objtype = -1;

	resultinfon1 = 0;
	for (int i = 0; i < tplayer->mv_i; i++) {
		if ((tplayer->mv_x[i] == mapx) && (tplayer->mv_y[i] == mapy)) {
			//objtype = objgettype(tplayer->mv_type[i], tplayer->mv_dir[i], tplayer->mv_frame[i]);
			objtype = tplayer->mv_type[i];
			resultinfon1 = 1;
			break;
		}
	}

	if (objtype == -1) {
		//mapx=tpx+x-1; mapy=tpy+y-1;
		int bufx=mapx-tplayer->sobj_bufoffx;
		int bufy=mapy-tplayer->sobj_bufoffy;

		if ((bufx >= 0) && (bufx < 96) && (bufy >= 0) && (bufy < 72)) {
			if (int i = tplayer->sobj_bufsize[bufx][bufy]) {
				objtype = tplayer->sobj[bufx][bufy][i - 1];
				resultinfon1 = 2;
			}

			if ( (objtype == -1) && (mapx >= 0) && (mapx < 1024) && (mapy >= 0) && (mapy < 2048)) {
				if (int i = tobjfixed_index[mapx][mapy]) {
					if ((tplayer->sobj_tempfixed[bufx][bufy] & 1) == 0)
						if ((i >= 0) && (i < 65536))
							if ((i + tobjfixed_type[i] >= 0) && (i + tobjfixed_type[i] < 65536))
								objtype = tobjfixed_type[i + tobjfixed_type[i]];
								resultinfon1 = 3;
				}
			}

		}

		/*
		if (objtype == -1) {
			if (int i = objfixed_index[mapx][mapy]) {
				objtype = objfixed_type[i + objfixed_type[i]];
			}
		}
		*/

	} //else {
		//if (obji[sprlnk[objtype & 1023]].v4 == 1) //"avatar"
			//objtype = OBJ_AVATAR;
	//}



	/*
	for (y = 0; y <= 25; y++) {
		for (x = 0; x <= 33; x++) {
			mapx = tpx + x - 1;
			mapy = tpy + y - 1;
			for (i = 0; i < tplayer->mv_i; i++) {
				if (tplayer->mv_x[i] == mapx) {
					if (tplayer->mv_y[i] == mapy) {
						x2 = x - 1; y2 = y - 1;
						x3 = objgettype(tplayer->mv_type[i], tplayer->mv_dir[i], tplayer->mv_frame[i]);
						myobj->type = x3;

						//non-pass specific changes
						//if (tplayer->mv_flags[i] & MV_PARALYZE) keyframe = 0;
						//check if sleeping
						//if (tplayer->mv_flags[i] & MV_SLEEP) {
						z3 = myobj->type & 1023; z2 = 0;
						//	if (z3 == 376) z2 = 339 + 6 * 1024;
						//}
						txtset(t3, "[");
						txtnumint(t, tplayer->mv_type[i]);
						txtadd(t3, t);
						txtadd(t3, "] ");
						txtnumint(t, z3);
						txtadd(t3, t);
						txtadd(t3, ": ");
						txtnumint(t, x);
						txtadd(t3, t);
						txtadd(t3, ", ");
						txtnumint(t, y);
						txtadd(t3, t);
						txtadd(t3, " // ");
						txtnumint(t, mapx);
						txtadd(t3, t);
						txtadd(t3, ", ");
						txtnumint(t, mapy);
						txtadd(t3, t);
						txtadd(t3, " // ");
						txtnumint(t, sfx[i3].x);
						txtadd(t3, t);
						txtadd(t3, ", ");
						txtnumint(t, sfx[i3].y);
						txtadd(t3, t);
						txtadd(t3, " // ");
						LOGadd(t3);
					}
				}
			}
		}
	}
	*/

	return objtype;
}

// s333
int checkdeadobjtypen1(int typeparam) {
	int objtypedead = 0;
	int type;

	if (typeparam >= 0)
		type = typeparam & 1023;
	else
		type = typeparam;

	switch (type) {
	case -1:
		objtypedead = 0;
		break;
	case OBJ_DEAD_ANIMAL:
	case OBJ_DEAD_BODY:
	case OBJ_DEAD_CYCLOPS:
	case OBJ_DEAD_GARGOYLE:
	case OBJ_DEAD_GIANT_RAT:
	case OBJ_DEAD_GIANT_BAT:
	case OBJ_DEAD_REAPER:
	case OBJ_DEAD_SHEEP:
	case OBJ_DEAD_DOG:
	case OBJ_DEAD_DEER:
	case OBJ_DEAD_WOLF:
	case OBJ_DEAD_GREMLIN:
	case OBJ_DEAD_MOUSE:
	case OBJ_DEAD_BIRD:
	case OBJ_DEAD_SNAKE:
	case OBJ_DEAD_RABBIT:
	case OBJ_DEAD_SPIDER:
	case OBJ_DEAD_DRAKE:
	case OBJ_DEAD_MONGBAT:
	case OBJ_DEAD_CAT:
		objtypedead = 1;
		break;
	default:
		objtypedead = 0;
		break;
	}

	return objtypedead;
}

// s333
int convertdeadobjtypen1(int typeparam) {
	int objtype;
	int type;

	if (typeparam >= 0)
		type = typeparam & 1023;
	else
		type = typeparam;

	switch (type) {
	case -1:
		objtype = -1;
		break;
	case OBJ_DEAD_ANIMAL:
		objtype = -2;
		break;
	case OBJ_DEAD_BODY:
		objtype = -2;
		// 0 = headless, 2 = gypsy?, 3 = mage/woman, 4 = guard, 5 = swashbucker, 6 = fighter
		break;
	case OBJ_DEAD_CYCLOPS:
		objtype = OBJ_CYCLOPS;
		break;
	case OBJ_DEAD_GARGOYLE:
		objtype = OBJ_GARGOYLE_WINGED;
		break;
	case OBJ_DEAD_GIANT_RAT:
		objtype = OBJ_GIANT_RAT;
		break;
	case OBJ_DEAD_GIANT_BAT:
		objtype = OBJ_GIANT_BAT;
		break;
	case OBJ_DEAD_REAPER:
		objtype = OBJ_REAPER;
		break;
	case OBJ_DEAD_SHEEP:
		objtype = OBJ_SHEEP;
		break;
	case OBJ_DEAD_DOG:
		objtype = OBJ_DOG;
		break;
	case OBJ_DEAD_DEER:
		objtype = OBJ_DEER;
		break;
	case OBJ_DEAD_WOLF:
		objtype = OBJ_WOLF;
		break;
	case OBJ_DEAD_GREMLIN:
		objtype = OBJ_GREMLIN;
		break;
	case OBJ_DEAD_MOUSE:
		objtype = OBJ_MOUSE;
		break;
	case OBJ_DEAD_BIRD:
		objtype = OBJ_BIRD;
		break;
	case OBJ_DEAD_SNAKE:
		objtype = OBJ_SNAKE;
		break;
	case OBJ_DEAD_RABBIT:
		objtype = OBJ_RABBIT;
		break;
	case OBJ_DEAD_SPIDER:
		objtype = OBJ_GIANT_SPIDER;
		break;
	case OBJ_DEAD_DRAKE:
		objtype = OBJ_DRAKE;
		break;
	case OBJ_DEAD_MONGBAT:
		objtype = OBJ_MONGBAT;
		break;
	case OBJ_DEAD_CAT:
		objtype = OBJ_CAT;
		break;
	default:
		objtype = -1;
		break;
	}

	//txt* tt22 = txtnew();
	//txtnumint(tt22, type);
	//LOGadd(tt22);
	return objtype;
}

// s333 check object type is creature
int checkobjtypecreaturen1(int typeparam) {
	int checkresult = -2;
	int type;

	if (typeparam >= 0)
		type = typeparam & 1023;
	else
		type = typeparam;

	switch (type) {
	case -1:
		//txtset(txtdesc, "INVALID");
		break;
	case OBJ_GIANT_RAT:
	case OBJ_INSECTS:
	case OBJ_GIANT_BAT:
	case OBJ_GIANT_SQUID:
	case OBJ_SEA_SERPENT:
	case OBJ_REAPER:
	case OBJ_SHEEP:
	case OBJ_DOG:
	case OBJ_DEER:
	case OBJ_WOLF:
	case OBJ_GHOST:
	case OBJ_GREMLIN:
	case OBJ_MOUSE:
	case OBJ_GAZER:
	case OBJ_BIRD:
	case OBJ_CORPSER:
	case OBJ_SNAKE:
	case OBJ_RABBIT:
	case OBJ_ROT_WORMS:
	case OBJ_GIANT_SPIDER:
	case OBJ_GARGOYLE_WINGED:
	case OBJ_GARGOYLE_WINGLESS:
	case OBJ_ACID_SLUG:
	case OBJ_TANGLE_VINE_POD:
	case OBJ_TANGLE_VINE:
	case OBJ_DAEMON:
	case OBJ_SKELETON:
	case OBJ_DRAKE:
	case OBJ_HEADLESS:
	case OBJ_TROLL:
	case OBJ_MONGBAT:
	case OBJ_WISP:
	case OBJ_HYDRA_MOUTH:
	case OBJ_SLIME:
	case OBJ_FIGHTER:
	case OBJ_SWASHBUCKLER:
	case OBJ_MAGE:
	case OBJ_VILLAGER:
	case OBJ_MERCHANT:
	case OBJ_CHILD:
	case OBJ_GUARD:
	case OBJ_JESTER:
	case OBJ_PEASANT:
	case OBJ_FARMER:
	case OBJ_MUSICIAN:
	case OBJ_WOMAN:
	case OBJ_CAT:
	case OBJ_LORD_BRITISH:
	case OBJ_AVATAR:
	case OBJ_DRAGON:
	case OBJ_SHIP:
	case OBJ_SILVER_SERPENT:
	case OBJ_CYCLOPS:
	//case OBJ_HYDRA_ARMS:
		//txtset(txtdesc, "");
		//break;
	case OBJ_GIANT_SCORPION:
	case OBJ_GIANT_ANT:
	case OBJ_COW:
	case OBJ_ALLIGATOR:
	case OBJ_HORSE:
		checkresult = 1;
		break;
	case OBJ_CHEST:
	case OBJ_WEB:
		checkresult = 2;
		break;
	default:
		checkresult = -2;
		break;
	}

	return checkresult;
}

// s333 get description of object type (for creature)
void getobjdescn1(txt* txtdesc, int typeparam) {
	if (txtdesc) {
		int type;

		if (typeparam >= 0)
			type = typeparam & 1023;
		else
			type = typeparam;

		switch (type) {
		case -1:
			txtset(txtdesc, "INVALID");
			break;
		case OBJ_GIANT_RAT:
			txtset(txtdesc, "Giant rat");
			break;
		case OBJ_INSECTS:
			txtset(txtdesc, "Insect");
			break;
		case OBJ_GIANT_BAT:
			txtset(txtdesc, "Giant bat");
			break;
		case OBJ_GIANT_SQUID:
			txtset(txtdesc, "Giant squid");
			break;
		case OBJ_SEA_SERPENT:
			txtset(txtdesc, "Sea serpent");
			break;
		case OBJ_REAPER:
			txtset(txtdesc, "Reaper");
			break;
		case OBJ_SHEEP:
			txtset(txtdesc, "Sheep");
			break;
		case OBJ_DOG:
			txtset(txtdesc, "Dog");
			break;
		case OBJ_DEER:
			txtset(txtdesc, "Deer");
			break;
		case OBJ_WOLF:
			txtset(txtdesc, "Wolf");
			break;
		case OBJ_GHOST:
			txtset(txtdesc, "Ghost");
			break;
		case OBJ_GREMLIN:
			txtset(txtdesc, "Gremlin");
			break;
		case OBJ_MOUSE:
			txtset(txtdesc, "Mouse");
			break;
		case OBJ_GAZER:
			txtset(txtdesc, "Gazer");
			break;
		case OBJ_BIRD:
			txtset(txtdesc, "Bird");
			break;
		case OBJ_CORPSER:
			txtset(txtdesc, "Corpser");
			break;
		case OBJ_SNAKE:
			txtset(txtdesc, "Snake");
			break;
		case OBJ_RABBIT:
			txtset(txtdesc, "Rabbit");
			break;
		case OBJ_ROT_WORMS:
			txtset(txtdesc, "Rot worms");
			break;
		case OBJ_GIANT_SPIDER:
			txtset(txtdesc, "Giant spider");
			break;
		case OBJ_GARGOYLE_WINGED:
			txtset(txtdesc, "Winged gargoyle");
			break;
		case OBJ_GARGOYLE_WINGLESS:
			txtset(txtdesc, "Gargoyle");
			break;
		case OBJ_ACID_SLUG:
			txtset(txtdesc, "Acid slug");
			break;
		case OBJ_TANGLE_VINE_POD:
			txtset(txtdesc, "Tangle vine pod");
			break;
		case OBJ_TANGLE_VINE:
			txtset(txtdesc, "Tangle vine");
			break;
		case OBJ_DAEMON:
			txtset(txtdesc, "Daemon");
			break;
		case OBJ_SKELETON:
			txtset(txtdesc, "Skeleton");
			break;
		case OBJ_DRAKE:
			txtset(txtdesc, "Drake");
			break;
		case OBJ_HEADLESS:
			txtset(txtdesc, "Headless");
			break;
		case OBJ_TROLL:
			txtset(txtdesc, "Troll");
			break;
		case OBJ_MONGBAT:
			txtset(txtdesc, "Mongbat");
			break;
		case OBJ_WISP:
			txtset(txtdesc, "Wisp");
			break;
		case OBJ_HYDRA_MOUTH:
			txtset(txtdesc, "Hydra");
			break;
		case OBJ_SLIME:
			txtset(txtdesc, "Slime");
			break;
		case OBJ_FIGHTER:
			txtset(txtdesc, "Fighter");
			break;
		case OBJ_SWASHBUCKLER:
			txtset(txtdesc, "Swashbuckler");
			break;
		case OBJ_MAGE:
			txtset(txtdesc, "Mage");
			break;
		case OBJ_VILLAGER:
			txtset(txtdesc, "Villager");
			break;
		case OBJ_MERCHANT:
			txtset(txtdesc, "Merchant");
			break;
		case OBJ_CHILD:
			txtset(txtdesc, "Child");
			break;
		case OBJ_GUARD:
			txtset(txtdesc, "Guard");
			break;
		case OBJ_JESTER:
			txtset(txtdesc, "Jester");
			break;
		case OBJ_PEASANT:
			txtset(txtdesc, "Peasant");
			break;
		case OBJ_FARMER:
			txtset(txtdesc, "Farmer");
			break;
		case OBJ_MUSICIAN:
			txtset(txtdesc, "Musician");
			break;
		case OBJ_WOMAN:
			txtset(txtdesc, "Woman");
			break;
		case OBJ_CAT:
			txtset(txtdesc, "Cat");
			break;

		case OBJ_LORD_BRITISH:
			txtset(txtdesc, "Lord British");
			break;
		case OBJ_AVATAR:
			txtset(txtdesc, "A player");
			break;
		case OBJ_DRAGON:
			txtset(txtdesc, "Dragon");
			break;
		case OBJ_SHIP:
			txtset(txtdesc, "A ship");
			break;
		case OBJ_SILVER_SERPENT:
			txtset(txtdesc, "Silver serpent");
			break;
		case OBJ_CYCLOPS:
			txtset(txtdesc, "Cyclops");
			break;
		//case OBJ_HYDRA_ARMS:
			//txtset(txtdesc, "");
			//break;
		case OBJ_GIANT_SCORPION:
			txtset(txtdesc, "Giant scorpion");
			break;
		case OBJ_GIANT_ANT:
			txtset(txtdesc, "Giant ant");
			break;
		case OBJ_COW:
			txtset(txtdesc, "Cow");
			break;
		case OBJ_ALLIGATOR:
			txtset(txtdesc, "Alligator");
			break;
		case OBJ_HORSE:
			txtset(txtdesc, "Horse");
			break;

		case OBJ_CHEST:
			txtset(txtdesc, "A chest");
			break;
		case OBJ_WEB:
			txtset(txtdesc, "A web");
			break;

		default:
			txtset(txtdesc, "Something (???)");
			break;
		}

		//txt* tt22 = txtnew();
		//txtnumint(tt22, type);
		//LOGadd(tt22);
	}
}

unsigned short objgettype(unsigned short type,unsigned char dir,unsigned char frm){
  switch (type) {
  case OBJ_SKIFF: 
    return type+(dir<<10);
  case OBJ_SHIP: 
    return type+((dir*2+9)<<10);
  case OBJ_GARGOYLE_WINGED: 
    return type+((dir*12)<<10)+(frm<<12)+3*1024;
  case OBJ_GARGOYLE_WINGLESS:
    return type+((dir*3)<<10)+(frm<<10);
  case OBJ_CYCLOPS: 
    return type+(dir<<13)+(frm<<12)+3*1024;
  case OBJ_DRAGON:
    return type+(dir<<11);
  case OBJ_BALLOON:
    return type+3*1024;//hot air balloon
  }
  switch (obji[sprlnk[type]].v4) {
    case 1: //"avatar"
      return type+(dir<<12)+((frm&3)<<10);
    case 4: //"rabbit"
      return type+(dir<<10);
    case 6: //"drake"
      return type+(dir<<11);
    case 2: //"rat" type
    case 8: //"ant" type
      return type+(dir<<11)+(frm<<10);
  }
  // normal case:
  return type+(frm<<10);
}

unsigned char objgetnextframe(unsigned short type,unsigned char frm){
  switch (type) {
    case OBJ_GARGOYLE_WINGED:
      frm++; if (frm>2) frm=0;
      return frm;
    case OBJ_GARGOYLE_WINGLESS:
      frm++; if (frm>2) frm=0;
      return frm;
    case OBJ_CYCLOPS:
      frm++; if (frm>1) frm=0;
  }
  switch (obji[sprlnk[type]].v4) {
    case 1://"avatar" type
      if (frm&128){//special frame check
	frm=0; return frm;
      }
      frm++;
      if (frm>2){
	frm=1+128; return frm;
      }
      return frm;
    case 2://"rat" type
    case 8: //"ant" type
      frm++; if (frm>1) frm=0;
      return frm;
  }
  return frm;//return frm, mover does not support multiple frames
}


void getsound(unsigned short type,long x,long y){
//  static long z; // luteijn: I doubt static is needed.
  long z;
  static unsigned char SNDticktock_done=FALSE;
  static unsigned char SNDfire_done=FALSE;

  if (!vischeck[x][y]) return;
  if (int(ett)==int(ett-et)){
    SNDticktock_done=FALSE;
    SNDfire_done=FALSE;
    return;
  };

  switch (type&1023) {
    case OBJ_FOUNTAIN:
      soundplay2(u6osound[3],u6osound_volume[3]);
      return;
    case OBJ_PROTECTION_FIELD:
      soundplay2(u6osound[33],u6osound_volume[33]);
      return;
    case OBJ_ELECTRIC_FIELD:   
	  // s222 new sound for electric field
      //soundplay2(u6osound[5],u6osound_volume[5]);
	  if (soundn1)
		soundplay2(u6osound[SOUND_FIELDN1],u6osound_volume[SOUND_FIELDN1]);
	  else
		soundplay2(u6osound[SOUND_FIELD],u6osound_volume[SOUND_FIELD]);
      return;
    case OBJ_LOG_SAW:
      soundplay2(u6osound[15],u6osound_volume[15]);
      return;
    case OBJ_CLOCK:
      if (type&1024) { // one part to tick is enough..
	if (SNDticktock_done==FALSE){
	  soundplay2(u6osound[17],u6osound_volume[17]);
	  SNDticktock_done=TRUE;
	}

      }
      return;
      /* luteijn: these fire checks are in a particular order to allow to fall through properly */
    case OBJ_COOKFIRE:
      if ((!(type&(5<<10))) && (!(type&(11<<10)))) {
	return;
	  }
    case OBJ_CAMPFIRE:
    case OBJ_FIREPLACE:
      if (!(type&1024)) { //doused so no sound
	return; 
      }
    case OBJ_BRAZIER:
      if (!(type>>10)) {// this also makes holy braziers sound
        return;
      } 
    case OBJ_FIRE_FIELD:
    case OBJ_FIRE:
    case OBJ_OVEN:
    case OBJ_VOLCANO:
    case OBJ_STOVE:
    case OBJ_LAMPPOST:
      if (((type&1023)==OBJ_LAMPPOST) && (!(type&(3<<10)))) { // only lamp part is noisy
	return;
      }
      if (SNDfire_done==FALSE){
	soundplay2(u6osound[16],u6osound_volume[16]);
	SNDfire_done=TRUE;
      }
      return;
  }


  if (getsound_MOVERSOUND==0) return;

  z=0;//flap
  if ((type&1023)==344) z=1;
  if ((type&1023)==369) z=1;
  if ((type&1023)==411) z=1;
  if (z){
    static unsigned char SNDflap=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDflap==FALSE){
	soundplay2(u6osound[24],u6osound_volume[24]);
      }
      SNDflap=TRUE;
    }
    else{
      SNDflap=FALSE;
    }
  }

  if ((type&1023)==348){
    static unsigned char SNDsheep_bleet=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDsheep_bleet==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[11],u6osound_volume[11]);
      }
      SNDsheep_bleet=TRUE;
    }
    else{
      SNDsheep_bleet=FALSE;
    }
  }

  if ((type&1023)==428){
    static unsigned char SNDcow_bleet=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDcow_bleet==FALSE){
	if (rnd*16.0f<1.0f) soundplay2(u6osound[12],u6osound_volume[12]);
      }
      SNDcow_bleet=TRUE;
    }
    else{
      SNDcow_bleet=FALSE;
    }
  }

  if ((type&1023)==349){
    static unsigned char SNDdog=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdog==FALSE){
	if (rnd*4.0f<1.0f) soundplay2(u6osound[21],u6osound_volume[21]);
      }
      SNDdog=TRUE;
    }
    else{
      SNDdog=FALSE;
    }
  }

  if ((type&1023)==430){
    static unsigned char SNDhorse=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDhorse==FALSE){
	if (rnd*16.0f<1.0f) soundplay2(u6osound[22],u6osound_volume[22]);
      }
      SNDhorse=TRUE;
    }
    else{
      SNDhorse=FALSE;
    }
  }

  if ((type&1023)==358){
    static unsigned char SNDsnake=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDsnake==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[23],u6osound_volume[23]);
      }
      SNDsnake=TRUE;
    }
    else{
      SNDsnake=FALSE;
    }
  }

  if ((type&1023)==356){ //bird
    static unsigned char SNDbird_bleet=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDbird_bleet==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[13],u6osound_volume[13]);

      }
      SNDbird_bleet=TRUE;
    }
    else{
      SNDbird_bleet=FALSE;
    }
  }

  if ((type&1023)==388){//cat.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[25],u6osound_volume[25]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

  if ((type&1023)==351){//wolf.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[26],u6osound_volume[26]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

  if ((type&1023)==343){//insects.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[30],u6osound_volume[30]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

  if ((type&1023)==146){//snore.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*32.0f<1.0f) soundplay2(u6osound[34],u6osound_volume[34]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

}//getsound

void getlight(unsigned short type,long x,long y){
  if (!vischeck[x][y]) return;
  
  int z=0;
  unsigned short frame=type>>10;
  type=type&1023;
  if (frame>0) { // only if the object is ON
    switch (type) {
      case OBJ_CANDLE: 
      case OBJ_BRAZIER:
      case OBJ_CAMPFIRE:
	z=11; 
	break;
      case OBJ_CANDELABRA:
	z=13;
	break;
      case OBJ_FIREPLACE:
	if (frame&1) { // frame 2 is also off 
	  z=7; 
	}
	break;
      case OBJ_CHEST:
	if (frame==3) { //(only if magic locked)
	  z=5;
	}
	break;
      case OBJ_STOVE:
	z=7;
	break;
	break;
      case OBJ_COOKFIRE:
	if (frame==4) {
	  z=9;
	}
	break;
      case OBJ_LAMPPOST:
	if (frame=3) {
	  z=13;
	}
	break;
      case OBJ_MOONGATE:
      case OBJ_RED_GATE:
	z=13;
	break;
    }
  }
  switch (type) { // always on types 
    case OBJ_MAGIC_HELM:
    case OBJ_MAGIC_SHIELD:
    case OBJ_MAGIC_ARMOUR:
    case OBJ_GLASS_SWORD:
    case OBJ_MAGIC_BOW:
    case OBJ_SNAKE_AMULET:
    case OBJ_AMULET_OF_SUBMISSION:
    case OBJ_GEM:
    case OBJ_STAFF:
    case OBJ_LIGHTNING_WAND:
    case OBJ_FIRE_WAND:
    case OBJ_PROTECTION_RING:
    case OBJ_REGENERATION_RING:
    case OBJ_INVISIBILITY_RING:
      z=3;
      break;
    case OBJ_FIRE:
    case OBJ_OVEN:
    case OBJ_HEATSOURCE:
    case OBJ_POISON_FIELD:
    case OBJ_SLEEP_FIELD:
      z=5;
      break;
    case OBJ_PROTECTION_FIELD:
    case OBJ_WISP:
      z=7;
      break;
    case OBJ_FIRE_FIELD:
      z=9;
      break;
    case OBJ_FORCE_FIELD:
      z=11;
      break;
    case OBJ_LIGHTSOURCE:
      z=13;
      break;
  }
  if ((type>=297)&&(type<=300)){ //magically locked doors
    if (frame==13) z=5;
    if (frame==15) z=5;
  }

  switch (z) {
    case 2: LIGHTnew(x,y,(unsigned long)&ls3b,3); break;
    case 3: LIGHTnew(x,y,(unsigned long)&ls3,3); break;
    case 4: LIGHTnew(x,y,(unsigned long)&ls5b,5); break;
    case 5: LIGHTnew(x,y,(unsigned long)&ls5,5); break;
    case 7: LIGHTnew(x,y,(unsigned long)&ls7,7); break;
    case 9: LIGHTnew(x,y,(unsigned long)&ls9,9); break;
    case 11: LIGHTnew(x,y,(unsigned long)&ls11,11); break;
    case 13: LIGHTnew(x,y,(unsigned long)&ls13,13); break;
  }
}//getlight

//portraits 2.0 functions
void loadportrait(unsigned short i,surf *s){
  //s is assumed to be a valid source of the original portrait which will not be deallocated/changed
  portrait_loaded[i]=TRUE;
  portrait[i]=s;
  portrait_doublesize[i]=newsurf(112,128,SURF_SYSMEM16);
  img(portrait_doublesize[i],portrait[i]);

  portrait_halfsize[i]=newsurf(28,32,SURF_SYSMEM16);
  //resample to halfsize image using brightness correction
  cls(portrait_halfsize[i],0);
  static long x,y,z,z2,z3,z4,i2,r,g,b,zz,zz2,zz3,zz4,brightness;
  for (y=0;y<=31;y++){ for (x=0;x<=27;x++){
    z=portrait[i]->o2[y*112+x*2];
    zz=(z&31)+((z>>6)&31)+((z>>11)&31);
    z2=portrait[i]->o2[y*112+x*2+1];
    zz2=(z2&31)+((z2>>6)&31)+((z2>>11)&31);
    z3=portrait[i]->o2[y*112+x*2+56];
    zz3=(z3&31)+((z3>>6)&31)+((z3>>11)&31);
    z4=portrait[i]->o2[y*112+x*2+1+56];
    zz4=(z4&31)+((z4>>6)&31)+((z4>>11)&31);
    brightness=0;
    if (zz>brightness) brightness=zz;
    if (zz2>brightness) brightness=zz2;
    if (zz3>brightness) brightness=zz3;
    if (zz4>brightness) brightness=zz4;
    brightness=brightness*12/16;
    i2=0; if (z) i2++; if (z2) i2++; if (z3) i2++; if (z4) i2++;
    if (i2>=2){
      b=0;
      if (z) b+=(z&31);
      if (z2) b+=(z2&31);
      if (z3) b+=(z3&31);
      if (z4) b+=(z4&31);
      b/=i2;
      g=0;
      if (z) g+=((z>>5)&63);
      if (z2) g+=((z2>>5)&63);
      if (z3) g+=((z3>>5)&63);
      if (z4) g+=((z4>>5)&63);
      g/=i2;
      r=0;
      if (z) r+=((z>>11)&31);
      if (z2) r+=((z2>>11)&31);
      if (z3) r+=((z3>>11)&31);
      if (z4) r+=((z4>>11)&31);
      r/=i2;
      if ((r+b+g)<brightness){
	brightness=brightness-(r+g+b);
	brightness/=3;
	r+=brightness;
	g+=brightness;
	b+=brightness;
      }
      portrait_halfsize[i]->o2[y*28+x]=b+(g<<5)+(r<<11);
    }
  }}

}

surf *getportrait(unsigned short i){
  if (portrait_loaded[i]){
    return portrait[i];
  }
  if (portrait_requested[i]==FALSE){
    portrait_requested[i]=TRUE;
    txtsetchar(portrait_request_txt,42); txtaddshort(portrait_request_txt,i);
    NET_send(NETplayer,NULL,portrait_request_txt);
  }
  return PORTRAIT_UNAVAILABLE;
}

surf *getportrait_doublesize(unsigned short i){
  if (portrait_loaded[i]){
    return portrait_doublesize[i];
  }
  if (portrait_requested[i]==FALSE){
    portrait_requested[i]=TRUE;
    txtsetchar(portrait_request_txt,42); txtaddshort(portrait_request_txt,i);
    NET_send(NETplayer,NULL,portrait_request_txt);
  }
  return PORTRAIT_UNAVAILABLE;
}

surf *getportrait_halfsize(unsigned short i){
  if (portrait_loaded[i]){
    return portrait_halfsize[i];
  }
  if (portrait_requested[i]==FALSE){
    portrait_requested[i]=TRUE;
    txtsetchar(portrait_request_txt,42); txtaddshort(portrait_request_txt,i);
    NET_send(NETplayer,NULL,portrait_request_txt);
  }
  return PORTRAIT_UNAVAILABLE;
}


unsigned char getambientlight(unsigned short x,unsigned short y){

  if ((x>=1280)&&(y>=344)&&(x<=1336)&&(y<=394)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//top floor of keep
  if ((x>=1024)&&(y>=616)&&(x<1072)&&(y<656)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//island floor II
  if ((x>=1280)&&(y>=256)&&(x<1351)&&(y<319)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//castle floor II
  if ((x>=1242)&&(y>=512)&&(x<=1265)&&(y<=538))  {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//underground of keep

  if ((x>=1808)&&(y>=424)&&(x<1902)&&(y<527)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//kat's high level maze
  if ((x>=1280)&&(y>=395)&&(x<1341)&&(y<=432)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//2nd Floor Shop in brit

  if ((x>=1024)&&(y>=0)&&(x<1072)&&(y<48)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//spirituality shrine

  if ((x>=1780)&&(y>=256)&&(x<=2047)&&(y<=657)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//kat's high level dungeon underground

  if ((x>=1280)&&(y>=319)&&(x<=1291)&&(y<=334)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//"guardians" guild hall (underground)
  if ((x>=1024)&&(y>=512)&&(x<=1258)&&(y<=614)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//newbie dungeon
  if ((x>=1072)&&(y>=616)&&(x<=1141)&&(y<=669)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//island (underground)
  if ((x>=1292)&&(y>=319)&&(x<=1309)&&(y<=333)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//
  if ((x>=1310)&&(y>=319)&&(x<=1319)&&(y<=335)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//
  if ((x>=1320)&&(y>=319)&&(x<=1326)&&(y<=324)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//
  //if ((x>=1327)&&(y>=319)&&(x<=1358)&&(y<=343))  {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//within toth's house
  
  if ((x>=1024)&&(!((y>=256)&&(x<1280)&&(y<512)))) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//underground 
  AMBIENTLIGHT_SHOWSUN=TRUE;
  return 0;
}




void txtmakeu6ocompatible(txt* t){
  static long i,ok,x;
txtmakeu6ocompatible_loop:
  for (i=0;i<t->l;i++){
    ok=0;
    x=t->d2[i];
    if ((x>=32)&&(x<=126)) ok=1;
    if (ok==0){
      if (i!=(t->l-1)){//if not the last character, shift the rest of the string
	memcpy(&t->d2[i],&t->d2[i+1],t->l-i-1);
      }
      t->l--; t->d2[t->l]=0;
      goto txtmakeu6ocompatible_loop;
    }//ok==0
  }//i
  for (i=0;i<t->l;i++){
    x=t->d2[i];
    if (x==64) t->d2[i]=7;//@
    if (x==92) t->d2[i]=8;//"\"
  }//i
}


void STATUSMESSadd(txt *t){
  static long i,i2;
  ps->s->GetDC(&taghdc); SelectObject(taghdc,fnt1);
  if (STATUSMESSpending->l) txtaddchar(STATUSMESSpending,13);
  i2=STATUSMESSpending->l;//starting scan position
  for (i=0;i<t->l;i++){
    txtaddchar(STATUSMESSpending,t->d2[i]);
    tagxy.cx=0; tagxy.cy=0;
    GetTextExtentPoint32(taghdc,&STATUSMESSpending->d[i2],STATUSMESSpending->l-i2,&tagxy);
    if ((tagxy.cx>=1008)&&(i<(t->l-1))){
      txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46);
      txtaddchar(STATUSMESSpending,13);
      i2=STATUSMESSpending->l;
    }//tagxy.cx>=1008
  }//i
  ps->s->ReleaseDC(taghdc);
}

void STATUSMESSadd(const char *t){
  static long i,i2,i3;
  i3=strlen(t);
  ps->s->GetDC(&taghdc); SelectObject(taghdc,fnt1);
  if (STATUSMESSpending->l) txtaddchar(STATUSMESSpending,13);
  i2=STATUSMESSpending->l;//starting scan position
  for (i=0;i<i3;i++){
    txtaddchar(STATUSMESSpending,t[i]);
    tagxy.cx=0; tagxy.cy=0;
    GetTextExtentPoint32(taghdc,&STATUSMESSpending->d[i2],STATUSMESSpending->l-i2,&tagxy);
    if ((tagxy.cx>=1008)&&(i<(i3-1))){
      txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46);
      txtaddchar(STATUSMESSpending,13);
      i2=STATUSMESSpending->l;
    }//tagxy.cx>=1008
  }//i
  ps->s->ReleaseDC(taghdc);
}


// s555
void STATUSMESSadd(txt *t, int skippable) {
	static txt* t2 = txtnew();

	if (skippable)
		txtsetchar(t2, 255);

	txtadd(t2, t);
	STATUSMESSadd(t2);
}

void STATUSMESSadd(const char *t, int skippable) {
	static txt* t2 = txtnew();

	txtset(t2, t);
	STATUSMESSadd(t2, skippable);
}

void STATUSMESSadd(const char *t, int skippable, int num) {
	static txt* t2 = txtnew();
	static txt* t3 = txtnew();

	txtset(t2, t);
	txtnumint(t3, num);
	txtadd(t2, t3);
	STATUSMESSadd(t2, skippable);
}


//X returned directly! unsigned long GETSETTING_OPTION;//a number from 0-? indicating the option chosen
//if getsetting returns non-zero it succeeded
long getsetting(const char*d){
  static file *tfh;
  static txt *t=txtnew();
  static txt *t2=txtnew();
  static txt *t3=txtnew();
  static long i,i2,i3,i4,i5;
  txtset(t3,d);
  tfh=open("settings.txt");
  if (tfh->h!=HFILE_ERROR){
getsetting_readnextline:
    i3=seek(tfh);
    if (i3<lof(tfh)){
      txtfilein(t,tfh);
      if (t->l){
	txtset(t2,"{"); i=txtsearch(t,t2); txtright(t,t->l-i);
	txtset(t2,","); i=txtsearch(t,t2); txtleft(t,i-1);
	if (txtsamewithoutcase(t,t3)){
	  seek(tfh,i3);
	  txtfilein(t,tfh);
	  txtset(t2,"["); i=txtsearch(t,t2);
	  txtright(t,t->l-i);
	  txtset(t2,"]"); i=txtsearch(t,t2);
	  txtset(GETSETTING_RAW,t); txtleft(GETSETTING_RAW,i-1); //txtright(t,t->l-i);


	  //get instruction
	  txtset(t2,"{"); i=txtsearch(t,t2); txtright(t,t->l-i);
	  txtset(t2,","); i=txtsearch(t,t2); txtright(t,t->l-i);//skip name of setting

	  i3=0;//if ==1 another instruction awaits
	  txtset(t2,","); i=txtsearch(t,t2); if (i==0) i=10000;
	  txtset(t2,"}"); i2=txtsearch(t,t2); if (i2==0) i2=10000;
	  if (i2<i) i=i2; else i3=1;
	  txtset(t3,t); txtleft(t3,i-1); txtright(t,t->l-i);


	  txtset(t2,"CHOICE"); if (txtsamewithoutcase(t3,t2)){
	    i4=0;
getsetting_choice_nextchoice:
	    txtset(t2,","); i=txtsearch(t,t2); if (i==0) i=10000;
	    txtset(t2,"}"); i2=txtsearch(t,t2); if (i2==0) i2=10000;
	    if (i2<i) i=i2; else i3=1;
	    txtset(t3,t); txtleft(t3,i-1); txtright(t,t->l-i);
	    i4++;
	    if (txtsamewithoutcase(t3,GETSETTING_RAW)){
	      close(tfh); return i4;
	    }
	    if (i3) goto getsetting_choice_nextchoice;
	    close(tfh); return FALSE;
	  }//CHOICE

	  close(tfh);
	  return FALSE;
	}//txtsamewithoutcase
      }//t->l
      goto getsetting_readnextline;
    }
    close(tfh);
  }//tfh->h!=HFILE_ERROR
  return FALSE;
}

// rrr added new mode handling
void refresh(){
//  if (smallwindow){
	if (smallwindow && windowsizecyclenum == 0) {
		// the original 512 resolution mode
		if (dxrefresh) {
			if (DDRAW_display_pixelformat.dwRGBBitCount != 16) {
				static unsigned long pebx, pecx;
				pebx = (unsigned long)ps->o;
				pecx = (unsigned long)ps2->o; //ps2=newsurf(1024/2,768/2,SURF_SYSMEM);
				_asm {
					mov ebx, pebx
						mov ecx, pecx
						mov esi, 196608
						p16to32b:
					mov ax, [ebx]
						mov dx, ax
						and edx, 01111100000000000b
						shl edx, 8
						mov dx, ax
						and dx, 011111100000b
						shl dx, 5
						mov dl, al
						and dl, 011111b
						shl dl, 3
						mov[ecx], edx
						add ebx, 4
						mov di, bx
						and di, 11111111111b
						jnz p16to32b2
						add ebx, 2048
						p16to32b2:
					add ecx, 4
						dec esi
						jnz p16to32b
				} //asm
				if (NEThost) img(vs, 0, 0, ps2); else img(vs, 512, 0, ps2);
			}
			else { //16->16 512x384 dx
				static unsigned long pebx, pecx;
				pebx = (unsigned long)ps->o;
				pecx = (unsigned long)ps2->o; //img(ps2,ps);
				_asm {
					mov ebx, pebx
						mov ecx, pecx
						mov si, 384
						mov di, 512
						p16to16c:
					mov ax, [ebx]
						mov[ecx], ax
						add ebx, 4
						add ecx, 2
						dec di
						jnz p16to16c
						mov di, 512
						add ebx, 2048
						dec si
						jnz p16to16c
				} //asm
				if (NEThost) img(vs, 0, 0, ps2); else img(vs, 512, 0, ps2);
			}
		}
		else { //not dxrefresh
			if (DDRAW_display_pixelformat.dwRGBBitCount != 16) {
				img(ps4, ps);   //ps4=newsurf(1024/2,768/2,SURF_SYSMEM16);
				refresh(ps4);
			}
			else {
				img(ps2, ps);
				refresh(ps2);
			}
		}
	} 
	else if (smallwindow && windowsizecyclenum == 1) {
		// the new resolution mode
		if (dxrefresh) {
			if (DDRAW_display_pixelformat.dwRGBBitCount != 16) {
				static unsigned long pebx, pecx;
				pebx = (unsigned long)ps->o;
				pecx = (unsigned long)psnew1->o; //ps2=newsurf(1024/2,768/2,SURF_SYSMEM);
				_asm {
					mov ebx, pebx
						mov ecx, pecx
						mov esi, 1296000
						zp16to32b:
					mov ax, [ebx]
						mov dx, ax
						and edx, 01111100000000000b
						shl edx, 8
						mov dx, ax
						and dx, 011111100000b
						shl dx, 5
						mov dl, al
						and dl, 011111b
						shl dl, 3
						mov[ecx], edx
						add ebx, 4
						mov di, bx
						and di, 11111111111b
						jnz zp16to32b2
						add ebx, 2048
						zp16to32b2:
					add ecx, 4
						dec esi
						jnz zp16to32b
				} //asm
				if (NEThost) img(vs, 0, 0, psnew1); else img(vs, 512, 0, psnew1);
			}
			else { //16->16 512x384 dx
				static unsigned long pebx, pecx;
				pebx = (unsigned long)ps->o;
				pecx = (unsigned long)psnew1->o; //img(ps2,ps);
				_asm {
					mov ebx, pebx
						mov ecx, pecx
						mov si, 900
						mov di, 1440
						zp16to16c:
					mov ax, [ebx]
						mov[ecx], ax
						add ebx, 4
						add ecx, 2
						dec di
						jnz zp16to16c
						mov di, 512
						add ebx, 2048
						dec si
						jnz zp16to16c
				} //asm
				if (NEThost) img(vs, 0, 0, psnew1); else img(vs, 512, 0, psnew1);
			}
		}
		else { //not dxrefresh
		    // i think the new mode only ever executes this condition; because the copy-pasted code in the other conditons are wrong.
			if (DDRAW_display_pixelformat.dwRGBBitCount != 16) {
				//img(ps4, ps);   //ps4=newsurf(1024/2,768/2,SURF_SYSMEM16);
				//refresh(ps4);

				//img(psnew1b, ps);
				//refresh(psnew1b);

				// r222 all the graphics are (originally and still is) done in the 1024 surface; copy whats on that surface and put it on the new surface.
				// it is scaled to the area on the new/destination surface.
				img(psnew1b, ps, 0, 0, resxn1m, resyn1m);

				// s444 display worldmap on top of game playing area
				if (showworldmapn1 > 0) {
					if (updateworldmapn1) {
						updateworldmapn1 = 0;
						img(uipanelsurf[uipanelworldmap][UI_WIDGET_DEF][UI_STATE_DEF], worldmapsurfn1[worldmapindexn1]);
					}

					imguip(psnew1b, uipanelworldmap);
					imguip(psnew1b, uipanelworldmapbar);

					// s444 worldmapbar buttons
					if (worldmapindexn1 == 1)
						imguiw(psnew1b, uipanelworldmapbar, UI_WIDGET_MAPBUTTON_U6CLOTH, 1);
					else if (worldmapindexn1 == 2)
						imguiw(psnew1b, uipanelworldmapbar, UI_WIDGET_MAPBUTTON_U6P, 1);
					else if (worldmapindexn1 == 3)
						imguiw(psnew1b, uipanelworldmapbar, UI_WIDGET_MAPBUTTON_U6G, 1);
					else if (worldmapindexn1 == 4)
						imguiw(psnew1b, uipanelworldmapbar, UI_WIDGET_MAPBUTTON_U6RUNE, 1);

					if (uihover) {
						if (hituipaneli == uipanelworldmapbar) {
							if (hituiwidgeti < 0)
								hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

							if (hituiwidgeti > 0)
								img0(psnew1b, uipanelx[hituipaneli][hituiwidgeti][UI_STATE_DEF], uipanely[hituipaneli][hituiwidgeti][UI_STATE_DEF], uiwidgetimgsurf[UI_IMGI_HOVER][1]);
						}
					}
				}

				refresh(psnew1b);
			}
			else {
				img(psnew1, ps);
				refresh(psnew1);
			}
		}
	}else{ //full screen
	// the original full screen / 1024 resolution mode
    if (dxrefresh){
      if (DDRAW_display_pixelformat.dwRGBBitCount!=16){ //16->32 1024x768 dx
	static unsigned long pebx,pecx;
	pebx=(unsigned long)ps->o;
	pecx=(unsigned long)ps3->o;
	_asm{
	  mov ebx,pebx
	    mov ecx,pecx
	    mov esi,786432
	    p16to32:
	    mov ax,[ebx]
	    mov dx,ax
	    and edx,01111100000000000b
	    shl edx,8
	    mov dx,ax
	    and dx,011111100000b
	    shl dx,5
	    mov dl,al
	    and dl,011111b
	    shl dl,3
	    mov [ecx],edx
	    add ebx,2
	    add ecx,4
	    dec esi
	    jnz p16to32
	} //asm
	img(vs,0,0,ps3);
      }else{
	img(vs,0,0,ps); //16->16 1024x768 dx
      }
    }else{ //no dxrefresh
      refresh(ps); //16->? 1024x768
    }
  }
}//refresh()

//screen log
void scrlog(const char*d){
  static txt *t=txtnew();
  static unsigned long i;
  txtset(t,d);
  LOGadd(t);

  if (SCRLOG_FILEONLY==FALSE){
    memcpy(ps->o,&ps->o2[16384],1540096); 
    for (i=770048;i<786432;i++){
      ps->o2[i]=(2<<11)+(27<<5)+18;
    }
    txtfnt=systemfont;
    txtout(ps,0,752,t);
    if (intro_ultimavi){
      img(ps,741,0,intro_ultimavi2);
      img0(ps,741,0,intro_ultimavi);
    }
  }

  refresh();
}

// r222 initialize new resolution mode stuff; read resolution from settings.txt (only the Y is used; everything else is computed)
void newmodeinit() {
	//partyresscale = (double) partyresxo / partyresxz;
	//int i2;

	// rrr
	/*
	resxn1w = getsetting("WINDOW_RESOLUTION_X");
	resxn1w = (unsigned int)txtnum(GETSETTING_RAW);
	//resyn1w = getsetting("WINDOW_RESOLUTION_Y");
	//resyn1w = txtnum(GETSETTING_RAW);

	resxn1m = resxn1w - 260;
	resyn1m = resxn1m * 3 / 4;
	respn1m = resxn1m * resyn1m;

	resyn1w = resyn1m;
	*/
	
	int resysettingoption;
	int uiscalesettingoption;
	int resysettingraw;

	resysettingoption = getsetting("WINDOW_RESOLUTION_Y");
	if (resysettingoption <= 1) {
		resysettingoption = 1;
		resysettingraw = desktop_rect.bottom - 100;
	} else //if (resysettingoption > 1)
		resysettingraw = (unsigned int)txtnum(GETSETTING_RAW);

	resyn1w = resysettingraw;
	if (resyn1w > desktop_rect.bottom) {
		resyn1w = desktop_rect.bottom - 100;
	}

	// scale new sidebar if it does not fit
	//txt *t = txtnew();
	//txtnumint(t, uiscalesettingoption);
	//LOGadd(t);
	uiscalesettingoption = getsetting("UI_SIDEBAR_SCALE");
	if ((uiscalesettingoption == 1) || (uiscalesettingoption == 3)) {
		uiscaling = 0;
		uipanelsizex[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF] = 260;
	} else if (uiscalesettingoption == 2) {
		if (resyn1w < 840) {
			uiscaling = 1;
			uiscalex = (float)resyn1w / 840;
			uiscaley = uiscalex;
			uipanelsizex[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF] = 260 * uiscalex;
		}
		else {
			uiscaling = 0;
			uipanelsizex[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF] = 260;
		}
	} else {
		uiscaling = 1;
		uiscalex = (float) ((unsigned int)txtnum(GETSETTING_RAW)) / 100;
		uiscaley = uiscalex;
		uipanelsizex[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF] = 260 * uiscalex;
	}

	//resyn1w = resysettingraw;
	resxn1w = resyn1w * 4 / 3 + uipanelsizex[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF];

	if (resxn1w > desktop_rect.right) {
		resxn1w = desktop_rect.right;
		resyn1w = (resxn1w - uipanelsizex[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF]) * 3 / 4;
	}

	resxn1m = resxn1w - uipanelsizex[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF];
	resyn1m = resyn1w;
	respn1m = resxn1m * resyn1m;



	/*
		if (resysettingoption == 2) {
			resysettingraw = (unsigned int)txtnum(GETSETTING_RAW);

			//if ((desktop_rect.right>1024)&&(desktop_rect.bottom>768)){
			resxn1w = resysettingraw * 4 / 3 + 260;
			if ((resxn1w <= desktop_rect.right) && (resyn1w <= desktop_rect.bottom)) {

				resxn1m = resxn1w - 260;
				resyn1m = resxn1m * 3 / 4;
				respn1m = resxn1m * resyn1m;

				resyn1w = resyn1m;
			} else
				resysettingoption = 1;
		}

		if (resysettingoption == 1) {
			resyn1w = desktop_rect.bottom - 100;
			resxn1w = resyn1w * 4 / 3 + 260;

			if (resxn1w > desktop_rect.right) {
				resxn1w = desktop_rect.right;
				resyn1w = (resxn1w - 260) * 3 / 4;
			}

			resxn1m = resxn1w - 260;
			resyn1m = resyn1w;
			respn1m = resxn1m * resyn1m;
		}

	// scale new sidebar if it does not fit
	resyn1w = resysettingraw;
	if (resyn1w < 840) {
		uiscaling = 1;
		uiscalex = (float) resyn1w / 840;
		uiscaley = uiscalex;

		resyn1w = desktop_rect.bottom - 100;
		resxn1w = resyn1w * 4 / 3 + 260;

		if (resxn1w > desktop_rect.right) {
			resxn1w = desktop_rect.right;
			resyn1w = (resxn1w - 260) * 3 / 4;
		}

		resxn1m = resxn1w - 260;
		resyn1m = resyn1w;
		respn1m = resxn1m * resyn1m;
	}
		*/

	/*
	txtset(t, "RES ");
	txtnumint(t2, resxn1w);
	txtadd(t, t2);
	LOGadd(t);

	txtset(t, "RES Y ");
	txtnumint(t2, resyn1w);
	txtadd(t, t2);
	LOGadd(t);

	txtset(t, "RES X2 ");
	txtnumint(t2, resxn1m);
	txtadd(t, t2);
	LOGadd(t);

	txtset(t, "RES Y2 ");
	txtnumint(t2, resyn1m);
	txtadd(t, t2);
	LOGadd(t);
	*/

	psnew1 = newsurf(resxn1w, resyn1w, SURF_SYSMEM);
	psnew1b = newsurf(resxn1w, resyn1w, SURF_SYSMEM16); //to temporarily convert to 16bit?
	//pspartytemp=newsurf(256,256,SURF_SYSMEM16);
	//pspartynew=newsurf(resxn1m,256,SURF_SYSMEM16);

	static RECT clrect;
	clrect.top = 0; clrect.left = 0; clrect.bottom = resyn1w; clrect.right = resxn1w;
	AdjustWindowRect(&clrect, WS_OVERLAPPED | WS_CAPTION | WS_BORDER, FALSE);

	hWnd4 = CreateWindow(szWindowClass, window_name, WS_OVERLAPPED | WS_CAPTION | WS_BORDER,
		0, 0, clrect.right - clrect.left, clrect.bottom - clrect.top, NULL, NULL, hInst, NULL);

	// s555
	if (!enhanceclientn1) {
		enhancen1 = 0;
		soundn1 = 0;
		combatsoundn1 = 0;
	}
/*
	// r999
	panelx[0] = resxn1m;
	panely[0] = 0;
	panelx2[0] = resxn1w;
	panely2[0] = resyn1m;
	panelscalex[0] = (float) 260 / (panelx2[0] - panelx[0]);
	panelscaley[0] = (float) resyn1m / panely2[0];
	panelsurf[0] = newsurf(260, resyn1m, SURF_SYSMEM16);

	panelsideui = panelcount++;
	panelnew[panelsideui].offset_x = resxn1m;
	panelnew[panelsideui].offset_y = 0;
	panelnew[panelsideui].default_size_x = 260;
	panelnew[panelsideui].default_size_y = 260 + 51 + 51 + 51 + 51;
	panelnew[panelsideui].size_x = panelnew[panelsideui].default_size_x;
	panelnew[panelsideui].size_y = panelnew[panelsideui].default_size_y;
	panelnew[panelsideui].graphic = newsurf(panelnew[panelsideui].size_x, panelnew[panelsideui].size_y, SURF_SYSMEM16);
	panelnew[panelsideui].scaling = FALSE;

	if (panelnew[panelsideui].scaling) {
		panelnew[panelsideui].scale_x = (float) panelnew[panelsideui].default_size_x / panelnew[panelsideui].size_x;
		panelnew[panelsideui].scale_y = (float) panelnew[panelsideui].default_size_y / panelnew[panelsideui].size_y;
	}


	// r999 new
	int uipi, uiwi, uisi;
	int uiscalex, uiscaley;
	int i2;

	uiscalex = UI_NOSCALE;
	uiscaley = UI_NOSCALE;

	uipanelsidebar = uipanelcount++;
	uipi = uipanelsidebar;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = resyn1m;
	uipanelx[uipi][uiwi][uisi] = resxn1w - uipanelsizex[uipi][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = 0;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);

	img(uipanelsurf[uipi][uiwi][uisi], statusbar_b255);
	
	// r999 new
	uipanelpartymember0= uipanelcount++;
	uipi = uipanelpartymember0;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 256;
	uipanelsizey[uipi][uiwi][uisi] = 256;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + 2;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = party_frame[0]->graphic;


	for (int i = 0; i < partyframenewmax; i++) {
		//party_surf[i] = newsurf(256, 256, SURF_SYSMEM16);
		//party_surf[i] = newsurf(partyresxz, partyresxz, SURF_SYSMEM16);
		party_frame_new[i] = FRMnew();
		//party_frame_new[i]->graphic = party_surf[i];
		//party_frame_new[i]->offset_x = resxn1m + 2; //startup offset off screen
		//party_frame_new[i]->offset_y = 0 + 2; //startup offset off screen
		//party_frame_new[i]->graphic_use0pixel = TRUE;
		//party_frame_new[i]->mouse_over_transparent = TRUE;
		//if (i2 = getsetting("INVENTORYWINDOW_TRANSPARENCYLEVEL")) party_frame[i]->mouse_over_transparent = i2 - 1;

		//party_frame_new[i]->offset_x = resxn1m + 2;
		//party_frame_new[i]->offset_y = 0 + 2;
		party_frame_new[i]->offset_x = 2;
		party_frame_new[i]->offset_y = 2;
		party_frame_new[i]->default_size_x = 256;
		party_frame_new[i]->default_size_y = 256;
		//party_frame_new[i]->size_x = party_frame_new[i]->default_size_x;
		//party_frame_new[i]->size_y = party_frame_new[i]->default_size_y;
		party_frame_new[i]->graphic = NULL;
		party_frame_new[i]->display = FALSE;
		party_frame_new[i]->active = FALSE;
		party_frame_new[i]->no_focus = TRUE;
		party_frame_new[i]->move = FALSE;
		party_frame_new[i]->scaling = FALSE;

		if (party_frame_new[i]->scaling) {
			party_frame_new[i]->scale_x = (float) party_frame_new[i]->default_size_x / party_frame_new[i]->size_x;
			party_frame_new[i]->scale_y = (float) party_frame_new[i]->default_size_y / party_frame_new[i]->size_y;
		}

		/*
		party_spellbook_surf[i] = newsurf(245, 173, SURF_SYSMEM16);
		party_spellbook_frame[i] = FRMnew();
		party_spellbook_frame[i]->graphic = party_spellbook_surf[i];
		party_spellbook_frame[i]->offset_x = 4096; //startup offset off screen
		party_spellbook_frame[i]->offset_y = 0; //startup offset off screen
		party_spellbook_frame[i]->mouse_over_transparent = TRUE;
		if (i2 = getsetting("SPELLBOOKWINDOW_TRANSPARENCYLEVEL")) party_spellbook_frame[i]->mouse_over_transparent = i2 - 1;
		spellbook_page[i] = 1; //default to first page (0=can't display spellbook, no spells)
		spellbook_flags[i] = 0;
		*/
/*
	}

	// r444 init for minimap
	//minimapnewx = resxn1m + 2;
	//minimapnewy = resyn1w-256-2;
	minimapnewx = 2;
	minimapnewy = panely2[0] - 256 - 2;

	panelminimap = panelcount++;
	panelnew[panelminimap].offset_x = resxn1m + 2;
	panelnew[panelminimap].offset_y = resyn1w-256-2;
	panelnew[panelminimap].default_size_x = 256;
	panelnew[panelminimap].default_size_y = 256;
	panelnew[panelminimap].size_x = panelnew[panelminimap].default_size_x;
	panelnew[panelminimap].size_y = panelnew[panelminimap].default_size_y;
	panelnew[panelminimap].graphic = newsurf(panelnew[panelminimap].size_x, panelnew[panelminimap].size_y, SURF_SYSMEM16);
	panelnew[panelminimap].scaling = FALSE;

	if (panelnew[panelminimap].scaling) {
		panelnew[panelminimap].scale_x = (float) panelnew[panelminimap].default_size_x / panelnew[panelminimap].size_x;
		panelnew[panelminimap].scale_y = (float) panelnew[panelminimap].default_size_y / panelnew[panelminimap].size_y;
	}

	// r999 new
	uipanelminimap = uipanelcount++;
	uipi = uipanelminimap;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 256;
	uipanelsizey[uipi][uiwi][uisi] = 256;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = (uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelsidebar][uiwi][uisi]) - (uipanelsizey[uipi][uiwi][uisi] + 2);
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	uipanelsurf[uipi][uiwi][uisi] = minimap_surf_new;


	if (minimaptype == 1) {
		minimaptilexstart = 1;
		minimaptilexend = 3;
		minimaptileystart = 1;
		minimaptileyend = 3;
		minimaptilesurf = minimaptilesurf1;
		//minimapplayerx = minimapnewx+128-13;
		//minimapplayery = minimapnewy+128-24;
		minimapplayerx = 128-13;
		minimapplayery = 128-24;
	} else if (minimaptype == 2) {
		minimaptilexstart = 0;
		minimaptilexend = 4;
		minimaptileystart = 0;
		minimaptileyend = 4;
		minimaptilesurf = minimaptilesurf2;
		//minimapplayerx = minimapnewx+128-9;
		//minimapplayery = minimapnewy+128-21;
		minimapplayerx = 128-9;
		minimapplayery = 128-21;
	}



	// r666 init for actionbar
	/*
	ZeroMemory(&actionbarx, sizeof(actionbarx));
	ZeroMemory(&actionbary, sizeof(actionbary));
	ZeroMemory(&actionbuttonx, sizeof(actionbuttonx));
	ZeroMemory(&actionbuttony, sizeof(actionbuttony));
	ZeroMemory(&actiontalkx, sizeof(actiontalkx));
	ZeroMemory(&actiontalky, sizeof(actiontalky));
	ZeroMemory(&actiontalkbuttonx, sizeof(actiontalkbuttonx));
	ZeroMemory(&actiontalkbuttony, sizeof(actiontalkbuttony));
	actionbarx = (int*)malloc(sizeof(int*)*5);
//            mmyobj=(mlobj*)malloc(sizeof(object*)*5);
	*/
/*

	//actionbarx[0] = resxn1m + 2;
	//actionbary[0] = 260;
	//actionbarx[1] = actionbarx[0];
	//actionbary[1] = actionbary[0] + ACTIONBAR_HEIGHT; // 314
	actionbarx[0] = 2;
	actionbary[0] = 260;
	actionbarx[1] = actionbarx[0];
	actionbary[1] = actionbary[0] + ACTIONBAR_HEIGHT; // 314

	panelactionbar1 = panelcount++;
	panelnew[panelactionbar1].offset_x = 2;
	panelnew[panelactionbar1].offset_y = 260;
	panelnew[panelactionbar1].default_size_x = 255;
	panelnew[panelactionbar1].default_size_y = 51;
	panelnew[panelactionbar1].size_x = panelnew[panelactionbar1].default_size_x;
	panelnew[panelactionbar1].size_y = panelnew[panelactionbar1].default_size_y;
	panelnew[panelactionbar1].graphic = actionbarsurf[0];
	panelnew[panelactionbar1].scaling = FALSE;

	if (panelnew[panelactionbar1].scaling) {
		panelnew[panelactionbar1].scale_x = (float) panelnew[panelactionbar1].default_size_x / panelnew[panelactionbar1].size_x;
		panelnew[panelactionbar1].scale_y = (float) panelnew[panelactionbar1].default_size_y / panelnew[panelactionbar1].size_y;
	}

	panelactionbar2 = panelcount++;
	panelnew[panelactionbar2].offset_x = panelnew[panelactionbar1].offset_x;
	panelnew[panelactionbar2].offset_y = panelnew[panelactionbar1].offset_y + ACTIONBAR_HEIGHT;
	panelnew[panelactionbar2].default_size_x = 255;
	panelnew[panelactionbar2].default_size_y = 51;
	panelnew[panelactionbar2].size_x = panelnew[panelactionbar2].default_size_x;
	panelnew[panelactionbar2].size_y = panelnew[panelactionbar2].default_size_y;
	panelnew[panelactionbar2].graphic = actionbarsurf[1];
	panelnew[panelactionbar2].scaling = FALSE;

	if (panelnew[panelactionbar2].scaling) {
		panelnew[panelactionbar2].scale_x = (float) panelnew[panelactionbar2].default_size_x / panelnew[panelactionbar2].size_x;
		panelnew[panelactionbar2].scale_y = (float) panelnew[panelactionbar2].default_size_y / panelnew[panelactionbar2].size_y;
	}

	actionbuttonsizex = 51;
	actionbuttonsizey = 51;
	
	for (int i2 = 0; i2 < ACTIONBAR_MAX; i2++) {
		actionbuttonx[i2][0] = actionbarx[i2];
		actionbuttony[i2][0] = actionbary[i2];
		for (int i = 1; i < ACTIONBUTTON_MAX; i++) {
			actionbuttonx[i2][i] = actionbuttonx[i2][i-1]+actionbuttonsizex;
			actionbuttony[i2][i] = actionbary[i2];
		}
	}

	// r999 new
	uipanelactionbar1 = uipanelcount++;
	uipi = uipanelactionbar1;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + 260;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = actionbarsurf[0];

	uipanelactionbar2 = uipanelcount++;
	uipi = uipanelactionbar2;
	uipanelwidgetcount[uipi] = 1;


	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactionbar1][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactionbar1][uiwi][uisi] + uipanelsizey[uipanelactionbar1][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = actionbarsurf[1];

	//actionbuttonsizex = 51;
	//actionbuttonsizey = 51;

	uipi = uipanelactionbar1;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = actionbarsurf[1];
		uiwi = ++uipanelwidgetcount[uipi];
	}

	uipi = uipanelactionbar2;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = actionbarsurf[1];
		uiwi = ++uipanelwidgetcount[uipi];
	}



	// r666 init for actiontalk
	actiontalkx[0] = actionbarx[0];
	actiontalky[0] = actionbary[1] + ACTIONBAR_HEIGHT;
	actiontalkx[1] = actionbarx[0];
	actiontalky[1] = actiontalky[0] + ACTIONBAR_HEIGHT;

	actiontalkbuttonsizex = 51;
	actiontalkbuttonsizey = 26;

	i2 = 0;
	for (int i3 = 0; i3 < ACTIONTALKBAR_MAX; i3++) {
		actiontalkbuttonx[i3][0] = actiontalkx[i3];
		actiontalkbuttony[i3][0] = actiontalky[i3];
		i2 = 0;
		for (int i = 1; i < ACTIONTALKBUTTON_MAX; i++) {
			if (i % 2 == 0) {
				i2++;
				actiontalkbuttony[i3][i] = actiontalky[i3];
			}
			else
				actiontalkbuttony[i3][i] = actiontalky[i3] + actiontalkbuttonsizey;

			actiontalkbuttonx[i3][i] = actiontalkx[i3] + (actiontalkbuttonsizex * i2);
		}
	}

	//actiontalkbuttonx[8] = actiontalkx + (actiontalkbuttonsizex * 4);
	//actiontalkbuttony[8] = actiontalky;


	// r999 new
	uipanelactiontalkbar1 = uipanelcount++;
	uipi = uipanelactiontalkbar1;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + 260 + 52 + 52;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = actiontalksurf[0];

	uipanelactiontalkbar2 = uipanelcount++;
	uipi = uipanelactiontalkbar2;
	uipanelwidgetcount[uipi] = 1;


	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactiontalkbar1][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][uiwi][uisi] + uipanelsizey[uipanelactiontalkbar1][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = actiontalksurf[1];

	//actionbuttonsizex = 51;
	//actionbuttonsizey = 51;

	uipi = uipanelactiontalkbar1;
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + 26;
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}

	uipi = uipanelactiontalkbar2;
	//uiwi = uipanelwidgetcount[uipi];
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + 26;
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}


	// r999 new setup ui non-default states
	// food
	uipi = uipanelactionbar2;
	uiwi = UI_WIDGET_ACTIONBUTTON_FOOD;

	// food hungry
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = actionbuttonsurf[ACTIONBUTTON_FOOD][1];

	// food full
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = actionbuttonsurf[ACTIONBUTTON_FOOD][2];

	// drop
	uipi = uipanelactionbar1;
	uiwi = UI_WIDGET_ACTIONBUTTON_DROP;

	// drop set
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = actionbuttonsurf[ACTIONBUTTON_DROP][0];

	// drop up
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = actionbuttonsurf[ACTIONBUTTON_DROP][1];

	// drop down
	uisi = 3;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = actionbuttonsurf[ACTIONBUTTON_DROP][2];

	// drop left
	uisi = 4;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = actionbuttonsurf[ACTIONBUTTON_DROP][3];

	// drop right
	uisi = 5;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = actionbuttonsurf[ACTIONBUTTON_DROP][4];
	*/
/*
*/

	// r999 new init ui data
	inituidatan1();


	// r777
	int i = U6OK[U6OK_QUIT][0];
	keyon[i] = FALSE;
	key[i] = FALSE;
	key_gotrelease[i] = TRUE;

}

// r999 new init ui data
void inituidatan1() {
	// r999 new
	int uipi, uiwi, uisi;
	float scalex, scaley;
	int i2;

	if (uiscaling) {
		scalex = uiscalex;
		scaley = uiscaley;
	} else {
		scalex = UI_NOSCALE;
		scaley = UI_NOSCALE;
	}

	uipanelsidebar = uipanelcount++;
	uipi = uipanelsidebar;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	//uipanelsizex[uipi][uiwi][uisi] = 260; // set in newmodeinit()
	uipanelsizey[uipi][uiwi][uisi] = resyn1m;
	//if (uiscaling) // not needed -- it's already scaled
	//	applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = resxn1w - uipanelsizex[uipi][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = 0;
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);

	img(uipanelsurf[uipi][uiwi][uisi], statusbar_b255);
	
	// r999 new
	// party member inventory parent
	uipanelpartymemberparent = uipanelcount++;
	uipi = uipanelpartymemberparent;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 260;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

	// party member0 inventory
	uipanelpartymember0 = uipanelcount++;
	uipi = uipanelpartymember0;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 256;
	uipanelsizey[uipi][uiwi][uisi] = 256;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelpartymemberparent][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelpartymemberparent][uiwi][uisi] + 2;
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = party_frame[0]->graphic;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);

	// mostly obsolete for the new mode
	for (int i = 0; i < partyframenewmax; i++) {
		//party_surf[i] = newsurf(256, 256, SURF_SYSMEM16);
		//party_surf[i] = newsurf(partyresxz, partyresxz, SURF_SYSMEM16);
		party_frame_new[i] = FRMnew();
		//party_frame_new[i]->graphic = party_surf[i];
		//party_frame_new[i]->offset_x = resxn1m + 2; //startup offset off screen
		//party_frame_new[i]->offset_y = 0 + 2; //startup offset off screen
		//party_frame_new[i]->graphic_use0pixel = TRUE;
		//party_frame_new[i]->mouse_over_transparent = TRUE;
		//if (i2 = getsetting("INVENTORYWINDOW_TRANSPARENCYLEVEL")) party_frame[i]->mouse_over_transparent = i2 - 1;

		//party_frame_new[i]->offset_x = resxn1m + 2;
		//party_frame_new[i]->offset_y = 0 + 2;
		party_frame_new[i]->offset_x = 2;
		party_frame_new[i]->offset_y = 2;
		party_frame_new[i]->default_size_x = 256;
		party_frame_new[i]->default_size_y = 256;
		//party_frame_new[i]->size_x = party_frame_new[i]->default_size_x;
		//party_frame_new[i]->size_y = party_frame_new[i]->default_size_y;
		party_frame_new[i]->graphic = NULL;
		party_frame_new[i]->display = FALSE;
		party_frame_new[i]->active = FALSE;
		party_frame_new[i]->no_focus = TRUE;
		party_frame_new[i]->move = FALSE;
		party_frame_new[i]->scaling = FALSE;

		if (party_frame_new[i]->scaling) {
			party_frame_new[i]->scale_x = (float) party_frame_new[i]->default_size_x / party_frame_new[i]->size_x;
			party_frame_new[i]->scale_y = (float) party_frame_new[i]->default_size_y / party_frame_new[i]->size_y;
		}

		/*
		party_spellbook_surf[i] = newsurf(245, 173, SURF_SYSMEM16);
		party_spellbook_frame[i] = FRMnew();
		party_spellbook_frame[i]->graphic = party_spellbook_surf[i];
		party_spellbook_frame[i]->offset_x = 4096; //startup offset off screen
		party_spellbook_frame[i]->offset_y = 0; //startup offset off screen
		party_spellbook_frame[i]->mouse_over_transparent = TRUE;
		if (i2 = getsetting("SPELLBOOKWINDOW_TRANSPARENCYLEVEL")) party_spellbook_frame[i]->mouse_over_transparent = i2 - 1;
		spellbook_page[i] = 1; //default to first page (0=can't display spellbook, no spells)
		spellbook_flags[i] = 0;
		*/
	}


	// r999 new
	uipanelminimap = uipanelcount++;
	uipi = uipanelminimap;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 256;
	uipanelsizey[uipi][uiwi][uisi] = 256;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = (uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelsidebar][uiwi][uisi]) - (uipanelsizey[uipi][uiwi][uisi] + 2);
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	if (uiscaling)
		uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	else
		uipanelsurf[uipi][uiwi][uisi] = minimap_surf_new;


	// r444 init for minimap
	if (minimaptype == 1) {
		minimaptilexstart = 1;
		minimaptilexend = 3;
		minimaptileystart = 1;
		minimaptileyend = 3;
		minimaptilesurf = minimaptilesurf1;
		//minimapplayerx = minimapnewx+128-13;
		//minimapplayery = minimapnewy+128-24;
		minimapplayerx = 128-13;
		minimapplayery = 128-24;
		minimapstepsize = 4.9f;
	} else if (minimaptype == 2) {
		minimaptilexstart = 0;
		minimaptilexend = 4;
		minimaptileystart = 0;
		minimaptileyend = 4;
		minimaptilesurf = minimaptilesurf2;
		//minimapplayerx = minimapnewx+128-9;
		//minimapplayery = minimapnewy+128-21;
		minimapplayerx = 128-9;
		minimapplayery = 128-21;
		minimapstepsize = 2.45f;
	}

	// s444 worldmap
	uipanelworldmap = uipanelcount++;
	uipi = uipanelworldmap;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	if (resyn1m < resxn1m) {
		uipanelsizex[uipi][uiwi][uisi] = resyn1m - 50;
		uipanelsizey[uipi][uiwi][uisi] = resyn1m - 50;
	} else {
		uipanelsizex[uipi][uiwi][uisi] = resxn1m - 50;
		uipanelsizey[uipi][uiwi][uisi] = resxn1m - 50;
	}

	uipanelx[uipi][uiwi][uisi] = resxn1m - uipanelsizex[uipi][uiwi][uisi] - 25; // old was 200
	if (uipanelx[uipi][uiwi][uisi] < 25)
		uipanelx[uipi][uiwi][uisi] = 25;
	uipanely[uipi][uiwi][uisi] = 25;
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);


	// s444 worldmapbar
	uipanelworldmapbar = uipanelcount++;
	uipi = uipanelworldmapbar;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 51;
	uipanelsizey[uipi][uiwi][uisi] = 255;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelworldmap][uiwi][uisi] - uipanelsizex[uipi][uiwi][uisi] - 1;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelworldmap][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+4];

	// worldmapbar widgets
	uipi = uipanelworldmapbar;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizey[uipi][uiwi][uisi] * (uiwi-1));
		uipanelscalex[uipi][uiwi][uisi] = scalex;
		uipanelscaley[uipi][uiwi][uisi] = scaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		uiwi = ++uipanelwidgetcount[uipi];
	}


	// r666 init for actionbar

	// r999 new
	// actionbar parent
	uipanelactionbarparent = uipanelcount++;
	uipi = uipanelactionbarparent;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 52 + 52 + 52;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelpartymemberparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+1];

	// actionbar1
	uipanelactionbar1 = uipanelcount++;
	uipi = uipanelactionbar1;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactionbarparent][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactionbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uipanelimgsurf[UI_IMGI_ACTIONBAR + 1]);

	// actionbar2
	uipanelactionbar2 = uipanelcount++;
	uipi = uipanelactionbar2;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactionbar1][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactionbar1][uiwi][uisi] + uipanelsizey[uipanelactionbar1][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+2];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uipanelimgsurf[UI_IMGI_ACTIONBAR + 2]);

	// optionbar1
	uipaneloptionbar1 = uipanelcount++;
	uipi = uipaneloptionbar1;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactionbar2][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactionbar2][uiwi][uisi] + uipanelsizey[uipanelactionbar2][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+3];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uipanelimgsurf[UI_IMGI_ACTIONBAR + 3]);

	//actionbuttonsizex = 51;
	//actionbuttonsizey = 51;

	// actionbar1 widgets
	uipi = uipanelactionbar1;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		if (uiscaling)
			applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = scalex;
		uipanelscaley[uipi][uiwi][uisi] = scaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
		uiwi = ++uipanelwidgetcount[uipi];
	}

	// actionbar2 widgets
	uipi = uipanelactionbar2;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		if (uiscaling)
			applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = scalex;
		uipanelscaley[uipi][uiwi][uisi] = scaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
		uiwi = ++uipanelwidgetcount[uipi];
	}

	// optionbar1 widgets
	uipi = uipaneloptionbar1;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		if (uiscaling)
			applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = scalex;
		uipanelscaley[uipi][uiwi][uisi] = scaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
		uiwi = ++uipanelwidgetcount[uipi];
	}



	// r666 init for actiontalk

	// r999 new
	// actiontalkbar parent
	uipanelactiontalkbarparent = uipanelcount++;
	uipi = uipanelactiontalkbarparent;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 52 + 52 + 52;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelpartymemberparent][uiwi][uisi] + uipanelsizey[uipanelactionbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = actiontalksurf[0];

	// actiontalkbar1
	uipanelactiontalkbar1 = uipanelcount++;
	uipi = uipanelactiontalkbar1;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactiontalkbarparent][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uipanelimgsurf[UI_IMGI_ACTIONTALKBAR + 1]);

	// actiontalkbar2
	uipanelactiontalkbar2 = uipanelcount++;
	uipi = uipanelactiontalkbar2;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactiontalkbar1][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][uiwi][uisi] + uipanelsizey[uipanelactiontalkbar1][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+2];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uipanelimgsurf[UI_IMGI_ACTIONTALKBAR + 2]);

	// actiontalkbar3
	uipanelactiontalkbar3 = uipanelcount++;
	uipi = uipanelactiontalkbar3;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	if (uiscaling)
		applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactiontalkbar2][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar2][uiwi][uisi] + uipanelsizey[uipanelactiontalkbar2][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+3];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uipanelimgsurf[UI_IMGI_ACTIONTALKBAR + 3]);

	//actionbuttonsizex = 51;
	//actionbuttonsizey = 51;

	// actiontalkbar1 widgets
	uipi = uipanelactiontalkbar1;
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		if (uiscaling)
			applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = scalex;
		uipanelscaley[uipi][uiwi][uisi] = scaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + uipanelsizey[uipi][uiwi][uisi];
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}

	// actiontalkbar2 widgets
	uipi = uipanelactiontalkbar2;
	//uiwi = uipanelwidgetcount[uipi];
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		if (uiscaling)
			applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = scalex;
		uipanelscaley[uipi][uiwi][uisi] = scaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + uipanelsizey[uipi][uiwi][uisi];
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}

	// actiontalkbar3 widgets
	uipi = uipanelactiontalkbar3;
	//uiwi = uipanelwidgetcount[uipi];
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		if (uiscaling)
			applyscaleuipanelwidget(uipi, uiwi, uisi, scalex, scaley);
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = scalex;
		uipanelscaley[uipi][uiwi][uisi] = scaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + uipanelsizey[uipi][uiwi][uisi];
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}


	// s555 obsolete
	/*
	uipaneloptioninfo = uipanelcount++;
	uipi = uipaneloptioninfo;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 16 * 5 + 10; //80
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelpartymemberparent][uiwi][uisi] + uipanelsizey[uipanelactionbarparent][uiwi][uisi] + uipanelsizey[uipanelactiontalkbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = scalex;
	uipanelscaley[uipi][uiwi][uisi] = scaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	
	updateoptioninfo();
	*/

	// r999 new setup ui non-default states
	// -- drop
	uipi = uipanelactionbar1;
	uiwi = UI_WIDGET_ACTIONBUTTON_DROP;

	// drop set
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 1][1]);

	// drop up
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][2];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 1][2]);

	// drop down
	uisi = 3;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][3];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 1][3]);

	// drop left
	uisi = 4;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][4];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 1][4]);

	// drop right
	uisi = 5;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][5];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 1][5]);

	// -- food
	uipi = uipanelactionbar2;
	uiwi = UI_WIDGET_ACTIONBUTTON_FOOD;

	// foods1 hungry
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+2][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 2][1]);

	// foods2 full
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+2][2];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 2][2]);

	// -- worldmap
	uipi = uipanelactionbar2;
	uiwi = UI_WIDGET_ACTIONBUTTON_WORLDMAP;

	// worldmaps1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+3][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 3][1]);

	// -- combatlog
	uipi = uipanelactionbar2;
	uiwi = UI_WIDGET_ACTIONBUTTON_COMBATLOG;

	// combatlogs1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+4][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 4][1]);

	// -- game enhancement
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_ENHANCE;

	// game enhancement s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+5][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 5][1]);

	// game enhancement s2
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+5][2];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 5][2]);

	// -- game sound enhancement
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_SOUND;

	// game sound enhancement s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+6][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 6][1]);

	// game sound enhancement s2
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+6][2];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 6][2]);

	// -- combat sound enhancement
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_COMBATSOUND;

	// combat sound enhancement s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+7][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 7][1]);

	// combat sound enhancement s2
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+7][2];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 7][2]);

	// -- minimap
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_MINIMAP;

	// minimaps1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	//uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+8][1];
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][UI_STATE_DEF], uipanelsizey[uipi][uiwi][UI_STATE_DEF], SURF_SYSMEM16);
	img(uipanelsurf[uipi][uiwi][uisi], uiwidgetimgsurf[UI_IMGI_WIDGET + 8][1]);

	// -- worldmap map1
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6CLOTH;

	// map1s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+9][1];

	// -- worldmap map2
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6P;

	// map2s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+10][1];

	// -- worldmap map3
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6G;

	// map3s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+11][1];

	// -- worldmap map4
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6RUNE;

	// map4s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+12][1];

	// ui hover surfs
	int sizex, sizey;

	// hover for action buttons
	sizex = uipanelsizex[uipanelactionbar1][UI_WIDGET_ACTIONBUTTON_ATTACK][UI_STATE_DEF];
	sizey = uipanelsizey[uipanelactionbar1][UI_WIDGET_ACTIONBUTTON_ATTACK][UI_STATE_DEF];

	if (uiscaling) {
		sizex++;
		sizey++;
	}

	uihoveractionbuttonsurf = newsurf(sizex, sizey, SURF_SYSMEM16);
	img(uihoveractionbuttonsurf, uiwidgetimgsurf[UI_IMGI_WIDGET + 13][1]);

	// hover for action talk buttons
	sizex = uipanelsizex[uipanelactiontalkbar1][UI_WIDGET_ACTIONTALKBUTTON_NAME][UI_STATE_DEF];
	sizey = uipanelsizey[uipanelactiontalkbar1][UI_WIDGET_ACTIONTALKBUTTON_NAME][UI_STATE_DEF];

	if (uiscaling) {
		sizex++;
		sizey++;
	}

	uihoveractiontalkbuttonsurf = newsurf(sizex, sizey, SURF_SYSMEM16);
	img(uihoveractiontalkbuttonsurf, uiwidgetimgsurf[UI_IMGI_WIDGET + 13][2]);

}

void applyscaleuipanelwidget(int uipi, int uiwi, int uisi, float scalex, float scaley) {
	uipanelsizex[uipi][uiwi][uisi] = uipanelsizex[uipi][uiwi][uisi] * scalex;
	uipanelsizey[uipi][uiwi][uisi] = uipanelsizey[uipi][uiwi][uisi] * scaley;
}

// obsolete
void updateoptioninfo() {
	static txt *t = txtnew();
	static txt *t2 = txtnew();
	int x, y;
	surf* optioninfosurf = uipanelsurf[uipaneloptioninfo][UI_WIDGET_DEF][UI_STATE_DEF];

	img(optioninfosurf, statusbar_b255);

	txtfntoldn1 = txtfnt;
	txtfnt = systemfont;
	//txtout(ps, 0, 752, t);
	txtcol = rgb(255, 255, 255);
	x = 5;
	y = 5;

	if (showenhancehostn1) {
		if (enhancehostn1) {
			txtset(t, "Server options: enabled");
			//txtnumint(t2, enhancehostn1);
			//txtadd(t, t2);
		} else
			txtset(t, "Server options: default");

		txtout(optioninfosurf, x, y, t);
		y += 16;
	}

	if (enhanceclientn1) {
		txtset(t, "Client options: enabled");
		//txtnumint(t2, enhanceclientn1);
		//txtadd(t, t2);
	} else
		txtset(t, "Client options: disabled");

	txtout(optioninfosurf, x, y, t);
	y += 16;

	if (enhancen1) {
		txtset(t, "Game option: alternate ");
		txtnumint(t2, enhancen1);
		txtadd(t, t2);
	} else
		txtset(t, "Game option: default");

	txtout(optioninfosurf, x, y, t);
	y += 16;

	if (soundn1) {
		txtset(t, "Sound option: alternate ");
		txtnumint(t2, soundn1);
		txtadd(t, t2);
	} else
		txtset(t, "Sound option: default");

	txtout(optioninfosurf, x, y, t);
	y += 16;

	if (combatsoundn1) {
		txtset(t, "Combat sound option: alternate ");
		txtnumint(t2, combatsoundn1);
		txtadd(t, t2);
	} else
		txtset(t, "Combat sound option: default");

	txtout(optioninfosurf, x, y, t);

	txtfnt = txtfntoldn1;
}


// r999 new
int gethituipaneli(int x, int y) {
	int uipaneli = -1;
	int uipi, uiwi, uisi;
	//int uiscalex, uiscaley;

	//uiscalex = UI_NOSCALE;
	//uiscaley = UI_NOSCALE;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	for (uipi = 0; uipi < uipanelcount; uipi++) {
		if ((uipanelhitenable[uipi][uiwi][uisi] == 1)
			&& (x >= uipanelx[uipi][uiwi][uisi]) && (x < uipanelx[uipi][uiwi][uisi] + uipanelsizex[uipi][uiwi][uisi])
			&& (y >= uipanely[uipi][uiwi][uisi]) && (y < uipanely[uipi][uiwi][uisi] + uipanelsizey[uipi][uiwi][uisi])) {
			uipaneli = uipi;
			break;
		}
	}

	return uipaneli;
}

// r999 new
int gethituipanelwidgeti(int x, int y, int uipaneli) {
	int uipanelwidgeti = -1;
	int uipi, uiwi, uisi;
	//int uiscalex, uiscaley;

	//uiscalex = UI_NOSCALE;
	//uiscaley = UI_NOSCALE;

	//uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	if (uipaneli > 0)
		uipi = uipaneli;
	else
		uipi = gethituipaneli(x, y);

	for (uiwi = 1; uiwi < uipanelwidgetcount[uipi]; uiwi++) {
		if ((uipanelhitenable[uipi][uiwi][uisi] == 1)
			&& (x >= uipanelx[uipi][uiwi][uisi]) && (x < uipanelx[uipi][uiwi][uisi] + uipanelsizex[uipi][uiwi][uisi])
			&& (y >= uipanely[uipi][uiwi][uisi]) && (y < uipanely[uipi][uiwi][uisi] + uipanelsizey[uipi][uiwi][uisi])) {
			uipanelwidgeti = uiwi;
			break;
		}
	}

	return uipanelwidgeti;
}

// r999 new
int gethituipanelwidgeti(int x, int y) {
	int uipanelwidgeti = -1;
	int uipi, uisi; // uiwi, 
	//int uiscalex, uiscaley;

	//uiscalex = UI_NOSCALE;
	//uiscaley = UI_NOSCALE;

	//uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;
	uipi = gethituipaneli(x, y);
	uipanelwidgeti = gethituipanelwidgeti(x, y, uipi);

	return uipanelwidgeti;
}

// r999 new
int testhituipanel(int x, int y, int uipaneli) {
	//int uipaneli = -1;
	int uipi, uiwi, uisi;
	//int uiscalex, uiscaley;
	int testhitresult = 0;

	//uiscalex = UI_NOSCALE;
	//uiscaley = UI_NOSCALE;

	uipi = uipaneli;
	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	if ((x >= uipanelx[uipi][uiwi][uisi]) && (x < uipanelx[uipi][uiwi][uisi] + uipanelsizex[uipi][uiwi][uisi])
		&& (y >= uipanely[uipi][uiwi][uisi]) && (y < uipanely[uipi][uiwi][uisi] + uipanelsizey[uipi][uiwi][uisi])) {
		testhitresult = 1;
	}

	return testhitresult;
}


/*
//old
void inituidatan1() {
	// r999 new
	int uipi, uiwi, uisi;
	int uiscalex, uiscaley;
	int i2;

	uiscalex = UI_NOSCALE;
	uiscaley = UI_NOSCALE;

	uipanelsidebar = uipanelcount++;
	uipi = uipanelsidebar;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = resyn1m;
	uipanelx[uipi][uiwi][uisi] = resxn1w - uipanelsizex[uipi][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = 0;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);

	img(uipanelsurf[uipi][uiwi][uisi], statusbar_b255);
	
	// r999 new
	// party member inventory parent
	uipanelpartymemberparent = uipanelcount++;
	uipi = uipanelpartymemberparent;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 260;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

	// party member0 inventory
	uipanelpartymember0 = uipanelcount++;
	uipi = uipanelpartymember0;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 256;
	uipanelsizey[uipi][uiwi][uisi] = 256;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelpartymemberparent][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelpartymemberparent][uiwi][uisi] + 2;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = party_frame[0]->graphic;


	for (int i = 0; i < partyframenewmax; i++) {
		//party_surf[i] = newsurf(256, 256, SURF_SYSMEM16);
		//party_surf[i] = newsurf(partyresxz, partyresxz, SURF_SYSMEM16);
		party_frame_new[i] = FRMnew();
		//party_frame_new[i]->graphic = party_surf[i];
		//party_frame_new[i]->offset_x = resxn1m + 2; //startup offset off screen
		//party_frame_new[i]->offset_y = 0 + 2; //startup offset off screen
		//party_frame_new[i]->graphic_use0pixel = TRUE;
		//party_frame_new[i]->mouse_over_transparent = TRUE;
		//if (i2 = getsetting("INVENTORYWINDOW_TRANSPARENCYLEVEL")) party_frame[i]->mouse_over_transparent = i2 - 1;

		//party_frame_new[i]->offset_x = resxn1m + 2;
		//party_frame_new[i]->offset_y = 0 + 2;
		party_frame_new[i]->offset_x = 2;
		party_frame_new[i]->offset_y = 2;
		party_frame_new[i]->default_size_x = 256;
		party_frame_new[i]->default_size_y = 256;
		//party_frame_new[i]->size_x = party_frame_new[i]->default_size_x;
		//party_frame_new[i]->size_y = party_frame_new[i]->default_size_y;
		party_frame_new[i]->graphic = NULL;
		party_frame_new[i]->display = FALSE;
		party_frame_new[i]->active = FALSE;
		party_frame_new[i]->no_focus = TRUE;
		party_frame_new[i]->move = FALSE;
		party_frame_new[i]->scaling = FALSE;

		if (party_frame_new[i]->scaling) {
			party_frame_new[i]->scale_x = (float) party_frame_new[i]->default_size_x / party_frame_new[i]->size_x;
			party_frame_new[i]->scale_y = (float) party_frame_new[i]->default_size_y / party_frame_new[i]->size_y;
		}

		/*
		party_spellbook_surf[i] = newsurf(245, 173, SURF_SYSMEM16);
		party_spellbook_frame[i] = FRMnew();
		party_spellbook_frame[i]->graphic = party_spellbook_surf[i];
		party_spellbook_frame[i]->offset_x = 4096; //startup offset off screen
		party_spellbook_frame[i]->offset_y = 0; //startup offset off screen
		party_spellbook_frame[i]->mouse_over_transparent = TRUE;
		if (i2 = getsetting("SPELLBOOKWINDOW_TRANSPARENCYLEVEL")) party_spellbook_frame[i]->mouse_over_transparent = i2 - 1;
		spellbook_page[i] = 1; //default to first page (0=can't display spellbook, no spells)
		spellbook_flags[i] = 0;
///
	}


	// r999 new
	uipanelminimap = uipanelcount++;
	uipi = uipanelminimap;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 256;
	uipanelsizey[uipi][uiwi][uisi] = 256;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = (uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelsidebar][uiwi][uisi]) - (uipanelsizey[uipi][uiwi][uisi] + 2);
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	uipanelsurf[uipi][uiwi][uisi] = minimap_surf_new;


	// r444 init for minimap
	if (minimaptype == 1) {
		minimaptilexstart = 1;
		minimaptilexend = 3;
		minimaptileystart = 1;
		minimaptileyend = 3;
		minimaptilesurf = minimaptilesurf1;
		//minimapplayerx = minimapnewx+128-13;
		//minimapplayery = minimapnewy+128-24;
		minimapplayerx = 128-13;
		minimapplayery = 128-24;
		minimapstepsize = 4.9f;
	} else if (minimaptype == 2) {
		minimaptilexstart = 0;
		minimaptilexend = 4;
		minimaptileystart = 0;
		minimaptileyend = 4;
		minimaptilesurf = minimaptilesurf2;
		//minimapplayerx = minimapnewx+128-9;
		//minimapplayery = minimapnewy+128-21;
		minimapplayerx = 128-9;
		minimapplayery = 128-21;
		minimapstepsize = 2.45f;
	}

	// s444 worldmap
	uipanelworldmap = uipanelcount++;
	uipi = uipanelworldmap;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = resyn1m - 50;
	uipanelsizey[uipi][uiwi][uisi] = resyn1m - 50;
	uipanelx[uipi][uiwi][uisi] = 200;
	uipanely[uipi][uiwi][uisi] = 25;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);


	// s444 worldmapbar
	uipanelworldmapbar = uipanelcount++;
	uipi = uipanelworldmapbar;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 51;
	uipanelsizey[uipi][uiwi][uisi] = 255;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelworldmap][uiwi][uisi] - uipanelsizex[uipi][uiwi][uisi] - 1;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelworldmap][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+4];

	// worldmapbar widgets
	uipi = uipanelworldmapbar;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizey[uipi][uiwi][uisi] * (uiwi-1));
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		uiwi = ++uipanelwidgetcount[uipi];
	}


	// r666 init for actionbar

	// r999 new
	// actionbar parent
	uipanelactionbarparent = uipanelcount++;
	uipi = uipanelactionbarparent;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 52 + 52 + 52;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelpartymemberparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+1];

	// actionbar1
	uipanelactionbar1 = uipanelcount++;
	uipi = uipanelactionbar1;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactionbarparent][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactionbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+1];

	// actionbar2
	uipanelactionbar2 = uipanelcount++;
	uipi = uipanelactionbar2;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactionbar1][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactionbar1][uiwi][uisi] + uipanelsizey[uipanelactionbar1][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+2];

	// optionbar1
	uipaneloptionbar1 = uipanelcount++;
	uipi = uipaneloptionbar1;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactionbar2][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactionbar2][uiwi][uisi] + uipanelsizey[uipanelactionbar2][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONBAR+3];

	//actionbuttonsizex = 51;
	//actionbuttonsizey = 51;

	// actionbar1 widgets
	uipi = uipanelactionbar1;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = actionbarsurf[1];
		uiwi = ++uipanelwidgetcount[uipi];
	}

	// actionbar2 widgets
	uipi = uipanelactionbar2;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = actionbarsurf[1];
		uiwi = ++uipanelwidgetcount[uipi];
	}

	// optionbar1 widgets
	uipi = uipaneloptionbar1;
	uiwi = uipanelwidgetcount[uipi];
	for (int i = 1; i < UI_WIDGET_ACTIONBUTTON_MAX; i++) {
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 51;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * (uiwi-1));
		uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
		//uipanelsurf[uipi][uiwi][uisi] = actionbarsurf[1];
		uiwi = ++uipanelwidgetcount[uipi];
	}



	// r666 init for actiontalk

	// r999 new
	// actiontalkbar parent
	uipanelactiontalkbarparent = uipanelcount++;
	uipi = uipanelactiontalkbarparent;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 52 + 52 + 52;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelpartymemberparent][uiwi][uisi] + uipanelsizey[uipanelactionbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	//uipanelsurf[uipi][uiwi][uisi] = actiontalksurf[0];

	// actiontalkbar1
	uipanelactiontalkbar1 = uipanelcount++;
	uipi = uipanelactiontalkbar1;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactiontalkbarparent][uiwi][uisi] + 2;
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+1];

	// actiontalkbar2
	uipanelactiontalkbar2 = uipanelcount++;
	uipi = uipanelactiontalkbar2;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactiontalkbar1][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][uiwi][uisi] + uipanelsizey[uipanelactiontalkbar1][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+2];

	// actiontalkbar3
	uipanelactiontalkbar3 = uipanelcount++;
	uipi = uipanelactiontalkbar3;
	uipanelwidgetcount[uipi] = 1;

	uipanelsizex[uipi][uiwi][uisi] = 255;
	uipanelsizey[uipi][uiwi][uisi] = 51;
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelactiontalkbar2][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar2][uiwi][uisi] + uipanelsizey[uipanelactiontalkbar2][uiwi][uisi] + 1;
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+3];

	//actionbuttonsizex = 51;
	//actionbuttonsizey = 51;

	// actiontalkbar1 widgets
	uipi = uipanelactiontalkbar1;
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + 26;
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}

	// actiontalkbar2 widgets
	uipi = uipanelactiontalkbar2;
	//uiwi = uipanelwidgetcount[uipi];
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + 26;
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}

	// actiontalkbar3 widgets
	uipi = uipanelactiontalkbar3;
	//uiwi = uipanelwidgetcount[uipi];
	i2 = 0;
	for (int i = 1; i < UI_WIDGET_ACTIONTALKBUTTON_MAX; i++) {
		uiwi = uipanelwidgetcount[uipi]++;
		uipanelsizex[uipi][uiwi][uisi] = 51;
		uipanelsizey[uipi][uiwi][uisi] = 26;
		uipanelx[uipi][uiwi][uisi] = uipanelx[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + (uipanelsizex[uipi][uiwi][uisi] * i2);
		//uipanely[uipi][uiwi][uisi] = uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF];
		uipanelscalex[uipi][uiwi][uisi] = uiscalex;
		uipanelscaley[uipi][uiwi][uisi] = uiscaley;
		uipanelhitenable[uipi][uiwi][uisi] = 1;
		uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;

		if (i % 2 == 0) {
			i2++;
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF] + 26;
		} else {
			uipanely[uipi][uiwi][uisi] = uipanely[uipi][UI_WIDGET_DEF][UI_STATE_DEF];
		}

		//uiwi = ++uipanelwidgetcount[uipi];
	}


	// s555
	/*
	uipaneloptioninfo = uipanelcount++;
	uipi = uipaneloptioninfo;
	uipanelwidgetcount[uipi] = 1;

	uiwi = UI_WIDGET_DEF;
	uisi = UI_STATE_DEF;

	uipanelsizex[uipi][uiwi][uisi] = 260;
	uipanelsizey[uipi][uiwi][uisi] = 16 * 5 + 10; //80
	uipanelx[uipi][uiwi][uisi] = uipanelx[uipanelsidebar][uiwi][uisi];
	uipanely[uipi][uiwi][uisi] = uipanely[uipanelsidebar][uiwi][uisi] + uipanelsizey[uipanelpartymemberparent][uiwi][uisi] + uipanelsizey[uipanelactionbarparent][uiwi][uisi] + uipanelsizey[uipanelactiontalkbarparent][uiwi][uisi];
	uipanelscalex[uipi][uiwi][uisi] = uiscalex;
	uipanelscaley[uipi][uiwi][uisi] = uiscaley;
	uipanelhitenable[uipi][uiwi][uisi] = 0;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 0;
	uipanelsurf[uipi][uiwi][uisi] = newsurf(uipanelsizex[uipi][uiwi][uisi], uipanelsizey[uipi][uiwi][uisi], SURF_SYSMEM16);
	
	updateoptioninfo();
///

	// r999 new setup ui non-default states
	// -- drop
	uipi = uipanelactionbar1;
	uiwi = UI_WIDGET_ACTIONBUTTON_DROP;

	// drop set
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][1];

	// drop up
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][2];

	// drop down
	uisi = 3;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][3];

	// drop left
	uisi = 4;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][4];

	// drop right
	uisi = 5;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+1][5];

	// -- food
	uipi = uipanelactionbar2;
	uiwi = UI_WIDGET_ACTIONBUTTON_FOOD;

	// foods1 hungry
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+2][1];

	// foods2 full
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+2][2];

	// -- worldmap
	uipi = uipanelactionbar2;
	uiwi = UI_WIDGET_ACTIONBUTTON_WORLDMAP;

	// worldmaps1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+3][1];

	// -- combatlog
	uipi = uipanelactionbar2;
	uiwi = UI_WIDGET_ACTIONBUTTON_COMBATLOG;

	// combatlogs1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+4][1];

	// -- game enhancement
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_ENHANCE;

	// game enhancement s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+5][1];

	// game enhancement s2
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+5][2];

	// -- game sound enhancement
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_SOUND;

	// game sound enhancement s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+6][1];

	// game sound enhancement s2
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+6][2];

	// -- combat sound enhancement
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_COMBATSOUND;

	// combat sound enhancement s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+7][1];

	// combat sound enhancement s2
	uisi = 2;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+7][2];

	// -- minimap
	uipi = uipaneloptionbar1;
	uiwi = UI_WIDGET_OPTIONBUTTON_MINIMAP;

	// minimaps1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+8][1];

	// -- worldmap map1
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6CLOTH;

	// map1s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+9][1];

	// -- worldmap map2
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6P;

	// map2s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+10][1];

	// -- worldmap map3
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6G;

	// map3s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+11][1];

	// -- worldmap map4
	uipi = uipanelworldmapbar;
	uiwi = UI_WIDGET_MAPBUTTON_U6RUNE;

	// map4s1
	uisi = 1;
	uipanelusedefaultstatedata[uipi][uiwi][uisi] = 1;
	uipanelsurf[uipi][uiwi][uisi] = uiwidgetimgsurf[UI_IMGI_WIDGET+12][1];


}
*/

/*
void updatepartyframe1(FRAME* f, int partyindex, double scale) {
	static txt *t = txtnew(), *t2 = txtnew();
	//txt* t = txtnew();
	//txt* t2 = txtnew();
	int x, y, i, x2;

	i = partyindex;
	img(f->graphic, status8); //clear frame




	img0(f->graphic, 0, 0, getportrait_doublesize(tnpc->port));


	txtset(t, tnpc->name);
	x = 56 - 4 * t->l; y = 128 - 16 - 8;
	txtcol = rgb(0, 0, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 255, 255);
	if (i == 0) txtcol = namecolour;
	txtout(f->graphic, x + 1, y + 1, t);
	txtcol = rgb(255, 255, 255);

	txtset(t, "Health");
	x = 128 - 16; y = 12 - 2;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);

	x2 = tnpc->hp & 32767;
	txtnumint(t, x2);
	x = 128 - 16 + 64 - 8 + 4 - 2; y = 12 - 2;
	if (x2<100) x = x + 10;
	if (x2<10) x = x + 10;


	txtcol = rgb(168, 30, 24);

	if (tnpc->hp & 32768) txtcol = rgb(24, 168, 24);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 64, 32);
	if (tnpc->hp & 32768) txtcol = rgb(64, 255, 64);
	txtout(f->graphic, x + 1, y + 1, t);

	txtset(t, "/");
	txtnumint(t2, tnpc->hp_max);
	txtadd(t, t2);
	x = 128 - 16 + 64 - 8 + 4 + 32 - 3; y = 12 - 2;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);

	txtset(t, "Magic");
	x = 128 - 16; y = 12 - 2 + 18;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);



	txtnumint(t, tnpc->mp);
	x = 128 - 16 + 64 - 8 + 4 - 2; y = 12 - 2 + 18;
	if (tnpc->mp<100) x = x + 10;
	if (tnpc->mp<10) x = x + 10;

	txtcol = rgb(72, 72, 168);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(96, 128, 255);
	txtout(f->graphic, x + 1, y + 1, t);

	txtset(t, "/");
	txtnumint(t2, tnpc->mp_max);
	txtadd(t, t2);
	x = 128 - 16 + 64 - 8 + 4 + 32 - 3; y = 12 - 2 + 18;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);

	txtset(t, "STR  ");
	txtnumint(t2, tnpc->s);
	txtadd(t, t2);
	x = 128 - 16; y = 12 + 16 + 18;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);

	txtset(t, "DEX  ");
	txtnumint(t2, tnpc->d);
	txtadd(t, t2);
	x = 128 - 16 + 64 + 8 - 4; y = 12 + 16 + 18;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);

	txtset(t, "INT  ");
	txtnumint(t2, tnpc->i);
	txtadd(t, t2);
	x = 128 - 16; y = 12 + 16 + 18 + 18;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);

	txtset(t, "LEV  ");
	txtnumint(t2, tnpc->lev);
	txtadd(t, t2);
	x = 128 - 16 + 64 + 8 - 4; y = 12 + 16 + 18 + 18;
	txtcol = rgb(192, 126, 0);
	txtout(f->graphic, x, y, t);
	txtout(f->graphic, x + 2, y + 2, t);
	txtout(f->graphic, x + 2, y, t);
	txtout(f->graphic, x, y + 2, t);
	txtcol = rgb(255, 128 + 64 + 16, 0);
	txtout(f->graphic, x + 1, y + 1, t);


	txtset(t, "EXP ");
	txtnumint(t2, tnpc->exp);
	txtadd(t, t2);


	x = 800;
	if (tnpc->lev >= 2) x = 1600;
	if (tnpc->lev >= 3) x = 3200;
	if (tnpc->lev >= 4) x = 6400;
	if (tnpc->lev >= 5) x = 12800;
	if (tnpc->lev >= 6) x = 25600;
	if (tnpc->lev >= 7) x = 51200;
	//if (tnpc->lev >= 8) goto skiplevnext;
	if (tnpc->lev < 8) {
		txtset(t2, "/");
		txtadd(t, t2);
		txtnumint(t2, x);
		txtadd(t, t2);
	}
}
*/

/* luteijn:
 * there was some #define magic here to 'replace' loadimage everywhere else with this loadimage2 version 
 * simplified this.
 */
#ifdef loadimage
#undef loadimage
#endif
#define loadimage loadimage

surf *loadimage2(LPCSTR name){
  //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name);
}
surf *loadimage2(txt* name){
  //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name);
}
surf *loadimage2(LPCSTR name,long flags){
  //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name,flags);
}
surf *loadimage2(txt* name,long flags){
  //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name,flags);
}
#undef loadimage
#define loadimage loadimage2

