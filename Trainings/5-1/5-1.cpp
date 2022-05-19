#include <windows.h>
#include <atlimage.h>

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
	PAINTSTRUCT ps;
	static int xPos,yPos, nW, nH, w, h, zw, zh; // xPos/yPos: DC에 그릴 위치
	//nW/nH: 대상 사각형, w/h: 소스 사각형
	static CImage img;
	HDC hdc, memdc;
	static RECT rect;
	static HBITMAP hBitmap;
	static DWORD dwRop;

	switch (iMsg)
	{
	case WM_CREATE:
		img.Load(TEXT("image1.jpg"));
		GetClientRect(hwnd, &rect);
		xPos = 0; yPos = 0;
		w = img.GetWidth();
		h = img.GetHeight();
		nW = w; nH = h;
		zw = 0; zh = 0;
		dwRop = SRCCOPY;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBitmap);
		img.Draw(memdc, 0, 0, nW, nH, 0, 0, w, h);
		//StretchBlt(hdc, 0, 0, nW + zw, nH + zh, memdc, 0, 0, w, h, SRCCOPY);
		BitBlt(hdc, xPos, yPos, nW, nH, memdc, 0, 0, dwRop);
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'r':
			if (dwRop == SRCCOPY)
				dwRop = NOTSRCCOPY;
			else dwRop = SRCCOPY;
			InvalidateRgn(hwnd, NULL, true);
			break;
		case 'a':
			if (nW == rect.right)
				nW = w;
			if (nH == rect.bottom)
				nH = h;
			else
			{
				nW = rect.right;
				nH = rect.bottom;
			}
			InvalidateRgn(hwnd, NULL, true);
			break;
		case '2':
			hdc = GetDC(hwnd);
			hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			memdc = CreateCompatibleDC(hdc);
			(HBITMAP)SelectObject(memdc, hBitmap);
			img.Draw(memdc, 0, 0, nW / 2, nH / 2, 0, 0, w, h);
			BitBlt(hdc, xPos, yPos, nW, nH, memdc, 0, 0, SRCCOPY);
			BitBlt(hdc, xPos + nW / 2, yPos, nW / 2, nH / 2, memdc, 0, 0, NOTSRCCOPY);
			BitBlt(hdc, xPos, yPos + nH / 2, nW / 2, nH / 2, memdc, 0, 0, NOTSRCCOPY);
			BitBlt(hdc, xPos + nW / 2, yPos + nH / 2, nW / 2, nH / 2, memdc, 0, 0, SRCCOPY);
			DeleteObject(hBitmap);
			DeleteDC(memdc);
			ReleaseDC(hwnd, hdc);
			break;
		case '3':
			hdc = GetDC(hwnd);
			hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			memdc = CreateCompatibleDC(hdc);
			(HBITMAP)SelectObject(memdc, hBitmap);
			img.Draw(memdc, 0, 0, nW / 3, nH / 3, 0, 0, w, h);
			BitBlt(hdc, xPos, yPos, nW / 3, nH / 3, memdc, 0, 0, NOTSRCCOPY); // 1
			BitBlt(hdc, xPos + nW / 3, yPos, nW / 3, nH / 3, memdc, 0, 0, SRCCOPY); // 2
			BitBlt(hdc, xPos + nW * 2 / 3, yPos, nW / 3, nH / 3, memdc, 0, 0, NOTSRCCOPY);  // 3
			BitBlt(hdc, xPos, yPos + nH/3, nW / 3, nH / 3, memdc, 0, 0, SRCCOPY); // 4
			BitBlt(hdc, xPos + nW / 3, yPos + nH / 3, nW / 3, nH / 3, memdc, 0, 0, NOTSRCCOPY); // 5
			BitBlt(hdc, xPos + nW * 2 / 3, yPos + nH / 3, nW / 3, nH / 3, memdc, 0, 0, SRCCOPY); // 6
			BitBlt(hdc, xPos, yPos + 2 * nH / 3, nW / 3, nH / 3, memdc, 0, 0, NOTSRCCOPY); // 7
			BitBlt(hdc, xPos + nW / 3, yPos + nH * 2 / 3, nW / 3, nH / 3, memdc, 0, 0, SRCCOPY); // 8
			BitBlt(hdc, xPos + nW * 2 / 3, yPos + nH * 2 / 3, nW / 3, nH / 3, memdc, 0, 0, NOTSRCCOPY); // 9
			DeleteObject(hBitmap);
			DeleteDC(memdc);
			ReleaseDC(hwnd, hdc);
			break;
		case 'q':
			img.Destroy();
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			xPos -= 10;
			InvalidateRgn(hwnd, NULL, true);
			break;

		case VK_RIGHT:
			xPos += 10;
			InvalidateRgn(hwnd, NULL, true);
			break;
		case VK_ADD:
			hdc = GetDC(hwnd);
			hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			memdc = CreateCompatibleDC(hdc);
			(HBITMAP)SelectObject(memdc, hBitmap);
			img.Draw(memdc, 0, 0, nW, nH, 0, 0, w, h);
			StretchBlt(hdc, 0, 0, nW + zw, nH + zh, memdc, 0, 0, w, h, WHITENESS);
			zw += 20; zh += 20;
			StretchBlt(hdc, 0, 0, nW + zw, nH + zh, memdc, 0, 0, w, h, SRCCOPY);
			DeleteObject(hBitmap);
			DeleteDC(memdc);
			ReleaseDC(hwnd, hdc);
			break;
		case VK_SUBTRACT:
			
			hdc = GetDC(hwnd);
			hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			memdc = CreateCompatibleDC(hdc);
			(HBITMAP)SelectObject(memdc, hBitmap);
			img.Draw(memdc, 0, 0, nW, nH, 0, 0, w, h);
			StretchBlt(hdc, 0, 0, nW + zw, nH + zh, memdc, 0, 0, w, h, WHITENESS);
			zw -= 20; zh -= 20;
			StretchBlt(hdc, 0, 0, nW + zw, nH + zh, memdc, 0, 0, w, h, SRCCOPY);
			DeleteObject(hBitmap);
			DeleteDC(memdc);
			ReleaseDC(hwnd, hdc);
			break;
		}
		break;

	case WM_DESTROY:
		img.Destroy();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}