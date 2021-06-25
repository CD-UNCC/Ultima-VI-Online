

SYSTEMTIME lt;
GetLocalTime(&lt);
txtset(t,"Date: ");
txtnumint(t2,lt.wYear);
txtadd(t,t2);
txtadd(t,"/");
txtnumint(t2,lt.wMonth);
if (t2->l<2) {txtadd(t,"0");}
txtadd(t,t2);
txtadd(t,"/");
txtnumint(t2,lt.wDay);
if (t2->l<2) {txtadd(t,"0");}
txtadd(t,t2);
txtadd(t," Time: ");
txtnumint(t2,lt.wHour);
if (t2->l<2) {txtadd(t,"0");}
txtadd(t,t2);
txtadd(t,":");
txtnumint(t2,lt.wMinute);
if (t2->l<2) {txtadd(t,"0");}
txtadd(t,t2);
txtfileout(t,log2file); /* don't use the normal log method, because we don't want a zero running time stamp for the start time */
#ifdef CONSOLE
_cprintf("%s\n",t->d);
txtset(t2,"u6o Host, ");
txtadd(t2,t);
SetConsoleTitle(t2->d);
#endif
setupddraw();

inbritannia=txtnew();

WSAStartup(MAKEWORD(2, 2), &wsaData);
//create host socket
u6osocket = socket(AF_INET, SOCK_STREAM, 0);
server.sin_family = AF_INET;
tfh=open("dns.txt"); txtfilein(t,tfh); close(tfh); if (t->l==0) txtset(t,"localhost:22"); 
txtset(t2,":"); 
i=txtsearch(t,t2); 
if (i==0) { // colon not found so use the default of 22 
      global_TCP_listen_port=22; 
} else { 
      txtset(t2,t); 
      txtright(t2,t2->l-i); 
      global_TCP_listen_port=atol(t2->d); 
} 
server.sin_port = htons(global_TCP_listen_port); 
server.sin_addr.s_addr = INADDR_ANY;
bind(u6osocket, (sockaddr*)&server, sizeof(server));
listen(u6osocket, SOMAXCONN);
hsockets_accept = CreateThread(NULL,
                               0,           
                               sockets_accept,
                               0,
                               0, //CREATE_SUSPENDED,           
                               &idsockets_accept);
x=1; setsockopt(u6osocket,IPPROTO_TCP,TCP_NODELAY,(char*)&x,4);
x=65536; setsockopt(u6osocket,SOL_SOCKET,SO_RCVBUF,(char*)&x,4);
x=65536; setsockopt(u6osocket,SOL_SOCKET,SO_SNDBUF,(char*)&x,4);
for (i=0;i<=SOCKETLAST;i++){
  socketclient[i]=INVALID_SOCKET;
  socketclient_si[i]=NULL;
  socketclient_ri[i]=NULL;
}

ZeroMemory(&economy_setup,sizeof(economy_setup));
//init AUTOPICKUP_OBJECTVALID[]
ZeroMemory(&AUTOPICKUP_OBJECTVALID,sizeof(AUTOPICKUP_OBJECTVALID));
AUTOPICKUP_OBJECTVALID[36]=1;//spear
AUTOPICKUP_OBJECTVALID[37]=1;//axe
AUTOPICKUP_OBJECTVALID[38]=1;//dagger
AUTOPICKUP_OBJECTVALID[49]=1;//boomerang
AUTOPICKUP_OBJECTVALID[55]=1;//arrow
AUTOPICKUP_OBJECTVALID[56]=1;//bolt
x=rnd*3; x--; windx=x;
x=rnd*3; x--; windy=x;
if (tname==NULL) tname=txtnew();
if (tusername==NULL) tusername=txtnew();
if (tuserpassword==NULL) tuserpassword=txtnew();
if (tnewuserpassword==NULL) tnewuserpassword=txtnew();

//keyb target
//setup array of x,y offsets in order of distance from the center & bubble sort them
//init ktar x,y offsets from closest to furthest
i=0;
for (y=-11;y<=12;y++){ for (x=-15;x<=16;x++){
  ktar_x[i]=x; ktar_y[i]=y; ktar_xydis[i]=sqrt((float)(x*x+y*y));
  i++;
}}
//bubble sort, closest to furthest
ktar_bs:
for (i=0;i<=766;i++){
  if (ktar_xydis[i]>ktar_xydis[i+1]){
    x=ktar_x[i]; ktar_x[i]=ktar_x[i+1]; ktar_x[i+1]=x;
    y=ktar_y[i]; ktar_y[i]=ktar_y[i+1]; ktar_y[i+1]=y;
    f=ktar_xydis[i]; ktar_xydis[i]=ktar_xydis[i+1]; ktar_xydis[i+1]=f;
    goto ktar_bs;
  }
}//i

//setup encryptcode
srand(128);
for (i=0;i<=65535;i++){
  encryptcode[i]=(unsigned char)(rndold*256.0f);
}
/* Luteijn: use this to dump this to a file */
/*
tfh=open2("ENCRYPTIONCODE.BIN",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE); put(tfh,encryptcode,65536); close(tfh);
*/


ZeroMemory(&od,sizeof(od));

ZeroMemory(&portcullis,sizeof(portcullis));
ZeroMemory(&lever,sizeof(lever));

//2003: setup basetiles _________________________________________________________________
//chunks->basetiles
tfh=open(".\\ultima6\\chunks");
//if (tfh->h==HFILE_ERROR){MessageBox(NULL,"U6O setup failed","Ultima 6 Online",MB_OK); setupfail=TRUE;}
get(tfh,&chunks,65536); close(tfh); //load chunks
tfh=open(".\\ultima6\\map");
//if (tfh->h==HFILE_ERROR){MessageBox(NULL,"U6O setup failed","Ultima 6 Online",MB_OK); setupfail=TRUE;}
get(tfh,&u6wci2,8*8*16*8*3);
//unsigned char u6wci2[8][8][16][8][3];
//unsigned short u6wci[128][128];
for (y2=0;y2<=7;y2++){for (x2=0;x2<=7;x2++){
  for (y=0;y<=15;y++){for (x=0;x<=7;x++){
    i=u6wci2[y2][x2][y][x][0];
    i2=u6wci2[y2][x2][y][x][1];
    i3=u6wci2[y2][x2][y][x][2];
    z=i+(i2&0xF)*256;
    z2=i3*16+(i2>>4);
    u6wci[y2*16+y][x2*16+x*2]=z;
    u6wci[y2*16+y][x2*16+x*2+1]=z2;
  }}
}}
for (y2=0;y2<=127;y2++){ for (x2=0;x2<=127;x2++){
  for (y=0;y<=7;y++){ for (x=0;x<=7;x++){
    bt[y2*8+y][x2*8+x]=chunks[u6wci[y2][x2]][y][x];
  }}
}}
get(tfh,&u60ci2,32*16*3);
//unsigned char u60ci2[32][16][3];
//unsigned short u60ci[32][32];
for (y=0;y<=31;y++){for (x=0;x<=15;x++){
  i=u60ci2[y][x][0];
  i2=u60ci2[y][x][1];
  i3=u60ci2[y][x][2];
  z=i+(i2&0xF)*256;
  z2=i3*16+(i2>>4);
  u60ci[y][x*2]=z;
  u60ci[y][x*2+1]=z2;
}}
for (y2=0;y2<=31;y2++){ for (x2=0;x2<=31;x2++){
  for (y=0;y<=7;y++){ for (x=0;x<=7;x++){
    bt[y2*8+y][x2*8+x+1024]=chunks[u60ci[y2][x2]][y][x];
  }}
}}
get(tfh,&u60ci2,32*16*3);
//unsigned char u60ci2[32][16][3];
//unsigned short u60ci[32][32];
for (y=0;y<=31;y++){for (x=0;x<=15;x++){
  i=u60ci2[y][x][0];
  i2=u60ci2[y][x][1];
  i3=u60ci2[y][x][2];
  z=i+(i2&0xF)*256;
  z2=i3*16+(i2>>4);
  u60ci[y][x*2]=z;
  u60ci[y][x*2+1]=z2;
}}
for (y2=0;y2<=31;y2++){ for (x2=0;x2<=31;x2++){
  for (y=0;y<=7;y++){ for (x=0;x<=7;x++){
    bt[y2*8+y][x2*8+x+1024+256]=chunks[u60ci[y2][x2]][y][x];
  }}
}}
get(tfh,&u60ci2,32*16*3);
//unsigned char u60ci2[32][16][3];
//unsigned short u60ci[32][32];
for (y=0;y<=31;y++){for (x=0;x<=15;x++){
  i=u60ci2[y][x][0];
  i2=u60ci2[y][x][1];
  i3=u60ci2[y][x][2];
  z=i+(i2&0xF)*256;
  z2=i3*16+(i2>>4);
  u60ci[y][x*2]=z;
  u60ci[y][x*2+1]=z2;
}}
for (y2=0;y2<=31;y2++){ for (x2=0;x2<=31;x2++){
  for (y=0;y<=7;y++){ for (x=0;x<=7;x++){
    bt[y2*8+y][x2*8+x+1024+256*2]=chunks[u60ci[y2][x2]][y][x];
  }}
}}
get(tfh,&u60ci2,32*16*3);
//unsigned char u60ci2[32][16][3];
//unsigned short u60ci[32][32];
for (y=0;y<=31;y++){for (x=0;x<=15;x++){
  i=u60ci2[y][x][0];
  i2=u60ci2[y][x][1];
  i3=u60ci2[y][x][2];
  z=i+(i2&0xF)*256;
  z2=i3*16+(i2>>4);
  u60ci[y][x*2]=z;
  u60ci[y][x*2+1]=z2;
}}
for (y2=0;y2<=31;y2++){ for (x2=0;x2<=31;x2++){
  for (y=0;y<=7;y++){ for (x=0;x<=7;x++){
    bt[y2*8+y][x2*8+x+1024+256*3]=chunks[u60ci[y2][x2]][y][x];
  }}
}}
get(tfh,&u60ci2,32*16*3);
//unsigned char u60ci2[32][16][3];
//unsigned short u60ci[32][32];
for (y=0;y<=31;y++){for (x=0;x<=15;x++){
  i=u60ci2[y][x][0];
  i2=u60ci2[y][x][1];
  i3=u60ci2[y][x][2];
  z=i+(i2&0xF)*256;
  z2=i3*16+(i2>>4);
  u60ci[y][x*2]=z;
  u60ci[y][x*2+1]=z2;
}}
for (y2=0;y2<=31;y2++){ for (x2=0;x2<=31;x2++){
  for (y=0;y<=7;y++){ for (x=0;x<=7;x++){
    bt[y2*8+y+256][x2*8+x+1024]=chunks[u60ci[y2][x2]][y][x];
  }}
}}
close(tfh); //U6data


x=0;
tfh=open2("admins.txt",OF_READWRITE|OF_SHARE_COMPAT); //admins
txtfilein(t6,tfh);
while (t6->l) { 
  admins[x]=txtnew();
  txtset(admins[x],t6);
  txtfilein(t6,tfh);
  x++;
  if (x==ADMINSMAX) {break;}     
}
close(tfh);

#include "host.inc"



house();
#include "host_setup.h"


