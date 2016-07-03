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
#define width (320.0)
#define height (240.0)

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
	
	image.Create(width,height,32,CImage::createAlphaChannel);

	{
		u8 r,g,b,a;		
		pi =  image.GetPitch();
		dpo = (u8*)image.GetBits();
		u8 *dp = dpo;
	
		for(u16 y=0;y<height;y++)		
		{
			for(u16 x=0;x<width;x++)
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

			//drawset();
			sx = -2, ex = 1; sy = -2, ey = 2;
			drawset2(sx,ex,sy,ey);

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      500, 900, 320+40, 240+40, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   gWnd = hWnd;

   return TRUE;
}


u16 fcx,fcy;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		image.Draw(hdc,0,0,width,height,0,0,width,height);
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
			dx=x/width*(ex-sx)+sx;
			dy=y/height*(ey-sy)+sy;

			sx+=dx;ex+=dx;
			sy+=dy;ey+=dy;

#define divfac 2
			sx/=divfac;ex/=divfac;
			sy/=divfac;ey/=divfac;

			drawset2(sx,ex,sy,ey);
			InvalidateRect(hWnd,0,true);

		}
		break;
	case WM_RBUTTONDOWN:
		{
			int x,y;
			x = LOWORD (lParam) ;
			y = HIWORD (lParam) ;

			//u16 xp,yp;
			//db dx,dy;
			//// double the range
			//
			//dx=x/width*(ex-sx)+sx;
			//dy=y/height*(ey-sy)+sy;

			//sx-=dx;ex-=dx;
			//sy-=dy;ey-=dy;

			sx*=2.0;ex*=2.0;
			sy*=2.0;ey*=2.0;
			

			drawset2(sx,ex,sy,ey);
			InvalidateRect(hWnd,0,true);
//			tgo[0]=1;
		}
		break;
	case WM_MBUTTONDOWN:
		{
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
			  dx=(fcx-x)/width*(ex-sx);
			  dy=(fcy-y)/height*(ey-sy);

			 sx+=dx;ex+=dx;
			sy+=dy;ey+=dy;
			drawset2(sx,ex,sy,ey);
			InvalidateRect(hWnd,0,true);

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


void drawset2(db sx,db ex, db sy, db ey)
{
	u32 c;u8 cc;
	double cx,cy;
	u8 val;

	for(u16 y=0;y<height;y++)
	{
		cy = y/height*(ey-sy)+sy;
		for(u16 x=0;x<width;x++)
		{			
			cx = x/width*(ex-sx)+sx;
			
			val = checkpoint(cx,cy);
			
			//cc = (1.0-val/limit)*0xff;
			cc = val;
			
			c = 0xff + (cc<<8)+(cc<<16)+(cc<<24);
			
			setpixel(x,y,c);
		}
	}
}
