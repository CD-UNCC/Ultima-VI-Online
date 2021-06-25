//host message processing ________________________________________________________________________

Randomc(timeGetTime());


tfh=open2("ban.txt",OF_READWRITE|OF_SHARE_COMPAT);
if (tfh->h==HFILE_ERROR){
	tfh=open2("ban.txt",OF_READWRITE|OF_CREATE);
}
close(tfh);







//set orb of the moons destinations
for (x=0;x<=4;x++){ for (y=0;y<=4;y++){
	orbx[x][y]=368;
	orby[x][y]=386;
}}
//outer circle
orbx[0][0]=899;
orby[0][0]=499;
orbx[1][0]=935;
orby[1][0]=262;
orbx[2][0]=435;
orby[2][0]=395;
orbx[3][0]=503;
orby[3][0]=358;
orbx[4][0]=147;
orby[4][0]=883;
orbx[4][1]=159;
orby[4][1]=942;
orbx[4][2]=227;
orby[4][2]=131;
orbx[4][3]=295;
orby[4][3]=38;
orbx[4][4]=667;
orby[4][4]=67;
orbx[3][4]=831;
orby[3][4]=166;
orbx[2][4]=387;
orby[2][4]=787;
orbx[1][4]=327;
orby[1][4]=822;
orbx[0][4]=75;
orby[0][4]=507;
orbx[0][3]=1047;
orby[0][3]=22;
orbx[0][2]=739;
orby[0][2]=699;
orbx[0][1]=919;
orby[0][1]=934;
//inner circle
orbx[1][1]=1092;
orby[1][1]=301;
orbx[2][1]=307;
orby[2][1]=352;
orbx[3][1]=1212;
orby[3][1]=301;
orbx[3][3]=923;
orby[3][3]=876;
orbx[2][3]=1132;
orby[2][3]=477;
orbx[1][3]=1152;
orby[1][3]=342;
//25 locations=25 bits used out of 32, only 1 global flag needs alloocation
tfh=open(".\\host\\crtenum.bin"); get(tfh,&crtenum,1073*2); close(tfh);
u6orevive_fh=open(".\\host\\revive.bin");
motd=txtnew(); tfh=open("motd.txt"); txtfilein(motd,tfh); close(tfh);
tfh=open(".\\host\\sc888.raw"); get(tfh,&stormcloak_mask,64); close(tfh);
tfh=open(".\\host\\u6bt.flg"); get(tfh,&btflags,lof(tfh)); close(tfh);
FillMemory(&objpassflags,sizeof(objpassflags),1); tfh=open(".\\host\\objpass.flg"); get(tfh,&objpassflags,lof(tfh)); close(tfh);
//update flags for world map (based on basetiles)
for (y=0;y<1024;y++){ for (x=0;x<2048;x++){
	OBJcheckflags(x,y);
}}







tfh=open(".\\host\\schedule.bin"); get(tfh,&schedule,lof(tfh));
//fix indexing
i3=lof(tfh)/8/32-1;
for (i2=0;i2<=i3;i2++){
	for (i=0;i<=31;i++){
		if (schedule[i2][i].hour!=255){
			//if (schedule[i2][i].x>1024) exit(0);
			if (schedule[i2][i].z==1) schedule[i2][i].x+=1024;
			if (schedule[i2][i].z==2) schedule[i2][i].x+=1024+256;
			if (schedule[i2][i].z==3) schedule[i2][i].x+=1024+256*2;
			if (schedule[i2][i].z==4) schedule[i2][i].x+=1024+256*3;
			if (schedule[i2][i].z==5) {schedule[i2][i].x+=1024; schedule[i2][i].y+=256;}
			//if (schedule[0][i].x>2048) exit(0);
		}

	}
}
close(tfh);
tfh=open(".\\host\\npcobj.bin"); get(tfh,&npcobj,lof(tfh)); close(tfh);

tfh=open(".\\host\\objname.bin");
objname=(unsigned long*)malloc(lof(tfh));
objname2=(unsigned char*)objname;
get(tfh,objname,lof(tfh)); close(tfh);

tfh=open(".\\host\\sign.bin");
tsign=(unsigned long*)malloc(lof(tfh));
tsign2=(unsigned char*)tsign;
get(tfh,tsign,lof(tfh)); close(tfh);


//load converse
tfh=open(".\\host\\npc.bin");
npcbin=(npcbin_i*)malloc(lof(tfh));
get(tfh,npcbin,lof(tfh)); close(tfh);
tfh=open(".\\host\\npc.inf");
npcinf=(unsigned char*)malloc(lof(tfh));
get(tfh,npcinf,lof(tfh)); close(tfh);
tfh=open(".\\host\\npci.bin");
get(tfh,&npci,lof(tfh)); close(tfh);

txtset(t,"  ");
txtset(t2,".\\ultima6\\savegame\\objblk");
for (x=0;x<=7;x++){
	for (y=0;y<=7;y++){
yyyi:
		t->d[0]=x+65;
		t->d[1]=y+65;
		txtset(t3,t2); txtadd(t3,t);
		tfh=open(t3);
		i=0; get(tfh,&i,2);
		if (i>0){
			get(tfh,&ol[oln],lof(tfh)-2);
			oln+=i;
		}
		close(tfh);
		if ((y==7)&&(x<=4)) {y++; goto yyyi;}
	}}


//first pass
for (i=oln-1;i>=0;i--){
	x=ol[i].unk_h; //get coordinates x,y
	y=ol[i].unk_d1;
	x=x+(y&3)*256; y=y/4; //fix coordinates
	y=y+(ol[i].unk_d2&15)*64;
	i6=ol[i].unk_d2>>4; //i6<-level
	if (x<1024){ if (y<1024){ //avoid non-coordinate based objects
		if (i6==1) x=x+1024; //set U6O pos
		if (i6==2) x=x+1024+256;
		if (i6==3) x=x+1024+256*2;
		if (i6==4) x=x+1024+256*3;
		if (i6==5) {x=x+1024; y=y+256;}
		i2=ol[i].type&1023; //get object identity
		x2=0;
		if ((i2>=29)&&(i2<=32)) x2=1; //overlay floors
		if (i2==229) x2=1; //shadow
		if (i2==231) x2=1;
		if (i2==238) x2=1;
		if (i2==240) x2=1;
		if (i2==251) x2=1;
		if (i2==260) x2=1;
		if (i2==262) x2=1;
		if ((i2==303)&&(x==122)&&(y==196)) x2=1; //carpet
		//*the healers bed has a table object on top!
		if (x2){
			myobj=OBJnew();
			myobj->type=ol[i].type;
			OBJadd(x,y,myobj);
			x2=ol[i].type>>10;
			i3=sprlnk[i2];
			i3+=x2;
			i4=i3;
			myobj2=myobj;
		}
	}} //x<1024,y<1024
} //i
//first pass end



static unsigned long ox,oy;

//add objects to the map
for (i=oln-1;i>=0;i--){
	i2=ol[i].type&1023; //get object identity
	if (i2>431) exit(901); //invalid range

	x2=0; //skip objects
	if (i2==397) x2=1; //minax, exodus, mondain
	if (i2==398) x2=1;
	if (i2==399) x2=1;
	if ((i2==412)&&((ol[i].type>>13)!=1)) x2=1; //non-core parts of boat
	if (i2==337) x2=1; //effect
	if (i2==248) x2=1; //rune of spirituality!
	if (i2==242) x2=1; //rune of honesty!
	if (i2==403) x2=1; //bonn's map
	if (i2==407) x2=1; //shipwreck map
	if (i2==400) x2=1; //ant map
	if (i2==270) x2=1; //balloon plans


	if (i2==173){
		ol[i].status=0; //trap
	}


	if (x2) goto skipobj;

	if ((ol[i].status&16)==0){ //16=object in party inventory *skip

		if (ol[i].status&8){ //8=container
			//object in a container
			//2003: note: first pass objects won't be inside containers! check unnecessary
			myobj=OBJnew();
			myobj->type=ol[i].type;
			if ((ol[i].status&1)==0) myobj->info|=128; //128 1=stealing, 0=ok to take
			if (obji[sprlnk[myobj->type&1023]].v4) myobj->more2=i; //*temp reference value
			if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){
				myobj->more2=ol[i].qty; //set quantity in more2
				if (myobj->more2==0) myobj->more2=1;
			}
			if ((myobj->type&1023)==64){ //key, set type!
				myobj->more2=ol[i].tag; //more2<-
			}

			if ((myobj->type&1023)==58){ //spell, set type!
				myobj->more2=ol[i].tag; //more2<-
				if (spellname[myobj->more2]->l==0) myobj->more2=(0<<4)+7;
			}


			//if ((myobj->type>=242)&&(myobj->type<=249)) myobj->info|=256; //rune <-quest item

			if ((myobj->type&1023)==332){ //sign
				myobj->more2=ol[i].tag; //more2<-
			}
			if ((myobj->type&1023)==333){ //gargoyle sign
				myobj->more2=ol[i].tag; //more2<-
			}
			if ((myobj->type&1023)==152){ //scroll
				myobj->more2=ol[i].tag; //more2<-
			}
			if ((myobj->type&1023)==143){ //picture
				myobj->more2=ol[i].tag; //more2<-
			}
			if ((myobj->type&1023)==151){ //book
				myobj->more2=ol[i].tag; //more2<-
			}
			if ((myobj->type&1023)==333){ //gsign
				myobj->more2=ol[i].tag; //more2<-
			}
			if ((myobj->type&1023)==59){ //codex
				myobj->more2=128;
			}
			if ((myobj->type&1023)==61){ //circles
				myobj->more2=ol[i].tag; //more2<-
			}

			if ((myobj->type&1023)==254){ //cross
				myobj->more2=ol[i].tag; //more2<-
			}
			if ((myobj->type&1023)==255){ //tombstone
				myobj->more2=ol[i].tag; //more2<-
			}

			if (newll==NULL){ //newll is a linked list, object is added to the start of this list
				newll=myobj;
			}
			else{
				myobj->next=newll;
				newll->prev=myobj;
				newll=myobj;
			}


		} //8
	} //16

	if (ol[i].status<=1){ //not in container, not party member item

		x=ol[i].unk_h; //get coordinates x,y
		y=ol[i].unk_d1;
		x=x+(y&3)*256; y=y/4; //fix coordinates
		y=y+(ol[i].unk_d2&15)*64;
		i6=ol[i].unk_d2>>4; //i6<-level
		if (x<1024){ if (y<1024){
			if (i6==1) x=x+1024; //set U6O pos
			if (i6==2) x=x+1024+256;
			if (i6==3) x=x+1024+256*2;
			if (i6==4) x=x+1024+256*3;
			if (i6==5) {x=x+1024; y=y+256;}


			ox=x; oy=y;




			x2=0;
			if ((i2>=29)&&(i2<=32)) x2=1; //overlay floors
			if (i2==229) x2=1; //shadow
			if (i2==231) x2=1;
			if (i2==238) x2=1;
			if (i2==240) x2=1;
			if (i2==251) x2=1;
			if (i2==260) x2=1;
			if (i2==262) x2=1;
			if ((i2==303)&&(x==122)&&(y==196)) x2=1; //carpet
			if (((i2&1023)==0)&&(x==1151)&&(y==309)) x2=1; //flashing spell near rune of singularity!

			if (x2==0){ //exclude first pass objects


				//add basic object
				myobj=OBJnew();
				myobj->type=ol[i].type;
				if (x<1024){
					if ((ol[i].status&1)==0) myobj->info|=128; //128 1=stealing, 0=ok to take
				}

				if ((x>=288)&&(x<=326)&&(y>=344)&&(y<=382)) myobj->info|=128; //castle
				//cheese exception!
				if ((x==320)&&(y==351)){
					if (myobj->type==132) myobj->info-=128;
				}

				if ((x==137)&&(y==185)) myobj->info|=128; //abby loot
				//if ((x==139)&&(y==185)) myobj->info|=128; //abby loot (grapes are free!)
				if ((x>=883)&&(x<=899)&&(y>=426)&&(y<=445)){ //lyceam
					if (!((x==897)&&(y==443))) myobj->info|=128;
				}
				if ((x>=1257)&&(x<=1261)&&(y>=49)&&(y<=53)){ //bonn
					myobj->info|=128;
				}


				ol_tag_prev=ol_tag;
				ol_tag=ol[i].tag;

				//if (ol_tag_prev==0xe){
				//txtnumint(t5,ol_tag_prev);
				//MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
				//}



				//if newll is set objects stored in temp array will be added
				if (newll!=NULL){ //add container's objects to container

					myobj->more=newll;
					newll->prev=myobj;

					myobj2=newll;
castle_obj:
					if ((x>=288)&&(x<=326)&&(y>=344)&&(y<=382)) myobj2->info|=128; //castle
					if ((x==138)&&(y==185)) myobj2->info|=128; //abby loot
					if ((x==140)&&(y==185)) myobj2->info|=128; //abby loot
					if ((x==138)&&(y==189)) myobj2->info|=128; //abby loot
					if ((x==139)&&(y==189)) myobj2->info|=128; //abby loot
					if ((x>=883)&&(x<=899)&&(y>=426)&&(y<=445)){ //lyceam
						if (!((x==897)&&(y==443))) myobj2->info|=128; 
					}
					if ((x>=1024)&&(myobj2->info&128)) myobj2->info-=128; //dungeon
					if ((x>=1257)&&(x<=1261)&&(y>=49)&&(y<=53)){ //bonn
						myobj2->info|=128;
					}
					if ((x>=1940)&&(x<=1954)&&(y>=212)&&(y<=218)){ //treasure room
						if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].flags&4096){
							myobj2->more2/=4; if (myobj2->more2==0) myobj2->more2=1;
						}
					}

					if ((x==1642)&&(y==64)) myobj2->more2=10; //gold nuggets in theif's house

					ENHANCEnewn(myobj2,8,4);
					if (myobj2->next) {myobj2=(object*)myobj2->next; goto castle_obj;}



					//egg
					if (i2==335){//extract egg

						//787 944
						//if ((x==787)&&(y==944)) exit(9);

						if ((x==778)&&(y==937)) goto eggskip; //mimic box in Sutek's house

						//txtnumint(t5,x);
						//MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


						myobj2=newll;
extractobj2:
						myobj4=(object*)myobj2->next;



						z2=ol[myobj2->more2].qty;
						if (z2==0) z2=1;
						if (z2>8) z2=8;
						if (z2>2){
							z2=int((z2-2)/2)+2;
						}

multiple_enemy:
						z2--;
						if (z2){
							myobj5=OBJnew();
							myobj5->type=myobj2->type;
							myobj5->more2=myobj5->more2;
						}else{
							myobj5=NULL;
						}

						myobj2->next=NULL;
						myobj2->prev=NULL;


						if (obji[sprlnk[myobj2->type&1023]].v4){
							y8=1;
							x8=myobj2->type&1023;
							//EXCLUDE tandglevines(365), reapers(347),squid&serpent(345&346),413(silver serp),2part movers(426-431)
							if (x8==365) y8=0;
							if (x8==347) y8=0;
							if (x8==345) y8=0;
							if (x8==346) y8=0;
							if (x8==413) y8=0;
							if ((x8>=426)&&(x8<=431)) y8=0;
							if (y8){
								//check original square for a crt
								if (myobj3=od[y][x]){
spreadoutchecknext:
									if (myobj3->info&4) goto spreadoutcrt;
									if (myobj3->next){myobj3=(object*)myobj3->next; goto spreadoutchecknext;}
								}
								goto spreadoutcrtnotneeded;
								//spread out
spreadoutcrt:
								static unsigned short spreadoutx[32];
								static unsigned short spreadouty[32];
								static long spreadouti;
								spreadouti=0;
								for (y8=y-2;y8<=(y+2);y8++){ for (x8=x-2;x8<=(x+2);x8++){
									if ((abs(y8-y)+abs(x8-x))<4){
										if (x8>=0){ if (y8>=0){ if (x8<2048){ if (y8<1024){//map boundries check
											if (!od[y8][x8]){
												if (bt[y8][x8]&1024){ //walkable location
													if (OBJcheckbolt(x,y,x8,y8)==NULL){ //not blocked
														spreadoutx[spreadouti]=x8;
														spreadouty[spreadouti]=y8;
														spreadouti++;
													}//OBJcheckbolt
												}//walkable
											}//od
										}}}}
									}//<4
								}}
								if (spreadouti==0) goto spreadoutcrtnotneeded;
								spreadouti=rnd*spreadouti;
								OBJadd(spreadoutx[spreadouti],spreadouty[spreadouti],myobj2); goto scatteradd;
							}
						}//.v4
spreadoutcrtnotneeded:


						//scatter if stationary enemy
						if (((myobj2->type&1023)==365)||((myobj2->type&1023)==347)){
scatter_retry:
							x8=x+(unsigned short)(rnd*7)-3; y8=y+(unsigned short)(rnd*7)-3;
							if (od[y8][x8]) goto scatter_retry;
							OBJcheckflags(x8,y8);
							if ((bt[y8][x8]&1024)==0) goto scatter_retry;

							OBJadd(x8,y8,myobj2); goto scatteradd;
						}

						//if ((myobj2->type&1023)==352) {OBJadd(x+(unsigned short)(rnd*7)-3,y+(unsigned short)(rnd*7)-3,myobj2); goto scatteradd;} //ghost
						OBJadd(x,y,myobj2);
scatteradd:

						if (obji[sprlnk[myobj2->type&1023]].v4==0){ //gold nugget(s)/reagent(s)/..
							myobj2->info=0;
							z=myobj2->more2; z=rnd*(z+1); if (z==0) z=1;
							myobj2->more2=z;
						}

						if (obji[sprlnk[myobj2->type&1023]].v4){//check if egg/spawn item is a monster

							//2006: use movernew(unsigned short type,unsigned short x,unsigned short y,unsigned long flags)
							x8=myobj2->x; y8=myobj2->y;
							OBJremove(myobj2);//discard existing object

							//FIX for 4 part movers to not be ignored by movernew
							if (myobj2->type==424) myobj2->type+=(3*1024);
							if (myobj2->type==362) myobj2->type+=(3*1024);


							movernew(myobj2->type,x8,y8,0);

							OBJrelease(myobj2);
							//exit(78);
						}

						/*


						myobj2->more=malloc(sizeof(creature));
						ZeroMemory(myobj2->more,sizeof(creature));
						crt=(creature*)myobj2->more;
						crt->crt_struct=TRUE;

						//OLD VERSION
						//crt->hp=obji[sprlnk[myobj2->type&1023]].v5*8;
						//if (crt->hp<8) crt->hp=8; //*unknown

						crt->hp=(obji[sprlnk[myobj2->type&1023]].v8>>8)*4;
						crt->mp=rnd*9;

						//if (crt->hp==(150*8)){
						//txtnumint(t5,crt->hp);
						//MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
						//}


						//if ((myobj2->type&1023)==384){
						//txtnumint(t5,x);
						//MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
						//txtnumint(t5,y);
						//MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
						//}


						z=1; //passive?
						if ((myobj2->type&1023)==382){ //guard
						if (x<1024) z=4; //not underground
						}
						if ((myobj2->type&1023)==373) z=0; //wisp
						if ((myobj2->type&1023)==430) z=0; //horse
						if ((myobj2->type&1023)==428) z=0; //cow
						if ((myobj2->type&1023)==356) z=0; //bird
						if ((myobj2->type&1023)==348) z=0; //sheep
						if ((myobj2->type&1023)==350) z=0; //deer

						if ((myobj2->type&1023)==388){ //cat
						if ((x==402)&&(y==583)) z=0; //Dr Cat's cats
						}

						if ((myobj2->type&1023)==352){ //ghost
						if ((x==139)&&(y==196)) z=0; //abby ghosts
						}


						crt->al=z;
						//if (rnd*4.0f<1.0f) crt->al=4; //*************************************


						myobj9=OBJnew(); myobj9->type=188; crt->items=myobj9; //(not included)bag

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


						//if ((myobj2->type&1023)==349) z=0; //dog



						myobj2->info|=4; //<-crt
						}




						if (obji[sprlnk[myobj2->type&1023]].v4==8){ //2-part creature
						myobj3=OBJnew(); myobj3->more=myobj2;
						myobj3->type=myobj2->type+8*1024; myobj3->info|=8; //<-base object
						OBJadd(x,y+1,myobj3);
						crt->more=myobj3;

						//sutek's animals
						z=0;
						if ((x>=747)&&(x<=829)&&(y>=905)&&(y<=983)) z=1; //on island
						if ((x>=792)&&(x<=796)&&(y>=937)&&(y<=938)) z=0; //in cage
						if (z) crt->hp*=4;

						}

						if (obji[sprlnk[myobj2->type&1023]].v4==5){ //cyclops/winged gargoyle

						static mlobj *mmyobj; //array size varies
						mmyobj=(mlobj*)malloc(sizeof(object*)*3);
						crt->more=mmyobj;
						myobj2->type=(myobj2->type&1023)+3*1024;
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[0]=myobj3;
						myobj3->type=myobj2->type-3*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[1]=myobj3;
						myobj3->type=myobj2->type-2*1024; myobj3->info|=8; //<-base object
						OBJadd(x,y-1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[2]=myobj3;
						myobj3->type=myobj2->type-1*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y,myobj3);

						//crt->more=myobj3;


						}


						if (obji[sprlnk[myobj2->type&1023]].v4==11){ //hydra

						static mlobj *mmyobj; //array size varies
						mmyobj=(mlobj*)malloc(sizeof(object*)*8);
						crt->more=mmyobj;
						//myobj2->type=(myobj2->type&1023)+3*1024;

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[0]=myobj3;
						myobj3->type=425+28*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[1]=myobj3;
						myobj3->type=425+0*1024; myobj3->info|=8; //<-base object
						OBJadd(x,y-1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[2]=myobj3;
						myobj3->type=425+4*1024; myobj3->info|=8; //<-base object
						OBJadd(x+1,y-1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[3]=myobj3;
						myobj3->type=425+8*1024; myobj3->info|=8; //<-base object
						OBJadd(x+1,y,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[4]=myobj3;
						myobj3->type=425+12*1024; myobj3->info|=8; //<-base object
						OBJadd(x+1,y+1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[5]=myobj3;
						myobj3->type=425+16*1024; myobj3->info|=8; //<-base object
						OBJadd(x,y+1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[6]=myobj3;
						myobj3->type=425+20*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y+1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[7]=myobj3;
						myobj3->type=425+24*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y,myobj3);

						}


						if (obji[sprlnk[myobj2->type&1023]].v4==10){ //silver serpent

						static mlobj *mmyobj; //array size varies
						mmyobj=(mlobj*)malloc(sizeof(object*)*7);
						crt->more=mmyobj;
						myobj2->type=(myobj2->type&1023)+4*1024;

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[0]=myobj3;
						myobj3->type=myobj2->type+8*1024; myobj3->info|=8; //<-base object
						OBJadd(x,y-1,myobj3);

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[1]=myobj3;
						myobj3->type=myobj2->type+7*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj3);

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[2]=myobj3;
						myobj3->type=myobj2->type+6*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y,myobj3);

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[3]=myobj3;
						myobj3->type=myobj2->type+9*1024; myobj3->info|=8; //<-base object
						OBJadd(x,y,myobj3);

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[4]=myobj3;
						myobj3->type=myobj2->type+8*1024; myobj3->info|=8; //<-base object
						OBJadd(x,y-1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[5]=myobj3;
						myobj3->type=myobj2->type+7*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj3);
						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[6]=myobj3;
						myobj3->type=myobj2->type-3*1024; myobj3->info|=8; //<-base object
						OBJadd(x-1,y,myobj3);



						//crt->more=myobj3;


						}

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

						if (obji[sprlnk[myobj2->type&1023]].v4==13){ //tangle vine

						static mlobj *mmyobj; //array size varies
						mmyobj=(mlobj*)malloc(sizeof(object*)*4);
						crt->more=mmyobj;
						myobj2->type=365; //(myobj2->type&1023)+3*1024;

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[0]=myobj3;
						myobj3->type=366+1*1024; //myobj3->info|=8; //<-base object
						OBJadd(myobj2->x,myobj2->y-1,myobj3);
						//myobj3->info|=4; myobj3->more=crt; //myobj2->more;
						myobj3->more=malloc(sizeof(creature));
						ZeroMemory(myobj3->more,sizeof(creature));
						crt2=(creature*)myobj3->more;
						crt2->crt_struct=TRUE;
						crt2->al=crt->al;
						crt2->hp=32767; //maximum allowed
						crt2->mp=0;
						crt2->more=myobj2;
						myobj3->info|=4; //<-crt
						crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[1]=myobj3;
						myobj3->type=366+1*1024; //myobj3->info|=8; //<-base object
						OBJadd(myobj2->x,myobj2->y+1,myobj3);
						//myobj3->info|=4; myobj3->more=crt; //myobj2->more;
						myobj3->more=malloc(sizeof(creature));
						ZeroMemory(myobj3->more,sizeof(creature));
						crt2=(creature*)myobj3->more;
						crt2->crt_struct=TRUE;
						crt2->al=crt->al;
						crt2->hp=32767; //maximum allowed
						crt2->mp=0;
						crt2->more=myobj2;
						myobj3->info|=4; //<-crt
						crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;

						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[2]=myobj3;
						myobj3->type=366; //myobj3->info|=8; //<-base object
						OBJadd(myobj2->x-1,myobj2->y,myobj3);
						//myobj3->info|=4; myobj3->more=crt; //myobj2->more;
						myobj3->more=malloc(sizeof(creature));
						ZeroMemory(myobj3->more,sizeof(creature));
						crt2=(creature*)myobj3->more;
						crt2->crt_struct=TRUE;
						crt2->al=crt->al;
						crt2->hp=32767; //maximum allowed
						crt2->mp=0;
						crt2->more=myobj2;
						myobj3->info|=4; //<-crt
						crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;


						myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[3]=myobj3;
						myobj3->type=366; //myobj3->info|=8; //<-base object
						OBJadd(myobj2->x+1,myobj2->y,myobj3);
						//myobj3->info|=4; myobj3->more=crt; //myobj2->more;

						myobj3->more=malloc(sizeof(creature));
						ZeroMemory(myobj3->more,sizeof(creature));
						crt2=(creature*)myobj3->more;
						crt2->crt_struct=TRUE;
						crt2->al=crt->al;
						crt2->hp=32767; //maximum allowed
						crt2->mp=0;
						crt2->more=myobj2;
						myobj3->info|=4; //<-crt
						crt2->respawn_hp=crt2->hp; crt2->respawn_x=myobj3->x; crt2->respawn_y=myobj3->y;

						//respawn info?


						}//tangle vine

						if ((myobj2->type&1023)==375) myobj2->info|=(3<<9); //slime divide!

						//respawn info
						crt->respawn_hp=crt->hp; crt->respawn_x=myobj2->x; crt->respawn_y=myobj2->y;

						*/

						if (myobj5) {myobj2=myobj5; goto multiple_enemy;}

						if (myobj4!=NULL) {myobj2=myobj4; goto extractobj2;}
eggskip:;
					}//egg


					newll=NULL;
				}


				if (myobj->type==78) myobj->more=NULL; //staff cannot hold original ultima6 type spells

				if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){ //set quantity
					myobj->more2=ol[i].qty; //more2<-
					if (myobj->more2==0) myobj->more2=1;
				}
				if ((myobj->type&1023)==64){ //key, set type!
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==58){ //spell, set type!
					myobj->more2=ol[i].tag; //more2<-
					if (spellname[myobj->more2]->l==0) myobj->more2=(0<<4)+7;
				}

				ENHANCEnewn(myobj,8,4);
				if ((x>=1940)&&(x<=1954)&&(y>=212)&&(y<=218)){ //treasure room
					if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){
						myobj->more2/=4; if (myobj->more2==0) myobj->more2=1;
					}
				}
				if (myobj->type==389) myobj->info|=256; //silver tablet
				if (myobj->type==390) myobj->info|=256; //silver tablet
				if (myobj->type==62) myobj->info|=256; //vortex cube <-quest item
				if (myobj->type==406) myobj->info|=256; //dungeon1 map <-quest item
				if ((myobj->type&1023)==73){
					myobj->info|=256; //moonstone <-quest item
					moongatex[myobj->type>>10]=x; moongatey[myobj->type>>10]=y-1;
					moongate[myobj->type>>10][0]=OBJnew(); moongate[myobj->type>>10][0]->type=85+1024;
					moongate[myobj->type>>10][1]=OBJnew(); moongate[myobj->type>>10][1]->type=85;
				}
				if ((myobj->type>=242)&&(myobj->type<=249)) myobj->info|=256; //rune <-quest item
				if (myobj->type==395) myobj->info|=256; //broken gargoyle lens <-quest item


				if ((myobj->type&1023)==182) myobj->info|=(15<<9); //beehive!

				//if (myobj->type==(98+2048)){  //locked chest
				//txtnumint(t5,ol[i].qty);
				//MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
				//myobj->more2=ol[i].tag; //more2<-
				//x2=1; //locked
				//}

				if ((myobj->type&1023)==332){ //sign
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==333){ //gargoyle sign
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==152){ //scroll
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==143){ //picture
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==151){ //book
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==333){ //gsign
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==59){ //codex
					myobj->more2=128;
				}
				if ((myobj->type&1023)==61){ //circles
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==254){ //cross
					myobj->more2=ol[i].tag; //more2<-
				}
				if ((myobj->type&1023)==255){ //tombstone
					myobj->more2=ol[i].tag; //more2<-
				}

				//skiff (stealing or not?)
				if ((myobj->type&1023)==414){
					//(horace's skiff, skiff on humility isle, skiff on north-west of Britannia, skiff for crossing to Justice town from east, skiffs underground)
					if (x>=1024) myobj->info|=(1<<9); //free skiff!
					if ((x==65)&&(y==492)) myobj->info|=(1<<9); //free skiff!
					if ((x==83)&&(y==177)) myobj->info|=(1<<9); //free skiff!
					if ((x==404)&&(y==90)) myobj->info|=(1<<9); //free skiff!
					if ((x==403)&&(y==94)) myobj->info|=(1<<9); //free skiff!
					if ((x==379)&&(y==89)) myobj->info|=(1<<9); //free skiff!
					if ((x==379)&&(y==93)) myobj->info|=(1<<9); //free skiff!
					if ((x==753)&&(y==715)) myobj->info|=(1<<9); //free skiff!
					//more?
				}

				x2=0; //skip add object to map
				if (i2==175) x2=1; //electric field
				if (i2==310) x2=1; //portcullis
				if (i2==335) x2=1; //egg
				if (x2==0) OBJadd(x,y,myobj);



				x2=ol[i].type>>10; //x2 sub-index
				i4=sprlnk[i2]+x2;

				if ((myobj->type&1023)==338){ //blood (remove&insert)
					if (myobj3=(object*)myobj->prev){
						if ((myobj3->type&1023)==339){
							OBJremove(myobj); OBJremove(myobj3);
							OBJadd(x,y,myobj); OBJadd(x,y,myobj3);
						}//339
					}
				}//338



				//x x-pos
				//y y-pos
				//i2=myobj->type&1023
				//x2=myobj->type>>10
				//i4=sprite index




				//if ((i2==412)&&((ol[i].type>>11)&2)==2) exit(myobj->x);

				//fix ladders
				if (i2==305){
					if ((x>=1024)&&(y<=256)){ //dungeon ladders
						if (x2==0){ //down
							myobj->more2=myobj->x+256+(myobj->y<<16);
							if (x>=(1024+768)) myobj->more2=myobj->x-768+((myobj->y+256)<<16);
						}else{ //up
							myobj->more2=myobj->x-256+(myobj->y<<16);
						}
					}




					if ((x==291)&&(y==347)){ //lb1
						x3=1099+(83<<16); myobj->more2=x3;}
					if ((x==1099)&&(y==83)){
						x3=291+(347<<16); myobj->more2=x3;}
					if ((x==(291+32))&&(y==347)){
						x3=(1099+8)+(83<<16); myobj->more2=x3;}
					if ((x==(1099+8))&&(y==83)){
						x3=(291+32)+(347<<16); myobj->more2=x3;}
					if ((x==(291+32))&&(y==(347+32))){
						x3=(1099+8)+((83+8)<<16); myobj->more2=x3;}
					if ((x==(1099+8))&&(y==(83+8))){
						x3=(291+32)+((347+32)<<16); myobj->more2=x3;}
					if ((x==(291))&&(y==(347+32))){
						x3=(1099)+((83+8)<<16); myobj->more2=x3;}
					if ((x==(1099))&&(y==(83+8))){
						x3=(291)+((347+32)<<16); myobj->more2=x3;}

					x3=707; y3=835; x4=(179+1024); y4=211; //treasure hole
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=73; y3=539; x4=1041; y4=131; //cellar
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=340; y3=433; x4=1108; y4=105; //britan hotel
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=930; y3=210; x4=1258; y4=50; //map under harpsichord ladder
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=868; y3=347; x4=1244; y4=83; //catacombs/dead remains
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=891; y3=435; x4=1243; y4=107; //library
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=787; y3=948; x4=1219; y4=236; //Sutek's castle
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}


					x3=605; y3=285; x4=1173; y4=69; //cyclops
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}
					x3=609; y3=285; x4=1177; y4=69; //cyclops
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}
					x3=607; y3=287; x4=1175; y4=71; //cyclops
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}
					x3=605; y3=289; x4=1173; y4=73; //cyclops
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}
					x3=609; y3=289; x4=1177; y4=73; //cyclops
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}


					x3=234; y3=409; x4=1082; y4=97; //mine west of britan
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=185; y3=436; x4=1065; y4=108; //cave west of britan
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=92; y3=250; x4=1044; y4=58; //spider cave west of empath abby
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=500; y3=81; x4=1148; y4=17; //WESTERN dungeon "WRONG", east of skiff bridge crossing
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=627; y3=113; x4=1179; y4=25; //EASTERN dungeon "WRONG", east of skiff bridge crossing
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=835; y3=195; x4=1235; y4=51; //WESTERN ant hole
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=867; y3=187; x4=1243; y4=43; //EAST ant hole
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=964; y3=306; x4=1268; y4=74; //spike and trap cave
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=948; y3=930; x4=1260; y4=234; //"HYTHLOTH"
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=348; y3=809; x4=1108; y4=201; //"Heroes' Hole" west of trisnic
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=132; y3=857; x4=1060; y4=209; //"Heftimus Cave" sherry town
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=284; y3=657; x4=1092; y4=161; //"Destard" west of trisnic
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=365; y3=265; x4=1117; y4=65; //"Despise" north of britain
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=611; y3=363; x4=1179; y4=91; //swamp cave
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=564; y3=594; x4=1164; y4=146; //buckeneers den
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

					x3=942; y3=521; x4=1262; y4=129; //remains2
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
					if ((x==x4)&&(y==y4)){myobj->more2=x3+(y3<<16);}

				}//ladder type


				if (i2==326){ //dungeon entrances
					if (y>=256) myobj->more2=myobj->x+768+((myobj->y-256)<<16);

					x3=234; y3=409; x4=1082; y4=97; //mine west of britan
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=185; y3=436; x4=1065; y4=108; //cave west of britan
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=92; y3=250; x4=1044; y4=58; //spider cave west of empath abby
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=500; y3=81; x4=1148; y4=17; //dungeon "Wrong", east of skiff bridge crossing
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=627; y3=113; x4=1179; y4=25; //EASTERN dungeon "Wrong", east of skiff bridge crossing
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=964; y3=306; x4=1268; y4=74; //spike and trap cave
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=948; y3=930; x4=1260; y4=234; //"Hythloth Dungeon"
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=348; y3=809; x4=1108; y4=201; //"Heroes' Hole" west of trisnic
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=132; y3=857; x4=1060; y4=209; //"Heftimus Cave" sherry town
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=284; y3=657; x4=1092; y4=161; //"Destard" west of trisnic
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=365; y3=265; x4=1117; y4=65; //"Despise" north of britain
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=611; y3=363; x4=1179; y4=91; //swamp cave
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=564; y3=594; x4=1164; y4=146; //buckeneers den
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

				}

				if (i2==308){ //hole entrances

					if ((x>=1024)&&(y<=256)){ //dungeon holes
						myobj->more2=myobj->x+256+(myobj->y<<16);
						if (x>=(1024+768)) myobj->more2=myobj->x-768+((myobj->y+256)<<16);
					}

					x3=835; y3=195; x4=1235; y4=51; //WESTERN ant hole
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}

					x3=867; y3=187; x4=1243; y4=43; //EAST ant hole
					if ((x==x3)&&(y==y3)){myobj->more2=x4+(y4<<16);}
				}

				myobj2=myobj; //backup original object


				if (i2==412){ //boat
					x3=(x2>>1)&3; //0-3

					static mlobj *mmyobj; //array size varies
					mmyobj=(mlobj*)malloc(sizeof(object*)*5);
					myobj2->more=mmyobj;

					if (x3==0){
						myobj=OBJnew(); mmyobj->obj[0]=myobj; //middle
						myobj->type=ol[i].type-1*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[1]=myobj; //front
						myobj->type=ol[i].type-8*1024; myobj->info|=8; //<-base object
						OBJadd(x,y-1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[2]=myobj;
						myobj->type=ol[i].type-9*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[3]=myobj; //back
						myobj->type=ol[i].type+8*1024; myobj->info|=8; //<-base object
						OBJadd(x,y+1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[4]=myobj;
						myobj->type=ol[i].type+7*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y+1,myobj); myobj->more=myobj2;
					}
					if (x3==1){
						myobj=OBJnew(); mmyobj->obj[0]=myobj; //middle
						myobj->type=ol[i].type-1*1024; myobj->info|=8; //<-base object
						OBJadd(x,y-1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[1]=myobj; //front
						myobj->type=ol[i].type-8*1024; myobj->info|=8; //<-base object
						OBJadd(x+1,y,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[2]=myobj;
						myobj->type=ol[i].type-9*1024; myobj->info|=8; //<-base object
						OBJadd(x+1,y-1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[3]=myobj; //back
						myobj->type=ol[i].type+8*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[4]=myobj;
						myobj->type=ol[i].type+7*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj); myobj->more=myobj2;
					}
					if (x3==2){
						myobj=OBJnew(); mmyobj->obj[0]=myobj; //middle
						myobj->type=ol[i].type-1*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[1]=myobj; //front
						myobj->type=ol[i].type-8*1024; myobj->info|=8; //<-base object
						OBJadd(x,y+1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[2]=myobj;
						myobj->type=ol[i].type-9*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y+1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[3]=myobj; //back
						myobj->type=ol[i].type+8*1024; myobj->info|=8; //<-base object
						OBJadd(x,y-1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[4]=myobj;
						myobj->type=ol[i].type+7*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj); myobj->more=myobj2;
					}
					if (x3==3){
						myobj=OBJnew(); mmyobj->obj[0]=myobj; //middle
						myobj->type=ol[i].type-1*1024; myobj->info|=8; //<-base object
						OBJadd(x,y-1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[1]=myobj; //front
						myobj->type=ol[i].type-8*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[2]=myobj;
						myobj->type=ol[i].type-9*1024; myobj->info|=8; //<-base object
						OBJadd(x-1,y-1,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[3]=myobj; //back
						myobj->type=ol[i].type+8*1024; myobj->info|=8; //<-base object
						OBJadd(x+1,y,myobj); myobj->more=myobj2;
						myobj=OBJnew(); mmyobj->obj[4]=myobj;
						myobj->type=ol[i].type+7*1024; myobj->info|=8; //<-base object
						OBJadd(x+1,y-1,myobj); myobj->more=myobj2;
					}









				}


				if (i4==(sprlnk[163]+1)){ //h bed
					myobj=OBJnew();
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[163]+7)){ //v bed
					myobj=OBJnew();
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[176]+1)){ //h draw
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj->info|=8;
				}
				if (i4==(sprlnk[176]+3)){ //v draw
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
					myobj->info|=8;
				}

				i5=85; //blue gate
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				i5=84; //red gate
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}

				i5=325; //bookstand
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				i5=134; //roast horse carcass
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024; myobj->info|=8;
					OBJadd(x-1,y,myobj);
				}

				i5=168; //remains
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024; myobj->info|=8;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024; myobj->info|=8;
					OBJadd(x,y-1,myobj);
				}

				i5=136; //skewer
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew(); myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				i5=326; //mineshaft
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew(); myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew(); myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+5)){
					myobj=OBJnew(); myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}

				i5=263; //stocks
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					if ((x==253)&&(y==169)) {myobj2->type=263+3*1024; myobj->type=263+2*1024;} //man in stocks
				}
				i5=327; //throne
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}

				i5=312; //lions
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				i5=313; //horn
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}

				/*
				i5=310; //portcullis h&v
				if (i4==(sprlnk[i5]+1)){
				myobj=OBJnew();
				myobj->more=myobj2;
				myobj->type=ol[i].type-1*1024;
				OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
				myobj=OBJnew();
				myobj->more=myobj2;
				myobj->type=ol[i].type-1*1024;
				OBJadd(x,y-1,myobj);
				}
				*/

				i5=278;
				if (i4==(sprlnk[i5]+2)){ //passthrough
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				i5=280;
				if (i4==(sprlnk[i5]+2)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}

				i5=309; //bones
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+4)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}

				i5=311; //stone table
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}



				i5=302; //archs
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}


				//doors (oaken,...)
				i5=297;
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}

				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+5)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+7)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+9)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+11)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);

					//unlock peer's door
					if ((x==152)&&(y==891)){
						myobj->type-=4*1024;
						myobj2->type-=4*1024;
					}

				}
				if (i4==(sprlnk[i5]+13)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+15)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}

				i5=298;
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+5)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					//exit(9);
				}
				if (i4==(sprlnk[i5]+7)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+9)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+11)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+13)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+15)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}

				i5=299;
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+5)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+7)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+9)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+11)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+13)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+15)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}

				i5=300;
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+5)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+7)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+9)){
					//if (ol[i].tag==1) exit(y);
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+11)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+13)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+15)){
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj2->more=myobj; //back link
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}



				//lever pointer
				i5=268; //lever type
				if ((myobj2->type&1023)==i5){
					if (ol[i].tag!=0){
						for (i6=0;i6<=15;i6++){
							if (lever[ol[i].tag][i6]==NULL) {lever[ol[i].tag][i6]=myobj2; goto mullev;}
						}
mullev:;
					}
				}
				//switch pointer
				i5=174; //switch type
				if ((myobj2->type&1023)==i5){
					if (ol[i].tag!=0){
						for (i6=0;i6<=15;i6++){
							if (eswitch[ol[i].tag][i6]==NULL) {eswitch[ol[i].tag][i6]=myobj2; goto mullev2;}
						}
mullev2:;
					}
				}

				//door arches
				i5=301;

				if ((myobj2->type&1023)==i5){
					if (ol[i].tag!=0){
						for (i6=0;i6<=15;i6++){
							if (portcullis[ol[i].tag][i6]==NULL) {portcullis[ol[i].tag][i6]=myobj2;goto gotport;}
						}
gotport:;
					}
				}

				//2003: doorway, doesn't need back linking, stores value of door lock in more2

				if (i4==(sprlnk[i5]+1)){
					myobj2->more2=ol[i-1].tag;
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+3)){
					myobj2->more2=ol[i-1].tag;
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+5)){
					myobj2->more2=ol[i-1].tag;
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+7)){
					myobj2->more2=ol[i-1].tag;
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+9)){
					myobj2->more2=ol[i-1].tag;
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				if (i4==(sprlnk[i5]+11)){
					myobj2->more2=ol[i-1].tag;
					myobj=OBJnew();
					myobj->more=myobj2; myobj->info|=8;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}


				i5=304; //fireplace
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				i5=304; //fireplace
				if (i4==(sprlnk[i5]+5)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				i5=304; //fireplace
				if (i4==(sprlnk[i5]+3)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}

				i5=304; //fireplacev
				if (i4==(sprlnk[i5]+7)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);
				}
				i5=304; //fireplace
				if (i4==(sprlnk[i5]+11)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				i5=304; //fireplace
				if (i4==(sprlnk[i5]+9)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}

				i5=144; //wall banner
				if (i4==(sprlnk[i5]+1)){
					myobj=OBJnew();
					myobj->more=myobj2;
					myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
				}
				i5=159; //clock
				if (i4==(sprlnk[i5]+1)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				i5=162; //stove
				if (i4==(sprlnk[i5]+1)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				i5=216; //bookcase
				if (i4==(sprlnk[i5]+1)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+5)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+7)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				if (i4==(sprlnk[i5]+9)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				if (i4==(sprlnk[i5]+11)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}



				i5=331; //sign
				if (i4==(sprlnk[i5]+1)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+5)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+7)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+9)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+11)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);}
				if (i4==(sprlnk[i5]+13)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				if (i4==(sprlnk[i5]+15)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				if (i4==(sprlnk[i5]+17)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				if (i4==(sprlnk[i5]+19)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}
				if (i4==(sprlnk[i5]+21)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x,y-1,myobj);}


				i5=341; //4xdead flying gargoyle
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024; myobj->info|=8;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024; myobj->info|=8;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024; myobj->info|=8;
					OBJadd(x-1,y-1,myobj);
				}
				i5=340; //4xdead cyclops
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024; myobj->info|=8;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024; myobj->info|=8;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024; myobj->info|=8;
					OBJadd(x-1,y-1,myobj);
				}

				i5=324; //4xpillar
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=322; //4xwater pool
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=329; //4xsingularity
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}

				i5=323; //4xmonolith
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=328; //4xalter
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=321; //4xstatue
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+7)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+11)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}


				i5=195;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=196;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=197;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=201;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=214;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=233;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=316;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=330;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=200;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}

				i5=198;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				i5=199;
				if (i4==(sprlnk[i5]+3)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}

				i5=267; //gilotene
				if (i4==(sprlnk[i5]+4)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}

				i5=163; //nicebed
				if (i4==(sprlnk[i5]+5)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}
				if (i4==(sprlnk[i5]+11)){ 
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-1*1024;
					OBJadd(x-1,y,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-2*1024;
					OBJadd(x,y-1,myobj);
					myobj=OBJnew(); myobj->more=myobj2; myobj->type=ol[i].type-3*1024;
					OBJadd(x-1,y-1,myobj);
				}


skipobj:;
			} //1st pass exception
		}} //x<1024,y<1024
	} //flag check

	//skipitem:; //UNUSED?
} //i loop


//fix portcullis properties
for (i=0;i<=255;i++){

	myobj=lever[i][0];
	if (myobj!=NULL){
		myobj2=myobj; //prev
		for (i2=0;i2<=15;i2++){
			if (portcullis[i][i2]){

				myobj3=OBJnew(); //add gate
				myobj2->more=myobj3;
				x=portcullis[i][i2]->x;
				y=portcullis[i][i2]->y;
				x2=(portcullis[i][i2]->type>>10)&2;

				myobj3->type=310+1024;
				if (x2!=0) myobj3->type=310+1024*3;

				OBJadd(x,y,myobj3);

				myobj4=OBJnew(); //add gate
				myobj3->more=myobj4;
				myobj4->type=310;
				x--;
				if (x2!=0) {myobj4->type=310+1024*2; x++; y--;}
				OBJadd(x,y,myobj4);

				myobj2=myobj4;




			}//port!=NULL
		}//i2

	} //!=null



	//2!
	myobj=eswitch[i][0];
	if (myobj!=NULL){
		myobj2=myobj; //prev
		for (i2=0;i2<=15;i2++){
			if (portcullis[i][i2]){

				myobj3=OBJnew(); //add gate
				myobj2->more=myobj3;
				x=portcullis[i][i2]->x;
				y=portcullis[i][i2]->y;
				//x2=(portcullis[i][i2]->type>>10)&2;
				myobj3->type=175; //field
				OBJadd(x,y,myobj3);

				myobj2=myobj3;
			}//port!=NULL
		}//i2
	} //!=null
} //i


for (i=0;i<=255;i++){
	for (i6=1;i6<=15;i6++){
		if (lever[i][i6]!=NULL)
			lever[i][i6]->more=lever[i][0]->more;
	}
}























/****************
//add internal objects to the map, backwards scan
for (i=0;i<=oln-1;i++){
x=ol[i].unk_h;
y=ol[i].unk_d1;
x2=y&3;
x=x+x2*256;
y=y/4;
y=y+(ol[i].unk_d2&15)*64;
i6=ol[i].unk_d2>>4; //level

if ((ol[i].status&16)==0){
if ((ol[i].status<=1)||(ol[i].status&8)){
if (ol[i].status&8){
x=ox;
y=oy;
goto yyyy;
}
//get new x and y co-ordinates
if (x<1024){ if (y<1024){
if (i6==1) x=x+1024;
if (i6==2) x=x+1024+256;
if (i6==3) x=x+1024+256*2;
if (i6==4) x=x+1024+256*3;
if (i6==5) {x=x+1024; y=y+256;}
ox=x; oy=y;

goto skipitem;
yyyy:
i2=ol[i].type&1023; //get object identity

//i2<=334
if ((i2<=375)||(i2>=411)){ //exclude creatures/characters

if ((i2<29)||(i2>32)){ //exclude first pass objects
myobj=OBJnew();
myobj->type=ol[i].type;
OBJadd(x,y,myobj);
skipitem:;

} //1st pass exception
} //exclude creatures
}}
} //flag check
}
}
*/
















for (y=0;y<=1023;y++){
	for (x=0;x<=2047;x++){
		OBJcheckflags(x,y);
	}}


//setup NPC
for (i=0;i<=255;i++){
	//if (i==130) goto smith;
	if (schedule[i][0].hour!=255){
		x=schedule[i][0].x; y=schedule[i][0].y;

		//if (schedule[i][0].z==0){


		//minax exception!

		//smith:
		myobj=OBJnew();


		f_ouln++;
		f_oul[f_ouln]=myobj;




		//tfh=open(".\\host\\npcobj.bin"); get(tfh,&npcobj,lof(tfh)); close(tfh);
		//myobj->type=386; //person male/female
		myobj->type=npcobj[i]; //person male/female

		tnpc=(npc*)malloc(sizeof(npc)); ZeroMemory(tnpc,sizeof(npc));

		myobj->more=tnpc; myobj->info|=2; //<-
		myobj2=OBJnew(); myobj2->type=188; tnpc->items=myobj2; //(not included)bag
		tnpc->port=i+1;
		tnpc->converse=i;
		tnpc->name=txtnew(); txtset(tnpc->name,"Unknown"); //*retrieve from npc.inf
		tnpc->schedule=&schedule[i][0];
		tnpc->schedule_i=-1; //unknown
		tnpc->order=2; //schedule

		//create an INVISIBLE container for player's items
		myobj2=OBJnew();
		myobj2->type=188; //bag (INVISIBLE)
		tnpc->items=myobj2;
		tnpc->baseitem=myobj2;





		if (i==0){
			tnpc->name=txtnew(); txtset(tnpc->name,"Dupre");
			//tnpc->player=playerlist[i];
			tnpc->upflags|=1; //full update
			tnpc->hp=90; tnpc->hp_max=90;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=26; tnpc->d=20; tnpc->i=17;
			tnpc->lev=3; tnpc->exp=374;
			//tnpc->wt=0; tnpc->wt_max=tnpc->s*2*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*100;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
			tnpc->converse=255;
		}

		if (i==1){
			tnpc->name=txtnew(); txtset(tnpc->name,"Shamino");
			//tnpc->player=playerlist[i];
			tnpc->upflags|=1; //full update
			tnpc->hp=90; tnpc->hp_max=90;
			tnpc->mp=9; tnpc->mp_max=9;
			tnpc->s=21; tnpc->d=23; tnpc->i=18;
			tnpc->lev=3; tnpc->exp=399;
			//tnpc->wt=0; tnpc->wt_max=tnpc->s*2*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*100;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==2){
			tnpc->name=txtnew(); txtset(tnpc->name,"Iolo");
			//tnpc->player=playerlist[i];
			tnpc->upflags|=1; //full update
			tnpc->hp=90; tnpc->hp_max=90;
			tnpc->mp=8; tnpc->mp_max=8;
			tnpc->s=20; tnpc->d=26; tnpc->i=17;
			tnpc->lev=3; tnpc->exp=424;
			//tnpc->wt=0; tnpc->wt_max=tnpc->s*2*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*100;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==3){//Lord British
			myobj3=OBJnew();
			myobj3->type=75;//silver snake amulet
			myobj3->info|=128;//stealing!
			tnpc->neck=myobj3;
			myobj3=OBJnew();
			myobj3->type=256;//protection ring
			myobj3->info|=128;//stealing!
			tnpc->ring_right=myobj3;
			myobj3=OBJnew();
			myobj3->type=257;//regeneration ring
			myobj3->info|=128;//stealing!
			tnpc->ring_left=myobj3;
			myobj3=OBJnew();
			myobj3->type=27;//leather boots
			myobj3->info|=128;//stealing!
			tnpc->boots=myobj3;
		}


		if (i==7){
			tnpc->name=txtnew(); txtset(tnpc->name,"Sherry");
			tnpc->upflags|=1; //full update
			tnpc->hp=30; tnpc->hp_max=30;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=1; tnpc->d=27; tnpc->i=12;
			tnpc->lev=1; tnpc->exp=73;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==60){
			tnpc->name=txtnew(); txtset(tnpc->name,"Jaana");
			tnpc->upflags|=1; //full update
			tnpc->hp=120; tnpc->hp_max=120;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=16; tnpc->d=21; tnpc->i=19;
			tnpc->lev=4; tnpc->exp=450;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==64){
			tnpc->name=txtnew(); txtset(tnpc->name,"Gwenno");
			tnpc->upflags|=1; //full update
			tnpc->hp=60; tnpc->hp_max=60;
			tnpc->mp=8; tnpc->mp_max=8;
			tnpc->s=18; tnpc->d=21; tnpc->i=17;
			tnpc->lev=2; tnpc->exp=160;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}


		if (i==95){
			tnpc->name=txtnew(); txtset(tnpc->name,"Katrina");
			tnpc->upflags|=1; //full update
			tnpc->hp=150; tnpc->hp_max=150;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=19; tnpc->d=16; tnpc->i=16;
			tnpc->lev=5; tnpc->exp=840;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==111){
			tnpc->name=txtnew(); txtset(tnpc->name,"Leodon");
			tnpc->upflags|=1; //full update
			tnpc->hp=90; tnpc->hp_max=90;
			tnpc->mp=7; tnpc->mp_max=7;
			tnpc->s=15; tnpc->d=19; tnpc->i=15;
			tnpc->lev=3; tnpc->exp=320;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==112){
			tnpc->name=txtnew(); txtset(tnpc->name,"Leonna");
			tnpc->upflags|=1; //full update
			tnpc->hp=90; tnpc->hp_max=90;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=15; tnpc->d=15; tnpc->i=18;
			tnpc->lev=3; tnpc->exp=270;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==129){
			tnpc->name=txtnew(); txtset(tnpc->name,"Gorn");
			tnpc->upflags|=1; //full update
			tnpc->hp=120; tnpc->hp_max=120;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=26; tnpc->d=21; tnpc->i=14;
			tnpc->lev=4; tnpc->exp=650;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==135){
			tnpc->name=txtnew(); txtset(tnpc->name,"Blaine");
			tnpc->upflags|=1; //full update
			tnpc->hp=60; tnpc->hp_max=60;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=16; tnpc->d=21; tnpc->i=18;
			tnpc->lev=2; tnpc->exp=120;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}


		if (i==160){
			tnpc->name=txtnew(); txtset(tnpc->name,"Seggallion");
			tnpc->upflags|=1; //full update
			tnpc->hp=150; tnpc->hp_max=150;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=28; tnpc->d=21; tnpc->i=20;
			tnpc->lev=5; tnpc->exp=850;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==162){
			tnpc->name=txtnew(); txtset(tnpc->name,"Beh Lem");
			tnpc->upflags|=1; //full update
			tnpc->hp=60; tnpc->hp_max=60;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=23; tnpc->d=24; tnpc->i=26;
			tnpc->lev=2; tnpc->exp=120;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}

		if (i==184){
			tnpc->name=txtnew(); txtset(tnpc->name,"Sentri");
			tnpc->upflags|=1; //full update
			tnpc->hp=90; tnpc->hp_max=90;
			tnpc->mp=0; tnpc->mp_max=0;
			tnpc->s=26; tnpc->d=18; tnpc->i=16;
			tnpc->lev=3; tnpc->exp=250;
			tnpc->wt=0; tnpc->wt_max=tnpc->s*3*100; tnpc->wt2=0; tnpc->wt2_max=tnpc->s*50;
		}


		if (i==130){ //smith
			myobj->type=430+6*1024;
			OBJadd(x+1,y,myobj);
			myobj3=OBJnew(); myobj3->type=430+14*1024;
			myobj3->more=myobj; myobj3->info|=8;
			OBJadd(x+2,y,myobj3);
			tnpc->more=myobj3;
		}

		if (i==128){ //pushmepullyou
			myobj->type=430+6*1024;
			OBJadd(x,y-1,myobj);
			myobj3=OBJnew(); myobj3->type=430+14*1024;
			myobj3->more=myobj; myobj3->info|=8;
			OBJadd(x+1,y-1,myobj3);
			tnpc->more=myobj3;
		}

		//1475 243
		//1475 244

		myobj2=myobj;
		if (obji[sprlnk[myobj2->type&1023]].v4==5){ //winged gargoyle
			OBJadd(x,y,myobj);
			static mlobj *mmyobj; //array size varies
			mmyobj=(mlobj*)malloc(sizeof(object*)*3);
			tnpc->more=mmyobj;
			myobj2->type=(myobj2->type&1023)+3*1024;
			myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[0]=myobj3;
			myobj3->type=myobj2->type-3*1024; myobj3->info|=8; //<-base object
			OBJadd(x-1,y-1,myobj3);
			myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[1]=myobj3;
			myobj3->type=myobj2->type-2*1024; myobj3->info|=8; //<-base object
			OBJadd(x,y-1,myobj3);
			myobj3=OBJnew(); myobj3->more=myobj2; mmyobj->obj[2]=myobj3;
			myobj3->type=myobj2->type-1*1024; myobj3->info|=8; //<-base object
			OBJadd(x-1,y,myobj3);
		}

		OBJmove_allow=TRUE;
		OBJmove2(myobj,x,y); //add to map, <-needs NPC pointer
		OBJmove_allow=FALSE;

	}//z

}

//exit(1);

//load them into the permanent update array
//give them the ORDER ->follow schedule

ZeroMemory(&schedule2,sizeof(schedule2));
/* load all hireling names to memory and store them somewhere, so that its not neccessary to read them from files more than once */
for (y5=0;y5<=HIRELINGS_MAX-1;y5++){
  hirl_obj[y5]=0;
  hirl_wait[y5]=0;
  addhireling(y5,0);
}//y5

i=-1; cmdline_length2: i++; if ((unsigned char*)lpCmdLine[i]) goto cmdline_length2;
if (i){
	txtNEWLEN(t,-i); memcpy(t->d2,lpCmdLine,i);
	txtset(t2,"dump"); if (txtsearch(t,t2)){
		save_dump=1;
		tfh3=open2(".\\save\\save.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
	}
}

//setup save buffer & delete duplicate savefiles

for (i=0;i<=SAVESLOTLAST;i++){
	save_buffer[i]=0; save_username[i]=txtnew(); save_password[i]=txtnew(); save_name[i]=txtnew(); save_exp[i]=0;
	txtset(t2,".\\save\\"); txtnumint(t,i); txtadd(t,".sav"); txtadd(t2,t); //t2=filename (i.sav)
	tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT);
	if (tfh->h==HFILE_ERROR){
		save_buffer[i]=0;
	}else{
		if (lof(tfh)==0){save_buffer[i]=0; close(tfh); goto save_0bytes;}
		save_buffer[i]=1;
		txtNEWLEN(t4,save_bytes[i]=lof(tfh)); get(tfh,t4->d2,t4->l);
		static unsigned long save_lof; save_lof=lof(tfh);
		close(tfh);
		i2=t4->ds[0]; //savegame version

		if (save_dump){
			txtset(t8,"?"); t8->d2[0]=255;
			txtnumint(t6,i); txtset(t7,".SAV"); txtadd(t6,t7); txtadd(t6,t8);
			txtadd(t6,"VERSION:");
			txtnumint(t7,i2); txtadd(t6,t7); txtadd(t6,t8);
		}

		if (i2==3){ //DECRYPT3.0
		 decrypt(t4);
		}

		txtright(t4,t4->l-2); //version
		if (i2==5){
            txtright(t4,t4->l-1);//for now do nothing with pw encrypt version will be useful later if some one implements a new encryption
        }
		x=t4->d2[0]; txtright(t4,t4->l-1); txtset(save_username[i],t4); txtleft(save_username[i],x); txtright(t4,t4->l-x);
		x=t4->d2[0]; txtright(t4,t4->l-1); txtset(save_password[i],t4); txtleft(save_password[i],x); txtright(t4,t4->l-x);

		if (save_dump){ //savegame will be converted to text, but no edit/quarantine will occur
			txtadd(t6,"USERNAME:");
			txtadd(t6,save_username[i]); txtadd(t6,t8);
			txtadd(t6,"USERPASSWORD:");
			x2=0;
			txtset(t7,save_password[i]); 
			for (x=0;x<t7->l;x++){
				x2+=t7->d2[x];
				if (x) t7->d2[x]=42;
			}
			txtadd(t6,t7);
			txtnumint(t7,x2);
			txtadd(t6,t7);
			txtadd(t6,t8);
			txtright(t4,t4->l-(2057-4-1024));
			//global flags
			x=t4->dl[249];
			if (x){
				txtadd(t6,"CREATECHARACTER:");
				x2=x/(32*12);
				x-=(x2*32*12);
				txtnumint(t7,x2);
				txtadd(t6,t7);
				txtadd(t6,"/");
				x2=x/32;
				x-=(x2*32);
				txtnumint(t7,x2+1);
				txtadd(t6,t7);
				txtadd(t6,"/");
				txtnumint(t7,x+1);
				txtadd(t6,t7);
				txtadd(t6,t8);
			}else{
				txtadd(t6,"CREATECHARACTER:?/?/?"); txtadd(t6,t8);
			}
			x=t4->dl[250];
			if (x){
				txtadd(t6,"LASTSAVE:");
				x2=x/(32*12);
				x-=(x2*32*12);
				txtnumint(t7,x2);
				txtadd(t6,t7);
				txtadd(t6,"/");
				x2=x/32;
				x-=(x2*32);
				txtnumint(t7,x2+1);
				txtadd(t6,t7);
				txtadd(t6,"/");
				txtnumint(t7,x+1);
				txtadd(t6,t7);
				txtadd(t6,t8);
			}else{
				txtadd(t6,"LASTSAVE:?/?/?"); txtadd(t6,t8);
			}
			txtadd(t6,"LOGIN#:");
			if (t4->dl[251]){
				txtnumint(t7,t4->dl[251]);
				txtadd(t6,t7);
			}else{
				txtadd(t6,"?");
			}
			txtadd(t6,t8);
			txtadd(t6,"PARTYMEMBERS:");
			txtnumint(t7,t4->dl[252]+1);
			txtadd(t6,t7);
			txtadd(t6,t8);

			txtadd(t6,"IP:");
			if (x=t4->dl[242]){
				txtnumint(t7,x&255); txtadd(t6,t7); txtadd(t6,".");
				txtnumint(t7,(x>>8)&255); txtadd(t6,t7); txtadd(t6,".");
				txtnumint(t7,(x>>16)&255); txtadd(t6,t7); txtadd(t6,".");
				txtnumint(t7,(x>>24)&255); txtadd(t6,t7);
			}else{
				txtadd(t6,"?");
			}
			txtadd(t6,t8);

			txtright(t4,t4->l-1024);
			txtadd(t6,"KARMA:");
			txtnumint(t7,t4->df[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-4);


			if (i2==1){
				txtright(t4,t4->l-8); //skip mobj in version 1
			}
			txtadd(t6,"TYPE:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //type
			txtadd(t6,"NAME:");
			x=t4->d2[0]; txtright(t4,t4->l-1); txtset(t5,t4); txtleft(t5,x); txtright(t4,t4->l-x); //name
			txtadd(t6,t5); txtadd(t6,t8);
			txtadd(t6,"PORT:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //port
			txtadd(t6,"HP:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //hp
			txtadd(t6,"HP_MAX:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //hp_max
			txtadd(t6,"MP:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //mp
			txtadd(t6,"MP_MAX:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //mp_max
			txtadd(t6,"STR:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //s
			txtadd(t6,"DEX:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //d
			txtadd(t6,"INT:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //i
			txtadd(t6,"EXP:");
			txtnumint(t7,t4->dl[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-4); //exp
			txtadd(t6,"LEV:");
			txtnumint(t7,t4->ds[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-2); //lev
			txtadd(t6,"FLAGS:");
			txtnumint(t7,t4->d2[0]);
			txtadd(t6,t7); txtadd(t6,t8);
			txtright(t4,t4->l-1); //flags
			if (i2!=1){
				txtright(t4,t4->l-10*4);
				//extract objects
				txtadd(t6,"ITEMS:");
dump_item:
				if (x=t4->dl[0]){
					x=t4->ds[0];
					x2=objname[(x&1023)*2];
					x3=objname[(x&1023)*2+1];
					txtNEWLEN(t7,x3);
					memcpy(&t7->d2[0],&objname2[x2],x3);
					txtadd(t6,t7); 
					txtadd(t6,"(");
					txtnumint(t7,t4->ds[0]);
					txtadd(t6,t7);
					txtadd(t6,";");
					txtnumint(t7,t4->ds[1]);
					txtadd(t6,t7);
					txtadd(t6,"|");
					txtright(t4,t4->l-4);
					txtnumint(t7,t4->dl[0]);
					txtadd(t6,t7);
					txtadd(t6,"),");
					//more2
					txtright(t4,t4->l-4);
					goto dump_item;
				}
				txtright(t4,t4->l-4);
				if (t4->dl[0]) goto dump_item;
				txtadd(t6,"END");
			}else{
				txtadd(t6,"MESSAGE: CANNOT EXTRACT OBJECT INFO FROM VERSION 1 SAVEGAME!");
			}
			txtfileout(t6,tfh3);
			goto save_dump_finished;
		}//save_dump

		//find name
		if (i2==1) txtright(t4,t4->l-2067); else txtright(t4,t4->l-2059); //skip unrequired info
		x=t4->d2[0]; txtright(t4,t4->l-1); txtset(save_name[i],t4); txtleft(save_name[i],x);
		//find xp (used to determine who gets the roman numeral)
		txtright(t4,t4->l-(x+16)); save_exp[i]=t4->dl[0];



		//auto-format name, username & password (when comparing names temp. ucase)
		//name
		txtset(t5,save_name[i]);
autoformat_name_cull:
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
			if (x==95){t5->d2[i3]=32; x=32; x2=0; goto autoformat_name_cull;} //_ to space
			if ((x==32)&&(t5->d2[i3+1]!=32)){
				if (i3&&(i3!=(t5->l-1))) x2=0;
			}
			if (x2){
				txtset(t6,t5); txtright(t6,t5->l-i3-1);
				txtleft(t5,i3); txtadd(t5,t6);
				goto autoformat_name_cull;
			}
		}
		if (t5->l>16) txtleft(t5,16);
		txtset(save_name[i],t5);
		//username
		txtset(t5,save_username[i]);
		txtucase(t5);
autoformat_username_cull:
		for (i3=0;i3<t5->l;i3++){
			x=t5->d2[i3];
			x2=0; //cull?
			if (x<48) x2=1;
			if (x>90) x2=1;
			if ((x>57)&&(x<65)) x2=1;
			if (x2){
				txtset(t6,t5); txtright(t6,t5->l-i3-1);
				txtleft(t5,i3); txtadd(t5,t6);
				goto autoformat_username_cull;
			}
		}
		if (t5->l>16) txtleft(t5,16);
		txtset(save_username[i],t5);
		//password
		if (i2<5){ //format pword only with no pw encryption
		  txtset(t5,save_password[i]);
		  txtucase(t5);
autoformat_password_cull:
		  for (i3=0;i3<t5->l;i3++){
			  x=t5->d2[i3];
			  x2=0; //cull?
			  if (x<48) x2=1;
			  if (x>90) x2=1;
			  if ((x>57)&&(x<65)) x2=1;
			  if (x2){
				  txtset(t6,t5); txtright(t6,t5->l-i3-1);
				  txtleft(t5,i3); txtadd(t5,t6);
				  goto autoformat_password_cull;
		  	  }
		  }
		  if (t5->l>16) txtleft(t5,16);
		  txtset(save_password[i],t5);
		}
		//delete file?
		i3=0;
		txtset(t5,"New Player"); if (txtsame(save_name[i],t5)) i3=1; //name is New Player
		if (save_username[i]->l==0) i3=1; //NULL length username
		if (save_password[i]->l==0) i3=1; //NULL length username
		if (i3){
			txtset(t2,".\\save\\"); txtnumint(t,i); txtadd(t,".sav"); txtadd(t2,t);
			DeleteFile(t2->d);
			save_buffer[i]=0;
			goto savefile_deleted;
		}

		/*
		//quarantine file? (i.QRT & iN.QRT)
		i5=i;
		for(i2=0;i2<i;i2++){
		if (save_buffer[i2]){
		if (txtsame(save_username[i5],save_username[i2])){
		if (save_exp[i5]<=save_exp[i2]) {i3=i; i5=i2;} else i3=i2;
		//quarantine i3
		//find a free quarantine slot
		i4=0;
		quarantine0;
		txtset(t2,".\\save\\"); txtnumint(t,i4); txtadd(t,".QRT"); txtadd(t2,t);
		tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT);
		if (tfh->h!=HFILE_ERROR){close(tfh); i4++; goto quarantine0;}
		tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE); //i.QRT
		txtset(t2,".\\save\\"); txtnumint(t,i4); txtadd(t,"N.QRT"); txtadd(t2,t);
		tfh2=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE); //iN.QRT
		txtset(t2,".\\save\\"); txtnumint(t,i3); txtadd(t,".sav"); txtadd(t2,t);
		tfh3=open2(t2,OF_READWRITE|OF_SHARE_COMPAT); //i.sav
		txtNEWLEN(t4,lof(tfh3)); get(tfh3,t4->d2,t4->l);
		close(tfh3);
		DeleteFile(t2->d);
		save_buffer[i3]=0;
		put(tfh,t4->d2,t4->l);
		txtfileout(save_username[i3],tfh2);
		txtfileout(save_name[i3],tfh2);
		close(tfh); close(tfh2);
		}
		}}
		*/

	}//!HFILE_ERROR

save_dump_finished: //no edit/quarantine can occur
save_0bytes:
savefile_deleted:;

}//i
if (save_dump) {close(tfh3); exit(1);}

//edit duplicate names using roman numerals
for (i=0;i<SAVESLOTLAST;i++){ if (save_buffer[i]){
	txtset(t,save_name[i]); txtset(t5,t); txtucase(t);
	for (i2=i+1;i2<=SAVESLOTLAST;i2++){ if (save_buffer[i2]){
		txtset(t2,save_name[i2]); txtucase(t2);
		if (txtsame(t,t2)){
			//search to find the first unassigned roman numeral for this name
			for (y=2;y<=16;y++){
				txtset(t3,t);
				if (y==2) txtadd(t3,"II");
				if (y==3) txtadd(t3,"III");
				if (y==4) txtadd(t3,"IV");
				if (y==5) txtadd(t3,"V");
				if (y==6) txtadd(t3,"VI");
				if (y==7) txtadd(t3,"VII");
				if (y==8) txtadd(t3,"VIII");
				if (y==9) txtadd(t3,"IX");
				if (y==10) txtadd(t3,"X");
				if (y==11) txtadd(t3,"XI");
				if (y==12) txtadd(t3,"XII");
				if (y==13) txtadd(t3,"XIII");
				if (y==14) txtadd(t3,"XIV");
				if (y==15) txtadd(t3,"XV");
				if (y==16) txtadd(t3,"XVI");
				y2=1;
				for (i3=0;i3<=SAVESLOTLAST;i3++){ if (save_buffer[i3]){
					txtset(t4,save_name[i3]); txtucase(t4);
					if (txtsame(t4,t3)) y2=0;
				}}
				if (y2) goto autoname_free;
			}
autoname_free:
			txtset(t3,t5);
			if (y==2) txtadd(t3,"II");
			if (y==3) txtadd(t3,"III");
			if (y==4) txtadd(t3,"IV");
			if (y==5) txtadd(t3,"V");
			if (y==6) txtadd(t3,"VI");
			if (y==7) txtadd(t3,"VII");
			if (y==8) txtadd(t3,"VIII");
			if (y==9) txtadd(t3,"IX");
			if (y==10) txtadd(t3,"X");
			if (y==11) txtadd(t3,"XI");
			if (y==12) txtadd(t3,"XII");
			if (y==13) txtadd(t3,"XIII");
			if (y==14) txtadd(t3,"XIV");
			if (y==15) txtadd(t3,"XV");
			if (y==16) txtadd(t3,"XVI");
			if (save_exp[i]<save_exp[i2]) txtset(save_name[i],t3); else txtset(save_name[i2],t3);
		}
	}}
}}

//edit duplicate usernames using roman numerals
for (i=0;i<SAVESLOTLAST;i++){ if (save_buffer[i]){
	txtset(t,save_username[i]); txtset(t5,t); txtucase(t);
	for (i2=i+1;i2<=SAVESLOTLAST;i2++){ if (save_buffer[i2]){
		txtset(t2,save_username[i2]); txtucase(t2);
		if (txtsame(t,t2)){
			//search to find the first unassigned roman numeral for this name
			for (y=2;y<=16;y++){
				txtset(t3,t);
				if (y==2) txtadd(t3,"2");
				if (y==3) txtadd(t3,"3");
				if (y==4) txtadd(t3,"4");
				if (y==5) txtadd(t3,"5");
				if (y==6) txtadd(t3,"6");
				if (y==7) txtadd(t3,"7");
				if (y==8) txtadd(t3,"8");
				if (y==9) txtadd(t3,"9");
				if (y==10) txtadd(t3,"10");
				if (y==11) txtadd(t3,"11");
				if (y==12) txtadd(t3,"12");
				if (y==13) txtadd(t3,"13");
				if (y==14) txtadd(t3,"14");
				if (y==15) txtadd(t3,"15");
				if (y==16) txtadd(t3,"16");
				y2=1;
				for (i3=0;i3<=SAVESLOTLAST;i3++){ if (save_buffer[i3]){
					txtset(t4,save_username[i3]); txtucase(t4);
					if (txtsame(t4,t3)) y2=0;
				}}
				if (y2) goto autousername_free;
			}
autousername_free:
			txtset(t3,t5);
			if (y==2) txtadd(t3,"2");
			if (y==3) txtadd(t3,"3");
			if (y==4) txtadd(t3,"4");
			if (y==5) txtadd(t3,"5");
			if (y==6) txtadd(t3,"6");
			if (y==7) txtadd(t3,"7");
			if (y==8) txtadd(t3,"8");
			if (y==9) txtadd(t3,"9");
			if (y==10) txtadd(t3,"10");
			if (y==11) txtadd(t3,"11");
			if (y==12) txtadd(t3,"12");
			if (y==13) txtadd(t3,"13");
			if (y==14) txtadd(t3,"14");
			if (y==15) txtadd(t3,"15");
			if (y==16) txtadd(t3,"16");
			if (save_exp[i]<save_exp[i2]) txtset(save_username[i],t3); else txtset(save_username[i2],t3);
		}
	}}
}}



ZeroMemory(&housesav,sizeof(housesav));
tfh=open2(".\\save\\house.sav",OF_READWRITE|OF_SHARE_COMPAT);
if (tfh->h!=HFILE_ERROR){
	txtNEWLEN(t,-lof(tfh)); get(tfh,&t->d2[0],t->l); close(tfh);
	/* luteijn: no decryption needed anymore */
	/* decrypt(t); */
    if (t->ds[0]==12345){ //not crypted
      txtright(t,t->l-2);
    }
    else {
      decrypt(t);
    }
	memcpy(&housesav,t->d,17408);
}



//set mover_body[] (hardcoded)
ZeroMemory(&mover_body,sizeof(mover_body));
//0=no body
//0xffff=as is
mover_body[342]=432;//rat
mover_body[344]=433;//bat
mover_body[347]=434;//reaper
mover_body[348]=435;//sheep
mover_body[349]=436;//dog
mover_body[350]=437;//deer
mover_body[351]=438;//wolf
mover_body[353]=439;//gremlin
mover_body[354]=440;//mouse
mover_body[356]=441;//bird
mover_body[358]=442;//snake
mover_body[359]=443;//rabbit
mover_body[361]=444;//spider
mover_body[362]=341+3*1024;//winged garg
mover_body[363]=339+0*1024;//garg (wingless)
mover_body[367]=339+0*1024;//daemon
mover_body[368]=339+9*1024;//skeleton
mover_body[369]=445;//drake
mover_body[370]=339+1*1024;//headless
mover_body[371]=339+0*1024;//troll
mover_body[372]=446;//mongbat
mover_body[376]=339+6*1024;//fighter
mover_body[377]=339+5*1024;//ranger
mover_body[378]=339+3*1024;//mage
mover_body[379]=339+2*1024;//villager
mover_body[380]=339+2*1024;//merchant
mover_body[381]=339+8*1024;//child
mover_body[382]=339+4*1024;//guard
mover_body[383]=339+8*1024;//jester
mover_body[384]=339+5*1024;//beggar
mover_body[385]=339+8*1024;//farmer
mover_body[386]=339+7*1024;//iolo
mover_body[387]=339+3*1024;//woman
mover_body[388]=447;//cat
mover_body[391]=339+8*1024;//farmer hoing
mover_body[392]=339+7*1024;//musician sitting
mover_body[410]=339+7*1024;//avatar
mover_body[424]=340+3*1024;//cyclops
mover_body[430]=134+1*1024;//carcass

//set mover_blood[] (hardcoded)
ZeroMemory(&mover_blood,sizeof(mover_blood));
mover_blood[342]=1;
mover_blood[344]=1;
mover_blood[348]=1;
mover_blood[349]=1;
mover_blood[350]=1;
mover_blood[351]=1;
mover_blood[353]=1;//gremlin
mover_blood[354]=1;
mover_blood[356]=1;
mover_blood[358]=1;
mover_blood[359]=1;
mover_blood[361]=1;
mover_blood[362]=1;
mover_blood[363]=1;
mover_blood[367]=1;
mover_blood[369]=1;
mover_blood[370]=1;
mover_blood[371]=1;
mover_blood[372]=1;
mover_blood[376]=1;
mover_blood[377]=1;
mover_blood[378]=1;
mover_blood[379]=1;
mover_blood[380]=1;
mover_blood[381]=1;
mover_blood[382]=1;
mover_blood[383]=1;
mover_blood[384]=1;
mover_blood[385]=1;
mover_blood[386]=1;
mover_blood[387]=1;
mover_blood[388]=1;
mover_blood[391]=1;
mover_blood[392]=1;
mover_blood[409]=1;
mover_blood[410]=1;
mover_blood[424]=1;
mover_blood[426]=1;
mover_blood[427]=1;
mover_blood[428]=1;
mover_blood[429]=1;
mover_blood[430]=1;
mover_blood[431]=1;
