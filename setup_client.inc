#ifdef loadimage
#undef loadimage
#endif
#define loadimage loadimage2
SCRLOG_FILEONLY=FALSE;
JDISABLED=FALSE;


GetWindowRect(GetDesktopWindow(),&desktop_rect);
if (!NEThost){
if ((desktop_rect.right<1024)||(desktop_rect.bottom<768)) MessageBox(NULL,"The recommended resolution is 1024x768 (Parts of the screen may not be visible!)","Ultima 6 Online",MB_OK);
GetWindowRect(GetDesktopWindow(),&desktop_rect);
}
if (!InitInstance(hInstance,nCmdShow)){
MessageBox(NULL,"InitInstance failed","Ultima 6 Online",MB_OK); exit(0);
}


if (NEThost==NULL) setupddraw();


/*
ps=newsurf(1024,768,SURF_SYSMEM16);
if (DDRAW_display_pixelformat.dwRGBBitCount!=16){
ps2=newsurf(1024/2,768/2,SURF_SYSMEM);
ps3=newsurf(1024,768,SURF_SYSMEM);
ps4=newsurf(1024/2,768/2,SURF_SYSMEM16); //to temporarily convert to 16bit?
}else{
ps2=newsurf(1024/2,768/2,SURF_SYSMEM16);
}
ps5=newsurf(1024,768,SURF_SYSMEM16); //unconverted mimimap
ps6=newsurf(80,60,SURF_SYSMEM16); //convert to minimap size
ps7=newsurf(120,120,SURF_SYSMEM16); //unconverted treasuremap
ps8=newsurf(240,240,SURF_SYSMEM16); //convert to treasuremap size
tmap_marker=newsurf(8,8,SURF_SYSMEM16); //surface for treasure map markers
*/

// r222 nothing changed here: just some constants which can't be changed

ps=newsurf(resxo,resyo,SURF_SYSMEM16);
if (DDRAW_display_pixelformat.dwRGBBitCount!=16){
ps2=newsurf(resxo/2,resyo/2,SURF_SYSMEM);
ps3=newsurf(resxo,resyo,SURF_SYSMEM);
ps4=newsurf(resxo/2,resyo/2,SURF_SYSMEM16); //to temporarily convert to 16bit?
}else{
ps2=newsurf(resxo/2,resyo/2,SURF_SYSMEM16);
}
ps5=newsurf(resxo,resyo,SURF_SYSMEM16); //unconverted mimimap
ps6=newsurf(80,60,SURF_SYSMEM16); //convert to minimap size
ps7=newsurf(120,120,SURF_SYSMEM16); //unconverted treasuremap
ps8=newsurf(240,240,SURF_SYSMEM16); //convert to treasuremap size
tmap_marker=newsurf(8,8,SURF_SYSMEM16); //surface for treasure map markers


// r444
minimaptype = 0;
i2=getsetting("MINIMAP");

if (i2 == 2) {
	minimaptype = 1;
	minimaptypemax = 1;
} else if (i2 == 3) {
	minimaptype = 2;
	minimaptypemax = 2;
} else if (i2 == 4) {
	minimaptype = 2;
	minimaptypemax = 2;
	droploc = 1;
}

/*
if (minimaptype == 2) {
	minimaptilexstart = 1;
	minimaptilexend = 3;
	minimaptileystart = 1;
	minimaptileyend = 3;
	minimaptilesurf=newsurf(80*2,60*2,SURF_SYSMEM16); //convert to minimap size
} else if (minimaptype == 3) {
	minimaptilexstart = 0;
	minimaptilexend = 4;
	minimaptileystart = 0;
	minimaptileyend = 4;
	minimaptilesurf=newsurf(80,60,SURF_SYSMEM16); //convert to minimap size
}
*/

//if (minimaptype != 0)
	minimap_surf_new=newsurf(256,256,SURF_SYSMEM16);
	minimaptilesurf1=newsurf(80*2,60*2,SURF_SYSMEM16); //convert to minimap size
	minimaptilesurf2=newsurf(80,60,SURF_SYSMEM16); //convert to minimap size


systemfont=CreateFont(16,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"System");
txtfnt=systemfont;

cls(ps,(2<<11)+(27<<5)+18);
scrlog("Welcome to the U6O Slim-Client");
scrlog("U6O DirectDraw engine setup has been completed");
scrlog("System-memory screen back-buffers created");
scrlog("System font [System] loaded");

//create client socket
WSAStartup(MAKEWORD(2, 2), &wsaData);

u6osocket2=socket(AF_INET, SOCK_STREAM, 0);
scrlog("Client socket created");
tfh=open("dns.txt"); txtfilein(t,tfh); close(tfh); if (t->l==0) txtset(t,"localhost:22"); 
txtset(t2,":"); 
i=txtsearch(t,t2); 
if (i==0) { // colon not found so use the default of 22 
      global_TCP_listen_port=22; 
} else { 
      txtset(t2,t); 
      txtright(t2,t2->l-i); 
      txtleft(t,i-1); 
      txtleft(t2,i); 
      global_TCP_listen_port=atol(t2->d); 
} 
if (NEThost) txtset(t,"localhost"); 
txtset(t3,"Port: ");
txtnumint(t4,global_TCP_listen_port);
txtadd(t3,t4);
txtadd(t3," Name: ");
txtadd(t3,t);
scrlog(t3->d); 
struct hostent *host_entry;
host_entry = gethostbyname(&t->d[0]);
if (host_entry == NULL) {MessageBox(NULL,"Invalid DNS","Ultima 6 Online",MB_OK); exit(0);}
scrlog("DNS translated to IP address successfully");
server.sin_family = AF_INET;
server.sin_port = htons(global_TCP_listen_port); 
server.sin_addr.s_addr = *(unsigned long*) host_entry->h_addr;
if (SOCKET_ERROR == connect(u6osocket2, (sockaddr*)&server, sizeof(server))) {MessageBox(NULL,"Could not connect. Server is down!","Ultima 6 Online",MB_OK); exit(0);}
scrlog("Connected to U6O server successfully!");
x=1; setsockopt(u6osocket2,IPPROTO_TCP,TCP_NODELAY,(char*)&x,4);
x=65536; setsockopt(u6osocket2,SOL_SOCKET,SO_RCVBUF,(char*)&x,4);
x=65536; setsockopt(u6osocket2,SOL_SOCKET,SO_SNDBUF,(char*)&x,4);
x=1; ioctlsocket(u6osocket2,FIONBIO,(unsigned long*)&x);

//init sending and receiving client thread info
socketclient[0]=u6osocket2;
socketclient_verified[0]=0;
socketclient_packetsize[0]=0;
socket_timeout[0]=0;
socket_disconnect[0]=0;
//init send struct
socketclient_si[0]=(sockets_info*)malloc(sizeof(sockets_info));
ZeroMemory(socketclient_si[0],sizeof(sockets_info));
socketclient_si[0]->i=u6osocket2;
for (i=0;i<=255;i++) socketclient_si[0]->d[i]=txtnew();
//init recv struct
socketclient_ri[0]=(sockets_info*)malloc(sizeof(sockets_info));
ZeroMemory(socketclient_ri[0],sizeof(sockets_info));
socketclient_ri[0]->i=u6osocket2;
for (i=0;i<=255;i++) socketclient_ri[0]->d[i]=txtnew();
socketclient_ri[0]->t=txtnew(); txtNEWLEN(socketclient_ri[0]->t,-65536);
socketclient_ri[0]->t->l=0; socketclient_ri[0]->t->d2[0]=0;
CreateThread(NULL,0,sockets_send,0,0,(unsigned long*)&socketclient_si[0]->thread_id);
//if (NEThost==NULL){
CreateThread(NULL,0,sockets_receive,0,0,(unsigned long*)&socketclient_ri[0]->thread_id);
//}else{
//socketclient_ri[0]->thread_id=0;
//}
scrlog("Send and Receive network data threads started");
//TCPIP2: CLIENT: SEND SIGNATURE TO HOST
send(u6osocket2,(char*)&U6O_SIGNATURE,4,0);
scrlog("U6O network signature sent to host");

intro_ultimavi=loadimage(".\\dr\\ultimavi.bmp",SURF_SYSMEM16);
intro_ultimavi2=newsurf(intro_ultimavi->d.dwWidth,intro_ultimavi->d.dwHeight,SURF_SYSMEM16);
cls(intro_ultimavi2,(2<<11)+(27<<5)+18);
scrlog("Ultima6 logo loaded");


scrlog("Clearing/Preparing/Loading following static arrays for use:");

ZeroMemory(&cloudactive,sizeof(cloudactive));
scrlog(">cloudactive");

//init STATUSMESSprev[]
for (i=0;i<=7;i++) STATUSMESSprev[i]=txtnew();
scrlog(">STATUSMESSprev");


for (i=0;i<=1023;i++){
intro_star_retry:
x=rnd*1021;
y=rnd*765;
f=sqrt((float)(x*x+y*y));
f2=rnd*1280; if (f>f2) goto intro_star_retry;
f2=rnd*1280; if (f>f2) goto intro_star_retry;
intro_starx[i]=1022-x;
intro_stary[i]=766-y;
}
scrlog(">intro_star(arrays)");

//portraits 2.0 init
ZeroMemory(&portrait,sizeof(portrait));
ZeroMemory(&portrait_doublesize,sizeof(portrait_doublesize));
ZeroMemory(&portrait_halfsize,sizeof(portrait_halfsize));
ZeroMemory(&portrait_loaded,sizeof(portrait_loaded));
ZeroMemory(&portrait_requested,sizeof(portrait_requested));
portrait_request_txt=txtnew();
ZeroMemory(&HOST_portrait_loaded,sizeof(HOST_portrait_loaded));
ZeroMemory(&HOST_portrait_data,sizeof(HOST_portrait_data));
scrlog(">portrait(arrays)");

inpmess_mostrecent=(inpmess_index*)malloc(sizeof(inpmess_index));
inpmess_mostrecent->t=txtnew(); inpmess_mostrecent->next=NULL;
scrlog(">inpmess");

//setup u6osound
ZeroMemory(&u6osound[0],sizeof(u6osound));
scrlog(">u6osound");

ZeroMemory(&U6OK,512); ZeroMemory(&U6OK_FLAGS,128);
U6OK[U6OK_RIGHT][0]=VK_RIGHT;
U6OK[U6OK_LEFT][0]=VK_LEFT;
U6OK[U6OK_UP][0]=VK_UP;
U6OK[U6OK_DOWN][0]=VK_DOWN;
U6OK[U6OK_TALK][0]=84;
U6OK[U6OK_TALK_GLOBAL][0]=VK_SHIFT; U6OK[U6OK_TALK_GLOBAL][1]=84;
U6OK[U6OK_TALK_DONE][0]=13;
U6OK[U6OK_ATTACK][0]=65;
U6OK_FLAGS[U6OK_ATTACK]|=2;
U6OK[U6OK_LOOK][0]=76;
U6OK_FLAGS[U6OK_LOOK]|=2;
U6OK[U6OK_USE][0]=85;
U6OK_FLAGS[U6OK_USE]|=2;
U6OK[U6OK_MAXMIN][0]=77;
U6OK[U6OK_QUIT][0]=81;
U6OK[U6OK_SOUND][0]=83;
U6OK[U6OK_CANCEL][0]=27;
U6OK[U6OK_RETYPE][0]=VK_F3;
U6OK[U6OK_RESPAWN][0]=VK_F1;
U6OK[U6OK_PARTY1][0]=49;
U6OK[U6OK_PARTY2][0]=50;
U6OK[U6OK_PARTY3][0]=51;
U6OK[U6OK_PARTY4][0]=52;
U6OK[U6OK_PARTY5][0]=53;
U6OK[U6OK_PARTY6][0]=54;
U6OK[U6OK_PARTY7][0]=55;
U6OK[U6OK_PARTY8][0]=56;
U6OK[U6OK_PARTY9][0]=57;
U6OK[U6OK_PARTY0][0]=48;
U6OK[U6OK_KTAR1][0]=49;
U6OK[U6OK_KTAR2][0]=50;
U6OK[U6OK_KTAR3][0]=51;
U6OK[U6OK_KTAR4][0]=52;
U6OK[U6OK_KTAR5][0]=53;
U6OK[U6OK_KTAR6][0]=54;
U6OK[U6OK_KTAR7][0]=55;
U6OK[U6OK_KTAR8][0]=56;
U6OK[U6OK_KTAR9][0]=57;
U6OK[U6OK_KTAR0][0]=48;
U6OK[U6OK_CASTRECALL1][0]=VK_F5;
U6OK[U6OK_CASTRECALL2][0]=VK_F6;
U6OK[U6OK_CASTRECALL3][0]=VK_F7;
U6OK[U6OK_CASTRECALL4][0]=VK_F8;
U6OK[U6OK_CASTRECALL5][0]=VK_F9;
U6OK[U6OK_CASTRECALL6][0]=VK_F10;
U6OK[U6OK_CASTRECALL7][0]=VK_F11;
U6OK[U6OK_CASTRECALL8][0]=VK_F12;
U6OK_FLAGS[U6OK_CASTRECALL1]|=2;
U6OK_FLAGS[U6OK_CASTRECALL2]|=2;
U6OK_FLAGS[U6OK_CASTRECALL3]|=2;
U6OK_FLAGS[U6OK_CASTRECALL4]|=2;
U6OK_FLAGS[U6OK_CASTRECALL5]|=2;
U6OK_FLAGS[U6OK_CASTRECALL6]|=2;
U6OK_FLAGS[U6OK_CASTRECALL7]|=2;
U6OK_FLAGS[U6OK_CASTRECALL8]|=2;
U6OK[U6OK_SETCASTRECALL1][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL2][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL3][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL4][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL5][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL6][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL7][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL8][0]=VK_SHIFT;
U6OK[U6OK_SETCASTRECALL1][1]=VK_F5;
U6OK[U6OK_SETCASTRECALL2][1]=VK_F6;
U6OK[U6OK_SETCASTRECALL3][1]=VK_F7;
U6OK[U6OK_SETCASTRECALL4][1]=VK_F8;
U6OK[U6OK_SETCASTRECALL5][1]=VK_F9;
U6OK[U6OK_SETCASTRECALL6][1]=VK_F10;
U6OK[U6OK_SETCASTRECALL7][1]=VK_F11;
U6OK[U6OK_SETCASTRECALL8][1]=VK_F12;
U6OK[U6OK_QUANTITY][0]=VK_SHIFT;
U6OK[U6OK_QUANTITY1][0]=49;
U6OK[U6OK_QUANTITY2][0]=50;
U6OK[U6OK_QUANTITY3][0]=51;
U6OK[U6OK_QUANTITY4][0]=52;
U6OK[U6OK_QUANTITY5][0]=53;
U6OK[U6OK_QUANTITY6][0]=54;
U6OK[U6OK_QUANTITY7][0]=55;
U6OK[U6OK_QUANTITY8][0]=56;
U6OK[U6OK_QUANTITY9][0]=57;
U6OK[U6OK_QUANTITY0][0]=48;
U6OK[U6OK_SCROLLUP][0]=0xD8;
U6OK[U6OK_SCROLLDOWN][0]=0xD9;
U6OK_FLAGS[U6OK_ATTACK2]|=2;
U6OK_FLAGS[U6OK_LOOK2]|=2;
U6OK_FLAGS[U6OK_USE2]|=2;
U6OK[U6OK_TALKPREV][0]=VK_CONTROL; U6OK[U6OK_TALKPREV][1]=84;
U6OK[U6OK_VOICECHAT][0]=86;
U6OK[U6OK_NOT4SALE][0]=88;//"X"
U6OK_FLAGS[U6OK_NOT4SALE]|=2;
U6OK[U6OK_RESPAWNFOLLOWERS][0]=VK_SHIFT; U6OK[U6OK_RESPAWNFOLLOWERS][1]=VK_F1;
memcpy(&U6OK_DEFAULT,&U6OK,512); memcpy(&U6OK_DEFAULT_FLAGS,&U6OK_FLAGS,128);
scrlog(">U6OK_(arrays)");

for (i=0;i<=7;i++) client_spellwait[i]=0;
scrlog(">client_spellwait");

ZeroMemory(&spellrecall_partymember,8);
scrlog(">spellrecall_partymember");

ZeroMemory(&midikeystack[0][0],sizeof(midikeystack));
scrlog(">midikeystack");

txtset(u6o_user_name,"UNKNOWN");
txtset(u6o_user_password,"UNKNOWN");
txtset(u6o_name,"New Player");
ZeroMemory(u6o_vq,28);
txtset(u6o_new_user_password,"UNKNOWN");
scrlog(">user name/password/etc.");

ZeroMemory(&sfx,sizeof(sfxtype)*256);
for (i=0;i<=7;i++) update[i]=1;
wateri[0]=3; wateri[1]=3; wateri[2]=5; wateri[3]=5;
wateri[4]=7; wateri[5]=7; wateri[6]=1; wateri[7]=1;
wateri[8]=5; wateri[9]=7; wateri[10]=3; wateri[11]=1;
wateri[12]=3; wateri[13]=5; wateri[14]=7; wateri[15]=1;
wateri[16]=5; wateri[17]=7; wateri[18]=7; wateri[19]=1;
wateri[20]=1; wateri[21]=3; wateri[22]=3; wateri[23]=5;
wateri[24]=1; wateri[25]=3; wateri[26]=7; wateri[27]=5;
wateri[28]=5; wateri[29]=7; wateri[30]=1; wateri[31]=3;
scrlog(">hybrid tile indexes");

unsigned char qkdir[5][5]; //array to locate projectile graphic
qkdir[0][0]=14; qkdir[1][0]=15; qkdir[2][0]=0; qkdir[3][0]=1;
qkdir[4][0]=2; qkdir[4][1]=3; qkdir[4][2]=4; qkdir[4][3]=5;
qkdir[4][4]=6; qkdir[3][4]=7; qkdir[2][4]=8; qkdir[1][4]=9;
qkdir[0][4]=10; qkdir[0][3]=11; qkdir[0][2]=12; qkdir[0][1]=13;
scrlog(">qkdir");

con_txt[0]=txtnew(); con_txt[1]=txtnew(); con_txt[2]=txtnew(); con_txt[3]=txtnew(); con_txt[4]=txtnew(); con_txt[5]=txtnew(); con_txt[6]=txtnew(); con_txt[7]=txtnew();
scrlog(">con_txt");


//load index in pal emu spr8 (used by getspr)
FillMemory(&sprpi,sizeof(sprpi),255); tfh=open(".\\dr\\objsprpi.u6o"); get(tfh,&sprpi,lof(tfh)); close(tfh);
scrlog(">sprpi [objsprpi.u6o]");


//unsigned char visalways[256][1024];//bit array, if =1 force visibility
ZeroMemory(visalways,sizeof(visalways));
tfh=open(".\\dr\\vis.bin");
i2=lof(tfh)/4;
for(i=0;i<i2;i++){
x=0; get(tfh,&x,2);
y=0; get(tfh,&y,2);
x2=x&7;
x>>=3;
visalways[x][y]|=1<<x2;
}
close(tfh);
scrlog(">visalways [vis.bin]");

scrlog("Init. surfaces, loading .BMP images:");
#include "client_bmp.inc"

//cursors
cur1=LoadCursor(NULL,IDC_ARROW); //load cursors
cur2=LoadCursorFromFile(".\\dr\\talk.cur");
cur3=LoadCursorFromFile(".\\dr\\use.cur");
cur4=LoadCursorFromFile(".\\dr\\look.cur");
cur5=LoadCursorFromFile(".\\dr\\attack.cur");
cur6=LoadCursorFromFile(".\\dr\\cast.cur");
cur7=LoadCursorFromFile(".\\dr\\useon.cur");
cur8=LoadCursorFromFile(".\\dr\\walk.cur");
cur9=LoadCursorFromFile(".\\dr\\not4sale.cur");
cur_u=LoadCursorFromFile(".\\dr\\arrowu.cur");
cur_ru=LoadCursorFromFile(".\\dr\\arrowru.cur");
cur_r=LoadCursorFromFile(".\\dr\\arrowr.cur");
cur_rd=LoadCursorFromFile(".\\dr\\arrowrd.cur");
cur_d=LoadCursorFromFile(".\\dr\\arrowd.cur");
cur_ld=LoadCursorFromFile(".\\dr\\arrowld.cur");
cur_l=LoadCursorFromFile(".\\dr\\arrowl.cur");
cur_lu=LoadCursorFromFile(".\\dr\\arrowlu.cur");
SetCursor(cur1);
scrlog("Mouse Cursors loaded");

//fonts
//fonts
AddFontResource(".\\dr\\u6o.ttf");
AddFontResource(".\\dr\\gargish.ttf");
AddFontResource(".\\dr\\runes.ttf");
AddFontResource(".\\dr\\u6o2.ttf");
SendNotifyMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
//use add font resource?
txtfnt=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
fnt1=txtfnt;
fnt1naa=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
fnt2=CreateFont(16,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
fnt3=CreateFont(8,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Terminal");
//fnt4=CreateFont(18,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
fnt4=CreateFont(22,8,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
//rune
fnt5=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Ultima Runes");
//gargish
fnt6=CreateFont(22,0,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Gargish");
//u6o2 (used for startup menu)
fnt7=CreateFont(24,9,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"PerryGothic");
fonts_added=TRUE;
scrlog("Fonts loaded");

//spellbook info
ZeroMemory(&spellicon,sizeof(spellicon));
spellicon[(0<<4)+0]=loadimage(".\\dr\\creafood.bmp",SURF_SYSMEM16); img0_0key(spellicon[(0<<4)+0],31*32*64+61*32+24);
spellicon[(0<<4)+2]=loadimage(".\\dr\\detetrap.bmp",SURF_SYSMEM16); img0_0key(spellicon[(0<<4)+2],31*32*64+61*32+24);
spellicon[(0<<4)+4]=loadimage(".\\dr\\dous.bmp",SURF_SYSMEM16); img0_0key(spellicon[(0<<4)+4],31*32*64+61*32+24);
spellicon[(0<<4)+6]=loadimage(".\\dr\\heal.bmp",SURF_SYSMEM16); img0_0key(spellicon[(0<<4)+6],31*32*64+61*32+24);
spellicon[(0<<4)+8]=loadimage(".\\dr\\igni.bmp",SURF_SYSMEM16); img0_0key(spellicon[(0<<4)+8],31*32*64+61*32+24);
spellicon[(0<<4)+9]=loadimage(".\\dr\\ligh.bmp",SURF_SYSMEM16); img0_0key(spellicon[(0<<4)+9],31*32*64+61*32+24);
ZeroMemory(&spell,sizeof(spell));
scrlog("Spellbook data setup");


scrlog("Setting up frames(windows)");
vf=FRMnew(); vf->graphic=ps; vf->graphic_use0pixel=TRUE; vf->move=FALSE;

inpf=FRMnew();
inpf->graphic=getportrait(198);
inpf->offset_x=256+2048;
inpf->offset_y=576;
inpf->no_focus=TRUE;
inpftxt=txtnew();
inpf->mouse_over_transparent=TRUE;
if (i2=getsetting("TEXTINPUTPORTRAIT_TRANSPARENCYLEVEL")) inpf->mouse_over_transparent=i2-1;

inpf2=FRM_INPUTnew(inpf,inpftxt);
inpf2->graphic=newsurf(1,1,SURF_SYSMEM16); cls(inpf2->graphic,0);
inpf2->offset_x=56+69;
inpf2->offset_y=32-8;
inpf2->col=rgb(255,255,255);
inpf2->special=1;
inpf2->cursor=34;
inpf2->cursor_blinkrate=0;
inpf2->length_limit=300;

inpft=FRM_TXTnew(inpf,txtnew());
txtset(inpft->text,"You say,?");
inpft->text->d2[8]=34;
inpft->offset_x=56;
inpft->offset_y=32-8;
inpft->special=1;
inpft->col=rgb(255,255,255);

con_frm=FRMnew();
con_frm->graphic=converse_arrows; //provided for dimentions only
con_frm->offset_x=0;
con_frm->offset_y=96-4;
con_frm->no_focus=TRUE;
con_frm->mouse_over_transparent=TRUE;
if (i2=getsetting("CONVERSATIONLOG_SCROLLARROWS_TRANSPARENCYLEVEL")) con_frm->mouse_over_transparent=i2-1;

con_frm_img=FRM_IMAGEnew(con_frm,newsurf(1024,256,SURF_SYSMEM16));
con_frm_img->offset_x=0;
con_frm_img->offset_y=-256;

// r222 init new mode stuff
extern void newmodeinit();

if (newmodestatus == 1) {
	newmodestatus++;
	newmodeinit();
}

for (i=0;i<=7;i++){
party_surf[i]=newsurf(256,256,SURF_SYSMEM16);
party_frame[i]=FRMnew();
party_frame[i]->graphic=party_surf[i];
party_frame[i]->offset_x=4096; //startup offset off screen
party_frame[i]->offset_y=0; //startup offset off screen
party_frame[i]->graphic_use0pixel=TRUE;
party_frame[i]->mouse_over_transparent=TRUE;
if (i2=getsetting("INVENTORYWINDOW_TRANSPARENCYLEVEL")) party_frame[i]->mouse_over_transparent=i2-1;

party_spellbook_surf[i]=newsurf(245,173,SURF_SYSMEM16);
party_spellbook_frame[i]=FRMnew();
party_spellbook_frame[i]->graphic=party_spellbook_surf[i];
party_spellbook_frame[i]->offset_x=4096; //startup offset off screen
party_spellbook_frame[i]->offset_y=0; //startup offset off screen
party_spellbook_frame[i]->mouse_over_transparent=TRUE;
if (i2=getsetting("SPELLBOOKWINDOW_TRANSPARENCYLEVEL")) party_spellbook_frame[i]->mouse_over_transparent=i2-1;
spellbook_page[i]=1; //default to first page (0=can't display spellbook, no spells)
spellbook_flags[i]=0;
}


qkstf=FRMnew();
qkstf->graphic=newsurf(128,32,SURF_SYSMEM16);
qkstf->graphic_use0pixel=TRUE;
qkstf->offset_x=1024-128;
qkstf->offset_y=128-16;
qkstf->mouse_over_transparent=TRUE;
if (i2=getsetting("PARTYLISTWINDOW_TRANSPARENCYLEVEL")) qkstf->mouse_over_transparent=i2-1;

volcontrol=FRMnew();
volcontrol->graphic=volcontrol_surf;
volcontrol->graphic_use0pixel=TRUE;
volcontrol->offset_x=1024-128;
volcontrol->offset_y=768-64-32-16;
volcontrol->mouse_over_transparent=TRUE;
if (i2=getsetting("VOLUMECONTROL_TRANSPARENCYLEVEL")) volcontrol->mouse_over_transparent=i2-1;

musickeyboard=FRMnew();
musickeyboard->graphic=loadimage(".\\dr\\keyb.bmp",SURF_SYSMEM16);
musickeyboard->offset_x=176+2048; musickeyboard->offset_y=677;
musickeyboard->graphic_use0pixel=TRUE; 
musickeyboard->mouse_over_transparent=TRUE;
if (i2=getsetting("PLAYINSTRUMENTPIANOKEYS_TRANSPARENCYLEVEL")) musickeyboard->mouse_over_transparent=i2-1;

voicechat_frame=FRMnew();
voicechat_frame->graphic=newsurf(395,18,SURF_SYSMEM16);
voicechat_frame->offset_x=314+2048;
voicechat_frame->offset_y=711;
img(voicechat_frame->graphic,0,0,voicechat_voicebar);

statusmessage_viewprev=FRMnew();
statusmessage_viewprev->graphic=statusmessage_arrowup;
statusmessage_viewprev->offset_x=0;
statusmessage_viewprev->offset_y=768-32-16;
statusmessage_viewprev->no_focus=TRUE;
statusmessage_viewprev->graphic_use0pixel=TRUE;
statusmessage_viewprev->mouse_over_transparent=TRUE;
if (i2=getsetting("STATUSMESSAGE_VIEWPREVIOUSBOX_TRANSPARENCYLEVEL")) statusmessage_viewprev->mouse_over_transparent=i2-1;

statusmessage_viewnpc=FRMnew();
statusmessage_viewnpc->graphic=viewnpc_temp;
statusmessage_viewnpc->offset_x=424+2048;
statusmessage_viewnpc->offset_y=112;
statusmessage_viewnpc->graphic_use0pixel=TRUE;


fs=FRMnew();
fs->graphic=ps;
fs->graphic_use0pixel=TRUE;
fs->move=FALSE;

minimap_surf=newsurf(256,256,SURF_SYSMEM16);
minimap_frame=FRMnew();
minimap_frame->graphic=minimap_surf;
minimap_frame->offset_x=4096; //startup offset off screen
minimap_frame->offset_y=0; //startup offset off screen
minimap_frame->graphic_use0pixel=TRUE;
minimap_frame->mouse_over_transparent=FALSE;
img(minimap_frame->graphic,minimap_b);

tmap_surf=newsurf(256,256,SURF_SYSMEM16);
tmap_frame=FRMnew();
tmap_frame->graphic=tmap_surf;
tmap_frame->offset_x=4096; //startup offset off screen
tmap_frame->offset_y=0; //startup offset off screen
tmap_frame->graphic_use0pixel=TRUE;
tmap_frame->mouse_over_transparent=FALSE;
img(tmap_frame->graphic,minimap_b);

// r444
//if (minimaptype != 0)
	img(minimap_surf_new, minimap_b);


scrlog("Setting up client player data structures");
//setup client-side player structure
for (i2=0;i2<=7;i2++){
for (i=0;i<=(15+4+1+1);i++){
tobj_i[i2][i]=OBJnew_local();
}
for (i=0;i<=7;i++){
tobj_e[i2][i]=OBJnew_local();
}
}
static object *tplayobj[8];
tplay=(player*)malloc(sizeof(player));
ZeroMemory(tplay,sizeof(player));
for (i=0;i<=7;i++){
myobj=OBJnew_local();
tplay->party[i]=myobj;
tnpc=(npc*)malloc(sizeof(npc));
ZeroMemory(tnpc,sizeof(npc));
myobj->more=tnpc; myobj->info|=2;
myobj2=OBJnew_local();
myobj2->type=188;//bag (INVISIBLE)
tnpc->items=myobj2;
tnpc->baseitem=myobj2;
tnpc->port=0;
tnpc->name=txtnew(); txtset(tnpc->name,"unknown");
tnpc->hp=255;
tnpc->hp_max=255;
tnpc->player=tplay;
tnpc->s=26; tnpc->d=20; tnpc->i=17;
tplayobj[i]=myobj;
tplay->party[i]=NULL;
}

scrlog("Setting up lighting data");
for (i=0;i<=15;i++){
i2=0;
f=1-(float)i/15.0f;
for (z=0;z<=31;z++){
for (y=0;y<=63;y++){
for (x=0;x<=31;x++){
f2=(float)x*f; x2=f2;
f2=(float)y*f; y2=f2;
f2=(float)z*f; z2=f2;
lval[i][i2]=x2+(y2<<5)+(z2<<11);
i2++;
}}}
}

for (y=0;y<=767;y++){for (x=0;x<=1023;x++){
x2=x-512+16; y2=y-384+16; x2*=x2; y2*=y2;
x3=640.0f-sqrt((float)(x2+y2)); //0-640 highest in centre!
if (x3<0) x3=0;
x4=x3-448; if (x4<0) x4=0; //0-128
x4+=47;
ls_moon1[y*1024+x]=x4/48;
x4=x3-384; if (x4<0) x4=0;
x4+=63;
ls_moon2[y*1024+x]=x4/64;
x4=x3-320; if (x4<0) x4=0;
x4+=79;
ls_moon3[y*1024+x]=x4/80;
x4=x3-256; if (x4<0) x4=0; //384
x4+=95;
ls_moon4[y*1024+x]=x4/96;
}}

//ls3
f2=(32.0f*3.0f)/2.0f;
for (x=0;x<(32*3);x++){
for (y=0;y<(32*3);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*15.0f+0.5f;
ls3[x][y]=15-i;
}}
//ls5
f2=(32.0f*5.0f)/2.0f;
for (x=0;x<(32*5);x++){
for (y=0;y<(32*5);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*15.0f+0.5f;
ls5[x][y]=15-i;
}}
//ls5b
f2=(32.0f*5.0f)/2.0f;
for (x=0;x<(32*5);x++){
for (y=0;y<(32*5);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*4.5f;
ls5b[x][y]=4-i;
}}
//ls7
f2=(32.0f*7.0f)/2.0f;
for (x=0;x<(32*7);x++){
for (y=0;y<(32*7);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*15.0f+0.5f;
ls7[x][y]=15-i;
}}
//ls9
f2=(32.0f*9.0f)/2.0f;
for (x=0;x<(32*9);x++){
for (y=0;y<(32*9);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*15.0f+0.5f;
ls9[x][y]=15-i;
}}
//ls11
f2=(32.0f*11.0f)/2.0f;
for (x=0;x<(32*11);x++){
for (y=0;y<(32*11);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*15.0f+0.5f;
ls11[x][y]=15-i;
}}
//ls13
f2=(32.0f*13.0f)/2.0f;
for (x=0;x<(32*13);x++){
for (y=0;y<(32*13);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*15.0f+0.5f;
ls13[x][y]=15-i;
}}
//ls3b
f2=(32.0f*3.0f)/2.0f;
for (x=0;x<(32*3);x++){
for (y=0;y<(32*3);y++){
f=sqrt((f2-(float)x)*(f2-(float)x)+(f2-(float)y)*(f2-(float)y)); //f<-dis
if (f>f2) f=f2;
i=f/f2*4.5f;
ls3b[x][y]=4-i;
}}


scrlog("Opening message log [talk.txt]");

messagelog=open2("talk.txt",OF_CREATE|OF_READWRITE|OF_SHARE_COMPAT);

scrlog("Initializing MIDI driver");
if (getsetting("ALLOWMIDI")==2) U6O_DISABLEMUSIC=TRUE;
if (U6O_DISABLEMUSIC) goto u6omidisetup_failed;
ZeroMemory(&midikeyboard2_keyon,sizeof(midikeyboard2_keyon));
ZeroMemory(&u6omidi_filename,sizeof(u6omidi_filename));
u6omidi=new CMidiMusic;
if (u6omidi->Initialize(FALSE)!=S_OK) goto u6omidisetup_failed;
u6omidisetup=1;
i=0; i2=0;
while (u6omidi->PortEnumeration(i,&u6omidi_infoport)==S_OK){
if (u6omidi_infoport.dwClass==DMUS_PC_OUTPUTCLASS){ //Ensure it is an output hardware device
if (i2==0){
if (u6omidi_infoport.dwFlags&DMUS_PC_SOFTWARESYNTH){
u6omidi->SelectPort(&u6omidi_infoport);
i2=1;
}
}
}
i++;
}
if (i2==0) u6omidisetup=0;
u6omidisetup_failed:

scrlog("Loading music keyboard state [midikeyb.bin]");
tfh=open(".\\dr\\midikeyb.bin"); get(tfh,&midikeyboard2,256); close(tfh);
//walkthrou pos
scrlog("Loading walkthrough position [walkthru.pos]");
txtset(t,".\\dr\\walkthru.pos");
tfh=open2(t,OF_READWRITE|OF_SHARE_COMPAT);
if (tfh->h==HFILE_ERROR){
tfh=open2(t,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
walkthru_pos_skip=0;
put(tfh,&walkthru_pos_skip,4); //lines to skip of the walkthru
}else{
get(tfh,&walkthru_pos_skip,4); //lines to skip of the walkthru
}
close(tfh);

scrlog("Loading walkthrough messages [walkthru.txt]");
walkthru_fh=open(".\\dr\\walkthru.txt");
if (U6O_WALKTHRU_REC) {close(walkthru_fh); U6O_WALKTHRU=FALSE; walkthru_fh=open2("walkthru.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);}

scrlog("Loading client settings [settings.bin]");
//restore client settings or set to default

tfh=open2(".\\dr\\settings.bin",OF_READWRITE|OF_SHARE_COMPAT);
if (tfh->h!=HFILE_ERROR){
cltset2_restored=1;
get(tfh,&cltset2,sizeof(client_settings));
close(tfh);
u6ovolume=cltset2.u6ovolume;
u6omidivolume=cltset2.u6omidivolume;
u6ovoicevolume=cltset2.u6ovoicevolume;
memcpy(&spellrecall_partymember,&cltset2.spellrecall_partymember,8);
memcpy(&spellrecall_i,&cltset2.spellrecall_i,8);
if (cltset2.musickeyboard_offset_x!=32767){musickeyboard->offset_x=cltset2.musickeyboard_offset_x+2048; musickeyboard->offset_y=cltset2.musickeyboard_offset_y;}
if (cltset2.inpf_offset_x!=32767){inpf->offset_x=cltset2.inpf_offset_x+2048; inpf->offset_y=cltset2.inpf_offset_y;}
if (cltset2.con_frm_offset_x!=32767){con_frm->offset_x=cltset2.con_frm_offset_x; con_frm->offset_y=cltset2.con_frm_offset_y;}
if (cltset2.volcontrol_offset_x!=32767){volcontrol->offset_x=cltset2.volcontrol_offset_x; volcontrol->offset_y=cltset2.volcontrol_offset_y;}
if (cltset2.qkstf_offset_x!=32767){qkstf->offset_x=cltset2.qkstf_offset_x; qkstf->offset_y=cltset2.qkstf_offset_y;}
memcpy(&cltset,&cltset2,sizeof(client_settings));
}else{
//set default/unknown(32767) values
for (i=0;i<=7;i++){
cltset.party_frame_offset_x[i]=32767;
cltset.party_spellbook_frame_offset_x[i]=32767;
}
cltset.musickeyboard_offset_x=32767;
cltset.inpf_offset_x=32767;
cltset.con_frm_offset_x=32767;
cltset.volcontrol_offset_x=32767;
cltset.qkstf_offset_x=32767;
cltset.minimap_offset_x=32767;
cltset.tmap_offset_x=32767;
}
FRAME_mbl[0]=FALSE; FRAME_mbl[1]=FALSE; lpn=NULL; //used by frame.h


tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
if (tfh->h==HFILE_ERROR){
tfh=open2("ignore.txt",OF_READWRITE|OF_CREATE);
}
close(tfh);
if (getsetting("ALLOWJOYSTICK")==2) U6O_DISABLEJOYSTICK=TRUE;

scrlog("Setting up low level MIDI driver");
if (U6O_DISABLEMUSIC) goto midiout_blocked;
if (midiOutOpen(&midiout_handle,(UINT)-1, 0, 0, CALLBACK_NULL)==MMSYSERR_NOERROR){
//set track instruments
midiOutShortMsg(midiout_handle,0x000000C0);//required
midiOutShortMsg(midiout_handle,0x000020C0);//required
midiOutShortMsg(midiout_handle,0x000000C0+24*256); //25 Acoustic Nylon Guitar
midiOutShortMsg(midiout_handle,0x000000C1);//required
midiOutShortMsg(midiout_handle,0x000020C1);//required
midiOutShortMsg(midiout_handle,0x000000C1+6*256); //7 Harpsichord
midiOutShortMsg(midiout_handle,0x000000C2);//required
midiOutShortMsg(midiout_handle,0x000020C2);//required
midiOutShortMsg(midiout_handle,0x000000C2+46*256); //47 Orchestral Harp
midiOutShortMsg(midiout_handle,0x000000C3);//required
midiOutShortMsg(midiout_handle,0x000020C3);//required
midiOutShortMsg(midiout_handle,0x000000C3+75*256); //76 Pan Flute
midiOutShortMsg(midiout_handle,0x000000C4);//required
midiOutShortMsg(midiout_handle,0x000020C4);//required
midiOutShortMsg(midiout_handle,0x000000C4+13*256); //14 Xylophone
x=u6omidivolume/2;//change 0-255 to 0-127
midiOutShortMsg(midiout_handle,0x000007B0+x*65536); //set volume
midiOutShortMsg(midiout_handle,0x000007B1+x*65536); //set volume
midiOutShortMsg(midiout_handle,0x000007B2+x*65536); //set volume
midiOutShortMsg(midiout_handle,0x000007B3+x*65536); //set volume
midiOutShortMsg(midiout_handle,0x000007B4+x*65536); //set volume
midiout_setup=TRUE;
}
midiout_blocked:


if (getsetting("CLOUDS")==2) noclouds=TRUE;

i=-1; cmdline_length3: i++; if ((unsigned char*)lpCmdLine[i]) goto cmdline_length3;
if (i){
txtNEWLEN(t,-i); memcpy(t->d2,lpCmdLine,i);
txtset(t2,"walkthrurec"); if (txtsearch(t,t2)){
U6O_WALKTHRU_REC=TRUE;
}
}

scrlog("Beginning main loop!");

tfh=open(".\\dr\\objfixed.bin"); get(tfh,&objfixed_index,2048*1024*2); get(tfh,&objfixed_type,65536*2); close(tfh);
//scrlog(">Fixed map objects loaded [objfixed.bin]");

if (NEThost==NULL){
ZeroMemory(&bt,sizeof(bt));
tfh=open2(".\\dr\\bt.bin",OF_READWRITE|OF_SHARE_COMPAT);
get(tfh,&bt,1024*2048*2);
close(tfh);
//scrlog(">bt(basetiles) [bt.bin]");
}

tfh=open(".\\dr\\tobjfix.bin"); get(tfh,&tobjfixed_index,2048*1024*2); get(tfh,&tobjfixed_type,65536*2); close(tfh);
