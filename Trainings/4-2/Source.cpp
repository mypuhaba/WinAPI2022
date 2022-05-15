#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 25 // 보드 가로 세로
#define WIDTH 30 //

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Practice";
HINSTANCE g_hinst;

void PaintBoard(const HDC& hdc);
BOOL IsInBoard(const int& mx, const int& my);
void PaintRect(const HDC& hdc, const int& mx, const int& my);

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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // 배경
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
	HDC hdc;
	PAINTSTRUCT ps;
	static BOOL isDrag = FALSE;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		PaintBoard(hdc);

		EndPaint(hwnd, &ps);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (isDrag)
			PaintRect(hdc, LOWORD(lParam), HIWORD(lParam));
		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONUP:
		isDrag = FALSE;
		break;

	case WM_LBUTTONDOWN:
		isDrag = TRUE;
		break;

	case WM_KEYDOWN:
		break;

	case WM_TIMER:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void PaintBoard(const HDC& hdc)
{
	HPEN pen;
	int startX = 0; // 보드 선긋기 시작점
	int startY = 0;
	int endX = SIZE * WIDTH; // 보드 선긋기 끝점
	int endY = 0;

	//40x40 보드 가로세로선 나타내기(하늘색)
	pen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(80, 188, 223)));
	// 보드 가로선
	startX = 0;
	startY = 0;
	endX = WIDTH * SIZE;
	endY = 0;
	for (int i = 0; i <= SIZE; ++i)
	{
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, endX, endY);
		startY += WIDTH; endY += WIDTH;
	}
	// 보드 세로선
	startX = 0;
	startY = 0;
	endX = 0;
	endY = WIDTH * SIZE;
	for (int i = 0; i <= SIZE; ++i)
	{
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, endX, endY);
		startX += WIDTH; endX += WIDTH;
	}
}

BOOL IsInBoard(const int& mx, const int& my)
{
	if (mx > 0 && mx < SIZE * WIDTH && my > 0 && my < SIZE * WIDTH)
		return TRUE;
	else return FALSE;
}

void PaintRect(const HDC& hdc, const int& mx, const int& my)
{
	if (IsInBoard(mx, my))
	{
		static int x, y;
		HBRUSH hBrush;
		x = mx - mx % WIDTH;
		y = my - my % WIDTH;
		hBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
		Rectangle(hdc, x, y, x + WIDTH, y + WIDTH);
		DeleteObject(hBrush);
	}

}


