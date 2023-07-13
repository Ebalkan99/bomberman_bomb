#include <windows.h>  
#include <gdiplus.h>
#include <string>
#include "chmat.h"
#include "ImageLoad.h"
//#include "ImageLoader.h"

using namespace Gdiplus;
using namespace std;
#define IDI_ICON1 101
#define IDC_OPEN	3000
#define IDC_BUTTON1 3001
#define IDC_BUTTON2 3002
#define IDC_BUTTON3 3003


HINSTANCE hInst;   // current instance
HWND     hWnd,HButton1, HButton2, HButton3, Hmainbmp;    //parent window

LPCTSTR lpszAppName = "GEN32";
LPCTSTR lpszTitle = "GENERIC32";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
ULONG_PTR           gdiplusToken;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	MSG      msg;
	WNDCLASSEX wc;
	HMENU MainMenu, FileMenu;
	MainMenu = CreateMenu();
	FileMenu = CreatePopupMenu();
	AppendMenu(FileMenu, MF_STRING, IDC_OPEN, "Open");
	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FileMenu, "FILE");

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = lpszAppName;
	wc.lpszClassName = lpszAppName;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImage(hInstance, lpszAppName,
		IMAGE_ICON, 16, 16,
		LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc))
		return(FALSE);

	hInst = hInstance;
	hWnd = CreateWindowEx(0, lpszAppName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		760, 740,
		NULL,
		MainMenu,
		hInstance,
		NULL
	);
	

	if (!hWnd)
		return(FALSE);
	GdiplusStartupInput gdiplusStartupInput;


	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(msg.wParam);
}


/////METHODS////////
void Drawer(CHMAT& matrix, int x, int y, int offset_x, int offset_y, bool Background);
void Eraser(CHMAT& matrix, int x, int y, int offset_x, int offset_y);
///////MATRİCES//////////////
CHMAT Map(1,1), CopyMap(1, 1);
CHMAT bomb1(1, 1), bomb2(1, 1), bomb3(1, 1);
//CHMAT green_copy(1, 1);
CHMAT bomb_effect1_center(1, 1), bomb_effect2_center(1, 1), bomb_effect3_center(1, 1), bomb_effect4_center(1, 1);
CHMAT down_tip1(1, 1), down_tip2(1, 1), down_tip3(1, 1), down_tip4(1, 1);
CHMAT up_tip1(1, 1), up_tip2(1, 1), up_tip3(1, 1), up_tip4(1, 1);
CHMAT left_tip1(1, 1), left_tip2(1, 1), left_tip3(1, 1), left_tip4(1, 1);
CHMAT right_tip1(1, 1), right_tip2(1, 1), right_tip3(1, 1), right_tip4(1, 1);
CHMAT brick_tip1(1, 1), brick_tip2(1, 1), brick_tip3(1, 1), brick_tip4(1, 1), brick_tip5(1, 1), brick_tip6(1, 1);

/////GLOBAL VARIABLES////////
int keypressed;
int green = Map(64, 67);
int black = Map(63, 91);
int white = Map(110, 91);
CHMAT bmb[3] = { bomb1, bomb2, bomb3 };
int bomb_x = 496, bomb_y = 136;
///////THREADS///////////////
void* UpperEffect(void* vv) {

	CHMAT bmb_effect_upper[4] = { up_tip1, up_tip2, up_tip3, up_tip4};
	for (int i = 0; i < 4; i++) {
		Drawer(bmb_effect_upper[i], 45, 45, bomb_x, bomb_y+45, false);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);
		Eraser(bmb_effect_upper[i], 45, 45, bomb_x, bomb_y+45);
		
	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);

	return 0;
}
void* RightEffect(void* vv) {
	CHMAT bmb_effect_right[4] = { right_tip1, right_tip2, right_tip3, right_tip4 };
	for (int i = 0; i < 4; i++) {
		Drawer(bmb_effect_right[i], 45, 45, bomb_x+45, bomb_y, false);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);
		Eraser(bmb_effect_right[i], 45, 45, bomb_x+45, bomb_y);
		
	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
	return 0;
}
void* LeftEffect(void* vv) {
	CHMAT bmb_effect_left[4] = { left_tip1, left_tip2, left_tip3, left_tip4 };
	for (int i = 0; i < 4; i++) {
		Drawer(bmb_effect_left[i], 45, 45, bomb_x-45, bomb_y, false);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);
		Eraser(bmb_effect_left[i], 45, 45, bomb_x-45, bomb_y);
		
	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
	return 0;
}

void* DownEffect(void* vv) {
	CHMAT bmb_effect_down[4] = { down_tip1,down_tip2,down_tip3,down_tip4 };
	for (int i = 0; i < 4; i++) {
		Drawer(bmb_effect_down[i], 45, 45, bomb_x, bomb_y-45, false);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);
		Eraser(bmb_effect_down[i], 45, 45, bomb_x, bomb_y-45);

	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
	return 0;
}
void* BrickUp(void* vv){
	CHMAT bmb_effect_brick[6] = { brick_tip1, brick_tip2, brick_tip3, brick_tip4, brick_tip5, brick_tip6 };
	
	for (int i = 0; i < 6; i++) {
		Drawer(bmb_effect_brick[i], 45, 45, bomb_x, bomb_y+45, true);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);
		
	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
	return 0;
}
void* BrickRight(void* vv) {
	CHMAT bmb_effect_brick[6] = { brick_tip1, brick_tip2, brick_tip3, brick_tip4, brick_tip5, brick_tip6 };
	for (int i = 0; i < 6; i++) {
		Drawer(bmb_effect_brick[i], 45, 45, bomb_x+45, bomb_y, true);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);
	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);

	return 0;
}
void* BrickLeft(void* vv) {
	CHMAT bmb_effect_brick[6] = { brick_tip1, brick_tip2, brick_tip3, brick_tip4, brick_tip5, brick_tip6 };
	for (int i = 0; i < 6; i++) {
		Drawer(bmb_effect_brick[i], 45, 45, bomb_x - 45, bomb_y, true);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);	
	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);

	return 0;
}
void* BrickDown(void* vv) {
	CHMAT bmb_effect_brick[6] = { brick_tip1, brick_tip2, brick_tip3, brick_tip4, brick_tip5, brick_tip6 };
	for (int i = 0; i < 6; i++) {
		Drawer(bmb_effect_brick[i], 45, 45, bomb_x, bomb_y-45, true);
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		Sleep(100);
	}
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);

	return 0;
}

void* Bomb(void* vv) {
	
	int green = Map(64, 67);
	int black = Map(65, 92);
	int white = Map(110, 91);
	DWORD dw;
	CHMAT bmb[3] = { bomb1, bomb2, bomb3 };
	CHMAT bmb_effect_center[4] = {bomb_effect1_center, bomb_effect2_center, bomb_effect3_center, bomb_effect4_center};

	//büyüyor
	//while (1) {
		for (int i = 0; i < 3; i++) {
			Drawer(bmb[i], 45, 45, bomb_x, bomb_y, false);
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
			Sleep(500);
			Eraser(bmb[i], 45, 45, bomb_x, bomb_y);
		}
		//bomba küçülürken
		for (int i = 1; i > -1; i--) {
			Drawer(bmb[i], 45, 45, bomb_x, bomb_y, false);
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
			Sleep(500);
			Eraser(bmb[i], 45, 45, bomb_x, bomb_y);

		}
		//büyüyor
		for (int i = 0; i < 3; i++) {
			Drawer(bmb[i], 45, 45, bomb_x, bomb_y, false);
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
			Sleep(500);
			Eraser(bmb[i], 45, 45, bomb_x, bomb_y);//0x00000

		}
		if (Map(bomb_x, bomb_y+45) == green) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpperEffect, NULL, 0, &dw);
		}
		if (Map(bomb_x, bomb_y + 45) == black) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BrickUp, NULL, 0, &dw);
		}
		if(Map(bomb_x , bomb_y + 45) == white) {
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		}
		
		if (Map(bomb_x + 45, bomb_y ) == green) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RightEffect, NULL, 0, &dw);
		}
		if (Map(bomb_x + 45, bomb_y ) == black) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BrickRight, NULL, 0, &dw);
		}
		if (Map(bomb_x + 45, bomb_y ) == white) {
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		}
		
		if (Map(bomb_x - 45, bomb_y ) == green) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LeftEffect, NULL, 0, &dw);
		}
		if (Map(bomb_x -45, bomb_y) == black) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BrickLeft, NULL, 0, &dw);
		}
		if (Map(bomb_x - 45, bomb_y ) == white) {
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		}
		if (Map(bomb_x , bomb_y - 45) == green) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DownEffect, NULL, 0, &dw);
		}
		if (Map(bomb_x , bomb_y - 45) == black) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BrickDown, NULL, 0, &dw);
		}
	    if (Map(bomb_x , bomb_y -45) == white) {
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
		}
		
		
		//patlıyor
		for (int i = 0; i < 4; i++) {
			Drawer(bmb_effect_center[i], 45, 45, bomb_x, bomb_y, false);
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
			Sleep(100);
			Eraser(bmb[i], 45, 45, bomb_x, bomb_y);

		}
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);


	//}
	


	return 0;
}

void Drawer(CHMAT& matrix, int x_length_of_pic, int y_length_of_pic, int offset_x, int offset_y, bool Background) {
	if (Background) {
		for (int i = 0; i < x_length_of_pic; i++) {
			for (int j = 0; j < y_length_of_pic; j++) {
				Map(offset_x + i, offset_y + j) = matrix(i + 1, j + 1);
			}
		}
	}
	else if (!Background) {
		int p = matrix(1, 1);
		for (int i = 0; i < x_length_of_pic; i++) {
			for (int j = 0; j < y_length_of_pic; j++) {
				if (matrix(i, j) != p) {
					Map(offset_x + i, offset_y + j) = matrix(i + 1, j + 1);
				}
			}
		}
	}
}
//SULEYMAN
void Eraser(CHMAT& matrix, int x_length_of_pic, int y_length_of_pic, int offset_x, int offset_y) {
	for (int i = 0; i < x_length_of_pic; i++) {
		for (int j = 0; j < y_length_of_pic; j++) {
			Map(offset_x + i, offset_y + j) = CopyMap(offset_x + i, offset_y + j);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		
	case WM_CREATE:
	{
		
		HButton2 = CreateWindowEx(NULL, "BUTTON", "START", WS_CHILD | WS_VISIBLE | SS_CENTER,
			82, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON2, hInst, NULL);
		Hmainbmp = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 23, 600, 500, hWnd, NULL, hInst, NULL);
	}
	break;
	case WM_KEYDOWN:
	{
		static int t, parameter = 5;
		static DWORD dw;
		t = (int)wParam;
		if (t == 32) 
		if (t > 36 && t < 41) keypressed = t;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OPEN:
		{
			
				OPENFILENAME ofn;

				char szFile[260];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				GetOpenFileName(&ofn);

				LoadImage(szFile, Map);

			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);
	
		}
		break;
		case IDC_BUTTON2:
		{

			DWORD dw;
			ImageLoad loader;
			loader.load(Map, "..\\Images\\Map.png");
			loader.load(CopyMap, "..\\Images\\Map.png");
			loader.load(bomb1, "..\\Images\\Bomb_1.png");
			loader.load(bomb2, "..\\Images\\Bomb_2.png");
			loader.load(bomb3, "..\\Images\\Bomb_3.png");
			loader.load(bomb_effect1_center, "..\\Images\\Bomb Effects\\Effect_1\\Center.png");
			loader.load(bomb_effect2_center, "..\\Images\\Bomb Effects\\Effect_2\\Center.png");
			loader.load(bomb_effect3_center, "..\\Images\\Bomb Effects\\Effect_3\\Center.png");
			loader.load(bomb_effect4_center, "..\\Images\\Bomb Effects\\Effect_4\\Center.png");
			loader.load(up_tip1, "..\\Images\\Bomb Effects\\Effect_1\\Up_Tip.png");
			loader.load(up_tip2, "..\\Images\\Bomb Effects\\Effect_2\\Up_Tip.png");
			loader.load(up_tip3, "..\\Images\\Bomb Effects\\Effect_3\\Up_Tip.png");
			loader.load(right_tip1, "..\\Images\\Bomb Effects\\Effect_1\\Right_Tip.png");
			loader.load(right_tip2, "..\\Images\\Bomb Effects\\Effect_2\\Right_Tip.png");
			loader.load(right_tip3, "..\\Images\\Bomb Effects\\Effect_3\\Right_Tip.png");
			loader.load(right_tip4, "..\\Images\\Bomb Effects\\Effect_4\\Right_Tip.png");
			loader.load(left_tip1, "..\\Images\\Bomb Effects\\Effect_1\\Left_Tip.png");
			loader.load(left_tip2, "..\\Images\\Bomb Effects\\Effect_2\\Left_Tip.png");
			loader.load(left_tip3, "..\\Images\\Bomb Effects\\Effect_3\\Left_Tip.png");
			loader.load(left_tip4, "..\\Images\\Bomb Effects\\Effect_4\\Left_Tip.png");
			loader.load(down_tip1, "..\\Images\\Bomb Effects\\Effect_1\\Down_Tip.png");
			loader.load(down_tip2, "..\\Images\\Bomb Effects\\Effect_2\\Down_Tip.png");
			loader.load(down_tip3, "..\\Images\\Bomb Effects\\Effect_3\\Down_Tip.png");
			loader.load(down_tip4, "..\\Images\\Bomb Effects\\Effect_4\\Down_Tip.png");
			loader.load(brick_tip1, "..\\Images\\Brick_Effects\\brick_disapp_1.png");
			loader.load(brick_tip2, "..\\Images\\Brick_Effects\\brick_disapp_2.png");
			loader.load(brick_tip3, "..\\Images\\Brick_Effects\\brick_disapp_3.png");
			loader.load(brick_tip4, "..\\Images\\Brick_Effects\\brick_disapp_4.png");
			loader.load(brick_tip5, "..\\Images\\Brick_Effects\\brick_disapp_5.png");
			loader.load(brick_tip6, "..\\Images\\Brick_Effects\\brick_disapp_6.png");
			//loader.load(green_copy, "..\\Images\\green.jpeg");


			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)Map.HBitmap);

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Bomb, NULL, 0, &dw);
			
			SetFocus(hWnd);
		}
		break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		GdiplusShutdown(gdiplusToken);
		break;

	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	return(0L);
}

