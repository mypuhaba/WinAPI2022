#include <windows.h>
#include <atlimage.h>
#include "resource.h"

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"5-1";
HINSTANCE g_hinst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void TextPrint(HDC hdc, int x, int y, TCHAR text[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ipszCmdPram, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX wndclass; // 구조체 정의
	g_hinst = hInstance;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW; // 윈도우 출력 스타일
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 아이콘 유형
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // 커서 유형
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 배경
	wndclass.lpszMenuName = NULL; // 메뉴 이름
	wndclass.lpszClassName = lpszClass; // 클래스 이름
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //작은 아이콘
	RegisterClassExW(&wndclass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		0, 0, 900, 800, NULL, (HMENU)NULL, hInstance, NULL); //윈도우 만들기 함수

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) { // 메시지 큐로부터 메시지를 얻어온다
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;
	static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
	static int yPos;
	static RECT rectView;
	TCHAR word[] = L"ARIA the Animation";
	//static DWORD dwRop;

	switch (iMsg)
	{
	case WM_CREATE:
		yPos = -30;
		GetClientRect(hwnd, &rectView);
		SetTimer(hwnd, 1, 70, NULL);
		hBit2 = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP5_2));
		break;
	case WM_PAINT:
		GetClientRect(hwnd, &rectView);
		hdc = BeginPaint(hwnd, &ps);
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);
		if (hBit1 == NULL)
			hBit1 = CreateCompatibleBitmap(hdc, 530, 530);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);
		BitBlt(mem1dc, 0, 0, 530, 530, mem2dc, 0, 0, SRCCOPY);
		SetBkMode(mem1dc, TRANSPARENT);
		TextPrint(mem1dc, 200, yPos, word);
		BitBlt(hdc, 0, 0, 530, 530, mem1dc, 0, 0, SRCCOPY);
		SelectObject(mem1dc, oldBit1);
		SelectObject(mem2dc, oldBit2);
		DeleteDC(mem2dc);
		DeleteDC(mem1dc);
		EndPaint(hwnd, &ps);
		return 0;
		break;
	case WM_TIMER:
		yPos += 5;
		if (yPos > rectView.bottom) yPos = -30;
		InvalidateRgn(hwnd, NULL, false);
		return 0;
		break;
	case WM_CHAR:
		break;
	case WM_DESTROY:
		if (hBit1) DeleteObject(hBit1);
		DeleteObject(hBit2);
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void TextPrint(HDC hdc, int x, int y, TCHAR text[])
	{
		int i, j;
		SetTextColor(hdc, RGB(255, 255, 255));
		for (i = -1; i <= 1; ++i)
			for (j = -1; j <= 1; ++j)
				TextOut(hdc, x + i, y + j, text, lstrlen(text));
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, x, y, text, lstrlen(text));
	}

	//HDC hdc, memdc;
	//static HBITMAP hBitmap;
	//PAINTSTRUCT ps;
	//static CImage img, imgSprite;
	//static int xPos = 0, yPos = 0;
	//static RECT rect;
	//static DWORD op = SRCCOPY;

	//switch (iMsg)
	//{
	//case WM_CREATE:
	//	img.Load(L"image.bmp"); //--- background
	//	//imgSprite.Load(L”bitmap2.bmp”); //--- sprite image
	//	GetClientRect(hwnd, &rect);
	//	break;
	//case WM_PAINT:
	//	hdc = BeginPaint(hwnd, &ps);
	//	hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	//	memdc = CreateCompatibleDC(hdc);
	//	(HBITMAP)SelectObject(memdc, hBitmap);
	//	img.Draw(memdc, 0, 0, rect.right, rect.bottom, 0, 0, img.GetWidth(), img.GetHeight()); //--- 메모리 DC에 배경 그리기
	//		//imgSprite.Draw(memdc, xPos, yPos, 100, 100, 0, 0, 100, 100); //--- 메모리 DC에 스프라이트 그리기
	//	BitBlt(hdc, 0, 0, rect.right, rect.bottom, memdc, 0, 0, op); //--- 메모리 DC의 그림을 화면 DC에 복사하기
	//	DeleteObject(hBitmap);
	//	DeleteDC(memdc);
	//	EndPaint(hwnd, &ps);
	//	break;

	//case WM_TIMER:
	//	xPos += 5;
	//	yPos += 5;
	//	InvalidateRect(hwnd, NULL, false);
	//	break;

	//case WM_MOUSEMOVE:
	//	break;

	//case WM_LBUTTONUP:

	//	break;

	//case WM_LBUTTONDOWN:

	//	break;

	//case WM_CHAR:
	//	switch (wParam)
	//	{
	//	case 'r':
	//		static int dir = 1;
	//		if (dir == 1)
	//		{
	//			op = NOTSRCCOPY;
	//			InvalidateRgn(hwnd, NULL, true);
	//		}
	//		else dir = 1;

	//		break;
	//	}
	//	break;


	//case WM_DESTROY:
	//	PostQuitMessage(0);
	//	break;
	//}
	//return DefWindowProc(hwnd, iMsg, wParam, lParam);
//}