//ws2_32.lib winmm.lib
#include "stdafx.h"
#include "u6o7.h"
#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include <direct.h>
#include <wininet.h>
#include <winreg.h>
#include "dmusic.h"
#include "myfile.h"
#include "myddraw.h"
#include "mytxt.h"
#include "sound.h"
#include "myddraw.h"
#include "frame.h"
#include "windows.h"

#ifdef CONSOLE
#include <conio.h>
#endif

/* define these in the project, as it also ties in with what to link
//#define CLIENT 
//#define HOST
/*
OPTIONS
i) define CLIENT only, don't link:
  house, function_host, data_host, usecode, spellcode
  this is the "SLIM CLIENT"
ii) define HOST only, this is the "SLIM HOST" ("host" param is assumed, house.cpp must be present)
iii) define HOST & CLIENT:
a) if "host" param present; this is the "TESTING HOST" (house.cpp must be present)
b) if "host" param absent; this is the "TESTING CLIENT" (house.cpp must be present)
*/
/* #define U6O_DEBUG TRUE */ /* moved to define_both.h */

#define SECURE

/*luteijn: Disabled this pragma to help with cleaning code but that opens up an enormous can of worms...*/
#pragma warning(disable:4244) //conversion from 'unsigned long' to 'unsigned short', possible loss of data
/*luteijn: Disabled these pragma's to help with cleaning code. Made me sad.*/
#pragma warning(disable:4018) //signed/unsigned mismatch



//#define TCHAR unsigned char



#include "define_both.h"//host and client definitions
#ifdef HOST
#include "define_host.h"//host definitions
#include "use_code/use_code.h"  // host only
#include "spell_code/spell_code.h"  // host only
#endif
#ifdef CLIENT
#include "define_client.h"//client definitions
#endif


#include "data_both.h"//host and client data
#ifdef HOST
#include "data_host.h"//host data
#endif
#ifdef CLIENT
#include "data_client.h"//client data
#endif

#include "function_both.h"//host and client functions
#ifdef HOST
#include "function_host.h"//host functions
#endif
#ifdef CLIENT
#include "function_client.h"//client functions
#endif


#include "globals.inc" // all the global variables are to be grouped here

static txt *logtext = txtnew();
static txt *ltt1 = txtnew();
static int iii = 0;
//static txt *ltt2 = txtnew();

#ifdef CONSOLE
char cyn=0;
void KeyEventProc(KEY_EVENT_RECORD ker) {
  if (ker.bKeyDown == false) { // Only if key is released
    if(!cyn && (char)(ker.uChar.UnicodeChar) == 'q') {
      _cprintf("Really quit? y/n\n");
      cyn=1;
      return;
    }
    else if(cyn==1 && (char)(ker.uChar.UnicodeChar) == 'y') {
      _cprintf("y\n Shutting down...\n");
      exitrequest=TRUE; exitrequest_noconfirm=TRUE;
      cyn=2;
      return;
    }
    else if(cyn==1 && (char)(ker.uChar.UnicodeChar) == 'n') {
      _cprintf("n\n");
      cyn=0;
      return;
    }
  }
}
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{

	//temporary use variables
	static long i=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,i9=0;
	static long x=0,x2=0,x3=0,x4=0,x5=0,x6=0,x7=0,x8=0,x9=0;
	static long y=0,y2=0,y3=0,y4=0,y5=0,y6=0,y7=0,y8=0,y9=0;
	static long z=0,z2=0,z3=0,z4=0,z5=0,z6=0,z7=0,z8=0,z9=0;
	static double d=0,d2=0,d3=0,d4=0,d5=0,d6=0,d7=0,d8=0,d9=0;
	static unsigned char b=0,b2=0,b3=0,b4=0,b5=0,b6=0,b7=0,b8=0,b9=0;
	static float f=0,f2=0,f3=0,f4=0,f5=0,f6=0,f7=0,f8=0,f9=0;
	static file *tfh,*tfh2,*tfh3,*tfh4,*tfh5,*tfh6,*tfh7,*tfh8,*tfh9;
	static txt *t=txtnew(),*t2=txtnew(),*t3=txtnew(),*t4=txtnew(),*t5=txtnew(),*t6=txtnew(),*t7=txtnew(),*t8=txtnew(),*t9=txtnew();
	static object *myobj,*myobj2,*myobj3,*myobj4,*myobj5,*myobj6,*myobj7,*myobj8,*myobj9;
	static creature *crt,*crt2,*crt3,*crt4,*crt5,*crt6,*crt7,*crt8,*crt9;
	// r666
	static txt *newt1 = txtnew();

	// t111
	//object *moblistnew[20];
	//unsigned int mobcount = 0;
	//for (int mi=0; mi<20; mi++)
	//	moblistnew[mi]=NULL;

#ifdef CONSOLE /* creates a console for the dedicated host */
    HANDLE hStdin; 
    DWORD cNumRead=0; 
    INPUT_RECORD irInBuf[1]; 
    AllocConsole();
    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    RemoveMenu(GetSystemMenu(GetConsoleWindow(), FALSE),SC_CLOSE, MF_BYCOMMAND);
    //ShowWindow(GetConsoleWindow(), SW_SHOWMINIMIZED); /* SW_HIDE , SW_SHOW, SW_SHOWMINIMIZED */
	ShowWindow(GetConsoleWindow(), SW_SHOW); /* SW_HIDE , SW_SHOW, SW_SHOWMINIMIZED */
#endif
	frame_init();  // luteijn: setup the globals originally defined in frame.h
	data_both_init(); // luteijn: setup the globals originally defined in data_both.h
#ifdef HOST
	data_host_init();  // luteijn: setup the globals originally defined in data_both.h
#endif
#ifdef CLIENT
	data_client_init(); // luteijn: setup the globals originally defined in data_client.h
#endif

	function_both_init(); // luteijn: setup the globals originally defined in function_both.h
#ifdef HOST
	function_host_init(); // luteijn: setup the globals originally defined in function_host.h
#endif
#ifdef CLIENT
	function_client_init(); // luteijn: setup the globals originally defined in function_client.h
#endif


	log2file =open2("log.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
	SCRLOG_FILEONLY=TRUE;
    
	//check if host command line option is defined
	i=-1; cmdline_length: i++; if ((unsigned char*)lpCmdLine[i]) goto cmdline_length;
	if (i){ //not really a good way to find multiple parameters, but I'm lazy so going to use it for now.
		txtNEWLEN(t,-i); memcpy(t->d2,lpCmdLine,i);
		txtlcase(t); txtset(t2,"host"); if (txtsearch(t,t2)){
			NEThost=1;
		}
#ifdef CLIENT
        txtset(t2,"-l"); if (txtsearch(t,t2)){
          leak=1;
        }
#endif
	}

#ifndef CLIENT
	NEThost=1;//"host" param is assumed
#endif

#ifndef HOST
	if (NEThost){
		MessageBox(NULL,"HOST command line parameter invalid in CLIENT ONLY build","Ultima 6 Online",MB_OK);
		exit(0);

	}
#endif

	//SHARED SETUP
	MSG msg; HACCEL hAccelTable;
	MyRegisterClass(hInstance);

	//2007fix hAccelTable=LoadAccelerators(hInstance,(LPCTSTR)IDC_ULTIMATE);
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_U6O7));


	//basic setup ONLY initializes static arrays or loads info from files
#include "setup_both_basic.h"

#ifdef HOST
	if (NEThost){
#include "setup_host.inc"
	}
#endif
#ifdef CLIENT
#include "setup_client.inc"
#endif
#include "setup_both.inc"

#ifdef CONSOLE
    _cprintf("This is a dedicated u6o host running on port %d.\nTo quit type q or ctrl-c to force quit.\n", global_TCP_listen_port);
#endif
	SCRLOG_FILEONLY=TRUE;
	//begin main loop _________________________________________________________________
	oldtime=timeGetTime();
mainloop:
delay_overprocess://check for messages again
	if (PeekMessage(&msg,NULL, NULL, 0, PM_NOREMOVE)){
		if (!GetMessage(&msg,NULL,0,0)){

#ifdef CLIENT
			if (fonts_added){
				DeleteObject(fnt1);
				DeleteObject(fnt1naa);
				DeleteObject(fnt2);
				DeleteObject(fnt3);
				DeleteObject(fnt4);
				DeleteObject(fnt5);
				DeleteObject(fnt6);
				DeleteObject(fnt7);
				DeleteObject(systemfont);
				RemoveFontResource(".\\dr\\u6o.ttf"); SendMessage( HWND_BROADCAST, WM_FONTCHANGE, 0, 0 );
				RemoveFontResource(".\\dr\\gargish.ttf"); SendMessage( HWND_BROADCAST, WM_FONTCHANGE, 0, 0 );
				RemoveFontResource(".\\dr\\runes.ttf"); SendMessage( HWND_BROADCAST, WM_FONTCHANGE, 0, 0 );
				RemoveFontResource(".\\dr\\u6o2.ttf"); SendMessage( HWND_BROADCAST, WM_FONTCHANGE, 0, 0 );
			}
#endif

#ifdef CLIENT
			if (clientsettingsvalid){
				cltset.u6omidivolume=u6omidivolume;
				cltset.u6ovolume=u6ovolume;
				cltset.u6ovoicevolume=u6ovoicevolume;
				memcpy(&cltset.spellrecall_partymember,&spellrecall_partymember,8);
				memcpy(&cltset.spellrecall_i,&spellrecall_i,8);
				tfh=open2(".\\dr\\settings.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
				put(tfh,&cltset,sizeof(client_settings));
				close(tfh);
			}
#endif

#ifdef HOST
			if (NEThost){
				closesocket(u6osocket);
				for (i=1;i<=socketclientlast;i++){ if (socketclient[i]!=INVALID_SOCKET){
					shutdown(socketclient[i],SD_RECEIVE|SD_SEND); SleepEx(2048,NULL); closesocket(socketclient[i]);
				}}
			}
#endif

#ifdef CLIENT
			shutdown(socketclient[0],SD_RECEIVE|SD_SEND); SleepEx(2048,NULL); closesocket(socketclient[0]);
			WSACleanup();
			if (midiout_setup) midiOutClose(midiout_handle);
#endif

			ExitProcess(0);
		}

		if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ){
			TranslateMessage( &msg ); 
			DispatchMessage( &msg );
		}

		goto mainloop; //process next message (if available)
	}
	if (endprogram==TRUE) goto mainloop;

#ifdef CLIENT
	if (U6O_DISABLEJOYSTICK==FALSE &&!JDISABLED){ //should do only once? and after that skip this
        JDISABLED=1;
		ZeroMemory(&joy,sizeof(JOYINFOEX));
		joy.dwSize=sizeof(JOYINFOEX);
		joy.dwFlags=JOY_RETURNALL;
		joyGetPosEx(JOYSTICKID1,&joy); //joystick information
		//assume ok

		//set POV keys
		i=joy.dwPOV;
		if ((i<=6750)||((i>=29250)&&(i<=35999))){//up
			keyon[256+32]=TRUE;
			if (key_gotrelease[256+32]){
				key[256+32]=TRUE; //user must release
				key_gotrelease[256+32]=FALSE;
			}
		}else{
			keyon[256+32]=FALSE;
			key_gotrelease[256+32]=TRUE;
		}//up
		if ((i>=2250)&&(i<=15750)){//right
			keyon[256+33]=TRUE;
			if (key_gotrelease[256+33]){
				key[256+33]=TRUE; //user must release
				key_gotrelease[256+33]=FALSE;
			}
		}else{
			keyon[256+33]=FALSE;
			key_gotrelease[256+33]=TRUE;
		}//right
		if ((i>=11250)&&(i<=24750)){//down
			keyon[256+34]=TRUE;
			if (key_gotrelease[256+34]){
				key[256+34]=TRUE; //user must release
				key_gotrelease[256+34]=FALSE;
			}
		}else{
			keyon[256+34]=FALSE;
			key_gotrelease[256+34]=TRUE;
		}//down
		if ((i>=20250)&&(i<=33750)){//left
			keyon[256+35]=TRUE;
			if (key_gotrelease[256+35]){
				key[256+35]=TRUE; //user must release
				key_gotrelease[256+35]=FALSE;
			}
		}else{
			keyon[256+35]=FALSE;
			key_gotrelease[256+35]=TRUE;
		}//left


		i2=1;
		for (i=0;i<=31;i++){
			if (joy.dwButtons&i2){
				//button pressed
				keyon[256+i]=TRUE;
				if (key_gotrelease[256+i]){
					key[256+i]=TRUE; //user must release
					key_gotrelease[256+i]=FALSE;
				}

			}else{
				//button released
				keyon[256+i]=FALSE;
				key_gotrelease[256+i]=TRUE;

			}
			i2<<=1;
		}

	}//U6O_DISABLEJOYSTICK
#endif

	f=((float)(timeGetTime()-oldtime))/1000.0f;

	f2=1.0f/16.0f;//client framerate, maybe I should change this to 32 frames and rewrite the effect system.
    if (NEThost) f2=1.0f/64.0f;//host framerate Kaldosh said that it should be running at 64 fps, so I set tit to 64, but It might cause some unwanted side effects, but shouldn't
	if (f<f2){
		f2-=f;
		//f2=seconds we need to wait
		f2*=1000.0f;
		//f2=milliseconds we need to wait
		if (f2>15.0f) f2=15.0f;
		f2+=1.0f;
		SleepEx(f2,FALSE);//CRITICAL: give processing time to other threads
		goto delay_overprocess; //delay overprocessing: 16 frame limit
	}
	newtime=timeGetTime();
	et=((float)(newtime-oldtime))/1000.0f;

	ett+=et;
	oldtime=newtime;
	SleepEx(0,FALSE);//CRITICAL: give EXTRA processing time to other threads

#ifdef HOST
	if (NEThost){
#include "loop_host.cpp"
	}
#endif
  
	if (NEThost){
		static unsigned long client_frameskip=0;
		client_frameskip++; if (client_frameskip==4) client_frameskip=0;
		if (client_frameskip!=0) goto mainloop;
	}
#ifdef CLIENT
#include "loop_client.cpp"
#else
    if (exitrequest && !inbritannia_totalplayers) { /* make sure the games are saved */
      endprogram=TRUE; //useless ?
      PostQuitMessage(0);
    }
#endif
#ifdef CONSOLE /* check if the user is requesting quit */
    GetNumberOfConsoleInputEvents(hStdin,&cNumRead);
    if (cNumRead) { /* read input only if there is something to read */
      if (ReadConsoleInput(hStdin,irInBuf,1,&cNumRead) ) {
        if (irInBuf[0].EventType == KEY_EVENT){
          KeyEventProc(irInBuf[0].Event.KeyEvent); 
        }
      }
      FlushConsoleInputBuffer(hStdin);
    }
#endif
	goto mainloop;
	return 0;//This line exists purely to keep the compiler happy!
}

WNDCLASSEX wcex;
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style		= NULL; //CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	//wcex.hIcon		= LoadIcon(hInstance, (LPCTSTR)IDI_ULTIMATE);
	wcex.hIcon		= LoadIcon(hInstance, (LPCTSTR)IDI_SMALL);


	wcex.hCursor		= NULL; //LoadCursor(NULL, IDC_ARROW); //we use custom cursor
	wcex.hbrBackground	= NULL; //(HBRUSH)(COLOR_WINDOW+1); //we use a custom refresh
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(hInstance, (LPCTSTR)IDI_SMALL);
	return RegisterClassEx( &wcex );
}

#ifdef CLIENT
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	static RECT clrect;
	hInst = hInstance;

	if ((desktop_rect.right>1024)&&(desktop_rect.bottom>768)){
		//create 1024x768 window with title bar
		// rrr can't change this; it will be broken
//		clrect.top=0; clrect.left=0; clrect.bottom=768; clrect.right=1024;
		clrect.top = 0; clrect.left = 0; clrect.bottom = resyo; clrect.right = resxo;
		AdjustWindowRect(&clrect,WS_OVERLAPPED|WS_CAPTION|WS_BORDER,FALSE);
		hWnd2 = CreateWindow(szWindowClass,window_name,WS_OVERLAPPED|WS_CAPTION|WS_BORDER,
			0, 0, clrect.right-clrect.left,clrect.bottom-clrect.top, NULL, NULL, hInstance, NULL);
	}else{

		//create a 1024x768 window (not movable, full screen window)
		hWnd2 = CreateWindow(szWindowClass,window_name,WS_POPUP,
			0, 0, 1024, 768, NULL, NULL, hInstance, NULL);
	}

	clrect.top=0; clrect.left=0; clrect.bottom= resys; clrect.right= resxs;
	AdjustWindowRect(&clrect,WS_OVERLAPPED|WS_CAPTION|WS_BORDER,FALSE);

	hWnd3 = CreateWindow(szWindowClass,window_name,WS_OVERLAPPED|WS_CAPTION|WS_BORDER,
		0, 0, clrect.right-clrect.left,clrect.bottom-clrect.top, NULL, NULL, hInstance, NULL);

	// rrr moved to newmodeinit
	/*
	static RECT clrect;
	clrect.top = 0; clrect.left = 0; clrect.bottom = resyn1w; clrect.right = resxn1w;
	AdjustWindowRect(&clrect, WS_OVERLAPPED | WS_CAPTION | WS_BORDER, FALSE);

	hWnd4 = CreateWindow(szWindowClass, window_name, WS_OVERLAPPED | WS_CAPTION | WS_BORDER,
		0, 0, clrect.right - clrect.left, clrect.bottom - clrect.top, NULL, NULL, hInstance, NULL);
	*/

	hWnd=hWnd2;
	ShowWindow(hWnd,nCmdShow);

	UpdateWindow(hWnd);


	return TRUE;
}
#endif
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	//int wmId, wmEvent;
	switch( message ) 
	{

	case WM_KILLFOCUS:
		break;
	case WM_SETFOCUS:
		break;

	case WM_KEYDOWN:
syskeydown:

#ifdef CLIENT
		if (musickeyboard_set!=255){
			midikeyboard2[wParam&0xFF]=musickeyboard_set;
			static file *tfh;
			tfh=open2(".\\dr\\midikeyb.bin",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE); put(tfh,&midikeyboard2,256); close(tfh);
			static txt *t=txtnew(),*t2=txtnew(); 
			txtset(t,"Computer key("); txtnumint(t2,wParam&0xFF); txtadd(t,t2);
			txtadd(t,") assigned to music keyboard note("); txtnumint(t2,musickeyboard_set-24); txtadd(t,t2); txtadd(t,")");
			STATUSMESSadd(t);
			musickeyboard_set=255; goto musickeyboard_done;
		}
		if ((wParam&0xFF)==VK_ESCAPE){
			if (playinstrument){
				if (clientinstrument==0) STATUSMESSadd("You finish playing the lute.");
				if (clientinstrument==1) STATUSMESSadd("You finish playing the harpsichord.");
				if (clientinstrument==2) STATUSMESSadd("You finish playing the harp.");
				if (clientinstrument==3) STATUSMESSadd("You finish playing the panpipes.");
				if (clientinstrument==4) STATUSMESSadd("You finish playing the xylophone.");
				playinstrument=0;
				if (musickeyboard->offset_x<1024) musickeyboard->offset_x+=2048;
				goto musickeyboard_done;
			}
		}
#endif

		keyon[wParam&0xFF]=TRUE;
		if (key_gotrelease[wParam&0xFF]){
			key[wParam&0xFF]=TRUE; //user must release
			key_gotrelease[wParam&0xFF]=FALSE;
		}

#ifdef CLIENT
		if (playinstrument==0) GETINPUT_update();
musickeyboard_done:
#endif

		break;

	case WM_KEYUP:
syskeyup:
		keyon[wParam&0xFF]=FALSE;
		key_gotrelease[wParam&0xFF]=TRUE;
		break;

		//F10 FIX
	case WM_SYSKEYDOWN:
		if ((wParam&0xFF)==VK_F10) goto syskeydown;
		break;
	case WM_SYSKEYUP:
		if ((wParam&0xFF)==VK_F10) goto syskeyup;
		break;

	case WM_MOUSEMOVE:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);
		break;

	case WM_CHAR:
		keyasc[wParam&0xFF]=TRUE; //user must release
#ifdef CLIENT
		GETINPUT_update();
#endif
		break;

	case WM_LBUTTONDOWN:
		mbclick|=1;
		mbheld|=1;
		mb=mb|1;
		break;
	case WM_LBUTTONUP:
		if (mbheld&1) mbheld-=1;
		mb_release|=1;
		break;

	case WM_RBUTTONDOWN:
		mbclick|=2;
		mbheld|=2;
		mb=mb|2;
		break;
	case WM_RBUTTONUP:
		if (mbheld&2) mbheld-=2;
		mb_release|=2;
		break;

	case WM_MBUTTONDOWN:
		keyon[0xDA]=TRUE;
		if (key_gotrelease[0xDA]){
			key[0xDA]=TRUE; //user must release
			key_gotrelease[0xDA]=FALSE;
		}
		break;
	case WM_MBUTTONUP:
		keyon[0xDA]=FALSE;
		key_gotrelease[0xDA]=TRUE;
		break;

	case WM_XBUTTONDOWN:
		if (HIWORD(wParam)&XBUTTON1){
			keyon[0xDB]=TRUE;
			if (key_gotrelease[0xDB]){
				key[0xDB]=TRUE; //user must release
				key_gotrelease[0xDB]=FALSE;
			}
		}//xbutton1
		if (HIWORD(wParam)&XBUTTON2){
			keyon[0xDC]=TRUE;
			if (key_gotrelease[0xDC]){
				key[0xDC]=TRUE; //user must release
				key_gotrelease[0xDC]=FALSE;
			}
		}//xbutton2
		return TRUE;
		break;

	case WM_XBUTTONUP:
		if (HIWORD(wParam)&XBUTTON1){
			keyon[0xDB]=FALSE;
			key_gotrelease[0xDB]=TRUE;
		}//xbutton1
		if (HIWORD(wParam)&XBUTTON2){
			keyon[0xDC]=FALSE;
			key_gotrelease[0xDC]=TRUE;
		}//xbutton2
		return TRUE;
		break;

	case 0x020A://WM_MOUSEWHEEL
		static short delta,keyi;
		delta=(wParam>>16); delta/=120;
		wheel_move+=delta;
		break;

		/*
		case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch( wmId ) 
		{
		default:
		return DefWindowProc( hWnd, message, wParam, lParam );
		}
		break;
		*/

	case WM_DESTROY:
		endprogram=TRUE;
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}
