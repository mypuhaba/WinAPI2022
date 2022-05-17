#include <windows.h>
#include <atlimage.h>

#define WindowWidth 750
#define WindowHeight 750
#define nWidth 25
#define nHeight 25
#define NUMBER 30
#define BULLET_NUMBER 40

typedef struct {
	int xPosTank, yPosTank;
	int dir;
	int moving;
	POINT gunLeft, gunRight;
	int shape;
} Tank;

typedef struct {
	int xPosBullet, yPosBullet;
	int dir;
	bool isOn;
} Bullet;

HINSTANCE g_hInst;
TCHAR lpszClass[]= TEXT("Top view shotting with obstacle");

int iBoard[NUMBER][NUMBER];		//-- 0: 빈 칸, 1: 장애물, 2: 아이템
bool makeRoad = false;
bool makeRoadDone = false;
int bulletCount = 0;
bool bulletFired = false;

Tank tankObject;
Bullet bulletList[BULLET_NUMBER];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void DrawBoard(HDC hdc);
void DrawBlock(HDC hdc);
void DrawTank(HDC hdc, int xP, int yP, int d);
void DrawBullet(HDC hdc, int xP, int yP, int d);

int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)		  
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	if(!hPrevInstance) {
		WndClass.cbClsExtra=0;
		WndClass.cbWndExtra=0;
		WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);		//--- 윈도우 배경색 변경 가능
		WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		WndClass.hInstance=hInstance;
		WndClass.lpfnWndProc=(WNDPROC)WndProc;
		WndClass.lpszClassName=lpszClass;
		WndClass.lpszMenuName=NULL;
		WndClass.style=CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);
	}
	hWnd = CreateWindow (lpszClass,lpszClass,WS_OVERLAPPEDWINDOW, 0, 0,800, 800,NULL,(HMENU)NULL,hInstance,NULL);		//--- 윈도우 크기 변경 가능
	ShowWindow(hWnd,nCmdShow);
	
	while(GetMessage(&Message,0,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oBrush;	
	int i, j, mx, my, tx, ty;
	RECT rect;
	POINT pt;

	static CImage img;
	static int iWidth, iHeight, drawBoard;
	static HBITMAP hBitmap;

	switch(iMessage) {
		case WM_CREATE:
			for (i = 0; i < NUMBER; i++)
				for (j = 0; j < NUMBER; j++)
					iBoard[i][j] = 0;

			tankObject.xPosTank = 1;
			tankObject.yPosTank = 1;
			tankObject.dir = 2;					//--- x+: 2,   x-: 0,   y+: 3,   y-: 1

			img.Load(TEXT("background.png"));
			iWidth = img.GetWidth();
			iHeight = img.GetHeight();
			drawBoard = 1;
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			hBitmap = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);
			memdc = CreateCompatibleDC(hdc);
			(HBITMAP) SelectObject(memdc, hBitmap);

			img.Draw(memdc, 0, 0, WindowWidth, WindowHeight, 0, 0, iWidth, iHeight);

			if ( drawBoard )
				DrawBoard(memdc);

			DrawBlock(memdc);

			DrawTank(memdc, tankObject.xPosTank, tankObject.yPosTank, tankObject.dir);

			if (bulletFired) {
				for ( i = 0; i < bulletCount; i++ )
					if ( bulletList[i].isOn == true)
						DrawBullet (memdc, bulletList[i].xPosBullet, bulletList[i].yPosBullet, bulletList[i].dir);
			}

			BitBlt(hdc, 0, 0, WindowWidth, WindowHeight, memdc, 0, 0, SRCCOPY);

			DeleteObject(hBitmap);
			DeleteDC(memdc);

			EndPaint(hWnd, &ps);
			break;

		case WM_LBUTTONDOWN:
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			makeRoad = true;
			break;

		case WM_MOUSEMOVE:
			if (makeRoad) {
				hdc = GetDC(hWnd);
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				i = mx / nWidth;
				j = my / nHeight;
				rect.left = i * nWidth; rect.right = rect.left + nWidth;
				rect.top = j * nHeight; rect.bottom = rect.top + nHeight;
				pt.x = mx; pt.y = my;
				//if (PtInRect(&rect, pt) && iBoard[i][j] == 0) {
				if (iBoard[i][j] == 0) {
					iBoard[i][j] = 1;					

					hBrush = CreateSolidBrush(RGB(200, 200, 200));
					oBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, i * nWidth, j * nHeight, i * nWidth + nWidth, j * nHeight + nHeight);
					SelectObject(hdc, oBrush);
					DeleteObject(hBrush);
				}
				ReleaseDC(hWnd, hdc);
			}
			break;

		case WM_LBUTTONUP:
			makeRoad = false;
			makeRoadDone = true;
			break;

		case WM_CHAR:
			switch (wParam)
			{
			case 'w':			// y-
				tankObject.dir = 1;
				if (tankObject.yPosTank > 1 && iBoard[tankObject.xPosTank][tankObject.yPosTank - 2] == 0)
					tankObject.yPosTank--;
				break;

			case 's':			// y+
				tankObject.dir = 3;
				if (tankObject.yPosTank < NUMBER - 2 && iBoard[tankObject.xPosTank][tankObject.yPosTank + 2] == 0)
					tankObject.yPosTank++;
				break;

			case 'a':			// x-a
				tankObject.dir = 0;
				if (tankObject.xPosTank > 1 && iBoard[tankObject.xPosTank - 2][tankObject.yPosTank] == 0)
					tankObject.xPosTank--;
				break;

			case 'd':			// x+
				tankObject.dir = 2;
				if (tankObject.xPosTank < NUMBER - 2 && iBoard[tankObject.xPosTank + 2][tankObject.yPosTank] == 0)
					tankObject.xPosTank++;
				break;

			case 'p':
				drawBoard++;
				drawBoard %= 2;
				break;
			case 'q':
				PostQuitMessage(0);
				break;
			}
			InvalidateRect(hWnd, NULL, false);
			break;

		case WM_KEYDOWN:
			switch (wParam) {
				case VK_RETURN:					//--- x+: 2,   x-: 0,   y+: 3,   y-: 1
					bulletFired = true;
					bulletList[bulletCount].isOn = 1;
					bulletList[bulletCount].dir = tankObject.dir;
					if (tankObject.dir == 0) {					//--- x-
						bulletList[bulletCount].xPosBullet = tankObject.gunLeft.x - 5;
						bulletList[bulletCount].yPosBullet = tankObject.gunLeft.y;
						bulletCount++;
					}
					else if (tankObject.dir == 2) {			//--- x+
						bulletList[bulletCount].xPosBullet = tankObject.gunLeft.x + 5;
						bulletList[bulletCount].yPosBullet = tankObject.gunLeft.y;
						bulletCount++;
					}
					else if (tankObject.dir == 1) {			//--- y-
						bulletList[bulletCount].xPosBullet = tankObject.gunLeft.x;
						bulletList[bulletCount].yPosBullet = tankObject.gunLeft.y - 5;
						bulletCount++;
					}
					else if (tankObject.dir == 3) {			//--- y+
						bulletList[bulletCount].xPosBullet = tankObject.gunLeft.x;
						bulletList[bulletCount].yPosBullet = tankObject.gunLeft.y + 5;
						bulletCount++;
					}
					bulletCount %= BULLET_NUMBER;
					SetTimer(hWnd, 100, 100, NULL);
					break;
				}
			break;

		case WM_TIMER:
				hdc = GetDC(hWnd);
				switch (wParam) {
				case 100:
					for (i = 0; i < bulletCount; i++) {
						tx = bulletList[i].xPosBullet / nWidth;
						ty = bulletList[i].yPosBullet / nHeight;

						if (iBoard[tx][ty] == 1)							//--- Block
							bulletList[i].isOn = false;

						if (bulletList[i].dir == 0) {						//--- 0: x-,   2: x+,   1: y-,  3: y+
							if (bulletList[i].xPosBullet > 0)
								bulletList[i].xPosBullet -= 10;
							else
								bulletList[i].isOn = false;
						}
						else if (bulletList[i].dir == 1) {
							if (bulletList[i].yPosBullet > 0)
								bulletList[i].yPosBullet -= 10;
							else
								bulletList[i].isOn = false;
						}
						else if (bulletList[i].dir == 2) {
							if (bulletList[i].xPosBullet < WindowWidth)
								bulletList[i].xPosBullet += 10;
							else
								bulletList[i].isOn = false;
						}
						else if (bulletList[i].dir == 3) {
							if (bulletList[i].yPosBullet < WindowHeight)
								bulletList[i].yPosBullet += 10;
							else
								bulletList[i].isOn = false;
						}
					}
				}
				ReleaseDC(hWnd, hdc);
				InvalidateRect(hWnd, NULL, false);
				break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void DrawBoard(HDC hdc)
{
	int i;

	for (i = 0; i <= NUMBER; i++) {
		MoveToEx(hdc, i * nWidth, 0, NULL);
		LineTo(hdc, i * nWidth, WindowHeight);

		MoveToEx(hdc, 0, i * nHeight, NULL);
		LineTo(hdc, WindowWidth, i * nHeight);
	}
}

void DrawBlock (HDC hdc)
{
	HBRUSH oldBrush, newBrush;
	int i, j;

	newBrush = CreateSolidBrush(RGB(200, 200, 200));
	oldBrush = (HBRUSH)SelectObject(hdc, newBrush);
	for (i = 0; i < NUMBER; i++)
		for (j = 0; j < NUMBER; j++) {
			if (iBoard[i][j] == 1) {
				Rectangle(hdc, i * nWidth, j * nHeight, i * nWidth + nWidth, j * nHeight + nHeight);
			}
		}
	SelectObject(hdc, oldBrush);
	DeleteObject(newBrush);
}

void DrawTank (HDC hdc, int xP, int yP, int d)
{
		HBRUSH hBrush, oBrush;

		hBrush = CreateSolidBrush(RGB(255, 0, 255));
		oBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, xP * nWidth + 2, yP * nHeight + 2, (xP + 1) * nWidth - 2, (yP + 1) * nHeight - 2);

		switch (d) {
		case 0:		// direction: left
				Rectangle(hdc, (xP - 1) * nWidth + 10, yP * nHeight + 15, xP * nWidth, (yP + 1) * nHeight - 15);
				tankObject.gunLeft.x = (xP - 1) * nWidth + 10;		tankObject.gunLeft.y = yP * nHeight + 15;
				tankObject.gunRight.x = xP * nWidth;			tankObject.gunRight.y = (yP + 1) * nHeight - 15;
				break;
		case 1:		// direction: top
				Rectangle(hdc, xP * nWidth + 15, (yP - 1) * nHeight + 10, (xP + 1) * nWidth - 15, yP * nHeight);
				tankObject.gunLeft.x = xP * nWidth + 15;		tankObject.gunLeft.y = (yP - 1) * nHeight + 10;
				tankObject.gunRight.x = (xP + 1) * nWidth - 15;			tankObject.gunRight.y = yP * nHeight;
				break;
		case 2:		// direction: right
				Rectangle(hdc, (xP + 1) * nWidth, yP * nHeight + 15, (xP + 2) * nWidth - 10, (yP + 1) * nHeight - 15);
				tankObject.gunLeft.x = (xP + 1) * nWidth;		tankObject.gunLeft.y = yP * nHeight + 15;
				tankObject.gunRight.x = (xP + 2) * nWidth - 10;			tankObject.gunRight.y = (yP + 1) * nHeight - 15;
				break;
		case 3:		// direction: bottom
				Rectangle(hdc, xP * nWidth + 15, (yP + 1) * nHeight, (xP + 1) * nWidth - 15, (yP + 2) * nHeight - 10);
				tankObject.gunLeft.x = xP * nWidth + 15;		tankObject.gunLeft.y = (yP + 1) * nHeight;
				tankObject.gunRight.x = (xP + 1) * nWidth - 15;			tankObject.gunRight.y = (yP + 2) * nHeight - 10;
				break;
			}
		SelectObject(hdc, oBrush);
		DeleteObject(hBrush);
}

void DrawBullet(HDC hdc, int xP, int yP, int d)
{
	HBRUSH hBrush, oldBrush;

	hBrush = CreateSolidBrush(RGB(0, 255, 255));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	switch (d) {				//--- x+: 2,   x-: 0,   y+: 3,   y-: 1
		case 0:
			Rectangle(hdc, xP, yP-7, xP - 10, yP + 3);
			break;
		case 1:
			Rectangle(hdc, xP-7, yP, xP + 3, yP - 10);
			break;
		case 2:
			Rectangle(hdc, xP, yP-7, xP + 10, yP + 3);
			break;
		case 3:
			Rectangle(hdc, xP-7, yP, xP + 3, yP + 10);
			break;
	}
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}