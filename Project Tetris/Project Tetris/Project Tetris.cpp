// Project Tetris.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project Tetris.h"


#define MAX_LOADSTRING 100
#define ELEM 30
#define X_SIZE 510
#define Y_SIZE 660
#define MARGIN 1
#define INFO_X 4 
#define INFO_Y 20
#define GAME_X 10 
#define GAME_Y 20
#define MYTIMER 1
#define INTERVAL 500

HDC g_hdc;
HDC g_hdcBuff;
HDC g_hdcBg;
HBITMAP g_hBmp;

INT g_astTetris[][4][4] = 
{
	{{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
	{{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}},
	{{0,1,1,1},{0,0,0,1},{0,0,0,0},{0,0,0,0}},
	{{1,1,1,0},{0,1,0,0},{0,0,0,0},{0,0,0,0}},
	{{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
	{{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}
};
#define CNT sizeof(g_astTetris)/sizeof(g_astTetris[0]);
INT g_astCur[4][4];
INT g_astNext[4][4];
INT g_x, g_y;
INT g_astMap[10][20];
INT g_score;

RECT sc;
INT Score;
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID LoadGame(HWND);
VOID ClearGame();
VOID PaintGame();
VOID InitGame();

BOOL CheckCollison(int, int);
VOID RefreshMap();

VOID Rotate();
BOOL GameOver();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECTTETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTTETRIS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTTETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = 0;//MAKEINTRESOURCEW(IDC_PROJECTTETRIS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, L"Tetris", WS_SYSMENU | WS_MINIMIZEBOX,
      500, 200, X_SIZE+18, Y_SIZE+45, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   LoadGame(hWnd);
   InitGame();
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, MYTIMER,INTERVAL, 0);
		break;
	case WM_TIMER:	
		if(CheckCollison(g_x,g_y+1))
			++g_y;
		else 
		{

			if (GameOver())
			{
				KillTimer(hWnd, MYTIMER);
				MessageBox(NULL, L"Game Over", L"TIPS:", MB_OK);
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}
				
			RefreshMap();
			memcpy(g_astCur, g_astNext, sizeof(g_astCur));
			g_x = MARGIN+GAME_X/2;
			g_y = 0;
			srand(GetTickCount());
			int x = rand() % CNT;
			memcpy(g_astNext, g_astTetris[x], sizeof(g_astNext));
			
		}
		PaintGame();
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (CheckCollison(g_x -1 , g_y))
			{
				--g_x;
			}
			break;
		case VK_RIGHT:
			if (CheckCollison(g_x + 1, g_y))
			{
				++g_x;
			}
			break;
		case VK_UP:
			Rotate();
			break;
		case VK_DOWN:
			while (CheckCollison(g_x, g_y + 1)) 
			{
				++g_y;
			}
			RefreshMap();	
		}

		PaintGame();
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			PaintGame();

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd, MYTIMER);
        PostQuitMessage(0);
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
VOID LoadGame(HWND hWnd) 
{
	g_hdc = GetDC(hWnd);
	g_hdcBuff = CreateCompatibleDC(g_hdc);
	g_hBmp = CreateCompatibleBitmap(g_hdc, X_SIZE, Y_SIZE);
	SelectObject(g_hdcBuff, g_hBmp);

	HBITMAP hBmp = LoadBitmap(hInst,MAKEINTRESOURCE(BG));
	g_hdcBg = CreateCompatibleDC(g_hdc);
	SelectObject(g_hdcBg, hBmp);
	DeleteObject(hBmp);

	return;
}
VOID ClearGame() 
{
	DeleteObject(g_hBmp);
	DeleteObject(g_hdcBuff);
	DeleteObject(g_hdcBg);
	return;
}
VOID InitGame() 
{

	srand(GetTickCount());
	int x = rand() % CNT;
	memcpy(g_astCur, g_astTetris[x], sizeof(g_astCur));

	x = rand() % CNT;
	memcpy(g_astNext, g_astTetris[x], sizeof(g_astNext));

	g_x = MARGIN+GAME_X/2;
	g_y = 0;

	memset(g_astMap, 0, sizeof(g_astMap));
	return;
}
VOID PaintGame() 
{
	TCHAR strScore[100];
	RECT rect;
	BitBlt(g_hdcBuff, 0, 0, X_SIZE, Y_SIZE, g_hdcBg, 0, 0, SRCCOPY);
	HBRUSH hBrush = CreateSolidBrush(RGB(0xbb, 0xff, 0xff));
	
	for (int x = 0;x < X_SIZE / ELEM;++x) 
	{
		for (int y = 0;y < Y_SIZE / ELEM;++y) 
		{
			if (x == 0 || y == 0 || x == X_SIZE / ELEM - 1 || y == Y_SIZE / ELEM - 1 || x == MARGIN+GAME_X) 
			{
				SelectObject(g_hdcBuff, hBrush);
				Rectangle(g_hdcBuff, x*ELEM, y*ELEM, (x + 1)*ELEM, (y + 1)*ELEM);
			}
		}
	}
	DeleteObject(SelectObject(g_hdcBuff, GetStockObject(NULL_BRUSH)));
	

	for (int x = 0;x < GAME_X;++x)
	{
		for (int y = 0;y < GAME_Y;++y)
		{
			if (g_astMap[x][y])
			{
				SelectObject(g_hdcBuff, HBRUSH(GetStockObject(LTGRAY_BRUSH)));
				Rectangle(g_hdcBuff, (MARGIN+x)*ELEM, (MARGIN+y)*ELEM, (MARGIN+x + 1)*ELEM, (MARGIN+ y + 1)*ELEM);
			}
		}
	}
	for (int x = 0;x < 4;++x)
	{
		for (int y = 0;y < 4;++y)
		{
			if (g_astCur[x][y])
			{

				SelectObject(g_hdcBuff, HBRUSH(GetStockObject(LTGRAY_BRUSH)));
				Rectangle(g_hdcBuff, (MARGIN+g_x + x)*ELEM, (MARGIN + g_y + y)*ELEM, (MARGIN+g_x + x + 1)*ELEM, (MARGIN + g_y + y + 1)*ELEM);

			}
		}
	}
	for (int x = 0;x < 4;++x) 
	{
		for (int y = 0;y < 4;++y) 
		{
			if (g_astNext[x][y]) 
			{
				SelectObject(g_hdcBuff, HBRUSH(GetStockObject(LTGRAY_BRUSH)));
				Rectangle(g_hdcBuff, (MARGIN*2 +GAME_X + x+1)*ELEM, (MARGIN *2+ y)*ELEM, (MARGIN*2+GAME_X+ x+2)*ELEM, (MARGIN*2+y+1)*ELEM);
			}
		}
	}
	rect.left = (MARGIN * 2 + GAME_X + 1)*ELEM;
	rect.right = (MARGIN * 2 + GAME_X + 6)*ELEM;
	rect.top = (MARGIN * 2 + 6)*ELEM;
	rect.bottom = (MARGIN * 2 + 8)*ELEM;
	wsprintf(strScore, L"Score: %d", g_score );
	DrawText(g_hdcBuff, strScore, wcslen(strScore), &rect, DT_CENTER);
	BitBlt(g_hdc, 0, 0, X_SIZE, Y_SIZE, g_hdcBuff, 0, 0, SRCCOPY);
	return;
}

BOOL CheckCollison(int xoffset,int yoffset)
{
	for (int x = 0;x < 4;++x)
	{
		for (int y = 0;y < 4;++y)
		{
			if (g_astCur[x][y]) 
			{
				if (xoffset < 0)
					return FALSE;
				if (yoffset + y >=GAME_Y)
					return FALSE;
				if (g_astMap[xoffset + x][yoffset+ y])
					return FALSE;
				if (xoffset + x >= GAME_X)
					return FALSE;
			}
				
		}
	}
	return TRUE;
}
VOID RefreshMap() 
{
	int x, y;
	int r_full = 0;
	int flag = 0;
	for (x = 0;x < 4;++x) 
	{
		for (y = 0;y < 4;++y) 
		{

			if (g_astCur[x][y]) 
			{
				g_astMap[g_x + x][g_y+ y] = g_astCur[x][y];
			}
		}
	}
	
	
	for (y = GAME_Y-1;y >=0;--y) {
		
		for (x=0;x <GAME_X;++x)
		{				
			if (g_astMap[x][y] == 0)
				flag = 1;
		}
		if (flag!=1)
		{
			++r_full;
			for (int i = y - 1;i >= 0;--i) {
				for (int j = 0;j < GAME_X;++j)
					g_astMap[j][i+1] = g_astMap[j][i];
			}
			y = GAME_Y;
			
		}
		flag = 0;
			
	}
	g_score += r_full * 10;

	
	return;
}
VOID Rotate()
{
	INT xpos, ypos;
	INT flag;
	INT astNew[4][4] = { 0 }, astMid[4][4] = { 0 };
	for (int x = 0,xpos=0;x < 4;++x)
	{
		flag = 0;
		for (int y = 0;y < 4;++y)
		{
			astMid[xpos][y] = g_astCur[3 - y][x];
			if (astMid[xpos][y]) flag = 1;
		}
		if (flag) xpos++;
	}
	for (int y = 0, ypos = 0;y <4;++y) {
		flag = 0;
		for (int x = 0;x < 4;++x) {
			astNew[x][ypos] = astMid[x][y];
			if (astNew[x][ypos]) flag = 1;
		}
		if (flag) ypos++;
	}
	memcpy(astMid, g_astCur, sizeof(astMid));
	memcpy(g_astCur, astNew, sizeof(g_astCur));
	if (!CheckCollison(g_x, g_y)) {
		memcpy(g_astCur, astMid, sizeof(g_astCur));
	}
}
BOOL GameOver()
{
	
	for (int y = 0;y < GAME_Y;++y)
	{
		if (g_astMap[y][0] == 1)
		{
			
			return TRUE;
		}
	}
	
	return FALSE;
}