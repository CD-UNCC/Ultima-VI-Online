#include "stdafx.h"
#include "resource.h"
#include <ddraw.h>
//#include <d3d.h> *REDUNDANT
#pragma warning(disable: 4018 4244 4731)
#include "myfile.h"
#include "commdlg.h"
// r999
#include "define_both.h"


//darklight.cpp extern
extern RECT desktop_rect;
extern bool smallwindow;
extern HWND hWnd;
extern HWND hWnd2;
extern HWND hWnd3;
extern HINSTANCE hInst;

// rrrd
extern HWND hWnd4;
extern unsigned int resxz;
extern unsigned int resyz;
extern unsigned int windowsizecyclenum;
extern unsigned int resxs;
extern unsigned int resys;
extern unsigned int resxn1m;
extern unsigned int resyn1m;
extern double scalexm;
extern double scaleym;
extern unsigned int resxo;
extern unsigned int resyo;




//direct draw surface structures and functions
IDirectDraw* dd1=NULL;
IDirectDraw4* dd=NULL;
DWORD txtcol=0xFFFFFF;
HFONT txtfnt=NULL;

struct surf
{
DDSURFACEDESC2 d;
LPDIRECTDRAWSURFACE4 s;
union{
unsigned long* o;
unsigned char* o1;
unsigned short* o2;
};
//IDirect3DTexture2* t; //only valid if SURF_TEX flag is used *REDUNDANT
};

extern surf *vs;

surf* surflist[16384];

DDPIXELFORMAT DDRAW_display_pixelformat;



// r999
extern surf* uipanelsurf[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelx[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanely[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelsizex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelsizey[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern float uipanelscalex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern float uipanelscaley[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelhitenable[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelusedefaultstatedata[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];

extern int uipaneli[UI_PANEL_MAX][UI_PANELWIDGET_MAX];
extern int uipanelcount;
extern int uipanelwidgetcount[UI_PANEL_MAX];
extern int uipanelsidebar, uipanelactionbar1, uipanelactionbar2, uipanelactiontalkbar1, uipanelactiontalkbar2, uipanelminimap;



bool setupddraw()
{
DirectDrawCreate(NULL,&dd1,NULL);
if (FAILED(dd1->SetCooperativeLevel(hWnd,DDSCL_NORMAL))) return FALSE;
if (dd1==NULL) return FALSE;
dd1->QueryInterface(IID_IDirectDraw4,(void**)&dd);
if (dd==NULL) return FALSE;
dd->Initialize(NULL);
if (FAILED(dd->SetCooperativeLevel(hWnd, DDSCL_NORMAL|DDSCL_NOWINDOWCHANGES))) return FALSE;
//if (FAILED(dd->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE|DDSCL_NOWINDOWCHANGES|DDSCL_FULLSCREEN))) return FALSE;


static surf* ts;
ts=(surf*)malloc(sizeof(surf));
ZeroMemory(ts,sizeof(surf));
ts->d.dwSize=sizeof(DDSURFACEDESC2);
ts->d.dwFlags=DDSD_CAPS;
ts->d.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE; 
if (dd->CreateSurface(&ts->d,&ts->s,NULL)!=DD_OK){
MessageBox(NULL,"CreateSurface failed: primary","Ultima 6 Online",MB_OK); exit(1);
}
ZeroMemory(&DDRAW_display_pixelformat,sizeof(DDRAW_display_pixelformat));
DDRAW_display_pixelformat.dwSize=sizeof(DDRAW_display_pixelformat);
ts->s->GetPixelFormat(&DDRAW_display_pixelformat);
//exit(DDRAW_display_pixelformat.dwGBitMask);
ts->s->Release();
//free((void*)ts);
//static long i;
ZeroMemory(&surflist[0],sizeof(surf*)*16384);
return TRUE;
}

surf* surfstruct()
{
static surf* ts;
static long i;
ts=(surf*)malloc(sizeof(surf));
ZeroMemory(ts,sizeof(surf));
ts->d.dwSize=sizeof(DDSURFACEDESC2);
for (i=0;i<16384;i++)
{
if (surflist[i]==NULL)
{
surflist[i]=ts;
return ts;
}
}
return ts;
}

surf* newsurf(long x,long y,long flags)
{
surf*ts=surfstruct();
if (flags&32){
ts->d.dwFlags=DDSD_CAPS;
ts->d.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
goto gotpixelformat;
}
ts->d.dwFlags=DDSD_HEIGHT|DDSD_WIDTH|DDSD_CAPS|DDSD_PIXELFORMAT;
ts->d.dwWidth=x;
ts->d.dwHeight=y;
ts->d.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY; //default
if ((flags&1)||(flags&64)) ts->d.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY; 
if (flags&16) ts->d.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
//if (flags&2) ts->d.ddsCaps.dwCaps+=DDSCAPS_3DDEVICE; 
if (flags&64){
ts->d.ddpfPixelFormat.dwSize=sizeof(DDPIXELFORMAT);
ts->d.ddpfPixelFormat.dwFlags=DDPF_RGB;
ts->d.ddpfPixelFormat.dwRGBBitCount=16;
ts->d.ddpfPixelFormat.dwRBitMask=63488;
ts->d.ddpfPixelFormat.dwGBitMask=2016;
ts->d.ddpfPixelFormat.dwBBitMask=31;
goto gotpixelformat;
}
if (flags&1){
ts->d.ddpfPixelFormat.dwSize=sizeof(DDPIXELFORMAT);
ts->d.ddpfPixelFormat.dwFlags=DDPF_RGB;
ts->d.ddpfPixelFormat.dwRGBBitCount=32;
ts->d.ddpfPixelFormat.dwRBitMask=0xFF0000;
ts->d.ddpfPixelFormat.dwGBitMask=0x00FF00;
ts->d.ddpfPixelFormat.dwBBitMask=0x0000FF;
goto gotpixelformat;
}
ts->d.ddpfPixelFormat=DDRAW_display_pixelformat;
gotpixelformat:
//if (flags&4) ts->d.ddsCaps.dwCaps=DDSCAPS_TEXTURE;
/*
if (flags&8) {
ts->d.ddsCaps.dwCaps=DDSCAPS_ZBUFFER;
if (flags&1) ts->d.ddsCaps.dwCaps+=DDSCAPS_SYSTEMMEMORY;
ts->d.ddpfPixelFormat.dwFlags=DDPF_ZBUFFER;
ts->d.ddpfPixelFormat.dwRGBBitCount=32;
ts->d.ddpfPixelFormat.dwRBitMask=0x0;
ts->d.ddpfPixelFormat.dwGBitMask=0xFFFFFFFF;
ts->d.ddpfPixelFormat.dwBBitMask=0x0;
}
*/
if (dd->CreateSurface(&ts->d,&ts->s,NULL)!=DD_OK){
if (ts->d.ddsCaps.dwCaps&DDSCAPS_VIDEOMEMORY){
ts->d.ddsCaps.dwCaps^=DDSCAPS_VIDEOMEMORY;
ts->d.ddsCaps.dwCaps|=DDSCAPS_SYSTEMMEMORY;
MessageBox(NULL,"CreateSurface failed: VIDEOMEMORY","Ultima 6 Online",MB_OK); exit(1);
if (dd->CreateSurface(&ts->d,&ts->s,NULL)==DD_OK) goto ns_sysmem;
}
MessageBox(NULL,"CreateSurface failed","Ultima 6 Online",MB_OK); exit(1);
}
ns_sysmem:

if ((flags&1)||(flags&64)||(flags&32)) {
ddgetlock: if (DD_OK!=ts->s->Lock(NULL,&ts->d,DDLOCK_WAIT,NULL)) goto ddgetlock;
ts->o=(unsigned long*) ts->d.lpSurface;
ts->s->Unlock(NULL);
}
//if (flags&4) { *REDUNDANT
//ts->s->QueryInterface(IID_IDirect3DTexture2,(void**)&ts->t);
//}
static DDCOLORKEY cc;
cc.dwColorSpaceHighValue=0;
cc.dwColorSpaceLowValue=0;
ts->s->SetColorKey(DDCKEY_SRCBLT,&cc);
return ts;
}

void pset(surf* s,long x,long y,DWORD c)
{
if (x<0) return;
if (y<0) return;
if (y>=s->d.dwHeight) return;
if (x>=s->d.dwWidth) return;
if (s->o==NULL) return;
s->o[y*s->d.lPitch/4+x]=c;
return;
}

DWORD point(surf* s,long x,long y)
{
if (x<0) return 0xFFFFFFFF;
if (y<0) return 0xFFFFFFFF;
if (y>=s->d.dwHeight) return 0xFFFFFFFF;
if (x>=s->d.dwWidth) return  0xFFFFFFFF;
if (s->o==NULL) return 0;
return s->o[y*s->d.lPitch/4+x]&0xFFFFFF;
}

void cls(surf* s,DWORD c)
{
static DDBLTFX b;
b.dwSize=sizeof(DDBLTFX); 
b.dwFillColor=c;
s->s->Blt(NULL,NULL,NULL,DDBLT_WAIT|DDBLT_COLORFILL,&b);
return;
}

// rrr refresh(surf* s)
void refresh(surf* s)
{
static HDC winhdc,ddhdc;
static RECT clrect,wrect; //,drect;
static long x,x2,y;
static HWND ohWnd;
if (smallwindow){
//switch to 512x384 window if not currently displayed
/*
if (hWnd != hWnd3) {
ShowWindow(hWnd3, SW_SHOW);
UpdateWindow(hWnd3);
ShowWindow(hWnd2, SW_HIDE); //hide current window
hWnd = hWnd3;
}
*/
	
	if (windowsizecyclenum == 0) {
		if (hWnd != hWnd3) {
			ShowWindow(hWnd3, SW_SHOW);
			UpdateWindow(hWnd3);
			//ShowWindow(hWnd2, SW_HIDE); //hide current window
			ShowWindow(hWnd, SW_HIDE); //hide current window
			hWnd = hWnd3;

			resxz = resxs;
			resyz = resys;
			scalexm = (double)resxo / resxz;
			scaleym = (double)resyo / resyz;
		}
	}
	else if (windowsizecyclenum == 1) {
		if (hWnd != hWnd4) {
			ShowWindow(hWnd4, SW_SHOW);
			UpdateWindow(hWnd4);
			ShowWindow(hWnd, SW_HIDE); //hide current window
			hWnd = hWnd4;

			resxz = resxn1m;
			resyz = resyn1m;
			scalexm = (double)resxo / resxz;
			scaleym = (double)resyo / resyz;
		}
	}


// title bar window refresh
GetWindowRect(hWnd,&wrect);
GetClientRect(hWnd,&clrect);
//resxz = clrect.right;
//resyz = clrect.bottom;
x=wrect.right-wrect.left; //full window width
x2=(x-clrect.right)/2; //width of single border
x=x2;
y=(wrect.bottom-wrect.top)-clrect.bottom-x2;
winhdc=GetWindowDC(hWnd);
s->s->GetDC(&ddhdc);

/*
typedef struct BITMAPINFO656 {
BITMAPINFOHEADER bmiHeader;
DWORD red;
DWORD green;
DWORD blue;
};

static BITMAPINFO656 bmi;
ZeroMemory(&bmi,sizeof(BITMAPINFO656));
bmi.bmiHeader.biSize=sizeof(BITMAPINFO656);
bmi.bmiHeader.biWidth=512;
bmi.bmiHeader.biHeight=384;
bmi.bmiHeader.biPlanes=1;
bmi.bmiHeader.biBitCount=16;
bmi.bmiHeader.biCompression=BI_BITFIELDS;
bmi.bmiHeader.biSizeImage=0;
bmi.bmiHeader.biXPelsPerMeter=0;
bmi.bmiHeader.biYPelsPerMeter=0;
bmi.bmiHeader.biClrUsed=0;
bmi.bmiHeader.biClrImportant=0;
bmi.red=0xF800;
bmi.green=0x7E0;
bmi.blue=0x1F;

////
SetDIBitsToDevice(
  winhdc,                 // handle to DC
  x,               // x-coord of destination upper-left corner
  y,               // y-coord of destination upper-left corner 
  512,           // source rectangle width
  384,          // source rectangle height
  0,                // x-coord of source lower-left corner
  0,                // y-coord of source lower-left corner
  0,         // first scan line in array
  384,         // number of scan lines
  s->o,     // array of DIB bits
  (BITMAPINFO*)&bmi, // bitmap information
  DIB_RGB_COLORS          // RGB or palette indexes
);
*/

BitBlt(winhdc,x,y,s->d.dwWidth,s->d.dwHeight,ddhdc,0,0,SRCCOPY);

s->s->ReleaseDC(ddhdc);
ReleaseDC(hWnd,winhdc);
return;


}


//switch to 1024x768 window if not currently displayed
if (hWnd!=hWnd2){
ShowWindow(hWnd2,SW_SHOW);
UpdateWindow(hWnd2);
//ShowWindow(hWnd3,SW_HIDE); //hide current window
ShowWindow(hWnd, SW_HIDE); //hide current window
hWnd=hWnd2;
}
//1024x768 title bar window refresh
if ((desktop_rect.right>1024)&&(desktop_rect.bottom>768)){
GetWindowRect(hWnd,&wrect);
GetClientRect(hWnd,&clrect);
x=wrect.right-wrect.left; //full window width
x2=(x-clrect.right)/2; //width of single border
x=x2;
y=(wrect.bottom-wrect.top)-clrect.bottom-x2;
winhdc=GetWindowDC(hWnd);
s->s->GetDC(&ddhdc);
BitBlt(winhdc,x,y,s->d.dwWidth,s->d.dwHeight,ddhdc,0,0,SRCCOPY);
s->s->ReleaseDC(ddhdc);
ReleaseDC(hWnd,winhdc);
return;
}
//1024x768 full screen refresh

/*
static unsigned long screen_offset,buffer_offset;
screen_offset=(unsigned long)vs->o;
buffer_offset=(unsigned long)s->o;
_asm{
push esi
push edi
push ebx
mov esi,buffer_offset
mov edi,screen_offset
sub edi,8
mov ebx,buffer_offset
add ebx,4
sub ebx,8
mov edx,screen_offset
add edx,4
sub edx,8
push ebp
mov ebp,196608
refresh0:
add ebx,8
mov eax,[esi]
add edi,8
mov ecx,[ebx]
add edx,8
mov [edi],eax
add esi,8
mov [edx],ecx
dec ebp
jnz refresh0
pop ebp
pop ebx
pop edi
pop esi
};
*/

/*
typedef struct BITMAPINFO656 {
BITMAPINFOHEADER bmiHeader;
DWORD red;
DWORD green;
DWORD blue;
};

static BITMAPINFO656 bmi;
ZeroMemory(&bmi,sizeof(BITMAPINFO656));
bmi.bmiHeader.biSize=sizeof(BITMAPINFO656);
bmi.bmiHeader.biWidth=1024;
bmi.bmiHeader.biHeight=768;
bmi.bmiHeader.biPlanes=1;
bmi.bmiHeader.biBitCount=16;
bmi.bmiHeader.biCompression=BI_BITFIELDS;
bmi.bmiHeader.biSizeImage=0;
bmi.bmiHeader.biXPelsPerMeter=0;
bmi.bmiHeader.biYPelsPerMeter=0;
bmi.bmiHeader.biClrUsed=0;
bmi.bmiHeader.biClrImportant=0;
bmi.red=0xF800;
bmi.green=0x7E0;
bmi.blue=0x1F;

////BitBlt(winhdc,x,y,s->d.dwWidth,s->d.dwHeight,ddhdc,0,0,SRCCOPY);
SetDIBitsToDevice(
  winhdc,                 // handle to DC
  0,               // x-coord of destination upper-left corner
  0,               // y-coord of destination upper-left corner 
  1024,           // source rectangle width
  768,          // source rectangle height
  0,                // x-coord of source lower-left corner
  0,                // y-coord of source lower-left corner
  0,         // first scan line in array
  768,         // number of scan lines
  s->o,     // array of DIB bits
  (BITMAPINFO*)&bmi, // bitmap information
  DIB_RGB_COLORS          // RGB or palette indexes
);
*/


winhdc=GetWindowDC(hWnd);
s->s->GetDC(&ddhdc);
BitBlt(winhdc,0,0,s->d.dwWidth,s->d.dwHeight,ddhdc,0,0,SRCCOPY);
s->s->ReleaseDC(ddhdc);
ReleaseDC(hWnd,winhdc);


}//refresh end


//ebx/edi/esi are NOT backed up!!

//assembly passing variables


void img(surf* d,long x,long y,surf* s)
{
//static variables
static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
static long asm_copy_vc_extra2bytes;
static long x2,y2,x3,y3,x4,y4;
static long sx,sy,dx,dy; //size of source and destination x and y axis
//surfaces valid?
if (s==NULL) return;
if (d==NULL) return;
//offscreen?
dx=d->d.dwWidth;
if (x>=dx) return;
dy=d->d.dwHeight;
if (y>=dy) return;
sx=s->d.dwWidth;
if (-x>=sx) return;
sy=s->d.dwHeight;
if (-y>=sy) return;
x2=x;//starting dest x offset
x3=0;//starting source x offset
x4=sx; //pixels onscreen of x axis
//part of the image is onscreen
if (x<0){x4+=x; x2=0; x3=-x;}
if ((x+sx)>dx) x4-=x+sx-dx;
//x is established, now for y
y2=y;//starting dest y offset
y3=0;//starting source y offset
y4=sy; //rows on screen
if (y<0){y4+=y; y2=0; y3=-y;}
if ((y+sy)>dy) y4-=y+sy-dy;
asm_copy_vc_bytesx=x4*2;
asm_copy_vc_sourceskip=(long)s->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_destskip=(long)d->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_sourceoffset=(unsigned long)s->o+x3*2+y3*(long)s->d.lPitch;
asm_copy_vc_destoffset=(unsigned long)d->o+x2*2+y2*(long)d->d.lPitch;
asm_copy_vc_rows=y4;
if (asm_copy_vc_bytesx&2) {asm_copy_vc_bytesx-=2; asm_copy_vc_extra2bytes=1;}else{asm_copy_vc_extra2bytes=0;}
_asm{
push esi
push edi
push ebx
mov ecx,asm_copy_vc_rows
mov edx,asm_copy_vc_bytesx
mov esi,asm_copy_vc_sourceoffset
mov edi,asm_copy_vc_destoffset
asm_copy1:
mov ebx,esi
add ebx,edx
and edx,edx
jz asm_copy7
asm_copy0:
mov eax,[esi]
add esi,4

//and eax,4158584798
//shr eax,1
//and DWORD PTR [edi],4158584798
//shr DWORD PTR [edi],1
//add [edi],eax

mov [edi],eax


add edi,4
cmp esi,ebx
jne asm_copy0
cmp asm_copy_vc_extra2bytes,0
je asm_copy3
asm_copy7:
mov ax,[esi]
add esi,2
mov [edi],ax
add edi,2
asm_copy3:
add esi,asm_copy_vc_sourceskip
add edi,asm_copy_vc_destskip
dec ecx
jnz asm_copy1
pop ebx
pop edi
pop esi
}

}//img(...)

//changes an image that would have has a colour key to use 0 for that colour, existing 0 changed to 1 greenscale!
void img0_0key(surf *s, unsigned short c){
static unsigned long i;
static unsigned short c2;
for (i=0;i<(s->d.lPitch/2*s->d.dwHeight);i++){
c2=s->o2[i];
if (c2==c){
s->o2[i]=0;
}else{
if (c2==0) s->o2[i]=32;
}
}
}//img0_0key(...)

void img0(surf* d,long x,long y,surf* s)
{
//static variables
static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
static long asm_copy_vc_extra2bytes;
static long x2,y2,x3,y3,x4,y4;
static long sx,sy,dx,dy; //size of source and destination x and y axis
//surfaces valid?
if (s==NULL) return;
if (d==NULL) return;
//offscreen?
dx=d->d.dwWidth;
if (x>=dx) return;
dy=d->d.dwHeight;
if (y>=dy) return;
sx=s->d.dwWidth;
if (-x>=sx) return;
sy=s->d.dwHeight;
if (-y>=sy) return;
x2=x;//starting dest x offset
x3=0;//starting source x offset
x4=sx; //pixels onscreen of x axis
//part of the image is onscreen
if (x<0){x4+=x; x2=0; x3=-x;}
if ((x+sx)>dx) x4-=x+sx-dx;
//x is established, now for y
y2=y;//starting dest y offset
y3=0;//starting source y offset
y4=sy; //rows on screen
if (y<0){y4+=y; y2=0; y3=-y;}
if ((y+sy)>dy) y4-=y+sy-dy;
asm_copy_vc_bytesx=x4*2;
asm_copy_vc_sourceskip=(long)s->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_destskip=(long)d->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_sourceoffset=(unsigned long)s->o+x3*2+y3*(long)s->d.lPitch;
asm_copy_vc_destoffset=(unsigned long)d->o+x2*2+y2*(long)d->d.lPitch;
asm_copy_vc_rows=y4;
if (asm_copy_vc_bytesx&2) {asm_copy_vc_bytesx-=2; asm_copy_vc_extra2bytes=1;}else{asm_copy_vc_extra2bytes=0;}
_asm{
push esi
push edi
push ebx
mov ecx,asm_copy_vc_rows
mov edx,asm_copy_vc_bytesx
mov esi,asm_copy_vc_sourceoffset
mov edi,asm_copy_vc_destoffset
asm_copy1:
mov ebx,esi
add ebx,edx
and edx,edx
jz asm_copy7
asm_copy0:
mov eax,[esi]
and ax,ax
jz asm_copy3
mov [edi],ax
asm_copy3:
add esi,4
shr eax,16
add edi,2
and ax,ax
jz asm_copy4
mov [edi],ax
asm_copy4:
add edi,2
cmp esi,ebx
jne asm_copy0
cmp asm_copy_vc_extra2bytes,0
je asm_copy5
asm_copy7:
mov ax,[esi]
and ax,ax
jz asm_copy6
mov [edi],ax
asm_copy6:
add esi,2
add edi,2
asm_copy5:
add esi,asm_copy_vc_sourceskip
add edi,asm_copy_vc_destskip
dec ecx
jnz asm_copy1
pop ebx
pop edi
pop esi
}

}//img0(...)


/*
void img(surf* d,long x,long y,surf* s)
{
if (s==NULL) return;
if (d==NULL) return;
static RECT r1,r2;
r1.top=y;
r1.bottom=y+s->d.dwHeight;
r1.left=x;
r1.right=x+s->d.dwWidth;
r2.left=0;
r2.top=0;
r2.bottom=s->d.dwHeight;
r2.right=s->d.dwWidth;

if (r1.right>d->d.dwWidth)
{
r2.right-=r1.right-d->d.dwWidth;
r1.right=d->d.dwWidth;
}
if (r1.bottom>d->d.dwHeight)
{
r2.bottom-=r1.bottom-d->d.dwHeight;
r1.bottom=d->d.dwHeight;
}
if (r1.left<0)
{
r2.left-=r1.left;
r1.left=0;
}
if (r1.top<0)
{
r2.top-=r1.top;
r1.top=0;
}
d->s->Blt(&r1,s->s,&r2,DDBLT_WAIT,NULL);
return;
}
*/

void img(surf* d,surf* s)
{
d->s->Blt(NULL,s->s,NULL,DDBLT_WAIT,NULL);
}

// r999 img to handle resizing and positioning
void img(surf* d, surf* s, int x, int y, int x2, int y2) {
	RECT drect;
	drect.left = x;
	drect.right = x2;
	drect.top = y;
	drect.bottom = y2;
	d->s->Blt(&drect, s->s, NULL, DDBLT_WAIT, NULL);
}

// r999
void imguiw(surf* d, int uipaneli, int uiwidgeti, int uistatei, surf* s) {
	if (uipanelusedefaultstatedata[uipaneli][uiwidgeti][uistatei] == 1)
		img(d, uipanelx[uipaneli][uiwidgeti][UI_STATE_DEF], uipanely[uipaneli][uiwidgeti][UI_STATE_DEF], s);
	else
		img(d, uipanelx[uipaneli][uiwidgeti][uistatei], uipanely[uipaneli][uiwidgeti][uistatei], s);
}

void imguiw(surf* d, int uipaneli, int uiwidgeti, int uistatei) {
	/*
	if (uipanelusedefaultstatedata[uipaneli][uiwidgeti][uistatei] == 1)
		img(d, uipanelx[uipaneli][uiwidgeti][UI_STATE_DEF], uipanely[uipaneli][uiwidgeti][UI_STATE_DEF], uipanelsurf[uipaneli][uiwidgeti][uistatei]);
	else
		img(d, uipanelx[uipaneli][uiwidgeti][uistatei], uipanely[uipaneli][uiwidgeti][uistatei], uipanelsurf[uipaneli][uiwidgeti][uistatei]);
	*/
	imguiw(d, uipaneli, uiwidgeti, uistatei, uipanelsurf[uipaneli][uiwidgeti][uistatei]);
}

void imguip(surf* d, int uipaneli, surf* s) {
	imguiw(d, uipaneli, UI_WIDGET_DEF, UI_STATE_DEF, s);
}

void imguip(surf* d, int uipaneli) {
	imguiw(d, uipaneli, UI_WIDGET_DEF, UI_STATE_DEF);
}



DWORD fixcol(DWORD c)
{
static unsigned char r,g,b;
r=c&255;
g=(c&0xFF00)>>8;
b=(c&0xFF0000)>>16;
return 65536*r+256*g+b;
}



/*void clear_font(HGDIOBJ fnt) {
  DeleteObject(fnt);
  if(!fnt1) {
    fnt1=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt1;
  }
  if(!fnt1naa) {
    fnt1naa=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt1naa;
  }  
  if(!fnt2) {
    fnt2=CreateFont(16,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt2;
  }  
  if(!fnt3) {
    fnt3=CreateFont(8,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Terminal");
    txtfnt=fnt3;
  }
  //fnt4=CreateFont(18,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
  if(!fnt4) {
    fnt4=CreateFont(22,8,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt4;
  }
  //rune
  if(!fnt5) {
    fnt5=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Ultima Runes");
    txtfnt=fnt5;
  }
  //gargish
  if(!fnt6) {
    fnt6=CreateFont(22,0,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Gargish");
    txtfnt=fnt6;
  }
  //u6o2 (used for startup menu)
  if(!fnt7) {
    fnt7=CreateFont(24,9,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"PerryGothic");
    txtfnt=fnt7;
  }
}*/

void txtout(surf* s,long x,long y,txt* t) //MEMLEAKING A LOT! thats why I added delete object and it works, but the font is fucked up if deleted right away
{
  static HDC pdc;
  static HGDIOBJ last_font;
  s->s->GetDC(&pdc);
  last_font=SelectObject(pdc,txtfnt);
  SelectObject(pdc,txtfnt);
  if ((txtcol&0xFF000000)==0) SetBkMode(pdc,TRANSPARENT); 
  SetTextColor(pdc,fixcol(txtcol));
  TextOut(pdc,x,y,t->d,t->l);
  //clear_font(SelectObject(pdc, last_font));
  //DeleteObject(SelectObject(pdc, last_font));
  /*if(fobjs==1024) {//buffer not big enough !
    clear_font_buffer();
  }
  font_objs[fobjs]=SelectObject(pdc, last_font);;
  fobjs++;*/
  //s->s->ReleaseDC(pdc);
  s->s->ReleaseDC(pdc);
  /*if(!(s->s->ReleaseDC(pdc))){
    //error releasing shit!
    clear_font_buffer();
  }*/
  
return;
}

void txtouts(surf* s,long x,long y,txt* t) //creates a shadow behind the text (8,8,8)
{
  static HDC pdc;
  static HGDIOBJ last_font;
  s->s->GetDC(&pdc);
  //last_font=SelectObject(pdc,txtfnt);
  SelectObject(pdc,txtfnt);
  if ((txtcol&0xFF000000)==0) SetBkMode(pdc,TRANSPARENT); 
  SetTextColor(pdc,8+8*256+8*65536); //8,8,8
  TextOut(pdc,x-1,y,t->d,t->l);
  TextOut(pdc,x+1,y,t->d,t->l);
  TextOut(pdc,x,y-1,t->d,t->l);
  TextOut(pdc,x,y+1,t->d,t->l);
  SetTextColor(pdc,fixcol(txtcol));
  TextOut(pdc,x,y,t->d,t->l);

  //clear_font(SelectObject(pdc, last_font));

  //DeleteObject(SelectObject(pdc, last_font));
  s->s->ReleaseDC(pdc);
return;
}



DWORD getcol(DWORD c)
{
static CHOOSECOLOR ccx;
static unsigned long cccs[32];
ShowWindow(hWnd,SW_HIDE);
ccx.lStructSize=sizeof(CHOOSECOLOR);
ccx.hInstance=NULL;
ccx.Flags=CC_FULLOPEN|CC_RGBINIT;
ccx.hwndOwner=NULL;
ccx.lCustData=NULL;
ccx.lpCustColors=&cccs[0];
ccx.lpfnHook=NULL;
ccx.lpTemplateName=NULL;
ccx.rgbResult=fixcol(c);
if (ChooseColor(&ccx)==NULL) ccx.rgbResult=0;
ShowWindow(hWnd,SW_SHOW);
return fixcol(ccx.rgbResult);
}

void purgesurfaces()
{
static long i;
for (i=0;i<16384;i++)
{
if (surflist[i]!=NULL)
{
surflist[i]->s->Release();
}
}
return;
}

/*
void img0(surf* d,long x,long y,surf* s)
{
static RECT r1,r2;
r1.top=y;
r1.bottom=y+s->d.dwHeight;
r1.left=x;
r1.right=x+s->d.dwWidth;
r2.left=0;
r2.top=0;
r2.bottom=s->d.dwHeight;
r2.right=s->d.dwWidth;

if (r1.right>d->d.dwWidth)
{
r2.right-=r1.right-d->d.dwWidth;
r1.right=d->d.dwWidth;
}
if (r1.bottom>d->d.dwHeight)
{
r2.bottom-=r1.bottom-d->d.dwHeight;
r1.bottom=d->d.dwHeight;
}
if (r1.left<0)
{
r2.left-=r1.left;
r1.left=0;
}
if (r1.top<0)
{
r2.top-=r1.top;
r1.top=0;
}
r1.right--;
r2.right--;
d->s->Blt(&r1,s->s,&r2,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
r1.right++;
r2.right++;
r1.left=r1.right-1;
r2.left=r2.right-1;
d->s->Blt(&r1,s->s,&r2,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
return;
}
*/

void img0(surf* d,surf* s)
{
d->s->Blt(NULL,s->s,NULL,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
}

surf* loadimage(LPCSTR name,long flags){
static HBITMAP bmh; //handle to loaded bitmap
static BITMAP bm; //bitmap info buffer
static long bmx,bmy; //width, height
static surf* s; //temp surf pointer, for new image
static HDC sdc,bdc; //surface device, bitmap device
bmh=(HBITMAP)LoadImage(hInst,name,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
if (bmh==NULL) return NULL;
GetObject(bmh,sizeof(BITMAP),&bm);
bmx=(DWORD)bm.bmWidth;
bmy=(DWORD)bm.bmHeight;
s=newsurf(bmx,bmy,flags); //1=SURF_SYSMEM
bdc=CreateCompatibleDC(NULL);
SelectObject(bdc,bmh);
s->s->GetDC(&sdc);
BitBlt(sdc,0,0,bmx,bmy,bdc,0,0,SRCCOPY);
s->s->ReleaseDC(sdc);
DeleteDC(bdc);
DeleteObject(bmh);
return s;
}

surf* loadimage(LPCSTR name){
return loadimage(name,NULL);
}
surf* loadimage(txt* name){
return loadimage(name->d,NULL); 
}
surf* loadimage(txt* name,long flags){
return loadimage(name->d,flags); 
}

void free(surf* s)
{
static long i;
for (i=0;i<16384;i++)
{
if (surflist[i]==s) surflist[i]=NULL;
}
s->s->Release();
free((void*)s);
return;
}

void imgt0(surf* d,long x,long y,surf* s)
{
//static variables
static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
static long asm_copy_vc_extra2bytes;
static long x2,y2,x3,y3,x4,y4;
static long sx,sy,dx,dy; //size of source and destination x and y axis
//surfaces valid?
if (s==NULL) return;
if (d==NULL) return;
//offscreen?
dx=d->d.dwWidth;
if (x>=dx) return;
dy=d->d.dwHeight;
if (y>=dy) return;
sx=s->d.dwWidth;
if (-x>=sx) return;
sy=s->d.dwHeight;
if (-y>=sy) return;
x2=x;//starting dest x offset
x3=0;//starting source x offset
x4=sx; //pixels onscreen of x axis
//part of the image is onscreen
if (x<0){x4+=x; x2=0; x3=-x;}
if ((x+sx)>dx) x4-=x+sx-dx;
//x is established, now for y
y2=y;//starting dest y offset
y3=0;//starting source y offset
y4=sy; //rows on screen
if (y<0){y4+=y; y2=0; y3=-y;}
if ((y+sy)>dy) y4-=y+sy-dy;
asm_copy_vc_bytesx=x4*2;
asm_copy_vc_sourceskip=(long)s->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_destskip=(long)d->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_sourceoffset=(unsigned long)s->o+x3*2+y3*(long)s->d.lPitch;
asm_copy_vc_destoffset=(unsigned long)d->o+x2*2+y2*(long)d->d.lPitch;
asm_copy_vc_rows=y4;
if (asm_copy_vc_bytesx&2) {asm_copy_vc_bytesx-=2; asm_copy_vc_extra2bytes=1;}else{asm_copy_vc_extra2bytes=0;}
_asm{
push esi
push edi
push ebx
push ebp
mov ecx,asm_copy_vc_rows
mov edx,asm_copy_vc_bytesx
mov esi,asm_copy_vc_sourceoffset
mov edi,asm_copy_vc_destoffset
asm_copy1:
mov ebx,esi
add ebx,edx
and edx,edx
jz asm_copy7
asm_copy0:
mov eax,[esi]
mov ebp,[edi]
add esi,4

and ax,ax
jz asm_copy_imgt0_1
and bp,63454
and ax,63454
shr bp,1
shr ax,1
add ax,bp
mov [edi],ax
asm_copy_imgt0_1:

shr ebp,16
shr eax,16
add edi,2

and ax,ax
jz asm_copy_imgt0_2
and bp,63454
and ax,63454
shr bp,1
shr ax,1
add ax,bp
mov [edi],ax
asm_copy_imgt0_2:

add edi,2

cmp esi,ebx
jne asm_copy0
cmp asm_copy_vc_extra2bytes,0
je asm_copy3
asm_copy7:





mov ax,[esi]
mov bp,[edi]
add esi,2

and ax,ax
jz asm_copy_imgt0_3
and bp,63454
and ax,63454
shr bp,1
shr ax,1
add ax,bp
mov [edi],ax
asm_copy_imgt0_3:

add edi,2
asm_copy3:
add esi,asm_copy_vc_sourceskip
add edi,asm_copy_vc_destskip
dec ecx
jnz asm_copy1
pop ebp
pop ebx
pop edi
pop esi
}

}//img(...)


//transparent image function
void imgt(surf* d,long x,long y,surf* s)
{
//static variables
static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
static long asm_copy_vc_extra2bytes;
static long x2,y2,x3,y3,x4,y4;
static long sx,sy,dx,dy; //size of source and destination x and y axis
//surfaces valid?
if (s==NULL) return;
if (d==NULL) return;
//offscreen?
dx=d->d.dwWidth;
if (x>=dx) return;
dy=d->d.dwHeight;
if (y>=dy) return;
sx=s->d.dwWidth;
if (-x>=sx) return;
sy=s->d.dwHeight;
if (-y>=sy) return;
x2=x;//starting dest x offset
x3=0;//starting source x offset
x4=sx; //pixels onscreen of x axis
//part of the image is onscreen
if (x<0){x4+=x; x2=0; x3=-x;}
if ((x+sx)>dx) x4-=x+sx-dx;
//x is established, now for y
y2=y;//starting dest y offset
y3=0;//starting source y offset
y4=sy; //rows on screen
if (y<0){y4+=y; y2=0; y3=-y;}
if ((y+sy)>dy) y4-=y+sy-dy;
asm_copy_vc_bytesx=x4*2;
asm_copy_vc_sourceskip=(long)s->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_destskip=(long)d->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_sourceoffset=(unsigned long)s->o+x3*2+y3*(long)s->d.lPitch;
asm_copy_vc_destoffset=(unsigned long)d->o+x2*2+y2*(long)d->d.lPitch;
asm_copy_vc_rows=y4;
if (asm_copy_vc_bytesx&2) {asm_copy_vc_bytesx-=2; asm_copy_vc_extra2bytes=1;}else{asm_copy_vc_extra2bytes=0;}
_asm{
push esi
push edi
push ebx
push ebp
mov ecx,asm_copy_vc_rows
mov edx,asm_copy_vc_bytesx
mov esi,asm_copy_vc_sourceoffset
mov edi,asm_copy_vc_destoffset
asm_copy1:
mov ebx,esi
add ebx,edx
and edx,edx
jz asm_copy7
asm_copy0:
mov eax,[esi]
mov ebp,[edi]
add esi,4
and eax,4158584798
and ebp,4158584798
shr eax,1
shr ebp,1
add eax,ebp
mov [edi],eax
add edi,4
cmp esi,ebx
jne asm_copy0
cmp asm_copy_vc_extra2bytes,0
je asm_copy3
asm_copy7:
mov ax,[esi]
mov bp,[edi]
add esi,2
and ax,63454
and bp,63454
shr ax,1
shr bp,1
add ax,bp
mov [edi],ax
add edi,2
asm_copy3:
add esi,asm_copy_vc_sourceskip
add edi,asm_copy_vc_destskip
dec ecx
jnz asm_copy1
pop ebp
pop ebx
pop edi
pop esi
}

}//img(...)


//75% transparency!
void img75t0(surf* d,long x,long y,surf* s)
{
//static variables
static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
static long asm_copy_vc_extra2bytes;
static long x2,y2,x3,y3,x4,y4;
static long sx,sy,dx,dy; //size of source and destination x and y axis
//surfaces valid?
if (s==NULL) return;
if (d==NULL) return;
//offscreen?
dx=d->d.dwWidth;
if (x>=dx) return;
dy=d->d.dwHeight;
if (y>=dy) return;
sx=s->d.dwWidth;
if (-x>=sx) return;
sy=s->d.dwHeight;
if (-y>=sy) return;
x2=x;//starting dest x offset
x3=0;//starting source x offset
x4=sx; //pixels onscreen of x axis
//part of the image is onscreen
if (x<0){x4+=x; x2=0; x3=-x;}
if ((x+sx)>dx) x4-=x+sx-dx;
//x is established, now for y
y2=y;//starting dest y offset
y3=0;//starting source y offset
y4=sy; //rows on screen
if (y<0){y4+=y; y2=0; y3=-y;}
if ((y+sy)>dy) y4-=y+sy-dy;
asm_copy_vc_bytesx=x4*2;
asm_copy_vc_sourceskip=(long)s->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_destskip=(long)d->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_sourceoffset=(unsigned long)s->o+x3*2+y3*(long)s->d.lPitch;
asm_copy_vc_destoffset=(unsigned long)d->o+x2*2+y2*(long)d->d.lPitch;
asm_copy_vc_rows=y4;
if (asm_copy_vc_bytesx&2) {asm_copy_vc_bytesx-=2; asm_copy_vc_extra2bytes=1;}else{asm_copy_vc_extra2bytes=0;}
_asm{
push esi
push edi
push ebx
push ebp
mov ecx,asm_copy_vc_rows
mov edx,asm_copy_vc_bytesx
mov esi,asm_copy_vc_sourceoffset
mov edi,asm_copy_vc_destoffset
asm_copy1:
mov ebx,esi
add ebx,edx
and edx,edx
jz asm_copy7
asm_copy0:
mov eax,[esi]
mov ebp,[edi]
add esi,4

and ax,ax
jz asm_copy_imgt0_1
and bp,59292
and ax,63454
shr bp,2 //bp=25% of dest
shr ax,1 //ax=50% of source
add bp,ax //bp=25%(bp)+50%(ax)=75%
and ax,63454
shr ax,1 //ax=25% of source
add bp,ax //bp=75%(bp)+25%(ax)=100%
mov [edi],bp
asm_copy_imgt0_1:

shr ebp,16
shr eax,16
add edi,2

and ax,ax
jz asm_copy_imgt0_2

and bp,59292
and ax,63454
shr bp,2 //25%
shr ax,1 //50%
add bp,ax
and ax,63454
shr ax,1 //50%
add bp,ax
mov [edi],bp

asm_copy_imgt0_2:

add edi,2

cmp esi,ebx
jne asm_copy0
cmp asm_copy_vc_extra2bytes,0
je asm_copy3
asm_copy7:





mov ax,[esi]
mov bp,[edi]
add esi,2

and ax,ax
jz asm_copy_imgt0_3

and bp,59292
and ax,63454
shr bp,2 //25%
shr ax,1 //50%
add bp,ax
and ax,63454
shr ax,1 //50%
add bp,ax
mov [edi],bp

asm_copy_imgt0_3:

add edi,2
asm_copy3:
add esi,asm_copy_vc_sourceskip
add edi,asm_copy_vc_destskip
dec ecx
jnz asm_copy1
pop ebp
pop ebx
pop edi
pop esi
}

}//img(...)






//75% transparency!
void img75t(surf* d,long x,long y,surf* s)
{
//static variables
static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
static long asm_copy_vc_extra2bytes;
static long x2,y2,x3,y3,x4,y4;
static long sx,sy,dx,dy; //size of source and destination x and y axis
//surfaces valid?
if (s==NULL) return;
if (d==NULL) return;
//offscreen?
dx=d->d.dwWidth;
if (x>=dx) return;
dy=d->d.dwHeight;
if (y>=dy) return;
sx=s->d.dwWidth;
if (-x>=sx) return;
sy=s->d.dwHeight;
if (-y>=sy) return;
x2=x;//starting dest x offset
x3=0;//starting source x offset
x4=sx; //pixels onscreen of x axis
//part of the image is onscreen
if (x<0){x4+=x; x2=0; x3=-x;}
if ((x+sx)>dx) x4-=x+sx-dx;
//x is established, now for y
y2=y;//starting dest y offset
y3=0;//starting source y offset
y4=sy; //rows on screen
if (y<0){y4+=y; y2=0; y3=-y;}
if ((y+sy)>dy) y4-=y+sy-dy;
asm_copy_vc_bytesx=x4*2;
asm_copy_vc_sourceskip=(long)s->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_destskip=(long)d->d.lPitch-asm_copy_vc_bytesx;
asm_copy_vc_sourceoffset=(unsigned long)s->o+x3*2+y3*(long)s->d.lPitch;
asm_copy_vc_destoffset=(unsigned long)d->o+x2*2+y2*(long)d->d.lPitch;
asm_copy_vc_rows=y4;
if (asm_copy_vc_bytesx&2) {asm_copy_vc_bytesx-=2; asm_copy_vc_extra2bytes=1;}else{asm_copy_vc_extra2bytes=0;}
_asm{
push esi
push edi
push ebx
push ebp
mov ecx,asm_copy_vc_rows
mov edx,asm_copy_vc_bytesx
mov esi,asm_copy_vc_sourceoffset
mov edi,asm_copy_vc_destoffset
asm_copy1:
mov ebx,esi
add ebx,edx
and edx,edx
jz asm_copy7
asm_copy0:

mov eax,[esi]
mov ebp,[edi]
and eax,4158584798 //11110111110111101111011111011110
and ebp,3885819804 //11100111100111001110011110011100
shr eax,1 //eax=50% of source
shr ebp,2 //ebp=25% of dest
add esi,4
add ebp,eax //ebp=25%(ebp)+50%(eax)=75%
and eax,4158584798 //11110111110111101111011111011110
shr eax,1 //eax=25% of source
add ebp,eax //bp=75%(bp)+25%(ax)=100%
mov [edi],ebp
add edi,4

cmp esi,ebx
jne asm_copy0
cmp asm_copy_vc_extra2bytes,0
je asm_copy3
asm_copy7:

mov ax,[esi]
mov bp,[edi]
add esi,2
and bp,59292
and ax,63454
shr bp,2 //25%
shr ax,1 //50%
add bp,ax
and ax,63454
shr ax,1 //50%
add bp,ax
mov [edi],bp
add edi,2

asm_copy3:
add esi,asm_copy_vc_sourceskip
add edi,asm_copy_vc_destskip
dec ecx
jnz asm_copy1
pop ebp
pop ebx
pop edi
pop esi
}

}//img(...)
