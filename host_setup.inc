
//house fixes
myobj=od[346][400]; OBJremove(myobj);
myobj=od[347][400]; OBJremove(myobj);


//-----------------------------------------------------------------------------

//2005: autosetup & map fixes

myobj=od[383][303]; //lever in LB's castle!!!
use_basic_toggle(NULL,myobj); //lever
myobj=od[393][327]; //mint!!!
use_basic_toggle(NULL,myobj); //lever
myobj=od[752][399]; //trisnic
use_basic_toggle(NULL,myobj); //lever


//NEWCODE LB's castle drawbridge
myobj=od[384][303]; //crank
myobj2=od[385][303];
myobj->more=myobj2;
myobj=od[384][311]; //crank
myobj->more=myobj2;
use_basic_toggle(NULL,myobj); //use crank to remove the "drawbridge up" items before linking the other parts.
for (myobj2=myobj;myobj2;myobj2=(object*)myobj2->more) {
  myobj=myobj2;
}
for (x=304;x<=310;x++) { 
  for (y=386;y<=390;y++) {
    myobj2=od[y][x];
    myobj->more=myobj2;
    myobj=myobj2;
  }
}
//NEWCODEEND
//balloon plans FIX
myobj3=OBJfindlast(1971,221);
OBJremove(myobj3);
myobj4=OBJnew(); myobj4->type=270; myobj4->info|=256;
OBJadd(1971,221,myobj4);
OBJadd(1971,221,myobj3);
//shipwreck map FIX
myobj4=OBJnew(); myobj4->type=407; myobj4->info|=256;
OBJadd(427,933,myobj4);
//ant map FIX
myobj3=OBJfindlast(2040,40);
OBJremove(myobj3);
myobj4=OBJnew(); myobj4->type=400; myobj4->info|=256;
OBJadd(2040,40,myobj4);
OBJadd(2040,40,myobj3);
//bonn's map FIX
myobj3=OBJfindlast(1259,53);
OBJremove(myobj3);
myobj4=OBJnew(); myobj4->type=403; myobj4->info|=256;
OBJadd(1259,53,myobj4);
OBJadd(1259,53,myobj3);
//rune of honesty FIX
myobj3=OBJfindlast(1499,147);
OBJremove(myobj3);
myobj4=OBJnew(); myobj4->type=242; myobj4->info|=256;
OBJadd(1499,147,myobj4);
OBJadd(1499,147,myobj3);
//rune of spirituality FIX
myobj3=OBJfindlast(81,493);
OBJremove(myobj3);
myobj4=OBJnew(); myobj4->type=248; myobj4->info|=256;
OBJadd(81,493,myobj4);
OBJadd(81,493,myobj3);
//treasure room FIX
myobj3=OBJfindlastall(1947,229); myobj3->more2=0; //release treasure room's lock
//Sutek FIX
myobj3=OBJfindlastall(787,964); myobj3->more2=0; //release Sutek's lock
myobj3=OBJnew(); myobj3->type=269+3*1024; OBJadd(786,960,myobj3);
myobj3=OBJnew(); myobj3->type=269+3*1024; OBJadd(786,961,myobj3);
myobj3=OBJnew(); myobj3->type=269+4*1024; OBJadd(787,960,myobj3);
myobj3=OBJnew(); myobj3->type=269+4*1024; OBJadd(787,961,myobj3);
myobj3=OBJnew(); myobj3->type=269+5*1024; OBJadd(788,960,myobj3);
myobj3=OBJnew(); myobj3->type=269+5*1024; OBJadd(788,961,myobj3);
myobj3=OBJnew(); myobj3->type=269+0*1024; OBJadd(786,962,myobj3);
myobj3=OBJnew(); myobj3->type=269+1*1024; OBJadd(787,962,myobj3);
myobj3=OBJnew(); myobj3->type=269+2*1024; OBJadd(788,962,myobj3);
//NEWCODE Sutek's drawbridge
myobj=od[959][785]; //crank
myobj->more=od[960][786];
myobj=od[959][789]; //crank
for (x=786;x<=788;x++) { 
  for (y=960;y<=962;y++) {
    myobj2=od[y][x];
    myobj->more=myobj2;
    myobj=myobj2;
  }
}
myobj=od[959][789]; //crank
use_basic_toggle(NULL,myobj); //use crank to remove the "drawbridge down" items before linking the other parts.
myobj=myobj2;
for (x=785;x<=788;x++) { 
  myobj2=OBJfindlastall(x,959);
  myobj->more=myobj2;
  myobj=myobj2;
}
//NEWCODEEND

//Codex FIX
myobj3=OBJnew(); myobj3->type=394; myobj3->info|=256; OBJadd(921,851,myobj3);
myobj3=OBJnew(); myobj3->type=396; myobj3->info|=256; OBJadd(925,851,myobj3);
myobj3=OBJnew(); myobj3->type=62; myobj3->info|=256; OBJadd(923,850,myobj3);
//?
// luteijn: view redirectors.
myobj3=OBJnew(); myobj3->type=416; OBJadd(921,850,myobj3);
myobj3=OBJnew(); myobj3->type=416; OBJadd(920,850,myobj3);
myobj3=OBJnew(); myobj3->type=416; OBJadd(925,850,myobj3);
myobj3=OBJnew(); myobj3->type=416; OBJadd(926,850,myobj3);
myobj3=OBJnew(); myobj3->type=416; OBJadd(927,850,myobj3);
//nuggets FIX
myobj3=OBJnew(); myobj3->type=89; myobj3->more2=20; OBJadd(1642,65,myobj3);
nuggetsfix=myobj3;
//smith FIX
OBJremove(od[232][200]);
//pushmepullyou FIX
OBJremove(od[243][1475]);
//potions FIX
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=1; OBJadd(1109,109,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=2; OBJadd(1105,105,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=4; OBJadd(1099,114,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=8; OBJadd(1638,82,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=16; OBJadd(1661,74,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=32; OBJadd(1640,66,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=64; OBJadd(1690,155,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=128; OBJadd(1612,126,myobj3);
myobj3=OBJnew(); myobj3->type=275+2*1024; myobj3->info|=256; myobj3->more2=256; OBJadd(402,406,myobj3);
//Avatar's room in LB's castle fix
myobj=od[367][297]; myobj->more=NULL; myobj->info-=128;
myobj=od[367][298]; myobj->more=NULL; myobj->info-=128;
myobj=(object*)od[365][296]->more; myobj->info-=128; myobj=(object*)myobj->next; myobj->info-=128;


//enumerate doors, secret doors and levers
for (y=0;y<=1023;y++){ for (x=0;x<=2047;x++){
  myobj=od[y][x];
doorclose0:
  if (myobj){
    if (((myobj->type&1023)>=297)&&((myobj->type&1023)<=300)){//door
      if (myobj->type&1024){//walkable/main part of door
        doorclose_last++;
        doorclose_wait[doorclose_last]=0;
        doorclose_obj[doorclose_last]=myobj; doorclose_oldtype[doorclose_last]=myobj->type;
        myobj2=(object*)myobj->more;
        doorclose_obj2[doorclose_last]=myobj2; doorclose_oldtype2[doorclose_last]=myobj2->type;
      }//walkable/main part of door
    }//door
    if ((myobj->type&1023)==334){//secret door
      doorclose_last++;
      doorclose_wait[doorclose_last]=0;
      doorclose_obj[doorclose_last]=myobj; doorclose_oldtype[doorclose_last]=myobj->type;
      doorclose_obj2[doorclose_last]=NULL;
    }//secret door
    if ((myobj->type&1023)==268){//lever
      leverchange_last++;
      leverchange_wait[leverchange_last]=0;
      leverchange_obj[leverchange_last]=myobj; leverchange_oldtype[leverchange_last]=myobj->type;
    }//lever
    //TODO enumarate cranks and make a drawbridge reset
    myobj=(object*)myobj->next;
    goto doorclose0;
  }//myobj
}}//x,y

//2005 new fixed object system (buildable parts of fixed objects will not be sent)
//WARNING: NEW SYSTEM CANNOT FIX ALL STATIC OBJECTS (SUCH AS DEAD gargoyleS), ONLY BY TYPE
/*
unsigned short objfixed_next=1;
unsigned short objfixed_type[65536]; //number, object types
unsigned short objfixed_index[1024][2048];
*/
ZeroMemory(&objfixed_type,65536*2); ZeroMemory(&objfixed_index,2048*1024*2);
objfixed_next=1;
for (y=0;y<=1023;y++){ for (x=0;x<=2047;x++){
  x3=objfixed_next;
  if (myobj=od[y][x]){
    if (myobj->type==416){//view redirector
      if (myobj->more2){//location valid
        myobj=od[myobj->more2>>16][myobj->more2&65535];
        if (myobj==NULL) goto viewredirector_null;
      }
    }
objfixed_nextobj:
    if (tclass_fixed[myobj->type]){
      objfixed_index[y][x]=x3;
      objfixed_type[x3]++;
      objfixed_type[x3+objfixed_type[x3]]=myobj->type;
    }//tclass_fixed[myobj->type]

    /*
    myobj2=myobj;
    if (myobj->info&8){//redirector
    myobj2=(object*)myobj->more;
    }//redirector
    //mover with no npc or crt pointer
    if (tclass_mover[myobj2->type]){
    if ((myobj2->info&6)==0){
    objfixed_index[y][x]=x3;
    objfixed_type[x3]++;
    objfixed_type[x3+objfixed_type[x3]]=myobj->type;
    }
    }
    */
    /*
    ZeroMemory(&tclass_object,65536); ZeroMemory(&tclass_mover,65536); ZeroMemory(&tclass_fixed,65536); ZeroMemory(&tclass_build,65536);
    tfh=open(".\\dr\\objclass.txt");
    for (i=1;i<=431;i++){
    for (i2=0;i2<(sprlnk[i+1]-sprlnk[i]);i2++){
    txtfilein(t,tfh);
    txtset(t2,"o"); if (txtsame(t,t2)) tclass_object[i+i2*1024]=1;
    txtset(t2,"m"); if (txtsame(t,t2)) tclass_mover[i+i2*1024]=1;
    txtset(t2,"md"); if (txtsame(t,t2)) tclass_mover[i+i2*1024]=1+2;
    txtset(t2,"f"); if (txtsame(t,t2)) tclass_fixed[i+i2*1024]=1;
    txtset(t2,"bs"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=1;
    txtset(t2,"bh"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=2;
    txtset(t2,"bv"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=4;
    txtset(t2,"b"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=8;
    }//i2
    }//i
    close(tfh);
    */

    if (!myobj->more){
      if ((myobj->type&1023)!=414){ if ((myobj->type&1023)!=415){//not a raft or skiff
        x5=myobj->type;
unknownclass: if (tclass_build[x5]&(1+2+4)){x5+=1024; goto unknownclass;}
        if ((tclass_build[x5]&8)||(tclass_mover[x5])){
          objfixed_index[y][x]=x3;
          objfixed_type[x3]++;
          objfixed_type[x3+objfixed_type[x3]]=myobj->type;
        }
      }}
    }

    if (myobj->next){ myobj=(object*)myobj->next; goto objfixed_nextobj;}
viewredirector_null:;
  }//od
  if (objfixed_type[x3]) objfixed_next+=objfixed_type[x3]+1;
}}//x,y
tfh=open2(".\\dr\\objfixed.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE); put(tfh,&objfixed_index,2048*1024*2); put(tfh,&objfixed_type,65536*2); close(tfh);

/*
unsigned short tobjfixed_next=1;
unsigned short tobjfixed_type[65536]; //[number of objects],[object type(s)],...
unsigned short tobjfixed_index[1024][2048];
*/
ZeroMemory(&tobjfixed_type,65536*2); ZeroMemory(&tobjfixed_index,2048*1024*2);
tobjfixed_next=1;
for (y=0;y<=1023;y++){ for (x=0;x<=2047;x++){
  x3=tobjfixed_next;
  if (myobj=od[y][x]){
    if (myobj->type==416){//view redirector
      if (myobj->more2){//location valid
        myobj=od[myobj->more2>>16][myobj->more2&65535];
        if (myobj==NULL) goto tviewredirector_null;
      }
    }
tobjfixed_nextobj:
    if (tclass_object[myobj->type]){
      if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0){//not a floating object
        tobjfixed_index[y][x]=x3;
        tobjfixed_type[x3]++;
        tobjfixed_type[x3+tobjfixed_type[x3]]=myobj->type;
      }//not a floating object
    }//tclass_object[myobj->type]
    if (myobj->next){ myobj=(object*)myobj->next; goto tobjfixed_nextobj;}
tviewredirector_null:;
  }//od
  if (tobjfixed_type[x3]) tobjfixed_next+=tobjfixed_type[x3]+1;
}}//x,y
tfh=open2(".\\dr\\tobjfix.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE); put(tfh,&tobjfixed_index,2048*1024*2); put(tfh,&tobjfixed_type,65536*2); close(tfh);






//unsigned char visalways[256][1024];//bit array, if =1 force visibility
tfh=open2(".\\dr\\vis.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
for (x=0;x<=2047;x++){ for (y=0;y<=1023;y++){
  if (myobj=od[y][x]){
    if (myobj->type==416){//view redirector
      z=0;
      for (x2=-1;x2<=1;x2++){ for (y2=-1;y2<=1;y2++){
        if (myobj2=od[y+y2][x+x2]){
          if (myobj2->type!=416) z=1;
        }else z=1;
      }}//y2,x2
      if (z){
        put(tfh,&x,2); put(tfh,&y,2);
      }
    }//416
  }//myobj
}}//y,x
close(tfh);

//create bt.bin for client to reference
tfh=open2("bt.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
put(tfh,&bt,1024*2048*2);
close(tfh);
//later we will compress and encrypt this

//NEWCODE
if (NEThost){//cant add objects as a client
  //random treasure chest
  x8=randomchestlocation(false);
  y8=x8>>10;
  x8=x8&1023;

  //Objects
  x3=2+rnd*2;
  object* item000001=OBJaddnew(x8,y8,98+1024*x3,16384,0);//chest

  for (x=0;x<16;x++) {
    //x2=rnd*3;
    x2=1;
    x3=0;
    /*	switch (x2) { //random contents    //no need for random here? because they are randomed by the save function even in host initialization? 
    case 0: //any armour
    x2=rnd*25;
    if (x2==0) {x2=27;}
    break;
    case 1: //any weapon (excluding wands, staves, glass swords and rollingpins, knives etc)
    x2=33+rnd*18;
    if (x2==48) {x2=54;}
    break; 
    case 2: //magic stuff
    x2=78+rnd*8;
    if (x2==82) {x2=48;}
    if (x2>82) {x2+=173;}
    break; 
    default: //should never happen
    break;
    } */
    object* item000002=OBJaddnew(x8,y8,x2+1024*0,16384,x3);
    OBJremove(item000002);//remove from the map (so it can go in a container)
    OBJaddtocontainer(item000001, item000002); //put this item inside its parent
  }
}//NEThost


//NEWCODEEND




