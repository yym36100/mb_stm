// mandelbrot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "mandelbrot.h"

#include <windows.h>

#include <atlimage.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef double db;

#define MAX_LOADSTRING 100

void drawset();
void drawset2(db sx,db ex, db sy, db ey);
double sx,ex, sy,ey;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
CImage image;	

int pi ;
u8 *dpo ;
void setpixel(u16 x, u16 y,u32 color);

 
HANDLE threadhandles[4]={0};
DWORD WINAPI Thread_no_0( LPVOID lpParam );

DWORD WINAPI Thread_no_1( LPVOID lpParam );
DWORD WINAPI Thread_no_2( LPVOID lpParam );
DWORD WINAPI Thread_no_3( LPVOID lpParam );
DWORD WINAPI Thread_no_4( LPVOID lpParam );

volatile int tgo[5];
HWND gWnd;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	image.Create(400,400,32,CImage::createAlphaChannel);

	{
		u8 r,g,b,a;		
		pi =  image.GetPitch();
		dpo = (u8*)image.GetBits();
		u8 *dp = dpo;
	
		for(u16 y=0;y<400;y++)		
		{
			for(u16 x=0;x<400;x++)
			{
				a = 0xff;
				b = 0x00;
				g = 0x00;
				r = 0x00;

				*dp++ = b;
				*dp++ = g;
				*dp++ = r;
				*dp++ = 0xff;				
			}
			dp+=2*pi;
		}
	}

threadhandles[0] = CreateThread( NULL, 0, 
           Thread_no_0, NULL, 0, NULL);  

	threadhandles[1] = CreateThread( NULL, 0, 
           Thread_no_1, NULL, 0, NULL);  
	threadhandles[2] = CreateThread( NULL, 0, 
           Thread_no_2, NULL, 0, NULL);  
	threadhandles[3] = CreateThread( NULL, 0, 
           Thread_no_3, NULL, 0, NULL);  
	threadhandles[4] = CreateThread( NULL, 0, 
           Thread_no_4, NULL, 0, NULL);  

			//setpixel(200,200,0xffffffff);
			//setpixel(100,100,0xffff);
			drawset();
			//void drawset2(db sx,db ex, db sy, db ey)
			//double sx,ex, sy,ey;
			sx = -2, ex = 2; sy = -2, ey = 2;
//			drawset2(-2,2,-2,2);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MANDELBROT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MANDELBROT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}

void setpixel(u16 x, u16 y,u32 color)
{
	u8 *dp = dpo + y*pi + x*4;
	*dp++ = (color>>24)&0xff;
	*dp++ = (color>>16)&0xff;
	*dp++ = (color>>8)&0xff;				
	*dp++ = color&0xff;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MANDELBROT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_MANDELBROT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      500, 900, 500, 500, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   gWnd = hWnd;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
u16 fcx,fcy;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		image.Draw(hdc,0,0,400,400,0,0,400,400);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		{
			int x,y;
			x = LOWORD (lParam) ;
			y = HIWORD (lParam) ;

			u16 xp,yp;
			db dx,dy;
			// double the range
			dx=x/400.0*(ex-sx)+sx;
			dy=y/400.0*(ey-sy)+sy;

			sx+=dx;ex+=dx;
			sy+=dy;ey+=dy;

#define divfac 2
			sx/=divfac;ex/=divfac;
			sy/=divfac;ey/=divfac;

			


//			drawset2(sx,ex,sy,ey);
			//InvalidateRect(hWnd,0,true);
			tgo[0]=1;
		}
		break;
	case WM_RBUTTONDOWN:
		{
			int x,y;
			x = LOWORD (lParam) ;
			y = HIWORD (lParam) ;

			u16 xp,yp;
			db dx,dy;
			// double the range
			
			dx=x/400.0*(ex-sx)+sx;
			dy=y/400.0*(ey-sy)+sy;

			sx-=dx;ex-=dx;
			sy-=dy;ey-=dy;

			sx*=2.0;ex*=2.0;
			sy*=2.0;ey*=2.0;
			

//			drawset2(sx,ex,sy,ey);
			//InvalidateRect(hWnd,0,true);
			tgo[0]=1;
		}
		break;
	case WM_MBUTTONDOWN:
		{
		/*	sx=-2;ex=+2;
			sy=-2;ey=+2;
			drawset2(sx,ex,sy,ey);
			InvalidateRect(hWnd,0,true);*/
			fcx = LOWORD (lParam) ;
			fcy = HIWORD (lParam) ;
		}
		break;
	case WM_MOUSEMOVE:
		if(wParam & MK_MBUTTON)
		{
			  u16 x = LOWORD (lParam) ;
              u16 y = HIWORD (lParam) ;
			  db dx, dy;
			  dx=(fcx-x)/400.0*(ex-sx);
			  dy=(fcy-y)/400.0*(ey-sy);

			  sx+=dx;ex+=dx;
			sy+=dy;ey+=dy;
//			drawset2(sx,ex,sy,ey);
			//InvalidateRect(hWnd,0,true);
			tgo[0]=1;

			fcx=x;fcy=y;

		}
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#define limit 250.0
inline u16 checkpoint(double cx, double cy)
{
	u8 res = 0;
	u16 i = 0;
	double x=cx,y=cy,xx,yy,xy;
	do{
		xx= x*x;
		yy = y*y;
		xy = x*y;
		x = xx-yy+cx;
		y = 2*xy+cy;
		i++;
	}while((xx+yy)<4.0 && i<limit);
return i;
}

void drawset()
{
	u32 c;
	u8 cc;
	for(u16 y=0;y<400;y++)
	{
		for(u16 x=0;x<400;x++)
		{
			double cx,cy;
			u8 val;
			cx = x/400.0*4-2;
			cy = y/400.0*4-2;
			val = checkpoint(cx,cy);
		/*	if(val>150)
				c = 0xffffffff;
			else c = 0xff;*/
			
			cc = (1.0-val/200.0)*0xff;
			c = 0xff + (cc<<8)+(cc<<16)+(cc<<24);
			
			setpixel(x,y,c);
		}
	}
}
//24
 static u8 Colors[] = {
      182,182,255, 198,182,255, 218,182,255, 234,182,255, 255,182,255,
      255,182,234, 255,182,218, 255,182,198, 255,182,182, 255,198,182,
      255,218,182, 255,234,182, 255,255,182, 234,255,182, 218,255,182,
      198,255,182, 182,255,182, 182,255,198, 182,255,218, 182,255,234,
      182,255,255, 182,234,255, 182,218,255, 182,198,255};

u8 bitrev(u8 i)
{
	return 
		((i&1)<<7)+
		((i&2)<<6)+
		((i&4)<<5)+
		((i&8)<<4)+
		((i&0x10)<<3)+
		((i&0x20)<<2)+
		((i&0x40)<<1)+
		((i&0x80)<<0);		
}

void drawset2(db sx,db ex, db sy, db ey)
{
	u32 c;u8 cc;
	for(u16 y=0;y<400;y++)
	{
		for(u16 x=0;x<400;x++)
		{
			double cx,cy;
			u8 val;
			cx = x/400.0*(ex-sx)+sx;
			cy = y/400.0*(ey-sy)+sy;
			val = checkpoint(cx,cy);
			/*if(val>50)
				c = 0xffffffff;
			else c = 0xff;*/

			/*cc = (1.0-val/limit)*0xff;
			c = 0xff + (cc<<8)+(cc<<16)+(cc<<24);*/

			/*cc = (1.0-val/limit)*24*3;
			c = 0xff + (Colors[cc+0]<<8)+(Colors[cc+1]<<16)+(Colors[cc+2]<<24);*/

			cc = (1.0-val/limit)*0xff;
			//cc = bitrev(cc);
			c = 0xff + (cc<<8)+(cc<<16)+(cc<<24);
			
			setpixel(x,y,c);
		}
	}
}
void drawset3(db sx,db ex, db sy, db ey,u16 ssx, u16 eex, u16 ssy, u16 eey)
{
	u32 c;u8 cc;
	double cx,cy;
			u8 val;
	for(u16 y=0;y<400;y++)
	{
		for(u16 x=0;x<400;x++)
		{
			cx = x/400.0*(ex-sx)+sx;
			cy = y/400.0*(ey-sy)+sy;
			val = checkpoint(cx,cy);
		
			cc = (1.0-val/limit)*0xff;
			//cc = bitrev(cc);
			c = 0xff + (cc<<8)+(cc<<16)+(cc<<24);
			
			setpixel(x,y,c);
		}
	}
}

DWORD WINAPI Thread_no_0( LPVOID lpParam )
{
	int tp=0;
	while(1)
	{
		Sleep(10);
		if(tgo[0]==1)
		{
			tgo[1]=1;tgo[2]=1;
			tgo[3]=1;tgo[4]=1;
			tgo[0]=0;
			tp=1;
		}
		if(tgo[1]==0 && tgo[2]==0 && tgo[3]==0 && tgo[4]==0 && tp==1)
		{
			InvalidateRect(gWnd,NULL,true);
			tp=0;
		}			
	}
	return 0;
}

DWORD WINAPI Thread_no_1( LPVOID lpParam )
{
	while(1)
	{
		if(tgo[1])
		{
		drawset3(sx,ex,sy,ey,0,200,0,200);
		tgo[1]=0;
		}
		Sleep(10);
	}
	return 0;
}
DWORD WINAPI Thread_no_2( LPVOID lpParam )
{
	while(1)
	{
		if(tgo[2])
		{
		drawset3(sx,ex,sy,ey,200,400,0,200);
		tgo[2]=0;
		}
		Sleep(10);
	}
	return 0;
}

DWORD WINAPI Thread_no_3( LPVOID lpParam )
{
	while(1)
	{
		if(tgo[3])
		{
		drawset3(sx,ex,sy,ey,200,400,0,200);
		tgo[3]=0;
		}
		Sleep(10);
	}
	return 0;
}
DWORD WINAPI Thread_no_4( LPVOID lpParam )
{
	while(1)
	{
		if(tgo[4])
		{
		drawset3(sx,ex,sy,ey,200,400,200,400);
		tgo[4]=0;
		}
		Sleep(10);
	}
	return 0;
}

