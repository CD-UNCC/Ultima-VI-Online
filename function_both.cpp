#include "function_both.h"
#include <windows.h>
#include <stdio.h>
#ifdef CONSOLE
#include <conio.h>
#endif
void function_both_init(void) {

}

void BITSadd(txt *t,unsigned long *start,unsigned long value,unsigned long nbits){
  static unsigned long *lp;
  static unsigned long shift;
  if (nbits==0) return;
  if (nbits>24){
    BITSadd(t,start,value&65535,16); value>>=16; nbits-=16;
  }
  lp=(unsigned long*)(t->d2+(*start>>3));
  shift=*start&7;
  *lp&=BITSleftmask[shift];
  *lp+=value<<shift;
  *start+=nbits;
}//BITSadd
unsigned long BITSget(txt *t,unsigned long *start,unsigned long nbits){
  static unsigned long *lp;
  static unsigned long shift,shift2;
  if (nbits==0) return 0;
  if (nbits>24) return BITSget(t,start,16)+(BITSget(t,start,nbits-16)<<16);
  lp=(unsigned long*)(t->d2+(*start>>3));
  shift=*start&7;
  *start+=nbits;
  return (*lp&BITSleftmask[shift+nbits])>>shift;
}//BITSget

void LOGadd(txt* t){
  static txt *t2=txtnew(),*t3=txtnew();

  long runtime,days,hours,mins,secs;

  char timebuf[64];

  txtset(t3,"");
  /*
     hours=ett/3600.0f;
     mins=(ett-((float)hours*3600.0f))/60.0f;
     secs=ett; secs=secs%60;

     txtnumint(t2,hours);
logadd0: if (t2->l<3){txtadd(t2," "); goto logadd0;}//hours since startup
txtadd(t3,t2); txtadd(t3,":");
txtnumint(t2,mins);
logadd1: if (t2->l<2){txtadd(t2," "); goto logadd1;}//minutes since startup
txtadd(t3,t2); txtadd(t3,":");
txtnumint(t2,secs);
logadd2: if (t2->l<3){txtadd(t2," "); goto logadd2;}//seconds since startup
txtadd(t3,t2);
*/

  runtime=(long)ett; /* luteijn: by the time this doesn't fit, I'm dead anyway */
  secs=runtime%60; mins=(runtime/60)%60; hours=(runtime/3600)%24; days=(runtime/86400); /*Like it will run for more than a day or 2*/
  _snprintf(timebuf,64,"%03d:%02d:%02d:%02d ",days,hours,mins,secs);

  txtadd(t3,timebuf);
  txtadd(t3,t);
  txtfileout(t3, log2file);
#ifdef CONSOLE
  _cprintf("%s\n",t3->d);
#endif
}


DWORD WINAPI sockets_send(LPVOID i){
sockets_send_loop_wait:
  SleepEx(8,FALSE);
sockets_send_loop:
  if (socketclient_si[(unsigned long)i]->exit_thread) goto sockets_send_close;
  if (socketclient_si[(unsigned long)i]->d[socketclient_si[(unsigned long)i]->next]->l){

    if (send(socketclient[(unsigned long)i],(const char*)&socketclient_si[(unsigned long)i]->d[socketclient_si[(unsigned long)i]->next]->d2[0],socketclient_si[(unsigned long)i]->d[socketclient_si[(unsigned long)i]->next]->l,0)==SOCKET_ERROR) goto sockets_send_close;

    socketclient_si[(unsigned long)i]->d[socketclient_si[(unsigned long)i]->next]->l=0;
    socketclient_si[(unsigned long)i]->next++;
    SleepEx(0,FALSE);
    goto sockets_send_loop;
  }
  goto sockets_send_loop_wait;
sockets_send_close:
  if (socketclient_ri[(unsigned long)i]->exit_thread==0) socketclient_ri[(unsigned long)i]->exit_thread=1;//close receiving thread
  socketclient_si[(unsigned long)i]->exit_thread=2;//set thread as closed
  if (socket_disconnect[(unsigned long)i]==0) socket_disconnect[(unsigned long)i]=1;//request socket disconnect
  ExitThread(0);
  return 0;
}

DWORD WINAPI sockets_receive(LPVOID i){
unsigned int sig_to=0;

socketclient_ri[(unsigned long)i]->x2=0;
sockets_receive_loop_wait:
  SleepEx(8,FALSE);
sockets_receive_loop:


  if (socketclient_ri[(unsigned long)i]->exit_thread) goto sockets_receive_close;

  if ((socketclient_ri[(unsigned long)i]->x4=recv(socketclient[(unsigned long)i],(char*)&socketclient_ri[(unsigned long)i]->t->d2[socketclient_ri[(unsigned long)i]->t->l],32768,0))==SOCKET_ERROR){


    socketclient_ri[(unsigned long)i]->x4=WSAGetLastError();
    if (socketclient_ri[(unsigned long)i]->x4==WSAEWOULDBLOCK){

      goto sockets_receive_loop_wait;
    }




    goto sockets_receive_close;
  }



  socketclient_ri[(unsigned long)i]->t->l+=socketclient_ri[(unsigned long)i]->x4;
  //VERIFY SIGNATURE
  if (!socketclient_verified[(unsigned long)i]){
    if (socketclient_ri[(unsigned long)i]->t->l<4){
      sig_to++;
      if (sig_to > 1000) /* ~15 seconds */
      SleepEx(15,FALSE); 
      goto sockets_receive_loop;
    }
    if ((unsigned long)socketclient_ri[(unsigned long)i]->t->dl[0]==U6O_SIGNATURE){
      socketclient_verified[(unsigned long)i]=1;
      txtright(socketclient_ri[(unsigned long)i]->t,socketclient_ri[(unsigned long)i]->t->l-4);
      SleepEx(0,FALSE);
      //MessageBox(NULL,"sockets_receive: verified signature","Ultima 6 Online",MB_OK);
      goto sockets_receive_loop;
    }


    //MessageBox(NULL,"sockets_receive: signature invalid, sending incorrect version message","Ultima 6 Online",MB_OK);

    //signature was incorrect: send old incorrect version message & close connection
    SleepEx(1000,FALSE);
    send(socketclient[(unsigned long)i],(char*)&incorrectversionmessage,9,0);
    //send again in case first message is missed
    SleepEx(1000,FALSE);
    send(socketclient[(unsigned long)i],(char*)&incorrectversionmessage,9,0);
    goto sockets_receive_close;
  }



sockets_receive_checkformessage:
  if (socketclient_ri[(unsigned long)i]->t->l>=2){
    socketclient_ri[(unsigned long)i]->x2=0;




    //enough data has arrived to know the length of the message we are reading
    if (socketclient_ri[(unsigned long)i]->t->ds[0]<=(socketclient_ri[(unsigned long)i]->t->l+2)){
      //message received
      if (socketclient_ri[(unsigned long)i]->d[socketclient_ri[(unsigned long)i]->nextfree]->bl<(socketclient_ri[(unsigned long)i]->t->ds[0]+1)){
	//free old allocated memory
	free(socketclient_ri[(unsigned long)i]->d[socketclient_ri[(unsigned long)i]->nextfree]->d);
	//extend buffer
	socketclient_ri[(unsigned long)i]->d[socketclient_ri[(unsigned long)i]->nextfree]->d=(char*)malloc(socketclient_ri[(unsigned long)i]->t->ds[0]+1);
	socketclient_ri[(unsigned long)i]->d[socketclient_ri[(unsigned long)i]->nextfree]->bl=socketclient_ri[(unsigned long)i]->t->ds[0]+1;
      }
      memcpy(&socketclient_ri[(unsigned long)i]->d[socketclient_ri[(unsigned long)i]->nextfree]->d2[0],&socketclient_ri[(unsigned long)i]->t->d2[2],socketclient_ri[(unsigned long)i]->t->ds[0]);
      //set NULL terminator of txt
      socketclient_ri[(unsigned long)i]->d[socketclient_ri[(unsigned long)i]->nextfree]->d2[socketclient_ri[(unsigned long)i]->t->ds[0]]=0;
      //set ->l (length) of txt
      socketclient_ri[(unsigned long)i]->d[socketclient_ri[(unsigned long)i]->nextfree]->l=socketclient_ri[(unsigned long)i]->t->ds[0];
      socketclient_ri[(unsigned long)i]->nextfree++;
      txtright(socketclient_ri[(unsigned long)i]->t,socketclient_ri[(unsigned long)i]->t->l-(socketclient_ri[(unsigned long)i]->t->ds[0]+2));

      //MessageBox(NULL,"sockets_receive: received a message","Ultima 6 Online",MB_OK);

      SleepEx(0,FALSE);
      goto sockets_receive_checkformessage;//another message's data may still be in the buffer!
    }
  }
  else { //timeout  there was endless loop bug in some cases this should prevent it.
    socketclient_ri[(unsigned long)i]->x2++;
    SleepEx(5,FALSE);
    if (socketclient_ri[(unsigned long)i]->x2 > 1000) {
      goto sockets_receive_close;
    }
  }
  //not enough data was received to complete the message
  SleepEx(0,FALSE);
  goto sockets_receive_loop;
  //invalid signature or error
sockets_receive_close:
  if (socketclient_si[(unsigned long)i]->exit_thread==0) socketclient_si[(unsigned long)i]->exit_thread=1;//close sending thread
  socketclient_ri[(unsigned long)i]->exit_thread=2;//set thread as closed
  if (socket_disconnect[(unsigned long)i]==0) socket_disconnect[(unsigned long)i]=1;//request socket disconnect
  ExitThread(0);
  return 0;

}



DWORD WINAPI sockets_disconnect(LPVOID i){
  if (socketclient_si[(unsigned long)i]->exit_thread==0) socketclient_si[(unsigned long)i]->exit_thread=1;
  if (socketclient_ri[(unsigned long)i]->exit_thread==0) socketclient_ri[(unsigned long)i]->exit_thread=1;
  shutdown(socketclient[(unsigned long)i],SD_RECEIVE|SD_SEND);
  SleepEx(4096,NULL);
  closesocket(socketclient[(unsigned long)i]);

  //wait for socket send/receive threads to close (if they haven't already)
  socket_disconnect_wait[(unsigned long)i]=0;
socket_disconnect_send_close_wait:
  socket_disconnect_wait[(unsigned long)i]++;
  if (socket_disconnect_wait[(unsigned long)i]<=16){//can wait up to 16 sec
    if (socketclient_si[(unsigned long)i]->exit_thread!=2){SleepEx(1024,NULL); goto socket_disconnect_send_close_wait;}
  }
  if (socketclient_si[(unsigned long)i]->exit_thread!=2){
    TerminateThread((void*)socketclient_si[(unsigned long)i]->thread_id,0);
  }

  socket_disconnect_wait[(unsigned long)i]=0;
socket_disconnect_receive_close_wait:
  socket_disconnect_wait[(unsigned long)i]++;
  if (socket_disconnect_wait[(unsigned long)i]<=16){//can wait up to 16 sec
    if (socketclient_ri[(unsigned long)i]->exit_thread!=2){SleepEx(1024,NULL); goto socket_disconnect_receive_close_wait;}
  }
  if (socketclient_ri[(unsigned long)i]->exit_thread!=2){
    TerminateThread((void*)socketclient_ri[(unsigned long)i]->thread_id,0);
  }

  socketclient[(unsigned long)i]=INVALID_SOCKET;
  ExitThread(0);
  return 0;
}






bool NET_send(void *s,unsigned long d,txt *t){
  static txt *t2=txtnew();
  static unsigned long bytestosend;
  static unsigned long bytessent;

  dv+=t->l;

  if (d==INVALID_NET) return(0);


  //if (d==1) d=0;

  txtNEWLEN(t2,-(2+t->l));
  t2->ds[0]=t->l;
  memcpy(&t2->ds[1],&t->d2[0],t->l);



  bytestosend=t2->l;
  bytessent=0;

  //if all buffers are full close the thread
  if (socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->l){
    if (socketclient_si[(unsigned long)d]->exit_thread==0) socketclient_si[(unsigned long)d]->exit_thread=1;
    return FALSE;
  }

  if (socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->bl<(bytestosend+1)){
    //free old allocated memory
    free(socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->d);
    //extend buffer
    socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->d=(char*)malloc(bytestosend+1);
    socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->bl=bytestosend+1;
  }
  socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->d2[bytestosend]=0;
  memcpy(&socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->d2[0],&t2->d2[bytessent],bytestosend);
  socketclient_si[(unsigned long)d]->d[socketclient_si[(unsigned long)d]->nextfree]->l=bytestosend;
  socketclient_si[(unsigned long)d]->nextfree++;


  return TRUE;
}

void getscreenoffset(long x,long y,long *mapx,long *mapy){
  *mapx=x-15; *mapy=y-11;
  if ((x<=1023)&&(y<=1023)){//within main overworld
    if (*mapx<0) *mapx=0;
    if (*mapy<0) *mapy=0;
    if (*mapx>992) *mapx=992;
    if (*mapy>1000) *mapy=1000;
    return;
  }
  if ((x>=1024)&&(y>=256)&&(x<=1279)&&(y<=511)){//within gargoyle lands
    if (*mapx<1024) *mapx=1024;
    if (*mapy<256) *mapy=256;
    if (*mapx>1248) *mapx=1248;
    if (*mapy>488) *mapy=488;
    return;
  }
  if ((x>=1327)&&(y>=319)&&(x<=1358)&&(y<=343)){//within toth's house
    *mapx=1327;
    *mapy=319;
    return;
  }
  //undefined
  if (*mapx<0) *mapx=0;
  if (*mapy<0) *mapy=0;
  if (*mapx>2016) *mapx=2016;
  if (*mapy>1000) *mapy=1000;
}

/* luteijn: this looks extermely inefficient, replaced with an inline function */
/*
//getnbits returns the number of bits required to store n combinations
//if combinations is 1, getnbits returns 0 (only 1 combination requires 0 bits represent)
//if combinations is 2, getnbits returns 1
unsigned char getnbits(unsigned long combinations){
  static unsigned long comparison_value,getnbits_return;
  comparison_value=1; getnbits_return=0;
getnbits_loop:
  if (comparison_value<combinations){
    getnbits_return++;
    comparison_value<<=1;
    goto getnbits_loop;
  }
  return getnbits_return;
}
*/

unsigned char objgetdir(unsigned short type){ /* FIXME this can be handeled nicer, e.g. switch */
  OBJGETDIR_FRAME=0;
  if ((type&1023)==414) return type>>10;//skiff
  if ((type&1023)==412) return (type>>11)&3;//ship
  if (obji[sprlnk[type&1023]].v4==2){//"rat"
    OBJGETDIR_FRAME=(type>>10)&1;
    return (type>>11)&3;
  }
  if (obji[sprlnk[type&1023]].v4==4) return type>>10;//"rabbit"
  if ((type&1023)==362){//"winged gargoyle"
    OBJGETDIR_FRAME=((type>>10)-((type>>10)/12)*12)/4;
    return (type>>10)/12;
  }
  if ((type&1023)==363){//"gargoyle"
    OBJGETDIR_FRAME=((type>>10)-((type>>10)/3)*3);
    return (type>>10)/3;
  }
  if (obji[sprlnk[type&1023]].v4==6){//"drake"
    return (type>>11)&3;
  }
  if (obji[sprlnk[type&1023]].v4==1){//"avatar"
    OBJGETDIR_FRAME=(type>>10)&3;
    return (type>>12)&3;
  }
  if ((type&1023)==411){//"dragon"
    return (type>>11)&3;
  }
  if ((type&1023)==424){//"cyclops"
    OBJGETDIR_FRAME=(type>>12)&1;
    return (type>>13)&3;
  }
  if (obji[sprlnk[type&1023]].v4==8){//"ant"
    OBJGETDIR_FRAME=(type>>10)&1;
    return (type>>11)&3;
  }
  return 0; //frame is irrlavent & has no direction
}

void txtaddcolouredname(txt *t,txt *name,player *p){
  static long x;
  txtaddchar(t,6);//colour change for n characters
  x=p->GNPCflags[245]&255; x>>=3;
  x+=48; if (x>57) x=x-48-10+65; if (x>90) x=x-65-26+97;//support for 0-61 values
  txtaddchar(t,x);//colour value
  x=(p->GNPCflags[245]>>8)&255; x>>=3;
  x+=48; if (x>57) x=x-48-10+65; if (x>90) x=x-65-26+97;//support for 0-61 values
  txtaddchar(t,x);//colour value
  x=(p->GNPCflags[245]>>16)&255; x>>=3;
  x+=48; if (x>57) x=x-48-10+65; if (x>90) x=x-65-26+97;//support for 0-61 values
  txtaddchar(t,x);//colour value
  x=name->l;
  x+=48; if (x>57) x=x-48-10+65; if (x>90) x=x-65-26+97;//support for 0-61 values
  txtaddchar(t,x);//num of characters
  txtadd(t,name);
}


#define	ROTATE_R(x,n)		((x >> n) | (x << (32 - (n))))

void SHA2(txt* t,txt* t2) {
  txt *t3=txtnew(); //on client side would be better not to be static but on host side static is ok oh well just going for no static for now.
  unsigned int hx[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
  };
  unsigned char chunk[64],i;
  unsigned int w[64],s0,s1,tmp1,tmp2,maj,ch,a,b,c,d,e,f,g,h;
  unsigned long len;

  //Initialize table of round constants
  //(first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
  static unsigned int k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };


  txtset(t3,t);//to keep the original txt intact
  //break message into 512-bit, 64 byte chunks
  len=(t3->l)&63;
  txtNEWLEN(t2,64-len);
  memset(t2->d2,0,t2->l);
  txtadd(t3,t2);
  len=t3->l;
  while(len >0) {
    memcpy(chunk,t3->d2,64);
    //break chunk into sixteen 32-bit big-endian words w[0..15]
    for(i=0;i<16;i++){
      w[i] = (((chunk[i*4 + 0])) << 24) | (((chunk[i*4 + 1])) << 16)|(((chunk[i*4 + 2])) <<  8)|(chunk[i*4 + 3]);
    }
    //Extend the sixteen 32-bit words into sixty-four 32-bit words:
    for(i=16;i<64;i++) {
      s0 = ROTATE_R(w[i-15],7) ^ ROTATE_R(w[i-15],18) ^ (w[i-15] >> 3);
      s1 = ROTATE_R(w[i-2],17) ^ ROTATE_R(w[i-2],19) ^ (w[i-2] >> 10);
      w[i] = w[i-16] + s0 + w[i-7] + s1;
    }


    //Initialize hash value for this chunk:
    a = hx[0];
    b = hx[1];
    c = hx[2];
    d = hx[3];
    e = hx[4];
    f = hx[5];
    g = hx[6];
    h = hx[7];

    //Main loop:
    for(i=0;i<64;i++){
      s0 = ROTATE_R(a,2) ^ ROTATE_R(a,13) ^ ROTATE_R(a,22);
      maj = (a&b) ^ (a&c) ^ (b&c);
      tmp2 = s0 + maj;
      s1 = ROTATE_R(e,6) ^ ROTATE_R(e,11) ^ ROTATE_R(e,25);
      ch = (e&f) ^ ((~e) & g);
      tmp1 = h + s1 + ch + k[i] + w[i];

      h = g;
      g = f;
      f = e;
      e = d + tmp1;
      d = c;
      c = b;
      b = a;
      a = tmp1 + tmp2;
    }
    //Add this chunk's hash to result so far:
    hx[0] = hx[0] + a;
    hx[1] = hx[1] + b;
    hx[2] = hx[2] + c;
    hx[3] = hx[3] + d;
    hx[4] = hx[4] + e;
    hx[5] = hx[5] + f;
    hx[6] = hx[6] + g;
    hx[7] = hx[7] + h;
    len-=64;
  }
  //Produce the final hash value (big-endian):
  //digest = hash = h0 append h1 append h2 append h3 append h4 append h5 append h6 append h7
  txtNEWLEN(t2,32);
  for (i=0;i<8;i++){
    t2->d2[i*4]=hx[i] >> 24;
	t2->d2[i*4+1]= hx[i] >> 16;
	t2->d2[i*4+2]= hx[i] >> 8;
	t2->d2[i*4+3]= hx[i];
  }
  free(t3);
}


// c111 both functions
unsigned long new1_getexprequired(unsigned short level) {
	// original exp required
	//x3=(800<<(tnpc3->lev-1)); //required xp to level
	//unsigned short level = npc->lev;
	unsigned long requiredexp = (800 << (level - 1));
	unsigned long level3xp = (800 << 2);

	if (level <= 3)
		requiredexp = (800 << (level - 1));
	else if (level > 3)
		requiredexp = level3xp + (level - 3) * 5000;

	return requiredexp;
}

unsigned long new1_getexprequired(npc* npc) {
	// original exp required
	//x3=(800<<(tnpc3->lev-1)); //required xp to level
	/*
	unsigned short level = npc->lev;
	unsigned long requiredexp = (800 << (level - 1));
	unsigned long level3xp = (800 << 2);

	if (level <= 3)
		requiredexp = (800 << (level - 1));
	else if (level > 3)
		requiredexp = level3xp + (level - 3) * 5000;

	return requiredexp;
	*/
	return new1_getexprequired(npc->lev);
}

