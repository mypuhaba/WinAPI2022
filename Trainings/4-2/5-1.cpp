#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <atlimage.h>
#define SIZE 25 // 보드 가로 세로
#define WIDTH 30 //

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Practice";
HINSTANCE g_hinst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

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
	HDC hdc, memdc;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;
	static CImage img, imgSprite;
	static int xPos = 0, yPos = 0;
	static RECT rect;
	static DWORD op = SRCCOPY;

	switch (iMsg)
	{
	case WM_CREATE:
		img.Load(L"image.bmp"); //--- background
		//imgSprite.Load(L”bitmap2.bmp”); //--- sprite image
		GetClientRect(hwnd, &rect);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBitmap);
		img.Draw(memdc, 0, 0, rect.right, rect.bottom, 0, 0, img.GetWidth(), img.GetHeight()); //--- 메모리 DC에 배경 그리기
			//imgSprite.Draw(memdc, xPos, yPos, 100, 100, 0, 0, 100, 100); //--- 메모리 DC에 스프라이트 그리기
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, memdc, 0, 0, op); //--- 메모리 DC의 그림을 화면 DC에 복사하기
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		xPos += 5;
		yPos += 5;
		InvalidateRect(hwnd, NULL, false);
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONUP:
		
		break;

	case WM_LBUTTONDOWN:
	
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'r':
			static int dir = 1;
			if (dir == 1)
			{
				op = NOTSRCCOPY;
				InvalidateRgn(hwnd, NULL, true);
			}
			else dir = 1;
			
			break;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
